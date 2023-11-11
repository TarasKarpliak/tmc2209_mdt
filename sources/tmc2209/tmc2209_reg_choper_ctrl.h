/*
 * tmc2209_reg.h
 *
 *  Created on: Nov 11, 2023
 *      Author: taras.karpliak
 */

#ifndef TMC2209_REG_CHOPPER_CTRL_H
#define TMC2209_REG_CHOPPER_CTRL_H

//-------------------------------------------------------------------
typedef struct
{
  uint32_t chop_conf : 32;
} TMC2209_chop_conf_bitmap_t;

typedef union
{
  uint32_t                   value;
  TMC2209_chop_conf_bitmap_t bitmap;
} TMC2209_chop_conf_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t drv_status : 32;
} TMC2209_drv_status_bitmap_t;

typedef union
{
  uint32_t                    value;
  TMC2209_drv_status_bitmap_t bitmap;
} TMC2209_drv_status_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t pwm_conf : 32;
} TMC2209_pwm_conf_bitmap_t;

typedef union
{
  uint32_t                  value;
  TMC2209_pwm_conf_bitmap_t bitmap;
} TMC2209_pwm_conf_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t pwm_scale_sum  : 8;
  uint32_t reserved_1     : 8;
  uint32_t pwm_scale_auto : 8;
  uint32_t reserved_2     : 8;
} TMC2209_pwm_scale_bitmap_t;

typedef union
{
  uint32_t                   value;
  TMC2209_pwm_scale_bitmap_t bitmap;
} TMC2209_pwm_scale_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t pwm_ofs_auto  : 8;
  uint32_t reserved_1    : 8;
  uint32_t pwm_grad_auto : 9;
  uint32_t reserved_2    : 7;
} TMC2209_pwm_auto_bitmap_t;

typedef union
{
  uint32_t                  value;
  TMC2209_pwm_auto_bitmap_t bitmap;
} TMC2209_pwm_auto_t;

#endif /* TMC2209_REG_CHOPPER_CTRL_H */
