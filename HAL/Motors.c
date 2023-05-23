
#define  MOTOR_PROG 1
#include "StdTypes.h"
#include "DIO_Interface.h"
#include "Motors_Interface.h"
#include "Motor_Cfg.h"
#include "MemMap.h"


void MOTOR_Init(void)
{
	DIO_WritePin(MotorPinsArray[M1][EN], HIGH);
	DIO_WritePin(MotorPinsArray[M2][EN], HIGH);
	DIO_WritePin(MotorPinsArray[M3][EN], HIGH);
	DIO_WritePin(MotorPinsArray[M4][EN], HIGH);
}


void MOTOR_Stop(MOTOR_type motor)
{	
	DIO_WritePin(MotorPinsArray[motor][IN1], LOW);
	DIO_WritePin(MotorPinsArray[motor][IN2], LOW);
}

void MOTOR_ForceStop(MOTOR_type motor)
{
	DIO_WritePin(MotorPinsArray[motor][IN1], HIGH);
	DIO_WritePin(MotorPinsArray[motor][IN2], HIGH);
}



void MOTOR_CW(MOTOR_type motor)
{
	DIO_WritePin(MotorPinsArray[motor][IN1], HIGH);
	DIO_WritePin(MotorPinsArray[motor][IN2], LOW);
}

void MOTOR_CCW(MOTOR_type motor)
{
	DIO_WritePin(MotorPinsArray[motor][IN1], LOW);
	DIO_WritePin(MotorPinsArray[motor][IN2], HIGH);
	
}


/*  speed from 0 to 100  %**/
void MOTOR_Speed(MOTOR_type motor,u8 speed)
{
	switch (motor)
	{
		case M1:
		M1_PWM_PIN  = (speed * 255) / 100;
		break;
		
		case M2:
		M2_PWM_PIN = (speed * 255) / 100;
		break;
		
		case M3:
		M3_PWM_PIN = (speed * 255) / 100;
		break;
		
		case M4:
		M4_PWM_PIN = (speed * 255) / 100;
		break;
		
		default:
		;
	}
	
}


void MOTOR_Speed_dir(MOTOR_type motor,u8 speed , Direction_status status)
{
	switch (motor)
	{
		case M1:
		M1_PWM_PIN  = (speed * 255) / 100;
		break;
		
		case M2:
		M2_PWM_PIN = (speed * 255) / 100;
		break;
		
		case M3:
		M3_PWM_PIN = (speed * 255) / 100;
		break;
		
		case M4:
		M4_PWM_PIN = (speed * 255) / 100;
		break;
		
		default:
		;
	}
	
	
	switch (status)
	{
		case M_CW:
		DIO_WritePin(MotorPinsArray[motor][IN1], HIGH);
		DIO_WritePin(MotorPinsArray[motor][IN2], LOW);
		break;
		case M_CCW:
		DIO_WritePin(MotorPinsArray[motor][IN1], LOW);
		DIO_WritePin(MotorPinsArray[motor][IN2], HIGH);
		break;
		case M_STOP:
		DIO_WritePin(MotorPinsArray[motor][IN1], LOW);
		DIO_WritePin(MotorPinsArray[motor][IN2], LOW);
		break;
	}

}

void MOTOR_dir(MOTOR_type motor, Direction_status status)
{
	switch (status)
	{
		case M_CW:
		DIO_WritePin(MotorPinsArray[motor][IN1], HIGH);
		DIO_WritePin(MotorPinsArray[motor][IN2], LOW);
		break;
		case M_CCW:
		DIO_WritePin(MotorPinsArray[motor][IN1], LOW);
		DIO_WritePin(MotorPinsArray[motor][IN2], HIGH);
		break;
		case M_STOP:
		DIO_WritePin(MotorPinsArray[motor][IN1], LOW);
		DIO_WritePin(MotorPinsArray[motor][IN2], LOW);
		break;
	}
}