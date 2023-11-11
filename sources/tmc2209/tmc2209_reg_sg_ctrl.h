/*
 * tmc2209_reg.h
 *
 *  Created on: Nov 11, 2023
 *      Author: taras.karpliak
 */

#ifndef TMC2209_REG_SG_CTRL_H
#define TMC2209_REG_SG_CTRL_H

//-------------------------------------------------------------------
typedef struct
{
  uint32_t t_cool_thrs : 20;
  uint32_t reserved    : 12;
} TMC2209_t_cool_thrs_bitmap_t;

typedef union
{
  uint32_t                     value;
  TMC2209_t_cool_thrs_bitmap_t bitmap;
} TMC2209_t_cool_thrs_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t sg_thrs  : 8;
  uint32_t reserved : 24;
} TMC2209_sg_thrs_bitmap_t;

typedef union
{
  uint32_t                 value;
  TMC2209_sg_thrs_bitmap_t bitmap;
} TMC2209_sg_thrs_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t sg_result : 10;
  uint32_t reserved  : 22;
} TMC2209_sg_result_bitmap_t;

typedef union
{
  uint32_t                   value;
  TMC2209_sg_result_bitmap_t bitmap;
} TMC2209_sg_result_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t cool_conf : 16;
  uint32_t reserved  : 16;
} TMC2209_cool_conf_bitmap_t;

typedef union
{
  uint32_t                   value;
  TMC2209_cool_conf_bitmap_t bitmap;
} TMC2209_cool_conf_t;

#endif /* TMC2209_REG_SG_CTRL_H */
