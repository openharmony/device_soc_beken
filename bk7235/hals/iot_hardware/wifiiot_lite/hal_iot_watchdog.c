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

