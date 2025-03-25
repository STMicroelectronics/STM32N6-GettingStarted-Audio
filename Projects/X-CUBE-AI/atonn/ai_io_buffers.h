/**
 ******************************************************************************
 * @file    ai_io_buffers.h
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

#ifndef AI_IO_BUFFERS_H
#define AI_IO_BUFFERS_H

#include <stdint.h>

/* ------ Allocated inputs ------ */

/* ------ Allocated outputs ------ */

/* ------ Utilities prototypes ------ */
void connect_input_buffers(void);
void connect_output_buffers(void);

#endif  /* AI_IO_BUFFERS_H */