

#define _PWM_OUTOUT_ARRAY 1

#include "StdTypes.h"
#include "MemMap.h"
#include "Timers.h"
#include "Timers_Services.h"
#include "DIO_interface.h"

#include "LCD_interface.h"

static volatile u16 t1,t2,t3,flag=0;



/**********************************************PWM*********************************************************/
void PWM_Init(void)
{
	Timer1_Init(TIMER1_FASTPWM_ICR_TOP_MODE, TIMER1_SCALER_8);
	Timer1_OCRA1Mode(OCRA_NON_INVERTING);
	Timer1_OCRB1Mode(OCRA_NON_INVERTING);
	
}
void PWM_Freq_KHZ(u16 freq)
{
	if (freq != 0)
	{
		u16 Ttotal = 1000 / freq;
		if (Ttotal > 1)
		{
			ICR1 = Ttotal - 1;
		}
		else
		{
			ICR1 = 0;
		}
	}
}
void PWM_Freq_HZ(u16 freq)
{
	if (freq != 0)
	{
		u16 Ttotal = (u16)((u32)1000000 / (u32)freq);
		if (Ttotal > 1)
		{
			ICR1 = Ttotal - 1;
		}
		else
		{
			ICR1 = 0;
		}
	}
}
void PWM_Duty(u16 duty, PWM_Freq_Output Output)
{
	// Select Duty Change from what pin OCR1A or OCR1B
	volatile u8 *Output_Reg = (volatile u8 *) PWM_FreqOutput_Array[Output];
	if (duty <= 100)
	{
		u8 ton = (u8)(((u32)duty * (u32)(ICR1 + 1)) / (u32)100);
		if (ton > 1)
		{
			*Output_Reg = ton - 1;
		}
		else
		{
			*Output_Reg = 0;
		}
	}
}
/******************************************Set Interrupt Time******************************************************/
/** 0 to 65 **/
static void Timer_func(void);
static u32 NofOverFlow=0;
static void(*fptr_timer)(void);

void Timer1_SetInterruptTime_ms (u16 time,void(*LocalFptr)(void))
{
	Timer1_Init(TIMER1_CTC_OCRA_TOP_MODE, TIMER1_SCALER_8);

	OCR1A=(time*1000)-1;
	Timer1_OCA_SetCallBack(LocalFptr);
	Timer1_OCA_InterruptEnable();	
}
void Timer1_SetInterruptTime_us (u16 time,void(*LocalFptr)(void))
{
	Timer1_Init(TIMER1_CTC_OCRA_TOP_MODE, TIMER1_SCALER_8);
	OCR1A=(time)-1;
	Timer1_OCA_SetCallBack(LocalFptr);
	Timer1_OCA_InterruptEnable();
}


void Timer1_SetInterruptTime_s (u16 time, void(*LocalFptr)(void))
{
	Timer1_Init(TIMER1_CTC_OCRA_TOP_MODE,TIMER1_SCALER_8);
	OCR1A = 999;
	NofOverFlow = time * 1000;
	fptr_timer = LocalFptr;
	Timer1_OCA_SetCallBack(Timer_func);
	Timer1_OCA_InterruptEnable();
}

static void Timer_func(void)
{
	static u32 c = 0;
	c++;
	if (c == NofOverFlow)
	{
		c = 0;
		fptr_timer();
	}
}


/**********************************************Measure PWM*********************************************************/

// Busy w8
void PWM_Measure2(u32* Pfreq,u8* Pduty)
{
	u16 Ton,Toff;
	flag=0;
	TCNT1=0;
	while (DIO_ReadPin(PIND6));
	while (!DIO_ReadPin(PIND6));
	TCNT1=0;
	while (DIO_ReadPin(PIND6));
	Ton=TCNT1;
	TCNT1=0;
	while (!DIO_ReadPin(PIND6));
	Toff=TCNT1;
	*Pduty=((u32)Ton*100)/(Ton+Toff);
	*Pfreq=(u32)1000000/((u32)Toff+Ton);
}

static void Func_ICU(void);
// Busy w8
void PWM_Measure(u32* Pfreq,u8* Pduty)
{
	u16 Ton,Toff;
	
	TCNT1=0;
	Timer1_ICU_SetCallBack(Func_ICU);
	Timer1_InputCaptureEdge(RISING);
	Timer1_ICU_InterruptEnable();
	flag=0;
	while(flag<3);//busy w8
	Ton=t2-t1;
	Toff=t3-t2;

	*Pduty=((u32)Ton*100)/((u32)Ton+Toff);
	*Pfreq=(u32)1000000/((u32)Toff+Ton);

}
static void Func_ICU(void)
{
	if (flag==0)
	{
		t1=ICR1;
		Timer1_InputCaptureEdge(FALLING);
		flag=1;
	}
	else if (flag==1)
	{
		t2=ICR1;
		Timer1_InputCaptureEdge(RISING);
		flag=2;
	}
	else if (flag==2)
	{
		t3=ICR1;
		Timer1_ICU_InterruptDisable();
		flag=3;
		
	}

}





/*
ISR(TIMER0_OVF_vect)
{
	static u8 flag=0;
	if (flag==0)
	{
		DIO_WritePin(PINC0,HIGH);
		flag=1;
		TCNT0=196;
	}
	else if (flag==1)
	{
		DIO_WritePin(PINC0,LOW);
		flag=0;
		TCNT0=116;
	}
}*/
/*
ISR(TIMER0_OVF_vect)
{
	static u8 c=0;
	c++;
	TCNT0=236;
	if (c==7)
	{
		DIO_WritePin(PINC0,HIGH);
	}
	else if (c==10)
	{
		DIO_WritePin(PINC0,LOW);
		c=0;
	}
}*/


/*
ISR(TIMER0_OVF_vect)
{
	static u16 c=0;
	static u16 c2=0;
	c++;
	c2++;
	if (c==3907)
	{
		DIO_TogglePin(PINC0);
		TCNT0=192;
		c=0;
	}
	if (c2==7812)
	{
		DIO_TogglePin(PINC1);
		c2=0;
	}
	
}*/
/*
ISR(TIMER0_OVF_vect)
{
	static u16 c=0;
	TCNT0=6;
	c++;
	if (c%20==0)
	{
		DIO_TogglePin(PINC0);
	}
	if (c%40==0)
	{
		DIO_TogglePin(PINC1);
	}
	if (c%100==0)
	{
		DIO_TogglePin(PINC2);
	}
	if (c==200)
	{
		c=0;
	}
}
*/


/*
ISR(int_5ms)
{
	static u16 c=0;
	TCNT0=6;
	c++;
	
		DIO_TogglePin(PINC0);
	if (c%2==0)
	{
		DIO_TogglePin(PINC1);
	}
	if (c%5==0)
	{
		DIO_TogglePin(PINC2);
	}
	if (c==10)
	{
		c=0;
	}
}*/



/*


static void f1(void)
{
	if (flag==0)
	{
	TCNT1=0;
	Timer1_ICU_SetCallBack(Func_ICU);
	Timer1_InputCaptureEdge(RISING);
	Timer1_ICU_InterruptEnable();

	}
}

static u8 f2(u32* Pfreq,u8* Pduty)
{
	u16 Ton,Toff;
	
	if (flag==3)
	{
	Ton=t2-t1;
	Toff=t3-t2;

	*Pduty=((u32)Ton*100)/((u32)Ton+Toff);
	*Pfreq=(u32)1000000/((u32)Toff+Ton);
	flag=0;
	return 1;
	
	}
	return 0;
}


*/
