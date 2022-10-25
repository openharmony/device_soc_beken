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

