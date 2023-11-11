/*
 * tmc2209.h
 *
 *  Created on: Nov 10, 2023
 *      Author: taras.karpliak
 */

#ifndef TMC2209_H
#define TMC2209_H

#include "sources/common_def/common_def.h"


typedef struct
{
  uint8_t node_addr;
} TMC2209_init_t;


typedef enum
{
  TMC2209_STATE_CONFIG,
  TMC2209_STATE_CHECK_CONFIG,
  TMC2209_STATE_IDLE,
  TMC2209_STATE_ERROR,
  TMC2209_STATE_TOTAL,
} TMC2209_state_t;


typedef enum
{
  TMC2209_ERROR_NONE,
  TMC2209_ERROR_TOTAL,
} TMC2209_error_t;


//-------------------------------------------------------------------
void TMC2209_Init(TMC2209_init_t* init_ptr);


//-------------------------------------------------------------------
void TMC2209_Reset(void);


//-------------------------------------------------------------------
void TMC2209_Update(void);


//-------------------------------------------------------------------
TMC2209_state_t TMC2209_GetState(void);


//-------------------------------------------------------------------
TMC2209_error_t TMC2209_GetError(void);

#endif /* TMC2209_H */
