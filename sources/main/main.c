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

  uint8_t data[15] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x13, 0x46, 0x79, 0x0A, 0xBE, 0xF1};

  USART_StartReception(8);

  USART_StartTransmission((uint8_t*)(&data[0]), sizeof(data));
	while(1u)
	{
	  MAIN_WaitForUpdate();

	  LEDCTRL_Update();
	}

	return 0u;
}


void MAIN_Init(void)
{
  MAIN.timer_ms = 0u;

  CHIP_Init();

  LEDCTRL_init_t ledctrl_init =
  {
    .enable_led_ptr[LEDCTRL_LED_1]  = CHIP_SetGreenLedPin,
    .disable_led_ptr[LEDCTRL_LED_1] = CHIP_ResetGreenLedPin,
    .enable_led_ptr[LEDCTRL_LED_2]  = CHIP_SetBlueLedPin,
    .disable_led_ptr[LEDCTRL_LED_2] = CHIP_ResetBlueLedPin,
    .enable_led_ptr[LEDCTRL_LED_3]  = CHIP_SetRedLedPin,
    .disable_led_ptr[LEDCTRL_LED_3] = CHIP_ResetRedLedPin,
  };
  LEDCTRL_Init(&ledctrl_init);

  USART_Init();
  USART_InitDma();
}


//-------------------------------------------------------------------
void MAIN_DelayMs(uint32_t timeout_ms)
{
  uint32_t end_delay_ms = MAIN_GetTimeMs() + timeout_ms;

  while (MAIN_GetTimeMs() < end_delay_ms);
}


void MAIN_WaitForUpdate(void)
{
  static uint32_t timestamp_begin = 0u;
  while (MAIN.timer_ms <= timestamp_begin);
  timestamp_begin = MAIN.timer_ms;
}


uint32_t MAIN_GetTimeMs(void)
{
  return MAIN.timer_ms;
}


//-------------------------------------------------------------------
void SysTick_Handler(void)
{
  MAIN.timer_ms++;
}
