/*
 * tmc2209_private.h
 *
 *  Created on: Nov 10, 2023
 *      Author: taras.karpliak
 */

#ifndef TMC2209_PRIVATE_H
#define TMC2209_PRIVATE_H

#include "tmc2209.h"
#include "sources/state_machine/state_machine.h"


typedef struct
{
  TMC2209_error_t error;
  SM_t sm;
} TMC2209_t;


//-------------------------------------------------------------------
void TMC2209_SetState(TMC2209_state_t new_state);


//-------------------------------------------------------------------
void TMC2209_SetError(TMC2209_error_t new_error);

#endif /* TMC2209_PRIVATE_H */
