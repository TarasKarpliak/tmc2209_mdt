/*
 * usart.c
 *
 *  Created on: Sep 11, 2023
 *      Author: Taras Karpliak
 */

#include "usart_private.h"

USART_t USART = {0};


//-------------------------------------------------------------------
void USART_Init(void)
{
  USART_Usart1Init();
  USART_DmaStream2Init();
  USART_DmaStream7Init();

  USART_SetError(USART_ERROR_NONE);
  SM_Init(&USART.sm, USART_STATE_IDLE);
}


//-------------------------------------------------------------------
bool_t USART_StartTransmission(uint8_t* data_ptr, uint8_t data_size)
{
  bool_t result = FALSE;

  if ((SM_GetState(&USART.sm) == USART_STATE_IDLE) &&
      (data_size <= USART_MAX_MSG_LENGTH))
  {
    memcpy(&USART.tx_buffer[0u], data_ptr, data_size);

    DMA2_Stream7->CR &= (~DMA_SxCR_EN);
    DMA2_Stream7->M0AR = (uint32_t)&USART.tx_buffer[0u];
    DMA2_Stream7->NDTR = data_size;
    DMA2_Stream7->CR |= DMA_SxCR_EN;

    SM_ChangeTo(&USART.sm, USART_STATE_TX_IN_PROGRESS);
    result = TRUE;
  }

  return result;
}


//-------------------------------------------------------------------
bool_t USART_StartReception(uint8_t data_size)
{
  bool_t result = FALSE;

  if ((SM_GetState(&USART.sm) == USART_STATE_IDLE) &&
      (data_size <= USART_MAX_MSG_LENGTH))
  {
    memset(&USART.rx_buffer[0u], 0u, USART_MAX_MSG_LENGTH);

    DMA2_Stream2->CR &= (~DMA_SxCR_EN);
    DMA2_Stream2->M0AR = (uint32_t)&USART.rx_buffer[0u];
    DMA2_Stream2->NDTR = data_size;
    DMA2_Stream2->CR |= DMA_SxCR_EN;

    SM_ChangeTo(&USART.sm, USART_STATE_RX_IN_PROGRESS);
    result = TRUE;
  }

  return result;
}


//-------------------------------------------------------------------
void USART_SetTransmitCompleteCb(USART_tx_complete_cb_ptr_t callback_ptr)
{
  ASSERT(callback_ptr != NULL);

  USART.transmit_complete_cb_ptr = callback_ptr;
}


//-------------------------------------------------------------------
void USART_SetReceiveCompleteCb(USART_rx_complete_cb_ptr_t callback_ptr)
{
  ASSERT(callback_ptr != NULL);

  USART.receive_complete_cb_ptr = callback_ptr;
}


//-------------------------------------------------------------------
void USART_GetReceivedData(uint8_t* buffer_ptr, uint8_t data_size)
{
  if (data_size <= USART_MAX_MSG_LENGTH)
  {
    memcpy(buffer_ptr, &USART.rx_buffer[0u], data_size);
  }
}


//-------------------------------------------------------------------
static void USART_SetError(USART_error_t error)
{
  USART.error = error;
}


//-------------------------------------------------------------------
static void USART_Usart1Init(void)
{
  USART1->CR1 &= ~(USART_CR1_M_0); // 8bit
  USART1->CR1 &= ~(USART_CR1_M_1); // 8bit

  USART1->CR2 &= ~(USART_CR2_STOP_0); // 1 stop bit
  USART1->CR2 &= ~(USART_CR2_STOP_1); // 1 stop bit

  USART1->CR1 &= ~(USART_CR1_OVER8); // Oversampling by 16

  USART1->CR1 &= ~(USART_CR1_PCE); // No parity

  USART1->CR3 |= USART_CR3_HDSEL; // Half duplex mode is selected
  USART1->CR2 &= ~(USART_CR2_LINEN); // must be cleared for Half duplex mode
  USART1->CR2 &= ~(USART_CR2_CLKEN); // must be cleared for Half duplex mode
  USART1->CR3 &= ~(USART_CR3_SCEN); // must be cleared for Half duplex mode
  USART1->CR3 &= ~(USART_CR3_IREN); // must be cleared for Half duplex mode

  USART1->BRR = CHIP_GetSysCoreClockHz() / 4000000; // select baudrate

  USART1->CR1 |= USART_CR1_TE; // transmit enable
  USART1->CR1 |= USART_CR1_RE; // transmit enable

  USART1->CR3 |= USART_CR3_DMAR; // DMA enable receiver
  USART1->CR3 |= USART_CR3_DMAT; // DMA enable receiver

  USART1->CR1 |= USART_CR1_UE; // Enable USART
}


//-------------------------------------------------------------------
static void USART_DmaStream2Init(void)
{
  DMA2_Stream2->CR |= DMA_SxCR_CHSEL_2; // channel 4 of DMA stream 2 (USART1_RX)
  DMA2_Stream2->CR |= DMA_SxCR_PL_0 | DMA_SxCR_PL_1; // priority level (very high)
  DMA2_Stream2->CR &= ~(DMA_SxCR_DIR_1 | DMA_SxCR_DIR_1); //  data transfer direction (peripheral-to-memory)
  DMA2_Stream2->CR &= ~(DMA_SxCR_PSIZE_0 | DMA_SxCR_PSIZE_1); // peripheral data size (byte (8-bit)
  DMA2_Stream2->CR &= ~(DMA_SxCR_MSIZE_0 | DMA_SxCR_MSIZE_1); // memory data size (byte (8-bit)
  DMA2_Stream2->CR |= DMA_SxCR_MINC; // memory increment mode
  DMA2_Stream2->CR &= ~DMA_SxCR_PFCTRL; // peripheral flow controller (DMA is the flow controller)
  DMA2_Stream2->CR |= DMA_SxCR_TCIE; // TC interrupt enabled
  DMA2_Stream2->PAR = (uint32_t)&USART1->RDR;
}


//-------------------------------------------------------------------
static void USART_DmaStream7Init(void)
{
  DMA2_Stream7->CR |= DMA_SxCR_CHSEL_2; // channel 4 of DMA stream 7 (USART1_TX)
  DMA2_Stream7->CR |= DMA_SxCR_PL_0 | DMA_SxCR_PL_1; // priority level (very high)
  DMA2_Stream7->CR |= DMA_SxCR_DIR_0; //  data transfer direction (memory-to-peripheral)
  DMA2_Stream7->CR &= ~(DMA_SxCR_PSIZE_0 | DMA_SxCR_PSIZE_1); // peripheral data size (byte (8-bit)
  DMA2_Stream7->CR &= ~(DMA_SxCR_MSIZE_0 | DMA_SxCR_MSIZE_1); // memory data size (byte (8-bit)
  DMA2_Stream7->CR |= DMA_SxCR_MINC; // memory increment mode
  DMA2_Stream7->CR &= ~DMA_SxCR_PFCTRL; // peripheral flow controller (DMA is the flow controller)
  DMA2_Stream7->CR |= DMA_SxCR_TCIE; // TC interrupt enabled
  DMA2_Stream7->PAR = (uint32_t)&USART1->TDR;
}


//-------------------------------------------------------------------
void DMA2_Stream7_IRQHandler(void)
{
  if ((DMA2->HISR & DMA_HISR_TCIF7) != 0)
  {
    DMA2->HIFCR |= DMA_HIFCR_CTCIF7;

    if (SM_GetState(&USART.sm) == USART_STATE_TX_IN_PROGRESS)
    {
      SM_ChangeTo(&USART.sm, USART_STATE_IDLE);
    }

    if (USART.transmit_complete_cb_ptr != NULL)
    {
      USART.transmit_complete_cb_ptr();
    }
  }
}


//-------------------------------------------------------------------
void DMA2_Stream2_IRQHandler(void)
{
  if ((DMA2->LISR & DMA_LISR_TCIF2) != 0)
  {
    DMA2->LIFCR |= DMA_LIFCR_CTCIF2;

    if (SM_GetState(&USART.sm) == USART_STATE_RX_IN_PROGRESS)
    {
      SM_ChangeTo(&USART.sm, USART_STATE_IDLE);
    }

    if (USART.receive_complete_cb_ptr != NULL)
    {
      USART.receive_complete_cb_ptr();
    }
  }
}
