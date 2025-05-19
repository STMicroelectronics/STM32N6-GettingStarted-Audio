/**
  ******************************************************************************
  * @file    postproc_dpu.h
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
#ifndef _POSTPROC_DPU_H
#define _POSTPROC_DPU_H

/* Includes ------------------------------------------------------------------*/
#include "audio_proc.h"

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/
int PostProc_DPUInit_f16(AudioProcCtx_f16_t *pxCtx);
int PostProc_DPU_f16(AudioProcCtx_f16_t *pxCtx, PREPROC_FLOAT_T *pDataIn,
		float32_t *pSpecMask, int16_t *pAudioOut);
#define AudioPostProcCtx_t  AudioProcCtx_f16_t
#define PostProc_DPUInit    PostProc_DPUInit_f16
#define PostProc_DPU        PostProc_DPU_f16
#define PostProc_rfft_init  arm_rfft_fast_init_f16
#define audio_iCplxFFTos16  audio_iCplxFFTf16xf16os16
#define POSTPROC_FLOAT_T   float16_t

#endif /* _POSTPROC_DPU_H*/
