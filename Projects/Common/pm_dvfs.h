/**
  ******************************************************************************
  * @file    pm_dvfs.h 
  * @author  GPM/AIS Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   dvfs implementation
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __PM_DVFS_H__
#define __PM_DVFS_H__

#ifndef APP_BARE_METAL
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#ifdef APP_DVFS
typedef enum _opp_t {
  OPP_MIN = 0,
  OPP_MAX
} opp_t;

typedef struct _dvfs_ctx_t
{
#ifndef APP_BARE_METAL
  SemaphoreHandle_t lock;    /* FreeRTOS mutex handle */
  StaticSemaphore_t lock_buffer;  // FreeRTOS Memory buffer for mutex internal data
#endif /* APP_BARE_METAL */
  opp_t opp;
  uint32_t cpu_freq;
  int32_t cnt;
}dvfs_ctx_t;
extern void pm_init_dvfs(void);
extern void pm_set_opp_min(opp_t opp);
#endif /* APP_DVFS */

#endif /* __PM_DVFS_H__*/
