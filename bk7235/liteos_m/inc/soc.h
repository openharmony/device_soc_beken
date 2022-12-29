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

#ifndef _SOC_H
#define _SOC_H

#include "soc_common.h"

#define RISCV_PLIC_VECTOR_CNT   (63)
#define RISCV_SYS_MAX_IRQ          (0)
#ifndef MTIMER
#define MTIMER        0xE6000000
#endif
#ifndef MTIMERCMP
#define MTIMERCMP    0xE6000008
#endif
#endif
