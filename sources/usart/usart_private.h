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
#include "sources/state_machine/state_machine.h"


#define USART_MAX_MSG_LENGTH ((uint32_t)32u) ///< Max message length that can be transmitted by USART


typedef struct
{
  USART_tx_complete_cb_ptr_t transmit_complete_cb_ptr;
  USART_tx_complete_cb_ptr_t receive_complete_cb_ptr;
  uint8_t tx_buffer[USART_MAX_MSG_LENGTH];
  uint8_t rx_buffer[USART_MAX_MSG_LENGTH];
  USART_error_t error;
  SM_t sm;
} USART_t;


//-------------------------------------------------------------------
static void USART_SetError(USART_error_t error);


//-------------------------------------------------------------------
static void USART_Usart1Init(void);


//-------------------------------------------------------------------
static void USART_DmaStream2Init(void);


//-------------------------------------------------------------------
static void USART_DmaStream7Init(void);

#endif /* USART_PRIVATE_H */
