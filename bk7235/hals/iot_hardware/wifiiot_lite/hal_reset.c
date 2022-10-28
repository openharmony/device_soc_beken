// Copyright (C) 2022 Beken Corporation
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "iot_errno.h"
#include "reset.h"
#include <common/bk_typedef.h>
#include <sys/types.h>
#include "reset_reason.h"
//#include <driver/wdt.h>

typedef enum {
    HI_SYS_REBOOT_CAUSE_UNKNOWN = 0,
    HI_SYS_REBOOT_CAUSE_CMD,        /**< system reset begin. */
    HI_SYS_REBOOT_CAUSE_UPG,        /**< upgrade reset begin. */
    HI_SYS_REBOOT_CAUSE_UPG_B,      /**< upgrade backup image reset. */
    HI_SYS_REBOOT_CAUSE_WIFI_MODE,  /**< wifi module reset begin. */
    HI_SYS_REBOOT_CAUSE_USR_NORMAL_REBOOT, /**< user reset begin. */
    HI_SYS_REBOOT_CAUSE_USR0,
    HI_SYS_REBOOT_CAUSE_USR1,
    HI_SYS_REBOOT_CAUSE_AT_BUSY,
} hi_sys_reboot_cause;


void RebootDevice(unsigned int cause)
{
	RESET_SOURCE_STATUS reset_source;

	/*
	switch(cause) {
		case :
			break;
		default:
			break;
	}
	*/
	
	reset_source = RESET_SOURCE_REBOOT;
	bk_misc_update_set_type(reset_source);

	bk_wdt_start(5);
	while(1)
		;
}

