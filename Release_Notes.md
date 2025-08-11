# Release Notes for STM32N6_GettingStarted_Audio Application

## Purpose

This project provides an STM32 Microcontroler embedded real time environement
to execute [X-CUBE-AI](https://www.st.com/en/embedded-software/x-cube-ai.html)
generated model targetting audio applications. The purpose of this package is
to stream physical data acquired by sensors into a processing chain including a
preprocessing step that typically would perform a first level of feature
extraction, the machine learning inference itself, and a post processing step
before exposing the results to the user in real time.

## Key Features

- Deployement through ST Model Zoo
- Support of Audio Event Detection (AED) using Yamnet 1024
- Low power support:
  - Dynamic Voltage & Frequency Scaling (DFVS)
  - Dynamic Power Scaling (DPS)
- Bare Metal (BM) Implementation
- Real Time Operating System (RTOS) Implementation
- Random load generator
- Audio bypass
- Audio loop back

## Software components

| Name                                | Version    | Release notes
|-----                                | -------    | -------------
| STEdge AI runtime                   |  v10.2.0   |
| STM32 AI AudioPreprocessing Library |  v1.2.0    | [release notes](Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Release_Notes.html)
| FreeRTOS kernel                     |  v10.6.2   | [release notes](Middlewares/ST/FreeRTOS/Source/History.txt)
| CMSIS                               |  v5.9.0    | [release notes](Drivers/CMSIS/Documentation/index.html)
| STM32N6xx CMSIS Device              |  v1.1.0    | [release notes](Drivers/CMSIS/Device/ST/STM32N6xx/Release_Notes.html)
| STM32N6xx HAL/LL Drivers            |  v1.1.0    | [release notes](Drivers/STM32N6xx_HAL_Driver/Release_Notes.html)
| STM32N6570-DK BSP Drivers           |  v1.1.0    | [release notes](Drivers/BSP/STM32N6570-DK/Release_Notes.html)
| BSP Component aps256xx              |  v1.0.6    | [release notes](Drivers/BSP/Components/aps256xx/Release_Notes.html)
| BSP Component cs42l51               |  v2.0.6    | [release notes](Drivers/BSP/Components/cs42l51/Release_Notes.html)
| BSP Component mx66uw1g45g           |  v1.1.0    | [release notes](Drivers/BSP/Components/mx66uw1g45g/Release_Notes.html)
| BSP Component wm8904                |  v1.1.0    | [release notes](Drivers/BSP/Components/wm8904/Release_Notes.html)
| BSP Component Common                |  v7.3.0    | [release notes](Drivers/BSP/Components/Common/Release_Notes.html)

## Update history

### V2.1.0

- Migrate ThreadX project to freertos project
- Upgrade to STEdgeAI 2.2.0

### V2.0.0 / May 2025

- upgraded to
  - Cube Firmware N6 V 1.1
  - STEdge AI V 2.1
  - STM32CUBEIDE V 1.18.1

### V1.0.0 / December 2024

- Support of Audio Event Detection (AED) using Yamnet 1024
- Low power support:
  - Dynamic Voltage & Frequency Scaling (DFVS)
  - Dynamic Power Scaling (DPS)
- Bare Metal (BM) Implementation
- Real Time Operating System (RTOS) Implementation
- Audio loop Back
- OTP management
