/**
 ******************************************************************************
 * @file    ai_io_buffers.c
 * @author  MCD/AIS Team
 * @brief   AI Validation application (entry points) - IO buffers allocation
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "ai_io_buffers.h"
#include "ll_aton_NN_interface.h"

/* ------ Allocated inputs ------ */

/* ------ Allocated outputs ------ */

/* ------ External definitions from the .c file ------ */
extern LL_ATON_User_IO_Result_t LL_ATON_Set_User_Input_Buffer_Default(uint32_t num, void* buffer, uint32_t size);
extern LL_ATON_User_IO_Result_t LL_ATON_Set_User_Output_Buffer_Default(uint32_t num, void* buffer, uint32_t size);

/* ------ Utilities ------ */
void connect_input_buffers(void)
{

}

void connect_output_buffers(void)
{
  
}