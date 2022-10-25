//开源协议
#ifndef _RISCV_HAL_H
#define _RISCV_HAL_H

#include "los_compiler.h"
#include "los_context.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern VOID HalIrqDisable(UINT32 vector);
extern VOID HalIrqEnable(UINT32 vector);
extern VOID HalSetLocalInterPri(UINT32 vector, UINT16 prior);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _RISCV_HAL_H */
