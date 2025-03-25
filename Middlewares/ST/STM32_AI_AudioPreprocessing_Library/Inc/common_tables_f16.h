/**
  ******************************************************************************
  * @file    common_tables_f16.h
  * @author  MCD Application Team
  * @brief   Header for common_tables_f16.c module
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
  
#ifndef _COMMON_TABLES_F16_H
#define _COMMON_TABLES_F16_H

#include "arm_math.h"
#include "arm_math_types_f16.h"
#include "common_tables.h"

#ifdef ARM_FLOAT16_SUPPORTED

extern const float16_t hannWin_1024_f16[1024];
extern const float16_t hannWin_2048_f16[2048];
extern const float16_t hammingWin_1024_f16[1024];
extern const float16_t blackmanWin_1024_f16[1024];

extern const float16_t melFilterLut_1024_30_f16[968];

extern const float16_t melFilterLut_2048_128_f16[2020];
#endif
#endif /* _COMMON_TABLES_F16_H */

