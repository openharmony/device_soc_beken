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

#include <common/bk_include.h> 
#include <stdio.h>
#include <sys/stat.h>
#include <sys/times.h>
//#include <sys/unistd.h>


#include <os/mem.h>

#include <os/os.h>
#include "common/bk_assert.h"

/************** wrap C library functions **************/
void *__wrap_malloc(size_t size)
{
	return os_malloc(size);
}

void *__wrap__malloc_r(void *p, size_t size)
{
	return os_malloc(size);
}

void __wrap_free(void *pv)
{
	os_free(pv);
}

void *__wrap_calloc(size_t a, size_t b)
{
	void *pvReturn;

	pvReturn = os_malloc(a * b);
	if (pvReturn)
    {
        os_memset(pvReturn, 0, a*b);
    }

	return pvReturn;
}

void *__wrap_realloc(void *pv, size_t size)
{
	return os_realloc(pv, size);
}

void __wrap__free_r(void *p, void *x)
{
	__wrap_free(x);
}

void *__wrap__realloc_r(void *p, void *x, size_t sz)
{
	return __wrap_realloc(x, sz);
}

void *__wrap_zalloc(size_t size)
{
	return os_zalloc(size);
}

int __wrap_strlen (char *src)
{
    int ret = 0;

    for (ret = 0; *src; src++)
        ret++;

    return ret;
}

int __wrap_strncmp(const char *s1, const char *s2, size_t n)
{
    BK_ASSERT(s1 && s2);

    if(0 == n) return 0;

    while(--n && *s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

int __wrap_printf(const char *fmt, ...)
{
	char string[128] = {0};
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = vsnprintf(string, sizeof(string) - 1, fmt, ap);
	va_end(ap);

	string[127] = 0;

	os_printf(string);
    return len;
}


void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
	os_printf("%s %d func %s expr %s\n", file, line, func, failedexpr);
	BK_ASSERT(0);
}
// eof

