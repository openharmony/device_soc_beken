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
#ifndef __FLASH_BYPASS_H__
#define __FLASH_BYPASS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CURRENT_ROUTINE_TEXT_SIZE            (0x400)

/*Write Enable for Status Register*/
#define FLASH_CMD_WR_EN_SR                    (0x6)
#define FLASH_CMD_WR_DISABLE                  (0x04)
/*Write Status Register*/
#define FLASH_CMD_WR_SR                       (0x31)
#define FLASH_CMD_QUAD_IO_FAST_READ     (0xEB)
#define FLASH_GD25Q32C_SR_QUAD_EN       (0x2)

#define SPI_CFG_TX_EN_ONE_BYTE          (0x10D)
#define SPI_CFG_TX_EN_TWO_BYTE          (0x20D)
#define SPI_STATUS_TX_FINISH_INT        (BIT(13))

#define FLASH_ID_GD25Q32C               (0xC84016)

#define QE_RETRY_TIMES                  (10)

__attribute__((section(".itcm_sec_code")))   void flash_bypass_quad_enable(void);
__attribute__((section(".itcm_sec_code"))) void flash_bypass_quad_test(uint32_t quad_enable, uint32_t delay_cycle1, uint32_t delay_cycle2);

#ifdef __cplusplus
}
#endif
#endif //__FLASH_BYPASS_H__
// eof

