# Audio event detection Getting Started header generation scripts

These scripts are derived from [STM32 model zoo](https://github.com/STMicroelectronics/stm32ai-modelzoo) used for Audio Event Detection deployment on target.
The purpôse is to generate the following files:

- user_mel_tables.c
- user_mel_tables.h

## Python enviromnent

`requirements.txt` is a file that contains a list of packages or libraries needed to work. To install those simply type:

```bash
pip install -r requirements.txt
```

## Configure the yaml file

You can refer to the AED configuration Yaml file [documentation](https://github.com/STMicroelectronics/stm32ai-modelzoo/blob/main/audio_event_detection/scripts/deployment/README.md).

The relevant section to update is feature_extraction (any other parameters are discarded):

### feature_extraction

```python
pre_processing:
  target_rate: 16000

feature_extraction:
  n_mels: 32
  n_fft: 512
  window_length: 400
  window: hann
  fmin: 50
  fmax: 7950
  norm: None
  htk : True
  ```

  where :
  
- `target_rate` - Desired sampling rate.
- `n_mels` - Number of mel filter bins. Patches will be of shape `n_mels` x `patch_length`
- `n_fft` - Size of the FFT, in number of samples. Must be a power of 2 for deployment.
- `window_length` - Size of the window. Set equal to `n_fft` if you want to avoid window padding
- `window` - Window type. Currently, only Hanning windows (`hann`) are supported for deployment.
- `fmin` - Minimum frequency used when computing mel filter bins
- `fmax` - Maximum frequency used when computing mel filter bins
- `norm` - Mel filter weights normalization. Set to "slaney" if you are unsure. Some models, like Yamnet, expect no normalization
- `htk` - If true, use the [HTK](https://htk.eng.cam.ac.uk/) formula to compute mel filter weights. Set to "False" if you are unsure. Some models, like Yamnet, expect this to be True.
