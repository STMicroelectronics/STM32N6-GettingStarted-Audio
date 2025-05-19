/**
  ******************************************************************************
  * @file    audio_acq_task.h
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

#ifndef __APP_AUDIO_ACQ_TASK_H__
#define __APP_AUDIO_ACQ_TASK_H__
#include "stm32n6570_discovery.h"
#include "tx_api.h"

typedef struct _AudioAcqTask_t
{
  TX_THREAD thread;
  TX_QUEUE queue;
  TX_MUTEX lock;
  AudioBM_acq_t ctx;
}AudioAcqTask_t;

extern void audio_acq_thread_func(ULONG arg);
extern AudioAcqTask_t AudioAcqTask;

#endif /* __APP_AUDIO_ACQ_TASK_H__ */
