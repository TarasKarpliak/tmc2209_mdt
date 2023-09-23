/*
 * common_def.c
 *
 *  Created on: Sep 23, 2023
 *      Author: Taras Karpliak
 */

#include "common_def.h"

//-------------------------------------------------------------------
void COMMON_IncrementUint32(uint32_t* counter_ptr)
{
  if (*counter_ptr < UINT32_MAX)
  {
    (*counter_ptr)++;
  }
}
