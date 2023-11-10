/*
 * usart.h
 *
 *  Created on: Sep 11, 2023
 *      Author: Taras Karpliak
 */

#ifndef USART_H
#define USART_H

#include "sources/common_def/common_def.h"


typedef void(*USART_tx_complete_cb_ptr_t)(void);
typedef void(*USART_rx_complete_cb_ptr_t)(void);


typedef enum
{
  USART_ERROR_NONE,
} USART_error_t;


typedef enum
{
  USART_STATE_IDLE,
  USART_STATE_TX_IN_PROGRESS,
  USART_STATE_RX_IN_PROGRESS,
} USART_state_t;


//-------------------------------------------------------------------
void USART_Init(void);


//-------------------------------------------------------------------
bool_t USART_StartTransmission(uint8_t* data_ptr, uint8_t data_size);


//-------------------------------------------------------------------
bool_t USART_StartReception(uint8_t data_size);


//-------------------------------------------------------------------
void USART_SetTransmitCompleteCb(USART_tx_complete_cb_ptr_t callback_ptr);


//-------------------------------------------------------------------
void USART_SetReceiveCompleteCb(USART_rx_complete_cb_ptr_t callback_ptr);


//-------------------------------------------------------------------
USART_error_t USART_GetError(void);


//-------------------------------------------------------------------
USART_state_t USART_GetState(void);

#endif /* USART_H */
