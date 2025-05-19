/**
******************************************************************************
* @file    load_gen_task.c
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
#include <stdio.h>
#include <stdbool.h> 
#include "app_config.h"
#include "stm32n657xx.h"
#include "tx_api.h"
#include "app_msg.h"
#include "load_gen_task.h"
#include "audio_proc_task.h"
#include "pm_dvfs.h"


LoadGenTask_t LoadGenTask;

void load_gen_thread_func(ULONG arg)
{
  AppMsg_t msg_in, msg_out;
  srand(335312);

  while(1)
  {
    if(TX_SUCCESS == tx_queue_receive(&LoadGenTask.queue, &msg_in, TX_WAIT_FOREVER))
    {
      switch(msg_in.msg_id)
      {
      case APP_MESSAGE_GENERATE_LOAD:
        if (LoadGenTask.nbRun>0) /* if load generation STOP generating loads */
        {
          msg_out.msg_id = APP_MESSAGE_GENERATE_LOAD_STOP;
          if(TX_SUCCESS != tx_queue_send(&LoadGenTask.queue, &msg_out, TX_NO_WAIT))
          {
            /* unable to send the report. Signal the error */
          }
        }
        else  /* start generating loads */
        {
          msg_out.msg_id = APP_MESSAGE_GENERATE_LOAD_START;
          LoadGenTask.time_slice = msg_in.generic_msg.param;
          LoadGenTask.duty_cycle = msg_in.generic_msg.sparam/100.0F;
          LoadGenTask.nbRun = LOAD_GEN_NB_RUN;
          if(TX_SUCCESS != tx_queue_send(&LoadGenTask.queue, &msg_out, TX_NO_WAIT))
          {
            /* unable to send the report. Signal the error */
          }           
        }
        break;
      case APP_MESSAGE_GENERATE_LOAD_START:
        LoadGenTask.nbRun=100;
        msg_out.msg_id = APP_MESSAGE_GENERATE_LOAD_STEP;
        if(TX_SUCCESS != tx_queue_send(&LoadGenTask.queue, &msg_out, TX_NO_WAIT))
        {
          /* unable to send the report. Signal the error */
        }           
        break;
      case APP_MESSAGE_GENERATE_LOAD_STEP:
        if (LoadGenTask.nbRun>0)
        {
#ifdef APP_DVFS 
          pm_set_opp_min(OPP_MAX); 
#endif
          float x = 2.0F*rand()/RAND_MAX*LoadGenTask.duty_cycle;
          int nb_ms_cpu_busy = (int)(LoadGenTask.time_slice*x);
          x = 2.0F*(1.0F-LoadGenTask.duty_cycle)*rand()/RAND_MAX;
          int nb_ms_thread_sleep = (int)(LoadGenTask.time_slice*x);
          LoadGenTask.nbRun--;
          BSP_LED_On(LED_RED);
          int32_t start_time = HAL_GetTick();
          while (HAL_GetTick() - start_time < nb_ms_cpu_busy ) {};
          BSP_LED_Off(LED_RED);
#ifdef APP_DVFS 
          pm_set_opp_min(OPP_MIN); 
#endif 
          tx_thread_sleep(nb_ms_thread_sleep);
          msg_out.msg_id = APP_MESSAGE_GENERATE_LOAD_STEP;
          if(TX_SUCCESS != tx_queue_send(&LoadGenTask.queue, &msg_out, TX_NO_WAIT))
          {
            /* unable to send the report. Signal the error */
          }    
        }
        break;
      case APP_MESSAGE_GENERATE_LOAD_STOP:
        if (LoadGenTask.nbRun>0)
        {
          LoadGenTask.nbRun=0;
        }
        else /* if nothing to stop, it means user probably wants re-start */
        {
          AppMsg_t report;
          report.msg_id = APP_MESSAGE_GENERATE_LOAD_START;
          if(TX_SUCCESS != tx_queue_send(&LoadGenTask.queue, &report, TX_NO_WAIT))
          {
            /* unable to send the report. Signal the error */
          } 
        }
        break;
      default:
        {
          /* unwanted report */
        }
      }
    }
  }
}

void BSP_PB_Callback(Button_TypeDef Button)
{
  if (BUTTON_USER1 == Button)
  {
    AppMsg_t msg;
    msg.generic_msg.msg_id = APP_MESSAGE_TOGGLE_PROC;
    if(TX_SUCCESS != tx_queue_send(&AudioProcTask.queue, &msg, TX_NO_WAIT))
    {
      /* unable to send the report. Signal the error */
    }
  }
  if (BUTTON_TAMP == Button)
  {
    AppMsg_t msg;
    msg.generic_msg.msg_id = APP_MESSAGE_GENERATE_LOAD;
    msg.generic_msg.param  = LOAD_GEN_TIME_SLICE ; /*  100 ms timeslices    */  
    msg.generic_msg.sparam = LOAD_GEN_DUTY_CYCLE ;  /* 20 % load on average */
    if(TX_SUCCESS != tx_queue_send(&LoadGenTask.queue, &msg, TX_NO_WAIT))
    {
      /* unable to send the report. Signal the error */
    } 
  }
}
