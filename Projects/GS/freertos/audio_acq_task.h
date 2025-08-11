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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "audio_bm.h"

typedef struct _AudioAcqTask_t
{
  TaskHandle_t      thread;  /* FreeRTOS task handle */
  QueueHandle_t     queue;   /* FreeRTOS queue handle */
  SemaphoreHandle_t lock;    /* FreeRTOS mutex handle */
  StaticSemaphore_t lock_buffer;  // FreeRTOS Memory buffer for mutex internal data
  AudioBM_acq_t     ctx;     /* Audio acquisition context */
} AudioAcqTask_t;

/* Declare the global AudioAcqTask instance */
extern AudioAcqTask_t AudioAcqTask;

/* Task function prototype adapted for FreeRTOS */
void audio_acq_thread_func(void *arg);

#endif /* __APP_AUDIO_ACQ_TASK_H__ */
