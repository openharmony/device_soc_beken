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

#include <common/bk_include.h>
#include <os/os.h>
#if CONFIG_FLASH_ORIGIN_API
#include "BkDriverFlash.h"
#else
#include <driver/flash_partition.h>
#endif
#include <common/bk_kernel_err.h>
#include <os/mem.h>
/* Logic partition on flash devices */

const bk_logic_partition_t bk7256_partitions[BK_PARTITION_MAX] = {
	[BK_PARTITION_BOOTLOADER] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "Bootloader",
		.partition_start_addr      = 0x00000000,
		.partition_length          = 0x11000,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_APPLICATION] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "Application",
		.partition_start_addr      = 0x11000,
		.partition_length          = 0x1CB000,      // 1836KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_APPLICATION1] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "Application1",
		.partition_start_addr      = 0x1DC000,
		.partition_length          = 0x77000,       // 476KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_OTA] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "ota",
		.partition_start_addr      = 0x253000,
		.partition_length          = 0x187000,      // 1564KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
#if CONFIG_SUPPORT_MATTER
	[BK_PARTITION_MATTER_FLASH] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "matter_data",
		.partition_start_addr	   = 0x3DA000,
		.partition_length          = 0x15000, //84KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_USR_CONFIG] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "user_data",
		.partition_start_addr	   = 0x3EF000,
		.partition_length          = 0xF000, //60KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
#else
	[BK_PARTITION_USR_CONFIG] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "user_data",
		.partition_start_addr	   = 0x3DA000,
		.partition_length          = 0x24000, //144KB
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
#endif
	[BK_PARTITION_RF_FIRMWARE] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "RF Firmware",
		.partition_start_addr      = 0x3FE000,
		.partition_length          = 0x1000,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_NET_PARAM] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "NET info",
		.partition_start_addr      = 0x3FF000,
		.partition_length          = 0x1000,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
};
