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
#ifndef LWIP_PING_H
#define LWIP_PING_H

extern int ping(char* target_name, uint32_t times, size_t size);
void ping_stop(void);
void ping_start(char* target_name, uint32_t times, size_t size);


#endif /* LWIP_PING_H */
