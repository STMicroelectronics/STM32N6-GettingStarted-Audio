/**
  ******************************************************************************
  * @file    stm32n6570_discovery_audio.patch.msi_4_4.c
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

static const MX_MDF_Config_t MDF1_CONFIG =
{
  .Gain                 = 8,
  .DecimationRatio      = 9,
  .CicMode              = MDF_ONE_FILTER_SINC4,
  .ProcClockDivider     = 1,
  .OutputClockDivider   = 7,
};

extern MDF_FilterConfigTypeDef Audio_MdfFilterConfig;
          
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
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  RCC_OscInitTypeDef        RCC_OscInitStruct = {0};
  
  /* Start MSI@16MHz*/ 
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState       = RCC_MSI_ON;
  RCC_OscInitStruct.MSIFrequency   = RCC_MSI_FREQ_4MHZ; 
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  if (SampleRate != AUDIO_FREQUENCY_16K)
  {
	while(1);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_MDF1;
  PeriphClkInitStruct.Mdf1ClockSelection = RCC_MDF1CLKSOURCE_MSI;
  
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    status = HAL_ERROR;
  }
 
  return status;
}

/**
  * @brief  Initialize MDF1.
  * @param  hmdf  MDF handle.
  * @param  MXInit MDF configuration structure.
  * @retval HAL_status.
  */
HAL_StatusTypeDef MX_MDF1_Init(MDF_HandleTypeDef *hmdf, MX_MDF_Config_t *MXInit)
{
  HAL_StatusTypeDef status = HAL_OK;

  MXInit->Gain               = MDF1_CONFIG.Gain;
  MXInit->DecimationRatio    = MDF1_CONFIG.DecimationRatio;
  MXInit->CicMode            = MDF1_CONFIG.CicMode;
  MXInit->ProcClockDivider   = MDF1_CONFIG.ProcClockDivider;
  MXInit->OutputClockDivider = MDF1_CONFIG.OutputClockDivider;

  /* MDF peripheral initialization */
  hmdf->Init.CommonParam.ProcClockDivider = MXInit->ProcClockDivider;
  hmdf->Init.CommonParam.OutputClock.Activation = ENABLE;
  hmdf->Init.CommonParam.OutputClock.Pins = MDF_OUTPUT_CLOCK_0;
  hmdf->Init.CommonParam.OutputClock.Divider = MXInit->OutputClockDivider;
  hmdf->Init.CommonParam.OutputClock.Trigger.Activation = DISABLE;
  hmdf->Init.SerialInterface.Activation = ENABLE;
  hmdf->Init.SerialInterface.Mode = MDF_SITF_LF_MASTER_SPI_MODE;
  hmdf->Init.SerialInterface.ClockSource = MDF_SITF_CCK0_SOURCE;
  hmdf->Init.SerialInterface.Threshold = 31U;
  hmdf->Init.FilterBistream = MDF_BITSTREAM0_FALLING;

  if (HAL_MDF_Init(hmdf) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  /* Prepare MDF filter configuration parameters */
  Audio_MdfFilterConfig.DataSource                = MDF_DATA_SOURCE_BSMX;
  Audio_MdfFilterConfig.Delay                     = 0U;
  Audio_MdfFilterConfig.CicMode                   = MXInit->CicMode;
  Audio_MdfFilterConfig.DecimationRatio           = MXInit->DecimationRatio;
  Audio_MdfFilterConfig.Offset                    = 0;
  Audio_MdfFilterConfig.Gain                      = MXInit->Gain;
  Audio_MdfFilterConfig.ReshapeFilter.Activation  = ENABLE;
  Audio_MdfFilterConfig.ReshapeFilter.DecimationRatio = MDF_RSF_DECIMATION_RATIO_4;

  Audio_MdfFilterConfig.HighPassFilter.Activation = ENABLE;
  Audio_MdfFilterConfig.HighPassFilter.CutOffFrequency = MDF_HPF_CUTOFF_0_000625FPCM;

  Audio_MdfFilterConfig.Integrator.Activation     = DISABLE;

  Audio_MdfFilterConfig.AcquisitionMode           = MDF_MODE_ASYNC_CONT;
  Audio_MdfFilterConfig.FifoThreshold             = MDF_FIFO_THRESHOLD_NOT_EMPTY;
  Audio_MdfFilterConfig.DiscardSamples            = 0U;

  return status;
}

/**
  * @brief  SAI1 clock Config.
  * @param  hsai SAI handle.
  * @param  SampleRate Audio sample rate used to play the audio stream.
  * @note   The SAI clock configuration done within this function assumes that
  *         HSE is already enabled.
  * @retval HAL status.
  */
HAL_StatusTypeDef MX_SAI1_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t SampleRate)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hsai);

  HAL_StatusTypeDef         status = HAL_OK;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  RCC_OscInitTypeDef        RCC_OscInitStruct = {0};
  
  /* Start MSI@4MHz*/
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState       = RCC_MSI_ON;
  RCC_OscInitStruct.MSIFrequency   = RCC_MSI_FREQ_4MHZ;
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  if (SampleRate != AUDIO_FREQUENCY_16K)
  {
	while(1);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  PeriphClkInitStruct.Sai1ClockSelection = LL_RCC_SAI1_CLKSOURCE_MSI;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
	  status = HAL_ERROR;
  }

  return status;
}
/**
  * @brief  Initialize SAI1.
  * @param  hsai SAI handle.
  * @param  MXInit SAI configuration structure.
  * @retval HAL status.
  */
HAL_StatusTypeDef MX_SAI1_Init(SAI_HandleTypeDef *hsai, MX_SAI_Config_t *MXInit)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Configure SAI1_Block_X */
  hsai->Init.MonoStereoMode       = MXInit->MonoStereoMode;
  hsai->Init.AudioFrequency       = MXInit->AudioFrequency;
  hsai->Init.AudioMode            = MXInit->AudioMode;
  hsai->Init.NoDivider            = SAI_MASTERDIVIDER_ENABLE;
  hsai->Init.Protocol             = SAI_FREE_PROTOCOL;
  hsai->Init.DataSize             = MXInit->DataSize;
  hsai->Init.FirstBit             = SAI_FIRSTBIT_MSB;
  hsai->Init.ClockStrobing        = MXInit->ClockStrobing;
  hsai->Init.Synchro              = MXInit->Synchro;
  hsai->Init.OutputDrive          = MXInit->OutputDrive;
  hsai->Init.FIFOThreshold        = SAI_FIFOTHRESHOLD_1QF;
  hsai->Init.SynchroExt           = MXInit->SynchroExt;
  hsai->Init.CompandingMode       = SAI_NOCOMPANDING;
  hsai->Init.TriState             = SAI_OUTPUT_NOTRELEASED;
  hsai->Init.Mckdiv               = 0U;
  hsai->Init.MckOutput            = SAI_MCK_OUTPUT_ENABLE;
  hsai->Init.MckOverSampling      = SAI_MCK_OVERSAMPLING_DISABLE;
  hsai->Init.PdmInit.Activation   = DISABLE;

  /* Configure SAI1_Block_X Frame */
  hsai->FrameInit.FrameLength       = MXInit->FrameLength;
  hsai->FrameInit.ActiveFrameLength = MXInit->ActiveFrameLength;
  hsai->FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  hsai->FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  hsai->FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  /* Configure SAI1_Block_X Slot */
  hsai->SlotInit.FirstBitOffset     = 0;
  if (MXInit->DataSize == AUDIO_RESOLUTION_24B)
  {
    hsai->SlotInit.SlotSize         = SAI_SLOTSIZE_32B;
  }
  else
  {
    hsai->SlotInit.SlotSize         = SAI_SLOTSIZE_16B;
  }
  hsai->SlotInit.SlotNumber         = 2;
  hsai->SlotInit.SlotActive         = MXInit->SlotActive;

  if (HAL_SAI_Init(hsai) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  return status;
}

