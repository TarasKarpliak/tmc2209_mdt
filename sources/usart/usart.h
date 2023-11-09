/*
 * usart.h
 *
 *  Created on: Sep 11, 2023
 *      Author: Taras Karpliak
 */

#ifndef USART_H
#define USART_H

#include "sources/common_def/common_def.h"

void USART_Init(void);
void USART_InitDma(void);

void USART_ReceptionCompleteCb(void);

void USART_TramsmissionCompleteCb(void);

void USRT_StartReception(uint8_t* rx_data_ptr, uint8_t data_size);



void USART_StartTransmission(uint8_t* data_ptr, uint8_t data_size);

void USART_StartReception(uint8_t data_size);

#endif /* USART_H */
