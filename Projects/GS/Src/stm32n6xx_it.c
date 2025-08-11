/**
  ******************************************************************************
  * @file    stm32n6xx_it.c 
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32n6xx_hal.h"
#include "stm32n6xx_it.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_audio.h"

/******************************************************************************/
/*            Cortex-M55 Processor Exceptions Handlers                        */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */

#ifdef APP_BARE_METAL
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
#endif

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
#ifdef APP_BARE_METAL
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
#endif

/**
  * @brief  This function handles Secure Fault exception.
  * @param  None
  * @retval None
  */
void SecureFault_Handler(void)
{
  /* Go to infinite loop when Secure Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
#ifdef APP_BARE_METAL
void SVC_Handler(void)
{
}
#endif

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#ifdef APP_BARE_METAL
void PendSV_Handler(void)
{
  while (1)
  {
  }
}
#endif

/**
  * @brief This function handles System tick timer.
  */
void _SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  /* Clear overflow flag */
  SysTick->CTRL;

#ifndef APP_BARE_METAL
  if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
  {
      /* Call the cortex-m33 port systick handler */
      SysTick_Handler();
  }
#endif
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32N6xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32n6xx.s).                                               */
/******************************************************************************/

void GPDMA1_Channel2_IRQHandler(void)
{
  BSP_AUDIO_OUT_IRQHandler(0, AUDIO_OUT_DEVICE_HEADPHONE);
}

void GPDMA1_Channel0_IRQHandler(void)
{
  BSP_AUDIO_IN_IRQHandler(1, AUDIO_IN_DEVICE_DIGITAL_MIC);
}

void EXTI13_IRQHandler(void)
{
  BSP_PB_IRQHandler(BUTTON_USER1);
}

void EXTI0_IRQHandler(void)
{
  BSP_PB_IRQHandler(BUTTON_TAMP);
}
/*--- patch-- */
/* void SysTick_Handler(void)
{
 while (1) {};
} */
void PVD_PVM_IRQHandler(void)
{
  while (1) {};
}
void DTS_IRQHandler(void)
{
  while (1) {};
}
void RCC_IRQHandler(void)
{
  while (1) {};
}
void LOCKUP_IRQHandler(void)
{
  while (1) {};
}
void CACHE_ECC_IRQHandler(void)
{
  while (1) {};
}
void TCM_ECC_IRQHandler(void)
{
  while (1) {};
}
void BKP_ECC_IRQHandler(void)
{
  while (1) {};
}
void FPU_IRQHandler(void)
{
  while (1) {};
}
void RTC_S_IRQHandler(void)
{
  while (1) {};
}
void TAMP_IRQHandler(void)
{
  while (1) {};
}
void RIFSC_TAMPER_IRQHandler(void)
{
  while (1) {};
}
void IAC_IRQHandler(void)
{
  while (1) {};
}
void RCC_S_IRQHandler(void)
{
  while (1) {};
}
void RTC_IRQHandler(void)
{
  while (1) {};
}
void IWDG_IRQHandler(void)
{
  while (1) {};
}
void WWDG_IRQHandler(void)
{
  while (1) {};
}
/*
 void EXTI0_IRQHandler(void)
{
  while (1) {};
}*/
void EXTI1_IRQHandler(void)
{
  while (1) {};
}
void EXTI2_IRQHandler(void)
{
  while (1) {};
}
void EXTI3_IRQHandler(void)
{
  while (1) {};
}
void EXTI4_IRQHandler(void)
{
  while (1) {};
}
void EXTI5_IRQHandler(void)
{
  while (1) {};
}
void EXTI6_IRQHandler(void)
{
  while (1) {};
}
void EXTI7_IRQHandler(void)
{
  while (1) {};
}
void EXTI8_IRQHandler(void)
{
  while (1) {};
}
void EXTI9_IRQHandler(void)
{
  while (1) {};
}
void EXTI10_IRQHandler(void)
{
  while (1) {};
}
void EXTI11_IRQHandler(void)
{
  while (1) {};
}
void EXTI12_IRQHandler(void)
{
  while (1) {};
}
/* void EXTI13_IRQHandler(void)
{
 while (1) {};
} */
void EXTI14_IRQHandler(void)
{
  while (1) {};
}
void EXTI15_IRQHandler(void)
{
  while (1) {};
}
void SAES_IRQHandler(void)
{
  while (1) {};
}
void CRYP_IRQHandler(void)
{
  while (1) {};
}
void PKA_IRQHandler(void)
{
  while (1) {};
}
void HASH_IRQHandler(void)
{
  while (1) {};
}
void RNG_IRQHandler(void)
{
  while (1) {};
}
void MCE1_IRQHandler(void)
{
  while (1) {};
}
void MCE2_IRQHandler(void)
{
  while (1) {};
}
void MCE3_IRQHandler(void)
{
  while (1) {};
}
void MCE4_IRQHandler(void)
{
  while (1) {};
}
void ADC1_2_IRQHandler(void)
{
  while (1) {};
}
void CSI_IRQHandler(void)
{
  while (1) {};
}
void DCMIPP_IRQHandler(void)
{
  while (1) {};
}
void PAHB_ERR_IRQHandler(void)
{
  while (1) {};
}
/* void NPU0_IRQHandler(void)
{
 while (1) {};
} */
void NPU1_IRQHandler(void)
{
  while (1) {};
}
void NPU2_IRQHandler(void)
{
  while (1) {};
}
void NPU3_IRQHandler(void)
{
  while (1) {};
}
void CACHEAXI_IRQHandler(void)
{
  while (1) {};
}
void LTDC_LO_IRQHandler(void)
{
  while (1) {};
}
void LTDC_LO_ERR_IRQHandler(void)
{
  while (1) {};
}
void DMA2D_IRQHandler(void)
{
  while (1) {};
}
void JPEG_IRQHandler(void)
{
  while (1) {};
}
void VENC_IRQHandler(void)
{
  while (1) {};
}
void GFXMMU_IRQHandler(void)
{
  while (1) {};
}
void GFXTIM_IRQHandler(void)
{
  while (1) {};
}
void GPU2D_IRQHandler(void)
{
  while (1) {};
}
void GPU2D_ER_IRQHandler(void)
{
  while (1) {};
}
void ICACHE_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel0_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel1_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel2_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel3_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel4_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel5_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel6_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel7_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel8_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel9_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel10_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel11_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel12_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel13_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel14_IRQHandler(void)
{
  while (1) {};
}
void HPDMA1_Channel15_IRQHandler(void)
{
  while (1) {};
}
/* void GPDMA1_Channel0_IRQHandler(void)
{
 while (1) {};
} */
void GPDMA1_Channel1_IRQHandler(void)
{
  while (1) {};
}
/* void GPDMA1_Channel2_IRQHandler(void)
{
 while (1) {};
} */
void GPDMA1_Channel3_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel4_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel5_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel6_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel7_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel8_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel9_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel10_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel11_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel12_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel13_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel14_IRQHandler(void)
{
  while (1) {};
}
void GPDMA1_Channel15_IRQHandler(void)
{
  while (1) {};
}
void I2C1_EV_IRQHandler(void)
{
  while (1) {};
}
void I2C1_ER_IRQHandler(void)
{
  while (1) {};
}
void I2C2_EV_IRQHandler(void)
{
  while (1) {};
}
void I2C2_ER_IRQHandler(void)
{
  while (1) {};
}
void I2C3_EV_IRQHandler(void)
{
  while (1) {};
}
void I2C3_ER_IRQHandler(void)
{
  while (1) {};
}
void I2C4_EV_IRQHandler(void)
{
  while (1) {};
}
void I2C4_ER_IRQHandler(void)
{
  while (1) {};
}
void I3C1_EV_IRQHandler(void)
{
  while (1) {};
}
void I3C1_ER_IRQHandler(void)
{
  while (1) {};
}
void I3C2_EV_IRQHandler(void)
{
  while (1) {};
}
void I3C2_ER_IRQHandler(void)
{
  while (1) {};
}
void TIM1_BRK_IRQHandler(void)
{
  while (1) {};
}
void TIM1_UP_IRQHandler(void)
{
  while (1) {};
}
void TIM1_TRG_COM_IRQHandler(void)
{
  while (1) {};
}
void TIM1_CC_IRQHandler(void)
{
  while (1) {};
}
void TIM2_IRQHandler(void)
{
  while (1) {};
}
void TIM3_IRQHandler(void)
{
  while (1) {};
}
void TIM4_IRQHandler(void)
{
  while (1) {};
}
void TIM5_IRQHandler(void)
{
  while (1) {};
}
void TIM6_IRQHandler(void)
{
  while (1) {};
}
void TIM7_IRQHandler(void)
{
  while (1) {};
}
void TIM8_BRK_IRQHandler(void)
{
  while (1) {};
}
void TIM8_UP_IRQHandler(void)
{
  while (1) {};
}
void TIM8_TRG_COM_IRQHandler(void)
{
  while (1) {};
}
void TIM8_CC_IRQHandler(void)
{
  while (1) {};
}
void TIM9_IRQHandler(void)
{
  while (1) {};
}
void TIM10_IRQHandler(void)
{
  while (1) {};
}
void TIM11_IRQHandler(void)
{
  while (1) {};
}
void TIM12_IRQHandler(void)
{
  while (1) {};
}
void TIM13_IRQHandler(void)
{
  while (1) {};
}
void TIM14_IRQHandler(void)
{
  while (1) {};
}
void TIM15_IRQHandler(void)
{
  while (1) {};
}
void TIM16_IRQHandler(void)
{
  while (1) {};
}
void TIM17_IRQHandler(void)
{
  while (1) {};
}
void TIM18_IRQHandler(void)
{
  while (1) {};
}
void LPTIM1_IRQHandler(void)
{
  while (1) {};
}
void LPTIM2_IRQHandler(void)
{
  while (1) {};
}
void LPTIM3_IRQHandler(void)
{
  while (1) {};
}
void LPTIM4_IRQHandler(void)
{
  while (1) {};
}
void LPTIM5_IRQHandler(void)
{
  while (1) {};
}
void ADF1_FLT0_IRQHandler(void)
{
  while (1) {};
}
void MDF1_FLT0_IRQHandler(void)
{
  while (1) {};
}
void MDF1_FLT1_IRQHandler(void)
{
  while (1) {};
}
void MDF1_FLT2_IRQHandler(void)
{
  while (1) {};
}
void MDF1_FLT3_IRQHandler(void)
{
  while (1) {};
}
void MDF1_FLT4_IRQHandler(void)
{
  while (1) {};
}
void MDF1_FLT5_IRQHandler(void)
{
  while (1) {};
}
void SAI1_A_IRQHandler(void)
{
  while (1) {};
}
void SAI1_B_IRQHandler(void)
{
  while (1) {};
}
void SAI2_A_IRQHandler(void)
{
  while (1) {};
}
void SAI2_B_IRQHandler(void)
{
  while (1) {};
}
void SPDIFRX1_IRQHandler(void)
{
  while (1) {};
}
void SPI1_IRQHandler(void)
{
  while (1) {};
}
void SPI2_IRQHandler(void)
{
  while (1) {};
}
void SPI3_IRQHandler(void)
{
  while (1) {};
}
void SPI4_IRQHandler(void)
{
  while (1) {};
}
void SPI5_IRQHandler(void)
{
  while (1) {};
}
void SPI6_IRQHandler(void)
{
  while (1) {};
}
void USART1_IRQHandler(void)
{
  while (1) {};
}
void USART2_IRQHandler(void)
{
  while (1) {};
}
void USART3_IRQHandler(void)
{
  while (1) {};
}
void UART4_IRQHandler(void)
{
  while (1) {};
}
void UART5_IRQHandler(void)
{
  while (1) {};
}
void USART6_IRQHandler(void)
{
  while (1) {};
}
void UART7_IRQHandler(void)
{
  while (1) {};
}
void UART8_IRQHandler(void)
{
  while (1) {};
}
void UART9_IRQHandler(void)
{
  while (1) {};
}
void USART10_IRQHandler(void)
{
  while (1) {};
}
void LPUART1_IRQHandler(void)
{
  while (1) {};
}
void XSPI1_IRQHandler(void)
{
  while (1) {};
}
void XSPI2_IRQHandler(void)
{
  while (1) {};
}
void XSPI3_IRQHandler(void)
{
  while (1) {};
}
void FMC_IRQHandler(void)
{
  while (1) {};
}
void SDMMC1_IRQHandler(void)
{
  while (1) {};
}
void SDMMC2_IRQHandler(void)
{
  while (1) {};
}
void UCPD1_IRQHandler(void)
{
  while (1) {};
}
void USB1_OTG_HS_IRQHandler(void)
{
  while (1) {};
}
void USB2_OTG_HS_IRQHandler(void)
{
  while (1) {};
}
void ETH1_IRQHandler(void)
{
  while (1) {};
}
void FDCAN1_IT0_IRQHandler(void)
{
  while (1) {};
}
void FDCAN1_IT1_IRQHandler(void)
{
  while (1) {};
}
void FDCAN2_IT0_IRQHandler(void)
{
  while (1) {};
}
void FDCAN2_IT1_IRQHandler(void)
{
  while (1) {};
}
void FDCAN3_IT0_IRQHandler(void)
{
  while (1) {};
}
void FDCAN3_IT1_IRQHandler(void)
{
  while (1) {};
}
void FDCAN_CU_IRQHandler(void)
{
  while (1) {};
}
void MDIOS_IRQHandler(void)
{
  while (1) {};
}
void DCMI_PSSI_IRQHandler(void)
{
  while (1) {};
}
void WAKEUP_PIN_IRQHandler(void)
{
  while (1) {};
}
void CTI_INT0_IRQHandler(void)
{
  while (1) {};
}
void CTI_INT1_IRQHandler(void)
{
  while (1) {};
}
void LTDC_UP_IRQHandler(void)
{
  while (1) {};
}
void LTDC_UP_ERR_IRQHandler(void)
{
  while (1) {};
}

