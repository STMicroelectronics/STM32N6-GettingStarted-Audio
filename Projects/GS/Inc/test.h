/**
  ******************************************************************************
  * @file    test.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  * @version V2.0.0
  * @date    02-May-2025
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TEST_H
#define TEST_H
/* Includes ------------------------------------------------------------------*/
#include "app_config.h"
#include "ai_dpu.h"
#include "preproc_dpu.h"       
/* Exported functions ------------------------------------------------------- */
extern void test_init(void);
extern bool test_probe_in(AIProcCtx_t * pAiCtx , AudioPreProcCtx_t *pAudioCtx);
extern bool test_probe_out(AIProcCtx_t * pAiCtx , AudioPreProcCtx_t *pAudioCtx);
extern void test_dump(void);
#endif /* TEST_H */

