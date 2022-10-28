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
#pragma once
#include "driver/dvp_camera_types.h"


#ifdef __cplusplus
extern "C" {
#endif


void lcd_spi_write_cmd(uint8_t data);
void lcd_spi_init_gpio(void);
void lcd_spi_write_data(uint8_t data);



#ifdef __cplusplus
}
#endif

