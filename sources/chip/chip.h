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
void CHIP_SetDirPin(void);


// ------------------------------------------------------------------
void CHIP_ResetDirPin(void);
void CHIP_SetStepPin(void);
// ------------------------------------------------------------------
void CHIP_ResetStepPin(void);
void CHIP_ChannelEnable(void);

void CHIP_ChannelDisable(void);


// ------------------------------------------------------------------
uint32_t CHIP_GetSysCoreClockHz(void);

// ------------------------------------------------------------------
void CHIP_MoveForward(uint16_t distance_0_1_mm, uint16_t speed_mm_per_s);
#endif // CHIP_H
