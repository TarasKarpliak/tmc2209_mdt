/*
 * usart_private.h
 *
 *  Created on: Sep 11, 2023
 *      Author: Taras Karpliak
 */

#ifndef USART_PRIVATE_H
#define USART_PRIVATE_H

#include "usart.h"
#include "string.h"
#include "sources/chip/chip.h"

#define USART_MAX_MSG_LENGTH ((uint32_t)32u) ///< Max message length that can be transmitted by USART

typedef enum
{
  USART_TX_IN_PROGRESS,
  USART_RX_IN_PROGRESS,
} USART_status_bitmap_t;

typedef struct
{
  uint8_t tx_buffer[USART_MAX_MSG_LENGTH];
  uint8_t rx_buffer[USART_MAX_MSG_LENGTH];
  uint8_t received_msg_length;
  uint16_t status_bitmap;
} USART_t;

#endif /* USART_PRIVATE_H */
