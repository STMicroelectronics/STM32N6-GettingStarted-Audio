/**
  ******************************************************************************
  * @file    mel_filterbank_f16.h
  * @author  MCD Application Team
  * @brief   Header for mel_filterbank_f16.c module
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
#ifndef __MEL_FILTERBANK_F16_H
#define __MEL_FILTERBANK_F16_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "arm_math.h"
#include "arm_math_types_f16.h"
#include "mel_filterbank.h"

#ifdef ARM_FLOAT16_SUPPORTED
#include "dsp/basic_math_functions_f16.h"

/**
 * @addtogroup groupMelFilterbank_f16
 * @{
 */

/**
 * @brief Instance structure for the floating-point MelFilterbank function.
 */
typedef struct
{
  uint32_t  *pStartIndices;    /*!< points to the mel filter pCoefficients start indexes */
  uint32_t  *pStopIndices;     /*!< points to the mel filter pCoefficients stop indexes */
  float16_t *pCoefficients;    /*!< points to the mel filter weights of length CoefficientsLength */
  uint32_t CoefficientsLength; /*!< (Set by Init) number pCoefficients elements */
  uint32_t NumMels;            /*!< number of Mel bands to generate. */
  uint32_t FFTLen;             /*!< number of input FFT points. */
  uint32_t SampRate;           /*!< input signal sampling rate. */
  eMelFormulaTypedef Formula;   /*!< Mel-Hz conversion formula type. */
  uint32_t Normalize;          /*!< if 0, leave all the triangles. Otherwise divide the triangular mel weights by the
                                    width of the mel band (area normalization) */
  uint32_t Mel2F; /*!< if 0, create bins in mel domain (TensorFlow). Otherwise, create bins in Hz domain (librosa) */
  float16_t FMin;              /*!< lowest frequency in Hz (typ. 0). */
  float16_t FMax;              /*!< highest frequency in Hz (typ. sr / 2.0). */
} MelFilterTypeDef_f16;


void MelFilterbank_f16_Init(MelFilterTypeDef_f16 *MelFilterStruct);
void MelFilterbank_f16(MelFilterTypeDef_f16 *M, float16_t *pSpectrCol, float16_t *pMelCol);

/**
 * @} end of groupMelFilterbank
 */

#ifdef __cplusplus
}
#endif

#endif

#endif /* __MEL_FILTERBANK_F16_H */

