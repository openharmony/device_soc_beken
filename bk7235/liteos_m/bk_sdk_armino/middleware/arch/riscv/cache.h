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

#ifndef __CACHE_H__
#define __CACHE_H__

#define SRAM_BLOCK_COUNT                        (4)
#define SRAM_BLOCK_SIZE                         (0x20000)

void sram_dcache_map(void);

int show_cache_config_info(void);

void flush_dcache(void *va, long size);

void flush_all_dcache(void);

#endif //__CACHE_H__