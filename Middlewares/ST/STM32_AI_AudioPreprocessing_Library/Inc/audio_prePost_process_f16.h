/**
 ******************************************************************************
 * @file    audio_prePost_process_f16.h
 * @author  MCD Application Team
 * @brief   Header for audio_prePost_process_f16.c module
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
#ifndef _AUDIO_PREPOST_PROCESS_F16_H
#define _AUDIO_PREPOST_PROCESS_F16_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "audio_prePost_process.h"
#include "feature_extraction_f16.h"

/**
 * @addtogroup groupSpeProcess
 * @{
 */

typedef struct
{
  FFTTypeDef_f16_t *pFFT;                    /*!< points toFFTType structure */
  float16_t *pScratch;                       /*!< points to the temporary calculation buffer of length FFTLen used as input modified by FFT */
  size_t hop_length;                         /*!< number of samples between frames */
  size_t nb_frames;                          /*!< numbers of frames in batch */
  size_t idxPos;                             /*!< in/out current index in nb_elems*/
  eSpectrogramTranspose transpose;           /*!< Transposed spectrogram flag */
  AudioSpENormalize_t *pNorm;                /*!< optional: normalize spectrum parameters */
} AudioToFFTBatch_f16_t;

typedef struct {
  FFTTypeDef_f16_t *pFFT;                    /*!< points to FFTType structure */
  float16_t *pScratch;                       /*!< points to the temporary calculation buffer of length FFTLen used as input modified by FFT */
  float16_t *pWeightsWindow;                 /*!< points to the temporary buffer of length FrameLen used to sum window weights */
  float16_t *pScratchOut;                    /*!< points to the temporary buffer of length FrameLen used temporary float output */
  size_t hop_length;                         /*!< number of samples between frames */
  size_t nb_frames;                          /*!< numbers of frames in batch */
  size_t idxPos;                             /*!< in/out index position in scratch window weights output*/
  eSpectrogramTranspose transposeSigIn;	     /*!< Transposed spectrogram flag */
  eSpectrogramTranspose transposeMask;	     /*!< Transposed spectrogram flag */
} AudioSpEPostProc_f16_t;

typedef struct {
  FFTTypeDef_f16_t *pFFT;                    /*!< points to FFTType structure */
  size_t nb_frames;                          /*!< numbers of frames in batch */
  eSpectrogramTranspose transposeIn;         /*!< Transposed spectrogram flag */
  eSpectrogramTranspose transposeOut;        /*!< Transposed spectrogram flag */
  eSpectrogram_TypeTypedef eType;            /*!< spectrum type */
  float16_t *pScratch;                       /*!< points to the temporary calculation buffer of length FFTLen used as input modified by FFT */
  quantParam_f16_t *pQuant;                  /*!< optional: quantization parameters */
  AudioSpENormalize_t *pNorm;                /*!< optional: normalize spectrum parameters */
}
AudioSpEExtraPre_f16_t;

void audio_is16oCplxFFTf16(const int16_t *pInSignal, AudioToFFTBatch_f16_t *S, float16_t *pOut);
void audio_iCplxFFTf16xf16os16(const float16_t *pFFT, const float16_t *pMask, AudioSpEPostProc_f16_t *S, int16_t **pInOut);
void audio_iCplxFFTf16oFFTx(const float16_t *pFFTData, AudioSpEExtraPre_f16_t *S, void *pOut);
/**
 * @} end of groupFeature
 */

#ifdef __cplusplus
}
#endif

#endif /* _AUDIO_PREPOST_PROCESS_F16_H */
