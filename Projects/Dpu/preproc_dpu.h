/**
  ******************************************************************************
  * @file    preproc_dpu.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   Header for preproc_dpu.c module
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
#ifndef __PREPROC_DPU_H__
#define __PREPROC_DPU_H__

/* Includes ------------------------------------------------------------------*/
#include "audio_proc.h"

/* Exported constants --------------------------------------------------------*/
#define AUDIO_HALF_BUFF_SIZE (CTRL_X_CUBE_AI_SPECTROGRAM_PATCH_LENGTH*2)
#define AUDIO_BUFF_SIZE  (AUDIO_HALF_BUFF_SIZE*2)
#define PATCH_OVERLAP    (CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH-CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH)
#define PATCH_NO_OVERLAP (CTRL_X_CUBE_AI_SPECTROGRAM_COL*CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH)
#define PATCH_LENGTH     (PATCH_OVERLAP + PATCH_NO_OVERLAP)

/* Exported functions --------------------------------------------------------*/
int PreProc_DPUInit_f16(AudioProcCtx_f16_t *pxCtx);
int PreProc_DPU_f16(AudioProcCtx_f16_t *pxCtx, uint8_t *pDataIn, int8_t *p_spectro);

#define AudioPreProcCtx_t AudioProcCtx_f16_t
#define PreProc_DPUInit PreProc_DPUInit_f16
#define PreProc_DPU PreProc_DPU_f16
#define PreProc_rfft_init arm_rfft_fast_init_f16
#define LogMelSpectrogramColumn LogMelSpectrogramColumn_q15_f16_Q8
#define audio_is16oCplxFFT  audio_is16oCplxFFTf16
#define audio_iCplxFFToFFTx audio_iCplxFFTf16oFFTx
#define PREPROC_FLOAT_T float16_t

#endif /* __PREPROC_DPU_H__*/
