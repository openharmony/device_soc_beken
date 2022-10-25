#include <common/bk_include.h>
#include "portmacro.h"
#include "riscv_hal.h"
#include "soc_common.h"
#include "los_config.h"
#include "los_debug.h"
#include "los_reg.h"
#include "los_timer.h"
#include "los_context.h"
#include "los_tick.h"
#include "los_arch_interrupt.h"

#include "bk_los_timer.h"


static HWI_PROC_FUNC g_tickHandler = NULL;


void HalTickHandler(u8 param)
{
    if (g_tickHandler != NULL) {
        g_tickHandler((void *)&param);
    }
}


/*void HalClockInit(HWI_PROC_FUNC handler, UINT64 responseTimeMAX)
{
	UINT32 ret = -1;
	UINT32 int_save;

	(VOID)responseTimeMAX;
	int_save = LOS_IntLock();
	g_tickHandler = handler;

    ret = ArchHwiCreate(RISCV_MACH_TIMER_IRQ, OS_HWI_PRIO_LOWEST, 0, (HWI_PROC_FUNC)HalTickHandler, 0);
    LOS_IntRestore(int_save);
    if (LOS_OK != ret) {
        PRINT_ERR("Clock init failed!\n");
    }
}


unsigned int HalTickStart(HWI_PROC_FUNC handler)
{

	g_sysClock = OS_SYS_CLOCK;
	g_cyclesPerTick = g_sysClock / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    g_intCount = 0;

    HalClockInit(handler, LOSCFG_BASE_CORE_TICK_RESPONSE_MAX);

    return LOS_OK;
}


void HalSysTickReload(unsigned long long nextResponseTime)
{
	unsigned int intSave;

	intSave = LOS_IntLock();

	UINT64 timeMax = (UINT64)LOSCFG_BASE_CORE_TICK_RESPONSE_MAX - 1;
	UINT64 timer;

	timer = prvReadMtime();
	if ((timeMax - nextResponseTime) > timer) {
		timer += nextResponseTime;
	} else {
		timer = timeMax;
	}

	prvWriteMtimeCmp(timer);

	LOS_IntRestore(intSave);
}

unsigned long long HalGetTickCycle(unsigned int *period)
{
	(VOID)period;
	unsigned int intSave;
	unsigned long long currentTick;

	intSave = LOS_IntLock();
	
	currentTick = prvReadMtime();
	LOS_IntRestore(intSave);

	return currentTick;
}

UINT32 ArchEnterSleep(VOID)
{
    wfi();

    return LOS_OK;
}

*/




STATIC UINT32 SysTickStart(HWI_PROC_FUNC handler);
STATIC UINT64 SysTickReload(UINT64 nextResponseTime);
STATIC UINT64 SysTickCycleGet(UINT32 *period);
STATIC VOID SysTickLock(VOID);
STATIC VOID SysTickUnlock(VOID);

STATIC ArchTickTimer g_archTickTimer = {
    .freq = 0,
    .irqNum = RISCV_MACH_TIMER_IRQ,
    .periodMax = LOSCFG_BASE_CORE_TICK_RESPONSE_MAX,
    .init = SysTickStart,
    .getCycle = SysTickCycleGet,
    .reload = SysTickReload,
    .lock = SysTickLock,
    .unlock = SysTickUnlock,
    .tickHandler = NULL,
};

STATIC UINT32 SysTickStart(HWI_PROC_FUNC handler)
{
    ArchTickTimer *tick = &g_archTickTimer;

    UINT32 period = (UINT32)LOSCFG_BASE_CORE_TICK_RESPONSE_MAX;
    HwiIrqParam irqParam;
      irqParam.pDevId = (VOID *)period;
      unsigned int intSave;
      intSave = LOS_IntLock();
      g_tickHandler = handler;


    UINT32 ret = LOS_HwiCreate(RISCV_MACH_TIMER_IRQ, 0x1, 0, handler, &irqParam);
    LOS_IntRestore(intSave);
      if (ret != LOS_OK) {
         return ret;
      }

      tick->freq = OS_SYS_CLOCK;

      intSave = LOS_IntLock();
	
    prvWriteMtimeCmp(period);
    LOS_IntRestore(intSave);
      return LOS_OK;
}

STATIC UINT64 SysTickReload(UINT64 nextResponseTime)
{
	unsigned int intSave;

	intSave = LOS_IntLock();

	UINT64 timeMax = (UINT64)LOSCFG_BASE_CORE_TICK_RESPONSE_MAX - 1;
	UINT64 timer;

	timer = prvReadMtime();
	if ((timeMax - nextResponseTime) > timer) {
		timer += nextResponseTime;
	} else {
		timer = timeMax;
	}

	prvWriteMtimeCmp(timer);

	LOS_IntRestore(intSave);
    return nextResponseTime;
}

STATIC UINT64 SysTickCycleGet(UINT32 *period)
{
    (VOID)period;
	UINT64 timer;

	unsigned int intSave;
	intSave = LOS_IntLock();
	timer = prvReadMtime();
	LOS_IntRestore(intSave);
	return timer;
}

STATIC VOID SysTickLock(VOID)
{
    //HalIrqDisable(RISCV_MACH_TIMER_IRQ);
}

STATIC VOID SysTickUnlock(VOID)
{
    //HalIrqEnable(RISCV_MACH_TIMER_IRQ);
}

ArchTickTimer *ArchSysTickTimerGet(VOID)
{
    return &g_archTickTimer;
}

UINT32 ArchEnterSleep(VOID)
{
    wfi();

    return LOS_OK;
}

