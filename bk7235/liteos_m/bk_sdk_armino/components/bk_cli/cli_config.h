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


#pragma once

#include <common/sys_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (CONFIG_WIFI_ENABLE)
#define CLI_CFG_WIFI        1
#else
#define CLI_CFG_WIFI        0
#endif //#if (CONFIG_WIFI_ENABLE)
#define CLI_CFG_BLE         1

#if (CONFIG_BK_NETIF)
#define CLI_CFG_NETIF       1
#else
#define CLI_CFG_NETIF       0
#endif //#if (CONFIG_LWIP)

#define CLI_CFG_MISC        0
#define CLI_CFG_MEM         0

#if (CONFIG_WIFI_ENABLE)
#define CLI_CFG_PHY         0
#else
#define CLI_CFG_PHY         0
#endif //#if (CONFIG_WIFI_ENABLE)

#if (CONFIG_STA_PS)
#define CLI_CFG_PWR         0
#else
#define CLI_CFG_PWR         0
#endif
#define CLI_CFG_TIMER       0
#if CONFIG_INT_WDT
#define CLI_CFG_WDT         0
#endif
#if CONFIG_TRNG_SUPPORT
#define CLI_CFG_TRNG        0
#else
#define CLI_CFG_TRNG        0
#endif
#if CONFIG_EFUSE
#define CLI_CFG_EFUSE       0
#else
#define CLI_CFG_EFUSE       0
#endif
#define CLI_CFG_GPIO        0
#define CLI_CFG_OS          0
#if ((CONFIG_OTA_TFTP) || (CONFIG_OTA_HTTP))
#define CLI_CFG_OTA         0
#else
#define CLI_CFG_OTA         0
#endif
#if(CONFIG_KEYVALUE)
#define CLI_CFG_KEYVALUE    0
#else
#define CLI_CFG_KEYVALUE    0
#endif
#if(CONFIG_SUPPORT_MATTER)
#define CLI_CFG_MATTER      0
#else
#define CLI_CFG_MATTER      0
#endif
#define CLI_CFG_UART        0
#define CLI_CFG_ADC         0
#define CLI_CFG_SPI         0
#define CLI_CFG_MICO        0
#define CLI_CFG_REG         0
#define CLI_CFG_EXCEPTION   0

#if(CONFIG_GENERAL_DMA)
#define CLI_CFG_DMA         0
#else
#define CLI_CFG_DMA         0
#endif

#if(CONFIG_PWM)
#define CLI_CFG_PWM         0
#else
#define CLI_CFG_PWM         0
#endif

#if(CONFIG_FLASH)
#define CLI_CFG_FLASH       0
#else
#define CLI_CFG_FLASH       0
#endif

#if(CONFIG_SDIO_HOST)
#define CLI_CFG_SDIO_HOST   0
#else
#define CLI_CFG_SDIO_HOST   0
#endif

#if(CONFIG_ICU)
#define CLI_CFG_ICU         0
#else
#define CLI_CFG_ICU         0
#endif

#if CONFIG_I2C
#define CLI_CFG_I2C         0
#else
#define CLI_CFG_I2C         0
#endif

#if CONFIG_QSPI
#define CLI_CFG_QSPI        0
#else
#define CLI_CFG_QSPI        0
#endif

#if CONFIG_AON_RTC_TEST
#define CLI_CFG_AON_RTC     0
#else
#define CLI_CFG_AON_RTC     0
#endif

#if CONFIG_JPEG_ENCODE
#define CLI_CFG_JPEGENC        0
#else
#define CLI_CFG_JPEGENC        0
#endif

#if CONFIG_JPEG_DECODE
#define CLI_CFG_JPEGDEC     0
#else
#define CLI_CFG_JPEGDEC     0
#endif

#if CONFIG_CALENDAR
#define CLI_CFG_CALENDAR    0
#else
#define CLI_CFG_CALENDAR    0
#endif

//TODO default to 0
#define CLI_CFG_EVENT       0

#if (CONFIG_SOC_BK7251)
#define CLI_CFG_SECURITY    0
#else
#define CLI_CFG_SECURITY    0
#endif

#if CONFIG_TEMP_DETECT
#define CLI_CFG_TEMP_DETECT 0
#else
#define CLI_CFG_TEMP_DETECT 0
#endif

#if CONFIG_SDCARD_HOST
#define CLI_CFG_SD          0
#else
#define CLI_CFG_SD          0
#endif

#if CONFIG_FATFS
#define CLI_FATFS          0
#else
#define CLI_FATFS          0
#endif

#if CONFIG_AIRKISS_TEST
#define CLI_CFG_AIRKISS     0
#else
#define CLI_CFG_AIRKISS     0
#endif

#if CONFIG_IPERF_TEST
#define CLI_CFG_IPERF       0
#else
#define CLI_CFG_IPERF       0
#endif

#if (CONFIG_I2S)
#define CLI_CFG_I2S         0
#else
#define CLI_CFG_I2S         0
#endif

#if (CONFIG_SOC_BK7256XX)

#if CONFIG_DMA2D_TEST
#define CLI_CFG_DMA2D  0
#else
#define CLI_CFG_DMA2D  0
#endif

#if (CONFIG_AUDIO && CONFIG_DUAL_CORE && CONFIG_AUDIO_TEST && CONFIG_AUDIO_RISCV_IP_V1_0)
#define CLI_CFG_AUD         0
#endif

#if CONFIG_AUD_INTF_TEST
#define CLI_CFG_AUD_INTF    0
#endif

#if (CONFIG_SBC)
#define CLI_CFG_SBC			0
#endif

#if CONFIG_LCD_TEST
#define CLI_CFG_LCD  0
#else
#define CLI_CFG_LCD  0
#endif

#if (CONFIG_DUAL_CORE && CONFIG_AUDIO_TEST && !CONFIG_SLAVE_CORE)
#define CLI_CFG_AUD_CP0     0
#endif

#if (CONFIG_FFT_TEST)
#define CLI_CFG_FFT         0
#else
#define CLI_CFG_FFT         0
#endif
#endif

#if (CONFIG_AUDIO_AEC_TEST)
#define CLI_CFG_AEC         0
#endif

#if (CONFIG_AUDIO_G711_TEST)
#define CLI_CFG_G711         0
#endif

#if (CONFIG_AUDIO_MP3_TEST)
#define CLI_CFG_MP3			0
#endif

#if (CONFIG_SECURITYIP)
#define CLI_CFG_VAULT		0
#else
#define CLI_CFG_vault		0
#endif

#if ((CONFIG_SOC_BK7256XX || CONFIG_USB_UVC) && CONFIG_DUAL_CORE && CONFIG_PSRAM)
#define CLI_CFG_UVC        0
#else
#define CLI_CFG_UVC        0
#endif

#if ((CONFIG_CAMERA || CONFIG_SPIDMA) && CONFIG_APP_DEMO_VIDEO_TRANSFER)
#define CLI_CFG_DVP        0
#else
#define CLI_CFG_DVP        0
#endif

#if (CONFIG_PSRAM && !CONFIG_SLAVE_CORE)
#define CLI_CFG_PSRAM        0
#else
#define CLI_CFG_PSRAM        0

#endif

#ifdef __cplusplus
}
#endif
