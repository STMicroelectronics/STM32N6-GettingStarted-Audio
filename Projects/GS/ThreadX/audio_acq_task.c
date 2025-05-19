/**
  ******************************************************************************
  * @file     audio_acq_task.c
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
#include "ai_dpu.h"                                 /* AI includes            */
#include "tx_api.h"
#include "app_msg.h"
#include "preproc_dpu.h"   
#include "audio_bm.h"
#include "audio_acq_task.h"
#include "audio_proc_task.h"

AudioAcqTask_t AudioAcqTask;

/* Private function prototypes -----------------------------------------------*/

void audio_acq_thread_func(ULONG arg)
{
  AppMsg_t report;
  initAudioCapture(&AudioAcqTask.ctx);
  startAudioCapture(&AudioAcqTask.ctx);
  
  while(1) {
    if(TX_SUCCESS == tx_queue_receive(&AudioAcqTask.queue, &report, TX_WAIT_FOREVER))
    {
      switch(report.msg_id){
      case APP_MESSAGE_ID_DATA_READY:
        AudioCapture_half_buf_cb(&AudioAcqTask.ctx.ring_buff, AudioAcqTask.ctx.acq_buf,\
          report.sensor_data_ready_msg.half);
        
        if (AudioAcqTask.ctx.ring_buff.availableSamples >= PATCH_NO_OVERLAP)
        {
          AppMsg_t report;
          report.sensor_data_ready_msg.msg_id = APP_MESSAGE_ID_DATA_BUFF_READY;
          if(TX_SUCCESS != tx_queue_send(&AudioProcTask.queue, &report, TX_NO_WAIT))
          {
            /* unable to send the report. Signal the error */
          } 
        }
        break;
      default: /* unwanted report */
        break;
      }
    }
  }
}

#ifndef APP_BARE_METAL
/**
* @brief  Manage the BSP audio in transfer complete event.
* @param  Instance Audio in instance.
* @retval None.
*/
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  if (Instance == 1U)
  {
    AppMsg_t report;
    report.sensor_data_ready_msg.msg_id = APP_MESSAGE_ID_DATA_READY;
    report.sensor_data_ready_msg.half   = 1U;
   
    if(TX_SUCCESS != tx_queue_send(&AudioAcqTask.queue, &report, TX_NO_WAIT))
    {
      /* unable to send the report. Signal the error */
    } 
  }  
}

/**
* @brief  Manage the BSP audio in half transfer complete event.
* @param  Instance Audio in instance.
* @retval None.
*/
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  if (Instance == 1U)
  {
    AppMsg_t report;
    report.sensor_data_ready_msg.msg_id = APP_MESSAGE_ID_DATA_READY;
    report.sensor_data_ready_msg.half   = 0U;
    
    if(TX_SUCCESS != tx_queue_send(&AudioAcqTask.queue, &report, TX_NO_WAIT))
    {
      /* unable to send the report. Signal the error */
    } 
  }
}

/**
* @brief  Manages the BSP audio in error event.
* @param  Instance Audio in instance.
* @retval None.
*/
void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance)
{
  __disable_irq();
  while (1)
  {
  }
}
#endif
