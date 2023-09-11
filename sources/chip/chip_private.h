/*
 * chip_private.h
 *
 *  Created on: Sep 3, 2023
 *      Author: taras.karpliak
 */

#ifndef CHIP_PRIVATE_H
#define CHIP_PRIVATE_H

#include "sources/chip/chip.h"

#include "system_stm32f7xx.h"

#include "string.h"


// ------------------------------------------------------------------
/// @brief
// ------------------------------------------------------------------
static void CHIP_ClockInit(void);


// ------------------------------------------------------------------
/// @brief
// ------------------------------------------------------------------
static void CHIP_GpioInit(void);



#endif // CHIP_PRIVATE_H
