/**
  ******************************************************************************
  * @file    Logging.h
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   This file contains basic implememtation for a logger
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOGGING_H
#define __LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif
 
#define LOG_NONE     0
#define LOG_ERROR    1
#define LOG_WARN     2
#define LOG_INFO     3
#define LOG_DEBUG    4
 
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
void LogPrintf( int LogLevel,char *format, ... );
  
/* Exported macro ------------------------------------------------------------*/
#define LogError( ... )  do { LogPrintf( LOG_ERROR, __VA_ARGS__ ); } while( 0 )
#define LogWarn( ... )   do { LogPrintf( LOG_WARN, __VA_ARGS__  ); } while( 0 )
#define LogInfo( ... )   do { LogPrintf( LOG_INFO, __VA_ARGS__  ); } while( 0 )
#define LogDebug( ... )  do { LogPrintf( LOG_DEBUG, __VA_ARGS__ ); } while( 0 )

extern const char * getLogLevelStr(int LogLevel);

#ifdef __cplusplus
}
#endif

#endif /*  __LOGGING_ */
