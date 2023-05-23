#include "StdTypes.h"
#include "ADC_interface.h"
#include "Joystick.h"
#define _TAN_ARRAYS
#include "Joystick_Private.h"
# define F_CPU 8000000
#include <util/delay.h>

static s16 X_axis;
static s16 Y_axis;
static s16 X_axis_Prev;
static s16 Y_axis_Prev;
static u16 Vx;
static u16 Vy;
static u16 Ceta;
static u16 Angle;
static u32 TanInv;
static u8 Index ;


void Joystick_Init(void)
{
	/*Do Nothing*/ 
}




void Joystick_Runnable(void)
{
	Vx = ADC_ReadVolt(VX_CHANNEL);
	Vy = ADC_ReadVolt(VY_CHANNEL);
	X_axis = Vx - 2495 - X_AXIS_CORRECTION;
	Y_axis = Vy - 2495 - Y_AXIS_CORRECTION;
	
	DetectDeadZone();

}

static void DetectDeadZone(void)
{
	bool_t AngleFlag = TRUE;
	
	if	(X_axis > -DEAD_ZONE_OFFSET && X_axis < DEAD_ZONE_OFFSET)
	{
		X_axis = 0;
		if ((Y_axis > -DEAD_ZONE_OFFSET && Y_axis < DEAD_ZONE_OFFSET))
		{
			// Do Nothing
			AngleFlag = FALSE;
			
		}
		
	}
	else if ((Y_axis > -DEAD_ZONE_OFFSET && Y_axis < DEAD_ZONE_OFFSET))
	{
		Y_axis = 0;
	}
	
	
	// Filter Wrong reads
	if ((X_axis != X_axis_Prev) && (Y_axis != Y_axis_Prev))
	{
		AngleFlag = FALSE;
	}
	
	// If axis's not in Dead Zone Calculate angle
	if (AngleFlag)
	{
		Calc_Angle();
	}
	
	Y_axis_Prev = Y_axis;
	X_axis_Prev = X_axis;
	_delay_ms(DELAY_ANGLE);
}


static void Calc_Angle(void)
{
	if (X_axis == 0)
	{
		if (Y_axis > 0)
		{
			Angle = 900;
			Ceta = 0;
		}
		else if (Y_axis < 0)
		{
			Angle = 2700;
			Ceta = 0;
		}
	}
	else
	{
		TanInv = abs_num(((s32)Y_axis * AMP_FACTOR) / (s32)X_axis);
		Search_ceta(TanInv);
		
		
		if (X_axis > 0)
		{
			if (Y_axis >= 0)
			{
				Angle = Ceta;
				
			}
			else if (Y_axis < 0)
			{
				Angle = 3600 - Ceta;
			}
		}
		
		else if (X_axis < 0)
		{
			if (Y_axis > 0)
			{
				Angle = 1800 - Ceta;
				
			}
			else if (Y_axis < 0)
			{
				Angle = 2700 - Ceta;
			}
			else if (Y_axis == 0)
			{
				Angle = 1800;
			}
			
		}
	}
		
}

u16 Joystick_GetAngle(void)
{
	return Angle;
}

static u32 abs_num(s32 num)
{
	if (num < 0)
	{
		num = num * (-1);
	}
	return num;
}




static void Search_ceta(u32 num)
{
	u16 cmp[3];
	u8 smallest;
	for (u8 i = 0; i < 180; i++)
	{
		if (TanArray[i] == num)
		{
			Ceta = AngleArray[i];
			break;
		}
		else if ((TanArray[i] > num) && i != 0)
		{
			cmp[0] = TanArray[i] - num;
			cmp[1] = num - TanArray[i - 1];
			cmp[2] = abs_num(AVR2NUM(TanArray[i], TanArray[i - 1]) - num);
			smallest = SmallestOfThree(cmp[0], cmp[1], cmp[2]);
			Index = smallest;
			if (smallest == 2)
			{
				Ceta = AVR2NUM(AngleArray[i] , AngleArray[i - 1]);
			}
			else if (smallest == 1)
			{
				Ceta = AngleArray[i];
			}
			else
			{
				Ceta = AngleArray[i];
			}
			break;
		}
	}
}





static u8 SmallestOfThree(u16 n1, u16 n2, u16 n3)
{
	if (n1 <= n2)
	{
		if(n1 <= n3)
		{
			return 0;
		}
	}
	else if (n2 <= n1)
	{
		if(n2 <= n3)
		{
			return 1;
		}
	}
	
	return 2;
	
}




#include "LCD_interface.h"

void Joystick_Display(void)
{

	
	LCD_SetCursor(1,0);
	LCD_WriteNumber_S4D(X_axis);
	
	LCD_SetCursor(1,7);
	LCD_WriteNumber_S4D(Y_axis);
	

	LCD_SetCursor(0, 6);
	LCD_WriteNumber_4D(Ceta);
	

	LCD_SetCursor(0,0);
	LCD_WriteNumber_4D(Angle);
}
