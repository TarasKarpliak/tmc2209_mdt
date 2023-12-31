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
#include "sources/usart/usart.h"
#include "tmc2209/tmc2209.h"

typedef struct
{
  uint32_t timer_ms;
} MAIN_t;


//-------------------------------------------------------------------
static void MAIN_Init(void);


//-------------------------------------------------------------------
static void MAIN_WaitForUpdate(void);

#endif /* MAIN_PRIVATE_H */
