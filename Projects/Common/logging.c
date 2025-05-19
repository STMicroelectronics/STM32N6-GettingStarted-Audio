/**
  ******************************************************************************
  * @file    Logging.c
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


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>

#include "app_config.h"
#include "logging.h"

#define LOG_COLOR_BLACK     "\033[0;30m"
#define LOG_COLOR_RED       "\033[0;31m"
#define LOG_COLOR_GREEN     "\033[0;32m"
#define LOG_COLOR_YELLOW    "\033[0;33m"
#define LOG_COLOR_BLUE      "\033[0;34m"
#define LOG_COLOR_MAGENTA   "\033[0;35m"
#define LOG_COLOR_CYAN      "\033[0;36m"
#define LOG_COLOR_WHITE     "\033[0;37m"
#define LOG_COLOR_RESET     "\033[0m"

#define LOG_COLOR_B_BLACK   "\033[1;30m"
#define LOG_COLOR_B_RED     "\033[1;31m"
#define LOG_COLOR_B_GREEN   "\033[1;32m"
#define LOG_COLOR_B_YELLOW  "\033[1;33m"
#define LOG_COLOR_B_BLUE    "\033[1;34m"
#define LOG_COLOR_B_MAGENTA "\033[1;35m"
#define LOG_COLOR_B_CYAN    "\033[1;36m"
#define LOG_COLOR_B_WHITE   "\033[1;37m"

#define LOG_LEVEL_NONE_STR     "None"
#define LOG_LEVEL_ERROR_STR    "Error"
#define LOG_LEVEL_WARN_STR     "Warn"
#define LOG_LEVEL_INFO_STR     "Info"
#define LOG_LEVEL_DEBUG_STR    "Debug"
#define LOG_LEVEL_INVALID_STR  "Invalid"

const char * getLogLevelStr(int LogLevel)
{
  switch(LogLevel){
  case LOG_ERROR: return LOG_LEVEL_ERROR_STR;
  case LOG_WARN: return LOG_LEVEL_WARN_STR;
  case LOG_INFO: return LOG_LEVEL_INFO_STR;
  case LOG_DEBUG: return LOG_LEVEL_DEBUG_STR;
  default:return LOG_LEVEL_INVALID_STR;
  }     
}
void LogPrintf(int LogLevel,char *format, ... )
{
  va_list args;
  if(LogLevel <= LOG_LEVEL)
  {
    switch(LogLevel){
    case LOG_ERROR:
      printf(LOG_COLOR_B_RED);
      break;
    case LOG_WARN:
      printf(LOG_COLOR_MAGENTA);
      break;
    case LOG_INFO:
      printf(LOG_COLOR_RESET);
      break;
    case LOG_DEBUG:
      printf(LOG_COLOR_BLUE);
      break;
    default:
      printf(LOG_COLOR_RESET);
      break;
    }     
    va_start(args,format);
    vprintf(format,args);
    printf(LOG_COLOR_RESET);
    va_end(args);
  }
}