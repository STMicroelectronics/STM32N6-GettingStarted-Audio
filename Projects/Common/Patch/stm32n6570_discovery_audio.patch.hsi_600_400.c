/**
  ******************************************************************************
  * @file    stm32n6570_discovery_audio_uc6.c
  * @author  MCD Application Team
  * @version $Version$
  * @date    $Date$
  * @brief   This file provides the Audio driver for the STM32N6570-DK board.
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
   *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32n6570_discovery_audio.h"
#include "stm32n6570_discovery_bus.h"


/**
  * @brief  MDF1 clock Config.
  * @param  hmdf MDF handle.
  * @param  SampleRate Audio sample rate used to record the audio stream.
  * @retval HAL status.
  */
HAL_StatusTypeDef MX_MDF1_ClockConfig(MDF_HandleTypeDef *hmdf, uint32_t SampleRate)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hmdf);

  HAL_StatusTypeDef         status = HAL_OK;
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL4.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL4.PLLFractional = 0;
  RCC_OscInitStruct.PLL4.PLLM = 8;  /* 64/8 = 8MHz */
  if ((SampleRate == AUDIO_FREQUENCY_11K) || (SampleRate == AUDIO_FREQUENCY_22K) || (SampleRate == AUDIO_FREQUENCY_44K))
  {
    RCC_OscInitStruct.PLL4.PLLN = 192; /* 8*192 = 1536MHz */
    RCC_OscInitStruct.PLL4.PLLP1 = 4;
    RCC_OscInitStruct.PLL4.PLLP2 = 2; /* 1536/8 = 192 MHZ */
  }
  else if (SampleRate == AUDIO_FREQUENCY_96K)
  {
    RCC_OscInitStruct.PLL4.PLLN = 172; /* 8*172 = 1376MHz */
    RCC_OscInitStruct.PLL4.PLLP1 = 2;
    RCC_OscInitStruct.PLL4.PLLP2 = 1; /* 1376/2 = 688MHz */
  }
  else /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_32K, AUDIO_FREQUENCY_48K */
  {
    RCC_OscInitStruct.PLL4.PLLN = 172; /* 8*172 = 1376MHz */
    RCC_OscInitStruct.PLL4.PLLP1 = 7;
    RCC_OscInitStruct.PLL4.PLLP2 = 4; /* 1376/28 = 49.142MHz */
  }
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_MDF1;
  PeriphClkInitStruct.Mdf1ClockSelection = RCC_MDF1CLKSOURCE_IC8;
  PeriphClkInitStruct.ICSelection[RCC_IC8].ClockSelection = RCC_ICCLKSOURCE_PLL4;
  if ((SampleRate == AUDIO_FREQUENCY_11K) || (SampleRate == AUDIO_FREQUENCY_22K) || (SampleRate == AUDIO_FREQUENCY_44K))
  {
    PeriphClkInitStruct.ICSelection[RCC_IC8].ClockDivider = 17;
  }
  else if (SampleRate == AUDIO_FREQUENCY_96K)
  {
    PeriphClkInitStruct.ICSelection[RCC_IC8].ClockDivider = 7;
  }
  else /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_32K, AUDIO_FREQUENCY_48K */
  {
    PeriphClkInitStruct.ICSelection[RCC_IC8].ClockDivider = 1;
  }
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  return status;
}

/**
  * @brief  SAI1 clock Config.
  * @param  hsai SAI handle.
  * @param  SampleRate Audio sample rate used to play the audio stream.
  * @note   The SAI clock configuration *** IS DONE USING  HSI  ***
  * @retval HAL status.
  */
HAL_StatusTypeDef MX_SAI1_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t SampleRate)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  HAL_StatusTypeDef         ret = HAL_OK;
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*  WE ASSUME THAT HSI is RUNNING   */
  /* if not the PLL setting will fail */
 
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL1.PLLState  = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState  = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState  = RCC_PLL_NONE;

  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL4.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL4.PLLFractional = 0;
  RCC_OscInitStruct.PLL4.PLLM = 8; /* 64/8 = 8MHz */
  if ((SampleRate == AUDIO_FREQUENCY_11K) || (SampleRate == AUDIO_FREQUENCY_22K) ||
      (SampleRate == AUDIO_FREQUENCY_44K))
  {
    RCC_OscInitStruct.PLL4.PLLN = 192; /* 8*192 = 1536MHz */
    RCC_OscInitStruct.PLL4.PLLP1 = 4;
    RCC_OscInitStruct.PLL4.PLLP2 = 2; /* 1536/8 = 192 MHZ */
  }
  else  /* 8K, 16K, , 24K, 32K, 48K or 96K */
  {
    RCC_OscInitStruct.PLL4.PLLN = 172; /* 8*172 = 1376MHz */
    RCC_OscInitStruct.PLL4.PLLP1 = 7;
    RCC_OscInitStruct.PLL4.PLLP2 = 4; /* 1376/28 = 49.142MHz */
  }
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_IC7;
  PeriphClkInitStruct.ICSelection[RCC_IC7].ClockSelection = RCC_ICCLKSOURCE_PLL4;
  if ((SampleRate == AUDIO_FREQUENCY_11K) || (SampleRate == AUDIO_FREQUENCY_22K) ||
      (SampleRate == AUDIO_FREQUENCY_44K))
  {
    PeriphClkInitStruct.ICSelection[RCC_IC7].ClockDivider = 17;
  }
  else
  {
    PeriphClkInitStruct.ICSelection[RCC_IC7].ClockDivider = 1;
  }
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}
