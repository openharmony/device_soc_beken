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
#ifndef _shell_task_h_
#define _shell_task_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

int handle_shell_input(char *inbuf, int in_buf_size, char * outbuf, int out_buf_size);
void shell_task(void *pvParameters);

#ifdef CONFIG_MASTER_CORE
int shell_cmd_forward(char *cmd, u16 cmd_len);
#endif

int shell_assert_out(bool bContinue, char * format, ...);
int shell_assert_raw(bool bContinue, char * data_buff, u16 data_len);
int shell_trace_out( u32 trace_id, ... );
int shell_spy_out( u16 spy_id, u8 * data_buf, u16 data_len);
int shell_log_out(u8 level, char * mod_name, const char * format, ...);
void shell_log_out_port(int level, char * mod_name, const char * format, va_list ap);
int shell_log_raw_data(const u8 *data, u16 data_len);

int shell_echo_get(void);
void shell_echo_set(int en_flag);
void shell_set_log_level(int level);
int shell_get_log_level(void);
int shell_get_log_statist(u32 * info_list, u32 num);

void shell_log_flush(void);
void shell_set_uart_port(uint8_t uart_port);

#ifdef __cplusplus
}
#endif

#endif /* _shell_task_h_ */
