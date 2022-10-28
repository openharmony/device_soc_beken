// Copyright (C) 2022 Beken Corporation
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _WIFI_DEVICE_UTIL_H
#define _WIFI_DEVICE_UTIL_H

#include "stdbool.h"

#include "wifi_device.h"
#include "wifi_device_config.h"
#include "wifi_error_code.h"
#include <modules/wifi_types.h>

/**
 * @brief convert hisi security type to os security type
 *
 * @param type [in] BK security type.
 *
 * @return os security type.
 */
WifiSecurityType BKSecToHoSec(wifi_security_t mode);
wifi_security_t HoSecToBKSec(WifiSecurityType mode);

/**
 * @brief get hotspot channel
 *
 * @return current channel.
 */
int GetHotspotChannel(void);

/**
 * @brief get hotspot interface name
 *
 * @param result [out] hotspot interface name.
 * @param size   [in] result buffer size, must bigger than WIFI_IFNAME_MAX_SIZE + 1.
 *
 * @return WifiErrorCode.
 */
WifiErrorCode GetHotspotInterfaceName(char* result, int size);

/**
 * @brief lock wifi global lock
 *
 * @return WifiErrorCode.
 */
WifiErrorCode LockWifiGlobalLock(void);

/**
 * @brief unlock wifi global lock
 *
 * @return WifiErrorCode.
 */
WifiErrorCode UnlockWifiGlobalLock(void);

/**
 * @brief lock wifi event lock
 *
 * @return WifiErrorCode.
 */
WifiErrorCode LockWifiEventLock(void);

/**
 * @brief unlock wifi event lock
 *
 * @return WifiErrorCode.
 */
WifiErrorCode UnlockWifiEventLock(void);

WifiErrorCode get_wifiscan_sema(void);
WifiErrorCode set_wifiscan_sema(void);

/**
 * @brief convert channel to frequency in 2G
 *
 * @return frequency.
 */
unsigned int ChannelToFrequency(unsigned int channel);

unsigned int FrequencyToChannel(unsigned int frequency);
#endif // _WIFI_DEVICE_UTIL_H
