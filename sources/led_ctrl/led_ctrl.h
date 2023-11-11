/*
 * led_ctrl.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Taras Karpliak
 */

#ifndef LED_CTRL_H
#define LED_CTRL_H

#include "sources/common_def/common_def.h"

typedef enum
{
  LEDCTRL_LED_1,
  LEDCTRL_LED_TOTAL,
} LEDCTRL_led_name_t;


typedef struct
{
  void (*enable_led_ptr[LEDCTRL_LED_TOTAL])(void);
  void (*disable_led_ptr[LEDCTRL_LED_TOTAL])(void);
} LEDCTRL_init_t;


void LEDCTRL_Init(LEDCTRL_init_t* init_ptr);


void LEDCTRL_Update(void);
//-------------------------------------------------------------------
void LEDCTRL_ToggleLed1(void);
#endif /* LED_CTRL_H */
