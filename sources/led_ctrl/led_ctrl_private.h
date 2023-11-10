/*
 * led_ctrl_private.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Taras Karpliak
 */

#ifndef LED_CTRL_PRIVATE_H
#define LED_CTRL_PRIVATE_H

#include "led_ctrl.h"

#include "sources/state_machine/state_machine.h"

#define LEDCTRL_BLINKING_PERIOD_MS     ((uint32_t) 1000u)

typedef enum
{
  LEDCTRL_STATE_RUNNING,
  LEDCTRL_STATE_TOTAL,
} LEDCTRL_state_t;

typedef enum
{
  LEDCTRL_IS_LED1_ENABLED,
} LEDCTRL_status_bitmap_t;

typedef struct
{
  void (*enable_led_ptr[LEDCTRL_LED_TOTAL])(void);
  void (*disable_led_ptr[LEDCTRL_LED_TOTAL])(void);
  uint32_t timeout_ms;
  uint8_t status_bitmap;
  SM_t sm;
} LEDCTRL_t;


//-------------------------------------------------------------------
void LEDCTRL_Reset(void);


//-------------------------------------------------------------------
void LEDCTRL_IncrementUint32(uint32_t* counter_ptr);


//-------------------------------------------------------------------
static void LEDCTRL_ToggleLed1(void);

#endif /* LED_CTRL_PRIVATE_H */
