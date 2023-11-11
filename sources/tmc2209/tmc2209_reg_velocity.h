/*
 * tmc2209_reg.h
 *
 *  Created on: Nov 11, 2023
 *      Author: taras.karpliak
 */

#ifndef TMC2209_REG_VELOCITY_H
#define TMC2209_REG_VELOCITY_H

//-------------------------------------------------------------------
typedef struct
{
  uint32_t i_hold       : 5;
  uint32_t reserved_1   : 3;
  uint32_t i_run        : 5;
  uint32_t reserved_2   : 3;
  uint32_t i_hold_delay : 4;
  uint32_t reserved_2   : 12;
} TMC2209_ihold_irun_bitmap_t;

typedef union
{
  uint32_t                    value;
  TMC2209_ihold_irun_bitmap_t bitmap;
} TMC2209_ihold_irun_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t t_power_down : 8;
  uint32_t reserved     : 24;
} TMC2209_t_power_down_bitmap_t;

typedef union
{
  uint32_t                      value;
  TMC2209_t_power_down_bitmap_t bitmap;
} TMC2209_t_power_down_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t t_step   : 20;
  uint32_t reserved : 12;
} TMC2209_t_step_bitmap_t;

typedef union
{
  uint32_t                value;
  TMC2209_t_step_bitmap_t bitmap;
} TMC2209_t_step_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t t_pwm_thrs : 20;
  uint32_t reserved   : 12;
} TMC2209_t_pwm_thrs_bitmap_t;

typedef union
{
  uint32_t                    value;
  TMC2209_t_pwm_thrs_bitmap_t bitmap;
} TMC2209_t_pwm_thrs_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t v_actual : 24;
  uint32_t reserved : 8;
} TMC2209_v_actual_bitmap_t;

typedef union
{
  uint32_t                  value;
  TMC2209_v_actual_bitmap_t bitmap;
} TMC2209_v_actual_t;


#endif /* TMC2209_REG_VELOCITY_H */
