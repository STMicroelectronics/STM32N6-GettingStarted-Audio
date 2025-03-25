#include <assert.h>
#include <stdint.h>

#include "cmsis_compiler.h"
#include "tx_api.h"
#include "tx_thread.h"
#include "tx_initialize.h"

#define IS_IRQ_MODE()     (__get_IPSR() != 0U)

uint32_t HAL_GetTick()
{
  return tx_time_get();
}

void HAL_Delay(uint32_t Delay)
{
  if (IS_IRQ_MODE())
    assert(0);

  tx_thread_sleep(Delay);
}

int HAL_InitTick(uint32_t TickPriority)
{
  return 0;
}
