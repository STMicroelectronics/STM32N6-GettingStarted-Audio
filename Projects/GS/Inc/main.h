/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32n6xx_hal.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_xspi.h"
#include "stm32n6570_discovery_audio.h"
#include "app_config.h"
#include "system_clock_config.h"
#include "AudioCapture_ring_buff.h"
#include "tasks_def.h"

#ifndef PREPROC_FLOAT_16
#error only PREPROC_FLOAT_16 is supported in this version
#endif
#ifndef POSTPROC_FLOAT_16
#error only POSTPROC_FLOAT_16 is supported in this version
#endif

#endif /* MAIN_H */

