/**
  ******************************************************************************
  * @file    audio_bm.h
  * @author  GPM/AIS Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   
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

#ifndef __APP_AUDIO_BM_H__
#define __APP_AUDIO_BM_H__
#include "stm32n6570_discovery.h"
#include "preproc_dpu.h"
#include "postproc_dpu.h"
#include "ai_dpu.h"

#if (POSTPROC_FLOAT_T!=float16_t)
#error only POSTPROC_FLOAT_16 is supported in this version
#endif
#if (PREPROC_FLOAT_T!=float16_t)
#error only PREPROC_FLOAT_16 is supported in this version
#endif

#define PLAYBACK_BUFFER_SIZE            (PATCH_LENGTH*4)
typedef struct _AudioBM_play_back_t
{
  AudioCapture_ring_buff_t ring_buff; 
  uint32_t cnt;
}AudioBM_play_back_t;

typedef struct _AudioBM_acq_t
{
  int16_t acq_buf[CAPTURE_BUFFER_SIZE];
  AudioCapture_ring_buff_t ring_buff; 
  uint32_t cnt;
}AudioBM_acq_t;

typedef struct _AudioBM_proc_t
{
  int16_t proc_buff[PATCH_LENGTH];
  int16_t audio_out[PATCH_LENGTH];
  AudioPreProcCtx_t audioPreCtx;
  AudioPostProcCtx_t audioPostCtx;
#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
  AudioBM_play_back_t  audioPlayBackCtx;
#endif
  AIProcCtx_t aiCtx;
  int8_t * ai_in_ptr;
  const LL_Buffer_InfoTypeDef * ai_out_ptr;
  uint32_t cnt;
}AudioBM_proc_t;

extern void init_bm(void);
extern void exec_bm(void);
extern void displaySystemSetting(void);
extern bool audio_process(AudioBM_acq_t * acq_ctx_ptr,\
                          AudioBM_proc_t * proc_ctx_ptr);
extern void initAudioCapture(AudioBM_acq_t *ctx_ptr);
extern void startAudioCapture(AudioBM_acq_t * ctx_ptr);
extern void stopAudioCapture(void);
extern void initAudioProc(AudioBM_proc_t * ctx_ptr);
extern void AudioCapture_half_buf_cb(AudioCapture_ring_buff_t *pHdle,\
                                      int16_t *pData, uint8_t half_buf);
extern void printInferenceResults(const LL_Buffer_InfoTypeDef* pBuffRes);
extern void toggle_audio_proc(void);
extern void printHeader(void);
#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
extern void stopAudioPlayBack(void);
#endif /* CTRL_X_CUBE_AI_AUDIO_OUT */
#ifdef APP_LP
extern void NPU_SRAM_on(void);
extern void NPU_SRAM_off(void);
extern void NPU_on(void);;
extern void NPU_off(void);
#endif /* APP_LP */

#endif /* __APP_AUDIO_BM_H__ */
