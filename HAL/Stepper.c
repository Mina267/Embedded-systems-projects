# define F_CPU 8000000
#include <util/delay.h>

#include "StdTypes.h"
#include "DIO_interface.h"
#include "Stepper_interface.h"

void Stepper_Init(void)
{
	/* Do nothing */
}
void Stepper_Pipolar_CW(void)
{
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
}
void Stepper_Pipolar_CCW(void)
{
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
}







void Stepper_Unipolar_CCW(void)
{
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
}
void Stepper_Unipolar_CW(void)
{
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
}






void Stepper_Unipolar_CCW_HS(void)
{
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
}



void Stepper_Unipolar_CW_HS(void)
{
	
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, HIGH);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, HIGH);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, LOW);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, HIGH);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	DIO_WritePin(COIL1A, HIGH);
	DIO_WritePin(COIL1B, LOW);
	DIO_WritePin(COIL2A, LOW);
	DIO_WritePin(COIL2B, LOW);
	_delay_ms(DELAY);
	
	
}




void Stepper_ByAngle(u16 angle, Stepper_Dir_type dir)
{
	u16 steps = (u16)(((u32)angle * (u32)STEP_PERON_ROT) / ((u32) 360));
	switch(dir)
	{
		case CW:
		for (u16 stepIndex = 0; stepIndex < steps; stepIndex++)
		{
			Stepper_Unipolar_CW();
		}
		break;
		
		case CCW:
		for (u16 stepIndex = 0; stepIndex < steps; stepIndex++)
		{
			Stepper_Unipolar_CCW();
		}
		break;
	}
}



void Stepper_ByAngle_HS(u16 angle, Stepper_Dir_type dir)
{
	u16 steps = (u16)(((u32)angle * (u32)STEP_PERON_ROT) / (u32) 360);
	switch(dir)
	{
		case CW:
		for (u16 stepIndex = 0; stepIndex < steps; stepIndex++)
		{
			Stepper_Unipolar_CW_HS();
		}
		break;
		
		case CCW:
		for (u16 stepIndex = 0; stepIndex < steps; stepIndex++)
		{
			Stepper_Unipolar_CCW_HS();
		}
		break;
	}
}



void Stepper_ByRotate(u16 rot, Stepper_Dir_type dir)
{
	
	
	switch(dir)
	{
		case CW:
		for (u16 stepIndex = 0; stepIndex < STEP_PERON_ROT * rot; stepIndex++)
		{
			Stepper_Unipolar_CW();
		}
		break;
		
		case CCW:
		for (u16 stepIndex = 0; stepIndex < STEP_PERON_ROT * rot; stepIndex++)
		{
			Stepper_Unipolar_CCW();
		}
		break;
	}
}

void Stepper_ByRotate_HS(u16 rot, Stepper_Dir_type dir)
{
	switch(dir)
	{
		case CW:
		for (u16 stepIndex = 0; stepIndex < STEP_PERON_ROT * rot; stepIndex++)
		{
			Stepper_Unipolar_CW_HS();
		}
		break;
		
		case CCW:
		for (u16 stepIndex = 0; stepIndex < STEP_PERON_ROT * rot; stepIndex++)
		{
			Stepper_Unipolar_CCW_HS();
		}
		break;
	}
}