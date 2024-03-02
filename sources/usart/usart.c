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

  if (data_size <= USART_MAX_MSG_LENGTH)
  {
    memcpy(&USART.tx_buffer[0u], data_ptr, data_size);

    LL_USART_DisableDirectionRx(USART1);

    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_7, data_size);
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_7);

    result = TRUE;
  }

  return result;
}


//-------------------------------------------------------------------
bool_t USART_StartReception(uint8_t data_size)
{
  bool_t result = FALSE;

  if (data_size <= USART_MAX_MSG_LENGTH)
  {
    memset(&USART.rx_buffer[0u], 0u, USART_MAX_MSG_LENGTH);

    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, data_size);
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);

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
  LL_USART_InitTypeDef init_struct =
  {
    .BaudRate            = 115200u,
    .DataWidth           = LL_USART_DATAWIDTH_8B,
    .StopBits            = LL_USART_STOPBITS_1,
    .Parity              = LL_USART_PARITY_NONE,
    .TransferDirection   = LL_USART_DIRECTION_TX_RX,
    .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
    .OverSampling        = LL_USART_OVERSAMPLING_16,
  };
  ASSERT(LL_USART_Init(USART1, &init_struct) == SUCCESS)

  LL_USART_ConfigHalfDuplexMode(USART1);

  LL_USART_EnableDMAReq_TX(USART1);
  LL_USART_EnableDMAReq_RX(USART1);

  LL_USART_EnableIT_TC(USART1);

  LL_USART_Enable(USART1);

  NVIC_SetPriority(USART1_IRQn, 0); // Set Priority for USART1
  NVIC_EnableIRQ(USART1_IRQn);      // Enable interrupt
}


//-------------------------------------------------------------------
static void USART_DmaStream2Init(void)
{
  LL_DMA_InitTypeDef init_struct =
  {
    .PeriphOrM2MSrcAddress = (uint32_t)&USART1->RDR,
    .MemoryOrM2MDstAddress = (uint32_t)&USART.rx_buffer[0u],
    .Direction             = LL_DMA_DIRECTION_PERIPH_TO_MEMORY,
    .Mode                  = LL_DMA_MODE_NORMAL,
    .PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT,
    .MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT,
    .PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE,
    .MemoryOrM2MDstDataSize = LL_DMA_PDATAALIGN_BYTE,
    .NbData                 = 0u,
    .Channel                = LL_DMA_CHANNEL_4,
    .Priority               = LL_DMA_PRIORITY_HIGH,
    .FIFOMode               = LL_DMA_FIFOMODE_DISABLE,
    .FIFOThreshold          = LL_DMA_FIFOTHRESHOLD_1_4,
    .MemBurst               = LL_DMA_MBURST_SINGLE,
    .PeriphBurst            = LL_DMA_PBURST_SINGLE,
  };
  ASSERT(LL_DMA_Init(DMA2, LL_DMA_STREAM_2, &init_struct) == SUCCESS)

  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_2);
}


//-------------------------------------------------------------------
static void USART_DmaStream7Init(void)
{
  LL_DMA_InitTypeDef init_struct =
  {
    .PeriphOrM2MSrcAddress = (uint32_t)&USART1->TDR,
    .MemoryOrM2MDstAddress = (uint32_t)&USART.tx_buffer[0u],
    .Direction             = LL_DMA_DIRECTION_MEMORY_TO_PERIPH,
    .Mode                  = LL_DMA_MODE_NORMAL,
    .PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT,
    .MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT,
    .PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE,
    .MemoryOrM2MDstDataSize = LL_DMA_PDATAALIGN_BYTE,
    .NbData                 = 0u,
    .Channel                = LL_DMA_CHANNEL_4,
    .Priority               = LL_DMA_PRIORITY_HIGH,
    .FIFOMode               = LL_DMA_FIFOMODE_DISABLE,
    .FIFOThreshold          = LL_DMA_FIFOTHRESHOLD_1_4,
    .MemBurst               = LL_DMA_MBURST_SINGLE,
    .PeriphBurst            = LL_DMA_PBURST_SINGLE,
  };
  ASSERT(LL_DMA_Init(DMA2, LL_DMA_STREAM_7, &init_struct) == SUCCESS)

  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_7);
}


//-------------------------------------------------------------------
void USART1_IRQHandler(void)
{
  if (LL_USART_IsActiveFlag_TC(USART1) != 0u)
  {
    LL_USART_ClearFlag_TC(USART1);

    LL_USART_EnableDirectionRx(USART1);

    if (USART.transmit_complete_cb_ptr != NULL)
    {
      USART.transmit_complete_cb_ptr();
    }
  }
}


//-------------------------------------------------------------------
void DMA2_Stream7_IRQHandler(void)
{
  if (LL_DMA_IsActiveFlag_TC7(DMA2) != 0u)
  {
    LL_DMA_ClearFlag_TC7(DMA2);
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_7);
  }
}


//-------------------------------------------------------------------
void DMA2_Stream2_IRQHandler(void)
{
  if (LL_DMA_IsActiveFlag_TC2(DMA2) != 0u)
  {
    LL_DMA_ClearFlag_TC2(DMA2);
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);

    if (USART.receive_complete_cb_ptr != NULL)
    {
      USART.receive_complete_cb_ptr();
    }
  }
}
