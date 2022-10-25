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
#include "wifi_hotspot.h"
#include <common/bk_err.h>
#include <modules/wifi.h>
#include <components/netif_types.h>


#define WLAN_DEFAULT_IP         "192.168.0.1"
#define WLAN_DEFAULT_GW         "192.168.0.1"
#define WLAN_DEFAULT_MASK       "255.255.255.0"

#define RSSI_LEVEL_4_2_G (-65)
#define RSSI_LEVEL_3_2_G (-75)
#define RSSI_LEVEL_2_2_G (-82)
#define RSSI_LEVEL_1_2_G (-88)

#define RSSI_LEVEL_4_5_G (-65)
#define RSSI_LEVEL_3_5_G (-72)
#define RSSI_LEVEL_2_5_G (-79)
#define RSSI_LEVEL_1_5_G (-85)


static int g_wifiApStatus = WIFI_HOTSPOT_NOT_ACTIVE;
static unsigned char g_wifiApCfgFlag = 0;
static unsigned char g_band = 0;

static void SetHotspotParam(char *ssid, char *key,unsigned char security, unsigned char channel)
{
	netif_ip4_config_t config = {0};
	wifi_ap_config_t ap_cfg = WIFI_DEFAULT_AP_CONFIG();
	
	strcpy(config.ip, WLAN_DEFAULT_IP);
	strcpy(config.mask, WLAN_DEFAULT_MASK);
	strcpy(config.gateway, WLAN_DEFAULT_GW);
	strcpy(config.dns, WLAN_DEFAULT_GW);

	bk_netif_set_ip4_config(NETIF_IF_AP, &config);
	if(ssid != NULL)
		strcpy(ap_cfg.ssid, ssid);
	if(key != NULL)
		strcpy(ap_cfg.password, key);

	ap_cfg.security = security;
	ap_cfg.channel = channel;
	printf("xxx channel = %d\n", channel);
	
	bk_wifi_ap_set_config(&ap_cfg);
}

extern void DispatchApStartEvent(int state);
WifiErrorCode EnableHotspot(void)
{

	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if (g_wifiApStatus == WIFI_HOTSPOT_ACTIVE)
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return ERROR_WIFI_BUSY;
	}
	if(g_wifiApCfgFlag != 1)
		return ERROR_WIFI_NOT_AVAILABLE;

	bk_wifi_ap_start();
	g_wifiApStatus = WIFI_HOTSPOT_ACTIVE;
	DispatchApStartEvent(WIFI_STATE_AVAILABLE);
	
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	return WIFI_SUCCESS;
}

WifiErrorCode DisableHotspot(void)
{
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if (g_wifiApStatus == WIFI_HOTSPOT_NOT_ACTIVE) 
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}

		return ERROR_WIFI_NOT_STARTED;
	}

	bk_wifi_ap_stop();
	DispatchApStartEvent(WIFI_STATE_NOT_AVAILABLE);
	
	g_wifiApStatus = WIFI_HOTSPOT_NOT_ACTIVE;
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS)
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	return WIFI_SUCCESS;
}

WifiErrorCode SetHotspotConfig(const HotspotConfig* config)
{
	if (config == NULL)
	{
		return ERROR_WIFI_INVALID_ARGS;
	}


	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	SetHotspotParam(config->ssid,config->preSharedKey,HoSecToBKSec(config->securityType),config->channelNum);	//lin : maybe 11
	
	g_wifiApCfgFlag = 1;
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}

	return WIFI_SUCCESS;
}

WifiErrorCode GetHotspotConfig(HotspotConfig* result)
{
	wifi_ap_config_t ap_info={0};
	
	if (result == NULL)
	{
		return ERROR_WIFI_INVALID_ARGS;
	}

	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	bk_wifi_ap_get_config(&ap_info);

	strcpy(result->preSharedKey, ap_info.password);
	strcpy(result->ssid, ap_info.ssid);
	result->channelNum = ap_info.channel;
	result->securityType = BKSecToHoSec(ap_info.security);
	result->band = g_band;
	
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	return WIFI_SUCCESS;
}

int IsHotspotActive(void)
{
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}

	if (UnlockWifiGlobalLock() != WIFI_SUCCESS)
	{
		return ERROR_WIFI_UNKNOWN;
	}

	return g_wifiApStatus;
}

extern WifiErrorCode GetApInfo(StationInfo* result, unsigned int* size);

WifiErrorCode GetStationList(StationInfo* result, unsigned int* size)
{
	if (result == NULL || size == NULL || *size == 0) 
	{
		return ERROR_WIFI_INVALID_ARGS;
	}

	return GetApInfo(result, size);
}

WifiErrorCode SetBand(int band)
{
	if(band != HOTSPOT_BAND_TYPE_2G)
		return ERROR_WIFI_NOT_SUPPORTED;
	g_band = band;
	return 	WIFI_SUCCESS;
}

WifiErrorCode GetBand(int* result)
{
	if (result == NULL) 
	{
		return ERROR_WIFI_INVALID_ARGS;
	}
	if(g_band == 0)
		return ERROR_WIFI_NOT_AVAILABLE;
	
	*result = HOTSPOT_BAND_TYPE_2G;
	return WIFI_SUCCESS;
}

int GetSignalLevel(int rssi, int band)
{

	if (band == HOTSPOT_BAND_TYPE_5G) 
	{
		if (rssi >= RSSI_LEVEL_4_5_G) 
		{
			return RSSI_LEVEL_4;
		}
		if (rssi >= RSSI_LEVEL_3_5_G) 
		{
			return RSSI_LEVEL_3;
		}
		if (rssi >= RSSI_LEVEL_2_5_G) 
		{
			return RSSI_LEVEL_2;
		}
		if (rssi >= RSSI_LEVEL_1_5_G) 
		{
			return RSSI_LEVEL_1;
		}
	}
	if(band == HOTSPOT_BAND_TYPE_2G)
	{

		if (rssi >= RSSI_LEVEL_4_2_G) 
		{
			return RSSI_LEVEL_4;
		}
		if (rssi >= RSSI_LEVEL_3_2_G) 
		{
			return RSSI_LEVEL_3;
		}
		if (rssi >= RSSI_LEVEL_2_2_G) 
		{
			return RSSI_LEVEL_2;
		}
		if (rssi >= RSSI_LEVEL_1_2_G)
		{
			return RSSI_LEVEL_1;
		}
	}

	return ERROR_WIFI_INVALID_ARGS;
}

int GetHotspotChannel(void)
{
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if (g_wifiApStatus == WIFI_HOTSPOT_NOT_ACTIVE) 
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS)
		{
			return ERROR_WIFI_UNKNOWN;
		}

		return ERROR_WIFI_NOT_STARTED;
	}
	
	HotspotConfig ap_config = {0};
	GetHotspotConfig(&ap_config);
	
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}

	return ap_config.channelNum;
}

WifiErrorCode GetHotspotInterfaceName(char* result, int size)
{
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if (g_wifiApStatus == WIFI_HOTSPOT_NOT_ACTIVE) 
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return ERROR_WIFI_NOT_STARTED;
	}
	HotspotConfig ap_config = {0};
	GetHotspotConfig(&ap_config);
	memcpy(result, ap_config.ssid, size);
	
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS)
	{
		return ERROR_WIFI_UNKNOWN;
	}
	return WIFI_SUCCESS;
}

void test_setHotspot(char *ssid,char *key,char security)
{
	HotspotConfig cfg={0};
	if(ssid == NULL)
		return;
	
	memcpy(cfg.ssid, ssid, strlen(ssid));
	if(key != NULL)
	{
		memcpy(cfg.preSharedKey,key,strlen(key));
		if(security > WIFI_SEC_TYPE_SAE)
			security = WIFI_SEC_TYPE_OPEN;
		cfg.securityType = security;
	}
	else
	{
		cfg.securityType = WIFI_SEC_TYPE_OPEN;
	}
	cfg.channelNum = 11;//default
	SetHotspotConfig(&cfg);
}
