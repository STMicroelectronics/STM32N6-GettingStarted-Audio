/**
  ******************************************************************************
  * @file    app_config.h
  * @author  GPM/AIS Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   APP configuration
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

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "logging.h"
#include "app_msg.h"

//#define LOG_LEVEL LOG_DEBUG
#define LOG_LEVEL LOG_INFO

#define INIT_TASK_CFG_HEAP_SIZE (60*1024)

#define INIT_THREAD_STACK_SIZE (4*1024)
#define INIT_THREAD_PRIO (0)

#define FREERTOS_AUDIO_PROC_THREAD_PRIO               (configMAX_PRIORITIES - 3)
#define FREERTOS_AUDIO_PROC_THREAD_STACK_SIZE         (configMINIMAL_STACK_SIZE * 4)  // example size
#define FREERTOS_AUDIO_PROC_THREAD_IN_QUEUE_SIZE      10U
#define FREERTOS_AUDIO_PROC_THREAD_IN_QUEUE_ITEM_SIZE (sizeof(AppMsg_t))

#define FREERTOS_AUDIO_ACQ_THREAD_PRIO               (configMAX_PRIORITIES - 4)
#define FREERTOS_AUDIO_ACQ_THREAD_STACK_SIZE         (configMINIMAL_STACK_SIZE * 4)
#define FREERTOS_AUDIO_ACQ_THREAD_IN_QUEUE_SIZE      10U
#define FREERTOS_AUDIO_ACQ_THREAD_IN_QUEUE_ITEM_SIZE (sizeof(AppMsg_t))

#define FREERTOS_LOAD_GEN_THREAD_PRIO                (configMAX_PRIORITIES - 2)
#define FREERTOS_LOAD_GEN_THREAD_STACK_SIZE          (configMINIMAL_STACK_SIZE * 4)
#define FREERTOS_LOAD_GEN_THREAD_IN_QUEUE_SIZE       10U
#define FREERTOS_LOAD_GEN_THREAD_IN_QUEUE_ITEM_SIZE  (sizeof(AppMsg_t))

/* in this version only  PREPROC_FLOAT_16 and  POSTPROC_FLOAT_16 are supported */
#define PREPROC_FLOAT_16
#define POSTPROC_FLOAT_16

#define my_printf LogInfo

/* UART usage/configuration */
#ifdef APP_DVFS
  #define USE_UART_BAUDRATE               (14400) /* 14400 is max value in DVFS mode */
#else
  #define USE_UART_BAUDRATE               (14400) /* can up set up upto 921600 */
#endif

#ifdef APP_BARE_METAL
#define APP_CONF_STR "Bare Metal"
#define CPU_STATS
#else
#define APP_CONF_STR "RTOS"
#endif

#define SEPARATION_LINE "------------------------------------------------------------\n\r"

#define USE_NPU_CACHE 1
/*
#define LOAD_GEN_NB_RUN     (100)
#define LOAD_GEN_TIME_SLICE (100)
#define LOAD_GEN_DUTY_CYCLE (20)
*/
/*
#define RECORD
#define PLAYBACK
*/
#if defined(RECORD) || defined(PLAYBACK)
#undef TEST
#define TEST
#define DUMP_NB 10
#endif

#endif /* __APP_CONFIG_H__ */
