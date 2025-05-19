/**
  ******************************************************************************
  * @file    audio_tx.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include <stdint.h>

#include "audio_tx.h"
#include "tx_api.h"
#include "app_msg.h"
#include "audio_bm.h"
#include "audio_acq_task.h"
#include "audio_proc_task.h"
#include "load_gen_task.h"

/* Private variables ---------------------------------------------------------*/

typedef  struct _System{
  /* System memory pool control block. */
  TX_BYTE_POOL m_xSysMemPool;

  /* System heap. This memory block is used to create the system byte pool.
     This is a convenient way to handle the memory allocation at application level.
  */
  uint8_t m_pnHeap[INIT_TASK_CFG_HEAP_SIZE];
}System_t;

/* The only instance of System object. */
static System_t s_xTheSystem;

static TX_THREAD init_thread;

/* Private functions ---------------------------------------------------------*/
static void *SysAlloc(size_t nSize) {
   void *pcMemory = NULL;
  if (TX_SUCCESS != tx_byte_allocate(&s_xTheSystem.m_xSysMemPool,\
   (VOID **)&pcMemory, nSize, TX_NO_WAIT)) {
    pcMemory = NULL;
  }
  return pcMemory;
}

static void init_thread_func(ULONG arg)
{
  VOID *pvStackStart, *pvQueueItemsBuff ;
  UINT status;
  
  /* allocate stack from system memory pool */
  pvStackStart = SysAlloc(AUDIO_PROC_THREAD_STACK_SIZE);
  
  /* create audio proc  thread  */
  status = tx_thread_create(&AudioProcTask.thread,\
                            "audio_proc_thread",\
                            audio_proc_thread_func, 0,\
                            pvStackStart,\
                            AUDIO_PROC_THREAD_STACK_SIZE,\
                            AUDIO_PROC_THREAD_PRIO, AUDIO_PROC_THREAD_PRIO,\
                            TX_NO_TIME_SLICE, TX_AUTO_START);
  assert(TX_SUCCESS==status);

  /* and its input queue */
  pvQueueItemsBuff = SysAlloc(AUDIO_PROC_THREAD_IN_QUEUE_SIZE);
  assert(pvQueueItemsBuff != NULL);
  status=tx_queue_create(&AudioProcTask.queue, "audio_proc_in_queue",\
                         AUDIO_PROC_THREAD_IN_QUEUE_ITEM_SIZE/sizeof(uint32_t),\
                         pvQueueItemsBuff, AUDIO_PROC_THREAD_IN_QUEUE_SIZE);
  assert(TX_SUCCESS==status);

  /* allocate stack from system memory pool */
  pvStackStart = SysAlloc(AUDIO_ACQ_THREAD_STACK_SIZE);
  assert(pvStackStart != NULL);
  
  /* create audio acquisition thread  */
  status = tx_thread_create(&AudioAcqTask.thread,\
                            "audio_acq_thread",\
                            audio_acq_thread_func, 0,\
                            pvStackStart,\
                            AUDIO_ACQ_THREAD_STACK_SIZE,\
                            AUDIO_ACQ_THREAD_PRIO, AUDIO_ACQ_THREAD_PRIO,\
                            TX_NO_TIME_SLICE, TX_DONT_START);
  assert(TX_SUCCESS==status);

  /* and its input queue */
  pvQueueItemsBuff = SysAlloc(AUDIO_ACQ_THREAD_IN_QUEUE_SIZE);
  assert(pvQueueItemsBuff != NULL);
  status= tx_queue_create(&AudioAcqTask.queue, "audio_acq_in_queue",\
                          AUDIO_ACQ_THREAD_IN_QUEUE_ITEM_SIZE/sizeof(uint32_t),\
                          pvQueueItemsBuff, AUDIO_ACQ_THREAD_IN_QUEUE_SIZE);
  assert(TX_SUCCESS==status);

  status = tx_mutex_create(&AudioAcqTask.lock, NULL, TX_INHERIT);
  
  assert(TX_SUCCESS==status);

  /* allocate stack from system memory pool */
  pvStackStart = SysAlloc(LOAD_GEN_THREAD_STACK_SIZE);
  
  /* create load generation thread  */
  status = tx_thread_create(&LoadGenTask.thread,\
                            "load_gen_thread",\
                            load_gen_thread_func, 0,\
                            pvStackStart,\
                            LOAD_GEN_THREAD_STACK_SIZE,\
                            LOAD_GEN_THREAD_PRIO, LOAD_GEN_THREAD_PRIO,\
                            TX_NO_TIME_SLICE, TX_AUTO_START);
  assert(TX_SUCCESS==status);

  /* and its input queue */
  pvQueueItemsBuff = SysAlloc(LOAD_GEN_THREAD_IN_QUEUE_SIZE);
  assert(pvQueueItemsBuff != NULL);
  status=tx_queue_create(&LoadGenTask.queue, "load_gen_in_queue",\
                         LOAD_GEN_THREAD_IN_QUEUE_ITEM_SIZE/sizeof(uint32_t),\
                         pvQueueItemsBuff, LOAD_GEN_THREAD_IN_QUEUE_SIZE);
  assert(TX_SUCCESS==status);
}

/**
* @brief  Main program
* @param  None
* @retval None
*/
void tx_application_define(void *first_unused_memory)
{
  VOID *pvStackStart;
  UINT status;
  
  status = tx_byte_pool_create(&s_xTheSystem.m_xSysMemPool,\
                               "SYS_MEM_POOL", s_xTheSystem.m_pnHeap,\
                               sizeof(s_xTheSystem.m_pnHeap));
  assert(TX_SUCCESS==status);
  
  /* allocate stack from system memory pool) */
  pvStackStart = SysAlloc(INIT_THREAD_STACK_SIZE);
  assert(NULL!=pvStackStart);
  
  /* create main thread  */
  status = tx_thread_create(&init_thread, "init_thread", init_thread_func, 0,\
                            pvStackStart, INIT_THREAD_STACK_SIZE,\
                            INIT_THREAD_PRIO, INIT_THREAD_PRIO,\
                            TX_NO_TIME_SLICE, TX_AUTO_START);
  assert(TX_SUCCESS==status);
}
#ifdef APP_LP
void low_power_enter (void)
{
/*    HAL_SuspendTick(); */
    HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFI);
/*    HAL_ResumeTick();*/
}
#endif
