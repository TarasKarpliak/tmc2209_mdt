/*
 * led_ctrl.c
 *
 *  Created on: Sep 10, 2023
 *      Author: Taras Karpliak
 */

#include "led_ctrl_private.h"

LEDCTRL_t LEDCTRL = {0};

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


void LEDCTRL_Reset(void)
{
  SM_Init(&LEDCTRL.sm, (uint8_t)LEDCTRL_STATE_INIT);

  RESETBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED1_ENABLED);
  RESETBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED2_ENABLED);
  RESETBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED3_ENABLED);
}


void LEDCTRL_Update(void)
{
  LEDCTRL_IncrementUint32(&LEDCTRL.timer_ms);

  switch (SM_GetState(&LEDCTRL.sm))
  {
    case LEDCTRL_STATE_INIT:
      if (SM_HasChanged(&LEDCTRL.sm) == TRUE)
      {
        LEDCTRL.timer_ms = 0u;

        for (uint8_t led_num = 0u; led_num < LEDCTRL_LED_TOTAL; led_num++)
        {
          LEDCTRL.enable_led_ptr[led_num]();
        }
      }

      if (LEDCTRL.timer_ms >= LEDCTRL_STATE_INIT_TIMEOUT_MS)
      {
        for (uint8_t led_num = 0u; led_num < LEDCTRL_LED_TOTAL; led_num++)
        {
          LEDCTRL.disable_led_ptr[led_num]();
        }

        SM_ChangeTo(&LEDCTRL.sm, (uint8_t)LEDCTRL_STATE_NORMAL_OPERATION);
      }
      break;

    case LEDCTRL_STATE_NORMAL_OPERATION:
      if (SM_HasChanged(&LEDCTRL.sm) == TRUE)
      {
        LEDCTRL.timer_ms = 0u;
      }

      if (LEDCTRL.timer_ms >= LEDCTRL_BLINKING_TIME_MS)
      {
        LEDCTRL.timer_ms = 0u;

        if (CHECKBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED1_ENABLED) == FALSE)
        {
          LEDCTRL.enable_led_ptr[LEDCTRL_IS_LED1_ENABLED]();
          SETBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED1_ENABLED);
        }
        else
        {
          LEDCTRL.disable_led_ptr[LEDCTRL_IS_LED1_ENABLED]();
          RESETBIT(LEDCTRL.status_bitmap, LEDCTRL_IS_LED1_ENABLED);
        }
      }
      break;

    case LEDCTRL_STATE_ERROR:
     break;

    default:
      break;
  }
}


void LEDCTRL_IncrementUint32(uint32_t* counter_ptr)
{
  if (*counter_ptr < UINT32_MAX)
  {
    (*counter_ptr)++;
  }
}
