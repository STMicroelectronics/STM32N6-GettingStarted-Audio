# AI folder

## runtime folder

ATONN runtime files
- imported from aton compiler package (ll_aton/src/*)

### history

- weekly build - 2023-03-19
- weekly build - 2023-02-26


## stm_ai_lib folder

X-CUBE-AI files (lib+headers files)
7.1.0 version

## notes



LL_ATON_PLATFORM=LL_ATON_PLAT_STM32N6
LL_ATON_OSAL=LL_ATON_OSAL_BARE_METAL
LL_ATON_RT_MODE = LL_ATON_RT_POLLING   or LL_ATON_RT_ASYNC  or LL_ATON_RT_IRQ_SCHED (not tested)

LL_ATON_SW_FALLBACK
LL_ATON_DUMP_DEBUG_API

- to test IRQ mode, the ll_aton_osal.h file should be patched to update the functions (functions in main.c)

```
	#define LL_ATON_OSAL_WFE() __WFE()
	#define LL_ATON_OSAL_SIGNAL_EVENT()
```

to 

```
extern void osal_wait_event();
extern void osal_set_event();

	#define LL_ATON_OSAL_WFE()            osal_wait_event()
	#define LL_ATON_OSAL_SIGNAL_EVENT()   osal_set_event()
```
