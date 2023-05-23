# define F_CPU 8000000
#include <util/delay.h>

#include "StdTypes.h"
#include "Keypad_interface.h"
#include "LCD_interface.h"
#include "Converter.h"
#include "Converter_private.h"
#include "Converter_Cfg.h"
#include "Buzz_interface.h"


static Converter_choice_type choice1;
static Converter_choice_type choice2 = FIRST;


void Converter_Init(void)
{
	Welcome();
}

void Converter_Runnable(void)
{
	LCD_Clear();
	BuzzSound();
	menu();
	convertionDisplay();
}


static void Welcome(void)
{
	_delay_ms(500);
	LCD_SetCursor(FIRST_LINE, 2);
	LCD_WriteString("Welcome To");
	_delay_ms(800);
	LCD_SetCursor(SECOND_LINE, FIRST_CELL);
	LCD_WriteString(" Mina Converter");
	_delay_ms(WELCOME_DELAY);
	LCD_Clear();
	
}


static void menu(void)
{
	_delay_ms(500);
	LCD_SetCursor(FIRST_LINE, 2);
	LCD_WriteString("Convert from:");
	LCD_SetCursor(SECOND_LINE, FIRST_CELL);
	_delay_ms(1500);
	LCD_Clear();
	LCD_SetCursor(0,0);
	LCD_WriteString("1-HEX  2-Binary");
	LCD_SetCursor(SECOND_LINE, FIRST_CELL);
	LCD_WriteString("3-Decimal");
	choice1 = takeChoice(_1ST_NUMBER_OF_CHOICE);
	LCD_Clear();
	
	_delay_ms(500);
	LCD_WriteString("Convert to:");
	LCD_SetCursor(SECOND_LINE, FIRST_CELL);
	_delay_ms(1500);
	LCD_Clear();
	LCD_SetCursor(FIRST_LINE, FIRST_CELL);

	
	if (choice1 == FIRST)
	{
		LCD_WriteString("1-Decimal  2-Binary");
		choice2 = takeChoice(_2ST_NUMBER_OF_CHOICE);
		LCD_Clear();

	}
	else if (choice1 == SECOND)
	{
		LCD_WriteString("1-Decimal  2-HEX");
		choice2 = takeChoice(_2ST_NUMBER_OF_CHOICE);
		LCD_Clear();

	}
	else if (choice1 == THIRD)
	{
		LCD_WriteString("1-HEX  2-Binary");
		choice2 = takeChoice(_2ST_NUMBER_OF_CHOICE);
		LCD_Clear();

	}
	

	

}

static void convertionDisplay(void)
{
	
	
	if (choice1 == FIRST)
	{
		LCD_WriteString("HEX: 0x");
		TakeHEX(FIRST_LINE, 7);
	}
	else if (choice1 == SECOND)
	{

		LCD_WriteString("BIN: ");
		TakeBin(FIRST_LINE, 5);
		
	}
	else if (choice1 == THIRD)
	{

		LCD_WriteString("DEC: ");
		TakeDec(FIRST_LINE, 5);
	}
}





static Converter_choice_type takeChoice(u8 numOFchoice)
{
	u8 key = NO_KEY;
	numOFchoice += '0'; 
	Converter_choice_type choice = NO_CHOICE;
	
	while (choice == NO_CHOICE)
	{
		
		key = KEYPAD_GetKey();
		
		if ((key >= '1' && key <= numOFchoice))
		{
			switch(key)
			{
				case '1':
				choice = FIRST;
				break;
				case '2':
				choice = SECOND;
				break;
				case '3':
				choice = THIRD;
				break;
			}
			BuzzSound();
		}
		else if (key != NO_KEY)
		{
			LCD_SetCursor(SECOND_LINE, 10);
			LCD_WriteString("wrong");
			BuzzSound();
			_delay_ms(50);
			BuzzSound();
			_delay_ms(100);
			LCD_ClearCell(1, 10, 5);
		}
		
	}
	return choice;
	
}






static u32 TakeDec(u8 line, u8 cell)
{
	u8 key = KEYPAD_GetKey();
	u32 num = 0;
	u8 startCell = cell;
	
	while(key != START_AGAIN)
	{
		LCD_SetCursor(line, cell);
		if ((key >= '0' && key <= '9')  && (cell != CELL_OUT_LCD))
		{
			BuzzSound();
			LCD_WriteChar(key);
			num = num * 10 + (key - '0');
			cell++;
		}
		if ((key == DELETE) && (startCell != cell))
		{
			BuzzSound();
			num = num / 10;
			LCD_ClearCell(line, cell - 1, 1);
			LCD_SetCursor(line, cell - 1);
			cell--;
			LCD_ClearCell(SECOND_LINE,5, 11);
		}
		else if (key == NEW_CONVERTION)
		{
			BuzzSound();
			num = 0;
			LCD_ClearCell(line, startCell, 10);
			LCD_SetCursor(line, startCell);
			cell = startCell;
			LCD_ClearCell(SECOND_LINE,5, 11);
		}
		key = KEYPAD_GetKey();
		display_num(num);
	}
	
	return num;
}






static u32 TakeHEX(u8 line, u8 cell)
{
	u8 key = NO_KEY;
	u32 num = 0;
	u8 hex = 0;
	u8 startCell = cell;
	
	
	while(key != START_AGAIN)
	{
		LCD_SetCursor(line, cell);
		if (((key >= '0' && key <= '9') && cell < (MAX_8BYTES + startCell))  && (cell != CELL_OUT_LCD))
		{
			BuzzSound();
			LCD_WriteChar(key);
			hex = key - '0';
			WRITEHEX(num, hex); 
			cell++;
		}
		else if ((key == SELECT_HEX_KEY) && cell < (MAX_8BYTES + startCell)  && (cell != CELL_OUT_LCD))
		{
			BuzzSound();
			key = Select_hex(line, cell);
			hex = key - 'A' + 10;
			WRITEHEX(num, hex);
			cell++;
		}
		if (key == DELETE && startCell != cell)
		{
			BuzzSound();
			num = num >> 4;
			LCD_ClearCell(line, cell - 1, 1);
			LCD_SetCursor(line, cell - 1);
			cell--;
			LCD_ClearCell(SECOND_LINE,5, 11);
		}
		else if (key == NEW_CONVERTION)
		{
			BuzzSound();
			num = 0;
			LCD_ClearCell(line, startCell, 10);
			LCD_SetCursor(line, startCell);
			cell = startCell;
			LCD_ClearCell(SECOND_LINE,5, 11);
		}
		
		key = KEYPAD_GetKey();
		display_num(num);
	}
	return num;
}




static u8 Select_hex(u8 line, u8 cell)
{
	u8 key = NO_KEY;
	u8 hex = 'A';
	
	LCD_WriteChar(hex);
	
	while(key != DONE)
	{
		LCD_SetCursor(line, cell);
		if (key == SELECT_HEX_KEY)
		{
			BuzzSound();
			hex = (hex + 1 );
			if (hex == 'G')
			{
				hex = 'A';
			}
			LCD_SetCursor(line, cell);
			LCD_WriteChar(hex);	
		}
		key = KEYPAD_GetKey();
		
	}
	BuzzSound();
	return hex;
}



static u32 TakeBin(u8 line, u8 cell)
{
	u8 key = NO_KEY;
	u32 num = 0;
	u8 bin = 0;
	u8 startCell = cell;
	
	while(key != START_AGAIN)
	{
		LCD_SetCursor(line, cell);
		if ((key >= '0' && key <= '1')  && (cell != 13))
		{
			BuzzSound();
			LCD_WriteChar(key);
			bin = key - '0';
			WRITEBIN_LAST(num, bin);
			cell++;
		}
		if (key == DELETE && startCell != cell)
		{
			BuzzSound();
			num = num >> 1;
			LCD_ClearCell(line, cell - 1, 1);
			LCD_SetCursor(line, cell - 1);
			cell--;
			LCD_ClearCell(SECOND_LINE,5, 11);
		}
		else if (key == NEW_CONVERTION)
		{
			BuzzSound();
			num = 0;
			LCD_ClearCell(line, startCell, 10);
			LCD_SetCursor(line, startCell);
			cell = startCell;
			LCD_ClearCell(SECOND_LINE,5, 11);
		}
		display_num(num);
		key = KEYPAD_GetKey();
	}
	return num;
}



static void display_num(u32 num)
{
	LCD_SetCursor(SECOND_LINE, FIRST_CELL);
	if (choice1 == FIRST)
	{
		if (choice2 == FIRST)
		{
			LCD_WriteString("DEC: ");
			LCD_WriteNumber(num);
		}
		else if (choice2 == SECOND)
		{
			LCD_WriteString("BIN: ");
			LCD_WriteBinary(num);
		}
	}
	else if (choice1 == SECOND)
	{	
		if (choice2 == FIRST)
		{
			LCD_WriteString("DEC: ");
			LCD_WriteNumber(num);
			
		}
		else if (choice2 == SECOND)
		{
			LCD_WriteString("HEX: ");
			LCD_WriteHexU32(num);
		}
	}
	else if (choice1 == THIRD)
	{	
		if (choice2 == FIRST)
		{
			LCD_WriteString("HEX: ");
			LCD_WriteHexU32(num);
			
		}
		else if (choice2 == SECOND)
		{
			LCD_WriteString("BIN: ");
			LCD_WriteBinary(num);
		}
	}
	
}





