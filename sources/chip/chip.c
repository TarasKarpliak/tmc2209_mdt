/*
 * chip.c
 *
 *  Created on: Sep 3, 2023
 *      Author: taras.karpliak
 */

#include "sources/chip/chip_private.h"


// ------------------------------------------------------------------
void CHIP_Init(void)
{
  CHIP_ClockInit();
  CHIP_GpioInit();
}


// ------------------------------------------------------------------
static void CHIP_ClockInit(void)
{
	RCC->CR |= RCC_CR_HSION;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
}


// ------------------------------------------------------------------
static void CHIP_GpioInit(void)
{
	GPIOB->MODER |= GPIO_MODER_MODER0_0;
}


// ------------------------------------------------------------------
void CHIP_SetIndicationPin(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BS0;
}


// ------------------------------------------------------------------
void CHIP_ResetIndicationPin(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BR0;
}
