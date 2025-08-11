/**
  ******************************************************************************
  * @file     audio_proc_task.c
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
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "preproc_dpu.h"                            /* Preprocessing includes */
#include "ai_dpu.h"                                 /* AI includes            */
#include "test.h"
#include "pm_dvfs.h"
#include "app_msg.h"
#include "audio_bm.h"
#include "audio_acq_task.h"
#include "audio_proc_task.h"

/* Private variables ---------------------------------------------------------*/
AudioProcTask_t AudioProcTask;

void audio_proc_thread_func(void *pvParameters)
{
    bool cont = true;
    AppMsg_t report;

    /* Initialize audio processing context */
    initAudioProc(&AudioProcTask.ctx);

    printHeader();

    while (cont)
    {
        /* Wait indefinitely for a message from the queue */
        if (xQueueReceive(AudioProcTask.queue, &report, portMAX_DELAY) == pdTRUE)
        {
            switch (report.msg_id)
            {
                case APP_MESSAGE_ID_DATA_BUFF_READY:
                    cont = audio_process(&AudioAcqTask.ctx, &AudioProcTask.ctx);
                    break;

                case APP_MESSAGE_TOGGLE_PROC:
                    toggle_audio_proc();
                    break;

                default:
                    /* Unwanted report - ignore */
                    break;
            }
        }
    }

    test_dump();

#if (CTRL_X_CUBE_AI_AUDIO_OUT == COM_TYPE_HEADSET)
    stopAudioPlayBack();
#endif

    my_printf("\r\n-------------- End Processing --------------------\r\n\n");

    /* Delete self */
    vTaskDelete(NULL);
}

#if (CTRL_X_CUBE_AI_AUDIO_OUT == COM_TYPE_HEADSET)
/**
  * @brief  Tx Transfer completed callbacks.
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
    AudioCapture_ring_buff_consume_no_cpy(&AudioProcTask.ctx.audioPlayBackCtx.ring_buff, PLAYBACK_BUFFER_SIZE / 2);
}

/**
  * @brief  Tx Transfer Half completed callbacks.
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
    AudioCapture_ring_buff_consume_no_cpy(&AudioProcTask.ctx.audioPlayBackCtx.ring_buff, PLAYBACK_BUFFER_SIZE / 2);
}
#endif /* (CTRL_X_CUBE_AI_AUDIO_OUT == COM_TYPE_HEADSET) */
