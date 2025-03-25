/**
  ******************************************************************************
  * @file    feature_extraction_f16.h
  * @author  MCD Application Team
  * @brief   Header for feature_extraction_f16.c module
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

#ifndef _FEATURE_EXTRACTION_F16_H
#define _FEATURE_EXTRACTION_F16_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "arm_math.h"
#include "arm_math_types_f16.h"
#include "feature_extraction.h"

#include "common_tables_f16.h"
#include "dct_f16.h"
#include "mel_filterbank_f16.h"
#include "window_f16.h"

#ifdef ARM_FLOAT16_SUPPORTED
#include "dsp/support_functions_f16.h"
#include "dsp/transform_functions_f16.h"
#include "dsp/complex_math_functions_f16.h"

/**
 *\mainpage STM32 float16_t Audio Preprocessing Library
 *
 * Introduction
 * ------------
 * This STM32_AI_AudioPreprocessing_Library is a C Library for
 * audio preprocessing in Artificial Intelligence applications optimized for
 * STM32 devices. It provides the building blocks for spectral analysis and
 * feature extraction, such as:
 * - Spectrogram computation
 * - Mel-scaled and LogMel-scaled spectrogram computation
 * - Mel-frequency cepstral coefficients (MFCCs) computation
 *
 *
 * Toolchain Support
 * ------------
 *
 * The library has been developed and tested with IAR version 8.20
 *
 *
 */

/**
 * @addtogroup groupFeature
 * @{
 */
typedef struct
{
  arm_rfft_fast_instance_f16 *pRfft;         /*!< points to the real FFT instance */
  float16_t *pWindow;                        /*!< points to the window function. The length must be equal to FrameLen. */
  float16_t *pScratch;                       /*!< point to the temporary calculation buffer of length FFTLen used as input modified by FFT */
  uint32_t FFTLen;                           /*!< length of the real FFT. */
  DataInTypeDef sDin;						 /*!< structure managing input data */
} FFTTypeDef_f16_t;

/**
 * @brief Instance structure for the floating-point Spectrogram function.
 */
typedef struct
{
  arm_rfft_fast_instance_f16 *pRfft;         /*!< points to the real FFT instance */
  eSpectrogram_TypeTypedef Type;              /*!< spectrum type */
  float16_t *pWindow;                        /*!< points to the window function. The length must be equal to FrameLen. */
  uint32_t SampRate;                         /*!< sampling rate of the input signal. */
  size_t FrameLen;                         /*!< length of the input signal. */
  size_t FFTLen;                           /*!< length of the real FFT. */
  size_t pad_left;                         /*!< zero padding on the left of the window . */
  size_t pad_right;                        /*!< zero padding on the right of the window . */
  float16_t *pScratch1;                      /*!< point to the first temporary calculation buffer of length FFTLen */
  float16_t *pScratch2;                      /*!< point to the second temporary calculation buffer of length FFTLen */
  float16_t spectro_sum;                     /*!< sum of spectrogram components */
} SpectrogramTypeDef_f16;

/**
 * @brief Instance structure for the floating-point Spectrogram function.
 */
typedef struct
{
  FFTTypeDef_f16_t *pFFT;                     /*!< pointer to FFT structure */
  eSpectrogram_TypeTypedef Type;              /*!< spectrum type */
  size_t SampRate;                            /*!< sampling rate of the input signal. */
  float16_t *pScratch;                        /*!< point to the second temporary calculation buffer of length FFTLen */
  float16_t spectro_sum;                      /*!< sum of spectrogram components */
} Spectrogram_f16_t;

/**
 * @brief Instance structure for the floating-point MelSpectrogram function.
 */
typedef struct
{
  SpectrogramTypeDef_f16 *SpectrogramConf;       /*!< points to the Spectrogram instance */
  MelFilterTypeDef_f16 *MelFilter;               /*!< points to the MelFilter instance */
} MelSpectrogramTypeDef_f16;

/**
 * @brief Instance structure for the floating-point Log-MelSpectrogram function.
 */
typedef struct
{
  MelSpectrogramTypeDef_f16 *MelSpectrogramConf; /*!< points to the Mel-Spectrogram instance */
  eLogMelSpectrogram_ScaleTypedef     LogFormula; /*!< returned mel energy scale (dB or Log) */
  float16_t Ref;                                 /*!< scaling for the input signal (typ 1.0).
                                                  Input signal is scaled by 1.0 / Ref. */
  float16_t TopdB;                               /*!< threshold the ouput to -TopdB when dB scaled is used (typ. 80.0 dB) */
} LogMelSpectrogramTypeDef_f16;

typedef struct
{
    LogMelSpectrogramTypeDef_f16 *S_LogMelSpect;    /*!< points to the LogMelSpect instance */
    int8_t *pScratchBuffer;                         /*!< to Handle one LogMelSpectrogram */
    size_t nb_frames;                               /*!< number of frames in a batch */
    size_t hop_length;                              /*!< number of samples by which to slide the window at each step*/
    eSpectrogramTranspose transpose;                /*!< transpoise output flag */
    float16_t output_Q_inv_scale;                   /*!< Quantization inverse scale 
                                                        OutS8 = Outf32 * QinvScale + Qoffset */
    int8_t output_Q_offset;                         /*!< Quantization offset */
}LogMelSpectrogramArray_f16;

typedef struct
{
    float16_t output_Q_inv_scale;               /*!< Quantization inverse scale 
                                                    OutS8 = Outf32 * QinvScale + Qoffset */
    int8_t output_Q_offset;                     /*!< Quantization offset */
} quantParam_f16_t;
/**
 * @brief Instance structure for the floating-point Mfcc function.
 */
typedef struct
{
  LogMelSpectrogramTypeDef_f16 *LogMelConf;      /*!< points to the LogMel instance */
  DCT_InstanceTypeDef_f16 *pDCT;                 /*!< points to the DCT instance. */
  uint32_t NumMfccCoefs;                         /*!< number of mel coefficients to return. */
  float16_t *pScratch;                           /*!< points to the temporary calculation buffer of length NumMels */
} MfccTypeDef_f16;

/* Utilities */
void buf_to_float16(int16_t *pInSignal, float16_t *pOutSignal, uint32_t len);
void buf_to_float16_normed(int16_t *pInSignal, float16_t *pOutSignal, uint32_t len);

/* Column calculation functions */
void SpectrogramColumn_f16(float16_t *pInSignal, float16_t *pOutCol, size_t n_fft, eSpectrogram_TypeTypedef eType, float16_t *p_sum);
void SpectrogramOneColumn_f16(Spectrogram_f16_t *S, float16_t *pInSignal, float16_t *pOutCol);
void SpectrogramOneColumn_f16(Spectrogram_f16_t *S, float16_t *pInSignal, float16_t *pOutCol);
void MelSpectrogramColumn_f16(MelSpectrogramTypeDef_f16 *S, float16_t *pInSignal, float16_t *pOutCol);
void LogMelSpectrogramColumn_f16(LogMelSpectrogramTypeDef_f16 *S, float16_t *pInSignal, float16_t *pOutCol);
void LogMelSpectrogramColumn_q15_f16_Q8(LogMelSpectrogramTypeDef_f16 *S, const int16_t *pInSignal, int8_t *pOutCol, int8_t offset, float16_t inv_scale);
void MfccColumn_f16(MfccTypeDef_f16 *S, float16_t *pInSignal, float16_t *pOutCol);
void audio_is16of16_fft(const int16_t *pInSignal, FFTTypeDef_f16_t *S, float16_t *pOut);
void LogMelSpectrogramColumn_q15_f16_os8_batch(LogMelSpectrogramArray_f16 *S, int16_t *pInSignal, int8_t *pOut);

/**
 * @} end of groupFeature
 */

#ifdef __cplusplus
}
#endif

#endif

#endif /* _FEATURE_EXTRACTION_F16_H */
