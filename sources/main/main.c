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
	CHIP_Init();


	while(1u)
	{
		CHIP_SetIndicationPin();
		CHIP_Delay(1000u);

		CHIP_ResetIndicationPin();
		CHIP_Delay(1000u);
	}

	return 0u;
}
