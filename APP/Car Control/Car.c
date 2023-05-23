#include "StdTypes.h"
#include "Butthon_interface.h"
#include "Motors_Interface.h"
#include "Car.h"
#include "Car_private.h"
#include "Car_Cfg.h"

void CAR_Init()
{
	CarStop();
}


void CAR_Runnable(void)
{
	
	while (BUTTON_ReadCont(B_F) == PRESSED)
	{
		MoveForward();
	}
	while (BUTTON_ReadCont(B_B) == PRESSED)
	{
		MoveBackword();
	}
	while (BUTTON_ReadCont(B_R) == PRESSED)
	{
		MoveRight();
	}
	while (BUTTON_ReadCont(B_L) == PRESSED)
	{
		MoveLeft();
	}
	
	CarStop();
}

static void MoveForward(void)
{
	MOTOR_CW(RIGHT_FORWARD_MOTOR);
	MOTOR_CW(LEFT_FORWARD_MOTOR);
	MOTOR_CW(RIGHT_BACKWARD_MOTOR);
	MOTOR_CW(LEFT_BACKWARD_MOTOR);
}


static void MoveBackword(void)
{
	MOTOR_CCW(RIGHT_FORWARD_MOTOR);
	MOTOR_CCW(LEFT_FORWARD_MOTOR);
	MOTOR_CCW(RIGHT_BACKWARD_MOTOR);
	MOTOR_CCW(LEFT_BACKWARD_MOTOR);
}

static void MoveRight(void)
{
	MOTOR_CW(RIGHT_FORWARD_MOTOR);
	MOTOR_CCW(LEFT_FORWARD_MOTOR);
	MOTOR_CW(RIGHT_BACKWARD_MOTOR);
	MOTOR_CCW(LEFT_BACKWARD_MOTOR);
}

static void MoveLeft(void)
{
	MOTOR_CCW(RIGHT_FORWARD_MOTOR);
	MOTOR_CW(LEFT_FORWARD_MOTOR);
	MOTOR_CCW(RIGHT_BACKWARD_MOTOR);
	MOTOR_CW(LEFT_BACKWARD_MOTOR);
}

static void CarStop(void)
{
	MOTOR_Stop(RIGHT_FORWARD_MOTOR);
	MOTOR_Stop(LEFT_FORWARD_MOTOR);
	MOTOR_Stop(RIGHT_BACKWARD_MOTOR);
	MOTOR_Stop(LEFT_BACKWARD_MOTOR);
}