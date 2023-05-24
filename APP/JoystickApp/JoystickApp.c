#include "StdTypes.h"
#include "Joystick.h"
#include "Stepper_interface.h"
#include "Servo_Interface.h"

void JoystickAPP_Init(void)
{
	Servo_Init();
	Stepper_Init();
	Joystick_Init();
}


void JoystickAPP_ServoRunnable(void)
{
	Joystick_Runnable();
	
	u16 Angle = Joystick_GetAngle();
	
	if (Angle <= 1800)
	{
		Servo_Setposition(Angle);
	}
	
}

void JoystickAPP_StepperRunnable(void)
{
	Joystick_Runnable();
	Stepper_AngleRunnable();
	
	u16 Angle = Joystick_GetAngle();
	
	Stepper_AngleSetter(Angle / 10);
	

}