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

#include "ohos_bt_def.h"
#include "ohos_bt_gatt.h"
#include "ohos_bt_gatt_server.h"
#include "common/bk_typedef.h"




#define BLE_ADV_ENABLE 	1

static const uint8_t ohos_adv_data[]={0x2,0x1,0x6,0x15,0x16,0xee,0xfd,0x1,0x1,0x7,0x4,0x0,0x11,0xf8,0x12,0x32,0x37,0x33,0x44,0xff,0x0,0x5,0x2,0x34,0x38};
static const uint8_t ohos_adv_rsp_data[]={0x11,0x9,0x48,0x69,0x2d,0x42,0x65,0x6b,0x65,0x6e,0x2d,0x31,0x32,0x37,0x33,0x44,0x30,0x30};



//static const uint8_t ohos_adv_data[]={0x02,0x01,0x06,0xb,0x9,'7','2','3','1','N','_','B','L','E',0x0};
//static const uint8_t ohos_adv_rsp_data[]={0x7,0x8,'7','2','3','1','N',0x0};
static const uint8_t ntf_ind_data[6]={0x11,0x22,0x33,0x44,0x55,0x66};
#if 1
static const uint8_t BK_ATT_SVC_UUID_1[]={0x00,0x91,0x8A,0xEF,0x39,0xDD,0x84,0xA4,0xFC,0x43,0x77,0xA2,0x00,0xE4,0xF1,0x15};
static const uint8_t BK_ATT_SVC_UUID_2[]={0x00,0x91,0x8A,0xEF,0x39,0xDD,0x84,0xA4,0xFC,0x43,0x77,0xA2,0x00,0xE5,0xF1,0x15};
static const uint8_t BK_ATT_SVC_UUID_3[]={0x00,0x91,0x8A,0xEF,0x39,0xDD,0x84,0xA4,0xFC,0x43,0x77,0xA2,0x00,0xE6,0xF1,0x15};

#define WRITE_REQ_CHARACTERISTIC_1        {0x00,0x91,0x8A,0xEF,0x39,0xDD,0x84,0xA4,0xFC,0x43,0x77,0xA2,0x01,0xE4,0xF1,0x15}

#define WRITE_REQ_CHARACTERISTIC_2        {0x00,0x91,0x8A,0xEF,0x39,0xDD,0x84,0xA4,0xFC,0x43,0x77,0xA2,0x01,0xE5,0xF1,0x15}

#define WRITE_REQ_CHARACTERISTIC_3           {0x00,0x91,0x8A,0xEF,0x39,0xDD,0x84,0xA4,0xFC,0x43,0x77,0xA2,0x02,0xE6,0xF1,0x15}
#define INDICATE_CHARACTERISTIC_3           {0x00,0x91,0x8A,0xEF,0x39,0xDD,0x84,0xA4,0xFC,0x43,0x77,0xA2,0x01,0xE6,0xF1,0x15}

#define BK_ATT_DECL_PRIMARY_SERVICE     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DECL_CHARACTERISTIC      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DESC_CLIENT_CHAR_CFG     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

enum
{
	TEST_IDX_SVC1,
	TEST_IDX_E400_VAL_CHAR,
	TEST_IDX_E400_VAL_VALUE,
	TEST_IDX_NB1,
};
enum
{
	TEST_IDX_SVC2,
	TEST_IDX_E500_VAL_CHAR,
	TEST_IDX_E500_VAL_VALUE,
	TEST_IDX_NB2,
};
enum
{
	TEST_IDX_SVC3,
	TEST_IDX_E600_VAL_CHAR3_1,
	TEST_IDX_E600_VAL_VALUE3_1,
	TEST_IDX_E600_VAL_CHAR3_2,
	TEST_IDX_E600_VAL_VALUE3_2,
	TEST_IDX_NB3,
};


BleGattAttr attrList1[TEST_IDX_NB1]={

	//  Service Declaration
	[TEST_IDX_SVC1]              = {OHOS_BLE_ATTRIB_TYPE_SERVICE,    OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_PRIMARY_SERVICE,0,0,0,{0}},

	//  Level Characteristic Declaration
	[TEST_IDX_E400_VAL_CHAR]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic Value
	[TEST_IDX_E400_VAL_VALUE]   = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, WRITE_REQ_CHARACTERISTIC_1,   0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_READ|OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE,{0}},

};
BleGattAttr attrList2[TEST_IDX_NB2]={

	//  Service Declaration
	[TEST_IDX_SVC2]              = {OHOS_BLE_ATTRIB_TYPE_SERVICE,    OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_PRIMARY_SERVICE,0,0,0,{0}},

	//  Level Characteristic Declaration
	[TEST_IDX_E500_VAL_CHAR]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic Value
	[TEST_IDX_E500_VAL_VALUE]   = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, WRITE_REQ_CHARACTERISTIC_2,   0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_READ|OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE,{0}},

};
BleGattAttr attrList3[TEST_IDX_NB3]={

	//  Service Declaration
	[TEST_IDX_SVC3]              = {OHOS_BLE_ATTRIB_TYPE_SERVICE,    OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_PRIMARY_SERVICE,0,0,0,{0}},

	//  Level Characteristic Declaration
	[TEST_IDX_E600_VAL_CHAR3_1]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic Value
	[TEST_IDX_E600_VAL_VALUE3_1]   = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, WRITE_REQ_CHARACTERISTIC_3,      0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY,{0}},
	//  Level Characteristic Declaration
	[TEST_IDX_E600_VAL_CHAR3_2]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic - Client Characteristic Configuration Descriptor
	[TEST_IDX_E600_VAL_VALUE3_2] = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, INDICATE_CHARACTERISTIC_3, 0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY,{0}},

};

//#else

#define BK_ATT_DECL_PRIMARY_SERVICE     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DECL_CHARACTERISTIC      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DESC_CLIENT_CHAR_CFG     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#define WRITE_REQ_CHARACTERISTIC        {0x01,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define INDICATE_CHARACTERISTIC         {0x02,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define NOTIFY_CHARACTERISTIC           {0x03,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}


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


BleGattAttr attrList[TEST_IDX_NB]={

	//  Service Declaration
	[TEST_IDX_SVC]              = {OHOS_BLE_ATTRIB_TYPE_SERVICE,    OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_PRIMARY_SERVICE,0,0,0,{0}},

	//  Level Characteristic Declaration
	[TEST_IDX_FF01_VAL_CHAR]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic Value
	[TEST_IDX_FF01_VAL_VALUE]   = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, WRITE_REQ_CHARACTERISTIC,   0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_READ|OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE,{0}},

	//  Level Characteristic Declaration
	[TEST_IDX_FF02_VAL_CHAR]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic Value
	[TEST_IDX_FF02_VAL_VALUE]   = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, INDICATE_CHARACTERISTIC,    0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE,{0}},
	//  Level Characteristic - Client Characteristic Configuration Descriptor
	[TEST_IDX_FF02_VAL_IND_CFG] = {OHOS_BLE_ATTRIB_TYPE_CHAR_CLIENT_CONFIG, OHOS_GATT_PERMISSION_READ|OHOS_GATT_PERMISSION_WRITE, OHOS_UUID_TYPE_128_BIT, BK_ATT_DESC_CLIENT_CHAR_CFG,  0,0,0,{0}},

	//  Level Characteristic Declaration
    [TEST_IDX_FF03_VAL_CHAR]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic Value
	[TEST_IDX_FF03_VAL_VALUE]   = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_128_BIT, NOTIFY_CHARACTERISTIC,      0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY,{0}},
	//  Level Characteristic - Client Characteristic Configuration Descriptor
	[TEST_IDX_FF03_VAL_NTF_CFG] = {OHOS_BLE_ATTRIB_TYPE_CHAR_CLIENT_CONFIG,  OHOS_GATT_PERMISSION_READ|OHOS_GATT_PERMISSION_WRITE, OHOS_UUID_TYPE_128_BIT, BK_ATT_DESC_CLIENT_CHAR_CFG, 0,0,0,{0}},

};
#endif
void ohos_advEnable_callback(int advId, int status)
{
	bk_printf("fun:%s,advId:%d,status:%d\r\n",__FUNCTION__,advId,status);
}

void ohos_advDisable_callback(int advId, int status)
{
	bk_printf("fun:%s,advId:%d,status:%d\r\n",__FUNCTION__,advId,status);
}

void ohos_advData_callback(int advId, int status)
{
	bk_printf("fun:%s,advId:%d,status:%d\r\n",__FUNCTION__,advId,status);
}

void ohos_advUpdate_callback(int advId, int status)
{
	bk_printf("fun:%s,advId:%d,status:%d\r\n",__FUNCTION__,advId,status);
}

void ohos_securityRespond_callback(const BdAddr *bdAddr)
{
	bk_printf("fun:%s,line:%d\r\n",__FUNCTION__,__LINE__);
}

void ohos_scanResult_callback(BtScanResultData *scanResultdata)
{
	bk_printf("fun:%s,line:%d\r\n",__FUNCTION__,__LINE__);
}

void ohos_scanParamSet_callback(int clientId, int status)
{
	bk_printf("fun:%s,line:%d,status:%d\r\n",__FUNCTION__,__LINE__,status);
}

void ohos_registerServerCb_callback(int status, int serverId, BtUuid *appUuid)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}

void ohos_connectServerCb_callback(int connId, int serverId, const BdAddr *bdAddr)
{
	bk_printf("fun:%s,serverId:%d,connId:%d\r\n",__FUNCTION__,serverId,connId);
}
	
void ohos_disconnectServerCb_callback(int connId, int serverId, const BdAddr *bdAddr)
{
	bk_printf("fun:%s,serverId:%d,connId:%d\r\n",__FUNCTION__,serverId,connId);
}

void ohos_ServiceAdd_callback(int status, int serverId, BtUuid *appUuid,int srvcHandle)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}

void ohos_characteristicAddCb_callback(int status, int serverId, BtUuid *uuid,int srvcHandle, int characteristicHandle)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}

void ohos_descriptorAddCb_callback(int status, int serverId, BtUuid *uuid,int srvcHandle, int descriptorHandle)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}

void ohos_serviceStartCb_callback(int status, int serverId, int srvcHandle)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}


void ohos_requestReadCb_callback(BtReqReadCbPara readCbPara)
{
	bk_printf("fun:%s\r\n",__FUNCTION__);
}

void ohos_requestWriteCb_callback(BtReqWriteCbPara writeCbPara)
{
	bk_printf("fun:%s,peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",__FUNCTION__,writeCbPara.bdAddr->addr[0], writeCbPara.bdAddr->addr[1],writeCbPara.bdAddr->addr[2], writeCbPara.bdAddr->addr[3], writeCbPara.bdAddr->addr[4], writeCbPara.bdAddr->addr[5]);
	for(int index=0;index<writeCbPara.length;index++)
	{
		bk_printf("%02x ",writeCbPara.value[index]);
	}
	bk_printf("\r\n");
}


void ohos_responseConfirmationCb_callback(int status, int handle)
{
	bk_printf("fun:%s,handle:%d,status:%d\r\n",__FUNCTION__,handle,status);
}

void ohos_indicationSentCb_callback(int connId, int status)
{
	bk_printf("fun:%s,serverId:%d,connId:%d\r\n",__FUNCTION__,connId,status);
}

void ble_test_main(void)
{
	/******* init ble stack **********/

	InitBtStack();
#if 1
	EnableBtStack();


	BtGattCallbacks gatt_func={ 							
		.advEnableCb=ohos_advEnable_callback,	
	    .advDisableCb=ohos_advDisable_callback,		
	    .advDataCb=ohos_advData_callback,			
	    .advUpdateCb=ohos_advUpdate_callback,
	    .securityRespondCb=ohos_securityRespond_callback,	
	    .scanResultCb=ohos_scanResult_callback,				
	    .scanParamSetCb=ohos_scanParamSet_callback			
	};


	BleGattRegisterCallbacks(&gatt_func);

#if 1

	BtGattServerCallbacks gatt_server_func={ 							
		.registerServerCb=ohos_registerServerCb_callback,	
	    .connectServerCb=ohos_connectServerCb_callback,		
	    .disconnectServerCb=ohos_disconnectServerCb_callback,			
	    .serviceAddCb=ohos_ServiceAdd_callback,			
	    .characteristicAddCb=ohos_characteristicAddCb_callback,	
	    .descriptorAddCb=ohos_descriptorAddCb_callback,	
	    .serviceStartCb=ohos_serviceStartCb_callback,		
	    .requestReadCb=ohos_requestReadCb_callback,
	    .requestWriteCb=ohos_requestWriteCb_callback,	
	    .responseConfirmationCb=ohos_responseConfirmationCb_callback,				
	    .indicationSentCb=ohos_indicationSentCb_callback				
	};
#endif

	BleGattsRegisterCallbacks(&gatt_server_func);


#if 1
	/******* create db server **********/
	BtUuid server_uuid;
	BleGattService srvcInfo;
	int srv_hdl;
	server_uuid.uuidLen=16;
	server_uuid.uuid=os_malloc(server_uuid.uuidLen);
	printf("server_uuid.uuid:%x\r\n",server_uuid.uuid);

	memcpy(server_uuid.uuid,BK_ATT_SVC_UUID_1,server_uuid.uuidLen);
	BleGattsRegister(server_uuid);


	srvcInfo.attrNum=TEST_IDX_NB1;
	srvcInfo.attrList=attrList1;
	srv_hdl = 0;

	BleGattsStartServiceEx(&srv_hdl,  &srvcInfo);


	server_uuid.uuidLen=16;
	server_uuid.uuid=os_malloc(server_uuid.uuidLen);


	memcpy(server_uuid.uuid,BK_ATT_SVC_UUID_2,server_uuid.uuidLen);
	BleGattsRegister(server_uuid);


	srvcInfo.attrNum=TEST_IDX_NB2;
	srvcInfo.attrList=attrList2;

	srv_hdl = 1;

	BleGattsStartServiceEx(&srv_hdl,  &srvcInfo);

	server_uuid.uuidLen=16;
	server_uuid.uuid=os_malloc(server_uuid.uuidLen);

	memcpy(server_uuid.uuid,BK_ATT_SVC_UUID_3,server_uuid.uuidLen);
	BleGattsRegister(server_uuid);


	srvcInfo.attrNum=TEST_IDX_NB3;
	srvcInfo.attrList=attrList3;

	srv_hdl = 2;

	BleGattsStartServiceEx(&srv_hdl,  &srvcInfo);

#elif 1
	/******* create db server **********/
	BtUuid server_uuid;
	server_uuid.uuidLen=2;
	server_uuid.uuid=os_malloc(server_uuid.uuidLen);

	server_uuid.uuid[0]=0xee;
	server_uuid.uuid[1]=0xff;

	BleGattsRegister(server_uuid);


	BleGattService srvcInfo;
	srvcInfo.attrNum=9;
	srvcInfo.attrList=attrList;

	BleGattsStartServiceEx(NULL,  &srvcInfo);
#else
	server_uuid.uuid[0]=0xaa;
	server_uuid.uuid[1]=0xbb;
	BleGattsAddService(0,server_uuid,0,9);

	server_uuid.uuid[0]=0x11;
	server_uuid.uuid[1]=0x22;
	BleGattsAddCharacteristic(0,0,server_uuid,OHOS_GATT_CHARACTER_PROPERTY_BIT_READ|OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE,0);

	server_uuid.uuid[0]=0x33;
	server_uuid.uuid[1]=0x44;
	BleGattsAddCharacteristic(0,0,server_uuid,OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE,0);

	server_uuid.uuid[0]=0x02;
	server_uuid.uuid[1]=0x29;
	BleGattsAddDescriptor(0,0,server_uuid,OHOS_GATT_PERMISSION_READ|OHOS_GATT_PERMISSION_WRITE);


	server_uuid.uuid[0]=0x55;
	server_uuid.uuid[1]=0x66;
	BleGattsAddCharacteristic(0,0,server_uuid,OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY,0);


	server_uuid.uuid[0]=0x02;
	server_uuid.uuid[1]=0x29;
	BleGattsAddDescriptor(0,0,server_uuid,OHOS_GATT_PERMISSION_READ|OHOS_GATT_PERMISSION_WRITE);

	BleGattsStartService(0,0);
#endif


#if BLE_ADV_ENABLE

#if 0
	/*****set adv data */
	BleConfigAdvData adv;
	adv.advLength=sizeof(ohos_adv_data);
	adv.scanRspLength=sizeof(ohos_adv_rsp_data);
	adv.advData= malloc(adv.advLength);
	memcpy(adv.advData,ohos_adv_data,adv.advLength);
	adv.scanRspData= malloc(adv.scanRspLength);
	memcpy(adv.scanRspData,ohos_adv_rsp_data,adv.scanRspLength);

	BleSetAdvData(0,&adv);

	/******* start ble adv **********/
	BleAdvParams adv_para;
	adv_para.minInterval=160;
	adv_para.maxInterval=160;
	adv_para.channelMap=7;
	adv_para.duration = 0;

	BleStartAdv(0,&adv_para);

#else
	int advId=0;
		
	StartAdvRawData rawData;
	rawData.advDataLen=sizeof(ohos_adv_data);
	rawData.rspDataLen=sizeof(ohos_adv_rsp_data);
	rawData.advData= os_malloc(rawData.advDataLen);
	memcpy(rawData.advData,ohos_adv_data,rawData.advDataLen);
	rawData.rspData= os_malloc(rawData.rspDataLen);
	memcpy(rawData.rspData,ohos_adv_rsp_data,rawData.rspDataLen);


	BleAdvParams advParam;
	advParam.minInterval=160;
	advParam.maxInterval=160;
	advParam.channelMap=7;
	advParam.duration = 0;

	BleStartAdvEx(&advId, rawData, advParam);
#endif

#endif


#if BLE_SCAN_ENABLE
	BleScanParams scan_param={.scanInterval=100,.scanWindow=50};
	BleSetScanParameters(0, &scan_param);

	BleStartScan();

	rtos_delay_milliseconds(5000);
	BleStopScan();
#endif


	rtos_delay_milliseconds(20000);

	GattsSendIndParam send_param;
	send_param.confirm=1;
	send_param.valueLen=sizeof(ntf_ind_data);
	send_param.value=(char *)ntf_ind_data;
	send_param.attrHandle=TEST_IDX_E400_VAL_VALUE;

	BleGattsSendIndication(0, &send_param);

	rtos_delay_milliseconds(2000);

	send_param.confirm=0;
	send_param.valueLen=sizeof(ntf_ind_data);
	send_param.value=(char *)ntf_ind_data;
	send_param.attrHandle=TEST_IDX_E400_VAL_VALUE;

	BleGattsSendIndication(0, &send_param);
#endif

}
