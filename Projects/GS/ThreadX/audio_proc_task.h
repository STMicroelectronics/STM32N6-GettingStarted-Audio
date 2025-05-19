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

typedef struct _AudioProcTask_t
{
  TX_THREAD      thread;
  TX_QUEUE       queue;
  AudioBM_proc_t ctx;
}AudioProcTask_t;

extern AudioProcTask_t AudioProcTask;
extern void audio_proc_thread_func(ULONG arg);

#endif /* __APP_AUDIO_PROC_TASK_H__ */
