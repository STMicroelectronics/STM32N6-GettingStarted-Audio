/**
  ******************************************************************************
  * @file    feature_extraction_f16.c
  * @author  MCD Application Team
  * @brief   Spectral feature extraction functions
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
#include "feature_extraction_f16.h"
#include "audio_din_f16.h"
#define FEATURE_EXTRACTION_SQRT_F16_DONE_IN_F32

#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288 /*!< pi */
#endif

#ifdef ARM_FLOAT16_SUPPORTED
/**
 * @defgroup groupFeature Feature Extraction
 * @brief Spectral feature extraction functions
 * @{
 */

/**
 * @brief      Convert 16-bit PCM into floating point values
 *
 * @param      *pInSignal  points to input signal buffer
 * @param      *pOutSignal points to output signal buffer
 * @param      len         signal length
 */
void buf_to_float16(int16_t *pInSignal, float16_t *pOutSignal, uint32_t len)
{
  for (uint32_t i = 0; i < len; i++)
  {
    pOutSignal[i] = (float16_t) pInSignal[i];
  }
}

/**
 * @brief      Convert 16-bit PCM into normalized floating point values
 *
 * @param      *pInSignal   points to input signal buffer
 * @param      *pOutSignal  points to output signal buffer
 * @param      len          signal length
 */
void buf_to_float16_normed(int16_t *pInSignal, float16_t *pOutSignal, uint32_t len)
{
  for (uint32_t i = 0; i < len; i++)
  {
    pOutSignal[i] = (float16_t) pInSignal[i] / (1 << 15);
  }
}

/**
 * @brief      Power Spectrogram column
 *
 * @param      *pInSignal  points to the in-place input signal frame of length FFTLen.
 * @param      *pOutCol    points to  output Spectrogram column.
 * @param	   n_fft number of points in FFT
 * @param      eTtype spectrogram type (magnitude or squared)
 * @return     None
 */
#include "stdio.h"
void SpectrogramColumn_f16(float16_t *pInSignal, float16_t *pOutCol, size_t n_fft, eSpectrogram_TypeTypedef eType, float16_t *p_sum)
{
  float16_t first_energy;
  float16_t last_energy;

  /* Power spectrum */
  first_energy = pInSignal[0] * pInSignal[0];
  last_energy = pInSignal[1] * pInSignal[1];
  pOutCol[0] = first_energy;
  pOutCol[n_fft / 2] = last_energy;

#ifdef FEATURE_EXTRACTION_SQRT_F16_DONE_IN_F32
  arm_cmplx_mag_squared_f16(&pInSignal[2], &pOutCol[1], (n_fft / 2) - 1);
  
  if (eType == SPECTRUM_TYPE_MAGNITUDE) {
    for (size_t i = 0; i < (n_fft / 2) + 1; i++) {
      arm_sqrt_f16(pOutCol[i], &pOutCol[i]);
    }
  }
#else
  /* Magnitude spectrum */
  if (eType == SPECTRUM_TYPE_MAGNITUDE)
  {
    arm_sqrt_f16(pOutCol[0], &pOutCol[0]);
    
    arm_sqrt_f16(pOutCol[n_fft / 2], &pOutCol[n_fft / 2]);

    arm_cmplx_mag_f16(&pInSignal[2], &pOutCol[1], (n_fft / 2) - 1);

  } else {
    arm_cmplx_mag_squared_f16(&pInSignal[2], &pOutCol[1], (n_fft / 2) - 1);
  }
#endif
  
#ifdef CMSIS_DSP_VERSION_CORRECT
  arm_accumulate_f16umulate_f32(pOutCol, (n_fft / 2) + 1, p_sum);
#else
  for (size_t i = 0; i < (n_fft / 2) + 1; i++) {
    *p_sum += pOutCol[i];
  }
#endif

}

/**
 * @brief      Power Spectrogram column
 *
 * @param      *S          points to an instance of the floating-point Spectrogram structure.
 * @param      *pInSignal  points to the in-place input signal frame of length FFTLen/2.
 * @param      *pOutCol    points to  output Spectrogram column.
 * @return     None
 */
void SpectrogramOneColumn_f16(Spectrogram_f16_t *S, float16_t *pInSignal, float16_t *pOutCol)
{
  uint32_t n_fft = S->pFFT->FFTLen;

  float16_t first_energy;
  float16_t last_energy;
  float16_t *p_sum = &S->spectro_sum;

  /* Power spectrum */
  first_energy = pInSignal[0] * pInSignal[0];
  last_energy = pInSignal[1] * pInSignal[1];
  pOutCol[0] = first_energy;
  pOutCol[n_fft / 2] = last_energy;

  /* Magnitude spectrum */
  if (S->Type == SPECTRUM_TYPE_MAGNITUDE)
  {
    arm_sqrt_f16(pOutCol[0], &pOutCol[0]);
    arm_sqrt_f16(pOutCol[n_fft / 2], &pOutCol[n_fft / 2]);
    arm_cmplx_mag_f16(&pInSignal[2], &pOutCol[1], (n_fft / 2) - 1);
  } else {
  arm_cmplx_mag_squared_f16(&pInSignal[2], &pOutCol[1], (n_fft / 2) - 1);
  }
#ifdef CMSIS_DSP_VERSION_CORRECT
  arm_accumulate_f16umulate_f32(pOutCol, (n_fft / 2) + 1, p_sum);
#else
  for (size_t i = 0; i < (n_fft / 2) + 1; i++) {
    *p_sum += pOutCol[i];
  }
#endif
}

/**
 * @brief      Mel Spectrogram column
 *
 * @param      *S          points to an instance of the floating-point Mel structure.
 * @param      *pInSignal  points to input signal frame of length FFTLen.
 * @param      *pOutCol    points to  output Mel Spectrogram column.
 * @return     None
 */
void MelSpectrogramColumn_f16(MelSpectrogramTypeDef_f16 *S, float16_t * pInSignal, float16_t * pOutCol)
{
  float16_t *tmp_buffer = S->SpectrogramConf->pScratch1;

  /* Power Spectrogram */
  SpectrogramColumn_f16(pInSignal, tmp_buffer, S->SpectrogramConf->FFTLen, S->SpectrogramConf->Type, &(S->SpectrogramConf->spectro_sum));

  /* Mel Filter Banks Application */
  MelFilterbank_f16(S->MelFilter, tmp_buffer, pOutCol);
}

/**
 * @brief      Log-Mel Spectrogram column
 *
 * @param      *S          points to an instance of the floating-point Log-Mel structure.
 * @param      *pInSignal  points to input signal frame of length FFTLen.
 * @param      *pOutCol    points to  output Log-Mel Spectrogram column.
 * @return     None
 */
void LogMelSpectrogramColumn_f16(LogMelSpectrogramTypeDef_f16 *S, float16_t *pInSignal, float16_t *pOutCol)
{
  uint32_t n_mels = S->MelSpectrogramConf->MelFilter->NumMels;
  float16_t top_dB = S->TopdB;
  float16_t ref = S->Ref;
  SpectrogramTypeDef_f16 *p_spectro = S->MelSpectrogramConf->SpectrogramConf;
  float16_t *tmp_buffer = p_spectro->pScratch1;

  SpectrogramColumn_f16(pInSignal, tmp_buffer, p_spectro->FFTLen, p_spectro->Type, &(p_spectro->spectro_sum));

  /* Mel Filter Banks Application to power spectrum column */
  MelFilterbank_f16(S->MelSpectrogramConf->MelFilter, tmp_buffer, pOutCol);

  /* Scaling */
  for (uint32_t i = 0; i < n_mels; i++) {
    pOutCol[i] /= ref;
  }

  /* Avoid log of zero or a negative number */
  for (uint32_t i = 0; i < n_mels; i++) {
    if (pOutCol[i] <= 0.0f16) {
      pOutCol[i] = FLT_MIN;
    }
  }
  if (S->LogFormula == LOGMELSPECTROGRAM_SCALE_DB)
  {
    /* Convert power spectrogram to decibel */
    for (uint32_t i = 0; i < n_mels; i++) {
      pOutCol[i] = 10.0f16 * log10f(pOutCol[i]);
    }

    /* Threshold output to -top_dB dB */
    for (uint32_t i = 0; i < n_mels; i++) {
      pOutCol[i] = (pOutCol[i] < -top_dB) ? (-top_dB) : (pOutCol[i]);
    }
  }
  else
  {
    /* Convert power spectrogram to log scale */
    for (uint32_t i = 0; i < n_mels; i++) {
      pOutCol[i] = logf(pOutCol[i]);
    }
  }
}

/**
 * @brief      Return FFT (float16) from Q15 input 
                    returned FFT is packed to NFFT/2 samples
 *
 * @param      *pInSignal points to input signal buffer
 * @param      *pOut      points to output FFT
 * @param      *S         points to FFT structure
 * @return     None
 */
void audio_is16of16_fft(const int16_t *pInSignal, FFTTypeDef_f16_t *S, float16_t *pOut) 
{
  // Get one padded frame from int16 to float16
  audio_is16of16_pad(pInSignal, S->pScratch, S->sDin.pad_left, S->sDin.FrameLen, S->sDin.pad_right);
  /* In-place window application (on signal length, not entire n_fft) */
  /* @note: OK to typecast because hannWin content is not modified */
  if (NULL != S->pWindow) {
    arm_mult_f16(S->pScratch+S->sDin.pad_left, S->pWindow, S->pScratch+S->sDin.pad_left, S->sDin.FrameLen);
  }
  /* FFT */
  arm_rfft_fast_f16(S->pRfft, S->pScratch, pOut, 0);

}

/**
 * @brief      Compute Log Mel Spectrogram for one frame input
 *               (float16 intermediate values)
 *
 * @param      *S         points to LogMelSpectrogram structure
 * @param      *pInSignal points to input signal int16 buffer
 * @param      *pOutCol   points to output logmelspectrogram for the frame
 * @param      offset    quantization offset 
 * @param      inv_scale quantization inverse scale 

 * @return     None
 */
void LogMelSpectrogramColumn_q15_f16_Q8(LogMelSpectrogramTypeDef_f16 *S, const int16_t *pInSignal, int8_t *pOutCol, int8_t offset, float16_t inv_scale)
{
  float16_t *p_in, *p_out;
  SpectrogramTypeDef_f16 *p_spectro = S->MelSpectrogramConf->SpectrogramConf;
  FFTTypeDef_f16_t sFFT = { 
    .pRfft = p_spectro->pRfft,
    .pWindow = p_spectro->pWindow,
    .pScratch= p_spectro->pScratch1,
    .FFTLen = p_spectro->FFTLen,
    .sDin.FrameLen = p_spectro->FrameLen,
    .sDin.pad_left = p_spectro->pad_left,
    .sDin.pad_right = p_spectro->pad_right
  };
  
  FFTTypeDef_f16_t *pFFT = &sFFT;

  float16_t top_dB                  = S->TopdB;
  float16_t ref                     = S->Ref;
  uint32_t n_mels                   = S->MelSpectrogramConf->MelFilter->NumMels;

  // First scratch buffer for float data after optional windowing
  // Output of FFT in second scratch buffer
  audio_is16of16_fft(pInSignal, pFFT, p_spectro->pScratch2);

  /* swap input & output buffers*/
  p_in  = p_spectro->pScratch2;
  p_out = p_spectro->pScratch2;
  /* compute spectrogram  with second scratch buffer as input*/

  SpectrogramColumn_f16(p_in, p_out, p_spectro->FFTLen, p_spectro->Type, &(p_spectro->spectro_sum));

  /* swap input & output buffers*/
  p_in  = p_spectro->pScratch2;
  p_out = pFFT->pScratch;

  /* Mel Filter Banks Application to power spectrum column */
  MelFilterbank_f16(S->MelSpectrogramConf->MelFilter,p_in, p_out);

  /* Scaling */
  for (uint32_t i = 0; i < n_mels; i++) {
    p_out[i] /= ref;
  }

  /* Avoid log of zero or a negative number */
  for (uint32_t i = 0; i < n_mels; i++) {
    if (p_out[i] <= 0.0f16) {
      p_out[i] = FLT_MIN;
    }
  }

  if (S->LogFormula == LOGMELSPECTROGRAM_SCALE_DB)
  {
    /* Convert power spectrogram to decibel */
    for (uint32_t i = 0; i < n_mels; i++) {
      p_out[i] = 10.0f16 * log10f(p_out[i]);
    }

    /* Threshold output to -top_dB dB */
    for (uint32_t i = 0; i < n_mels; i++) {
      p_out[i] = (p_out[i] < -top_dB) ? (-top_dB) : (p_out[i]);
    }
  }
  else
  {
    /* Convert power spectrogram to log scale */
    for (uint32_t i = 0; i < n_mels; i++) {
      p_out[i] = logf(p_out[i]);
    }
  }

  /*  Quantization */
  for (uint32_t i=0 ; i < n_mels ; i++ ){
    pOutCol[i]= (int8_t)__SSAT((int16_t)roundf(p_out[i]*inv_scale + (float16_t) offset), 8);
  }
}


/**
 * @brief      Mel-Frequency Cepstral Coefficients (MFCCs) column
 *               (float16 intermediate values)
 * @param      *S          points to an instance of the floating-point MFCC structure.
 * @param      *pInSignal  points to input signal frame of length FFTLen.
 * @param      *pOutCol    points to  output MFCC spectrogram column.
 * @return     None
 */
void MfccColumn_f16(MfccTypeDef_f16 *S, float16_t *pInSignal, float16_t *pOutCol)
{
  float16_t *tmp_buffer = S->pScratch;

  LogMelSpectrogramColumn_f16(S->LogMelConf, pInSignal, tmp_buffer);

  /* DCT for computing MFCCs from spectrogram slice. */
  DCT_f16(S->pDCT, tmp_buffer, pOutCol);
}

/**
 * @brief      Compute Log Mel Spectrograms for nb_frames input 
               (Audio event detection preprocess)
 *
 * @param      *S         points to LogMelSpectrogram structure
 * @param      *pInSignal points to input signal int16 buffer
 * @param      *pOut      points to output logmelspectrogram for nb_frame

 * @return     None
 */
void LogMelSpectrogramColumn_q15_f16_os8_batch(LogMelSpectrogramArray_f16 *S, int16_t *pInSignal, int8_t *pOut){ 

  uint32_t n_mels = S->S_LogMelSpect->MelSpectrogramConf->MelFilter->NumMels;
  for (uint32_t i = 0; i < S->nb_frames; i++ )
  {
    // Process one column
    LogMelSpectrogramColumn_q15_f16_Q8(S->S_LogMelSpect, pInSignal, S->pScratchBuffer, S->output_Q_offset, S->output_Q_inv_scale);
    pInSignal += S->hop_length; 
    // transpose
    if (E_SPECTROGRAM_TRANSPOSE == S->transpose) {
      for (uint32_t j=0 ; j < n_mels ; j++ ){
        pOut[i+S->nb_frames*j]= S->pScratchBuffer[j];
      }
    } else {
      for (uint32_t j=0 ; j < n_mels ; j++ ){
        pOut[S->nb_frames*i+j]= S->pScratchBuffer[j];
      }
    }
  }

}

/**
 * @} end of groupFeature
 */

#endif
