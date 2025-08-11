/**
  ******************************************************************************
  * @file    app_msg.h
  * @author  GPM/AIS Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   APP configuration
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

#ifndef __APP_MSG_H__
#define __APP_MSG_H__

#include <stdint.h>

#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
#include <arm_cmse.h>
#endif

typedef union _AppMsg{

  uint8_t msg_id;

  struct generic_msg_t
  {
    uint8_t   msg_id;
    uint8_t   sparam;       /* optional small parameter */
    uint16_t  cmd_id;                     /* command ID */
    uint32_t  param;             /* optional parameter. */
  } generic_msg;

  struct sensor_data_ready_msg_t
  {
    uint8_t  msg_id;
    uint8_t  half;       /* used only by the microphone */
    uint8_t  nReserved[2];                  /* Reserved */
    double   fTimestamp;              /* timestamp in s.*/
  } sensor_data_ready_msg;

}AppMsg_t;

#define APP_MESSAGE_ID_DATA_READY           (0x01)
#define APP_MESSAGE_ID_DATA_BUFF_READY      (0x02)
#define APP_MESSAGE_ID_START_ACQ            (0x03)
#define APP_MESSAGE_TOGGLE_PROC             (0x04)
#define APP_MESSAGE_GENERATE_LOAD           (0x06)
#define APP_MESSAGE_GENERATE_LOAD_START     (0x07)
#define APP_MESSAGE_GENERATE_LOAD_STEP      (0x08)
#define APP_MESSAGE_GENERATE_LOAD_STOP      (0x09)

#endif /* __APP_MSG_H__ */
