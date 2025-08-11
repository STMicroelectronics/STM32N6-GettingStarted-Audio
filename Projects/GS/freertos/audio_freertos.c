 /**
 ******************************************************************************
 * @file    app.c
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

#include <stdint.h>

#include "app_config.h"
#include "ll_aton_runtime.h"
#include "stm32n6xx_hal.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_xspi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "audio_acq_task.h"
#include "audio_proc_task.h"
#include "load_gen_task.h"
#include "audio_freertos.h"

static StaticTask_t main_thread;
static StackType_t main_thread_stack[configMINIMAL_STACK_SIZE];


#define ALIGN_VALUE(_v_,_a_) (((_v_) + (_a_) - 1) & ~((_a_) - 1))


/* rtos */
static StaticTask_t audio_proc_thread;
static StackType_t audio_proc_thread_stack[44 * configMINIMAL_STACK_SIZE];
static StaticTask_t audio_acq_thread;
static StackType_t audio_acq_stack[4 *configMINIMAL_STACK_SIZE];
static StaticTask_t load_gen_thread;
static StackType_t load_gen_stack[4 *configMINIMAL_STACK_SIZE];

static void main_thread_func(void *arg);

int main_freertos()
{
  TaskHandle_t hdl;

  hdl = xTaskCreateStatic(main_thread_func, "main", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1,
                          main_thread_stack, &main_thread);
  assert(hdl != NULL);

  vTaskStartScheduler();
  assert(0);

  return -1;
}

void app_run()
{
  UBaseType_t audio_acq_priority  = FREERTOS_AUDIO_ACQ_THREAD_PRIO;
  UBaseType_t load_gen_priority   = FREERTOS_LOAD_GEN_THREAD_PRIO;
  UBaseType_t audio_proc_priority = FREERTOS_AUDIO_PROC_THREAD_PRIO;
  TaskHandle_t hdl;

  printf("Init application\n");
  /* Enable DWT so DWT_CYCCNT works when debugger not attached */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;


  /* Static storage for queue items */
  static uint8_t AudioAcqQueueStorage[FREERTOS_AUDIO_ACQ_THREAD_IN_QUEUE_SIZE * FREERTOS_AUDIO_ACQ_THREAD_IN_QUEUE_ITEM_SIZE];

  /* Static control block for the queue */
  static StaticQueue_t AudioAcqQueueControlBlock;

  /* Create the queue statically */
  AudioAcqTask.queue = xQueueCreateStatic(
      FREERTOS_AUDIO_ACQ_THREAD_IN_QUEUE_SIZE,
      FREERTOS_AUDIO_ACQ_THREAD_IN_QUEUE_ITEM_SIZE,
      AudioAcqQueueStorage,
      &AudioAcqQueueControlBlock);

  assert(AudioAcqTask.queue != NULL);

  static uint8_t LoadGenQueueStorage[FREERTOS_LOAD_GEN_THREAD_IN_QUEUE_SIZE * FREERTOS_LOAD_GEN_THREAD_IN_QUEUE_ITEM_SIZE];
  static StaticQueue_t LoadGenQueueControlBlock;

  /* Create the queue statically */
  LoadGenTask.queue = xQueueCreateStatic(
      FREERTOS_LOAD_GEN_THREAD_IN_QUEUE_SIZE,
      FREERTOS_LOAD_GEN_THREAD_IN_QUEUE_ITEM_SIZE,
      LoadGenQueueStorage,
      &LoadGenQueueControlBlock);

  assert(LoadGenTask.queue != NULL);

  static uint8_t AudioProcQueueStorage[FREERTOS_AUDIO_PROC_THREAD_IN_QUEUE_SIZE * FREERTOS_AUDIO_PROC_THREAD_IN_QUEUE_ITEM_SIZE];
  static StaticQueue_t AudioProcQueueControlBlock;

  /* Create the queue with static buffers */
  AudioProcTask.queue = xQueueCreateStatic(
      FREERTOS_AUDIO_PROC_THREAD_IN_QUEUE_SIZE,          /* Queue length */
      FREERTOS_AUDIO_PROC_THREAD_IN_QUEUE_ITEM_SIZE, /* Item size in bytes */
      AudioProcQueueStorage,                   /* Pointer to queue storage buffer */
      &AudioProcQueueControlBlock                  /* Pointer to queue control block */
  );

  assert(AudioProcTask.queue != NULL);  /* Check queue creation success */

  /* Create mutex for Audio Acquisition Task */
  AudioAcqTask.lock = xSemaphoreCreateMutexStatic(&AudioAcqTask.lock_buffer);
  assert(AudioAcqTask.lock != NULL);

  /* threads init */
  hdl = xTaskCreateStatic(audio_proc_thread_func, "audio_proc", configMINIMAL_STACK_SIZE * 44, NULL, audio_proc_priority, audio_proc_thread_stack,
                          &audio_proc_thread);
  assert(hdl != NULL);
  hdl = xTaskCreateStatic(audio_acq_thread_func, "audio_acq_thread", configMINIMAL_STACK_SIZE * 4, NULL, audio_acq_priority, audio_acq_stack,
                          &audio_acq_thread);
  assert(hdl != NULL);
  hdl = xTaskCreateStatic(load_gen_thread_func, "load_gen_thread", configMINIMAL_STACK_SIZE * 4, NULL, load_gen_priority, load_gen_stack,
                          &load_gen_thread);
  assert(hdl != NULL);
}

static void main_thread_func(void *arg)
{
  uint32_t preemptPriority;
  uint32_t subPriority;
  IRQn_Type i;

  /* Copy SysTick_IRQn priority set by RTOS and use it as default priorities for IRQs. We are now sure that all irqs
   * have default priority below or equal to configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY.
   */
  HAL_NVIC_GetPriority(SysTick_IRQn, HAL_NVIC_GetPriorityGrouping(), &preemptPriority, &subPriority);
  for (i = PVD_PVM_IRQn; i <= LTDC_UP_ERR_IRQn; i++)
    HAL_NVIC_SetPriority(i, preemptPriority, subPriority);


  vPortSetupTimerInterrupt();

  app_run();

  vTaskDelete(NULL);
}

#ifdef APP_LP
void vApplicationIdleHook(void)
{
    /* Enter low power mode when idle */
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}
#endif