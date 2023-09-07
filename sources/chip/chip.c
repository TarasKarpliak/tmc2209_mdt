/*
 * chip.c
 *
 *  Created on: Sep 3, 2023
 *      Author: taras.karpliak
 */

#include "sources/chip/chip_private.h"

CHIP_t CHIP = {0};

// ------------------------------------------------------------------
void CHIP_Init(void)
{
  CHIP_ClockInit();
  CHIP_GpioInit();

  SysTick_Config(SystemCoreClock / 1000u);

  CHIP_Reset();
}


// ------------------------------------------------------------------
static void CHIP_Reset(void)
{
  memset(&CHIP, 0, sizeof(CHIP_t));
}


// ------------------------------------------------------------------
static void CHIP_ClockInit(void)
{
  // Disable the PLL to configure it
  RCC->CR &= ~RCC_CR_PLLON;

  // Wait until PLLRDY bit is cleared
  while ((RCC->CR & RCC_CR_PLLRDY) != 0);

  // Configure the PLL multiplication factor and division factors
  RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ)) |
                 ((RCC_PLLCFGR_PLLSRC_HSI) |
                  (16U << RCC_PLLCFGR_PLLM_Pos) |  // HSI frequency is divided by 2
                  (432U << RCC_PLLCFGR_PLLN_Pos) | // Desired HCLK = 2 * PLLN = 216MHz
                  (0u << RCC_PLLCFGR_PLLP_Pos) |
                  (9u << RCC_PLLCFGR_PLLQ_Pos));

  // Enable the PLL
  RCC->CR |= RCC_CR_PLLON;

  // Wait until PLLRDY bit is set
  while ((RCC->CR & RCC_CR_PLLRDY) == 0);

  // Configure the Flash latency and prefetch
  FLASH->ACR &= ~FLASH_ACR_LATENCY;
  FLASH->ACR |= FLASH_ACR_LATENCY_7WS;
  FLASH->ACR |= FLASH_ACR_PRFTEN;

  // Configure the AHB, APB1, and APB2 prescalers
  RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
  RCC->CFGR |= (RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2);

  // Select the PLL as the system clock source
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  // Wait until the PLL is used as the system clock source
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  SystemCoreClockUpdate();
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


//-------------------------------------------------------------------
void CHIP_Delay(uint32_t timeout_ms)
{
  CHIP.ticks_ms = 0u;

  while (CHIP.ticks_ms < timeout_ms);
}


//-------------------------------------------------------------------
void SysTick_Handler(void)
{
  CHIP.ticks_ms++;
}


