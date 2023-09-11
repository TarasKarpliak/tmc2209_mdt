/*
 * state_machine.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Taras Karpliak
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "sources/common_def/common_def.h"


typedef struct
{
  uint8_t state;
  bool_t has_changed;
} SM_t;


void SM_Init(SM_t* this_ptr, uint8_t init_state);

void SM_ChangeTo(SM_t* this_ptr, uint8_t new_state);

bool_t SM_HasChanged(SM_t* this_ptr);

uint8_t SM_GetState(SM_t* this_ptr);

#endif /* STATE_MACHINE_H */
