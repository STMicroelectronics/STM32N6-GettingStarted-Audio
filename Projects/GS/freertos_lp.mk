BUILD_DIR_FREERTOS_LP  = $(BUILD_DIR_BASE)/FREERTOS_LP

AUDIO_PATCH_FREERTOS_LP = ../../Projects/Common/Patch/stm32n6570_discovery_audio.patch.msi_4_4.c
C_SOURCES_FREERTOS_LP = $(filter-out $(AUDIO_PATCH_FREERTOS),$(C_SOURCES_FREERTOS))
C_SOURCES_FREERTOS_LP += $(AUDIO_PATCH_FREERTOS_LP)

AS_SOURCES_FREERTOS_LP = $(AS_SOURCES_FREERTOS)

OBJECTS_FREERTOS_LP  = $(addprefix $(BUILD_DIR_FREERTOS_LP)/,$(notdir $(C_SOURCES_FREERTOS_LP:.c=.o)))
OBJECTS_FREERTOS_LP += $(addprefix $(BUILD_DIR_FREERTOS_LP)/,$(notdir $(patsubst %.s,%.o,$(patsubst %.S,%.o,$(AS_SOURCES_FREERTOS_LP)))))

C_FLAGS_FREERTOS_LP  = $(C_FLAGS) -DLL_ATON_OSAL=LL_ATON_OSAL_FREERTOS
C_FLAGS_FREERTOS_LP  += -DAPP_HAS_PARALLEL_NETWORKS=0 -DUSE_FREERTOS -DAPP_FREERTOS -DAPP_LP -DAPP_DVFS
AS_FLAGS_FREERTOS_LP = $(AS_FLAGS)
LD_FLAGS_FREERTOS_LP = $(LD_FLAGS) -Wl,-Map=$(BUILD_DIR_FREERTOS_LP)/$(TARGET).map

$(BUILD_DIR_FREERTOS_LP)/%.o: %.c | $(BUILD_DIR_FREERTOS_LP)
	$(CC) -c "$<" $(C_FLAGS_FREERTOS_LP) -o "$@"

$(BUILD_DIR_FREERTOS_LP)/%.o: %.s | $(BUILD_DIR_FREERTOS_LP)
	$(AS) -c "$<" $(AS_FLAGS_FREERTOS_LP) -o "$@"

$(BUILD_DIR_FREERTOS_LP)/%.o: %.S | $(BUILD_DIR_FREERTOS_LP)
	$(AS) -c "$<" $(AS_FLAGS_FREERTOS_LP) -o "$@"

$(BUILD_DIR_FREERTOS_LP)/freertos_lp.list: $(OBJECTS_FREERTOS_LP)
	$(file > $@, $(OBJECTS_FREERTOS_LP))

$(BUILD_DIR_FREERTOS_LP)/$(TARGET).elf: $(BUILD_DIR_FREERTOS_LP)/freertos_lp.list | $(BUILD_DIR_FREERTOS_LP)
	$(CC) @$(BUILD_DIR_FREERTOS_LP)/freertos_lp.list $(LD_FLAGS_FREERTOS_LP) -o "$@"
	$(SZ) $@

$(BUILD_DIR_FREERTOS_LP)/%.bin: $(BUILD_DIR_FREERTOS_LP)/%.elf
	$(BIN) $< $@

$(BUILD_DIR_FREERTOS_LP):
	mkdir -p $@

freertos_lp: $(BUILD_DIR_FREERTOS_LP)/$(TARGET).bin

flash_freertos_lp: $(BUILD_DIR_FREERTOS_LP)/$(TARGET)_sign.bin
	$(FLASHER) -c port=SWD mode=HOTPLUG -el $(EL) -hardRst -w $< 0x70100000
	@echo FLASH $<

$(BUILD_DIR_FREERTOS_LP)/$(TARGET)_sign.bin: $(BUILD_DIR_FREERTOS_LP)/$(TARGET).bin
	$(SIGNER) -s -bin $< -nk -t ssbl -hv 2.3 -o $(BUILD_DIR_FREERTOS_LP)/$(TARGET)_sign.bin

clean_freertos_lp:
	@echo "clean freertos lp"
	@rm -fR $(BUILD_DIR_FREERTOS_LP)

.PHONY: freertos_lp flash_freertos_lp clean_freertos_lp

