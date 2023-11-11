/*
 * tmc2209_private.h
 *
 *  Created on: Nov 10, 2023
 *      Author: taras.karpliak
 */

#ifndef TMC2209_PRIVATE_H
#define TMC2209_PRIVATE_H

#include "tmc2209.h"
#include "string.h"
#include "sources/state_machine/state_machine.h"
#include "sources/usart/usart.h"

#define TMC2209_SYNC_NIBBLE_MASK                ((uint8_t)0x0A)
#define TMC2209_MASTER_ADDR_MASK                ((uint8_t)0xFF)

#define TMC2209_BYTE_SYNC                       ((uint8_t)0x00)
#define TMC2209_BYTE_NODE_ADDR                  ((uint8_t)0x01)
#define TMC2209_BYTE_REG_ADDR                   ((uint8_t)0x02)

#define TMC2209_DATA_BYTE_3                     ((uint8_t)0x03)
#define TMC2209_DATA_BYTE_2                     ((uint8_t)0x04)
#define TMC2209_DATA_BYTE_1                     ((uint8_t)0x05)
#define TMC2209_DATA_BYTE_0                     ((uint8_t)0x06)

#define TMC2209_WRITE_TELEGRAM_SIZE             ((uint8_t)0x08)
#define TMC2209_READ_REQUEST_TELEGRAM_SIZE      ((uint8_t)0x04)
#define TCM2209_REPLY_TELEGRAM_SIZE             ((uint8_t)0x08)


typedef enum
{
  TMC2209_TRANSMIT_COMPLETE,
  TMC2209_RECEIVE_COMPLETE,
  TMC2209_RECEPTION_REQUESTED,
} TMC2209_status_bitmap_t;


typedef struct
{
  uint8_t node_addr;
  TMC2209_error_t error;
  SM_t sm;
  uint8_t status_bitmap;
  uint8_t received_data[TCM2209_REPLY_TELEGRAM_SIZE];
} TMC2209_t;


//-------------------------------------------------------------------
static void TMC2209_SetState(TMC2209_state_t new_state);


//-------------------------------------------------------------------
static void TMC2209_SetError(TMC2209_error_t new_error);


//-------------------------------------------------------------------
static void TMC2209_TransmitCompleteCb(void);


//-------------------------------------------------------------------
static void TMC2209_ReceiveCompleteCb(void);


//-------------------------------------------------------------------
static void TMC2209_SendData(uint8_t node_addr, uint8_t reg_addr, uint32_t data);


//-------------------------------------------------------------------
static bool_t TMC2209_IsTransmissionCompleted(void);

//-------------------------------------------------------------------
static void TMC2209_RequestData(uint8_t node_addr, uint8_t reg_addr);


//-------------------------------------------------------------------
static bool_t TMC2209_IsReceptionCompleted(void);

//-------------------------------------------------------------------
static bool_t TMC2209_CheckRequestedData(uint8_t reg_addr, uint32_t expected_data);


//-------------------------------------------------------------------
static void TMC2209_CalcCrc(uint8_t* datagram, uint8_t datagram_length);

#endif /* TMC2209_PRIVATE_H */
