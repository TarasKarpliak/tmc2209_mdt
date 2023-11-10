/*
 * state_machine.c
 *
 *  Created on: Sep 10, 2023
 *      Author: Taras Karpliak
 */

#include "state_machine.h"


//-------------------------------------------------------------------
void SM_Init(SM_t* this_ptr, uint8_t init_state)
{
  ASSERT(this_ptr != NULL);

  this_ptr->state = init_state;
  this_ptr->has_changed = TRUE;
}


//-------------------------------------------------------------------
void SM_ChangeTo(SM_t* this_ptr, uint8_t new_state)
{
  ASSERT(this_ptr != NULL);

  this_ptr->state = new_state;
}


//-------------------------------------------------------------------
bool_t SM_HasChanged(SM_t* this_ptr)
{
  ASSERT(this_ptr != NULL);

  bool_t result = FALSE;

  if (this_ptr->has_changed == TRUE)
  {
    this_ptr->has_changed = FALSE;
    result = TRUE;
  }

  return result;
}


//-------------------------------------------------------------------
uint8_t SM_GetState(SM_t* this_ptr)
{
  ASSERT(this_ptr != NULL);

  return this_ptr->state;
}
