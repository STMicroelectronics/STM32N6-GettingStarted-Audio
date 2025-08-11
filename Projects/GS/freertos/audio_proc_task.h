/**
  ******************************************************************************
  * @file    audio_proc_task.h
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

#ifndef __APP_AUDIO_PROC_TASK_H__
#define __APP_AUDIO_PROC_TASK_H__

#include "audio_bm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct _AudioProcTask_t
{
  TaskHandle_t    thread;   /* FreeRTOS task handle */
  QueueHandle_t   queue;    /* FreeRTOS queue handle */
  AudioBM_proc_t  ctx;      /* Audio processing context */
} AudioProcTask_t;

/* Declare the global AudioProcTask instance */
extern AudioProcTask_t AudioProcTask;

/* Task function prototype adapted for FreeRTOS */
void audio_proc_thread_func(void *arg);

#endif /* __APP_AUDIO_PROC_TASK_H__ */
