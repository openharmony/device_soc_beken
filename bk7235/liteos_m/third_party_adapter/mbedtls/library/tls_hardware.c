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
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)

static int os_get_random(unsigned char *buf, size_t len)
{
	    int i, j;
	        unsigned long tmp;

		    for (i = 0; i < (((int)len + 3) & ~3) / 4; i++) {
			            tmp = rand();

				            for (j = 0; j < 4; j++) {
						                if ((i * 4 + j) < (int)len) {
									                buf[i * 4 + j] = (unsigned char)(tmp >> (j * 8));
											            } else {
													                    break;
															                }
								        }
					        }

		        return 0;
}

int mbedtls_hardware_poll( void *data,
		                           unsigned char *output, size_t len, size_t *olen )
{
	    (void)data;
	    os_get_random(output, len);
	    *olen = len;

	    return 0;
}
#endif


