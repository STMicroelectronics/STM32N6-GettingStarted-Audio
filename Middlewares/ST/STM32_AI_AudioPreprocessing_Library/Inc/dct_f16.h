/**
  ******************************************************************************
  * @file    dct_f16.h
  * @author  MCD Application Team
  * @brief   Header for dct_f16.c module
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

#ifndef __DCT_F16_H
#define __DCT_F16_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "arm_math.h"
#include "arm_math_types_f16.h"
#include "dct.h"

#ifdef ARM_FLOAT16_SUPPORTED

/**
 * @addtogroup groupDCT
 * @{
 */

/**
 * @brief Instance structure for the floating-point DCT functions.
 */
typedef struct
{
  uint32_t NumFilters;     /*!< output length (e.g. number of MFCCs). */
  uint32_t NumInputs;      /*!< input signal length (e.g. number of mel bands). */
  eDCT_TypeTypeDef Type;    /*!< DCT type and normalization mode. */
  uint32_t RemoveDCTZero;  /*!< compute DCT of length NumFilters + 1 and return the last NumFilters points. */
  float16_t *pDCTCoefs;    /*!< points to the cosFactor table of length (NumFilters * NumInputs) elements. */
} DCT_InstanceTypeDef_f16;

int32_t DCT_f16_Init(DCT_InstanceTypeDef_f16 *S);
void DCT_f16(DCT_InstanceTypeDef_f16 *S, float16_t *pIn, float16_t *pOut);

/**
 * @} end of groupDCT
 */

#ifdef __cplusplus
}
#endif
#endif

#endif /* __DCT_F16_H */

