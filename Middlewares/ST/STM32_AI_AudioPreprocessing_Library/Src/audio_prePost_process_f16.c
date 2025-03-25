/**
 ******************************************************************************
 * @file    audio_prePost_process_f16.c
 * @author  MCD Application Team
 * @brief   Spectral feature extraction functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */
#include "audio_prePost_process_f16.h"

/**
 * @brief      Compute complex FFT for nb_frames input
 *              (speech enhancement preprocess)
 *              (float16 intermediate values)
 *
 * @param      *pInSignal points to input signal int16 buffer
 * @param      *S         points to AudioToFFTBatch structure
 * @param      *pOut      points to FFT buffer for nb_frames
 * @param      nb_iter    number of frames to be processed (max is nb_frames)

 * @return     None
 */
void audio_is16oCplxFFTf16(const int16_t *pInSignal, AudioToFFTBatch_f16_t *S, float16_t *pOut) {
  FFTTypeDef_f16_t *pFFT = S->pFFT;
  size_t nFFT = pFFT->FFTLen;
  uint32_t _idx;

  if (S->pNorm) {
    arm_absmax_q15(pInSignal, (S->nb_frames - 1)*S->hop_length + S->pFFT->sDin.FrameLen, (int16_t *)&S->pNorm->audioMax, &_idx);
  }
  for (size_t frame = 0; frame < S->nb_frames; frame++ )
  {
    /* Audio to Cplx FFT Packed format: all data one nFFT
     * (Re[0] = Re[0], Im[0] = Re[nFFT]) */
    audio_is16of16_fft(pInSignal, pFFT, S->pScratch);
    pInSignal += S->hop_length;

    /* Manage output */
    /*****************/
    // transpose
    if ( (E_SPECTROGRAM_CPLX_FFT_TRANSPOSE == S->transpose)
      || (E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE == S->transpose) ) {

      if (E_SPECTROGRAM_CPLX_FFT_TRANSPOSE == S->transpose) {
      // First
      pOut[2*frame + 0 ] = S->pScratch[0];
      pOut[2*frame + 1 ]  = 0.0f16;
      // Last
      pOut[2*frame + S->nb_frames * nFFT]     = S->pScratch[1];
      pOut[2*frame + S->nb_frames * nFFT + 1] = 0.0f16;
      }
      else if (E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE == S->transpose) {
        pOut[2*frame + 0 ] = S->pScratch[0];
        pOut[2*frame + 1]  = S->pScratch[1];
      }

      // all others
      for (size_t j = 1 ; j <nFFT/2; j++ ){
        pOut[2*(frame + S->nb_frames*j) + 0] = S->pScratch[2*j+0];
        pOut[2*(frame + S->nb_frames*j) + 1] = S->pScratch[2*j+1];
      }
    }
    // No transpose
    else {
      size_t loop_step = 0; // Should be updaded in one of the 2 if block

      if (E_SPECTROGRAM_CPLX_FFT == S->transpose) {
        loop_step = nFFT + 2;
      // First
      pOut[frame * loop_step + 0 ] = S->pScratch[0];
      pOut[frame * loop_step + 1 ] = 0.0f16;
      // Last
      pOut[frame * loop_step + nFFT + 0] = S->pScratch[1];
      pOut[frame * loop_step + nFFT + 1] = 0.0f16;
      }
      if (E_SPECTROGRAM_CPLX_FFT_PACK == S->transpose) {
        loop_step = nFFT;
        pOut[frame * loop_step + 0 ] = S->pScratch[0];
        pOut[frame * loop_step + 1 ] = S->pScratch[1];
      }
      // all others
      for (size_t j = 2 ; j < nFFT; j+=2 ){
        pOut[frame * loop_step + j + 0] = S->pScratch[j + 0];
        pOut[frame * loop_step + j + 1] = S->pScratch[j + 1];
      }
    }
  }
}

/**
 * @brief      Compute complex FFT for nb_frames input
               (speech enhancement posprocess)
 *
 * @param      *pFFTData  points to complex float FFT buffer
 * @param      *S         points to AudioSpEExtraPre_t structure
 * @param      *pOut      points to output of this extra preprocessing

 * @return     None
 */
void audio_iCplxFFTf16oFFTx(const float16_t *pFFTData, AudioSpEExtraPre_f16_t *S, void *pOut)
{
  size_t nFFT = S->pFFT->FFTLen;
  size_t nb_frames = S->nb_frames;

  for (size_t frame = 0; frame < S->nb_frames; frame++) {
    float16_t *pScratch = S->pScratch;

    /* Manage input */
    /****************/
    // Transpose
    if ( (E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE == S->transposeIn)
      || (E_SPECTROGRAM_CPLX_FFT_TRANSPOSE == S->transposeIn)) {

      pScratch = S->pScratch;

      if (E_SPECTROGRAM_CPLX_FFT_TRANSPOSE == S->transposeIn) {
        // Handle first/last
          pScratch[0]      = pFFTData[2*frame + 0];
          pScratch[1]      = pFFTData[2*frame + nFFT * nb_frames];
      }
      else if (E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE == S->transposeIn) {
        // Handle first/last
        pScratch[0] = pFFTData[2*frame + 0];
        pScratch[1] = pFFTData[2*frame + 1];
      }
      for (size_t i = 1; i < nFFT / 2; i++){
        pScratch[2*i    ] = pFFTData[2*(frame + i * nb_frames)    ];
        pScratch[2*i + 1] = pFFTData[2*(frame + i * nb_frames) + 1];
      }
    }
    else
    // No transpose
    {
      size_t loop_step = 0;

      if (E_SPECTROGRAM_CPLX_FFT == S->transposeIn) {
        loop_step = 2 * ( nFFT/ 2 + 1);
        pScratch[0] = pFFTData[frame * loop_step];
        pScratch[1] = pFFTData[frame * loop_step + nFFT];
      }
      else if (E_SPECTROGRAM_CPLX_FFT_PACK == S->transposeIn) {
        loop_step = 2 * ( nFFT/ 2);
        pScratch[0] = pFFTData[frame * loop_step];
        pScratch[1] = pFFTData[frame * loop_step + 1];
      }
      memcpy(&pScratch[2], &pFFTData[frame*loop_step + 2], 2 * (nFFT/2 - 1) * sizeof(float16_t));
    }

    float16_t sumf16;
    switch (S->eType) {
      case SPECTRUM_TYPE_POWER:
      case SPECTRUM_TYPE_MAGNITUDE:
        SpectrogramColumn_f16(pScratch, pScratch, nFFT, S->eType, &sumf16);
        break;
      default:
      break;
    }
    // Normalize if required
    if (S->pNorm) {
      if ((S->pNorm->maxNormValue != 0) && (S->pNorm->audioMax > S->pNorm->minThreshold) ){
        float16_t normFactor = (float16_t)S->pNorm->maxNormValue / (float16_t)S->pNorm->audioMax;
        arm_scale_f16(pScratch, normFactor, pScratch, nFFT/2 + 1);
      }
    }
    // Internal format is packed
    if (NULL == S->pQuant) {
    // output float16_t
      float16_t *pDataOut = (float16_t *)pOut;
      switch (S->transposeOut) {
        case E_SPECTROGRAM_TRANSPOSE:
         for (size_t i = 0; i < nFFT/2 + 1; i++) {
            pDataOut[frame + i*nb_frames]= pScratch[i];
          }
          break;
        case E_SPECTROGRAM_TRANSPOSE_NO:
         for (size_t i = 0; i < nFFT/2 + 1; i++) {
            pDataOut[frame * ( nFFT/2 + 1) + i ]= pScratch[i];
          }
        break;
        case E_SPECTROGRAM_CPLX_FFT_TRANSPOSE:
          // TBD CEN
        case E_SPECTROGRAM_CPLX_FFT:
          // TBD CEN
        case E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE:
          // TBD CEN
        case E_SPECTROGRAM_CPLX_FFT_PACK:
          // TBD CEN
        default:
          //Trouble
        break;
      }
     }
    else {
      // output int8_t
      int8_t *pDataOut = (int8_t *)pOut;
      float16_t inv_scale = S->pQuant->output_Q_inv_scale;
      int8_t offset = S->pQuant->output_Q_offset;
      switch (S->transposeOut) {
        case E_SPECTROGRAM_TRANSPOSE:
          for (size_t i = 0; i < nFFT/2 + 1; i++) {
            pDataOut[frame + i*nb_frames]= (int8_t)__SSAT((int32_t)roundf(pScratch[i]*inv_scale + (float) offset), 8);
          }
          break;
        case E_SPECTROGRAM_TRANSPOSE_NO:
          for (size_t i = 0; i < nFFT/2 + 1; i++) {
            pDataOut[frame * (nFFT/2 + 1) + i]= (int8_t)__SSAT((int32_t)roundf(pScratch[i]*inv_scale + (float) offset), 8);
          }
        break;
        case E_SPECTROGRAM_CPLX_FFT_TRANSPOSE:
          // TBD CEN
        case E_SPECTROGRAM_CPLX_FFT:
          // TBD CEN
        case E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE:
          // TBD CEN
        case E_SPECTROGRAM_CPLX_FFT_PACK:
          // TBD CEN
        default:
          //Trouble
        break;
      }
    }
  } // for frames
}
/**
 * @brief      Compute complex FFT for nb_frames input
 *              (speech enhancement posprocess)
 *              (float16 intermediate values)
 *
 * @param      *pFFTData  points to complex float FFT buffer (also model input)
 * @param      *pMask     points to mask to be applied (Model output)
 * @param      *S         points to AudioSpEPostProc structure
 * @param      **pInOut   points to audio int16 buffer for nb_frames
 * @param      nb_iter    number of frames to be processed (max is nb_frames)

 * @return     None
 */
void audio_iCplxFFTf16xf16os16(const float16_t *pFFTDataIn, const float16_t *pMask, AudioSpEPostProc_f16_t *S, int16_t **pInOut)
{
    int16_t *pOut = *pInOut;
    FFTTypeDef_f16_t *pFFT = S->pFFT;
    float16_t *pWeightsWindow = S->pWeightsWindow;
    float16_t *pScatchOut = S->pScratchOut;
    float16_t *pData = S->pScratch+pFFT->sDin.pad_left;
    float16_t *pWindow = pFFT->pWindow;
    const float16_t *pFFTData = pFFTDataIn;
    size_t idxW = S->idxPos;
    size_t nFFT = pFFT->FFTLen;
    size_t nb_frames = S->nb_frames;

    for (size_t frame = 0; frame < S->nb_frames; frame++) {

        /* Apply Mask and write to scratch buffer */
        /******************************************/
        float16_t *pScratch = pFFT->pScratch;
        // Element wise multiply
        if ( ( (E_SPECTROGRAM_CPLX_FFT_TRANSPOSE == S->transposeSigIn) ||
               (E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE == S->transposeSigIn) ) &&
             (E_SPECTROGRAM_TRANSPOSE          == S->transposeMask) )
        {
          if (E_SPECTROGRAM_CPLX_FFT_TRANSPOSE == S->transposeSigIn) {
            pScratch[0] = pFFTData[0] * pMask[0];
            pScratch[1] = pFFTData[(nb_frames)*(nFFT)] * pMask[(nb_frames)*(nFFT/2)];
          }
          if (E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE == S->transposeSigIn) {
            pScratch[0] = pFFTData[0] * pMask[0];
            pScratch[1] = pFFTData[nb_frames] * pMask[(nb_frames)*(nFFT/2)];
          }
            for (size_t j = 1 ; j < nFFT/2; j++ ) {
              pScratch[2*j+0] = pFFTData[nb_frames*2*j+0] * pMask[nb_frames*j];
              pScratch[2*j+1] = pFFTData[nb_frames*2*j+1] * pMask[nb_frames*j];
            }
            pFFTData+=2; // 2 - Cplx
            pMask++;
        }
        else if ( ( (E_SPECTROGRAM_CPLX_FFT_TRANSPOSE == S->transposeSigIn) ||
                    (E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE == S->transposeSigIn) ) &&
                  (E_SPECTROGRAM_TRANSPOSE_NO       == S->transposeMask) )
        {
          if (E_SPECTROGRAM_CPLX_FFT_TRANSPOSE == S->transposeSigIn) {
            pScratch[0] = pFFTData[0] * pMask[0];
            pScratch[1] = pFFTData[(nb_frames)*(nFFT)] * pMask[nFFT/2];
          }
          if (E_SPECTROGRAM_CPLX_FFT_PACK_TRANSPOSE == S->transposeSigIn) {
            pScratch[0] = pFFTData[0] * pMask[0];
            pScratch[1] = pFFTData[nb_frames] * pMask[nFFT/2];
          }
            for (size_t j = 1 ; j < nFFT/2; j++ ) {
              pScratch[2*j+0] = pFFTData[nb_frames*2*j+0] * pMask[j];
              pScratch[2*j+1] = pFFTData[nb_frames*2*j+1] * pMask[j];
            }
            pFFTData+=2; // 2 - Cplx
            pMask    += (nFFT/2 + 1);
        }
        else if ( ( (E_SPECTROGRAM_CPLX_FFT      == S->transposeSigIn) ||
                    (E_SPECTROGRAM_CPLX_FFT_PACK == S->transposeSigIn) ) &&
                  (E_SPECTROGRAM_TRANSPOSE_NO == S->transposeMask) )
        {
          if (E_SPECTROGRAM_CPLX_FFT      == S->transposeSigIn) {
            pScratch[0] = pFFTData[0] * pMask [0];
            pScratch[1] = pFFTData[nFFT] * pMask[nFFT/2];
          }
          if (E_SPECTROGRAM_CPLX_FFT_PACK      == S->transposeSigIn) {
            pScratch[0] = pFFTData[0] * pMask [0];
            pScratch[1] = pFFTData[1] * pMask[nFFT/2];
          }
            for (size_t j = 1 ; j < nFFT/2; j++ ) {
              pScratch[2*j+0] = pFFTData[2*j+0] * pMask [j];
              pScratch[2*j+1] = pFFTData[2*j+1] * pMask [j];
            }
            pFFTData += (nFFT + 2);
            pMask    += (nFFT/2 + 1);
        }
        else if ( ( (E_SPECTROGRAM_CPLX_FFT  == S->transposeSigIn) ||
                    (E_SPECTROGRAM_CPLX_FFT_PACK == S->transposeSigIn) )  &&
                  (E_SPECTROGRAM_TRANSPOSE == S->transposeMask) )
        {
          if (E_SPECTROGRAM_CPLX_FFT      == S->transposeSigIn) {
            pScratch[0] = pFFTData[0] * pMask [0];
            pScratch[1] = pFFTData[nFFT] * pMask[nb_frames*nFFT/2];
          }
          if (E_SPECTROGRAM_CPLX_FFT_PACK      == S->transposeSigIn) {
            pScratch[0] = pFFTData[0] * pMask [0];
            pScratch[1] = pFFTData[1] * pMask[nb_frames*nFFT/2];
          }
            for (size_t j = 1 ; j < nFFT/2; j++ ) {
              pScratch[2*j+0] = pFFTData[2*j+0] * pMask [nb_frames*j];
              pScratch[2*j+1] = pFFTData[2*j+1] * pMask [nb_frames*j];
            }
            pFFTData += (nFFT + 2);
            pMask    ++;
        }
        else
        {
            // Undefined
            return;
        }

        // ISFFT
        /* Reverse FFT */
        /***************/

        arm_rfft_fast_f16(pFFT->pRfft, pScratch, S->pScratch, 1);

        /* Windowing */
        /*************/
        pData = S->pScratch + pFFT->sDin.pad_left;
        pWindow = pFFT->pWindow;
        if (NULL !=pWindow) {
            arm_mult_f16(pData, pWindow, pData, pFFT->sDin.FrameLen);
        }

        /* Previous data not impact by overlapping */
        for (size_t i = 0; i < pFFT->sDin.pad_left; i++) {
            float16_t convert;
            if ( 0.0f16 == S->pWeightsWindow[idxW]) {
                convert = 0.0f16;
            } else {
                convert = pScatchOut[idxW] / S->pWeightsWindow[idxW];
                convert = convert * 32768.0f16;
                convert += convert > 0.0f16 ? 0.5f16 : -0.5f16;
            }
            *pOut = (q15_t) (__SSAT((q31_t) (convert), 16));
            pScatchOut[idxW] = 0.0f16;
            S->pWeightsWindow[idxW] = 0.0f16;
            pOut++;
            idxW++;
            if (idxW >= (pFFT->sDin.FrameLen)) {
                idxW=0;
            }
        }
        // apply windowing correction
        // overlap-add + normalisation(2 times windowed: preproc + upper)
        pData = S->pScratch + pFFT->sDin.pad_left;
        pWindow = pFFT->pWindow;
        for (size_t i =  pFFT->sDin.pad_left; i < S->hop_length; i++) {
            float16_t convert;
            S->pWeightsWindow[idxW] += (*pWindow) * (*pWindow);
            pScatchOut[idxW] += *pData;
            if ( 0.0f16 == S->pWeightsWindow[idxW]) {
                convert = 0.0f16;
            } else {
                convert = pScatchOut[idxW] / S->pWeightsWindow[idxW];
                convert = convert * 32768.0f16;
                convert += convert > 0.0f16 ? 0.5f16 : -0.5f16;
            }
            *pOut = (q15_t) (__SSAT((q31_t) (convert), 16));
            pScatchOut[idxW] = 0.0f16;
            S->pWeightsWindow[idxW] = 0.0f16;
            pOut++;
            idxW++;
            if (idxW >= (pFFT->sDin.FrameLen)) {
                idxW=0;
            }
            pData++;
            pWindow++;
        }

        // data will be overlapped next call
        for (size_t i = 0; i < pFFT->sDin.FrameLen - S->hop_length; i++) {
            pWeightsWindow[(idxW+i)%pFFT->sDin.FrameLen] += (*pWindow) * (*pWindow);
            pScatchOut[(idxW+i)%pFFT->sDin.FrameLen] += (*pData);
            pData++;
            pWindow++;
        }
    } // for frames
    // Final
    #if 0
    arm_f16_to_q15(&pScatchOut[idxW],pOut,S->pFFT->sDin.FrameLen-S->hop_length);
    pOut += (pFFT->sDin.FrameLen-S->hop_length);
    idxW += (pFFT->sDin.FrameLen-S->hop_length);
    if (idxW>=pFFT->sDin.FrameLen) {
        idxW=0;
    }
    #endif
    S->idxPos = idxW;
    *pInOut = pOut;
}
/**
 * @} end of groupFeature
 */
