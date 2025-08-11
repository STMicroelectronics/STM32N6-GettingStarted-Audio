/**
  ******************************************************************************
  * @file    AudioCapture_rin_buff.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    02-May-2025
  * @brief   handles ping pong buffer of large size fed by smaller ones
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021(-2021) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "AudioCapture_ring_buff.h"
#include "stm32n6570_discovery.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h" 
#include "cmsis_compiler.h"
#include "app_config.h"
#include "audio_bm.h"
#ifndef APP_BARE_METAL
#include "audio_acq_task.h"
#include "FreeRTOS.h"
#include "semphr.h"
#endif

/**
 * This Ring buffer (in fact a FIFO) is designed for UC below:
 *
 *      .one Audio HW IT is feeding the buffer
 *      .One audio SW IT (or Thread) is consumming the buffer
 * 
 * The Number of samples injected and consummed is constant.
 * The total size of the buffer is a multiple of this constant
 * 
 * The only variable shared between producer and consummer is the "availableSamples."
 * The calcultaion of this member is protected by simple atomic operation, in order
 * to avoid  _disable_irq();
 */


/**
 * @brief adds a value to a reference / insure the data has not changed betwwen load an store
 * 
 * @param p32 the reference pointer 
 * @param inc the inc value
 * @return void 
 */
static inline void atomic_add(uint32_t *p32,int32_t inc);
static inline void atomic_add(uint32_t *p32,int32_t inc)
{

  do{}while (__STREXW(__LDREXW(p32) + inc, p32));
}

/**
  * @brief  reset all field of the handler
  * @param  Handler of the ring buffer
  * @retval None
  */
void AudioCapture_ring_buff_reset(AudioCapture_ring_buff_t *pHdle)
{
  memset(pHdle, 0, sizeof(AudioCapture_ring_buff_t));
  pHdle->pData = NULL;
}

/**
  * @brief  Allocate buffer as function of info inside the handler fields
  * @param  Handler of the ring buffer
  * @retval None
  */
void AudioCapture_ring_buff_alloc(AudioCapture_ring_buff_t *pHdle)
{
  size_t size = pHdle->nbSamples * pHdle->nbBytesPerSample * pHdle->nbFrames;
  pHdle->pData = malloc(size);
  if (!pHdle->pData )
  {
    LogError("AudioCapture_ring_buff_alloc :Malloc failed \n");
    while(1);
  }
}

/**
  * @brief  Free buffer
  * @param  Handler of the ring buffer
  * @retval None
  */
void AudioCapture_ring_buff_free(AudioCapture_ring_buff_t *pHdle)
{
  free(pHdle->pData);
}
/**
  * @brief  Update read index of data bufer as function of given number of samples and returns new data pointer
  * @param  Handler of the ring buffer
  * @param  number of samples
  * @retval data pointer
  */

uint8_t *AudioCapture_ring_buff_consume_no_cpy(AudioCapture_ring_buff_t *pHdle, uint32_t nbSamples)
{
  uint8_t  *pDestData = NULL;
  int32_t s32_nbSamples = (int32_t)nbSamples;

  if (pHdle->availableSamples >= s32_nbSamples)
  {
    pDestData =  pHdle->pData + ((uint32_t)pHdle->nbBytesPerSample * pHdle->readSampleIndex);
    pHdle->readSampleIndex = (pHdle->readSampleIndex + nbSamples) % (pHdle->nbSamples *pHdle->nbFrames);
    atomic_add(&pHdle->availableSamples, -s32_nbSamples);
  }

  return pDestData;
}

uint8_t *AudioCapture_ring_buff_consume(uint8_t *pData, AudioCapture_ring_buff_t *pHdle, uint32_t nbSamples)
{
    int32_t  s32_nbSamples = (int32_t)nbSamples;
    uint32_t u32_nbSamples = (uint32_t)nbSamples;

#ifdef APP_BARE_METAL
    __disable_irq();
#else
    // Acquire the mutex, block indefinitely
    if (xSemaphoreTake(AudioAcqTask.lock, portMAX_DELAY) != pdTRUE)
    {
        // Optional: handle error if mutex not acquired
    }
#endif

    if (pHdle->availableSamples >= s32_nbSamples)
    {
        uint32_t bps    = pHdle->nbBytesPerSample;
        uint32_t idx    = pHdle->readSampleIndex;
        uint32_t idxMax = pHdle->nbSamples * pHdle->nbFrames;
        uint32_t toRead = u32_nbSamples < (idxMax - idx) ? u32_nbSamples : idxMax - idx;

        memcpy(pData, pHdle->pData + idx * bps, toRead * bps);
        uint32_t read = toRead;
        toRead = u32_nbSamples - toRead;
        pHdle->readSampleIndex += u32_nbSamples;

        if (toRead)
        {
            memcpy(pData + read * bps, pHdle->pData, toRead * bps);
            pHdle->readSampleIndex = toRead;
        }

        atomic_add(&pHdle->availableSamples, -s32_nbSamples);
    }

#ifdef APP_BARE_METAL
    __enable_irq();
#else
    // Release the mutex
    xSemaphoreGive(AudioAcqTask.lock);
#endif

    return pData;
}
/**
* @brief  write new data inside buffer, update write index as function of given number of samples
* @param  Handler of the ring buffer
* @param  pointer on data to be written inside the ring buffer
* @param  number of samples
* @retval None
*/
void AudioCapture_ring_buff_feed(AudioCapture_ring_buff_t *pHdle, uint8_t *pData, int16_t nbSamples)
{
    int32_t s32_nbSamples = (int32_t)nbSamples;
    uint32_t u32_nbSamples = (uint32_t)nbSamples;

#ifdef APP_BARE_METAL
    __disable_irq();
#else
    // Take the mutex, wait indefinitely
    if (xSemaphoreTake(AudioAcqTask.lock, portMAX_DELAY) != pdTRUE)
    {
        // Handle error if needed (optional)
    }
#endif

    if ((pData != NULL) && (pHdle->pData != NULL))
    {
        if ((pHdle->availableSamples + u32_nbSamples) > (pHdle->nbSamples * pHdle->nbFrames))
        {
            LogDebug("Audio capture => Overrun\r\n");
        }
        else
        {
            uint32_t bps     = pHdle->nbBytesPerSample;
            uint32_t idx     = pHdle->writeSampleIndex;
            uint32_t idxMax  = pHdle->nbSamples * pHdle->nbFrames;
            uint32_t toWrite = u32_nbSamples < (idxMax- idx) ? u32_nbSamples: idxMax - idx;
            memcpy(pHdle->pData + idx*bps, pData ,toWrite * bps);
            uint32_t written = toWrite;
            toWrite = u32_nbSamples - toWrite ;
            pHdle->writeSampleIndex += u32_nbSamples;
            if (toWrite)
            {
            memcpy(pHdle->pData, pData+written*bps,toWrite*bps);
            pHdle->writeSampleIndex=toWrite;
            }
            atomic_add(&pHdle->availableSamples, s32_nbSamples);
        }
    }
    else
    {
        LogError("Audio capture => Error, NULL pointer\n");
    }

#ifdef APP_BARE_METAL
    __enable_irq();
#else
    // Give back the mutex
    xSemaphoreGive(AudioAcqTask.lock);
#endif
}