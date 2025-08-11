# freertos.mk

BUILD_DIR_FREERTOS = $(BUILD_DIR_BASE)/FREERTOS
AUDIO_PATCH_FREERTOS = ../../Projects/Common/Patch/stm32n6570_discovery_audio.patch.hsi_600_400.c
C_SOURCES_FREERTOS = $(C_SOURCES) $(AUDIO_PATCH_FREERTOS)

# FreeRTOS source files
C_SOURCES_FREERTOS += \
../../Middlewares/ST/FreeRTOS/Source/croutine.c \
../../Middlewares/ST/FreeRTOS/Source/event_groups.c \
../../Middlewares/ST/FreeRTOS/Source/list.c \
../../Middlewares/ST/FreeRTOS/Source/queue.c \
../../Middlewares/ST/FreeRTOS/Source/stream_buffer.c \
../../Middlewares/ST/FreeRTOS/Source/tasks.c \
../../Middlewares/ST/FreeRTOS/Source/timers.c \
../../Middlewares/ST/FreeRTOS/Source/portable/GCC/ARM_CM55_NTZ/non_secure/port.c \
freertos/audio_freertos.c \
freertos/audio_acq_task.c \
freertos/audio_proc_task.c \
freertos/freertos_bsp.c \
freertos/load_gen_task.c \
Src/freertos_libc.c

# Assembly sources for FreeRTOS
AS_SOURCES_FREERTOS = $(AS_SOURCES) \
../../Middlewares/FreeRTOS/Source/portable/IAR/ARM_CM55_NTZ/non_secure/portasm.s

# Object files
OBJECTS_FREERTOS  = $(addprefix $(BUILD_DIR_FREERTOS)/,$(notdir $(C_SOURCES_FREERTOS:.c=.o)))
OBJECTS_FREERTOS += $(addprefix $(BUILD_DIR_FREERTOS)/,$(notdir $(patsubst %.s,%.o,$(patsubst %.S,%.o,$(AS_SOURCES_FREERTOS)))))

# Compiler flags for FreeRTOS
C_FLAGS_FREERTOS  = $(C_FLAGS) -DLL_ATON_OSAL=LL_ATON_OSAL_FREERTOS
C_FLAGS_FREERTOS  += -DAPP_HAS_PARALLEL_NETWORKS=0 -DUSE_FREERTOS -DAPP_FREERTOS
AS_FLAGS_FREERTOS = $(AS_FLAGS)
LD_FLAGS_FREERTOS = $(LD_FLAGS) -Wl,-Map=$(BUILD_DIR_FREERTOS)/$(TARGET).map

# Build rules
$(BUILD_DIR_FREERTOS)/%.o: %.c | $(BUILD_DIR_FREERTOS)
	$(CC) -c "$<" $(C_FLAGS_FREERTOS) -o "$@"

$(BUILD_DIR_FREERTOS)/%.o: %.s | $(BUILD_DIR_FREERTOS)
	$(AS) -c "$<" $(AS_FLAGS_FREERTOS) -o "$@"

$(BUILD_DIR_FREERTOS)/%.o: %.S | $(BUILD_DIR_FREERTOS)
	$(AS) -c "$<" $(AS_FLAGS_FREERTOS) -o "$@"

$(BUILD_DIR_FREERTOS)/freertos.list: $(OBJECTS_FREERTOS)
	$(file > $@, $(OBJECTS_FREERTOS))

$(BUILD_DIR_FREERTOS)/$(TARGET).elf: $(BUILD_DIR_FREERTOS)/freertos.list | $(BUILD_DIR_FREERTOS)
	$(CC) @$(BUILD_DIR_FREERTOS)/freertos.list $(LD_FLAGS_FREERTOS) -o "$@"
	$(SZ) $@

$(BUILD_DIR_FREERTOS)/%.bin: $(BUILD_DIR_FREERTOS)/%.elf
	$(BIN) $< $@

$(BUILD_DIR_FREERTOS):
	mkdir -p $@

plot_ld_freertos:
	echo "$(AS_SOURCES_FREERTOS)"

freertos: $(BUILD_DIR_FREERTOS)/$(TARGET).bin

flash_freertos: $(BUILD_DIR_FREERTOS)/$(TARGET)_sign.bin
	$(FLASHER) -c port=SWD mode=HOTPLUG -el $(EL) -hardRst -w $< 0x70100000
	@echo FLASH $<

$(BUILD_DIR_FREERTOS)/$(TARGET)_sign.bin: $(BUILD_DIR_FREERTOS)/$(TARGET).bin
	$(SIGNER) -s -bin $< -nk -t ssbl -hv 2.3 -o $(BUILD_DIR_FREERTOS)/$(TARGET)_sign.bin

clean_freertos:
	@echo "clean freertos"
	@rm -fR $(BUILD_DIR_FREERTOS)

.PHONY: freertos clean_freertos flash_freertos