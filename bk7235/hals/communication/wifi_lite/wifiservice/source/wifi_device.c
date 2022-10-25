// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "stdbool.h"
#include "wifi_device_util.h"
#include "wifi_device.h"
#include <securec.h>
#include "wifi_hotspot_config.h"
#include "utils_file.h"

#include <common/bk_err.h>
#include <modules/wifi.h>
#include <components/event.h>
#include <components/netif.h>

#define MAX_STORE_FILE_LEN 8

typedef struct bk_msg
{
	void *data;
	int event_id;
	int event_module;
}BKMSG;

static int g_wifiStaStatus = WIFI_STA_NOT_ACTIVE;
static wifi_scan_result_t scan_apList = {0,0};
static WifiDeviceConfig g_wifiConfigs[WIFI_MAX_CONFIG_SIZE] = { 
	[0 ... (WIFI_MAX_CONFIG_SIZE-1)] = {{0}, {0}, {0}, 0, WIFI_CONFIG_INVALID, 0, 0}
};
static WifiEvent * g_wifiEvents[WIFI_MAX_EVENT_SIZE] = {0};
static const char wifi_store_filename[MAX_STORE_FILE_LEN] = "ApInfo";



#if 0
static WifiEvent g_wifitest ={0};
static void testOnWifiScanStateChangedHandler(int state, int size)
{
	bk_printf("====%s,state:%d,size:%d===\r\n",__FUNCTION__,state,size);
}
static void testOnWifiConnectionChangedHandler(int state, WifiLinkedInfo* info)
{
	bk_printf("====%s,state:%d===\r\n",__FUNCTION__,state);
}

#endif


#define MAC_LEN 6

struct sta_list {
	struct sta_list *next;
	unsigned char value[MAC_LEN];
};

struct sta_list *sta_info = NULL;

static int same_mac(unsigned char* mac1, unsigned char* mac2) {
	int i;
	for (i = 0; i < MAC_LEN; i++)
		if (mac1[i] != mac2[i])
			return 0;
	return 1;
}

static void add_sta(unsigned char* value) {
	struct sta_list *new = malloc(sizeof(struct sta_list));
	if (!new)
		return;

	memcpy(new->value, value, MAC_LEN);
	new->next = sta_info;
	sta_info = new;
}

static void rm_sta(unsigned char* value) {
	struct sta_list *tmp;
	struct sta_list *prev;
	
	if (!sta_info) {
		printf("no entry for %d found\n", value[0]);
		return;
	}
	
	if (same_mac(sta_info->value, value)) {
		tmp = sta_info->next;
		free(sta_info);
		sta_info = tmp;
		return;
	}
	
	prev = sta_info;
	tmp = sta_info->next;
	while (tmp && !same_mac(tmp->value, value)) {
		prev = tmp;
		tmp = tmp->next;
	}
	
	if (!tmp) {
		printf("no entry for %d found\n", value[0]);
		return;
	}
	
	prev->next = tmp->next;
	free(tmp);
}

WifiErrorCode GetApInfo(StationInfo* result, unsigned int* size) {
	int cnt = 0;
	struct sta_list *tmp;

	tmp = sta_info;
	while(tmp) {
		memcpy(result[cnt].macAddress, tmp->value, MAC_LEN);
		result[cnt].ipAddress = 0x0100007f;	//fake
		cnt++;
		if (cnt >= *size)
			break;
		tmp = tmp->next;
	}
	*size = cnt;

	return WIFI_SUCCESS;
}
	


static void DispatchScanStateChangeEvent(const void *evt, const WifiEvent* hosEvent)
{
	int size = 0;
	BKMSG *msg = (BKMSG *)evt;
	if(hosEvent->OnWifiScanStateChanged == NULL)
		return;

	if(msg == NULL)
	{
		hosEvent->OnWifiScanStateChanged(WIFI_STATE_NOT_AVAILABLE, 0);
		return;
	}
	if(scan_apList.aps != NULL)
	{
		bk_wifi_scan_free_result(&scan_apList);
	}

	if (bk_wifi_scan_get_result(&scan_apList) == 0) 
	{
		size = scan_apList.ap_num;
	}
	
	hosEvent->OnWifiScanStateChanged(WIFI_STATE_AVAILABLE, size);
	return;
}
static void DispatchStaConnectEvent(const void *evt, const WifiEvent* hosEvent)
{
	WifiLinkedInfo info = {0};
	BKMSG *msg = (BKMSG *)evt;
	
	if(hosEvent->OnWifiConnectionChanged == NULL)
		return;
	
	if(msg->event_id == EVENT_WIFI_STA_CONNECTED)
	{
		GetLinkedInfo(&info);
		hosEvent->OnWifiConnectionChanged(WIFI_STATE_AVAILABLE, &info);
		return;
	}
	else
	{
		wifi_event_sta_disconnected_t *sta_disconnected = (wifi_event_sta_disconnected_t *)msg->data;
		bk_printf("STA disconnected ,reason(%d)\r\n",sta_disconnected->disconnect_reason);
		hosEvent->OnWifiConnectionChanged(WIFI_STATE_NOT_AVAILABLE, NULL);
	}
	
}

static void DispatchAPConnectEvent(const void *evt, const WifiEvent* hosEvent)
{
	StationInfo info = {0};
	BKMSG *msg = (BKMSG *)evt;
	wifi_event_ap_disconnected_t *ap_disconnected;
	wifi_event_ap_connected_t *ap_connected;
	bk_printf("======dispatch event %d :======\r\n",msg->event_id);
	if(msg->event_id == EVENT_WIFI_AP_CONNECTED)
	{
		if(hosEvent->OnHotspotStaJoin == NULL)
			return;

		ap_connected = (wifi_event_ap_connected_t *)msg->data;
		add_sta(ap_connected->mac);
		memcpy(info.macAddress, ap_connected->mac, WIFI_MAC_LEN);
		 hosEvent->OnHotspotStaJoin(&info);
		return;
	}
	else
	{
		if(hosEvent->OnHotspotStaLeave == NULL)
			return;
		ap_disconnected = (wifi_event_ap_disconnected_t *)msg->data;
		rm_sta(ap_disconnected->mac);
		memcpy(info.macAddress, ap_disconnected->mac, WIFI_MAC_LEN);
		hosEvent->OnHotspotStaLeave(&info);
	}
}
static void DispatchEvent(void* BkEvent, const WifiEvent* hosEvent)
{
	BKMSG *msg = (BKMSG *)BkEvent;
	
	switch(msg->event_id)
	{
		case EVENT_WIFI_STA_CONNECTED:
		case EVENT_WIFI_STA_DISCONNECTED:
			 DispatchStaConnectEvent(BkEvent, hosEvent);
			break;

		case EVENT_WIFI_AP_CONNECTED:
		case EVENT_WIFI_AP_DISCONNECTED:
			 DispatchAPConnectEvent(BkEvent, hosEvent);
			break;
		case EVENT_WIFI_SCAN_DONE:
			DispatchScanStateChangeEvent(BkEvent, hosEvent);
			break;
			
		default:
			
			break;
	}
	
}


static void BkWifiEventCb(void *arg, event_module_t event_module, int event_id, void *event_data)
{
	BKMSG msg;
	msg.data = event_data;
	msg.event_id = event_id;

	for(int i=0; i<WIFI_MAX_EVENT_SIZE; i++)
	{
		if(g_wifiEvents[i] == NULL)
			continue;
		DispatchEvent(&msg, g_wifiEvents[i]);
	}
}

//for ap start callback
void DispatchApStartEvent(int state)
{
	for(int i=0; i<WIFI_MAX_EVENT_SIZE; i++)
	{
		if(g_wifiEvents[i] == NULL)
			continue;
		if(g_wifiEvents[i]->OnHotspotStateChanged == NULL)
			continue;
		g_wifiEvents[i]->OnHotspotStateChanged(state);
	}
}


WifiErrorCode EnableWifi(void)
{
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (g_wifiStaStatus == WIFI_STA_ACTIVE) {
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_BUSY;
    }

#if 0
	wifi_init_config_t wifi_config = WIFI_DEFAULT_INIT_CONFIG();
	bk_event_init();
	bk_wifi_init(&wifi_config);
#endif
	bk_printf("==%s 22===\r\n",__FUNCTION__);
    g_wifiStaStatus = WIFI_STA_ACTIVE;
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }

    return WIFI_SUCCESS;
}
WifiErrorCode DisableWifi(void)
{
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }

    if (g_wifiStaStatus == WIFI_STA_NOT_ACTIVE) {
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_NOT_STARTED;
    }
#if 0
	bk_event_deinit();
	bk_wifi_deinit();
#endif
    g_wifiStaStatus = WIFI_STA_NOT_ACTIVE;
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }

    return WIFI_SUCCESS;
}
int IsWifiActive(void)
{
	return g_wifiStaStatus;
}

WifiErrorCode Scan(void)
{
    if (LockWifiEventLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    for (int i = 0; i < WIFI_MAX_EVENT_SIZE; i++) {
        if (g_wifiEvents[i] == NULL) {
            continue;
        }
        DispatchScanStateChangeEvent(NULL, g_wifiEvents[i]);
    }
    if (UnlockWifiEventLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
	if(bk_wifi_scan_start(NULL) == 0)
		return WIFI_SUCCESS;
	else
		return ERROR_WIFI_UNKNOWN;
}

WifiErrorCode GetScanInfoList(WifiScanInfo* result, unsigned int* size)
{
	unsigned int i;
	if( (result == NULL) || (size == NULL) || (size == 0) )
		return ERROR_WIFI_INVALID_ARGS;

	printf("YY : ap_num=%d\n", scan_apList.ap_num);
	
	if(scan_apList.ap_num > *size)
		return ERROR_WIFI_INVALID_ARGS;

	for(i=0;i<scan_apList.ap_num;i++)
	{
		memcpy(result[i].ssid, scan_apList.aps[i].ssid, WIFI_MAX_SSID_LEN);
		memcpy(result[i].bssid,scan_apList.aps[i].bssid, WIFI_MAC_LEN);
		result[i].securityType = BKSecToHoSec(scan_apList.aps[i].security);
		result[i].rssi = scan_apList.aps[i].rssi;
		result[i].frequency = ChannelToFrequency(scan_apList.aps[i].channel);
		result[i].band = HOTSPOT_BAND_TYPE_2G;
	}
	*size = scan_apList.ap_num;
	//bk_wifi_scan_free_result(&scan_apList);

	return WIFI_SUCCESS;
}



WifiErrorCode AddDeviceConfig(const WifiDeviceConfig* config, int* result)
{
	int fd;
	unsigned int file_size,write_size;
	char file_name[MAX_STORE_FILE_LEN + 2];

 	if (config == NULL || result == NULL) 
	{
		return ERROR_WIFI_INVALID_ARGS;
	}
	int netId = WIFI_CONFIG_INVALID;
	//printf("AddDeviceonfig ssid=%s-%s\r\n",config->ssid,config->preSharedKey);
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	for (int i = 0; i < WIFI_MAX_CONFIG_SIZE; i++) 
	{
		if (g_wifiConfigs[i].netId != i) 
		{
			netId = i;
			break;
		}
	}
	if(netId == WIFI_CONFIG_INVALID)
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return ERROR_WIFI_BUSY;
	}
	memcpy(&g_wifiConfigs[netId],config,sizeof(WifiDeviceConfig));
	g_wifiConfigs[netId].netId = netId;

#if 1
	/*save*/
	snprintf(file_name, sizeof(file_name), "%s%02d",wifi_store_filename,netId);
	fd = UtilsFileOpen(file_name, O_WRONLY_FS | O_CREAT_FS |O_TRUNC_FS, 0);
	if(fd < 0)
		return ERROR_WIFI_UNKNOWN;
	file_size = sizeof(WifiDeviceConfig);
	write_size = UtilsFileWrite(fd, (const char*)&g_wifiConfigs[netId], file_size);
	UtilsFileClose(fd);
//bk_printf("====save:%s,%d,%d,%d==\r\n",file_name,netId,file_size,write_size);
	if(write_size != file_size)
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
	}
#endif
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	*result = netId;
	
	return WIFI_SUCCESS;
	
}

WifiErrorCode GetDeviceConfigs(WifiDeviceConfig* result, unsigned int* size)
{
	int fd;
	unsigned int retIndex, i;
	unsigned int file_size,read_size;
	char file_name[MAX_STORE_FILE_LEN + 2];
	
	if(result == NULL || size == NULL || *size == 0)
	{
		return ERROR_WIFI_INVALID_ARGS;
	}

	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	retIndex = 0;	

#if 1
	for(i=0; i<WIFI_MAX_CONFIG_SIZE; i++)
	{
		snprintf(file_name, sizeof(file_name), "%s%02d",wifi_store_filename,i);
		if(0 !=UtilsFileStat(file_name,&file_size))	
			break; 
		fd = UtilsFileOpen(file_name,O_RDONLY_FS, 0);
		if(fd < 0)
			goto error_ret;
		 read_size = UtilsFileRead(fd, (char* )&g_wifiConfigs[i], file_size);	
		if(read_size != file_size)
			goto error_ret;
		UtilsFileClose(fd);
	}
	if(i == 0)
		goto error_ret;
#else
	i = WIFI_MAX_CONFIG_SIZE;
#endif

	for (i = 0; i < WIFI_MAX_CONFIG_SIZE; i++) {
		if (g_wifiConfigs[i].netId != WIFI_CONFIG_INVALID) {
			memcpy(&result[retIndex], &g_wifiConfigs[i], sizeof(WifiDeviceConfig));
			retIndex++;
			if (retIndex >= *size)
				break;
		}
	}

	/*
	while( retIndex < i )
	{
		memcpy(&result[retIndex], &g_wifiConfigs[retIndex], sizeof(WifiDeviceConfig));
		retIndex++;
		if(*size < retIndex)
		{
			if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
			{
				return ERROR_WIFI_UNKNOWN;
			}
			return ERROR_WIFI_INVALID_ARGS;
		}
	}
	*/
error_ret:
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if(retIndex == 0)
		return ERROR_WIFI_NOT_AVAILABLE;



//for test

	if(g_wifiConfigs[0].ssid == NULL)
		return ERROR_WIFI_NOT_AVAILABLE;




	
	*size = retIndex;
	return WIFI_SUCCESS;

}

WifiErrorCode RemoveDevice(int networkId)
{
	char file_name[MAX_STORE_FILE_LEN + 2];
	//int i,file_size;
	//int fd;
	if (networkId >= WIFI_MAX_CONFIG_SIZE || networkId < 0) 
	{
		return ERROR_WIFI_INVALID_ARGS;
	}
	
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	memset(&g_wifiConfigs[networkId], 0, sizeof(WifiDeviceConfig)) ;
	g_wifiConfigs[networkId].netId = WIFI_CONFIG_INVALID;
#if 1
	snprintf(file_name, sizeof(file_name), "%s%02d",wifi_store_filename,networkId);
	UtilsFileDelete(file_name);
#endif	
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}

	return WIFI_SUCCESS;
}

WifiErrorCode ConnectTo(int networkId)
{
	int chan = 0;
	int ret=0;
	wifi_sta_config_t sta_config = WIFI_DEFAULT_STA_CONFIG();
	if (networkId >= WIFI_MAX_CONFIG_SIZE || networkId < 0) 
	{
       	return ERROR_WIFI_INVALID_ARGS;
	}
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	if (g_wifiConfigs[networkId].netId != networkId) 
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
       	return ERROR_WIFI_NOT_AVAILABLE;
	}
	
	if(NULL == strcpy(sta_config.ssid, g_wifiConfigs[networkId].ssid))
		return ERROR_WIFI_NOT_AVAILABLE;
	if(NULL ==  strcpy(sta_config.password, g_wifiConfigs[networkId].preSharedKey))
		return ERROR_WIFI_NOT_AVAILABLE;
	ret = memcpy_s(sta_config.bssid, sizeof(sta_config.bssid), g_wifiConfigs[networkId].bssid, sizeof(g_wifiConfigs[networkId].bssid));
	sta_config.channel =  FrequencyToChannel(g_wifiConfigs[networkId].freq);
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if(ret != EOK)
		return ERROR_WIFI_UNKNOWN;

	if (LockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	// bk_printf("---ssid:%s,psw:%s,%d-----\r\n",sta_config.ssid,sta_config.password,sta_config.security);
//	bk_printf("--bssid:%x,%x,%x,%x,%x--\r\n",sta_config.bssid[0],sta_config.bssid[1],sta_config.bssid[2],sta_config.bssid[3],sta_config.bssid[4]);
	
	bk_wifi_sta_set_config(&sta_config);
	bk_wifi_sta_start();
	if (UnlockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
#if 1
//wait for connect result		
	int time_out_cnt = 15;
	WifiLinkedInfo info ={0};
	ret = ERROR_WIFI_NOT_AVAILABLE;
	
	while(time_out_cnt --)
	{
		osDelay(500);//4//1s
		GetLinkedInfo(&info);
		if(info.connState == WIFI_CONNECTED)
		{
			bk_printf("--wifi connected--\r\n");
			ret = WIFI_SUCCESS;
			break;
		}
	}
#endif

	return ret;
}

WifiErrorCode Disconnect(void)
{
	int ret;
	WifiLinkedInfo info ={0};
	
	ret = GetLinkedInfo(&info);
	if(ret != WIFI_SUCCESS)
		return ret;
	//printf("connState=%d, ssid=%s\n", info.connState, info.ssid);
	bk_wifi_sta_stop();
	if(info.connState != WIFI_CONNECTED)
		return ERROR_WIFI_NOT_AVAILABLE;
	return WIFI_SUCCESS;
}

extern unsigned int ipaddr_addr(const char *cp);

WifiErrorCode GetLinkedInfo(WifiLinkedInfo* result)
{
	wifi_link_status_t linkstat={0};
	int ret;
	netif_ip4_config_t config;
	if(result == NULL)
		return ERROR_WIFI_INVALID_ARGS;
	if(bk_wifi_sta_get_link_status(&linkstat) != 0)
		return ERROR_WIFI_UNKNOWN;
	
	memcpy(result->ssid,  linkstat.ssid, sizeof(linkstat.ssid));
	memcpy(result->bssid, linkstat.bssid, WIFI_MAC_LEN);
	result->connState = ((linkstat.state == 4) || (linkstat.state == 9) || (linkstat.state == 10)) ? WIFI_CONNECTED : WIFI_DISCONNECTED;
	result->rssi = linkstat.rssi;
	//printf("link info : ssid=%s, state=%d, rssi=%d\n", linkstat.ssid, linkstat.state, linkstat.rssi);
	ret = bk_netif_get_ip4_config(NETIF_IF_STA, &config);
	if (ret) {
		printf("get ip failed\n");
		return ERROR_WIFI_UNKNOWN;
	}
	result->ipAddress = ipaddr_addr(config.ip);
	//printf("ip:0x%x\n",result->ipAddress);
	return WIFI_SUCCESS;
}

WifiErrorCode RegisterWifiEvent(WifiEvent* event)
{
	if(event == NULL)
		return ERROR_WIFI_INVALID_ARGS;

	int emptySlot = WIFI_CONFIG_INVALID;
	if (LockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	for(int i=0; i<WIFI_MAX_EVENT_SIZE;i++)
	{
		if(g_wifiEvents[i] == event)
		{
			return ERROR_WIFI_INVALID_ARGS;
		}
		if(g_wifiEvents[i] != NULL)
		{
			continue;
		}
		emptySlot = i;
		break;
	}
	 if (emptySlot == WIFI_CONFIG_INVALID) 
	 {
		if (UnlockWifiEventLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return ERROR_WIFI_BUSY;
	 }
	 g_wifiEvents[emptySlot] = event;
	if (UnlockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	} 
	
	int ret=bk_event_register_cb(EVENT_MOD_WIFI, EVENT_ID_ALL, BkWifiEventCb, NULL);
	 return WIFI_SUCCESS;
}

WifiErrorCode UnRegisterWifiEvent(const WifiEvent* event)
{
	if (event == NULL)
	{
		return ERROR_WIFI_INVALID_ARGS;
	}
	
	if (LockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	for(int i=0; i<WIFI_MAX_EVENT_SIZE; i++)
	{
		if(g_wifiEvents[i] != event)
			continue;

		g_wifiEvents[i] = NULL;
		if (UnlockWifiEventLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return WIFI_SUCCESS;
	}
	
	if (UnlockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}	
	return ERROR_WIFI_UNKNOWN;	
}

WifiErrorCode GetDeviceMacAddress(unsigned char* result)
{
	bk_wifi_sta_get_mac(result);
	return WIFI_SUCCESS;
}


#define SSID_SCAN_TYPE 0
#define NORMAL_SCAN_TYPE 1
static WifiErrorCode ScanTypeJudge(WifiScanParams *params, uint8_t *BkScanType)
{
	*BkScanType = NORMAL_SCAN_TYPE;
	
	if((params->scanType == WIFI_FREQ_SCAN) && ((params->freqs < 2412 ) || (params->freqs > 2472)))
		return ERROR_WIFI_UNKNOWN;
	
	if( (params->scanType == WIFI_BSSID_SCAN) && 
	    ( (params->bssid[0] == 0)&& (params->bssid[1] == 0)&& (params->bssid[2] == 0)&& (params->bssid[3] == 0)&&
	      (params->bssid[4] == 0)&& (params->bssid[5] == 0)) )
	    return ERROR_WIFI_UNKNOWN;
	
	if((params->scanType == WIFI_SSID_SCAN) &&(params->ssidLen == 0))
		return ERROR_WIFI_UNKNOWN;

	if(params->scanType == WIFI_SSID_SCAN)
		*BkScanType = SSID_SCAN_TYPE;

	return WIFI_SUCCESS;

}

//only support ssid advance scan now TODO
WifiErrorCode AdvanceScan(WifiScanParams *params)
{
	uint8_t scan_type;
	WifiErrorCode ret;
	wifi_scan_config_t scan_config = {0};
	
	if(params == NULL)
		return ERROR_WIFI_UNKNOWN;

	if (LockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	for (int i = 0; i < WIFI_MAX_EVENT_SIZE; i++) 
	{
		if (g_wifiEvents[i] == NULL)
		{
			continue;
		}
		DispatchScanStateChangeEvent(NULL, g_wifiEvents[i]);
	}
	if (UnlockWifiEventLock() != WIFI_SUCCESS)
	{
		return ERROR_WIFI_UNKNOWN;
	}

	ret = ScanTypeJudge(params, &scan_type);
	if(ret != WIFI_SUCCESS)
		return ret;

	if(scan_type== NORMAL_SCAN_TYPE)
	{
		ret=bk_wifi_scan_start(NULL);
	}
	else
	{
		memcpy(scan_config.ssid,params->ssid,WIFI_MAX_SSID_LEN);
		ret=bk_wifi_scan_start(&scan_config);
	}
	if(ret == 0)
		return WIFI_SUCCESS;
	else
		return ERROR_WIFI_UNKNOWN;

}

void test_sta_connect(void)
{
	int result;
	WifiDeviceConfig config={0};
	strcpy(config.ssid, "BK-SZ-2.4G");
	strcpy(config.preSharedKey,"sundray123");
	
	AddDeviceConfig(&config, &result);
	ConnectTo(result);
}
void test_sta_scan(void)
{
bk_printf("==%s 11===\r\n",__FUNCTION__);
#if 0
	g_wifitest.OnWifiScanStateChanged = testOnWifiScanStateChangedHandler;
    g_wifitest.OnWifiConnectionChanged = testOnWifiConnectionChangedHandler;
    g_wifitest.OnHotspotStaJoin = NULL;
    g_wifitest.OnHotspotStaLeave = NULL;
    g_wifitest.OnHotspotStateChanged = NULL;
    RegisterWifiEvent(&g_wifitest);
#endif
	Scan();
#if 0
	WifiScanParams * params;
	AdvanceScan(WifiScanParams * params)
#endif
	bk_printf("==%s 22===\r\n",__FUNCTION__);
}

