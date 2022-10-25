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

#include "wifiaware.h"


typedef enum {
    WIFI_SDP_PUBLISH            = 0x01,
    WIFI_SDP_SUBSCRIBE          = 0x02,
    WIFI_SDP_BUTT
}wifi_sdp_type_enum;

typedef enum {
    WIFI_SDP_SYNC_MODE_PRIVATE,
    WIFI_SDP_SYNC_MODE_STANDARD,
    WIFI_SDP_SYNC_MODE_BOTH,
    WIFI_SDP_SYNC_MODE_BUTT
}wifi_sdp_sync_enum;

#define WIFI_OK		0
#define WIFI_FAIL	-1

typedef int (*bk_wifi_sdp_recv_cb)(unsigned char* mac, unsigned char peer_handle, unsigned char local_handle,
    unsigned char* msg, unsigned char len);
int bk_wifi_sdp_init(const char* ifname);
int bk_wifi_sdp_deinit(void);
int bk_wifi_sdp_start_service(const char* service_name, unsigned char local_handle,
    bk_wifi_sdp_recv_cb recv_cb, unsigned char role);
int bk_wifi_sdp_stop_service(unsigned char local_handle, unsigned char role);
int bk_wifi_sdp_send(unsigned char* mac_addr, unsigned char peer_handle, unsigned char local_handle,
    unsigned char* msg, int len);
int bk_wifi_sdp_adjust_tx_power(const char *ifname, signed char power);
int bk_wifi_sdp_restore_tx_power(const char *ifname);
int bk_wifi_sdp_adjust_rx_param(const char *ifname, signed char rssi);
int bk_wifi_sdp_restore_rx_param(const char *ifname);
int bk_wifi_sdp_beacon_switch(const char *ifname, unsigned char enable);
int bk_wifi_sdp_set_retry_times(unsigned int retries);
int bk_wifi_sdp_get_sync_mode(void);


int bk_wifi_sdp_init(const char* ifname) { return WIFI_FAIL; }
int bk_wifi_sdp_deinit(void) { return WIFI_FAIL; }
int bk_wifi_sdp_start_service(const char* service_name, unsigned char local_handle,
    bk_wifi_sdp_recv_cb recv_cb, unsigned char role) { return WIFI_FAIL; }
int bk_wifi_sdp_stop_service(unsigned char local_handle, unsigned char role) { return WIFI_FAIL; }
int bk_wifi_sdp_send(unsigned char* mac_addr, unsigned char peer_handle, unsigned char local_handle,
    unsigned char* msg, int len) { return WIFI_FAIL; }
int bk_wifi_sdp_adjust_tx_power(const char *ifname, signed char power) { return WIFI_FAIL; }
int bk_wifi_sdp_restore_tx_power(const char *ifname) { return WIFI_FAIL; }
int bk_wifi_sdp_adjust_rx_param(const char *ifname, signed char rssi) { return WIFI_FAIL; }
int bk_wifi_sdp_restore_rx_param(const char *ifname) { return WIFI_FAIL; }
int bk_wifi_sdp_beacon_switch(const char *ifname, unsigned char enable) { return WIFI_FAIL; }
int bk_wifi_sdp_set_retry_times(unsigned int retries) { return WIFI_FAIL; }
int bk_wifi_sdp_get_sync_mode(void) { return WIFI_FAIL; }

typedef unsigned int            uintptr_t;
unsigned int hi_cipher_hash_sha256(uintptr_t input, unsigned int input_len, unsigned char *hash, unsigned int hash_len);
unsigned int hi_cipher_hash_sha256(uintptr_t input, unsigned int input_len, unsigned char *hash, unsigned int hash_len) { return WIFI_FAIL; }



int HalWifiSdpInit(const char* ifname)
{
    if (bk_wifi_sdp_init(ifname) != WIFI_OK) {
        return -1;
    }
    return 0;
}

unsigned int HalCipherHashSha256(const char* input, unsigned int inputLen, unsigned char* hash, unsigned hashLen)
{
    if (hi_cipher_hash_sha256((uintptr_t)input, inputLen, hash, hashLen) != WIFI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpStartService(const char* svcName, unsigned char localHandle, RecvCallback recvCB, unsigned char role)
{
    if (bk_wifi_sdp_start_service(svcName, localHandle, (bk_wifi_sdp_recv_cb)recvCB, role) != WIFI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpSend(unsigned char* macAddr, unsigned char peerHandle, unsigned char localHandle,
    unsigned char* msg, int len)
{
    if (bk_wifi_sdp_send(macAddr, peerHandle, localHandle, msg, len) != WIFI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpStopService(unsigned char localHandle, unsigned char role)
{
    if (bk_wifi_sdp_stop_service(localHandle, role) != WIFI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpDeinit(void)
{
    if (bk_wifi_sdp_deinit() != WIFI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpAdjustTxPower(const char *ifname, signed char power)
{
    if (bk_wifi_sdp_adjust_tx_power(ifname, power) != WIFI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpRestoreTxPower(const char *ifname)
{
    if (bk_wifi_sdp_restore_tx_power(ifname) != WIFI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpBeaconSwitch(const char *ifname, unsigned char enable)
{
    if (bk_wifi_sdp_beacon_switch(ifname, enable) != WIFI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpSetRetryTimes(unsigned int retries)
{
    if (bk_wifi_sdp_set_retry_times(retries) != WIFI_OK) {
        return -1;
    }
    return 0;
}

int HalWifiSdpGetSyncMode(void)
{
    return bk_wifi_sdp_get_sync_mode();
}
