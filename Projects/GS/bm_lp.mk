BUILD_DIR_BM_LP  = $(BUILD_DIR_BASE)/BM_LP
C_SOURCES_BM_LP  = $(C_SOURCES)
C_SOURCES_BM_LP += \
../../Projects/Common/Patch/stm32n6570_discovery_audio.patch.msi_4_4.c

AS_SOURCES_BM_LP = $(AS_SOURCES)

OBJECTS_BM_LP  = $(addprefix $(BUILD_DIR_BM_LP)/,$(notdir $(C_SOURCES_BM_LP:.c=.o)))
OBJECTS_BM_LP += $(addprefix $(BUILD_DIR_BM_LP)/,$(notdir $(AS_SOURCES_BM_LP:.s=.o)))

C_FLAGS_BM_LP  = $(C_FLAGS_BM) -DAPP_LP -DAPP_DVFS
AS_FLAGS_BM_LP = $(AS_FLAGS)
LD_FLAGS_BM_LP = $(LD_FLAGS) -Wl,-Map=$(BUILD_DIR_BM_LP)/$(TARGET).map

$(BUILD_DIR_BM_LP)/%.o: %.c | $(BUILD_DIR_BM_LP)
	$(CC) -c "$<" $(C_FLAGS_BM_LP) -o "$@"

$(BUILD_DIR_BM_LP)/%.o: %.s | $(BUILD_DIR_BM_LP)
	$(AS) -c "$<" $(AS_FLAGS_BM_LP) -o "$@"  

$(BUILD_DIR_BM_LP)/bm_lp.list: $(OBJECTS_BM_LP)
	$(file > $@, $(OBJECTS_BM_LP))

$(BUILD_DIR_BM_LP)/$(TARGET).elf: $(BUILD_DIR_BM_LP)/bm_lp.list | $(BUILD_DIR_BM_LP)
	$(CC) @$(BUILD_DIR_BM_LP)/bm_lp.list $(LD_FLAGS_BM_LP) -o "$@"
	$(SZ) $@

$(BUILD_DIR_BM_LP)/%.bin: $(BUILD_DIR_BM_LP)/%.elf
	$(BIN) $< $@

$(BUILD_DIR_BM_LP):
	mkdir -p $@

bm_lp: $(BUILD_DIR_BM_LP)/$(TARGET).bin

flash_bm_lp: $(BUILD_DIR_BM_LP)/$(TARGET)_sign.bin
	$(FLASHER) -c port=SWD mode=HOTPLUG -el $(EL) -hardRst -w $< 0x70100000
	@echo FLASH $<

$(BUILD_DIR_BM_LP)/$(TARGET)_sign.bin: $(BUILD_DIR_BM_LP)/$(TARGET).bin
	$(SIGNER) -s -bin $< -nk -t ssbl -hv 2.3 -o $(BUILD_DIR_BM_LP)/$(TARGET)_sign.bin

clean_bm_lp:
	@echo "clean bm lp"
	@rm -fR $(BUILD_DIR_BM_LP)

.PHONY: bm_lp flash_bm_lp clean_bm_lp