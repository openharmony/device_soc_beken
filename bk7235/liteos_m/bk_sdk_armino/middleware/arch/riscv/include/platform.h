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


#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "core_v5.h"
#include "mcu_ip.h"

/*
 * Define 'NDS_PLIC_BASE' and 'NDS_PLIC_SW_BASE' before include platform
 * intrinsic header file to active PLIC/PLIC_SW related intrinsic functions.
 */
#define NDS_PLIC_BASE        PLIC_BASE
#define NDS_PLIC_SW_BASE     PLIC_SW_BASE
#include "nds_v5_platform.h"

/*****************************************************************************
 * Peripheral device HAL declaration
 ****************************************************************************/
#define DEV_PLMT             MCUIP_PLMT
#define DEV_SMU              MCUIP_SMU

#define CFG_XIP              (1)

#ifdef __cplusplus
}
#endif

#endif	/* __PLATFORM_H__ */
