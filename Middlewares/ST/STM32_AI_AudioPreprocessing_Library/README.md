# STM32 Audio preprocessing library

This project provides some functions related to audio preprocessing on STM32.

Audio preprocessing is performing some floating point intermediate computation.
Usual float format used is float32_t but float16_t type may also be used updating all structure from <structure_name> to <structure_name>_f16

## Yamnet 256 model preprocessing

The library allows to perform computation either using f32 which the floatting point format enabled on most stm32 MCU with FPU but also with f16 (half float) for some MCU owning this feature (Helium extension for instance).
Below is shown a current usage of the library to perform preprocessing for Yamnet 256 NN model.

### Defines
```C
#define AUDIO_PREPROC_AI_SENSOR_ODR                (16000.0F)
#define AUDIO_PREPROC_AI_SPECTROGRAM_NMEL          (64U)
#define AUDIO_PREPROC_AI_SPECTROGRAM_COL           (96U)
#define AUDIO_PREPROC_AI_SPECTROGRAM_HOP_LENGTH    (160U)
#define AUDIO_PREPROC_AI_SPECTROGRAM_NFFT          (512U)
#define AUDIO_PREPROC_AI_SPECTROGRAM_WINDOW_LENGTH (400U)
#define AUDIO_PREPROC_AI_SPECTROGRAM_MEL_LUT_FILTERLENGTH (461U)
#define AUDIO_PREPROC_AI_SPECTROGRAM_NORMALIZE     (0U)
#define AUDIO_PREPROC_AI_SPECTROGRAM_FORMULA       (MEL_HTK)
#define AUDIO_PREPROC_AI_SPECTROGRAM_FMIN          (125U)
#define AUDIO_PREPROC_AI_SPECTROGRAM_FMAX          (7500U)
#define AUDIO_PREPROC_AI_SPECTROGRAM_TYPE          (SPECTRUM_TYPE_MAGNITUDE)
#define AUDIO_PREPROC_AI_SPECTROGRAM_LOG_FORMULA   (LOGMELSPECTROGRAM_SCALE_LOG) 
#define AUDIO_PREPROC_AI_SPECTROGRAM_SILENCE_THR   (100)
#define AUDIO_PREPROC_AI_SPECTROGRAM_WIN           (float32_t userDefinedWindow[AUDIO_PREPROC_AI_SPECTROGRAM_WINDOW_LENGTH])
#define AUDIO_PREPROC_AI_SPECTROGRAM_MEL_LUT       (float32_t userMelFilterLut[AUDIO_PREPROC_AI_SPECTROGRAM_MEL_LUT_FILTERLENGTH])
#define AUDIO_PREPROC_AI_SPECTROGRAM_MEL_START_IDX (user_melFiltersStartIndices)
#define AUDIO_PREPROC_AI_SPECTROGRAM_MEL_STOP_IDX  (user_melFiltersStopIndices)
```


> Changes to switch to f16
>
> | float32_t                 | float16_t |
> | -----------               | ----------- |
> | float32_t userDefinedWindow[]  | float16_t userDefinedWindow[]   |
> | float32_t userMelFilterLut[] | float16_t userMelFilterLut[] |


### Variables
```C
  arm_rfft_fast_instance_f32    S_Rfft;
  MelFilterTypeDef              S_MelFilter;
  SpectrogramTypeDef            S_Spectr;
  MelSpectrogramTypeDef         S_MelSpectr;
  LogMelSpectrogramTypeDef      S_LogMelSpectr;

  float32_t pSpectrScratchBuffer1[AUDIO_PREPROC_AI_SPECTROGRAM_NFFT];
  float32_t pSpectrScratchBuffer2[AUDIO_PREPROC_AI_SPECTROGRAM_NFFT];

  float32_t output_Q_inv_scale;
  int8_t    output_Q_offset;
```

> Changes to switch to f16
> 
> | float32_t                 | float16_t |
> | -----------               | ----------- |
> | arm_rfft_fast_instance_f32    | arm_rfft_fast_instance_f16       |
> | float32_t | float16_t |


### Initialization phase
```C
   /* Init phase */
  arm_rfft_fast_init_f32(&S_Rfft, AUDIO_PREPROC_AI_SPECTROGRAM_NFFT);  

  /* Init FFT */
  S_Spectr.pRfft                    = &S_Rfft;

  /* Init Spectrogram */
  S_Spectr.Type                     = AUDIO_PREPROC_AI_SPECTROGRAM_TYPE; // SPECTRUM_TYPE_POWER; //
  S_Spectr.pWindow                  = (float32_t *) userDefinedWindow;
  S_Spectr.SampRate                 = AUDIO_PREPROC_AI_SENSOR_ODR;
  S_Spectr.FrameLen                 = AUDIO_PREPROC_AI_SPECTROGRAM_WINDOW_LENGTH;
  S_Spectr.FFTLen                   = AUDIO_PREPROC_AI_SPECTROGRAM_NFFT;
  S_Spectr.pScratch1                = pSpectrScratchBuffer1;
  S_Spectr.pScratch2                = pSpectrScratchBuffer2;
  pad                               = AUDIO_PREPROC_AI_SPECTROGRAM_NFFT - AUDIO_PREPROC_AI_SPECTROGRAM_WINDOW_LENGTH;
  S_Spectr.pad_left                 = pad/2;
  S_Spectr.pad_right                = (pad+1)/2;

  /* Init mel filterbank */
  S_MelFilter.pStartIndices         = (uint32_t *)  AUDIO_PREPROC_AI_SPECTROGRAM_MEL_START_IDX;
  S_MelFilter.pStopIndices          = (uint32_t *)  AUDIO_PREPROC_AI_SPECTROGRAM_MEL_STOP_IDX;
  S_MelFilter.pCoefficients         = (float32_t *) userMelFilterLut;
  S_MelFilter.NumMels               = AUDIO_PREPROC_AI_SPECTROGRAM_NMEL;
  S_MelFilter.FFTLen                = AUDIO_PREPROC_AI_SPECTROGRAM_NFFT;
  S_MelFilter.SampRate              = (uint32_t) AUDIO_PREPROC_AI_SENSOR_ODR;
  S_MelFilter.FMin                  = (float32_t) AUDIO_PREPROC_AI_SPECTROGRAM_FMIN;
  S_MelFilter.FMin                  = (float32_t) AUDIO_PREPROC_AI_SPECTROGRAM_FMIN;
  S_MelFilter.FMax                  = (float32_t) AUDIO_PREPROC_AI_SPECTROGRAM_FMAX;
  S_MelFilter.Formula               = AUDIO_PREPROC_AI_SPECTROGRAM_FORMULA;
  S_MelFilter.Normalize             = AUDIO_PREPROC_AI_SPECTROGRAM_NORMALIZE;
  S_MelFilter.Mel2F                 = 1U;  
  
  /* Init MelSpectrogram */
  S_MelSpectr.SpectrogramConf       = &S_Spectr;  
  S_MelSpectr.MelFilter             = &S_MelFilter;  
  
  /* Init LogMelSpectrogram */
  S_LogMelSpectr.MelSpectrogramConf = &S_MelSpectr;  
  S_LogMelSpectr.LogFormula         = AUDIO_PREPROC_AI_SPECTROGRAM_LOG_FORMULA;  
  S_LogMelSpectr.Ref                = (float32_t)1.0f;  
  S_LogMelSpectr.TopdB              = HUGE_VALF;  
```
> Changes to switch to f16  
>
> | float32_t                 | float16_t |
> | -----------               | ----------- |
> | arm_rfft_fast_init_f32    | arm_rfft_fast_init_f16       |
> | float32_t | float16_t |

### Running loop
```C
/* processing loop */  

/* Loop */  

/* processing phase \*/   
int8_t  tmp_out_i8[AUDIO_PREPROC_AI_SPECTROGRAM_NMEL];  
int8_t output_Q_offset = <Q_value>;  
float32_t output_Q_inv_scale = <value>;  
/********************  
/* processing  
/* loop over columns  
********************/  
for (int i = 0; i < AUDIO_PREPROC_AI_SPECTROGRAM_COL; i++ )  
{  
 // Process one column  
 LogMelSpectrogramColumn_q15_Q8(p_LogMelSpectr, p_in_q15, tmp_out_i8, output_Q_offset, output_Q_inv_scale);  
 p_in_q15 += AUDIO_PREPROC_AI_SPECTROGRAM_HOP_LENGTH;  
 // transpose  
 for (int j=0 ; j < pS_MelFilter.NumMels ; j++ ) {  
      p_out_i8[i+AUDIO_PREPROC_AI_SPECTROGRAM_COL*j]= tmp_out_i8[j];  
 }  
}  
```
> Changes to switch to f16  
>
> | float32_t                 | float16_t |
> | -----------               | ----------- |
> | LogMelSpectrogramColumn_q15_Q8    | LogMelSpectrogramColumn_f16_q15_Q8       |
> | float32_t | float16_t |





| float32_t                 | float16_t |
| -----------               | ----------- |
| arm_rfft_fast_init_f32    | arm_rfft_fast_init_f16       |



for float16_t, replace all _f32 function calls by _f16 equivalent one (for functions without suffix _f16 is added)

## Speech enhancement pre/post processing

The library allows to perform computation either using f32 which the floating point format enabled on most stm32 MCU with FPU but also with f16 (half float) for some MCU owning this feature (Helium extension for instance).
Below is shown a current usage of the library to perform preprocessing for speech enhancement NN pre and post process.

### Defines
```C
#define AUDIO_AI_PP_SPE_NB_FRAMES      (20U)
#define AUDIO_AI_PP_SPE_HOP_LENGTH    (160U)
#define AUDIO_AI_PP_SPE_NFFT           (512U)
#define AUDIO_AI_PP_SPE_WINDOW_LENGTH  (400U)
#define AUDIO_AI_PP_SPE_WIN            (float32_t userDefinedWindow[AUDIO_AI_PP_SPE_WINDOW_LENGTH])
#define AUDIO_AI_PP_SPE_QUANT_SCALE    ((float32_t)0.005f)
#define AUDIO_AI_PP_SPE_QUANT_OFFSET   (int8_t)-128)
```

> Changes to switch to f16  
>
> | float32_t                 | float16_t |
> | -----------               | ----------- |
> | float32_t userDefinedWindow[]  | float16_t userDefinedWindow[]   |

### Variables
```C  
  arm_rfft_fast_instance_f32    S_Rfft;   
  FFTTypeDefFloat_t FFT_s;
  float32_t pScratchBufferFFT[AUDIO_AI_PP_SPE_NFFT];
  float32_t pScratchBuffer[AUDIO_AI_PP_SPE_NFFT];
  
  /* Preproc Only from here */
  AudioToFFTBatch_t sAudioSpePre;
  AudioSpEExtraPre_t sAudioSpeExraPre;
  /*if quantized preprocessing output */
  quantParam_t Quant;
  /*if normalization in preproc extra */
  AudioSpENormalize_t sAudioNormalize;
  
  /* Preproc Only done */
  
  /* Postproc Only from now */
  AudioSpEPostProc_t sAudioSpePost;
  float32_t pScratchBufferSumDataOut[AUDIO_AI_PP_SPE_WINDOW_LENGTH];
  float32_t pScratchBufferSumWindow[AUDIO_AI_PP_SPE_WINDOW_LENGTH];
  /* Postproc Only done */

```

> Changes to switch to f16
>
> | float32_t                 | float16_t |
> | -----------               | ----------- |
> | arm_rfft_fast_instance_f32    | arm_rfft_fast_instance_f16       |
> | FFTTypeDefFloat_t   |   FFTTypeDefFloat_f16_t
> | AudioToFFTBatch_t   | AudioToFFTBatch_f16_t
> | AudioSpEExtraPre_t  | AudioSpEExtraPre_f16_t
> | AudioSpEPostProc_t  | AudioSpEPostProc_f16_t
> | quantParam_t        | quantParam_f16_t
> | float32_t | float16_t |

### Initialization phase
```C
   /* Init phase */
  arm_rfft_fast_init_f32(&S_Rfft, AUDIO_AI_PP_SPE_NFFT);

  /* Init FFT */
  FFT_s.pRfft                       = &S_Rfft;
  FFT_s.FFTLen                      = AUDIO_AI_PP_SPE_NFFT;
  FFT_s.pScratch                    = pScratchBufferFFT;
  FFT_s.pWindow                     = (FLOAT_T *) AUDIO_AI_PP_SPE_WIN;
  FFT_s.sDin.FrameLen               = AUDIO_AI_PP_SPE_WINDOW_LENGTH;
  pad                               = AUDIO_AI_PP_SPE_NFFT - AUDIO_AI_PP_SPE_WINDOW_LENGTH;  
  FFT_s.sDin.pad_left               = pad/2;
  FFT_s.sDin.pad_right              = (pad+1)/2;

  /* Preprocessing only from now */
  /*******************************/
  sAudioSpePre.pFFT                 = &FFT_s;
  sAudioSpePre.pScratch             = pScratchBuffer;
  sAudioSpePre.hop_length           = AUDIO_AI_PP_SPE_HOP_LENGTH;
  sAudioSpePre.nb_frames            = AUDIO_AI_PP_SPE_NB_FRAMES;
  sAudioSpePre.transpose            = E_SPECTROGRAM_CPLX_FFT_TRANSPOSE;

  /* optional: to add normalization in audio_spe_preprocess_extra */
  sAudioNormalize.maxNormValue      = INT16_MAX; // values from spectrgram will be multiplied by maxNormValue/audioMax (audioMax is the maximum audio sample value for the patch, registered during preprocessing)
  sAudioNormalize.minThreshold      = 2000; // example: normalization would be applied only if audioMax is greater than minThreshold
  sAudioSpePre.pNorm                = &sAudioNormalize; // if normalization in preproc extra else NULL;

  /* if quantized output */
  Quant.output_Q_inv_scale          = 1.0f/AUDIO_AI_PP_SPE_QUANT_SCALE;
  Quant.output_Q_offset             = AUDIO_AI_PP_SPE_QUANT_OFFSET;
  sAudioSpeExraPre.pQuant           = &Quant; // if quantized output else NULL;
  /* if quantized output done */

  /* Preprocessing extra : from Complex FFT to real FFT */
  sAudioSpeExraPre.pFFT             = &FFT_s;
  sAudioSpeExraPre.nb_frames        = AUDIO_AI_PP_SPE_NB_FRAMES;
  sAudioSpeExraPre.eType            = SPECTRUM_TYPE_MAGNITUDE;
  sAudioSpeExraPre.pScratch         = pScratchBuffer;
  sAudioSpeExraPre.transposeIn      = E_SPECTROGRAM_CPLX_FFT_TRANSPOSE;
  sAudioSpeExraPre.transposeOut     = E_SPECTROGRAM_TRANSPOSE;
  /* optional: to add normalization in audio_spe_preprocess_extra */
  sAudioSpeExraPre.pNorm            = &sAudioNormalize; // if normalization in preproc extra else NULL;
  /* Preprocessing only done */
  /***************************/
  

  /* Postprocessing only from now */
  /********************************/
  sAudioSpePost.pFFT                = &FFT_s;
  sAudioSpePost.pScratch            = pScratchBuffer;
  sAudioSpePost.pWeightsWindow      = pScratchBufferSumDataOut;
  sAudioSpePost.pScratchOut         = pScratchBufferSumWindow;
  sAudioSpePost.hop_length          = AUDIO_AI_PP_SPE_HOP_LENGTH;
  sAudioSpePost.nb_frames           = AUDIO_AI_PP_SPE_NB_FRAMES;
  sAudioSpePost.idxPos              = 0;
  sAudioSpePost.transposeSigIn      = E_SPECTROGRAM_CPLX_FFT_TRANSPOSE; // or E_SPECTROGRAM_CPLX_FFT 
  sAudioSpePost.transposeMask       = E_SPECTROGRAM_TRANSPOSE; // or E_SPECTROGRAM_TRANSPOSE_NO 
  /* Posprocessing only done */
  /***************************/
```


### Running loop

```C
/* pre processing phase \*/   
float32_t  preprocOut[(AUDIO_AI_PP_SPE_NFFT/2 + 1) * 2 * AUDIO_AI_PP_SPE_NB_FRAMES];  // 257 (nFFT) * 20 (nbFRames) Cplx in a case
float32_t  outToModel[(AUDIO_AI_PP_SPE_NFFT/2 + 1) * AUDIO_AI_PP_SPE_NB_FRAMES];  // 257 (nFFT) * 20 (nbFRames) in a case
/********************  
/* processing  
********************/  
   audio_is16oCplxFFTf32(p_in, &sAudioSpePre, preprocOut, sAudioSpePre.nb_frames);
   
   audio_spe_preprocess_extra(preprocOut, &sAudioSpeExraPre, outToModel);
```

> Changes to switch to f16  
>
> | float32_t                 | float16_t |
> | -----------               | ----------- |
> | audio_is16oCplxFFTf32    | audio_is16oCplxFFTf16       |
> | float32_t | float16_t |


```C

/* post processing phase \*/   
float32_t preprocOut[(AUDIO_AI_PP_SPE_NFFT/2 + 1) * 2 * AUDIO_AI_PP_SPE_NB_FRAMES];  // 257 (nFFT) * 20 (nbFRames) * 2 (Re + Im parts) in a case
float32_t NN_modelOutput[(AUDIO_AI_PP_SPE_NFFT/2 + 1) * AUDIO_AI_PP_SPE_NB_FRAMES];
int16_t audioOut[AUDIO_AI_PP_SPE_NB_FRAMES * AUDIO_AI_PP_SPE_HOP_LENGTH]
/********************  
/* processing  
********************/  
   audio_iCplxFFTf32xf32os16(preprocOut, pMaskFromModel, &sAudioSpePost, audioOut, sAudioSpePost.nb_frames);
```

> Changes to switch to f16  
>
> | float32_t                 | float16_t |
> | -----------               | ----------- |
> | audio_iCplxFFTf32xf32os16    | audio_iCplxFFTf16xf16os16       |
> | float32_t | float16_t |


