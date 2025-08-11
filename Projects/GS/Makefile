######################################
# target
######################################
TARGET = GS_Audio_N6
OPT = -g3 -Os

#######################################
# paths
#######################################
# Build path
BUILD_DIR_BASE := BuildGCC

######################################
# source
######################################
PATCH_DIR = ../../Projects/Common/Patch/

C_SOURCES = \
../../Projects/Common/Patch/stm32n6570_discovery_audio.c\

C_SOURCES += \
../../Projects/Common/logging.c\
../../Projects/Common/misc_toolbox.c\
../../Projects/Common/pm_dvfs.c\
../../Projects/Common/cpu_stats.c\
../../Projects/Common/system_clock_config.c\

C_SOURCES += \
../../Projects/Dpu/ai_dpu.c\
../../Projects/Dpu/postproc_dpu.c\
../../Projects/Dpu/preproc_dpu.c\
../../Projects/Dpu/user_mel_tables.c\

C_SOURCES += \
../../Projects/GS/Src/AudioCapture_ring_buff.c\
../../Projects/GS/Src/audio_bm.c\
../../Projects/GS/Src/main.c\
../../Projects/GS/Src/stm32n6xx_it.c\
../../Projects/GS/Src/syscalls.c\
../../Projects/GS/Src/sysmem.c\
../../Projects/GS/Src/test.c\

C_SOURCES += \
../../Projects/X-CUBE-AI/aiTestUtility.c\
../../Projects/X-CUBE-AI/ai_device_adaptor.c\
../../Projects/X-CUBE-AI/atonn/ai_io_buffers.c\
../../Projects/X-CUBE-AI/atonn/ai_wrapper_ATON.c\
../../Projects/X-CUBE-AI/models/network.c\

C_SOURCES += \
../../Drivers/BSP/Components/aps256xx/aps256xx.c\

C_SOURCES += \
../../Drivers/BSP/Components/cs42l51/cs42l51.c\
../../Drivers/BSP/Components/cs42l51/cs42l51_reg.c\

C_SOURCES += \
../../Drivers/BSP/Components/mx66uw1g45g/mx66uw1g45g.c\

C_SOURCES += \
../../Drivers/BSP/Components/wm8904/wm8904.c\
../../Drivers/BSP/Components/wm8904/wm8904_reg.c\

C_SOURCES += \
../../Drivers/BSP/STM32N6570-DK/stm32n6570_discovery.c\
../../Drivers/BSP/STM32N6570-DK/stm32n6570_discovery_bus.c\
../../Drivers/BSP/STM32N6570-DK/stm32n6570_discovery_xspi.c\

C_SOURCES += \
../../Drivers/CMSIS/DSP/Source/BasicMathFunctions/BasicMathFunctions.c\
../../Drivers/CMSIS/DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.c\
../../Drivers/CMSIS/DSP/Source/CommonTables/CommonTables.c\
../../Drivers/CMSIS/DSP/Source/CommonTables/CommonTablesF16.c\
../../Drivers/CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.c\
../../Drivers/CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctionsF16.c\
../../Drivers/CMSIS/DSP/Source/StatisticsFunctions/StatisticsFunctions.c\
../../Drivers/CMSIS/DSP/Source/StatisticsFunctions/StatisticsFunctionsF16.c\
../../Drivers/CMSIS/DSP/Source/SupportFunctions/SupportFunctions.c\
../../Drivers/CMSIS/DSP/Source/SupportFunctions/SupportFunctionsF16.c\
../../Drivers/CMSIS/DSP/Source/TransformFunctions/TransformFunctions.c\
../../Drivers/CMSIS/DSP/Source/TransformFunctions/TransformFunctionsF16.c\

C_SOURCES += \
../../Drivers/CMSIS/Device/ST/STM32N6xx/Source/Templates/system_stm32n6xx_fsbl.c\

C_SOURCES += \
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_bsec.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_cacheaxi.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_cortex.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_dma.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_dma_ex.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_exti.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_gpio.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_i2c.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_i2c_ex.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_icache.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_mdf.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_mdios.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_pwr.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_pwr_ex.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_ramcfg.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_rcc.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_rcc_ex.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_rif.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_sai.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_sdio.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_uart.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_xspi.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_util_i3c.c\
../../Drivers/STM32N6xx_HAL_Driver/Src/stm32n6xx_hal_tim.c\

C_SOURCES += \
../../Middlewares/ST/AI/Npu/Devices/STM32N6XX/mcu_cache.c\
../../Middlewares/ST/AI/Npu/Devices/STM32N6XX/npu_cache.c\
../../Middlewares/ST/AI/Npu/ll_aton/ecloader.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_cipher.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_dbgtrc.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_debug.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_lib.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_lib_sw_operators.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_osal_freertos.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_profiler.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_rt_main.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_runtime.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_aton_util.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_sw_float.c\
../../Middlewares/ST/AI/Npu/ll_aton/ll_sw_integer.c\

C_SOURCES += \
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/audio_din.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/audio_din_f16.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/audio_prePost_process.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/audio_prePost_process_f16.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/common_tables.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/common_tables_f16.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/dct.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/dct_f16.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/feature_extraction_f16.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank_f16.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/window.c\
../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/window_f16.c\

# ASM sources
AS_SOURCES = ../../Drivers/CMSIS/Device/ST/STM32N6xx/Source/Templates/gcc/startup_stm32n657xx.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
GCC_PATH =
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
READELF = $(GCC_PATH)/$(PREFIX)readelf
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
CP = $(PREFIX)objcopy
READELF = $(PREFIX)readelf
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary

FLASHER = STM32_Programmer_CLI
SIGNER = STM32_SigningTool_CLI
OBJCOPY = arm-none-eabi-objcopy
EL = "$(shell dirname "$(shell which $(FLASHER))")/ExternalLoader/MX66UW1G45G_STM32N6570-DK.stldr"

#######################################
# CFLAGS
#######################################
CPU = -mcpu=cortex-m55 -std=gnu11 -mcmse -mthumb
FPU = -mfpu=fpv5-d16 -mfloat-abi=hard
MCU = $(CPU) $(FPU)

C_DEFS =\
-DSTM32N657xx\
-DARM_MATH_CM55\
-DUSE_FULL_ASSERT\
-DUSE_FULL_LL_DRIVER\
-DVECT_TAB_SRAM\
-DLL_ATON_PLATFORM=LL_ATON_PLAT_STM32N6\
-DLL_ATON_RT_MODE=LL_ATON_RT_ASYNC\
-DLL_ATON_SW_FALLBACK -DLL_ATON_EB_DBG_INFO -DLL_ATON_DUMP_DEBUG_API\
-DLL_ATON_DBG_BUFFER_INFO_EXCLUDED=1\
-DFEAT_FREERTOS\

C_INCLUDES = \
-IInc\
-I../Dpu\
-I../Common\
-I../X-CUBE-AI/atonn\
-I../X-CUBE-AI\
-I../../Drivers/CMSIS/Core/Include \
-I../../Drivers/CMSIS/DSP/Include\
-I../../Drivers/CMSIS/DSP/PrivateInclude \
-I../../Drivers/CMSIS/Device/ST/STM32N6xx/Include\
-I../../Drivers/CMSIS/Device/ST/STM32N6xx/Include/Templates\
-I../../Drivers/STM32N6xx_HAL_Driver/Inc\
-I../../Middlewares/ST/AI/Inc\
-I../../Middlewares/ST/AI/Npu/Devices/STM32N6XX\
-I../../Middlewares/ST/AI/Npu/ll_aton\
-I../../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Inc\
-I../../Drivers/BSP/Components/mx66uw1g45g\
-I../../Middlewares/ST/FreeRTOS/Source/include \
-I../../Middlewares/ST/FreeRTOS/Source/portable/GCC/ARM_CM55_NTZ/non_secure \
-I../../Drivers/BSP/STM32N6570-DK \
-Ifreertos

OPT  += -c -Wall -fstack-usage -fdata-sections -ffunction-sections \
-fcyclomatic-complexity -flax-vector-conversions --specs=nano.specs
OPT  += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"

C_FLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT)

AS_DEFS =\
-DDEBUG\

AS_INCLUDES = \
-IInc\

AS_FLAGS = -mcpu=cortex-m55 -g3 $(AS_DEFS) $(AS_INCLUDES)
AS_FLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"
AS_FLAGS += --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32CubeIDE/STM32N657XX_LRUN.ld

# libraries
LIBS = -lc -lm -lnosys -l:NetworkRuntime1020_CM55_GCC.a
LIBDIR = -L../../Middlewares/ST/AI/Lib/GCC/ARMCortexM55
LD_FLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS)
LD_FLAGS += -Wl,--cref -Wl,--gc-sections
LD_FLAGS += -u _printf_float
LD_FLAGS += -Wl,--print-memory-usage
LD_FLAGS += -Wl,--no-warn-rwx-segments

all: freertos freertos_lp bm bm_lp

# Includes
include bm.mk
include bm_lp.mk
include freertos.mk
include freertos_lp.mk

# Fonction pour supprimer les doublons sans trier
unique = $(if $1,$(firstword $1) $(call unique,$(filter-out $(firstword $1),$1)))

SRC_DIRS = $(call unique , $(PATCH_DIR) $(dir $(C_SOURCES_BM)) $(dir $(C_SOURCES_FREERTOS)))
AS_DIRS  = $(call unique , $(PATCH_DIR) $(dir $(AS_SOURCES_BM)) $(dir $(AS_SOURCES_FREERTOS)))

display_sources:
	echo "C_SOURCES_FREERTOS: $(dir $(C_SOURCES_FREERTOS))"

display_dirs:
	echo "AS_DIRS: $(AS_DIRS)"

vpath %.c $(SRC_DIRS)
vpath %.s $(AS_DIRS)
vpath %.S $(AS_DIRS)

env:
	@echo "Target          : "$(TARGET)
	@echo "Build Directory : "$(BUILD_DIR)
	@echo "Flasher         : "$(FLASHER)
	@echo "External Loader : "$(EL)
	@echo "Signer          : "$(SIGNER)
	@echo $(C_SOURCES_FREERTOS)
	@echo $(C_SOURCES_FREERTOS_LP)

clean_all : clean_bm clean_bm_lp clean_freertos clean_freertos_lp
	@echo "all cleaned"

Binaries:  $(BUILD_DIR_BM)/$(TARGET)_sign.bin \
$(BUILD_DIR_BM_LP)/$(TARGET)_sign.bin\
$(BUILD_DIR_FREERTOS)/$(TARGET)_sign.bin\
$(BUILD_DIR_FREERTOS_LP)/$(TARGET)_sign.bin\
$(BUILD_DIR_FREERTOS)/$(TARGET)_sign.bin

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR_BM)/*.d)
-include $(wildcard $(BUILD_DIR_FREERTOS)/*.d)

.PHONY: clean all env Binaries