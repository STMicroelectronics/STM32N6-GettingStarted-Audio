/**
  ******************************************************************************
  * @file    audio_bm.c
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
#include "stm32n6xx_hal.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_xspi.h"
#include "stm32n6570_discovery_audio.h"
#include "app_config.h"
#include "system_clock_config.h"
#include "misc_toolbox.h"
#ifdef APP_DVFS
#include "pm_dvfs.h"
#endif
#ifdef CPU_STATS
#include "cpu_stats.h"
#endif
#include "AudioCapture_ring_buff.h"
#include "preproc_dpu.h"                           /* Preprocessing includes  */
#include "postproc_dpu.h"                          /* Postprocessing includes */
#include "ai_dpu.h"                                /* AI includes             */
#include "test.h"
#include "audio_bm.h"

/* Private define ------------------------------------------------------------*/
#define AUDIO_ACQ_LEN     (CTRL_X_CUBE_AI_ACQ_LENGTH)
#if (CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL > 0)
#define AUDIO_ACQ_OFFSET  ((CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL*2 -1)*CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH+CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH)
#else
#define AUDIO_ACQ_OFFSET  (CTRL_X_CUBE_AI_SPECTROGRAM_WINDOW_LENGTH-CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH)
#endif
#define AUDIO_OUT_FIRST   (CTRL_X_CUBE_AI_SPECTROGRAM_COL_OVL*CTRL_X_CUBE_AI_SPECTROGRAM_HOP_LENGTH)

/* Private function prototypes -----------------------------------------------*/
static void IAC_Config(void);
static void MPU_Config(void);
static void Ext_Mem_Config(void);
static void Int_Mem_Config(void);
static void SleepClks_init(void);
static void Error_Handler(void);  
static void Record_Init(void);
static void NPU_SettingsLog(void);

#ifdef CPU_STATS
static void printCpuStats(void);
#endif

#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
static void initAudioPlayBack(AudioBM_play_back_t *ctx_ptr);
static void AudioPlayBack(AudioBM_play_back_t *ctx_ptr, int16_t *pData, \
                                                uint16_t nbSamples);
static float vumeter(int16_t * pAudioSmp,int nb_sample);
#endif

/* Private variables ---------------------------------------------------------*/
static bool AudioProcIsOn;

#ifdef APP_BARE_METAL
static AudioBM_acq_t  audio_acq_ctx;
static AudioBM_proc_t audio_proc_ctx;
#endif
#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
int16_t  playback_buf[PLAYBACK_BUFFER_SIZE] __NON_CACHEABLE;
#endif

/**
  * @brief  Initializes the system according to the application 
  *         system requirement.
  * @param  None
  * @retval None
  */
void init_bm(void)
{
  /* Power on ICACHE */
  MEMSYSCTL->MSCR |= MEMSYSCTL_MSCR_ICACTIVE_Msk;

  /* Set back system and CPU clock source to HSI */
  __HAL_RCC_CPUCLK_CONFIG(RCC_CPUCLKSOURCE_HSI);
  __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_HSI);

  HAL_Init();
  
  SystemClock_Config_Full();

#ifdef APP_DVFS  
  pm_init_dvfs();
#endif
  /* Force fusing of the OTP when using a Nucleo/DK board only */
#if (defined(USE_STM32N6xx_NUCLEO) || defined(USE_STM32N6570_DK))
  fuse_vddio();
#endif
  
  MPU_Config();
  Int_Mem_Config();
  Ext_Mem_Config();
  NPU_Config();
  IAC_Config();
  SCB_EnableICache();
  SCB_EnableDCache();
  SleepClks_init(); /* configures for sleep */

  /* BSP inits */
  UART_Config();
  BSP_PB_Init(BUTTON_USER1, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_TAMP, BUTTON_MODE_EXTI);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /* configuration information on console */
  displaySystemSetting();

  /* by default processing is active */
  AudioProcIsOn = true;
}

#ifdef APP_BARE_METAL
/**
  * @brief  main loop for bare metal implementation.
  * @param  None
  * @retval None
  */
void exec_bm(void)
{
  bool cont = true;
  time_stats_init();

  initAudioProc(&audio_proc_ctx);
  initAudioCapture(&audio_acq_ctx);
  startAudioCapture(&audio_acq_ctx);
  printHeader();

  while(cont)
  {
    __NOP(); /* to be resilient to gcc optim ... to investigate */
#ifdef  APP_LP
    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFI);
    HAL_ResumeTick();
#endif /* APP_LP */
     if (audio_acq_ctx.ring_buff.availableSamples >= AUDIO_ACQ_LEN)
     {
        cont = audio_process(&audio_acq_ctx,&audio_proc_ctx);
#ifdef CPU_STATS
        printCpuStats();
#endif
     }
  }
  stopAudioCapture();

#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
  stopAudioPlayBack();
#endif

  test_dump();
  my_printf("\n\r# End Processing\n\r");
}
#endif
/**
 * @brief  Initializes all Audio processing
 * @param  proc_ctx_ptr pointer to processing context
 * @retval None
 */
void initAudioProc(AudioBM_proc_t * ctx_ptr)
{
  struct npu_model_info *pxInfo;
  /* init test facilities */
  test_init();
  /* get the AI model */
  AiDPULoadModel( &ctx_ptr->aiCtx, CTRL_X_CUBE_AI_MODEL_NAME );
  pxInfo     = &ctx_ptr->aiCtx.net_exec_ctx->info;
  ctx_ptr->ai_in_ptr = (int8_t *) LL_Buffer_addr_start(pxInfo->in_bufs[0]);
  ctx_ptr->ai_out_ptr = pxInfo->out_bufs[0] ;
  /* clear input samples array ( get silence on first overlayed patch */
  memset(ctx_ptr->proc_buff,0,PATCH_LENGTH*sizeof(int16_t));
  /* Audio Preprocessing init */
  PreProc_DPUInit(&ctx_ptr->audioPreCtx);
  /* Audio Postprocessing init */
  PostProc_DPUInit(&ctx_ptr->audioPostCtx);
  /* transfer quantization parametres included in AI model to the Audio DPU   */
  ctx_ptr->audioPreCtx.output_Q_offset    = ctx_ptr->aiCtx.input_Q_offset;
  ctx_ptr->audioPreCtx.output_Q_inv_scale =
            (PREPROC_FLOAT_T) ctx_ptr->aiCtx.input_Q_inv_scale;
  ctx_ptr->audioPreCtx.quant.output_Q_inv_scale = ctx_ptr->audioPreCtx.output_Q_inv_scale;
  ctx_ptr->audioPreCtx.quant.output_Q_offset = ctx_ptr->audioPreCtx.output_Q_offset;
  ctx_ptr->cnt = 0;
#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
  initAudioPlayBack(&ctx_ptr->audioPlayBackCtx);
#endif
}

/**
  * @brief  audio processing - pre, main, and post
  * @param  acq_ctx_ptr pointer to acquisition context
  * @param  proc_ctx_ptr pointer to processing context
  * @retval true if continue condition is met
  */
bool audio_process(AudioBM_acq_t * acq_ctx_ptr,AudioBM_proc_t * proc_ctx_ptr)
{

#if (CTRL_X_CUBE_AI_MODE_OUTPUT_1 == CTRL_AI_CLASS_DISTRIBUTION )
  bool isNotSilence, isPlayback;
#endif

  uint8_t *proc_buf = (uint8_t *) proc_ctx_ptr->proc_buff;
  uint8_t *proc_buf_ovl = (uint8_t *) (&proc_ctx_ptr->proc_buff[AUDIO_ACQ_LEN]);
  uint8_t *acq_buf = (uint8_t *) (&proc_ctx_ptr->proc_buff[AUDIO_ACQ_OFFSET]);
  bool cont = true;

#ifdef APP_DVFS 
  pm_set_opp_min(OPP_MAX);
#endif /* APP_DVFS */

  /* prepare overlapping samples from previous patch */
  memcpy(proc_buf,proc_buf_ovl,AUDIO_ACQ_OFFSET*sizeof(int16_t));

  /* Audio samples acquisition */
  AudioCapture_ring_buff_consume(acq_buf,&acq_ctx_ptr->ring_buff,AUDIO_ACQ_LEN);
#ifdef APP_LP
  NPU_SRAM_on();
#endif /* APP_LP */

  /* Audio pre processing */
  PreProc_DPU(&proc_ctx_ptr->audioPreCtx, proc_buf, proc_ctx_ptr->ai_in_ptr );

#if (CTRL_X_CUBE_AI_MODE_OUTPUT_1 == CTRL_AI_CLASS_DISTRIBUTION )
  isNotSilence = (proc_ctx_ptr->audioPreCtx.S_Spectr.spectro_sum > CTRL_X_CUBE_AI_SPECTROGRAM_SILENCE_THR);
  isPlayback = test_probe_in(&proc_ctx_ptr->aiCtx,&proc_ctx_ptr->audioPreCtx);
#endif    

#ifdef APP_LP      
  NPU_on();
#endif /* APP_LP */
  /* AI processing */
  AiDPUProcess(&proc_ctx_ptr->aiCtx);

#if (CTRL_X_CUBE_AI_MODE_OUTPUT_1 == CTRL_AI_CLASS_DISTRIBUTION )
  if (isNotSilence || isPlayback) printInferenceResults(proc_ctx_ptr->ai_out_ptr);
#endif

#if (CTRL_X_CUBE_AI_POSTPROC==CTRL_AI_ISTFT)
  PostProc_DPU(&proc_ctx_ptr->audioPostCtx,
      proc_ctx_ptr->audioPreCtx.pCplxSpectrum,
      (float32_t *) LL_Buffer_addr_start(proc_ctx_ptr->ai_out_ptr),
      proc_ctx_ptr->audio_out);
#endif

#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
  int16_t * audioPtr = (AudioProcIsOn) ?
      &proc_ctx_ptr->audio_out[AUDIO_OUT_FIRST] : (int16_t *)acq_buf ;
  AudioPlayBack(&proc_ctx_ptr->audioPlayBackCtx, audioPtr , AUDIO_ACQ_LEN);
#endif

#ifdef APP_LP
  NPU_off();
#endif /* APP_LP */

#ifdef APP_DVFS 
  pm_set_opp_min(OPP_MIN);
#endif 

  BSP_LED_Toggle(LED_GREEN);
  
  return cont;
}

/**
* @brief  Initializes Audio capture from microphone
* @param  acq_ctx_ptr pointer to acquisition context
* @retval None
*/
void initAudioCapture(AudioBM_acq_t *ctx_ptr)
{
  ctx_ptr->ring_buff.nbSamples = ((PATCH_LENGTH / CAPTURE_BUFFER_SIZE) + 1 ) \
                                                        * CAPTURE_BUFFER_SIZE ;
  ctx_ptr->ring_buff.nbBytesPerSample = 2;
  ctx_ptr->ring_buff.nbFrames = 2;
  AudioCapture_ring_buff_alloc(&ctx_ptr->ring_buff);
  /* Initialize record */
  Record_Init();
}

/**
* @brief  Starts Audio capture from microphone
* @param  acq_ctx_ptr pointer to acquisition context
* @retval None
*/
void startAudioCapture(AudioBM_acq_t * ctx_ptr)
{
  /* Start record */
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_Record(1, (uint8_t *) ctx_ptr->acq_buf,
                                         CAPTURE_BUFFER_SIZE * sizeof(int16_t)))
  {
    Error_Handler();
  }
}

#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
/**
* @brief  Stops Audio Playback
* @param  None
* @retval None
*/
void stopAudioPlayBack(void)
{
  BSP_AUDIO_OUT_Stop(1);
}
#endif

/**
* @brief  Stops Audio Capture
* @param  None
* @retval None
*/
void stopAudioCapture(void)
{
  BSP_AUDIO_IN_Stop(1);
}

#ifdef APP_LP

/**
* @brief  NPU (Neural Processing Unit) RAM power on.
* @param  None.
* @retval None.
*/
void NPU_SRAM_on(void)
{
  RAMCFG_HandleTypeDef hramcfg = {0};
  hramcfg.Instance =  RAMCFG_SRAM6_AXI;
  HAL_RAMCFG_EnableAXISRAM(&hramcfg);
  __HAL_RCC_AXISRAM6_MEM_CLK_ENABLE();
  __HAL_RCC_CACHEAXIRAM_MEM_CLK_ENABLE();
}

/**
* @brief  NPU (Neural Processing Unit) power ram off.
* @param  None.
* @retval None.
*/
void NPU_SRAM_off(void)
{
  RAMCFG_HandleTypeDef hramcfg = {0};
  hramcfg.Instance =  RAMCFG_SRAM6_AXI;
  HAL_RAMCFG_DisableAXISRAM(&hramcfg);
  __HAL_RCC_AXISRAM6_MEM_CLK_DISABLE();
  __HAL_RCC_CACHEAXIRAM_MEM_CLK_DISABLE();
}

/**
* @brief  NPU (Neural Processing Unit) power on.
* @param  None.
* @retval None.
*/
void NPU_on(void)
{
  NPU_SRAM_on();
  NPU_Config();
  __HAL_RCC_XSPI2_CLK_ENABLE();
}

/**
* @brief  NPU (Neural Processing Unit) power off.
* @param  None.
* @retval None.
*/
void NPU_off(void)
{
  __HAL_RCC_NPU_CLK_DISABLE();
  __HAL_RCC_CACHEAXI_CLK_DISABLE();
  NPU_SRAM_off();
  __HAL_RCC_XSPI2_CLK_DISABLE();
}

#endif /* APP_LP */

#ifdef APP_DVFS
/**
  * @brief Initialize the UART MSP.
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState       = RCC_MSI_ON;
  RCC_OscInitStruct.MSIFrequency   = RCC_MSI_FREQ_4MHZ;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  /*  Select MSI for UART */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_MSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
  __BKPT(0);
  }
}
#endif /* APP_DVFS */

void printHeader(void)
{
	my_printf(SEPARATION_LINE);
	my_printf("# Start Processing\n\r");
	my_printf(SEPARATION_LINE);

#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
	my_printf("| Vu meter          ");
#else
	my_printf("                    ");
#endif
#ifdef CPU_STATS
	my_printf("| Frame   |  Cpu  |  Pre |  AI  | Post |");
#endif
	my_printf("\r\n");
}

#ifdef CPU_STATS
void printCpuStats(void)
{
	/* display real time statistics */
	float pre_load = 100 * time_stats_get_avg(TIME_STAT_PRE_PROC)/CTRL_X_CUBE_AI_ACQ_LENGTH_MS;
	float ai_load = 100 * time_stats_get_avg(TIME_STAT_AI_PROC)/CTRL_X_CUBE_AI_ACQ_LENGTH_MS;
	float post_load = 100 * time_stats_get_avg(TIME_STAT_POST_PROC)/CTRL_X_CUBE_AI_ACQ_LENGTH_MS;
#if (CTRL_X_CUBE_AI_AUDIO_OUT!=COM_TYPE_HEADSET)
	my_printf("                    ");
#endif
	printf ("| %-8d|%6.2f%%|%6.2f|%6.2f|%6.2f|\r",time_stats_get_cnt(TIME_STAT_AI_PROC),pre_load+ai_load+post_load,pre_load, ai_load, post_load);
	fflush(stdout);
}
#endif

#if (CTRL_X_CUBE_AI_MODE_OUTPUT_1 == CTRL_AI_CLASS_DISTRIBUTION )
/**
* @brief  Displays Inference processing outputs
* @param  None      
* @retval None
*/
void printInferenceResults(const LL_Buffer_InfoTypeDef* pBuffRes)
{
  /**
  * Specifies the labels for the classes of the demo.
  */
  
  const char* sAiClassLabels[CTRL_X_CUBE_AI_MODE_CLASS_NUMBER] \
  = CTRL_X_CUBE_AI_MODE_CLASS_LIST;
  float *nn_out =  (float *) LL_Buffer_addr_start(pBuffRes);
  
  uint32_t nn_out_len = get_ll_buffer_size(pBuffRes)/sizeof(float);
  float max_out = nn_out[0];
  uint32_t max_idx = 0;
  for(uint32_t i = 1; i < nn_out_len; i++)
  {
    if(nn_out[i] > max_out)
    {
      max_idx = i;
      max_out = nn_out[i];
    }
  }
#ifdef CPU_STATS
   my_printf("\r\n");
#endif

  if (max_out > CTRL_X_CUBE_AI_OOD_THR )
  {
    my_printf("{\"class\":\"%s\"}",sAiClassLabels[max_idx]);
  }
  else
  {
    my_printf("{\"class\":\"%s\"}","unknown");
  }
  my_printf("\r\n");

}

#endif

/**
* @brief  Displays System Settings
* @param  None
* @retval None
*/
void displaySystemSetting(void)
{
  my_printf("\n\r");
  my_printf(SEPARATION_LINE);
  my_printf("        System configuration (%s)\n\r",APP_CONF_STR);
  my_printf(SEPARATION_LINE);
  printf("\n\rLog Level: %s\n\n\r", getLogLevelStr(LOG_LEVEL));
  systemSettingLog();
  NPU_SettingsLog();
#if defined (APP_DVFS) || defined (APP_LP)
  my_printf("\r\nLow Power Options:");
#ifdef APP_LP
  my_printf("\r\n DPS");
#endif /* APP_LP */
#ifdef APP_DVFS  
  my_printf("\r\n DFVS");
#endif /* APP_DFVS */
  my_printf("\r\n");
#endif /* APP_DVFS || APP_LP */
}

void toggle_audio_proc(void)
{
  BSP_LED_Toggle(LED_RED);
  AudioProcIsOn = !AudioProcIsOn;
}

/**
* @brief  Manages the BSP audio half transfer.
* @param  pHdle pointer to audio ring buffer handle
* @param  pData pointer to audio acquired samples buffer
* @param  half_buf id ( 0 or 1 )
* @retval None.
*/
void AudioCapture_half_buf_cb(AudioCapture_ring_buff_t *pHdle, int16_t *pData, uint8_t half_buf)
{
  int16_t *in_p = pData + half_buf * (CAPTURE_BUFFER_SIZE/ 2);
  AudioCapture_ring_buff_feed(pHdle, (uint8_t *)in_p, CAPTURE_BUFFER_SIZE/ 2);
}

#ifdef APP_BARE_METAL
/**
* @brief  Manage the BSP audio in transfer complete event.
* @param  Instance Audio in instance.
* @retval None.
*/
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  if (Instance == 1U)
  {
    AudioCapture_half_buf_cb(&audio_acq_ctx.ring_buff, audio_acq_ctx.acq_buf, 1U);
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
    AudioCapture_half_buf_cb(&audio_acq_ctx.ring_buff, audio_acq_ctx.acq_buf, 0U);
  }
}
#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)
/**
  * @brief  Tx Transfer completed callbacks.
  * @param  None.
  * @retval None.
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  AudioCapture_ring_buff_consume_no_cpy(&audio_proc_ctx.audioPlayBackCtx.ring_buff, PLAYBACK_BUFFER_SIZE/2);
}

/**
  * @brief  Tx Transfer Half completed callbacks
  * @param  None.
  * @retval None.
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  AudioCapture_ring_buff_consume_no_cpy(&audio_proc_ctx.audioPlayBackCtx.ring_buff, PLAYBACK_BUFFER_SIZE/2);
}
#endif /* (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)  */

/**
* @brief  Manages the BSP audio in error event.
* @param  Instance Audio in instance.
* @retval None.
*/
void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance)
{
  Error_Handler();
}

void BSP_PB_Callback(Button_TypeDef Button)
{
  if (BUTTON_USER1 == Button)
  {
    toggle_audio_proc();
  }
}
#endif

/*==============================================================================
                    private  functions definition
 ============================================================================= */

#if (CTRL_X_CUBE_AI_AUDIO_OUT==COM_TYPE_HEADSET)  
/**
  * @brief  Playback initialization
  * @param  ctx_ptr play back execution context
  * @retval None
  */
static void initAudioPlayBack(AudioBM_play_back_t *ctx_ptr)
{
   BSP_AUDIO_Init_t AudioInit;

  /* Configure playback */ 
  AudioInit.Device        = AUDIO_OUT_DEVICE_HEADPHONE;
  AudioInit.SampleRate    = AUDIO_FREQUENCY;
  AudioInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInit.ChannelsNbr   = 1;
  AudioInit.Volume        = 0; /* not used */

  if (BSP_ERROR_NONE != BSP_AUDIO_OUT_Init(0, &AudioInit))
  {
    Error_Handler();
  }
  ctx_ptr->ring_buff.readSampleIndex  = 0 ;
  ctx_ptr->ring_buff.writeSampleIndex = 0 ;
  ctx_ptr->ring_buff.nbFrames = 1;
  ctx_ptr->ring_buff.nbBytesPerSample = 2;
  ctx_ptr->ring_buff.nbSamples = PLAYBACK_BUFFER_SIZE;
  ctx_ptr->ring_buff.pData = (uint8_t *) playback_buf;
  ctx_ptr->cnt = 0 ;
}

static void AudioPlayBack(AudioBM_play_back_t *ctx_ptr, int16_t *pData,\
    uint16_t nbSamples)
{
  float lev_db = vumeter((int16_t *)pData,nbSamples);
  if (lev_db < CTRL_X_CUBE_AI_AUDIO_OUT_DB_THRESHOLD)
  {
    for (int i=0; i<nbSamples; i++){
      pData[i] = 0;
    }
  }
  AudioCapture_ring_buff_feed(& ctx_ptr->ring_buff,(uint8_t *) pData, nbSamples);

  if (ctx_ptr->cnt== 1) 
  {
    /* Start the playback */
    if (BSP_ERROR_NONE != BSP_AUDIO_OUT_Play(0, ctx_ptr->ring_buff.pData, \
      PLAYBACK_BUFFER_SIZE * sizeof(int16_t)))
    {
      Error_Handler();
    }
  }    
  ctx_ptr->cnt++;
}

/**
 * @brief  Displays level of audio on the console in the from of a colored bar
 * @param  IN pAudioSmp : pointer to audio samples
 * @param  IN nb_samples : number of samples
 * @retval audio level in dB
 */
static float vumeter(int16_t * pAudioSmp,int nb_samples)
{
	float sum=0 ;
	for (int i = 0 ; i < nb_samples ; i ++)
	{
		sum += pAudioSmp[i]*pAudioSmp[i];
	}
	// Float value here corresponds to log10(2**30)
	float lev_db = (float)(10*(log10(sum/(nb_samples)) - 9.03089986F));
	// Casting it back to int and removing scale factor for display
	int lev = (int) (lev_db + 10 * 9.03089986F) / 5;
  lev=(lev<0)? 0 : lev;
  lev=(lev>20)? 20 : lev;
	printf("\r\033[42m");
	for (int i = 0 ; i < lev && i < 6 ; i ++)
	{
		printf(" ");
	}
	printf("\033[43m");
	for (int i = 6 ; i < lev && i < 12 ; i ++)
	{
		printf(" ");
	}
	printf("\033[41m");
	for (int i = 12 ; i < lev ; i ++)
	{
		printf(" ");
	}
	printf("\033[0m");
	for (int i = 0 ; i < 20 - lev ; i ++)
	{
		printf(" ");
	}
	fflush(stdout);
	return lev_db;
}

#endif

static void Int_Mem_Config(void)
{
  RAMCFG_HandleTypeDef hramcfg = {0};

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_CRC_CLK_ENABLE();

#ifdef APP_LP  
/*  hramcfg.Instance =  RAMCFG_SRAM2_AXI;
  HAL_RAMCFG_DisableAXISRAM(&hramcfg);
  HAL_RAMCFG_EnableAXISRAM(&hramcfg); */
  hramcfg.Instance =  RAMCFG_SRAM6_AXI;
  HAL_RAMCFG_EnableAXISRAM(&hramcfg);
  
  __HAL_RCC_CACHEAXIRAM_MEM_CLK_ENABLE();
  __HAL_RCC_AXISRAM6_MEM_CLK_ENABLE();
  
/*  __HAL_RCC_AXISRAM2_MEM_CLK_DISABLE(); */
  __HAL_RCC_AHBSRAM1_MEM_CLK_DISABLE();
  __HAL_RCC_AHBSRAM2_MEM_CLK_DISABLE();
  __HAL_RCC_BKPSRAM_MEM_CLK_DISABLE();

#else /* APP_LP */

  RCC->MEMENR |= RCC_MEMENR_AXISRAM3EN | RCC_MEMENR_AXISRAM4EN | RCC_MEMENR_AXISRAM5EN | RCC_MEMENR_AXISRAM6EN;
  RCC->MEMENR |= RCC_MEMENR_CACHEAXIRAMEN; 
  hramcfg.Instance =  RAMCFG_SRAM2_AXI;
  HAL_RAMCFG_EnableAXISRAM(&hramcfg);
  hramcfg.Instance =  RAMCFG_SRAM3_AXI;
  HAL_RAMCFG_EnableAXISRAM(&hramcfg);
  hramcfg.Instance =  RAMCFG_SRAM4_AXI;
  HAL_RAMCFG_EnableAXISRAM(&hramcfg);
  hramcfg.Instance =  RAMCFG_SRAM5_AXI;
  HAL_RAMCFG_EnableAXISRAM(&hramcfg);
  hramcfg.Instance =  RAMCFG_SRAM6_AXI;
  HAL_RAMCFG_EnableAXISRAM(&hramcfg);

  __HAL_RCC_CACHEAXIRAM_MEM_CLK_ENABLE();
  __HAL_RCC_AXISRAM2_MEM_CLK_ENABLE();
  __HAL_RCC_AXISRAM3_MEM_CLK_ENABLE();
  __HAL_RCC_AXISRAM4_MEM_CLK_ENABLE();
  __HAL_RCC_AXISRAM5_MEM_CLK_ENABLE();
  __HAL_RCC_AXISRAM6_MEM_CLK_ENABLE();
  
#endif  
  /* Allow caches to be activated. Default value is 1, but the current boot sets it to 0 */
  MEMSYSCTL->MSCR |= MEMSYSCTL_MSCR_DCACTIVE_Msk | MEMSYSCTL_MSCR_ICACTIVE_Msk;
}

/**
* @brief  Record initialization
* @param  None
* @retval None
*/
static void Record_Init(void)
{
  BSP_AUDIO_Init_t AudioInit;
  uint32_t         GetData;

  /* Test of state */
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_GetState(1, &GetData)) Error_Handler();
  if (GetData != AUDIO_IN_STATE_RESET) Error_Handler();

  AudioInit.Device        = AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInit.SampleRate    = AUDIO_FREQUENCY;
  AudioInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInit.ChannelsNbr   = 1;
  AudioInit.Volume        = 80; /* Not used */

  if (BSP_ERROR_NONE != BSP_AUDIO_IN_Init(1, &AudioInit))
  {
    Error_Handler();
  }

  /* Test of state */
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_GetState(1, &GetData)) Error_Handler();
  if (GetData != AUDIO_IN_STATE_STOP) Error_Handler();

  /* Test set and get functions */
  if (BSP_ERROR_FEATURE_NOT_SUPPORTED != BSP_AUDIO_IN_SetVolume(1, 10)) Error_Handler();
  /*if (BSP_ERROR_NONE != BSP_AUDIO_IN_SetSampleRate(1, SAI_AUDIO_FREQUENCY_96K)) Error_Handler(); */
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_SetDevice(1, AUDIO_IN_DEVICE_ANALOG_MIC)) Error_Handler();
  if (BSP_ERROR_FEATURE_NOT_SUPPORTED != BSP_AUDIO_IN_SetBitsPerSample(1, AUDIO_RESOLUTION_8B)) Error_Handler();
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_SetBitsPerSample(1, AUDIO_RESOLUTION_16B)) Error_Handler();
  if (BSP_ERROR_FEATURE_NOT_SUPPORTED != BSP_AUDIO_IN_SetBitsPerSample(1, AUDIO_RESOLUTION_24B)) Error_Handler();
  if (BSP_ERROR_FEATURE_NOT_SUPPORTED != BSP_AUDIO_IN_SetBitsPerSample(1, AUDIO_RESOLUTION_32B)) Error_Handler();
  if (BSP_ERROR_FEATURE_NOT_SUPPORTED != BSP_AUDIO_IN_SetChannelsNbr(1, 2)) Error_Handler();
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_SetChannelsNbr(1, 1)) Error_Handler();

  if (BSP_ERROR_FEATURE_NOT_SUPPORTED != BSP_AUDIO_IN_GetVolume(1, &GetData)) Error_Handler();
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_GetSampleRate(1, &GetData)) Error_Handler();
  /*if (GetData != AUDIO_FREQUENCY_96K) Error_Handler();*/
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_GetDevice(1, &GetData)) Error_Handler();
  if (GetData != AUDIO_IN_DEVICE_DIGITAL_MIC) Error_Handler();
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_GetBitsPerSample(1, &GetData)) Error_Handler();
  if (GetData != AUDIO_RESOLUTION_16B) Error_Handler();
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_GetChannelsNbr(1, &GetData)) Error_Handler();
  if (GetData != 1U) Error_Handler();

  /* Set the initial sample rate */
  if (BSP_ERROR_NONE != BSP_AUDIO_IN_SetSampleRate(1, AudioInit.SampleRate)) Error_Handler();
}

static void NPU_SettingsLog(void)
{
    struct mcu_conf sys_conf;
    getSysConf(&sys_conf);
    my_printf("\n\rNPU Runtime configuration...\r\n");
    my_printf(" NPU clock    : %u MHz\r\n", (int)sys_conf.extra[1]/1000000);
    my_printf(" NIC clock    : %u MHz\r\n", (int)sys_conf.extra[2]/1000000);
}

/**
* @brief  external memories configuration (Flash & RAM).
* @param  None.
* @retval None.
*/
static void Ext_Mem_Config(void)
{
  BSP_XSPI_NOR_Init_t Flash;
  Flash.InterfaceMode = MX66UW1G45G_OPI_MODE;
  Flash.TransferRate = MX66UW1G45G_DTR_TRANSFER;
  
  if(BSP_XSPI_NOR_Init(0, &Flash) != BSP_ERROR_NONE)
  {
    __BKPT(0);
  }
  BSP_XSPI_NOR_EnableMemoryMappedMode(0);
  MODIFY_REG(XSPI2->CR, XSPI_CR_NOPREF, HAL_XSPI_AUTOMATIC_PREFETCH_DISABLE); /* Hotfix for xspi: no prefetch */

#ifdef USE_EXT_SRAM
  BSP_XSPI_RAM_Init(0);
  BSP_XSPI_RAM_EnableMemoryMappedMode(0);
  MODIFY_REG(XSPI1->CR, XSPI_CR_NOPREF, HAL_XSPI_AUTOMATIC_PREFETCH_DISABLE); /* Hotfix for xspi: no prefetch */
#endif
  
}
static void IAC_Config(void)
{
/* Configure IAC to trap illegal access events */
  __HAL_RCC_IAC_CLK_ENABLE();
  __HAL_RCC_IAC_FORCE_RESET();
  __HAL_RCC_IAC_RELEASE_RESET();
}

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef default_config = {0};
  MPU_Attributes_InitTypeDef attr_config = {0};
  uint32_t primask_bit = __get_PRIMASK();
  __disable_irq();
  /* disable the MPU */
  HAL_MPU_Disable();
  /* create an attribute configuration for the MPU */
  attr_config.Attributes = INNER_OUTER(MPU_NOT_CACHEABLE);
  attr_config.Number = MPU_ATTRIBUTES_NUMBER0;
  HAL_MPU_ConfigMemoryAttributes(&attr_config);
  /* Create a non cacheable region */
  /*Normal memory type, code execution allowed */
  default_config.Enable = MPU_REGION_ENABLE;
  default_config.Number = MPU_REGION_NUMBER0;
  default_config.BaseAddress = __NON_CACHEABLE_SECTION_BEGIN;
  default_config.LimitAddress = __NON_CACHEABLE_SECTION_END;
  default_config.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  default_config.AccessPermission = MPU_REGION_ALL_RW;
  default_config.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  default_config.AttributesIndex = MPU_ATTRIBUTES_NUMBER0;
  HAL_MPU_ConfigRegion(&default_config);
  /* enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
  /* Exit critical section to lock the system and avoid any issue around MPU mechanisme */
  __set_PRIMASK(primask_bit);
}

static void SleepClks_init(void)
{
  /* Keep all IP's enabled during WFE so they can wake up CPU. Fine tune
     this if you want to save maximum power
     this is specially needed when  LL_ATON_RT_MODE=LL_ATON_RT_ASYNC is used
  */
  LL_BUS_EnableClockLowPower(~0);
  LL_MEM_EnableClockLowPower(~0);
  LL_AHB1_GRP1_EnableClockLowPower(~0);
  LL_AHB2_GRP1_EnableClockLowPower(~0);
  LL_AHB3_GRP1_EnableClockLowPower(~0);
  LL_AHB4_GRP1_EnableClockLowPower(~0);
  LL_AHB5_GRP1_EnableClockLowPower(~0);
  LL_APB1_GRP1_EnableClockLowPower(~0);
  LL_APB1_GRP2_EnableClockLowPower(~0);
  LL_APB2_GRP1_EnableClockLowPower(~0);
  LL_APB4_GRP1_EnableClockLowPower(~0);
  LL_APB4_GRP2_EnableClockLowPower(~0);
  LL_APB5_GRP1_EnableClockLowPower(~0);
  LL_MISC_EnableClockLowPower(~0);
}

/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
static void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

