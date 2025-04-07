# __Audio Getting started ModelZoo__

This project provides an STM32 microcontroller embedded real-time environment to
execute [X-CUBE-AI](https://www.st.com/en/embedded-software/x-cube-ai.html)
generated models targeting audio application. The code developement is lead by its understandability rather than its performance, and is intended to be used as a starting point for further development.

## __Software environment__

To deploy the application on your board, you need to download the
STM32N6_GettingStarted software package from the
[ST website](https://www.st.com/en/development-tools/stm32n6-ai.html). Download
it and unzip it. Then copy/paste the
`STM32N6_GettingStarted_V1.0.0/application_code` folder into the root folder of
the ModelZoo (`model_zoo_services/`). This will fill all the application
software of each use case and needs only to be done once.
You can find the installation information in the parent [README](../../../audio_event_detection/deployment/README.md)
of the deployment part and
the general [README](../../README.md) of the model zoo.

## __Hardware environment__

To run these audio application examples, you need the following
hardware:

- [STM32N6570-DK](https://www.st.com/en/evaluation-tools/stm32n6570-dk.html)
 discovery board

## __Tools installations__

This getting started requires
[STM32CubeIDE](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-ides/stm32cubeide.html) as well as
[X-CUBE-AI](https://www.st.com/en/embedded-software/x-cube-ai.html) (from
v10.0.0 to the latest version).
