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
  CHIP_NvicInit();
  CHIP_GpioInit();

  SysTick_Config(218857); //(SystemCoreClock / 1000u + 12);
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

  RCC->DCKCFGR2 |= RCC_DCKCFGR2_USART1SEL_0; // System clock is selected as USART 1 clock

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  SystemCoreClockUpdate();
}


// ------------------------------------------------------------------
static void CHIP_GpioInit(void)
{
	GPIOB->MODER |= GPIO_MODER_MODER0_0;
  GPIOB->MODER |= GPIO_MODER_MODER7_0;
  GPIOB->MODER |= GPIO_MODER_MODER14_0;

  // Configure the alternate function for PA9 (USART1_TX)
  GPIOA->AFR[1] &= ~(0xF << (4 * (9 - 8))); // Clear AF bits
  GPIOA->AFR[1] |= (7 << (4 * (9 - 8)));    // Set AF7 for USART1_TX
  GPIOA->MODER |= GPIO_MODER_MODER9_1;      // Set as alternate function
  GPIOA->OTYPER |= GPIO_OTYPER_OT9;         // Open-drain
}


// ------------------------------------------------------------------
static void CHIP_NvicInit(void)
{
  NVIC_SetPriority(DMA2_Stream7_IRQn, 0); // Set Priority for DMA2
  NVIC_EnableIRQ(DMA2_Stream7_IRQn);      // Enable interrupt
  NVIC_SetPriority(DMA2_Stream2_IRQn, 0); // Set Priority for DMA2
  NVIC_EnableIRQ(DMA2_Stream2_IRQn);      // Enable interrupt
}


// ------------------------------------------------------------------
void CHIP_SetGreenLedPin(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BS0;
}


// ------------------------------------------------------------------
void CHIP_ResetGreenLedPin(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BR0;
}


// ------------------------------------------------------------------
uint32_t CHIP_GetSysCoreClockHz(void)
{
  return SystemCoreClock;
}





