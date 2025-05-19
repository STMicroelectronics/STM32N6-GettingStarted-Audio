BUILD_DIR_TX  = $(BUILD_DIR_BASE)/TX
AUDIO_PATCH_TX = ../../Projects/Common/Patch/stm32n6570_discovery_audio.patch.hsi_600_400.c
C_SOURCES_TX = $(C_SOURCES) $(AUDIO_PATCH_TX)

C_SOURCES_TX += \
../../Middlewares/ST/ThreadX/common/src/tx_block_allocate.c \
../../Middlewares/ST/ThreadX/common/src/tx_block_pool_cleanup.c \
../../Middlewares/ST/ThreadX/common/src/tx_block_pool_create.c \
../../Middlewares/ST/ThreadX/common/src/tx_block_pool_delete.c \
../../Middlewares/ST/ThreadX/common/src/tx_block_pool_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_block_pool_initialize.c \
../../Middlewares/ST/ThreadX/common/src/tx_block_pool_performance_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_block_pool_performance_system_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_block_pool_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/tx_block_release.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_allocate.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_pool_cleanup.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_pool_create.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_pool_delete.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_pool_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_pool_initialize.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_pool_performance_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_pool_performance_system_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_pool_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_pool_search.c \
../../Middlewares/ST/ThreadX/common/src/tx_byte_release.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_cleanup.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_create.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_delete.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_initialize.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_performance_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_performance_system_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_set.c \
../../Middlewares/ST/ThreadX/common/src/tx_event_flags_set_notify.c \
../../Middlewares/ST/ThreadX/common/src/tx_initialize_high_level.c \
../../Middlewares/ST/ThreadX/common/src/tx_initialize_kernel_enter.c \
../../Middlewares/ST/ThreadX/common/src/tx_initialize_kernel_setup.c \
../../Middlewares/ST/ThreadX/common/src/tx_misra.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_cleanup.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_create.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_delete.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_initialize.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_performance_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_performance_system_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_priority_change.c \
../../Middlewares/ST/ThreadX/common/src/tx_mutex_put.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_cleanup.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_create.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_delete.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_flush.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_front_send.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_initialize.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_performance_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_performance_system_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_receive.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_send.c \
../../Middlewares/ST/ThreadX/common/src/tx_queue_send_notify.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_ceiling_put.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_cleanup.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_create.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_delete.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_initialize.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_performance_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_performance_system_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_put.c \
../../Middlewares/ST/ThreadX/common/src/tx_semaphore_put_notify.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_create.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_delete.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_entry_exit_notify.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_identify.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_initialize.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_performance_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_performance_system_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_preemption_change.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_priority_change.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_relinquish.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_reset.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_resume.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_shell_entry.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_sleep.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_stack_analyze.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_stack_error_handler.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_stack_error_notify.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_suspend.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_system_preempt_check.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_system_resume.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_system_suspend.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_terminate.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_time_slice.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_time_slice_change.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_timeout.c \
../../Middlewares/ST/ThreadX/common/src/tx_thread_wait_abort.c \
../../Middlewares/ST/ThreadX/common/src/tx_time_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_time_set.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_activate.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_change.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_create.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_deactivate.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_delete.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_expiration_process.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_initialize.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_performance_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_performance_system_info_get.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_system_activate.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_system_deactivate.c \
../../Middlewares/ST/ThreadX/common/src/tx_timer_thread_entry.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_buffer_full_notify.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_disable.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_enable.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_event_filter.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_event_unfilter.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_initialize.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_interrupt_control.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_isr_enter_insert.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_isr_exit_insert.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_object_register.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_object_unregister.c \
../../Middlewares/ST/ThreadX/common/src/tx_trace_user_event_insert.c \
../../Middlewares/ST/ThreadX/common/src/txe_block_allocate.c \
../../Middlewares/ST/ThreadX/common/src/txe_block_pool_create.c \
../../Middlewares/ST/ThreadX/common/src/txe_block_pool_delete.c \
../../Middlewares/ST/ThreadX/common/src/txe_block_pool_info_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_block_pool_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/txe_block_release.c \
../../Middlewares/ST/ThreadX/common/src/txe_byte_allocate.c \
../../Middlewares/ST/ThreadX/common/src/txe_byte_pool_create.c \
../../Middlewares/ST/ThreadX/common/src/txe_byte_pool_delete.c \
../../Middlewares/ST/ThreadX/common/src/txe_byte_pool_info_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_byte_pool_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/txe_byte_release.c \
../../Middlewares/ST/ThreadX/common/src/txe_event_flags_create.c \
../../Middlewares/ST/ThreadX/common/src/txe_event_flags_delete.c \
../../Middlewares/ST/ThreadX/common/src/txe_event_flags_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_event_flags_info_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_event_flags_set.c \
../../Middlewares/ST/ThreadX/common/src/txe_event_flags_set_notify.c \
../../Middlewares/ST/ThreadX/common/src/txe_mutex_create.c \
../../Middlewares/ST/ThreadX/common/src/txe_mutex_delete.c \
../../Middlewares/ST/ThreadX/common/src/txe_mutex_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_mutex_info_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_mutex_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/txe_mutex_put.c \
../../Middlewares/ST/ThreadX/common/src/txe_queue_create.c \
../../Middlewares/ST/ThreadX/common/src/txe_queue_delete.c \
../../Middlewares/ST/ThreadX/common/src/txe_queue_flush.c \
../../Middlewares/ST/ThreadX/common/src/txe_queue_front_send.c \
../../Middlewares/ST/ThreadX/common/src/txe_queue_info_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_queue_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/txe_queue_receive.c \
../../Middlewares/ST/ThreadX/common/src/txe_queue_send.c \
../../Middlewares/ST/ThreadX/common/src/txe_queue_send_notify.c \
../../Middlewares/ST/ThreadX/common/src/txe_semaphore_ceiling_put.c \
../../Middlewares/ST/ThreadX/common/src/txe_semaphore_create.c \
../../Middlewares/ST/ThreadX/common/src/txe_semaphore_delete.c \
../../Middlewares/ST/ThreadX/common/src/txe_semaphore_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_semaphore_info_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_semaphore_prioritize.c \
../../Middlewares/ST/ThreadX/common/src/txe_semaphore_put.c \
../../Middlewares/ST/ThreadX/common/src/txe_semaphore_put_notify.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_create.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_delete.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_entry_exit_notify.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_info_get.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_preemption_change.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_priority_change.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_relinquish.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_reset.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_resume.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_suspend.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_terminate.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_time_slice_change.c \
../../Middlewares/ST/ThreadX/common/src/txe_thread_wait_abort.c \
../../Middlewares/ST/ThreadX/common/src/txe_timer_activate.c \
../../Middlewares/ST/ThreadX/common/src/txe_timer_change.c \
../../Middlewares/ST/ThreadX/common/src/txe_timer_create.c \
../../Middlewares/ST/ThreadX/common/src/txe_timer_deactivate.c \
../../Middlewares/ST/ThreadX/common/src/txe_timer_delete.c \
../../Middlewares/ST/ThreadX/common/src/txe_timer_info_get.c 

C_SOURCES_TX +=\
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_secure_stack.c \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/txe_thread_secure_stack_allocate.c \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/txe_thread_secure_stack_free.c 

C_SOURCES_TX +=\
../../Projects/GS/ThreadX/audio_tx.c \
../../Projects/GS/ThreadX/audio_acq_task.c \
../../Projects/GS/ThreadX/audio_proc_task.c \
../../Projects/GS/ThreadX/load_gen_task.c \
../../Projects/GS/ThreadX/threadx_hal.c \
../../Projects/GS/ThreadX/threadx_libc.c \

AS_SOURCES_TX = $(AS_SOURCES) \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_context_restore.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_context_save.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_interrupt_control.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_interrupt_disable.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_interrupt_restore.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_schedule.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_secure_stack_allocate.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_secure_stack_free.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_secure_stack_initialize.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_stack_build.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_thread_system_return.S \
../../Middlewares/ST/ThreadX/ports/cortex_m55/gnu/src/tx_timer_interrupt.S 

AS_SOURCES_TX += \
../../Projects/GS/ThreadX/tx_initialize_low_level.S \

OBJECTS_TX  = $(addprefix $(BUILD_DIR_TX)/,$(notdir $(C_SOURCES_TX:.c=.o)))
OBJECTS_TX += $(addprefix $(BUILD_DIR_TX)/,$(notdir $(patsubst %.s,%.o,$(patsubst %.S,%.o,$(AS_SOURCES_TX)))))

C_FLAGS_TX  = $(C_FLAGS) -DLL_ATON_OSAL=LL_ATON_OSAL_THREADX
C_FLAGS_TX  += -DAPP_HAS_PARALLEL_NETWORKS=0 -DTX_INCLUDE_USER_DEFINE_FILE
AS_FLAGS_TX = $(AS_FLAGS)
LD_FLAGS_TX = $(LD_FLAGS) -Wl,-Map=$(BUILD_DIR_TX)/$(TARGET).map

$(BUILD_DIR_TX)/tx_initialize_low_level.o: ../../Projects/GS/ThreadX/tx_initialize_low_level.S | $(BUILD_DIR_TX)
	$(AS) -c "$<" $(AS_FLAGS_TX) -o "$@"  

$(BUILD_DIR_TX)/%.o: %.c | $(BUILD_DIR_TX)
	$(CC) -c "$<" $(C_FLAGS_TX) -o "$@"

$(BUILD_DIR_TX)/%.o: %.s | $(BUILD_DIR_TX)
	$(AS) -c "$<" $(AS_FLAGS_TX) -o "$@"  

$(BUILD_DIR_TX)/%.o: %.S | $(BUILD_DIR_TX)
	$(AS) -c "$<" $(AS_FLAGS_TX) -o "$@"  

$(BUILD_DIR_TX)/$(TARGET).elf: $(OBJECTS_TX) | $(BUILD_DIR_TX)
	$(CC) $(OBJECTS_TX) $(LD_FLAGS_TX) -o "$@"
	$(SZ) $@

$(BUILD_DIR_TX)/%.bin: $(BUILD_DIR_TX)/%.elf
	$(BIN) $< $@

$(BUILD_DIR_TX):
	mkdir -p $@

tx: $(BUILD_DIR_TX)/$(TARGET).bin

flash_tx: $(BUILD_DIR_TX)/$(TARGET)_sign.bin
	$(FLASHER) -c port=SWD mode=HOTPLUG -el $(EL) -hardRst -w $< 0x70100000
	@echo FLASH $<

$(BUILD_DIR_TX)/$(TARGET)_sign.bin: $(BUILD_DIR_TX)/$(TARGET).bin
	$(SIGNER) -s -bin $< -nk -t ssbl -hv 2.3 -o $(BUILD_DIR_TX)/$(TARGET)_sign.bin

clean_tx:
	@echo "clean tx"
	@rm -fR $(BUILD_DIR_TX)

.PHONY: tx clean_tx flash_tx

