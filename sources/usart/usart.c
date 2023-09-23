/*
 * usart.c
 *
 *  Created on: Sep 11, 2023
 *      Author: Taras Karpliak
 */

#include "usart_private.h"

void USART_Init(void)
{
  USART1->CR1 &= ~(USART_CR1_M_0); // 8bit
  USART1->CR1 &= ~(USART_CR1_M_1); // 8bit

  USART1->CR2 &= ~(USART_CR2_STOP_0); // 1 top bit
  USART1->CR2 &= ~(USART_CR2_STOP_1); // 1stop bit

  USART1->CR1 &= ~(USART_CR1_OVER8); // Oversampling by 16

  USART1->CR1 &= ~(USART_CR1_PCE); // No parity

  USART1->CR3 |= USART_CR3_HDSEL; // Half duplex mode is selected
  USART1->CR2 &= ~(USART_CR2_LINEN); // must be cleared for Half duplex mode
  USART1->CR2 &= ~(USART_CR2_CLKEN); // must be cleared for Half duplex mode
  USART1->CR3 &= ~(USART_CR3_SCEN); // must be cleared for Half duplex mode
  USART1->CR3 &= ~(USART_CR3_IREN); // must be cleared for Half duplex mode

  USART1->CR3 |= USART_CR3_DMAR; // DMA enable receiver
  USART1->CR3 |= USART_CR3_DMAT; // DMA enable receiver

  USART1->BRR = CHIP_GetSysCoreClockHz() / 4000000u; // select baudrate

  USART1->CR1 |= USART_CR1_TE; // transmit enable
  USART1->CR1 |= USART_CR1_RE; // transmit enable

  USART1->CR1 |= USART_CR1_UE; // Enable USART
}
