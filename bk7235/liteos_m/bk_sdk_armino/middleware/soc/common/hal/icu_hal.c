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
#include "bk_arm_arch.h"
#include "icu_hal.h"
#include "icu_ll.h"

//TODO complete the ICU hal init, currently only set/get some ICU registers
bk_err_t icu_hal_init(icu_hal_t *hal)
{
	hal->hw = (icu_hw_t *)ICU_LL_REG_BASE();
	return BK_OK;
}
