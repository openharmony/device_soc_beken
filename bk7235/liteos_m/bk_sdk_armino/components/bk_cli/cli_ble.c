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

#include "bk_cli.h"
#include <common/sys_config.h>

#define BLE_CMD_CNT (sizeof(s_ble_commands) / sizeof(struct cli_command))

static const struct cli_command s_ble_commands[] = {
#if (CONFIG_BLE == 1)
    {"ble", "ble arg1 arg2",  ble_command},
#endif
};

int cli_ble_init(void)
{
    return cli_register_commands(s_ble_commands, BLE_CMD_CNT);
}
