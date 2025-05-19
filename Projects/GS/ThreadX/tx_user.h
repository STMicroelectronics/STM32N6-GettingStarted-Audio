 /**
 ******************************************************************************
 * @file    tx_user.h
 * @author  GPM Application Team
 * @version V2.0.0
 * @date    02-May-2025*
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef TX_USER
#define TX_USER

#define TX_MAX_PRIORITIES 64
#define TX_SINGLE_MODE_SECURE

#define TX_TIMER_TICKS_PER_SECOND (1000UL)

#ifndef SYSTEM_CLOCK
#define SYSTEM_CLOCK 600000000
#endif

#ifndef TICK_FREQ
#define TICK_FREQ TX_TIMER_TICKS_PER_SECOND
#endif

#ifdef APP_LP
/* #define TX_ENABLE_WFI */
#define TX_LOW_POWER
 /* Define the TX_LOW_POWER_USER_ENTER macro. */
#define TX_LOW_POWER_USER_ENTER low_power_enter
#endif

#endif
