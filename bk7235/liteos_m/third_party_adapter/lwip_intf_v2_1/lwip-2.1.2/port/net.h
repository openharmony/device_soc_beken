/*
 * Copyright (C) 2022 Beken Corporation
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _NET_H_
#define _NET_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "lwip_netif_address.h"

extern void uap_ip_down(void);
extern void uap_ip_start(void);
extern void sta_ip_down(void);
extern void sta_ip_start(void);
extern uint32_t uap_ip_is_start(void);
extern uint32_t sta_ip_is_start(void);
extern void *net_get_sta_handle(void);
extern void *net_get_uap_handle(void);
extern int net_wlan_remove_netif(uint8_t *mac);
extern int net_get_if_macaddr(void *macaddr, void *intrfc_handle);
extern int net_get_if_addr(struct wlan_ip_config *addr, void *intrfc_handle);
extern void ip_address_set(int iface, int dhcp, char *ip, char *mask, char*gw, char*dns);
extern void sta_ip_mode_set(int dhcp);
#if CONFIG_WIFI6_CODE_STACK
extern bool etharp_tmr_flag;
extern void net_begin_send_arp_reply(bool is_send_arp, bool is_allow_send_req);
#endif
#if CONFIG_LWIP_FAST_DHCP
extern void net_restart_dhcp(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // _NET_H_
// eof

