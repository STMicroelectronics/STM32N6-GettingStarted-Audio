/**
 ******************************************************************************
 * @file    data_in.c
 * @author  MCD Application Team
 * @brief   Audio input early process
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
#include "audio_din.h"

/**
 * @brief      Return float32 from Q15 input adding padding if any
 *
 * @param      *pInSignal points to input signal buffer
 * @param      *pOut      points to output buffer
 * @param      pad        left padding
 * @param      frame_len  signal length to keep
 * @param      pad_right  right padding
 * @return     None
 */
void audio_is16of32_pad(const int16_t *pInSignal, float32_t *pOut, size_t pad, size_t frame_len, size_t pad_right) {


  /* Zero pad left */
  memset(pOut, 0, pad*sizeof(*pOut));

  /* scale input signal */
  arm_q15_to_float( pInSignal, pOut + pad, frame_len);
    
  /* Zero pad right */
  memset(pOut + pad + frame_len, 0, pad_right*sizeof(*pOut));
}
