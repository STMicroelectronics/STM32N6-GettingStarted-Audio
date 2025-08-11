 /**
 ******************************************************************************
 * @file    freertos_bsp.c
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef APP_BARE_METAL

#include <assert.h>
#include <stdint.h>

#include "cmsis_compiler.h"
#include "FreeRTOS.h"
#include "stm32n6xx_hal.h"
#include "stm32n6xx_hal_tim.h"
#include "task.h"

#define IS_IRQ_MODE()     (__get_IPSR() != 0U)

static TIM_HandleTypeDef tim4;

uint32_t HAL_GetTick()
{
  if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)
  {
      return uwTick;
  }
  else
  {
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
  }
}

void HAL_Delay(uint32_t Delay)
{

  if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)
  {
    // If the scheduler is not started, use HAL_Delay directly
    // This is useful for initialization code before FreeRTOS starts
    uint32_t tickstart = HAL_GetTick();
    uint32_t wait = Delay;

    /* Add a freq to guarantee minimum wait */
    if (wait < HAL_MAX_DELAY)
    {
      wait += (uint32_t)(uwTickFreq);
    }

    while ((HAL_GetTick() - tickstart) < wait)
    {
    }
  }
  else
  {
    if (IS_IRQ_MODE())
      assert(0);
    vTaskDelay(Delay / portTICK_PERIOD_MS);
  }
}


HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)
  {
    /* Check uwTickFreq for MisraC 2012 (even if uwTickFreq is a enum type that don't take the value zero)*/
    if ((uint32_t)uwTickFreq == 0UL)
    {
      return HAL_ERROR;
    }

    /* Configure the SysTick to have interrupt in 1ms time basis*/
    if (HAL_SYSTICK_Config(SystemCoreClock / (1000UL / (uint32_t)uwTickFreq)) > 0U)
    {
      return HAL_ERROR;
    }

    /* Configure the SysTick IRQ priority */
    if (TickPriority < (1UL << __NVIC_PRIO_BITS))
    {
      HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
      uwTickPrio = TickPriority;
    }
    else
    {
      return HAL_ERROR;
    }

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    return HAL_OK;
  }
}

void TIM4_Config()
{
  const uint32_t tmr_clk_freq = 100000;
  int ret;

  __HAL_RCC_TIM4_CLK_ENABLE();

  tim4.Instance = TIM4;
  tim4.Init.Period = ~0;
  tim4.Init.Prescaler = (HAL_RCC_GetPCLK1Freq() / tmr_clk_freq) - 1;
  tim4.Init.ClockDivision = 0;
  tim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  ret = HAL_TIM_Base_Init(&tim4);
  assert(ret == HAL_OK);

  ret = HAL_TIM_Base_Start(&tim4);
  assert(ret == HAL_OK);
}

uint32_t TIM4_Get_Value()
{
  return __HAL_TIM_GET_COUNTER(&tim4);
}

#if (configSUPPORT_STATIC_ALLOCATION == 1)
void vApplicationGetIdleTaskMemory (StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
  /* Idle task control block and stack */
  static StaticTask_t Idle_TCB;
  static StackType_t  Idle_Stack[configMINIMAL_STACK_SIZE];

  *ppxIdleTaskTCBBuffer   = &Idle_TCB;
  *ppxIdleTaskStackBuffer = &Idle_Stack[0];
  *pulIdleTaskStackSize   = (uint32_t)configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory (StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
  /* Timer task control block and stack */
  static StaticTask_t Timer_TCB;
  static StackType_t  Timer_Stack[configTIMER_TASK_STACK_DEPTH];

  *ppxTimerTaskTCBBuffer   = &Timer_TCB;
  *ppxTimerTaskStackBuffer = &Timer_Stack[0];
  *pulTimerTaskStackSize   = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}
#endif

#endif