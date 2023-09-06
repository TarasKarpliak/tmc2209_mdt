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
  SETBIT(RCC->CR, RCC_CR_HSION_Pos); // HSI ON

  RCC->PLLCFGR |= (uint32_t)8u << RCC_PLLCFGR_PLLM_Pos; // PLLM /8
  RCC->PLLCFGR |= (uint32_t)216u << RCC_PLLCFGR_PLLN_Pos; // PLLN *216

  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  RCC->PLLSAICFGR |= (uint32_t)192u << RCC_PLLSAICFGR_PLLSAIN_Pos;
  SETBIT(RCC->PLLSAICFGR, RCC_PLLSAICFGR_PLLSAIP_Pos);

  SETBIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL_Pos);

  SETBIT(RCC->CR, RCC_CR_PLLON_Pos);

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
