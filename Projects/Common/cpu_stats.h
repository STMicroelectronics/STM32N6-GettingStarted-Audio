/**
  ******************************************************************************
  * @file    cpu_stats.h
  * @version V2.0.0
  * @date    02-May-2025
  * @brief
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
#ifndef __CPU_STATS_H
#define __CPU_STATS_H

#ifdef __cplusplus
extern "C" {
#endif
 

/* Includes ------------------------------------------------------------------*/
  
/* Exported macro ------------------------------------------------------------*/
/* Define the maximum number of log entries */
#define TIME_STAT_PRE_PROC   (0)
#define TIME_STAT_AI_PROC    (1)
#define TIME_STAT_POST_PROC  (2)
#define TIME_STAT_NB_ENTRIES (TIME_STAT_POST_PROC + 1)

void time_stats_init(void);
void time_stats_reset(unsigned int idx);
void time_stats_store(unsigned int idx, float time_in_ms);
float time_stats_get_avg(unsigned int idx);
float time_stats_get_min(unsigned int idx);
float time_stats_get_max(unsigned int idx);
float time_stats_get_latest(unsigned int idx);
unsigned int time_stats_get_cnt(unsigned int idx);

#ifdef __cplusplus
}
#endif

#endif /* __CPU_STATS_H */
