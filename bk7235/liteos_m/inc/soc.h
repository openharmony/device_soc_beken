//开源协议
#ifndef _SOC_H
#define _SOC_H

#include "soc_common.h"

#define MCAUSE_INT_ID_MASK       (0xF)
#define __start_and_irq_stack_top _stack
#define __except_stack_top _stack

#define RISCV_PLIC_VECTOR_CNT   (63)
#define RISCV_SYS_MAX_IRQ          (0)

#define MTIMER		0xE6000000
#define MTIMERCMP	0xE6000008

#endif
