/*
 * tmc2209.c
 *
 *  Created on: Nov 10, 2023
 *      Author: taras.karpliak
 */

#include "tmc2209_private.h"


TMC2209_t TMC2209 = {0};


//-------------------------------------------------------------------
void TMC2209_Init(TMC2209_init_t* init_ptr)
{
  ASSERT(init_ptr != NULL);

  TMC2209_SetError(TMC2209_ERROR_NONE);
  SM_Init(&TMC2209.sm, TMC2209_STATE_CHECK_ID);
}


//-------------------------------------------------------------------
void TMC2209_Update(void)
{
  switch (TMC2209_GetState())
  {
    case TMC2209_STATE_CHECK_ID:
      break;

    case TMC2209_STATE_CONFIG:
      break;

    case TMC2209_STATE_CHECK_CONFIG:
      break;

    case TMC2209_STATE_IDLE:
      break;

    case TMC2209_STATE_TOTAL:
    default:
      break;
  }
}


//-------------------------------------------------------------------
void TMC2209_SetState(TMC2209_state_t new_state)
{
  SM_ChangeTo(&TMC2209.sm, new_state);
}


//-------------------------------------------------------------------
void TMC2209_SetError(TMC2209_error_t new_error)
{
  TMC2209.error = new_error;
}


//-------------------------------------------------------------------
TMC2209_state_t TMC2209_GetState(void)
{
  return (TMC2209_state_t)SM_GetState(&TMC2209.sm);
}


//-------------------------------------------------------------------
TMC2209_error_t TMC2209_GetError(void)
{
  return TMC2209.error;
}
