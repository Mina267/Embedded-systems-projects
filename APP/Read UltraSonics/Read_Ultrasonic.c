#include "StdTypes.h"
#include "ULTRASONIC.h"
#include "Timers_Services.h"

#define ULTRASONIC_READ_ARRAY 0
#include "Read_Ultrasonic.h"


static u16 Read_Array[NUM_OF_ULTRASONIC];


void Read_Ultrasonic_Init(void)
{
	ULTRASONICInt_Init();

}




void Read_Ultrasonic_Runnable(void)
{
	
	static u8 UltraIndex = FIRST_ULTRA;
	static bool_t TakeNewRead_flag = TRUE;
	ULTRASONIC_Distance_t Dis_status;
	u16 Distance;
	
	if (TakeNewRead_flag)
	{
		ULTRASONIC_StartRead(Ultrasonic_pins[UltraIndex]);
		TakeNewRead_flag = FALSE;
	}
	
	
	Dis_status = ULTRASONIC_DistanceGetterCheck(&Distance);
	
	
	if (Dis_status == ULTRA_NEWDISTANCE)
	{	
		Read_Array[UltraIndex] = Distance;
		UltraIndex = (UltraIndex + 1) % NUM_OF_ULTRASONIC;
		TakeNewRead_flag = TRUE;	
	}
	
	

}





u16 Read_Ultrasonic_Getter(Read_Ultrasonic_t UltraSonic)
{
	return Read_Array[UltraSonic];
}

