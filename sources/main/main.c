/*
 * main.c
 *
 *  Created on: Sep 4, 2023
 *      Author: Taras Karpliak
 */

#include "main_private.h"


//-------------------------------------------------------------------
int main(void)
{
//	CHIP_Init();

	while(1u)
	{
//		CHIP_SetIndicationPin();
//		MAIN_Delay();
//
//		CHIP_ResetIndicationPin();
//		MAIN_Delay();
	}

	return 0u;
}


//-------------------------------------------------------------------
static void MAIN_Delay(void)
{
	uint32_t timer = 16000000;

	while(timer--);
}
