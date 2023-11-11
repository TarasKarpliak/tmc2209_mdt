/*
 * tmc2209_reg.h
 *
 *  Created on: Nov 11, 2023
 *      Author: taras.karpliak
 */

#ifndef TMC2209_REG_GENERAL_H
#define TMC2209_REG_GENERAL_H

//-------------------------------------------------------------------
typedef struct
{
  uint32_t i_scale_analog   : 1;
  uint32_t internal_r_sens  : 1;
  uint32_t en_spread_cycle  : 1;
  uint32_t shaft            : 1;
  uint32_t index_otpw       : 1;
  uint32_t index_step       : 1;
  uint32_t pdn_disable      : 1;
  uint32_t mstep_reg_select : 1;
  uint32_t multistep_filt   : 1;
  uint32_t test_mode        : 1;
  uint32_t reserved         : 22;
} TMC2209_gconf_bitmap_t;

typedef union
{
  uint32_t               value;
  TMC2209_gconf_bitmap_t bitmap;
} TMC2209_gconf_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t reset    : 1;
  uint32_t drv_err  : 1;
  uint32_t uv_cp    : 1;
  uint32_t reserved : 29;
} TMC2209_gstat_bitmap_t;

typedef union
{
  uint32_t               value;
  TMC2209_gstat_bitmap_t bitmap;
} TMC2209_gstat_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t counter_value : 8;
  uint32_t reserved      : 24;
} TMC2209_ifcnt_bitmap_t;

typedef union
{
  uint32_t               value;
  TMC2209_ifcnt_bitmap_t bitmap;
} TMC2209_ifcnt_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t reserved_1 : 8;
  uint32_t send_delay : 4;
  uint32_t reserved_2 : 20;
} TMC2209_nodeconf_bitmap_t;

typedef union
{
  uint32_t                  value;
  TMC2209_nodeconf_bitmap_t bitmap;
} TMC2209_nodeconf_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t bit        : 2;
  uint32_t reserved_1 : 1;
  uint32_t byte       : 2;
  uint32_t reserved_2 : 2;
  uint32_t magig      : 8;
  uint32_t reserved   : 16;
} TMC2209_otp_prog_bitmap_t;

typedef union
{
  uint32_t                  value;
  TMC2209_otp_prog_bitmap_t bitmap;
} TMC2209_otp_prog_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t otp_0    : 8;
  uint32_t otp_1    : 8;
  uint32_t otp_2    : 8;
  uint32_t reserved : 8;
} TMC2209_otp_read_bitmap_t;

typedef union
{
  uint32_t                  value;
  TMC2209_otp_read_bitmap_t bitmap;
} TMC2209_otp_read_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t enn        : 1;
  uint32_t reserved_1 : 1;
  uint32_t ms1        : 1;
  uint32_t ms2        : 1;
  uint32_t diag       : 1;
  uint32_t reserved_2 : 1;
  uint32_t pdn_uart   : 1;
  uint32_t step       : 1;
  uint32_t stread_en  : 1;
  uint32_t dir        : 1;
  uint32_t reserved_3 : 14;
  uint32_t version    : 8;
} TMC2209_ioin_bitmap_t;

typedef union
{
  uint32_t              value;
  TMC2209_ioin_bitmap_t bitmap;
} TMC2209_ioin_t;


//-------------------------------------------------------------------
typedef struct
{
  uint32_t fclk_trim  : 5;
  uint32_t reserved_1 : 3;
  uint32_t ot_trim    : 2;
  uint32_t reserved_2 : 22;
} TMC2209_factory_conf_bitmap_t;

typedef union
{
  uint32_t                      value;
  TMC2209_factory_conf_bitmap_t bitmap;
} TMC2209_factory_conf_t;

#endif /* TMC2209_REG_GENERAL_H */
