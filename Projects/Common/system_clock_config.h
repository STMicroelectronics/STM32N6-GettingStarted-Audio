/**
  ******************************************************************************
  * @file    system_clock_config.h 
  * @author  GPM/AIS Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   system clock config
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

#ifndef __SYSTEM_CLOCK_CONFIG_H__
#define __SYSTEM_CLOCK_CONFIG_H__

#define SystemClock_Config_Full SystemClock_Config_HSI_no_overdrive
#define SystemClock_Config_Low  SystemClock_Config_MSI_4MHz_4MHz_exp

extern void SystemClock_Config_ResetClocks(void);
extern void SystemClock_Config_MSI_4MHz_4MHz_exp(void);
extern void SystemClock_Config_HSI_overdrive(void);
extern void SystemClock_Config_HSI_no_overdrive(void);

#endif /* __SYSTEM_CLOCK_CONFIG_H__ */
