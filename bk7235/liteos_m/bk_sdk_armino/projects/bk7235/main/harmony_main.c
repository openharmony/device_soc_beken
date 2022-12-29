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
#include "bk_private/legacy_init.h"
#include <stdio.h>
#include <os/os.h>

void __attribute__((weak)) OHOS_SystemInit(void)
{
    os_printf("@NOOS@");
    return;
}

void __attribute__((weak)) OHOS_TEST(void)
{
}

extern void bk_set_printf_sync(uint8_t enable);
extern void shell_set_log_level(int level);

int main(void)
{
    bk_set_printf_sync(1);
    shell_set_log_level(2);
    legacy_init();
    OHOS_SystemInit();
    OHOS_TEST();

    return 0;
}
