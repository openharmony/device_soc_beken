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

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "ohos_bt_gatt.h"
#include "ohos_bt_gatt_server.h"
#include "ohos_bt_def.h"

#include <common/bk_err.h>
#include <modules/ble.h>
#include <modules/ble_types.h>
#include "ble.h"
#include <components/system.h>
#include <os/mem.h>


struct scan_param
{
    uint8_t  filter_en;                   /**< The control of filter */
    uint8_t  channel_map;                 /**< Channel mapping */
    uint16_t interval;                    /**< The scanning interval */
    uint16_t window;                      /**< The scanning window */
};

struct adv_param
{
    uint8_t  advData[BK_BLE_MAX_ADV_DATA_LEN];   /**< Advertising data */
    uint8_t  advDataLen;                  /**< The length of advertising data */
    uint8_t  respData[BK_BLE_MAX_ADV_DATA_LEN];  /**< Scan response data */
    uint8_t  respDataLen;                 /**< The length of scan response data */
    uint8_t  channel_map;                 /**< Advertising channel map */
    uint16_t interval_min;                /**< Minimum advertising interval */
    uint16_t interval_max;                /**< Maximum advertising interval */
    uint16_t duration;                    /**< Advertising duration */
};

//extern ble_err_t bk_ble_send_ntf_value(uint32_t len, uint8_t *buf, uint16_t prf_id, uint16_t att_idx);
//extern ble_err_t bk_ble_send_ind_value(uint32_t len, uint8_t *buf, uint16_t prf_id, uint16_t att_idx);
extern ble_err_t bk_ble_adv_start(uint8_t actv_idx, struct adv_param *adv, ble_cmd_cb_t callback);
extern ble_err_t bk_ble_adv_stop(uint8_t actv_idx, ble_cmd_cb_t callback);
extern ble_err_t bk_ble_scan_start(uint8_t actv_idx, struct scan_param *scan, ble_cmd_cb_t callback);
extern ble_err_t bk_ble_scan_stop(uint8_t actv_idx, ble_cmd_cb_t callback);


static BtGattCallbacks *gatt_callbackfunc=NULL;
static BtGattServerCallbacks * gatt_server_callbackfunc=NULL;

#define MAX_SERVER_NUM		5


enum
{
	TEST_IDX_SVC,
	TEST_IDX_FF01_VAL_CHAR,
	TEST_IDX_FF01_VAL_VALUE,
	TEST_IDX_FF02_VAL_CHAR,
	TEST_IDX_FF02_VAL_VALUE,
	TEST_IDX_FF02_VAL_IND_CFG,
	TEST_IDX_FF03_VAL_CHAR,
	TEST_IDX_FF03_VAL_VALUE,
	TEST_IDX_FF03_VAL_NTF_CFG,
	TEST_IDX_NB,
};

typedef struct{
	unsigned char adv_idx;
	unsigned char connidx;
	unsigned char scan_idx;
	unsigned char peer_addr[6];
	unsigned char adv_type;
	unsigned char  advDataLen;
	unsigned char  respDataLen;
	unsigned char  advData[BK_BLE_MAX_ADV_DATA_LEN];
	unsigned char  respData[BK_BLE_MAX_ADV_DATA_LEN];
	
} BT_HAL_ENV_T;



typedef struct {
    /** UUID length */
    unsigned char uuidLen;
    /** Server UUID ID**/
    unsigned char server_id;
    unsigned char max_att_handle;
    unsigned char next_att_handle;
    /** UUID field */
    char *uuid;

    ble_attm_desc_t *ohos_att_db;
} BtServer;


static BtServer *ohos_server_uuid_para[MAX_SERVER_NUM];
static BleGattOperateFunc g_ohos_Op_Func_cb[MAX_SERVER_NUM][2];
static BT_HAL_ENV_T hal_bt_env;
static struct scan_param ohos_scan_info;
static uint8_t svc_cnt =0;

enum
{
	BK_Attr_IDX_SVC,
	BK_Attr_IDX_CHAR,
	BK_Attr_IDX_CFG,
	BK_Attr_IDX_NB,
};
#define BK_ATT_DECL_PRIMARY_SERVICE     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DECL_CHARACTERISTIC      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DESC_CLIENT_CHAR_CFG     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

ble_attm_desc_t attrListPattern[BK_Attr_IDX_NB]={

	//  Service Declaration
	[BK_Attr_IDX_SVC]         =  {BK_ATT_DECL_PRIMARY_SERVICE,BK_BLE_PERM_SET(RD, ENABLE), 0, 0},

	//  Level Characteristic Declaration
	[BK_Attr_IDX_CHAR]    = {BK_ATT_DECL_CHARACTERISTIC,	 BK_BLE_PERM_SET(RD, ENABLE), 0, 0},

	//  Level Characteristic - Client Characteristic Configuration Descriptor
	[BK_Attr_IDX_CFG] = {BK_ATT_DESC_CLIENT_CHAR_CFG,	 BK_BLE_PERM_SET(RD, ENABLE)|BK_BLE_PERM_SET(WRITE_REQ,ENABLE), 0, 0},

};




static void ble_notice_cb(ble_notice_t notice, void *param)
{
	switch (notice) {
	case BLE_5_STACK_OK:
		bk_printf("ble stack ok");
		break;
	case BLE_5_WRITE_EVENT:
	{
		ble_write_req_t *w_req = (ble_write_req_t *)param;
		bk_printf("write_cb:conn_idx:%d, prf_id:%d, add_id:%d, len:%d, data[0]:%02x\r\n",
			w_req->conn_idx, w_req->prf_id, w_req->att_idx, w_req->len, w_req->value[0]);

		BtReqWriteCbPara writeCbPara;
		writeCbPara.connId=w_req->conn_idx;
		writeCbPara.transId=OHOS_BT_TRANSPORT_LE;
		writeCbPara.bdAddr=(BdAddr *)(&hal_bt_env.peer_addr[0]);
		writeCbPara.attrHandle=w_req->att_idx;
		writeCbPara.offset=0;
		writeCbPara.length=w_req->len;
		writeCbPara.value=w_req->value;
		writeCbPara.needRsp=false;
		writeCbPara.isPrep=false; 

		if(g_ohos_Op_Func_cb[w_req->prf_id][(w_req->att_idx>>1)-1].write){
			bk_printf("fun:%x\r\n",g_ohos_Op_Func_cb[w_req->prf_id][(w_req->att_idx>>1)-1].write);
			g_ohos_Op_Func_cb[w_req->prf_id][(w_req->att_idx>>1)-1].write(&(w_req->value[0]),w_req->len);
		}
		if(gatt_server_callbackfunc&&gatt_server_callbackfunc->requestWriteCb)
			gatt_server_callbackfunc->requestWriteCb(writeCbPara);
		break;

	}
	case BLE_5_READ_EVENT:
	{
		ble_read_req_t *r_req = (ble_read_req_t *)param;
		bk_printf("read_cb:conn_idx:%d, prf_id:%d, add_id:%d\r\n",
			r_req->conn_idx, r_req->prf_id, r_req->att_idx);

#if 0
//		r_req->value[0] = 0x99;
//		r_req->value[1] = 0x34;
//		r_req->value[2] = 0x56;
//		r_req->length = 3;

		if(g_ohos_Op_Func_cb[r_req->prf_id][(r_req->att_idx>>1)-1].read){
			bk_printf("fun:%x\r\n",g_ohos_Op_Func_cb[r_req->prf_id][(r_req->att_idx>>1)-1].read);
			g_ohos_Op_Func_cb[r_req->prf_id][(r_req->att_idx>>1)-1].read(&(r_req->value[0]),&(r_req->length));
		}
#endif
		BtReqReadCbPara readCbPara;
		readCbPara.connId=r_req->conn_idx;
		readCbPara.transId=OHOS_BT_TRANSPORT_LE;
		readCbPara.bdAddr=(BdAddr *)(&hal_bt_env.peer_addr[0]);
		readCbPara.attrHandle=r_req->att_idx;
		readCbPara.offset=0;
		readCbPara.isLong=0;

		if(gatt_server_callbackfunc&&gatt_server_callbackfunc->requestReadCb)
			gatt_server_callbackfunc->requestReadCb(readCbPara);

		break;
	}

	case BLE_5_REPORT_ADV:
	{
		ble_recv_adv_t *r_ind = (ble_recv_adv_t *)param;
		bk_printf("r_ind:actv_idx:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
			r_ind->actv_idx, r_ind->adv_addr[0], r_ind->adv_addr[1], r_ind->adv_addr[2],
			r_ind->adv_addr[3], r_ind->adv_addr[4], r_ind->adv_addr[5]);
		break;
	}
	case BLE_5_MTU_CHANGE:
	{
		ble_mtu_change_t *m_ind = (ble_mtu_change_t *)param;
		bk_printf("m_ind:conn_idx:%d, mtu_size:%d\r\n", m_ind->conn_idx, m_ind->mtu_size);

		if(gatt_server_callbackfunc&&gatt_server_callbackfunc->mtuChangeCb)
			gatt_server_callbackfunc->mtuChangeCb(m_ind->conn_idx, m_ind->mtu_size);

		break;
	}
	case BLE_5_CONNECT_EVENT:
	{
		ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
		bk_printf("c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
			c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
			c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);

		memcpy(&hal_bt_env.peer_addr, c_ind->peer_addr, BK_BLE_GAP_BD_ADDR_LEN);
		BdAddr bdAddr;
		memcpy(bdAddr.addr,c_ind->peer_addr,BK_BLE_GAP_BD_ADDR_LEN);

		if(gatt_server_callbackfunc&&gatt_server_callbackfunc->connectServerCb)
			gatt_server_callbackfunc->connectServerCb(c_ind->conn_idx, 0, &bdAddr);

		break;
	}
	case BLE_5_DISCONNECT_EVENT:
	{
		ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
		bk_printf("d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);

		BdAddr bdAddr;
		memcpy(bdAddr.addr,hal_bt_env.peer_addr,BK_BLE_GAP_BD_ADDR_LEN);

		if(gatt_server_callbackfunc&&gatt_server_callbackfunc->disconnectServerCb)
			gatt_server_callbackfunc->disconnectServerCb(d_ind->conn_idx, 0, &bdAddr);;


		break;
	}
	case BLE_5_ATT_INFO_REQ:
	{
		ble_att_info_req_t *a_ind = (ble_att_info_req_t *)param;
		bk_printf("a_ind:conn_idx:%d\r\n", a_ind->conn_idx);
		a_ind->length = 128;
		a_ind->status = BK_ERR_BLE_SUCCESS;
		break;
	}

	case BLE_5_TX_DONE:
	{
		bk_printf("BLE_5_TX_DONE\r\n");
		if(gatt_server_callbackfunc&&gatt_server_callbackfunc->indicationSentCb)
			gatt_server_callbackfunc->indicationSentCb(0, 0);
		break;
	}

	case BLE_5_CREATE_DB:
	{
		ble_create_db_t *cd_ind = (ble_create_db_t *)param;
		bk_printf("cd_ind:prf_id:%d, status:%d\r\n", cd_ind->prf_id, cd_ind->status);

		if(gatt_server_callbackfunc&&gatt_server_callbackfunc->serviceStartCb)
			gatt_server_callbackfunc->serviceStartCb(cd_ind->status,cd_ind->prf_id, 0);

		break;
	}
	case BLE_5_INIT_CONNECT_EVENT:
	{
		ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
		bk_printf("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
			c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
			c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
		break;
	}
	case BLE_5_INIT_DISCONNECT_EVENT:
	{
		ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
		bk_printf("BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
		break;
	}
	case BLE_5_SDP_REGISTER_FAILED:
		bk_printf("BLE_5_SDP_REGISTER_FAILED\r\n");
		break;
	default:
		break;
	}
}

static void HAL_ble_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
	bk_printf("cmd:%d idx:%d status:%d\r\n", cmd, param->cmd_idx, param->status);
	switch(cmd){
		case BLE_INIT_ADV:
			if(gatt_callbackfunc&&gatt_callbackfunc->advEnableCb)
				gatt_callbackfunc->advEnableCb(0,param->status);
			break;
		case BLE_DEINIT_ADV:
			if(gatt_callbackfunc&&gatt_callbackfunc->advDisableCb)
				gatt_callbackfunc->advDisableCb(0,param->status);
			break;

		case BLE_SET_ADV_DATA:
			if(gatt_callbackfunc&&gatt_callbackfunc->advUpdateCb)
				gatt_callbackfunc->advUpdateCb(0,param->status);
			break;
		default:
			break;

	}

}

static void ble_stack_init(void)
{
	bk_ble_set_notice_cb(ble_notice_cb);
}


int InitBtStack(void)
{
	return OHOS_BT_STATUS_SUCCESS;
}


int EnableBtStack(void)
{
	ble_stack_init();
	svc_cnt = 0;
	return OHOS_BT_STATUS_SUCCESS;
}


int DisableBtStack(void)
{
	bk_printf("DisableBtStack NOT SUPPORT!!");
	return OHOS_BT_STATUS_UNSUPPORTED;
}


int SetDeviceName(const char *name, unsigned int len)
{
	bk_ble_appm_set_dev_name(len,(uint8_t*)name);
	return OHOS_BT_STATUS_SUCCESS;
}

int BleSetAdvData(int advId, const BleConfigAdvData *data)
{
	memcpy(&(hal_bt_env.advData[0]), &(data->advData[0]), data->advLength);
	hal_bt_env.advDataLen = data->advLength;

	memcpy(&(hal_bt_env.respData[0]), &(data->scanRspData[0]), data->scanRspLength);
	hal_bt_env.respDataLen = data->scanRspLength;

	if(gatt_callbackfunc&&gatt_callbackfunc->advDataCb)
		gatt_callbackfunc->advDataCb(0,0);

	return OHOS_BT_STATUS_SUCCESS;
}

int BleUpdateAdv(int advId, const BleAdvParams *param)
{
	bk_ble_adv_stop(advId, HAL_ble_cmd_cb);
	BleStartAdv(advId,param);
	return OHOS_BT_STATUS_SUCCESS;
}

int BleStartAdv(int advId, const BleAdvParams *param)
{
	struct adv_param hal_adv;

	hal_adv.channel_map=param->channelMap;
	hal_adv.interval_min=param->minInterval;
	hal_adv.interval_max=param->maxInterval;
	hal_adv.duration=param->duration;

	memcpy(&(hal_adv.advData[0]), &(hal_bt_env.advData[0]), hal_bt_env.advDataLen);
	hal_adv.advDataLen = hal_bt_env.advDataLen;

	memcpy(&(hal_adv.respData[0]), &(hal_bt_env.respData[0]), hal_bt_env.respDataLen);
	hal_adv.respDataLen = hal_bt_env.respDataLen;

	#if 1
	bk_printf("respDataLen:%d,advDataLen:%d,duration:%d,interval_min:%d,interval_max:%d,channel_map:%d\r\n",hal_adv.respDataLen,hal_adv.advDataLen,hal_adv.duration,hal_adv.interval_min,hal_adv.interval_max,hal_adv.channel_map);

	for(int i=0;i<hal_adv.advDataLen;i++)
	{
		bk_printf("%x ",hal_adv.advData[i]);
	}

	bk_printf("\r\n");

	for(int i=0;i<hal_adv.respDataLen;i++)
	{
		bk_printf("%x ",hal_adv.respData[i]);
	}
	bk_printf("\r\n");
	#endif

	hal_bt_env.adv_idx=0;//bk_ble_get_idle_actv_idx_handle();
	bk_ble_adv_start(hal_bt_env.adv_idx,&hal_adv,HAL_ble_cmd_cb);

	return OHOS_BT_STATUS_SUCCESS;
}

int BleStartAdvEx(int *advId, const StartAdvRawData rawData, BleAdvParams advParam)
{
	struct adv_param hal_adv;
	//*advId=0;

	hal_adv.channel_map=advParam.channelMap;
	hal_adv.interval_min=advParam.minInterval;
	hal_adv.interval_max=advParam.maxInterval;
	hal_adv.duration=advParam.duration;
	//advParam.advType;
	memcpy(&(hal_adv.advData[0]), &(rawData.advData[0]), rawData.advDataLen);
	hal_adv.advDataLen = rawData.advDataLen;
	memcpy(&(hal_adv.respData[0]), &(rawData.rspData[0]), rawData.rspDataLen);
	hal_adv.respDataLen = rawData.rspDataLen;

#if 1
	bk_printf("respDataLen:%d,advDataLen:%d,duration:%d,interval_min:%d,interval_max:%d,channel_map:%d\r\n",hal_adv.respDataLen,hal_adv.advDataLen,hal_adv.duration,hal_adv.interval_min,hal_adv.interval_max,hal_adv.channel_map);

	for(int i=0;i<hal_adv.advDataLen;i++)
	{
		bk_printf("%x ",hal_adv.advData[i]);
	}

	bk_printf("\r\n");

	for(int i=0;i<hal_adv.respDataLen;i++)
	{
		bk_printf("%x ",hal_adv.respData[i]);
	}
	bk_printf("\r\n");
#endif

	hal_bt_env.adv_idx=0;//bk_ble_get_idle_actv_idx_handle();
	bk_ble_adv_start(hal_bt_env.adv_idx,&hal_adv,HAL_ble_cmd_cb);

	return OHOS_BT_STATUS_SUCCESS;

}

int BleStopAdv(int advId)
{
	bk_ble_adv_stop(hal_bt_env.adv_idx, HAL_ble_cmd_cb);
	return OHOS_BT_STATUS_SUCCESS;
}


int BleSetSecurityIoCap(BleIoCapMode mode)
{
	return OHOS_BT_STATUS_SUCCESS;
}

int BleSetSecurityAuthReq(BleAuthReqMode mode)
{
	return OHOS_BT_STATUS_SUCCESS;
}

int BleGattSecurityRsp(BdAddr bdAddr, bool accept)
{
	return OHOS_BT_STATUS_SUCCESS;
}

extern struct bd_addr_t common_default_bdaddr;

int ReadBtMacAddr(unsigned char *mac, unsigned int len)
{
	memcpy(mac, (uint8 *)&common_default_bdaddr, BK_BLE_GAP_BD_ADDR_LEN);	//xxx
	return OHOS_BT_STATUS_SUCCESS;
}


int BleSetScanParameters(int clientId, BleScanParams *param)
{
	ohos_scan_info.channel_map = 7;
	ohos_scan_info.interval = param->scanInterval;
	ohos_scan_info.window = param->scanWindow;
	hal_bt_env.scan_idx = bk_ble_get_idle_actv_idx_handle();

	if(gatt_callbackfunc&&gatt_callbackfunc->scanParamSetCb)
				gatt_callbackfunc->scanParamSetCb(0,0);

	return OHOS_BT_STATUS_SUCCESS;	
}

int BleStartScan(void)
{
	bk_ble_scan_start(hal_bt_env.scan_idx, &ohos_scan_info, HAL_ble_cmd_cb);
	return OHOS_BT_STATUS_SUCCESS;
}

int BleStopScan(void)
{
	bk_ble_stop_scaning(hal_bt_env.scan_idx, HAL_ble_cmd_cb);
	return OHOS_BT_STATUS_SUCCESS;
}

int BleGattRegisterCallbacks(BtGattCallbacks *func)
{
	gatt_callbackfunc=func;
	return OHOS_BT_STATUS_SUCCESS;
}



int BleGattsRegister(BtUuid appUuid)
{
	if(ohos_server_uuid_para[svc_cnt]==NULL)
	{
		bk_printf("%s:%d,svc:%d\r\n",__FUNCTION__,__LINE__,svc_cnt);
		ohos_server_uuid_para[svc_cnt]=os_malloc(sizeof(BtServer));
		ohos_server_uuid_para[svc_cnt]->uuid=os_malloc(appUuid.uuidLen);
		memcpy(ohos_server_uuid_para[svc_cnt]->uuid,appUuid.uuid,appUuid.uuidLen);
		ohos_server_uuid_para[svc_cnt]->uuidLen=appUuid.uuidLen;

		if(gatt_server_callbackfunc&&gatt_server_callbackfunc->registerServerCb)
			gatt_server_callbackfunc->registerServerCb(0, 0, &appUuid);
	}

	return OHOS_BT_STATUS_SUCCESS;
}


int BleGattsUnRegister(int serverId)
{
	return OHOS_BT_STATUS_SUCCESS;
}


int BleGattsDisconnect(int serverId, BdAddr bdAddr, int connId)
{
	bk_ble_disconnect(hal_bt_env.scan_idx,  HAL_ble_cmd_cb);
	return OHOS_BT_STATUS_SUCCESS;	
}

int BleGattsAddService(int serverId, BtUuid srvcUuid, bool isPrimary, int number)
{

	if(serverId>= MAX_SERVER_NUM)
	{
		bk_printf("BleGattsAddService add server failed ,serverId is %d,max is %d\r\n",serverId, MAX_SERVER_NUM-1);
		return OHOS_BT_STATUS_FAIL;
	}

	if(ohos_server_uuid_para[serverId]== NULL)
	{
		ohos_server_uuid_para[serverId]= os_malloc(sizeof(BtServer));
		ohos_server_uuid_para[serverId]->uuid=os_malloc(srvcUuid.uuidLen);
		ohos_server_uuid_para[serverId]->ohos_att_db=os_malloc(sizeof(ble_attm_desc_t)*number);
		memcpy(ohos_server_uuid_para[serverId]->uuid,srvcUuid.uuid,srvcUuid.uuidLen);
		
    //primary service declartion
		ohos_server_uuid_para[serverId]->ohos_att_db[0].uuid[0]=0x00;
		ohos_server_uuid_para[serverId]->ohos_att_db[0].uuid[1]=0x28;

		ohos_server_uuid_para[serverId]->ohos_att_db[0].perm=BK_BLE_PERM_SET(RD, ENABLE);
		ohos_server_uuid_para[serverId]->ohos_att_db[0].ext_perm=0;
		ohos_server_uuid_para[serverId]->ohos_att_db[0].max_size=0;

		ohos_server_uuid_para[serverId]->uuidLen=srvcUuid.uuidLen;
		ohos_server_uuid_para[serverId]->server_id=serverId;
		ohos_server_uuid_para[serverId]->max_att_handle=number;

		ohos_server_uuid_para[serverId]->next_att_handle=1;

		if(gatt_server_callbackfunc&&gatt_server_callbackfunc->serviceAddCb)
			gatt_server_callbackfunc->serviceAddCb(0, serverId, &srvcUuid, 0);

		return OHOS_BT_STATUS_SUCCESS;

	}

	return OHOS_BT_STATUS_FAIL;
	
}


/* ????? */
int BleGattsAddCharacteristic(int serverId, int srvcHandle, BtUuid characUuid, int properties, int permissions)
{
	
	int index;
	int max_index;
	uint32_t mode=0;

	if(ohos_server_uuid_para[serverId])
	{

		index=ohos_server_uuid_para[serverId]->next_att_handle;
		max_index=ohos_server_uuid_para[serverId]->max_att_handle;
		if(index<max_index-1)
		{
	//characteristic declartion
			ohos_server_uuid_para[serverId]->ohos_att_db[index].uuid[0]=0x03;
			ohos_server_uuid_para[serverId]->ohos_att_db[index].uuid[1]=0x28;
			ohos_server_uuid_para[serverId]->ohos_att_db[index].perm=BK_BLE_PERM_SET(RD, ENABLE);
			ohos_server_uuid_para[serverId]->ohos_att_db[index].ext_perm=0;
			ohos_server_uuid_para[serverId]->ohos_att_db[index].max_size=0;

			index++;
	//characterisitc value declartion
			ohos_server_uuid_para[serverId]->next_att_handle++;
			memcpy(ohos_server_uuid_para[serverId]->ohos_att_db[index].uuid,characUuid.uuid,characUuid.uuidLen);

			if(properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_READ)
			{
				mode|=BK_BLE_PERM_SET(RD, ENABLE);
			}
			if(properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP)
			{
				mode|=BK_BLE_PERM_SET(WRITE_COMMAND,ENABLE);
			}
			if(properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE)
			{
				mode|=BK_BLE_PERM_SET(WRITE_REQ,ENABLE);
			}
			if(properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY)
			{
				mode|=BK_BLE_PERM_SET(NTF,ENABLE);
			}
			if(properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE)
			{
				mode|=BK_BLE_PERM_SET(IND,ENABLE);
			}
			ohos_server_uuid_para[serverId]->ohos_att_db[index].perm=mode;

			if(characUuid.uuidLen==2)
			{
				ohos_server_uuid_para[serverId]->ohos_att_db[index].ext_perm=BK_BLE_PERM_SET(RI, ENABLE)|BK_BLE_PERM_SET(UUID_LEN, UUID_16);
			}
			else if(characUuid.uuidLen==16)
			{
				ohos_server_uuid_para[serverId]->ohos_att_db[index].ext_perm=BK_BLE_PERM_SET(RI, ENABLE)|BK_BLE_PERM_SET(UUID_LEN, UUID_128);
			}
			ohos_server_uuid_para[serverId]->ohos_att_db[index].max_size=128;
			ohos_server_uuid_para[serverId]->next_att_handle++;

			if(gatt_server_callbackfunc&&gatt_server_callbackfunc->serviceAddCb)
				gatt_server_callbackfunc->characteristicAddCb(0, serverId, &characUuid,srvcHandle, index);

			return OHOS_BT_STATUS_SUCCESS;
		}

	}

	return OHOS_BT_STATUS_FAIL;


}

/* Adds a descriptor to a specified characteristic */
int BleGattsAddDescriptor(int serverId, int srvcHandle, BtUuid descUuid, int permissions)
{
	int index,max_index;
	if(ohos_server_uuid_para[serverId])
	{
		index=ohos_server_uuid_para[serverId]->next_att_handle;
		max_index=ohos_server_uuid_para[serverId]->max_att_handle;
		if(index<max_index)
		{

			memcpy(ohos_server_uuid_para[serverId]->ohos_att_db[index].uuid,descUuid.uuid,descUuid.uuidLen);
			//bk_printf("3att uuid:%x %x\r\n",ohos_server_uuid_para[serverId]->ohos_att_db[index].uuid[0],ohos_server_uuid_para[serverId]->ohos_att_db[index].uuid[1]);
			if(permissions&OHOS_GATT_PERMISSION_READ)
			{
				ohos_server_uuid_para[serverId]->ohos_att_db[index].perm|=BK_BLE_PERM_SET(RD, ENABLE);
			}
			if(permissions&OHOS_GATT_PERMISSION_WRITE)
			{
				ohos_server_uuid_para[serverId]->ohos_att_db[index].perm|=BK_BLE_PERM_SET(WRITE_REQ, ENABLE);
			}

			ohos_server_uuid_para[serverId]->ohos_att_db[index].ext_perm=0;
			ohos_server_uuid_para[serverId]->ohos_att_db[index].max_size=0;
			ohos_server_uuid_para[serverId]->next_att_handle++;

			if(gatt_server_callbackfunc&&gatt_server_callbackfunc->descriptorAddCb)
				gatt_server_callbackfunc->descriptorAddCb(0, serverId, &descUuid,srvcHandle, index);

			return OHOS_BT_STATUS_SUCCESS;

		}
	}

	return OHOS_BT_STATUS_FAIL;
}

int BleGattsStartService(int serverId, int srvcHandle)
{
    ble_err_t status = BK_ERR_BLE_SUCCESS;

    struct bk_ble_db_cfg ble_db_cfg;

    ble_db_cfg.att_db = ohos_server_uuid_para[serverId]->ohos_att_db;
    ble_db_cfg.att_db_nb = ohos_server_uuid_para[serverId]->max_att_handle;
    ble_db_cfg.prf_task_id =serverId;
    ble_db_cfg.start_hdl = 0;


    if( ohos_server_uuid_para[serverId]->uuidLen==2)
	{
		ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);		
	}
	else if( ohos_server_uuid_para[serverId]->uuidLen==16)
	{
		ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_128);		
	}


    memcpy(&(ble_db_cfg.uuid[0]), &ohos_server_uuid_para[serverId]->uuid[0], 16);

    status = bk_ble_create_db(&ble_db_cfg);

    for(int i=0;i<ble_db_cfg.att_db_nb;i++)
    {
    	bk_printf("index:%d,bd uuid:%x %x\r\n",i,ohos_server_uuid_para[serverId]->ohos_att_db[i].uuid[0],ohos_server_uuid_para[serverId]->ohos_att_db[i].uuid[1]);
    	bk_printf("index:%d,perm:%x ext_perm:%x  max_size:%d\r\n",i,ohos_server_uuid_para[serverId]->ohos_att_db[i].perm,ohos_server_uuid_para[serverId]->ohos_att_db[i].ext_perm,ohos_server_uuid_para[serverId]->ohos_att_db[i].max_size);
	
    }

    bk_printf("ble_db_cfg.prf_task_id :%d,nb:%d,uuid:%x %x\r\n",ble_db_cfg.prf_task_id,ble_db_cfg.att_db_nb,ohos_server_uuid_para[serverId]->uuid[0],ohos_server_uuid_para[serverId]->uuid[1]);

    status = bk_ble_create_db(&ble_db_cfg);

    return (status==BK_ERR_BLE_SUCCESS)?OHOS_BT_STATUS_SUCCESS:OHOS_BT_STATUS_FAIL;
}

/*No implementation required */
int BleGattsStopService(int serverId, int srvcHandle)
{
	(void)serverId;
	(void)srvcHandle;

	return OHOS_BT_STATUS_SUCCESS;
}

/*No implementation required */
int BleGattsDeleteService(int serverId, int srvcHandle)
{
	(void)serverId;
	(void)srvcHandle;

	return OHOS_BT_STATUS_SUCCESS;
}

/*No implementation required */
int BleGattsClearServices(int serverId)
{
	(void)serverId;

	return OHOS_BT_STATUS_SUCCESS;
}

/*No implementation required */
int BleGattsSendResponse(int serverId, GattsSendRspParam *param)
{	
	(void)serverId;
	(void)param;

	return OHOS_BT_STATUS_SUCCESS;
}


int BleGattsSendIndication(int serverId, GattsSendIndParam *param)
{
bk_printf("BleGattsSendIndication srvID:%d,attr_hdl:%d->%d,cfm:%d\r\n",serverId,param->attrHandle,param->attrHandle*2,param->confirm);
	param->attrHandle *= 2;

	if(1==param->confirm)  //IND
	{
		bk_ble_send_ind_value(0,param->valueLen,(uint8_t *)param->value,serverId,param->attrHandle);
	}
	else if(0==param->confirm) //NTF
	{
		bk_ble_send_noti_value(0,param->valueLen,(uint8_t *)param->value,serverId,param->attrHandle);
	}

	return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsSetEncryption(BdAddr bdAddr, BleSecAct secAct)
{
	return OHOS_BT_STATUS_SUCCESS;
}

int BleGattsRegisterCallbacks(BtGattServerCallbacks *func)
{
	gatt_server_callbackfunc=func;
	return OHOS_BT_STATUS_SUCCESS;
}

uint8_t BleConvertAttr2BK(ble_attm_desc_t *bk_attr,BleGattAttr *hi_attr)
{
	uint8_t attr_uuidlen=0;
	uint16_t ext_perm=0;
	bk_attr->perm = bk_attr->ext_perm = 0;

	switch(hi_attr->uuidType) 
	{
		case OHOS_UUID_TYPE_16_BIT:
			attr_uuidlen=2;
			ext_perm = BK_BLE_PERM_SET(RI, ENABLE)|BK_BLE_PERM_SET(UUID_LEN, UUID_16);
			break;
	
		case OHOS_UUID_TYPE_32_BIT:
			attr_uuidlen=4;
			ext_perm = BK_BLE_PERM_SET(RI, ENABLE)|BK_BLE_PERM_SET(UUID_LEN, UUID_32);
			break;
	
		case OHOS_UUID_TYPE_128_BIT:
			attr_uuidlen=16;
			ext_perm = BK_BLE_PERM_SET(RI, ENABLE)|BK_BLE_PERM_SET(UUID_LEN, UUID_128);
			break;
		default:
			attr_uuidlen=0;
	}
	memcpy(&(bk_attr->uuid[0]),&(hi_attr->uuid[0]),attr_uuidlen);
	
	switch(hi_attr->attrType)
	{
		case OHOS_BLE_ATTRIB_TYPE_SERVICE:
			bk_attr->ext_perm=0;
			bk_attr->max_size=0;
			if(hi_attr->permission&OHOS_GATT_PERMISSION_READ){
				bk_attr->perm|=BK_BLE_PERM_SET(RD, ENABLE);
			}
			if(hi_attr->permission&OHOS_GATT_PERMISSION_WRITE){	
					bk_attr->perm|=BK_BLE_PERM_SET(WRITE_REQ,ENABLE);
			}
			break;
		case OHOS_BLE_ATTRIB_TYPE_CHAR:
		case OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE:
		case OHOS_BLE_ATTRIB_TYPE_CHAR_CLIENT_CONFIG:
			
			if(hi_attr->properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_READ)
			{
				bk_attr->perm|=BK_BLE_PERM_SET(RD, ENABLE);
			}
			if(hi_attr->properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP)
			{
				bk_attr->perm|=BK_BLE_PERM_SET(WRITE_COMMAND,ENABLE);
			}
			if(hi_attr->properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE)
			{
				bk_attr->perm|=BK_BLE_PERM_SET(WRITE_REQ,ENABLE);
			}
			if(hi_attr->properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY)
			{
				bk_attr->perm|=BK_BLE_PERM_SET(NTF,ENABLE);
			}
			if(hi_attr->properties&OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE)
			{
				bk_attr->perm|=BK_BLE_PERM_SET(IND,ENABLE);
			}
	
			bk_attr->max_size=200;
	
			bk_attr->ext_perm=ext_perm;
			break;
		default:
			break;
	}
	
	printf("hi_perm:0x%x,prop:0x%x,bk_perm:0x%x,ext:%x\r\n",hi_attr->permission,hi_attr->properties,bk_attr->perm,bk_attr->ext_perm);
	return attr_uuidlen;
}

int BleGattsStartServiceEx(int *srvcHandle, BleGattService *srvcInfo)
{
	BleGattAttr *attr_list=srvcInfo->attrList;
	uint8_t attr_num=srvcInfo->attrNum;
//	BtServer tmp_sever;
//	uint8_t attr_index=0;
//	uint8_t attr_uuidlen=0;
	uint8_t attr_num_total = 0;
    struct bk_ble_db_cfg ble_db_cfg;
	int i;

	
	//svc_cnt = (uint8_t)*srvcHandle;
	if(svc_cnt>=MAX_SERVER_NUM)
	{
		bk_printf("add server failed ,max serverId is %d\r\n",MAX_SERVER_NUM-1);
		return OHOS_BT_STATUS_FAIL;
	}

	if(ohos_server_uuid_para[svc_cnt]==NULL)
	{
		ohos_server_uuid_para[svc_cnt]=os_malloc(sizeof(BtServer));
		ohos_server_uuid_para[svc_cnt]->uuid=os_malloc(16);
	}

	//get total attr_num
	for(i=0;i<attr_num;i++){
		switch(attr_list[i].attrType){
			
			case OHOS_BLE_ATTRIB_TYPE_SERVICE:
					attr_num_total += 1;
					break;
			case OHOS_BLE_ATTRIB_TYPE_CHAR:

			case OHOS_BLE_ATTRIB_TYPE_CHAR_CLIENT_CONFIG:

			case OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE:

					attr_num_total += 2;
			default:
					break;
			}
		if(attr_list[i].properties&(OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY|OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE)){
			attr_num_total += 1;
		}
	}
bk_printf("----------attr_num:%x,total:%x----\r\n",attr_num,attr_num_total);

	ohos_server_uuid_para[svc_cnt]->ohos_att_db=os_malloc(sizeof(ble_attm_desc_t)*attr_num_total);
	ble_attm_desc_t *attr_bk_list=ohos_server_uuid_para[svc_cnt]->ohos_att_db;
	bk_printf("%s:%d,attr_num:%d,srvID:%d\r\n",__FUNCTION__,__LINE__,attr_num_total,*srvcHandle);
	unsigned char attr_idx = 0;
	while(attr_list && attr_num)
	{
		if(attr_list->attrType == OHOS_BLE_ATTRIB_TYPE_SERVICE){

			uint8_t uuidlen = 		BleConvertAttr2BK(attr_bk_list,attr_list);

			memcpy(&(ble_db_cfg.uuid[0]), &(attr_list->uuid[0]), uuidlen); //svc uuid
			
			memcpy(&(attr_bk_list->uuid[0]),&attrListPattern[BK_Attr_IDX_SVC].uuid[0],uuidlen);
			if(uuidlen==16)
				ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_128);
			else if(uuidlen==4)
				ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_32);	
			else
				ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);	

			attr_bk_list++;
			attr_list++;
			
		}else if(attr_list->attrType == OHOS_BLE_ATTRIB_TYPE_CHAR){
			memcpy(attr_bk_list,&attrListPattern[BK_Attr_IDX_CHAR],sizeof(ble_attm_desc_t));
			attr_bk_list++;
 			BleConvertAttr2BK(attr_bk_list,attr_list);
			bk_printf("reg fun[%d][%d] R 0x%x\r\n",svc_cnt,attr_idx,(attr_list->func).read);
			bk_printf("reg fun[%d][%d] W 0x%x\r\n",svc_cnt,attr_idx,(attr_list->func).write);
			bk_printf("reg fun[%d][%d] I 0x%x\r\n",svc_cnt,attr_idx,(attr_list->func).indicate);
			memcpy(&g_ohos_Op_Func_cb[svc_cnt][attr_idx],&(attr_list->func),sizeof(BleGattOperateFunc));
			//bk_printf("g fun[%d][%d] W:%x\r\n",svc_cnt,attr_idx,g_ohos_Op_Func_cb[svc_cnt][attr_idx].write);
			attr_bk_list++;

			if(attr_list->properties&(OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY|OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE)){
				memcpy(attr_bk_list,&attrListPattern[BK_Attr_IDX_CFG],sizeof(ble_attm_desc_t));
				attr_bk_list++;
			}
			attr_list++;
			attr_idx++;
		}else{
			BleConvertAttr2BK(attr_bk_list,attr_list);
			attr_bk_list++;
			attr_list++;
		}
		attr_num--;

	}

	ble_err_t status = BK_ERR_BLE_SUCCESS;


    ble_db_cfg.att_db = ohos_server_uuid_para[svc_cnt]->ohos_att_db;
    ble_db_cfg.att_db_nb = attr_num_total;//srvcInfo->attrNum;
    ble_db_cfg.prf_task_id =svc_cnt;
    ble_db_cfg.start_hdl = 0;
#if 1
for(i=0,attr_bk_list=&ohos_server_uuid_para[svc_cnt]->ohos_att_db[0];i<attr_num_total;i++,attr_bk_list++)
{
	for(int m=0;m<16;m++)
		printf("%02x ",attr_bk_list->uuid[m]);
	printf("perm:0x%02x,ext_perm:0x%02x,max_size:%d\r\n",attr_bk_list->perm,attr_bk_list->ext_perm,attr_bk_list->max_size);
	printf("\r\n-----------------------\r\n");
}
#endif

	status = bk_ble_create_db(&ble_db_cfg);	
	if (status == BK_ERR_BLE_SUCCESS) svc_cnt++;

	return (status==BK_ERR_BLE_SUCCESS)?OHOS_BT_STATUS_SUCCESS:OHOS_BT_STATUS_FAIL;
}

int BleGattsStopServiceEx(int srvcHandle)
{
	(void)srvcHandle;
	return OHOS_BT_STATUS_SUCCESS;
}


