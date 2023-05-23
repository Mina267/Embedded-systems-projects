#define F_CPU 8000000
#include <util/delay.h>

#include "StdTypes.h"
#include "DIO_Interface.h"
#include "Segment.h"


#if SEGMENT_TYPE == MLX


// Function has total delay 10ms
void Segment_DisplayMLX(int num)
{
	unsigned char arr[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	
	
	DIO_WritePin(DEG1_CATH, HIGH);	// Digit 1 on
	DIO_WritePin(DEG2_CATH, LOW);	// Digit 2 off
	DIO_WritePort(SEG_PORT, arr[num % 10] << 1);
	_delay_ms(5);
	
	DIO_WritePin(DEG1_CATH, LOW);	// Digit 1 on
	DIO_WritePin(DEG2_CATH, HIGH);	// Digit 2 off
	DIO_WritePort(SEG_PORT, arr[num / 10] << 1);
	_delay_ms(5);
	
}




#elif SEGMENT_TYPE == BCD

void Segment_DisplayBCD(int num)
{
	DIO_WritePort(SEG_PORT, (num % 10) | ((num / 10) << 4));
}

void segment_disWith2BCD_Hex(u8 n)
{
	DIO_WritePort(SEG_PORT,  n);
}

#else 

void segment_dis(u8 n)
{
	u8 arr[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	DIO_WritePort(SEG_PORT, arr[n]);
}


#endif


void Segment_Stop(void)
{
	DIO_WritePin(DEG1_CATH, LOW);	// turn off digit 1
	DIO_WritePin(DEG2_CATH, LOW);	// turn off digit 2

}


