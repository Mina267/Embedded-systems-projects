# define F_CPU 8000000
#include <util/delay.h>

#define _KEYSARRAY
#include "StdTypes.h"
#include "DIO_interface.h"
#include "KeyPad_interface.h"



	
void KEYPAD_Init(void)
{
	DIO_WritePin(FIRST_OUTPUT, HIGH);
	DIO_WritePin(FIRST_OUTPUT + 1, HIGH);
	DIO_WritePin(FIRST_OUTPUT + 2, HIGH);
	DIO_WritePin(FIRST_OUTPUT + 3, HIGH);
}
u8 KEYPAD_GetKey(void)
{
	u8 key = NO_KEY;
	for (u8 r = 0; r < ROWS; r++)
	{
		DIO_WritePin(FIRST_OUTPUT + r, LOW);
		for (u8 c = 0; c < COLS; c++)
		{
			if (DIO_ReadPin(FIRST_INPUT + c) == LOW)
			{
				key = KeysArray[r][c];
				while(DIO_ReadPin(FIRST_INPUT + c) == LOW);
			}
		}
		DIO_WritePin(FIRST_OUTPUT + r, HIGH);
	}
	return key;
}


