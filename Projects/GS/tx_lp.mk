BUILD_DIR_TX_LP  = $(BUILD_DIR_BASE)/TX_LP

AUDIO_PATCH_TX_LP = ../../Projects/Common/Patch/stm32n6570_discovery_audio.patch.msi_4_4.c
C_SOURCES_TX_LP = $(filter-out $(AUDIO_PATCH_TX),$(C_SOURCES_TX))
C_SOURCES_TX_LP += $(AUDIO_PATCH_TX_LP)

AS_SOURCES_TX_LP = $(AS_SOURCES_TX)

OBJECTS_TX_LP  = $(addprefix $(BUILD_DIR_TX_LP)/,$(notdir $(C_SOURCES_TX_LP:.c=.o)))
OBJECTS_TX_LP += $(addprefix $(BUILD_DIR_TX_LP)/,$(notdir $(patsubst %.s,%.o,$(patsubst %.S,%.o,$(AS_SOURCES_TX_LP)))))

C_FLAGS_TX_LP  = $(C_FLAGS) -DLL_ATON_OSAL=LL_ATON_OSAL_THREADX
C_FLAGS_TX_LP  += -DAPP_HAS_PARALLEL_NETWORKS=0 -DTX_INCLUDE_USER_DEFINE_FILE
AS_FLAGS_TX_LP = $(AS_FLAGS)
LD_FLAGS_TX_LP = $(LD_FLAGS) -Wl,-Map=$(BUILD_DIR_TX_LP)/$(TARGET).map

$(BUILD_DIR_TX_LP)/tx_initialize_low_level.o: ../../Projects/GS/ThreadX/tx_initialize_low_level.S | $(BUILD_DIR_TX_LP)
	$(AS) -c "$<" $(AS_FLAGS_TX_LP) -o "$@"  

$(BUILD_DIR_TX_LP)/%.o: %.c | $(BUILD_DIR_TX_LP)
	$(CC) -c "$<" $(C_FLAGS_TX_LP) -o "$@"

$(BUILD_DIR_TX_LP)/%.o: %.s | $(BUILD_DIR_TX_LP)
	$(AS) -c "$<" $(AS_FLAGS_TX_LP) -o "$@"  

$(BUILD_DIR_TX_LP)/%.o: %.S | $(BUILD_DIR_TX_LP)
	$(AS) -c "$<" $(AS_FLAGS_TX_LP) -o "$@"  

$(BUILD_DIR_TX_LP)/$(TARGET).elf: $(OBJECTS_TX_LP) | $(BUILD_DIR_TX_LP)
	$(CC) $(OBJECTS_TX_LP) $(LD_FLAGS_TX_LP) -o "$@"
	$(SZ) $@

$(BUILD_DIR_TX_LP)/%.bin: $(BUILD_DIR_TX_LP)/%.elf
	$(BIN) $< $@

$(BUILD_DIR_TX_LP):
	mkdir -p $@

tx_lp: $(BUILD_DIR_TX_LP)/$(TARGET).bin

flash_tx_lp: $(BUILD_DIR_TX_LP)/$(TARGET)_sign.bin
	$(FLASHER) -c port=SWD mode=HOTPLUG -el $(EL) -hardRst -w $< 0x70100000
	@echo FLASH $<

$(BUILD_DIR_TX_LP)/$(TARGET)_sign.bin: $(BUILD_DIR_TX_LP)/$(TARGET).bin
	$(SIGNER) -s -bin $< -nk -t ssbl -hv 2.3 -o $(BUILD_DIR_TX_LP)/$(TARGET)_sign.bin

clean_tx_lp:
	@echo "clean tx lp"
	@rm -fR $(BUILD_DIR_TX_LP)

.PHONY: tx_lp flash_tx_lp clean_tx_lp 

