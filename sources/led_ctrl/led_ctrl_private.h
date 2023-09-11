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

#define LEDCTRL_STATE_INIT_TIMEOUT_MS     ((uint32_t) 1000u)
#define LEDCTRL_BLINKING_TIME_MS          ((uint32_t) 150u)

typedef enum
{
  LEDCTRL_STATE_INIT,
  LEDCTRL_STATE_NORMAL_OPERATION,
  LEDCTRL_STATE_ERROR,
  LEDCTRL_STATE_TOTAL,
} LEDCTRL_state_t;

typedef enum
{
  LEDCTRL_IS_LED1_ENABLED,
  LEDCTRL_IS_LED2_ENABLED,
  LEDCTRL_IS_LED3_ENABLED,
  LEDCTRL_STATUS_BITMAP_TOTAL,
} LEDCTRL_status_bitmap_t;

typedef struct
{
  void (*enable_led_ptr[LEDCTRL_LED_TOTAL])(void);
  void (*disable_led_ptr[LEDCTRL_LED_TOTAL])(void);
  uint32_t timer_ms;
  LEDCTRL_state_t state;
  SM_t sm;
  uint8_t status_bitmap;
} LEDCTRL_t;


void LEDCTRL_Reset(void);


void LEDCTRL_IncrementUint32(uint32_t* counter_ptr);

#endif /* LED_CTRL_PRIVATE_H */
