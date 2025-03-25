/**
 ******************************************************************************
 * @file    audio_din.h
 * @author  MCD Application Team
 * @brief   Header for audio_din.c module
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
#ifndef _AUDIO_DIN_H_
#define _AUDIO_DIN_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "arm_math.h"
#include "feature_extraction.h"


/**
 * @addtogroup audio_din
 * @{
 */
void audio_is16of32_pad(const int16_t *pInSignal, float32_t *pOut, size_t pad, size_t frame_len, size_t pad_right);


/**
 * @} end of audio_din
 */

#ifdef __cplusplus
}
#endif

#endif /* _AUDIO_DIN_H_ */
