BUILD_DIR_BM  = $(BUILD_DIR_BASE)/BM
AUDIO_PATCH_BM = ../../Projects/Common/Patch/stm32n6570_discovery_audio.patch.hsi_600_400.c
C_SOURCES_BM  = $(C_SOURCES) $(AUDIO_PATCH_BM)
AS_SOURCES_BM = $(AS_SOURCES)

OBJECTS_BM  = $(addprefix $(BUILD_DIR_BM)/,$(notdir $(C_SOURCES_BM:.c=.o)))
OBJECTS_BM += $(addprefix $(BUILD_DIR_BM)/,$(notdir $(AS_SOURCES_BM:.s=.o)))

C_FLAGS_BM  = $(C_FLAGS) -DLL_ATON_OSAL=LL_ATON_OSAL_BARE_METAL -DAPP_BARE_METAL
AS_FLAGS_BM = $(AS_FLAGS)
LD_FLAGS_BM = $(LD_FLAGS) -Wl,-Map=$(BUILD_DIR_BM)/$(TARGET).map

$(BUILD_DIR_BM)/%.o: %.c | $(BUILD_DIR_BM)
	$(CC) -c "$<" $(C_FLAGS_BM) -o "$@"

$(BUILD_DIR_BM)/%.o: %.s | $(BUILD_DIR_BM)
	$(AS) -c "$<" $(AS_FLAGS_BM) -o "$@"  

$(BUILD_DIR_BM)/bm.list: $(OBJECTS_BM)
	$(file > $@, $(OBJECTS_BM))

$(BUILD_DIR_BM)/$(TARGET).elf: $(BUILD_DIR_BM)/bm.list | $(BUILD_DIR_BM)
	$(CC) @$(BUILD_DIR_BM)/bm.list $(LD_FLAGS_BM) -o "$@"
	$(SZ) $@

$(BUILD_DIR_BM)/%.bin: $(BUILD_DIR_BM)/%.elf
	$(BIN) $< $@

$(BUILD_DIR_BM):
	mkdir -p $@

bm: $(BUILD_DIR_BM)/$(TARGET).bin

flash_bm: $(BUILD_DIR_BM)/$(TARGET)_sign.bin
	$(FLASHER) -c port=SWD mode=HOTPLUG -el $(EL) -hardRst -w $< 0x70100000
	@echo FLASH $<

$(BUILD_DIR_BM)/$(TARGET)_sign.bin: $(BUILD_DIR_BM)/$(TARGET).bin
	$(SIGNER) -s -bin $< -nk -t ssbl -hv 2.3 -o $(BUILD_DIR_BM)/$(TARGET)_sign.bin

clean_bm:
	@echo "clean bm"
	@rm -fR $(BUILD_DIR_BM)

.PHONY: bm flash_bm clean_bm

