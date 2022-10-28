/*
// Copyright (C) 2022 Beken Corporation
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

void mac_ps_Command ( char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv );
int mac_ps_dtim_rf_ps_disable_send_msg ( void );
void sctrl_unconditional_mac_sleep ( void );
void sctrl_unconditional_mac_wakeup ( void );

