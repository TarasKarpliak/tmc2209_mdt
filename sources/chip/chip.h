/*
 * chip.h
 *
 *  Created on: Sep 3, 2023
 *      Author: taras.karpliak
 */

#ifndef CHIP_H
#define CHIP_H


#include "sources/common_def/common_def.h"

// ------------------------------------------------------------------
/// @brief
// ------------------------------------------------------------------
void CHIP_Init(void);


// ------------------------------------------------------------------
void CHIP_SetGreenLedPin(void);


// ------------------------------------------------------------------
void CHIP_ResetGreenLedPin(void);


// ------------------------------------------------------------------
uint32_t CHIP_GetSysCoreClockHz(void);

#endif // CHIP_H
