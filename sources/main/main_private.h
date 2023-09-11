/*
 * main_private.h
 *
 *  Created on: Sep 4, 2023
 *      Author: Taras Karpliak
 */

#ifndef MAIN_PRIVATE_H
#define MAIN_PRIVATE_H

#include "sources/chip/chip.h"
#include "stdint.h"
#include "sources/led_ctrl/led_ctrl.h"


typedef struct
{
  uint32_t timer_ms;
} MAIN_t;


void MAIN_Init(void);

// ------------------------------------------------------------------
/// @brief
//-------------------------------------------------------------------
void MAIN_DelayMs(uint32_t timeout_ms); ///< [ms] Wait timeout

void MAIN_WaitForUpdate(void);

uint32_t MAIN_GetTimeMs(void);

void LEDCTRL_IncrementUint32(uint32_t* counter_ptr);

#endif /* MAIN_PRIVATE_H */
