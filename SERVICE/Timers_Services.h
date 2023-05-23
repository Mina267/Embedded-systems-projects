
#ifndef TIMERS_SERVICES_H_
#define TIMERS_SERVICES_H_







typedef enum
{
	FREQ_OUTPUT1,
	FREQ_OUTPUT2,
	TOTAL_OUTPUTS
	}PWM_Freq_Output;

#ifdef _PWM_OUTOUT_ARRAY
const volatile unsigned short *PWM_FreqOutput_Array[TOTAL_OUTPUTS] =
{
	&OCR1A, // Output 1
	&OCR1B	// Output 2
};	
#endif




void PWM_Init(void);
void PWM_Freq_KHZ(u16 freq);
void PWM_Freq_HZ(u16 freq);
void PWM_Duty(u16 duty, PWM_Freq_Output Output);

void Timer1_SetInterruptTime_ms (u16 time,void(*LocalFptr)(void));
void Timer1_SetInterruptTime_us (u16 time,void(*LocalFptr)(void));
void Timer1_SetInterruptTime_s (u16 time,void(*LocalFptr)(void));
void PWM_Measure(u32* Pfreq,u8* Pduty);
void PWM_Measure2(u32* Pfreq,u8* Pduty);






#endif /* TIMERS_SERVICES_H_ */