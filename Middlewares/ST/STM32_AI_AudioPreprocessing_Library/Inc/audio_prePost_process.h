/**
 ******************************************************************************
 * @file    audio_prePost_process.h
 * @author  MCD Application Team
 * @brief   Header for audio_spe_process.c module
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
#ifndef _AUDIO_PREPOST_PROCESS_H
#define _AUDIO_PREPOST_PROCESS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "feature_extraction.h"

/**
 * @addtogroup groupSpeProcess
 * @{
 */
 typedef struct
{
  int32_t maxNormValue;                      /*!< Max normalized value : 2**15 - 1 for full range */
  int32_t audioMax;                          /*!< Max audio sample value (used for normalization) */
  int32_t minThreshold;                      /*!< Min value for audioMax for which normalization would be applied */
} AudioSpENormalize_t;


#define AUDIO_PREPROC_NORMALIZATION_THRESHOLD (0.1f)
typedef enum {
  AUDIO_PROC_EXTRA_NORMALIZE,
  AUDIO_PROC_EXTRA_NORMALIZE_DISABLE,
}eAudioProcessNormDis;

typedef struct
{
  FFTTypeDef_t *pFFT;                        /*!< points to FFTType structure */
  float32_t *pScratch;                       /*!< points to the temporary calculation buffer of length FFTLen used as input modified by FFT */
  size_t hop_length;                         /*!< number of samples between frames */
  size_t nb_frames;                          /*!< numbers of frames in batch */
  eSpectrogramTranspose transpose;           /*!< Transposed spectrogram flag */
  AudioSpENormalize_t *pNorm;
} AudioToFFTBatch_t;

typedef struct {
  FFTTypeDef_t *pFFT;                        /*!< points to FFTType structure */
  float32_t *pScratch;                       /*!< points to the temporary calculation buffer of length FFTLen used as input modified by FFT */
  float32_t *pWeightsWindow;                 /*!< points to the temporary buffer of length FrameLen used to sum window weights */
  float32_t *pScratchOut;                    /*!< points to the temporary  buffer of length FrameLen used temporary float output */
  size_t hop_length;                         /*!< number of samples between frames */
  size_t nb_frames;                          /*!< numbers of frames in batch */
  size_t idxPos;                             /*!< in/out index position in scratch window weights output*/
  eSpectrogramTranspose transposeSigIn;	     /*!< Transposed spectrogram flag */
  eSpectrogramTranspose transposeMask;	     /*!< Transposed spectrogram flag */
} AudioSpEPostProc_t;

typedef struct {
  FFTTypeDef_t *pFFT;                        /*!< points to FFTType structure */
  size_t nb_frames;                          /*!< numbers of frames in batch */
  eSpectrogramTranspose transposeIn;         /*!< Transposed spectrogram flag */
  eSpectrogramTranspose transposeOut;        /*!< Transposed spectrogram flag */
  eSpectrogram_TypeTypedef eType;            /*!< spectrum type */
  float32_t *pScratch;                       /*!< points to the temporary calculation buffer of length FFTLen used as input modified by FFT */
  quantParam_t *pQuant;                      /*!< optional: quantization parameters */
  AudioSpENormalize_t *pNorm;
}
AudioSpEExtraPre_t;

void audio_is16oCplxFFTf32( const int16_t *pInSignal, AudioToFFTBatch_t *S, float32_t *pOut);
void audio_iCplxFFTf32xf32os16(const float32_t *pFFT, const float32_t *pMask, AudioSpEPostProc_t *S, int16_t **pInOut);
void audio_iCplxFFTf32oFFTx(const float32_t *pFFTData, AudioSpEExtraPre_t *S, void *pOut);
/**
 * @} end of groupFeature
 */

#ifdef __cplusplus
}
#endif

#endif /* _AUDIO_PREPOST_PROCESS_H */
