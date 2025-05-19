/**
  ******************************************************************************
  * @file    load_gen_task.h
  * @author  GPM/AIS Application Team
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

#ifndef __LOAD_GEN_TASK_H__
#define __LOAD_GEN_TASK_H__
#include "stm32n6570_discovery.h"
#include "tx_api.h"

#ifndef LOAD_GEN_NB_RUN
  #define LOAD_GEN_NB_RUN     (100)
#endif

#ifndef LOAD_GEN_TIME_SLICE
  #define LOAD_GEN_TIME_SLICE (100)
#endif

#ifndef LOAD_GEN_DUTY_CYCLE
  #define LOAD_GEN_DUTY_CYCLE (20)
#endif

typedef struct _LoadGenTask_t
{
  TX_THREAD thread;
  TX_QUEUE queue;
  uint32_t nbRun;
  uint32_t time_slice;
  float duty_cycle;
}LoadGenTask_t;

extern void load_gen_thread_func(ULONG arg);
extern LoadGenTask_t LoadGenTask;

#endif /* __LOAD_GEN_TASK_H__ */
