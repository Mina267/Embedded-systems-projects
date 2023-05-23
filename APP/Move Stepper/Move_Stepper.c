# define F_CPU 8000000
#include <util/delay.h>


#include "StdTypes.h"
#include "Stepper_interface.h"
#include "LCD_interface.h"
#include "Keypad_interface.h"
#include "Move_Stepper.h"
#include "Buzz_interface.h"
#include "Move_Stepper_private.h"



static s16 num;
static bool_t NumDone_Flag = FALSE;
static bool_t NewOperation_Flag = TRUE;
static bool_t Print_flag = TRUE;
static MoveStepper_choice_type choice = MOVE_NO_CHOICE;





void Move_Stepper_Init(void)
{
	Stepper_Init();
	Welcome();
}


void Move_Stepper_Runnable(void)
{

	if (NewOperation_Flag)
	{
		LCD_Clear();
		BuzzSound();
		menu();
		MoveStepper_Display();
		NewOperation_Flag = FALSE;
	}
	else
	{
		Take_SignedDecNum();
	}
	
	
	if (choice == MOVE_THIRDCHOICE)
	{
		u8 r = Stepper_AngleRunnable();
		if (r == 1)
		{
			if (Print_flag)
			{
				LCD_SetCursor(SECOND_LINE, FIRST_CELL);
				LCD_WriteString("MOVING! ");
				Print_flag = FALSE;
			}
		}
		else
		{
			Print_flag = TRUE;
			LCD_ClearCell(SECOND_LINE, FIRST_CELL, 7);
		}
	}
	
	
}


static void Welcome(void)
{
	_delay_ms(500);
	LCD_SetCursor(FIRST_LINE, SECOND_CELL);
	LCD_WriteString("  Welcome To");
	_delay_ms(800);
	LCD_SetCursor(SECOND_LINE, FIRST_CELL);
	LCD_WriteString("Stepper  Control");
	_delay_ms(2000);
	LCD_Clear();
	
}


static void menu(void)
{
	_delay_ms(500);
	LCD_SetCursor(0,1);
	LCD_WriteString("Stepper Select ");
	_delay_ms(1500);
	LCD_Clear();
	LCD_SetCursor(FIRST_LINE, FIRST_CELL);
	LCD_WriteString("1-ByAngle");
	LCD_SetCursor(FIRST_LINE, 11);
	LCD_WriteString("3-Pos");
	LCD_SetCursor(SECOND_LINE ,FIRST_CELL);
	LCD_WriteString("2-ByRotate");

	takeChoice(3);
	LCD_Clear();
}





static void takeChoice(u8 numOFchoice)
{
	u8 key = NO_KEY;
	numOFchoice += '0';

	
	while (choice == MOVE_NO_CHOICE)
	{
		
		key = KEYPAD_GetKey();
		
		if ((key >= '1' && key <= numOFchoice))
		{
			switch(key)
			{
				case '1':
				choice = MOVE_FIRSTCHOICE;
				break;
				case '2':
				choice = MOVE_SECONDCHOICE;
				break;
				case '3':
				choice = MOVE_THIRDCHOICE;
				break;
			}
			BuzzSound();
		}
		else if (key != NO_KEY)
		{
			LCD_SetCursor(SECOND_LINE, 10);
			LCD_WriteString(" wrong");
			BuzzSound();
			_delay_ms(50);
			BuzzSound();
			_delay_ms(100);
			LCD_ClearCell(SECOND_LINE, 10, 6);
		}
		
	}
	
}



static void MoveStepper_Display(void)
{
	
	if (choice == MOVE_FIRSTCHOICE) // By Angle
	{
		LCD_WriteString("Angle:  ");
		Take_SignedDecNum();
	}
	else if (choice == MOVE_SECONDCHOICE)
	{
		LCD_WriteString("Rotate: ");
		Take_SignedDecNum();
	}
	else if (choice == MOVE_THIRDCHOICE)
	{
		LCD_WriteString("Angle: ");
		Take_SignedDecNum();
	}
}

static void MoveStepper_Action(void)
{
	if (choice == MOVE_FIRSTCHOICE) // By Angle
	{
		if (num >= 0)
		{
			LCD_SetCursor(SECOND_LINE, FIRST_CELL);
			LCD_WriteString("MOVING! ");
			LCD_WriteNumber(num);
			Stepper_ByAngle((u16)num, CW);
			
		}
		else
		{
			LCD_SetCursor(SECOND_LINE, FIRST_CELL);
			LCD_WriteString("MOVING! ");
			LCD_WriteNumber(num);
			num = num * (-1);
			Stepper_ByAngle((u16)num, CCW);
		}
		Stepper_PosMatch();
	}
	else if (choice == MOVE_SECONDCHOICE)
	{
		if (num >= 0)
		{
			LCD_SetCursor(SECOND_LINE, FIRST_CELL);
			LCD_WriteString("MOVING! ");
			LCD_WriteNumber(num);
			Stepper_ByRotate((u16)num, CW);
		}
		else
		{
			LCD_SetCursor(SECOND_LINE, FIRST_CELL);
			LCD_WriteString("MOVING! ");
			LCD_WriteNumber(num);
			num = num * (-1);
			Stepper_ByRotate((u16)num, CCW);
		}
		Stepper_PosMatch();
	}
	
	else if (choice == MOVE_THIRDCHOICE)
	{
		if (num >= 0)
		{
			Stepper_AngleSetter(num);
		}
		else
		{
			num = num + (360);
			Stepper_AngleSetter(num);
		}
	}
}





static void Take_SignedDecNum(void)
{
	u8 key = NO_KEY;
	u8 startCell = CELL_8_;
	static u8 cell = CELL_8_;
	static bool_t Sign_flag = FALSE;
	LCD_SetCursor(FIRST_LINE, cell);

	key = KEYPAD_GetKey();
	
	if ((key >= '0' && key <= '9'))
	{
		BuzzSound();
		LCD_WriteChar(key);
		num = num * 10 + (key - '0');
		cell++;
	}
	else if (( key == MINUS) && num == 0)
	{
		BuzzSound();
		LCD_WriteChar(key);
		Sign_flag = TRUE;
		cell++;
	}
	if (key == DELETE && startCell != cell)
	{
		BuzzSound();
		num = num / 10;
		LCD_ClearCell(FIRST_LINE, cell - 1, 1);
		LCD_SetCursor(FIRST_LINE, cell - 1);
		cell--;
		if (cell == startCell)
		{
			Sign_flag = FALSE;
		}
	}
	
	
	if (key == INPUTDONE)
	{
		BuzzSound();
		if (Sign_flag)
		{
			num = num * (-1);
			NumDone_Flag = TRUE;
			
		}
		MoveStepper_Action();
		num = 0;
		Sign_flag = FALSE;
		cell = CELL_8_;
		LCD_ClearCell(FIRST_LINE, CELL_8_, 10);
		LCD_ClearCell(SECOND_LINE, FIRST_CELL, CELL_OUT_LCD);
		
	}
	if (key == START_AGAIN)
	{
		BuzzSound();
		num = 0;
		Sign_flag = FALSE;
		NewOperation_Flag = TRUE;
		cell = CELL_8_;
		choice = MOVE_NO_CHOICE;
		LCD_Clear();
	}
	
}


