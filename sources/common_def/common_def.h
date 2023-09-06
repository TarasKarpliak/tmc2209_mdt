/*
 * common_def.h
 *
 *  Created on: Sep 3, 2023
 *      Author: taras.karpliak
 */

#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include "stdint.h"
#include "stm32f7xx.h"

typedef uint8_t bool_t;

#ifndef NULL
#define NULL  ((*void)0u)
#endif

#ifndef TRUE
#define TRUE  ((uint8_t)1u)
#endif

#ifndef FALSE
#define FALSE ((uint8_t)0u)
#endif


#ifndef SETBIT
#define SETBIT(REG, BIT_NUM)       ((REG) = ((REG) | (1u << (BIT_NUM))))
#endif

#ifndef RESETBIT
#define RESETBIT(REG, BIT_NUM)     ((REG) = ((REG) & ~(1u << (BIT_NUM))))
#endif

#ifndef CHECKBIT
#define CHECKBIT(REG, BIT_NUM)     (((REG) & (1u << (BIT_NUM))) != 0u)
#endif

#endif // COMMON_DEF_H
