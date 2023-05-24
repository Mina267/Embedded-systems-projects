# define F_CPU 8000000
#include <util/delay.h>

#include "StdTypes.h"
#include "DIO_interface.h"
#include "Stepper_interface.h"
#include "EEPROM_intrface.h"

static u16 Pos_Cnt = 0;


#define POS_CNT_ADDRESS_1ST_	0x70
#define POS_CNT_ADDRESS_2ND_	0x71

void Stepper_Init(void)
{
	u8 b1 = EEPROM_readBusy(POS_CNT_ADDRESS_1ST_);
	u8 b2 = EEPROM_readBusy(POS_CNT_ADDRESS_2ND_);
	Pos_Cnt = b1 | ((u16) b2 << 8);
	
	

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


#include "LCD_Interface.h"

void Stepper_ByAngle(u16 angle, Stepper_Dir_type dir)
{
	
	u16 steps = (u16)(((u32)angle * (u32)STEP_PERONE_ROT) / ((u32) 360));
	switch(dir)
	{
		case CW:
		for (u16 stepIndex = 0; stepIndex < steps; stepIndex++)
		{
			Stepper_Unipolar_CW();
		}
		Pos_Cnt = (Pos_Cnt + steps) % STEP_PERONE_ROT;
		break;
		
		case CCW:
		for (u16 stepIndex = 0; stepIndex < steps; stepIndex++)
		{
			Stepper_Unipolar_CCW();
		}
		
		Pos_Cnt = (Pos_Cnt - steps) + STEP_PERONE_ROT;
		break;
	}
	
	EEPROM_writeBusy(POS_CNT_ADDRESS_1ST_, (u8)Pos_Cnt);
	EEPROM_writeBusy(POS_CNT_ADDRESS_2ND_, (u8)(Pos_Cnt >> 8));
}


static u16 Target_Pos;
static Stepper_Dir_type Rot_dir;
static bool_t StartPos_flag = FALSE;

typedef struct
{
	s16 Len;
	Stepper_Dir_type dir;
	}Pos_dir_t;

s16 AbsNum(s16 n)
{
	if (n < 0)
	{
		n = n * (-1);
	}
	return n;
}

void Stepper_AngleSetter(u16 angle)
{
	u16 Pos;
	
	Pos = 512 - ((u16)(((u32)angle * (u32)STEP_PERONE_ROT) / ((u32) 360)));
	
	if (Pos == 512)
	{
		Pos = 0;
	}
	
	static Stepper_Dir_type Rot;
	Pos_dir_t  Dir_1;
	Pos_dir_t  Dir_2;
	
	Dir_1.Len = Pos - Pos_Cnt;
	
	if (Dir_1.Len < 0)
	{
		Dir_1.Len = Dir_1.Len * (-1);
		Dir_1.dir = CCW;
	}
	else if (Dir_1.Len >= 0)
	{
		Dir_1.dir = CW;
	}
	
	Dir_2.Len = AbsNum(Dir_1.Len - 512);
	
	if (Dir_1.dir == CW)
	{
		Dir_2.dir = CCW;
	}
	else if (Dir_1.dir == CCW)
	{
		Dir_2.dir = CW;
	}
	
	
	if ((Dir_1.Len < Dir_2.Len) && (Target_Pos != Pos))
	{
		Rot = Dir_1.dir;
	}
	else if ((Dir_1.Len > Dir_2.Len) && (Target_Pos != Pos))
	{
		Rot = Dir_2.dir;
	}
	else if ((Target_Pos != Pos))
	{
		Rot = CW;
	}
	Rot_dir = Rot;
	
	
	
	Target_Pos = Pos;
	StartPos_flag = TRUE;
	
	
	
}


u8 Stepper_AngleRunnable(void)
{
	if ((Target_Pos != Pos_Cnt) && (StartPos_flag))
	{
		switch(Rot_dir)
		{
			case CW:
			Stepper_Unipolar_CW();
			Pos_Cnt = (Pos_Cnt + 1) % STEP_PERONE_ROT;
			break;
			
			case CCW:
			Stepper_Unipolar_CCW();
			
			if (Pos_Cnt == 0)
			{
				Pos_Cnt = 511;
			}
			else
			{
				Pos_Cnt = (Pos_Cnt - 1);
			}
			break;
		}
		
		EEPROM_writeBusy(POS_CNT_ADDRESS_1ST_, (u8)Pos_Cnt);
		EEPROM_writeBusy(POS_CNT_ADDRESS_2ND_, (u8)(Pos_Cnt >> 8));
		return 1;
	}
	return 0;
}

void Stepper_PosMatch(void)
{
	Target_Pos = Pos_Cnt;
}



void Stepper_ByAngle_HS(u16 angle, Stepper_Dir_type dir)
{
	u16 steps = (u16)(((u32)angle * (u32)STEP_PERONE_ROT) / (u32) 360);
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
		for (u16 stepIndex = 0; stepIndex < STEP_PERONE_ROT * rot; stepIndex++)
		{
			Stepper_Unipolar_CW();
		}
		break;
		
		case CCW:
		for (u16 stepIndex = 0; stepIndex < STEP_PERONE_ROT * rot; stepIndex++)
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
		for (u16 stepIndex = 0; stepIndex < STEP_PERONE_ROT * rot; stepIndex++)
		{
			Stepper_Unipolar_CW_HS();
		}
		break;
		
		case CCW:
		for (u16 stepIndex = 0; stepIndex < STEP_PERONE_ROT * rot; stepIndex++)
		{
			Stepper_Unipolar_CCW_HS();
		}
		break;
	}
}

u16 Position_Getter(void)
{
	return Pos_Cnt;
}

void Position_Setter(u16 Pos)
{
	Pos_Cnt = Pos;
}





