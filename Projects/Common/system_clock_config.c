/**
  ******************************************************************************
  * @file    system_clock_config.c
  * @author  GPM/AIS Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   system clock config
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

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "main.h"
#include "app_config.h"

uint32_t npur_freq_mhz = 64;         /* NIC freq */
uint32_t npu_freq_mhz = 64;          /* NPU freq */

/**
  * @brief  Reset System Clock Configuration to reset value
  *
  * Reset oscillators / main clocks to their reset value
  * This can be useful at the start of main to erase possible leftovers from
  * a previous reset or from the bootloader
  */
void SystemClock_Config_ResetClocks(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Stop clocks that may be ON after a reset/bootloader and may cause freeze */
  __HAL_RCC_XSPI1_CLK_DISABLE();
  __HAL_RCC_XSPI2_CLK_DISABLE();
  __HAL_RCC_NPU_CLK_DISABLE();

  /* Ensure default clocks are ON and others are OFF
     Bootrom/FlexRAM/AXISRAM1, 2/BKUPSRAM/AHBSRAM1,2 are on
     AXISRAM3/4/5/6 / VencRAM / NPUCACHERAM are off
  */

  RCC->MEMENSR = 0x000013F0; 
  RCC->MEMENCR = ~0x000013F0;
  
  /* RCC->CFGR1 = 0x00000000;  Reset cpuclk/sysclk/stopclk source to hsi      */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK |\
    RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2  |\
    RCC_CLOCKTYPE_PCLK4  | RCC_CLOCKTYPE_PCLK5);
  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; 
  /* RCC->CFGR2 = 0x00100000;  Reset all prescalers to their default value    */
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Stop all PLLs just in case something is still on after boot.             */
  /* RCC->CR = 0x00000008U;  All PLL off, HSI on only */
  RCC_OscInitStruct.OscillatorType = (RCC_OSCILLATORTYPE_HSI |\
    RCC_OSCILLATORTYPE_HSE );
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_OFF;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1);
  }
}

/**
  * @brief  Configures the clock tree as follows:
  * - PLL1 @ 800MHz / PLL2 @ 1000MHz / PLL3 @ 900 MHz
  * 
  * - CPUCLK (clock of the CPU)        800 MHz
  * - SYSCLK (clock of the CPU NIC)    400MHz
  * - IC6    (clock of the NPU)        1000MHz
  * - IC11   (clock of the NPU RAMs)   900MHz
  * - IC3/4  (clocks of the xSPIs)     200MHz
  */
void SystemClock_Config_HSI_overdrive(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  
  /* HSE selected as source (stable clock on Level 0 samples
      PLL1 output = ((HSI/PLLM)*PLLN)/PLLP1/PLLP2
                  = ((64000000/2)*25)/1/1
                  = 800000000 (800 MHz)
      PLL2 output = HSI (1 GHz) - For NPU 
      PLL3 output = HSI (900 MHz) - For AXIRAMs 3..6 
  */

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 2;
  RCC_OscInitStruct.PLL1.PLLN = 25;
  RCC_OscInitStruct.PLL1.PLLP1 = 1;
  RCC_OscInitStruct.PLL1.PLLP2 = 1;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  /* PLL2: 64 x 125 / 8 = 1GHz */
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL2.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL2.PLLM = 8;
  RCC_OscInitStruct.PLL2.PLLFractional = 0;
  RCC_OscInitStruct.PLL2.PLLN = 125;
  RCC_OscInitStruct.PLL2.PLLP1 = 1;
  RCC_OscInitStruct.PLL2.PLLP2 = 1;
  /* PLL3: 64 x 225 / 16 = 900MHz */
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL3.PLLM = 16;
  RCC_OscInitStruct.PLL3.PLLFractional = 0;
  RCC_OscInitStruct.PLL3.PLLN = 225;
  RCC_OscInitStruct.PLL3.PLLP1 = 1;
  RCC_OscInitStruct.PLL3.PLLP2 = 1;
  /* PLL4: OFF */
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_OFF;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL1 outputs as CPU and System bus clock source
     CPUCLK = ic1_ck = PLL1 output/ic1_divider = 800 MHz
     SYSCLK = ic2_ck = PLL1 output/ic2_divider = 400 MHz
     Configure the HCLK, PCLK1, PCLK2, PCLK4 and PCLK5 clocks dividers
     HCLK = SYSCLK/HCLK divider = 200 MHz
     PCLKx = HCLK / PCLKx divider = 100 MHz
  */

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK |\
    RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |\
    RCC_CLOCKTYPE_PCLK4 | RCC_CLOCKTYPE_PCLK5);
  /* IC1 is used for CPUCLK @ 800MHz (=800/1) */
  RCC_ClkInitStruct.IC1Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC1Selection.ClockDivider = 1;
  /* IC2 is used for SYSCLK @ 400MHz (=800/2) */
  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider = 2;
  /* IC6 (used for NPU) @ 1GHz */
  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL2; 
  RCC_ClkInitStruct.IC6Selection.ClockDivider = 1;
  npu_freq_mhz = 1000;
  /* IC11 (for AXISRAM3->6) @ 900MHz */

  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL3; 
  RCC_ClkInitStruct.IC11Selection.ClockDivider = 1;
  npur_freq_mhz = 900;

  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_IC1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_IC2_IC6_IC11; 

  /* WARNING: Do not put dividers != 1 for PCLK 
     This leads to unstable behaviour
  */
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* PERIPHS 
       Select IC4 clock from PLL1 (800 MHz), 
       divided by 4 = 200MHz as XSPI1 source
       This clock is used as-is in the xspi1-RAM bsp
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_XSPI1;
  PeriphClkInit.Xspi1ClockSelection = RCC_XSPI1CLKSOURCE_IC4;
  PeriphClkInit.ICSelection[RCC_IC4].ClockSelection = RCC_ICCLKSOURCE_PLL1;
  /* IC4 @ 200MHz --> directly sourced to xspi1 (prescaler is bypassed in
     the end of the init phase)
  */
  PeriphClkInit.ICSelection[RCC_IC4].ClockDivider = 4; 
  /* XSPI2 clock source configuration
    Select IC3 clock from PLL1 (800 MHz), 
       divided by 4 = 200MHz as XSPI2 source
    This clock is used as-is in the xspi2-NOR bsp
  */
  PeriphClkInit.PeriphClockSelection |= RCC_PERIPHCLK_XSPI2;
  PeriphClkInit.Xspi2ClockSelection = RCC_XSPI2CLKSOURCE_IC3;
  PeriphClkInit.ICSelection[RCC_IC3].ClockSelection = RCC_ICCLKSOURCE_PLL1;
  /* IC3 @ 200MHz --> directly sourced to xspi2 (prescaler is bypassed when
     memory-mapping)
  */
  PeriphClkInit.ICSelection[RCC_IC3].ClockDivider = 4; 
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    __BKPT(0);
  }
}

/**
  * @brief  Configures the clock tree as follows:
  * - PLL1 @ 800MHz / PLL2 @ 600MHz 
  *
  * - CPUCLK (clock of the CPU)        600 MHz
  * - SYSCLK (clock of the CPU NIC)    400MHz
  * - IC6    (clock of the NPU)        800MHz
  * - IC11   (clock of the NPU RAMs)   800MHz
  * - IC3/4  (clocks of the xSPIs)     200MHz
  */
void SystemClock_Config_HSI_no_overdrive(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  /* HSE selected as source (stable clock on Level 0 samples
     PLL1 output = ((HSI/PLLM)*PLLN)/PLLP1/PLLP2
                 = ((64000000/2)*25)/1/1
                 = 800000000 (800 MHz)
     PLL2 output = HSI (600 MHz) - For CPU
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 2;
  RCC_OscInitStruct.PLL1.PLLN = 25;
  RCC_OscInitStruct.PLL1.PLLP1 = 1;
  RCC_OscInitStruct.PLL1.PLLP2 = 1;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  /* PLL2: 64 x 75 / 8 = 600MHz */
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL2.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL2.PLLM = 8;
  RCC_OscInitStruct.PLL2.PLLFractional = 0;
  RCC_OscInitStruct.PLL2.PLLN = 75;
  RCC_OscInitStruct.PLL2.PLLP1 = 1;
  RCC_OscInitStruct.PLL2.PLLP2 = 1;
  /* PLL3: OFF */
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_OFF;
  /* PLL4: OFF */
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_OFF;
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL1 outputs as CPU and System bus clock source
      CPUCLK = ic1_ck = PLL2 output/ic1_divider = 600 MHz
      SYSCLK = ic2_ck = PLL1 output/ic2_divider = 400 MHz
      Configure the HCLK, PCLK1, PCLK2, PCLK4 and PCLK5 clocks dividers
      HCLK = SYSCLK/HCLK divider = 200 MHz
      PCLKx = HCLK / PCLKx divider = 100 MHz
  */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK |\
    RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |\
    RCC_CLOCKTYPE_PCLK4 | RCC_CLOCKTYPE_PCLK5);

  /* IC1 is used for CPUCLK @ 600MHz (=600/1) */
  RCC_ClkInitStruct.IC1Selection.ClockSelection = RCC_ICCLKSOURCE_PLL2;
  RCC_ClkInitStruct.IC1Selection.ClockDivider = 1;
  /* IC2 is used for SYSCLK @ 400MHz (=800/2) */
  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider = 2;
   /* IC6 (used for NPU) @ 800MHz */
  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1; 
  RCC_ClkInitStruct.IC6Selection.ClockDivider = 1;
  npu_freq_mhz = 800;
  /* IC11 (for AXISRAM3->6) @ 800MHz */
  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1; 
  RCC_ClkInitStruct.IC11Selection.ClockDivider = 1;
  npur_freq_mhz = 800;

  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_IC1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_IC2_IC6_IC11;

  /* WARNING: Do not put dividers != 1 for PCLK 
     This leads to unstable behaviour
  */
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;
  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  /* PERIPHS
      Select IC4 clock from PLL1 (800 MHz), 
      divided by 4 = 200MHz as XSPI1 source
      This clock is used as-is in the xspi1-RAM bsp
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_XSPI1;
    PeriphClkInit.Xspi1ClockSelection = RCC_XSPI1CLKSOURCE_IC4;
    PeriphClkInit.ICSelection[RCC_IC4].ClockSelection = RCC_ICCLKSOURCE_PLL1;
    /* IC4 @ 200MHz --> directly sourced to xspi1 (prescaler is bypassed in
       the end of the init phase)
    */
    PeriphClkInit.ICSelection[RCC_IC4].ClockDivider = 4; 
    /* XSPI2 clock source configuration
        Select IC3 clock from PLL1 (800 MHz), 
        divided by 4 = 200MHz as XSPI2 source
        This clock is used as-is in the xspi2-NOR bsp
    */
    PeriphClkInit.PeriphClockSelection |= RCC_PERIPHCLK_XSPI2;
    PeriphClkInit.Xspi2ClockSelection = RCC_XSPI2CLKSOURCE_IC3;
    PeriphClkInit.ICSelection[RCC_IC3].ClockSelection = RCC_ICCLKSOURCE_PLL1;
    /* IC3 @ 200MHz --> directly sourced to xspi2 (prescaler is bypassed when
      memory-mapping)
    */
    PeriphClkInit.ICSelection[RCC_IC3].ClockDivider = 4; 
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      __BKPT(0);
    }
}

void SystemClock_Config_MSI_4MHz_4MHz_exp(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Start MSI@4MHz*/ 
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState       = RCC_MSI_ON;
  RCC_OscInitStruct.MSIFrequency   = RCC_MSI_FREQ_4MHZ; 
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select MSI outputs as CPU and System bus clock source */
  RCC_ClkInitStruct.ClockType = ( RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK |\
    RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |\
    RCC_CLOCKTYPE_PCLK4 | RCC_CLOCKTYPE_PCLK5);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_MSI; 

  /* WARNING: Do not put dividers != 1 for PCLK 
     This leads to unstable behaviour
  */
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;
  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Stop All PLL*/ 
  RCC_OscInitStruct.PLL1.PLLState  = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL2.PLLState  = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL3.PLLState  = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL4.PLLState  = RCC_PLL_OFF;  
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Stop HSI */ 
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState       = RCC_HSI_OFF;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}