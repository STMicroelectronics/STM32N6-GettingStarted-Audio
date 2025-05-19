/**
  ******************************************************************************
  * @file    audio_proc.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   Header for postproc_dpu.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_PROC_H__
#define __AUDIO_PROC_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ai_device_adaptor.h"
#include "dpu_config.h"
#include "feature_extraction_f16.h"
#include "mel_filterbank_f16.h"
#include "audio_prePost_process_f16.h"
#include "feature_extraction.h"
#include "mel_filterbank.h"
#include "audio_prePost_process.h"

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
  SPECTROGRAM_BYPASS,
  SPECTROGRAM_MEL,
  SPECTROGRAM_LOG_MEL,
  SPECTROGRAM_MFCC
}spectrogram_type_t;

typedef struct {
  spectrogram_type_t           type;
  arm_rfft_fast_instance_f16   S_Rfft;
  arm_rfft_fast_instance_f16   S_iRfft;
  AudioSpENormalize_t          AudioNorm;
  FFTTypeDef_f16_t             S_STFT;
  AudioToFFTBatch_f16_t        S_STFT_b;
  AudioSpEExtraPre_f16_t       S_STFT_b_extra;
  FFTTypeDef_f16_t             S_iSTFT;
  AudioSpEPostProc_f16_t       S_iSTFT_b;
  SpectrogramTypeDef_f16       S_Spectr;
  MelSpectrogramTypeDef_f16    S_MelSpectr;
  LogMelSpectrogramTypeDef_f16 S_LogMelSpectr;
  MelFilterTypeDef_f16         S_MelFilter;
  float16_t                    pSpectrScratchBuffer1[CTRL_X_CUBE_AI_SPECTROGRAM_NFFT];
  float16_t                    pSpectrScratchBuffer2[CTRL_X_CUBE_AI_SPECTROGRAM_NFFT];
  float16_t                    pSpectrScratchBuffer3[CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH];
  float16_t                    pSpectrScratchBuffer4[CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH];
  float16_t                    pCplxSpectrum[(CTRL_X_CUBE_AI_SPECTROGRAM_NFFT/2 + 1) * 2 * CTRL_X_CUBE_AI_SPECTROGRAM_COL];
  DCT_InstanceTypeDef          S_DCT;
  MfccTypeDef                  S_Mfcc;
  int8_t                       output_Q_offset;
  float16_t                    output_Q_inv_scale;
  quantParam_f16_t             quant;
}AudioProcCtx_f16_t;

#endif /* __AUDIO_PROC_H__*/
