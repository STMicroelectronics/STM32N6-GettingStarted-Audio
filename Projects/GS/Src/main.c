/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief
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
#include <assert.h>

#include "main.h"
#include "audio_bm.h"

#ifndef APP_BARE_METAL
#include "audio_freertos.h"
#endif

int main(void)
{
  init_bm();
#ifdef APP_BARE_METAL
  exec_bm();
#else
  main_freertos();
#endif
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(file);
  UNUSED(line);

  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) 
  */
  printf("FAIL on file %s on line %lu\r\n", file, line);
  __BKPT(0);
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
