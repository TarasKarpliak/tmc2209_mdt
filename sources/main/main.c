/*
 * main.c
 *
 *  Created on: Sep 4, 2023
 *      Author: Taras Karpliak
 */

#include "main_private.h"


MAIN_t MAIN = {0};


//-------------------------------------------------------------------
int main(void)
{
  MAIN_Init();

	while(TRUE)
	{
	  MAIN_WaitForUpdate();

	  LEDCTRL_Update();
	  TMC2209_Update();
	}

	return 0u;
}


//-------------------------------------------------------------------
static void MAIN_Init(void)
{
  MAIN.timer_ms = 0u;

  CHIP_Init();
  USART_Init();

  LEDCTRL_init_t ledctrl_init =
  {
    .enable_led_ptr[LEDCTRL_LED_1]  = CHIP_SetGreenLedPin,
    .disable_led_ptr[LEDCTRL_LED_1] = CHIP_ResetGreenLedPin,
  };
  LEDCTRL_Init(&ledctrl_init);

  TMC2209_init_t tmc2209_init =
  {
    .node_addr = 0x00,
  };
  TMC2209_Init(&tmc2209_init);
}


//-------------------------------------------------------------------
void MAIN_WaitForUpdate(void)
{
  static uint32_t timestamp_begin = 0u;
  while (MAIN.timer_ms <= timestamp_begin);
  timestamp_begin = MAIN.timer_ms;
}


//-------------------------------------------------------------------
void SysTick_Handler(void)
{
  MAIN.timer_ms++;
}
