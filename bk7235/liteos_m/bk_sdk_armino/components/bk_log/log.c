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
#include <components/log.h>

void bk_mem_dump(const char *title, uint32_t start, uint32_t len)
{
	uint32_t val = 0;
	uint32_t mem;

	BK_LOG_RAW("%s - dump(len=%u)\n", title, len);

	for (uint32_t i = 0; i < len; i += 4) {
		mem = start + i;

		if ((i % 32) == 0) {
			if (i)
				BK_LOG_RAW("\n");

			BK_LOG_RAW("%x:", mem);
		}

		val = *((volatile uint32_t *)(mem));
		BK_LOG_RAW(" %08x", val);
	}
	BK_LOG_RAW("\n");
}


void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len)
{
	uint32_t cnt = 0;

	BK_DUMP_OUT("====== %s - dump(len=%u) begin ======\r\n", title, data_len);

	for (int i = 0;  i < data_len; i++) {
		if ((cnt++ & 0xf) == 0) {
			BK_DUMP_OUT("\r\n");
		}
		BK_DUMP_OUT("%02x ", data[i]);
	}

	BK_DUMP_OUT("\r\n");
	BK_DUMP_OUT("====== %s - dump(len=%u)   end ======\r\n", title, data_len);
	BK_DUMP_OUT("\r\n");
}