/**
  ******************************************************************************
  * @file    window_f16.h
  * @author  MCD Application Team
  * @brief   Header for window_f16.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef __WINDOW_F16_H
#define __WINDOW_F16_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "arm_math.h"
#include "arm_math_types_f16.h"
#include "window.h"

#ifdef ARM_FLOAT16_SUPPORTED

/**
 * @addtogroup groupWindow
 * @{
 */

/**
 * @brief Window function types
 */
int32_t Window_f16_Init(float16_t *pDst, uint32_t len, eWindowTypedef type);

/**
 * @} end of groupWindow
 */

#ifdef __cplusplus
}
#endif

#endif

#endif /* __WINDOW_F16_H */

