#include "iot_errno.h"
#include "iot_watchdog.h"

#include <driver/wdt.h>

static int wdg_inited = 0;

#define DEFAULT_WDG_TIMEOUT_MS		1000

void IoTWatchDogEnable(void)
{
	if (wdg_inited)
		return;

//	bk_wdt_start(DEFAULT_WDG_TIMEOUT_MS);
	wdg_inited = 1;
}

void IoTWatchDogKick(void)
{
	if (!wdg_inited)
		return;
	
	bk_wdt_feed();
}

void IoTWatchDogDisable(void)
{
	if (!wdg_inited)
		return;

//	bk_wdt_stop();
	wdg_inited = 0;
}

