/**
  ******************************************************************************
  * @file    tasks_def.h
  * @author  STMicroelectronics AIS application team
  * @version V2.0.0
  * @date    02-May-2025
  *
  * @brief
  *
  * <DESCRIPTIOM>
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TASKS_DEF_H
#define _TASKS_DEF_H

#ifndef APP_BARE_METAL
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "init_task.h"
// #include "audio_preproc_task.h"
// #include "audio_ai_task.h"
// #include "motion_preproc_task.h"
// #include "motion_ai_task.h"

#define INIT_TASK_PRIORITY              (configMAX_PRIORITIES)
#define INIT_TASK_STACK_SIZE            (configMINIMAL_STACK_SIZE)
#define MOTION_AI_TASK_PRIORITY         (6)
#define MOTION_AI_TASK_STACK_SIZE       (configMINIMAL_STACK_SIZE)
#define MOTION_PRE_PROC_TASK_PRIORITY   (10)
#define MOTION_PRE_PROC_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE)
#define AUDIO_AI_TASK_PRIORITY          (6)
#define AUDIO_AI_TASK_STACK_SIZE        (configMINIMAL_STACK_SIZE)
#define AUDIO_PRE_PROC_TASK_PRIORITY    (10)
#define AUDIO_PRE_PROC_TASK_STACK_SIZE  (configMINIMAL_STACK_SIZE)

#define EVT_DMA_HALF                    ( 1<<0 )
#define EVT_DMA_CPLT                    ( 1<<1 )
#define EVT_PREPROC_CPLT                ( 1<<2 )
#define EVT_EXIT                        ( 1<<3 )
#define EVT_PROC_INIT                   ( 1<<4 )
#define EVT_FIFO_FULL                   ( 1<<5 )

extern void vInitTask( void * pvArgs );
#endif
#endif /* _TASKS_DEF_H */
