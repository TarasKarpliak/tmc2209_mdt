/*
 * chip_private.h
 *
 *  Created on: Sep 3, 2023
 *      Author: taras.karpliak
 */

#ifndef CHIP_PRIVATE_H
#define CHIP_PRIVATE_H

#include "sources/chip/chip.h"
#include "stm32f7xx.h"


// ------------------------------------------------------------------
/// @brief
// ------------------------------------------------------------------
static void CHIP_ClockInit(void);


// ------------------------------------------------------------------
/// @brief
// ------------------------------------------------------------------
static void CHIP_GpioInit(void);

#endif // CHIP_PRIVATE_H
