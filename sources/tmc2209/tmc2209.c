/*
 * tmc2209.c
 *
 *  Created on: Nov 10, 2023
 *      Author: taras.karpliak
 */

#include "tmc2209_private.h"


TMC2209_t TMC2209 = {0};


//-------------------------------------------------------------------
void TMC2209_Init(TMC2209_init_t* init_ptr)
{
  ASSERT(init_ptr != NULL);

  TMC2209.node_addr = init_ptr->node_addr;

  TMC2209_Reset();
}


//-------------------------------------------------------------------
void TMC2209_Reset(void)
{
  SM_Init(&TMC2209.sm, TMC2209_STATE_CONFIG);

  USART_SetTransmitCompleteCb(TMC2209_TransmitCompleteCb);
  USART_SetReceiveCompleteCb(TMC2209_ReceiveCompleteCb);

  RESETBIT(TMC2209.status_bitmap, TMC2209_TRANSMIT_COMPLETE);
  RESETBIT(TMC2209.status_bitmap, TMC2209_RECEIVE_COMPLETE);
  RESETBIT(TMC2209.status_bitmap, TMC2209_RECEPTION_REQUESTED);

  TMC2209_SetError(TMC2209_ERROR_NONE);
}


//-------------------------------------------------------------------
void TMC2209_Update(void)
{
  switch (TMC2209_GetState())
  {
    case TMC2209_STATE_CONFIG:
      if (SM_HasChanged(&TMC2209.sm) == TRUE)
      {
        TMC2209_SendData(TMC2209.node_addr, 0x00, 0x00);
      }

      if (TMC2209_IsTransmissionCompleted() == TRUE)
      {
        TMC2209_SetState(TMC2209_STATE_CHECK_CONFIG);
      }
      break;

    case TMC2209_STATE_CHECK_CONFIG:
      if (SM_HasChanged(&TMC2209.sm) == TRUE)
      {
        TMC2209_RequestData(TMC2209.node_addr, 0x00);
      }

      if (TMC2209_IsReceptionCompleted() == TRUE)
      {
        if (TMC2209_CheckRequestedData(0x00, 0x00) == TRUE)
        {
          TMC2209_SetState(TMC2209_STATE_IDLE);
        }
        else
        {
          TMC2209_SetState(TMC2209_STATE_ERROR);
        }
      }
      break;

    case TMC2209_STATE_IDLE:
      break;

    case TMC2209_STATE_ERROR:
      break;

    case TMC2209_STATE_TOTAL:
    default:
      break;
  }
}


//-------------------------------------------------------------------
TMC2209_state_t TMC2209_GetState(void)
{
  return (TMC2209_state_t)SM_GetState(&TMC2209.sm);
}


//-------------------------------------------------------------------
TMC2209_error_t TMC2209_GetError(void)
{
  return TMC2209.error;
}


//-------------------------------------------------------------------
static void TMC2209_SetState(TMC2209_state_t new_state)
{
  SM_ChangeTo(&TMC2209.sm, new_state);
}


//-------------------------------------------------------------------
static void TMC2209_SetError(TMC2209_error_t new_error)
{
  TMC2209.error = new_error;
}


//-------------------------------------------------------------------
static void TMC2209_TransmitCompleteCb(void)
{
  SETBIT(TMC2209.status_bitmap, TMC2209_TRANSMIT_COMPLETE);

  if (CHECKBIT(TMC2209.status_bitmap, TMC2209_RECEPTION_REQUESTED) == TRUE)
  {
    USART_StartReception(TCM2209_REPLY_TELEGRAM_SIZE);
  }
}


//-------------------------------------------------------------------
static void TMC2209_ReceiveCompleteCb(void)
{
  memset(&TMC2209.received_data[0u], 0, TCM2209_REPLY_TELEGRAM_SIZE);

  SETBIT(TMC2209.status_bitmap, TMC2209_RECEIVE_COMPLETE);
  RESETBIT(TMC2209.status_bitmap, TMC2209_RECEPTION_REQUESTED);

  USART_GetReceivedData(&TMC2209.received_data[0u], TMC2209_WRITE_TELEGRAM_SIZE);
}


//-------------------------------------------------------------------
static void TMC2209_SendData(uint8_t node_addr, uint8_t reg_addr, uint32_t data)
{
  uint8_t transmit_data[TMC2209_WRITE_TELEGRAM_SIZE] = {0u};

  RESETBIT(TMC2209.status_bitmap, TMC2209_TRANSMIT_COMPLETE);

  transmit_data[TMC2209_BYTE_SYNC]      = TMC2209_SYNC_NIBBLE_MASK;
  transmit_data[TMC2209_BYTE_NODE_ADDR] = node_addr;
  transmit_data[TMC2209_BYTE_REG_ADDR]  = reg_addr | 0x80;
  transmit_data[TMC2209_DATA_BYTE_3]    = (uint8_t)((data >> 24u) & 0xFF);
  transmit_data[TMC2209_DATA_BYTE_2]    = (uint8_t)((data >> 16u) & 0xFF);
  transmit_data[TMC2209_DATA_BYTE_1]    = (uint8_t)((data >> 8u) & 0xFF);
  transmit_data[TMC2209_DATA_BYTE_0]    = (uint8_t)((data >> 0u) & 0xFF);

  TMC2209_CalcCrc(&transmit_data[0], TMC2209_WRITE_TELEGRAM_SIZE);

  USART_StartTransmission(&transmit_data[0], TMC2209_WRITE_TELEGRAM_SIZE);
}


//-------------------------------------------------------------------
static bool_t TMC2209_IsTransmissionCompleted(void)
{
  bool_t result = FALSE;

  if (CHECKBIT(TMC2209.status_bitmap, TMC2209_TRANSMIT_COMPLETE) == TRUE)
  {
    result = TRUE;
  }

  return result;
}


//-------------------------------------------------------------------
static void TMC2209_RequestData(uint8_t node_addr, uint8_t reg_addr)
{
  uint8_t transmit_data[TMC2209_READ_REQUEST_TELEGRAM_SIZE] = {0u};

  RESETBIT(TMC2209.status_bitmap, TMC2209_RECEIVE_COMPLETE);
  SETBIT(TMC2209.status_bitmap, TMC2209_RECEPTION_REQUESTED);

  transmit_data[TMC2209_BYTE_SYNC]      = TMC2209_SYNC_NIBBLE_MASK;
  transmit_data[TMC2209_BYTE_NODE_ADDR] = node_addr;
  transmit_data[TMC2209_BYTE_REG_ADDR]  = reg_addr & 0x7F;

  TMC2209_CalcCrc(&transmit_data[0], TMC2209_READ_REQUEST_TELEGRAM_SIZE);

  USART_StartTransmission(&transmit_data[0], TMC2209_READ_REQUEST_TELEGRAM_SIZE);
}


//-------------------------------------------------------------------
static bool_t TMC2209_IsReceptionCompleted(void)
{
  bool_t result = FALSE;

  if (CHECKBIT(TMC2209.status_bitmap, TMC2209_RECEIVE_COMPLETE) == TRUE)
  {
    result = TRUE;
  }

  return result;
}


//-------------------------------------------------------------------
static bool_t TMC2209_CheckRequestedData(uint8_t reg_addr, uint32_t expected_data)
{
  bool_t result = FALSE;
  uint8_t expected_data_buff[TCM2209_REPLY_TELEGRAM_SIZE] = {0u};

  expected_data_buff[TMC2209_BYTE_SYNC]      = TMC2209_SYNC_NIBBLE_MASK;
  expected_data_buff[TMC2209_BYTE_NODE_ADDR] = TMC2209_MASTER_ADDR_MASK;
  expected_data_buff[TMC2209_BYTE_REG_ADDR]  = reg_addr & 0x7F;
  expected_data_buff[TMC2209_DATA_BYTE_3]    = (uint8_t)((expected_data >> 24u) & 0xFF);
  expected_data_buff[TMC2209_DATA_BYTE_2]    = (uint8_t)((expected_data >> 16u) & 0xFF);
  expected_data_buff[TMC2209_DATA_BYTE_1]    = (uint8_t)((expected_data >> 8u) & 0xFF);
  expected_data_buff[TMC2209_DATA_BYTE_0]    = (uint8_t)((expected_data >> 0u) & 0xFF);

  TMC2209_CalcCrc(&expected_data_buff[0], TCM2209_REPLY_TELEGRAM_SIZE);

  if (memcmp(&TMC2209.received_data[0u], &expected_data_buff[0u], TCM2209_REPLY_TELEGRAM_SIZE) == 0u)
  {
    result = TRUE;
  }

  return result;
}


//-------------------------------------------------------------------
static void TMC2209_CalcCrc(uint8_t* datagram, uint8_t datagram_length)
{
  uint8_t* crc = datagram + (datagram_length - 1);        // CRC located in last byte of message
  uint8_t current_byte;

  *crc = 0;

  for (uint8_t i = 0; i < (datagram_length - 1); i++)     // Execute for all bytes of a message
  {
    current_byte = datagram[i];                           // Retrieve a byte to be sent from Array

    for (uint8_t j = 0; j < 8u; j++)
    {
      if ((*crc >> 7u) ^ (current_byte & 0x01))           // update CRC based result of XOR operation
      {
        *crc = (*crc << 1) ^ 0x07;
      }
      else
      {
        *crc = (*crc << 1);
      }

      current_byte = current_byte >> 1;
    }
  }
}
