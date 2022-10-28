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
#ifndef _KEY_HANDLE_H_
#define _KEY_HANDLE_H_

#include "sys_rtos.h"
#include <os/os.h>

#define KEY_DEBUG

#ifdef KEY_DEBUG
#define KEY_PRT       os_printf
#endif
#ifdef CONFIG_COMPONENTS_TX_BUFING
#define APP_PRT       os_null_printf
#endif


#endif
