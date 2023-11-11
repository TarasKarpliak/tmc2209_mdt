/*
 * tmc2209_reg.h
 *
 *  Created on: Nov 11, 2023
 *      Author: taras.karpliak
 */

#ifndef TMC2209_REG_SEQUENCER_H
#define TMC2209_REG_SEQUENCER_H

//-------------------------------------------------------------------
typedef struct
{
  uint32_t ms_cnt   : 10;
  uint32_t reserved : 22;
} TMC2209_ms_cnt_bitmap_t;

typedef union
{
  uint32_t                value;
  TMC2209_ms_cnt_bitmap_t bitmap;
} TMC2209_ms_cnt_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t cur_b      : 9;
  uint32_t reserved_1 : 7;
  uint32_t cut_a      : 9;
  uint32_t reserved_2 : 7;
} TMC2209_ms_cur_act_bitmap_t;

typedef union
{
  uint32_t                    value;
  TMC2209_ms_cur_act_bitmap_t bitmap;
} TMC2209_ms_cur_act_t;


#endif /* TMC2209_REG_SEQUENCER_H */
