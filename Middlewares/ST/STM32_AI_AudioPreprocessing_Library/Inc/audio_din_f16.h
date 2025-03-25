/**
 ******************************************************************************
 * @file    audio_din_f16.h
 * @author  MCD Application Team
 * @brief   Header for audio_din_f16.c module
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
#ifndef _AUDIO_DIN_F16_H_
#define _AUDIO_DIN_F16_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "arm_math_f16.h"

/**
 * @addtogroup audio_din_f16
 * @{
 */
void audio_is16of16_pad(const int16_t *pInSignal, float16_t *pOut, size_t pad, size_t frame_len, size_t pad_right);

/**
 * @} end of audio_din_f16
 */

#ifdef __cplusplus
}
#endif

#endif /* _AUDIO_DIN_F16_H_ */
