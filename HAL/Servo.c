#include "StdTypes.h"
#include "Timers.h"
#include "MemMap.h"

void Servo_Init(void)
{
	Timer1_Init(TIMER1_FASTPWM_OCRA_TOP_MODE, TIMER1_SCALER_8);
	Timer1_OCRB1Mode(OCRB_NON_INVERTING);
	// freq 50 Hz
	OCR1A = 19999;
}


void Servo_Setposition(u16 angle)
{
	OCR1B = ((angle * (u32)2000) / 1800) + 499;
}

void Servo_Setposition2(u16 angle)
{
	OCR1B = ((angle * (u32)1000) / 1800) + 999;
}