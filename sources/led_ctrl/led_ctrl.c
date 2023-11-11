/*
 * led_ctrl.c
 *
 *  Created on: Sep 10, 2023
 *      Author: Taras Karpliak
 */

#include "led_ctrl_private.h"


LEDCTRL_t LEDCTRL = {0};


//-------------------------------------------------------------------
void LEDCTRL_Init(LEDCTRL_init_t* init_ptr)
{
  ASSERT(init_ptr != NULL);

  for (uint8_t led_num = 0u; led_num < LEDCTRL_LED_TOTAL; led_num++)
  {
    ASSERT(init_ptr->enable_led_ptr[led_num] != NULL);
    ASSERT(init_ptr->disable_led_ptr[led_num] != NULL);
  }

  for (uint8_t led_num = 0u; led_num < LEDCTRL_LED_TOTAL; led_num++)
  {
    LEDCTRL.enable_led_ptr[led_num] = init_ptr->enable_led_ptr[led_num];
    LEDCTRL.disable_led_ptr[led_num] = init_ptr->disable_led_ptr[led_num];
  }

  LEDCTRL_Reset();
}


//-------------------------------------------------------------------
void LEDCTRL_Reset(void)
{
  SM_Init(&LEDCTRL.sm, (uint8_t)LEDCTRL_STATE_RUNNING);

  LEDCTRL.timeout_ms = 0u;

  RESETBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED1_ENABLED);
}


//-------------------------------------------------------------------
void LEDCTRL_Update(void)
{
  COMMON_IncrementUint32(&LEDCTRL.timeout_ms);

  switch (SM_GetState(&LEDCTRL.sm))
  {
    case LEDCTRL_STATE_RUNNING:
      if (SM_HasChanged(&LEDCTRL.sm) == TRUE)
      {
        LEDCTRL.timeout_ms = 0u;
      }

      if (LEDCTRL.timeout_ms >= (LEDCTRL_BLINKING_PERIOD_MS / 2u))
      {
        LEDCTRL.timeout_ms = 0u;
        LEDCTRL_ToggleLed1();
      }
      break;

    case LEDCTRL_STATE_TOTAL:
    default:
      break;
  }
}


//-------------------------------------------------------------------
 void LEDCTRL_ToggleLed1(void)
{
  if (CHECKBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED1_ENABLED) == FALSE)
  {
    LEDCTRL.enable_led_ptr[LEDCTRL_LED_1]();
    SETBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED1_ENABLED);
  }
  else
  {
    LEDCTRL.disable_led_ptr[LEDCTRL_LED_1]();
    RESETBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED1_ENABLED);
  }
}
