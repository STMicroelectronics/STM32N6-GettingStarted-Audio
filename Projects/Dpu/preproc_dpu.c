/**
  ******************************************************************************
  * @file    preproc_dpu.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   This file is implementing pre-processing functions that are making
  *          use of Audio pre-processing libraries
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

/* Includes ------------------------------------------------------------------*/


#include "preproc_dpu.h"
#include "user_mel_tables.h"
#include "logging.h"
#include "mcu_cache.h"
#ifdef APP_BARE_METAL
#include "cpu_stats.h"
#endif

int PreProc_DPUInit(AudioPreProcCtx_t *pCtx)
{
	printf("\n\rPreprocessing\n\r");
	printf(SEPARATION_LINE);
#if ( CTRL_X_CUBE_AI_PREPROC == CTRL_AI_SPECTROGRAM_LOG_MEL )  
  assert_param(CTRL_X_CUBE_AI_SPECTROGRAM_NFFT >= CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH );
  assert_param(CTRL_X_CUBE_AI_SPECTROGRAM_NFFT >= CTRL_X_CUBE_AI_SPECTROGRAM_NMEL);
  uint32_t pad = CTRL_X_CUBE_AI_SPECTROGRAM_NFFT - CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH;
  
  PreProc_rfft_init(&pCtx->S_Rfft, CTRL_X_CUBE_AI_SPECTROGRAM_NFFT);
  pCtx->type = SPECTROGRAM_LOG_MEL;
  pCtx->S_Spectr.pRfft                    = &pCtx->S_Rfft;
  pCtx->S_Spectr.Type                     = CTRL_X_CUBE_AI_SPECTROGRAM_TYPE;
  pCtx->S_Spectr.pWindow                  = (PREPROC_FLOAT_T *) CTRL_X_CUBE_AI_SPECTROGRAM_WIN;
  pCtx->S_Spectr.SampRate                 = CTRL_X_CUBE_AI_SENSOR_ODR;
  pCtx->S_Spectr.FrameLen                 = CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH;
  pCtx->S_Spectr.FFTLen                   = CTRL_X_CUBE_AI_SPECTROGRAM_NFFT;
  pCtx->S_Spectr.pScratch1                = pCtx->pSpectrScratchBuffer1;
  pCtx->S_Spectr.pScratch2                = pCtx->pSpectrScratchBuffer2;
  pCtx->S_Spectr.pad_left                 = pad/2;
  pCtx->S_Spectr.pad_right                = pad/2 + (pad & 1);
  pCtx->S_MelFilter.pStartIndices         = (uint32_t *) CTRL_X_CUBE_AI_SPECTROGRAM_MEL_START_IDX;
  pCtx->S_MelFilter.pStopIndices          = (uint32_t *) CTRL_X_CUBE_AI_SPECTROGRAM_MEL_STOP_IDX;
  pCtx->S_MelFilter.pCoefficients         = (PREPROC_FLOAT_T *) CTRL_X_CUBE_AI_SPECTROGRAM_MEL_LUT;
  pCtx->S_MelFilter.NumMels               = CTRL_X_CUBE_AI_SPECTROGRAM_NMEL;
  pCtx->S_MelFilter.FFTLen                = CTRL_X_CUBE_AI_SPECTROGRAM_NFFT;
  pCtx->S_MelFilter.SampRate              = CTRL_X_CUBE_AI_SENSOR_ODR;
  pCtx->S_MelFilter.FMin                  = CTRL_X_CUBE_AI_SPECTROGRAM_FMIN;
  pCtx->S_MelFilter.FMax                  = CTRL_X_CUBE_AI_SPECTROGRAM_FMAX;
  pCtx->S_MelFilter.Formula               = CTRL_X_CUBE_AI_SPECTROGRAM_FORMULA;
  pCtx->S_MelFilter.Normalize             = CTRL_X_CUBE_AI_SPECTROGRAM_NORMALIZE;
  pCtx->S_MelFilter.Mel2F                 = 1U;
  pCtx->S_MelSpectr.SpectrogramConf       = &pCtx->S_Spectr;
  pCtx->S_MelSpectr.MelFilter             = &pCtx->S_MelFilter;
  pCtx->S_LogMelSpectr.MelSpectrogramConf = &pCtx->S_MelSpectr;
  pCtx->S_LogMelSpectr.LogFormula         = CTRL_X_CUBE_AI_SPECTROGRAM_LOG_FORMULA;
  pCtx->S_LogMelSpectr.Ref                = 1.0f;
  pCtx->S_LogMelSpectr.TopdB              = HUGE_VALF;
  printf("MEL spectrogram %d mel x %d col\n\r",CTRL_X_CUBE_AI_SPECTROGRAM_NMEL,CTRL_X_CUBE_AI_SPECTROGRAM_COL);
  printf("- sampling freq : %u Hz\n\r",(uint32_t)CTRL_X_CUBE_AI_SENSOR_ODR);
  printf("- acq period    : %u ms\n\r",(uint32_t)CTRL_X_CUBE_AI_ACQ_LENGTH_MS);
  printf("- window length : %u samples\n\r",CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH);
  printf("- hop length    : %u samples\n\r",CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH);

 #elif ( CTRL_X_CUBE_AI_PREPROC == CTRL_AI_STFT )
  assert_param(CTRL_X_CUBE_AI_SPECTROGRAM_NFFT >= CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH );
  uint32_t pad = CTRL_X_CUBE_AI_SPECTROGRAM_NFFT - CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH;

  PreProc_rfft_init(&pCtx->S_Rfft, CTRL_X_CUBE_AI_SPECTROGRAM_NFFT);

  /* Init FFT */
  pCtx->S_STFT.pRfft                       = &pCtx->S_Rfft;
  pCtx->S_STFT.FFTLen                      = CTRL_X_CUBE_AI_SPECTROGRAM_NFFT;
  pCtx->S_STFT.pScratch                    = pCtx->pSpectrScratchBuffer1;
  pCtx->S_STFT.pWindow                     = (PREPROC_FLOAT_T *) CTRL_X_CUBE_AI_SPECTROGRAM_WIN;
  pCtx->S_STFT.sDin.FrameLen               = CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH;
  pCtx->S_STFT.sDin.pad_left               = pad/2;
  pCtx->S_STFT.sDin.pad_right              = (pad+1)/2;
  pCtx->S_STFT_b.pFFT                      = &pCtx->S_STFT;
  pCtx->S_STFT_b.pScratch                  = pCtx->pSpectrScratchBuffer2;
  pCtx->S_STFT_b.hop_length                = CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH;
  pCtx->S_STFT_b.nb_frames                 = CTRL_X_CUBE_AI_SPECTROGRAM_COL;
  pCtx->S_STFT_b.transpose                 = E_SPECTROGRAM_CPLX_FFT_TRANSPOSE;
  /* extra */
  pCtx->S_STFT_b_extra.eType               = SPECTRUM_TYPE_MAGNITUDE;
  pCtx->S_STFT_b_extra.nb_frames           = pCtx->S_STFT_b.nb_frames;
  pCtx->S_STFT_b_extra.pQuant              = &pCtx->quant;
  pCtx->S_STFT_b_extra.pFFT                = &pCtx->S_STFT;
  pCtx->S_STFT_b_extra.transposeIn         = E_SPECTROGRAM_CPLX_FFT_TRANSPOSE;
  pCtx->S_STFT_b_extra.transposeOut        = E_SPECTROGRAM_TRANSPOSE;
  pCtx->S_STFT_b_extra.pScratch            = pCtx->pSpectrScratchBuffer2;
  pCtx->AudioNorm.maxNormValue             = INT16_MAX ;
  pCtx->S_STFT_b.pNorm                     = &pCtx->AudioNorm ;
  pCtx->S_STFT_b_extra.pNorm               = &pCtx->AudioNorm ;
  pCtx->AudioNorm.minThreshold             = 2000;

  printf("STFT %u x %u\n\r",CTRL_X_CUBE_AI_SPECTROGRAM_NFFT/2 + 1,CTRL_X_CUBE_AI_SPECTROGRAM_COL);
  printf("- sampling freq : %lu Hz\n\r",(uint32_t)CTRL_X_CUBE_AI_SENSOR_ODR);
  printf("- acq period    : %lu ms\n\r",CTRL_X_CUBE_AI_ACQ_LENGTH_MS);
  printf("- window length : %u samples\n\r",CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH);
  printf("- hop length    : %u samples\n\r",CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH);
  printf("- overlap       : %u + %u columns\n\r",CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL,CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL);
#endif /* ( CTRL_X_CUBE_AI_PREPROC == CTRL_AI_STFT )*/

  return 0;
}

int PreProc_DPU(AudioPreProcCtx_t * pxCtx, uint8_t *pDataIn, int8_t *p_spectro)
{
  assert_param(pxCtx != NULL);
  assert_param(pDataIn != NULL);
  assert_param(p_spectro != NULL);

#ifdef APP_BARE_METAL
  port_dwt_reset();
#endif

#if (CTRL_X_CUBE_AI_PREPROC==CTRL_AI_SPECTROGRAM_LOG_MEL )
  int8_t  out[CTRL_X_CUBE_AI_SPECTROGRAM_NMEL];
  int16_t *p_in;
  pxCtx->S_Spectr.spectro_sum = 0 ;
  /* Create a quantized Mel-scaled spectrogram column */
  for (uint32_t i = 0; i < CTRL_X_CUBE_AI_SPECTROGRAM_COL; i++ )
  {
	p_in = (int16_t *)pDataIn + CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH * i;
	LogMelSpectrogramColumn(&pxCtx->S_LogMelSpectr, p_in,out,pxCtx->output_Q_offset,pxCtx->output_Q_inv_scale);
	/* transpose */
	for (uint32_t j=0 ; j < pxCtx->S_MelFilter.NumMels ; j++ ){
	  p_spectro[i+CTRL_X_CUBE_AI_SPECTROGRAM_COL*j]= out[j];
	}
  }
  mcu_cache_clean_invalidate_range((uint32_t)p_spectro, (uint32_t)(p_spectro+
		  CTRL_X_CUBE_AI_SPECTROGRAM_NMEL*CTRL_X_CUBE_AI_SPECTROGRAM_COL));
#elif (CTRL_X_CUBE_AI_PREPROC==CTRL_AI_STFT)
  audio_is16oCplxFFT((int16_t *)pDataIn, &pxCtx->S_STFT_b, pxCtx->pCplxSpectrum);
  audio_iCplxFFToFFTx(pxCtx->pCplxSpectrum, &pxCtx->S_STFT_b_extra,p_spectro);
  mcu_cache_clean_invalidate_range((uint32_t)p_spectro,(uint32_t)(p_spectro+
		  (CTRL_X_CUBE_AI_SPECTROGRAM_NFFT/2 + 1) * CTRL_X_CUBE_AI_SPECTROGRAM_COL )) ;
#endif /* (CTRL_X_CUBE_AI_PREPROC==CTRL_AI_STFT) */  

#ifdef APP_BARE_METAL
  time_stats_store(TIME_STAT_PRE_PROC,port_dwt_get_cycles()*1000.0F/port_hal_get_cpu_freq());
#endif

  return 1;
}
