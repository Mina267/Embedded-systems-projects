# define F_CPU 8000000
#include <util/delay.h>

#include "StdTypes.h"
#include "MemMap.h"
#include "Timers.h"
#include "Read_Ultrasonic.h"
#include "Timers_Services.h"
#include "BCM.h"
#include "UART_Services.h"
#include "MOTOR_interface.h"
#include "Servo_Interface.h"
#include "LCD_interface.h"
#include "LEDS.h"
#include "AutoParking.h"
#include "AutoParking_Private.h"
#include "AutoParking_Cfg.h"





	
static volatile u16 Car_velocity;
/* Finding parking Spot Variables */
static volatile bool_t StartCompDis_Flag = FALSE;
static volatile u16 Distance;
static volatile u16 DistanceToStart_Parking;
static volatile bool_t PLaceFit_FirstFlag = FALSE;
static volatile bool_t PLaceFit_SecondFlag = FALSE;
static volatile bool_t ParkingProtocol_flag = FALSE;
static volatile bool_t SpotCondition_flag = FALSE;
static bool_t ResetToFindingParkingSpot_flag = TRUE;
static u8 Sent_flags[4] = {1, 1, 1, 1};

/* Parking Protocol Variables */
static volatile u8 ParkingSteps_flag = FIRST_STEP;
static volatile bool_t StartStep_flag = TRUE;
static volatile bool_t ParkingDone_Flag = FALSE;
static volatile Direction_status Dir_Motor = M_CCW;
/* Reverse Parking Protocol Variables */
static volatile bool_t WaitingHindrance_flag = FALSE;
static volatile bool_t ReverseParkingProtocol_flag = FALSE;
static volatile u32 HindranceCnt = 0;
static bool_t Handicaps_flag = FALSE;


void AutoParking_Init(void)
{
	Servo_Init();
	Read_Ultrasonic_Init();
	Servo_Setposition2(SERVO_DEFAULTANGLE);
	
	// Calculate Distance
	Timer1_OVF_SetCallBack(Timer1_OV_CallBack_func);
	Timer1_OVF_InterruptEnable();
	
	
	//Speed of Car in cm/sec 
	Car_velocity = START_CAR_VELOCITY;
	
	
	// Motor Start
	TIMER0_Init(TIMER0_PHASECORRECT_MODE, TIMER0_SCALER_1024);
	TIMER0_OC0Mode(OC0_NON_INVERTING);
	MOTOR_Init();
	MOTOR_Speed_dir(CAR_MOTOR, 80, M_CW);
	
	// Parking Steps
	TIMER2_Init(TIMER2_NORMAL_MODE, TIMER2_SCALER_64);
	TIMER2_OV_SetCallBack(OCR_timer2_callBack);
	TIMER2_OV_InterruptEnable();
	
	// Send Message of car status with UART
	BCM_SendStringInit();
	
}






void AutoParking_Runnable(void)
{
	Read_Ultrasonic_Runnable();
	
	
	BCM_SendStringRunnable();
	

	//ParkingProtocol_flag = TRUE;
	if ((ParkingProtocol_flag) && (!ParkingDone_Flag))
	{
		Parking_Protocol();
	}
	else
	{
		if (ResetToFindingParkingSpot_flag)
		{
			Servo_Setposition2(SERVO_DEFAULTANGLE);
			MOTOR_Speed_dir(CAR_MOTOR, 80, M_CW);
			ResetToFindingParkingSpot_flag = FALSE;
			// Reset Variables 
			Distance = 0;
			DistanceToStart_Parking = 0;
			PLaceFit_SecondFlag = FALSE;
			PLaceFit_FirstFlag = FALSE;
			StartCompDis_Flag = FALSE;
			SpotCondition_flag = FALSE;
		}

		FindingParkingSpot_Protocol();
	}
	
	
	DisplayOn_LCD();

	
	
	
	
}


static void FindingParkingSpot_Protocol(void)
{
	// If Distance of Computed large enough to fit car Length.
	if (Distance >= (CAR_LENGTH + LENGTH_TOLERANCE))
	{
		PLaceFit_FirstFlag = TRUE;
	}
	
	// If Side Back UltraSonic Find width for car.
	if (Read_Ultrasonic_Getter(SIDE_BACK_ULTRA) > (CAR_WIDTH + WIDTH_TOLERANCE))
	{
		
		PLaceFit_SecondFlag = TRUE;
	}
	
	// If Side Front UltraSonic Find width for car.
	if (Read_Ultrasonic_Getter(SIDE_FRONT_ULTRA) > (CAR_WIDTH + WIDTH_TOLERANCE))
	{
		
		StartCompDis_Flag = TRUE;
	}
	else
	{
		StartCompDis_Flag = FALSE;
		Distance = 0;
	}
	
	
	// When Distance is Larger than half length start Parking Protocol
	if (DistanceToStart_Parking >= (CAR_LENGTH + LENGTH_TOLERANCE))
	{
		TIMER2_OV_InterruptEnable();
		ParkingProtocol_flag = TRUE;
		
	}
	
	// If Spot Condition Satisfied.
	if (PLaceFit_FirstFlag && PLaceFit_SecondFlag)
	{
		SpotCondition_flag = TRUE;
	}
	
}






// Time of Interrupt is 20 ms
static void Timer1_OV_CallBack_func(void)
{
	// Calculate Distance When Side Front Ultra Find Enough Width
	if (StartCompDis_Flag)
	{
		Distance = Distance + (Car_velocity / 50);
	}
	
	// Calculate Distance needed for car in front to start Parking mode.
	if (SpotCondition_flag)
	{
		DistanceToStart_Parking += (Car_velocity / 50);
	}
	
}





static void Parking_Protocol(void)
{
	if (!ReverseParkingProtocol_flag)
	{
		Detect_Handicaps();
	}
		
	if (StartStep_flag)
	{
		Parking_steps();
		StartStep_flag = FALSE;
	}
	
	Adjust_Car();
	
	
}



static void Parking_steps(void)
{
	
	MOTOR_Speed_dir(CAR_MOTOR, 0 , M_STOP);
	_delay_ms(ENGINE_STOP_DELAY);
	LCD_SetCursor(0, 0);
	if (!ReverseParkingProtocol_flag)
	{
		LCD_WriteString("    Parking       ");
		Sent_flags[3] = 1;	
		BCM_SendStringSetter("Car is Parking. ");
	}
	else
	{
		LCD_WriteString("Reverse Parking");
		BCM_SendStringSetter("Car can not Parking is this spot. Reveres Parking Process. ");
	}
	
	/* First Step */
	if (ParkingSteps_flag == FIRST_STEP)
	{
		Servo_Setposition2(SERVO_DEFAULTANGLE - 450);
		_delay_ms(SERVO_ADJ_DELAY);
		MOTOR_Speed_dir(CAR_MOTOR, 50 , Dir_Motor);
		// time
		
	}
	/* Second Step */
	else if (ParkingSteps_flag == SECOND_STEP)
	{
		Servo_Setposition2(SERVO_DEFAULTANGLE);
		_delay_ms(SERVO_ADJ_DELAY);
		MOTOR_Speed_dir(CAR_MOTOR, 50 , Dir_Motor);
		// time
	}
	/* Third Step */
	else if (ParkingSteps_flag == THIRD_STEP)
	{
		Servo_Setposition2(450 + SERVO_DEFAULTANGLE);
		_delay_ms(SERVO_ADJ_DELAY);
		MOTOR_Speed_dir(CAR_MOTOR, 50 , Dir_Motor);
		// time
	}
	/* Fourth Step */
	else if (ParkingSteps_flag == FOURTH_STEP)
	{
		Servo_Setposition2(SERVO_DEFAULTANGLE);
		_delay_ms(SERVO_ADJ_DELAY);
		// time
	}

}


static void Adjust_Car(void)
{
	/* Fifth Step */
	if (ParkingSteps_flag == FIFTH_STEP)
	{
		
		if (Read_Ultrasonic_Getter(BACK_RIGHT_ULTRA) > (LENGTH_TOLERANCE / 2))
		{
			MOTOR_Speed_dir(CAR_MOTOR, 50 , M_CW);
		}
		else
		{
			MOTOR_Speed_dir(CAR_MOTOR, 0 , M_STOP);
			ParkingDone_Flag =  TRUE;
		}
	}
}





static void Detect_Handicaps(void)
{
	if (ParkingSteps_flag >= FIRST_STEP && ParkingSteps_flag <= THIRD_STEP)
	{
		
		if ((Read_Ultrasonic_Getter(BACK_RIGHT_ULTRA) <= (MIN_DIS_OFBARRIER)) ||
			(Read_Ultrasonic_Getter(BACK_LEFT_ULTRA) <= (MIN_DIS_OFBARRIER)))
		{
			WaitingHindrance_flag = TRUE;
			StartStep_flag = FALSE;
			if (Handicaps_flag == FALSE)
			{
				MOTOR_Speed_dir(CAR_MOTOR, 0 , M_STOP);
				Handicaps_flag = TRUE;
			}
		}
		else
		{	
			HindranceCnt = 0;
			WaitingHindrance_flag = FALSE;
			if (Handicaps_flag)
			{
				StartStep_flag = TRUE;
				Handicaps_flag = FALSE;
			}
		}
		
		
	}
}






static void OCR_timer2_callBack(void)
{
	TCNT2 = 132;
	
	static u32 Time_Cnt = 0;

	if ((ParkingProtocol_flag) && (!WaitingHindrance_flag))
	{
		// Parking Stop Flag Increment
		Time_Cnt++;
		
		if (Time_Cnt == _1ST_STEP)
		{
			ParkingSteps_flag = SECOND_STEP;
			StartStep_flag = TRUE;
		}
		else if (Time_Cnt == _2ND_STEP)
		{
			ParkingSteps_flag = THIRD_STEP;
			StartStep_flag = TRUE;
		}
		else if (Time_Cnt == _3RD_STEP)
		{
			ParkingSteps_flag = FOURTH_STEP;
			StartStep_flag = TRUE;
		}
		else if (Time_Cnt == _4TH_STEP)
		{
			ParkingSteps_flag = FIFTH_STEP;
		}
	}
	
	
	if ((WaitingHindrance_flag))
	{	
		HindranceCnt++;
		if (HindranceCnt == HINDRANCE_TIME)
		{
			Dir_Motor = M_CW;
			ReverseParkingProtocol_flag = TRUE;
			if (Time_Cnt > 50)
			{
				StartStep_flag = TRUE;	
			}
			
		}
	}
	
	if (ReverseParkingProtocol_flag == TRUE)
	{
		Time_Cnt--;

		if (Time_Cnt == 0)
		{
			Reset_System();
		}
		else if (Time_Cnt == _1ST_STEP)
		{
			ParkingSteps_flag = FIRST_STEP;
			StartStep_flag = TRUE;
		}
		else if (Time_Cnt == _2ND_STEP)
		{
			ParkingSteps_flag = SECOND_STEP;
			StartStep_flag = TRUE;
		}
	
	}
	
}



static void DisplayOn_LCD(void)
{

	LCD_SetCursor(0, 0);
	
	if ((SpotCondition_flag) && (!ParkingProtocol_flag))
	{
		if (Sent_flags[0] == 1)
		{
			LCD_WriteString("Park Spot Found");
			BCM_SendStringSetter("Car founding parking spot. ");
			Sent_flags[0] = 0;
			 Flag_ture(0);
		}
	}
	else if ((ParkingDone_Flag))
	{
		if (Sent_flags[1] == 1)
		{
			LCD_WriteString("   Car Parked");
			BCM_SendStringSetter("Car Parked. ");
			Sent_flags[1] = 0;
			Flag_ture(1);
		}
	}
	else if (!ParkingProtocol_flag)
	{
		if (Sent_flags[2] == 1)
		{
			LCD_WriteString(" Searching Spot    ");
			BCM_SendStringSetter("Car searching for parking spot. ");
			Sent_flags[2] = 0;
			Flag_ture(2);
		}
	}
	else if (WaitingHindrance_flag)
	{
		if (Sent_flags[3] == 1)
		{
			LCD_WriteString("    Barrier    ");
			BCM_SendStringSetter("There is Barrier. Car Wait Barrier to remove.");
			Sent_flags[3] = 0;
			Flag_ture(3);
		}
	}
}


static void Flag_ture (u8 Index)
{
	for (u8 FlagIndex = 0; FlagIndex < 4; FlagIndex++)
	{
		if (FlagIndex != Index)
		{
			Sent_flags[FlagIndex] = 1;
		}
	}
}


static void Reset_System(void)
{
	ParkingProtocol_flag = FALSE;
	StartStep_flag = TRUE;
	Dir_Motor = M_CCW;
	ReverseParkingProtocol_flag = FALSE;
	ParkingSteps_flag = FIRST_STEP;
	ParkingDone_Flag = FALSE;
	ResetToFindingParkingSpot_flag = TRUE;
	
	WaitingHindrance_flag = FALSE;
	HindranceCnt = 0;
	 Handicaps_flag = FALSE;

	
}
