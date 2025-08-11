/**
******************************************************************************
* @file    pm_dvfs.c
* @author  GPM/AIS Application Team
* @version V2.0.0
* @date    02-May-2025
* @brief   dvfs implementation
******************************************************************************
* @attention
*
* Copyright (c) 2023-2025 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "main.h"
#include "app_config.h"
#include "pm_dvfs.h"

#ifndef APP_BARE_METAL
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#ifdef APP_DVFS
dvfs_ctx_t dvfs_ctx;

void pm_init_dvfs(void)
{
  dvfs_ctx.cnt = 0;
  dvfs_ctx.opp = OPP_MAX;
  SystemClock_Config_Full();
  dvfs_ctx.cpu_freq = HAL_RCC_GetCpuClockFreq();
  /* configure Systick according to new CPU frequency                         */
  SysTick->LOAD  = (uint32_t)(dvfs_ctx.cpu_freq/1000 - 1UL);
  SysTick->VAL   = 0UL;
}

void pm_set_opp_min(opp_t opp)
{
#ifdef APP_BARE_METAL
  __disable_irq();
#else
  if (dvfs_ctx.lock == NULL)
  {
    dvfs_ctx.lock = xSemaphoreCreateMutexStatic(&dvfs_ctx.lock_buffer);
    assert(dvfs_ctx.lock != NULL);
  }
  BaseType_t status = xSemaphoreTake(dvfs_ctx.lock, portMAX_DELAY);
  configASSERT(status == pdTRUE);
#endif

  switch(dvfs_ctx.opp)
  {
    case OPP_MIN:
      if (OPP_MAX == opp)
      {
#ifndef APP_BARE_METAL
        taskENTER_CRITICAL();
#endif
        dvfs_ctx.cnt = 1;
        SystemClock_Config_Full();
        dvfs_ctx.cpu_freq = HAL_RCC_GetCpuClockFreq();
        /* configure Systick according to new CPU frequency */
        SysTick->LOAD  = (uint32_t)(dvfs_ctx.cpu_freq / 1000 - 1UL);
        SysTick->VAL   = 0UL;
        dvfs_ctx.opp = OPP_MAX;
#ifndef APP_BARE_METAL
        taskEXIT_CRITICAL();
#endif
      }
      break;

    case OPP_MAX:
      dvfs_ctx.cnt += (OPP_MAX == opp) ? 1 : -1;
      if (dvfs_ctx.cnt <= 0)
      {
#ifndef APP_BARE_METAL
        taskENTER_CRITICAL();
#endif
        RCC_S->AHB2LPENCR = 0x00010000; /* ToDo: workaround; check impacts */
        SystemClock_Config_Low();
        RCC_S->AHB2LPENSR = 0x00010000;
        dvfs_ctx.cpu_freq = HAL_RCC_GetCpuClockFreq();
        /* configure Systick according to new CPU frequency */
        SysTick->LOAD  = (uint32_t)(dvfs_ctx.cpu_freq / 1000 - 1UL);
        SysTick->VAL   = 0UL;
        dvfs_ctx.opp = OPP_MIN;
        dvfs_ctx.cnt = 0;
#ifndef APP_BARE_METAL
        taskEXIT_CRITICAL();
#endif
      }
      break;

    default:
      break;
  }

#ifdef APP_BARE_METAL
  __enable_irq();
#else
  xSemaphoreGive(dvfs_ctx.lock);
#endif
}
#endif /* APP_DVFS */