#include "StdTypes.h"
#include "Utils.h"
#include "MemMap.h"
#include "DIO_interface.h"
#include "EX_Interrupt.h"
#include "Timers.h"
#include "Timers_Services.h"
#include "BCM.h"
#include "SPI_Service.h"
#include "MOTOR_interface.h"
#include "LEDS.h"
#include "RELAY_Interface.h"
#include "LCD_interface.h"
#include "RGB_Interface.h"
#include "SmartHome_Cfg.h"
#include "SmartHome_Private.h"
#include "SmartHome.h"
#include "Servo_Interface.h"
#include "TEMP_Filter.h"
#include "Sensors.h"
#include "EEPROM_intrface.h"

# define F_CPU 8000000
#include <util/delay.h>

static volatile bool_t ReadTemp_flag = TRUE;
static bool_t TempControlMode_flag = FALSE;
static u16 HomeTemperature;

static u16 LDRread;
static volatile bool_t ReadLDR_Flag = TRUE;

static Master_Frame_t ReceiveSPI_MasterFrame;
static Slave_Frame_t SendSPI_SlaveFrame;

static volatile bool_t Save_Flag = FALSE;



static void Timer2CallBackOC_SPIframes(void)
{
	TCNT2 = 6;
	static u16 TimeCnt = 0;
	TimeCnt++;
	if (TimeCnt == 2)
	{
		ReadTemp_flag = TRUE;
		ReadLDR_Flag = TRUE;
		TimeCnt = 0;
		
	}
	
	static u16 E_Cnt = 0;
	E_Cnt++;
	if (E_Cnt == 400)
	{
		Save_Flag = TRUE;
		E_Cnt = 0;
	}
	DIO_TogglePin(PINC7);
}






void SmartHome_Init(void)
{
	/************************ Draw Pattern ************************/
	u8 arr1[8] = {0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00};
	LCD_makeNewShape(arr1, DEGREE);
	
	// Garage door and curtain
	EXI_Init();
	EXI_SetCallBack(EX_INT0, EX_INT0_CallBack_CurtainProximity);
	EXI_SetCallBack(EX_INT1, EX_INT1_CallBack_GarageProximity);
	EXI_Enable(EX_INT0);
	EXI_Enable(EX_INT1);
	
	// Fan Speed
	TIMER0_Init(TIMER0_PHASECORRECT_MODE, TIMER0_SCALER_8);
	TIMER0_OC0Mode(OC0_NON_INVERTING);
	
	// Window Position
	Servo_Init();
	
	TIMER2_Init(TIMER2_NORMAL_MODE, TIMER2_SCALER_1024);
	TIMER2_OV_SetCallBack(Timer2CallBackOC_SPIframes);
	TIMER2_OV_InterruptEnable();
	TCNT2 = 6;
	
	Read_SystemData();
	MOTOR_dir(WINDOW_CURTION_MOTOR, M_STOP);
	MOTOR_dir(GRAGEDOOR_MOTOR, M_STOP);
	
}


void SmartHome_Runnable(void)
{
	
	SPI_SetterSendFrame(SendSPI_SlaveFrame.SPI_Array);
	SPI_ReceiveFrameGetter(ReceiveSPI_MasterFrame.SPI_Array);
	
	if (ReadTemp_flag)
	{
		HomeTemperature = TEMP_Read();
		print_Temp(HomeTemperature, 1, 10);
		ReadTemp_flag = FALSE;
	}
	if (ReadLDR_Flag)
	{
		LDRread = LIGHT_Read();
		ReadLDR_Flag = FALSE;
	}
	
	if (Save_Flag)
	{
		Save_SystemData();
		Save_Flag = FALSE;

	}
	
	
	// Running of Tasks
	ControlWindow();
	GarageDoor();
	WindowCurtain();
	Temp_ControlMode();
	Fan_Operate();
	AirCondition();
	OutDoorLeds();
	LDR_Control();

}

// Control Position of windows
static void ControlWindow(void)
{
	u8 WindowPositon = ReceiveSPI_MasterFrame.WindowPostion;
	Servo_Setposition2(WindowPositon * 10);
}



// 0 for Close 1 for open
static void GarageDoor(void)
{
	u8 GarageDoor_Rec = READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, GRAGEDOOR);
	u8 GarageDoor_Send = READ_BIT(SendSPI_SlaveFrame.MotorsStatus, GRAGEDOOR);

	if ((GarageDoor_Rec == 1) && (GarageDoor_Send == 0))
	{
		MOTOR_dir(GRAGEDOOR_MOTOR, M_CW);
	}
	else if ((GarageDoor_Rec == 0) && (GarageDoor_Send == 1))
	{
		MOTOR_dir(GRAGEDOOR_MOTOR, M_CCW);
	}
	
	
}
// External interrupt coming from Proximity Sensor when Garage door is closed and opened
// When it close or open change condition in SPI frame from open to close
static void EX_INT1_CallBack_GarageProximity(void)
{
	u8 GarageDoor_Rec = READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, GRAGEDOOR);
	if (GarageDoor_Rec == 0)
	{
		CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, GRAGEDOOR);
	}
	else if (GarageDoor_Rec == 1)
	{
		SET_BIT(SendSPI_SlaveFrame.MotorsStatus, GRAGEDOOR);
	}
	MOTOR_dir(GRAGEDOOR_MOTOR, M_STOP);
}


// Open and close window Curtain
static void WindowCurtain(void)
{
	u8 WindowCurtain_Rec = READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, WINDOW_CURTION);
	u8 WindowCurtain_Send = READ_BIT(SendSPI_SlaveFrame.MotorsStatus, WINDOW_CURTION);
	if ((WindowCurtain_Rec == 1) && (WindowCurtain_Send == 0))
	{
		MOTOR_dir(WINDOW_CURTION_MOTOR, M_CW);
	}
	else if ((WindowCurtain_Rec == 0) && (WindowCurtain_Send == 1))
	{
		MOTOR_dir(WINDOW_CURTION_MOTOR, M_CCW);
	}
}

// External interrupt coming from Proximity Sensor when Curtain is closed and opened
// When it close or open change condition in SPI frame from open to close
static void EX_INT0_CallBack_CurtainProximity(void)
{
	u8 WindowCurtain_Rec = READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, WINDOW_CURTION);
	
	if (WindowCurtain_Rec == 0)
	{
		CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, WINDOW_CURTION);
	}
	else if (WindowCurtain_Rec == 1)
	{
		SET_BIT(SendSPI_SlaveFrame.MotorsStatus, WINDOW_CURTION);
	}
	MOTOR_dir(WINDOW_CURTION_MOTOR, M_STOP);
}




static bool_t OutDoorLedLDR_Control_Flag = FALSE;


// 0 for Close 1 for open
static void OutDoorLeds(void)
{
	u8 OutDoorLeds_Rec = READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, OUTDOORLEDS);
	if (OutDoorLedLDR_Control_Flag)
	{
		if (LDRread <= MIN_LIGHT)
		{
			DIO_WritePin(OUTDOORLEDS_PIN, HIGH);
			SET_BIT(SendSPI_SlaveFrame.MotorsStatus, OUTDOORLEDS);
		}
		else if (LDRread >= MAX_LIGHT)
		{
			DIO_WritePin(OUTDOORLEDS_PIN, LOW);
			CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, OUTDOORLEDS);
		}
	}
	else if ((OutDoorLeds_Rec == 1) && (!OutDoorLedLDR_Control_Flag))
	{
		DIO_WritePin(OUTDOORLEDS_PIN, HIGH);
		SET_BIT(SendSPI_SlaveFrame.MotorsStatus, OUTDOORLEDS);
	}
	else if ((OutDoorLeds_Rec == 0) && (!OutDoorLedLDR_Control_Flag))
	{
		DIO_WritePin(OUTDOORLEDS_PIN, LOW);
		CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, OUTDOORLEDS);
	}
}


static void LDR_Control(void)
{
	u8 LDR_Control_Rec = READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, LDR_CONTROL);
	if (LDR_Control_Rec == 1)
	{
		OutDoorLedLDR_Control_Flag = TRUE;
		SET_BIT(SendSPI_SlaveFrame.MotorsStatus, LDR_CONTROL);
	}
	else
	{
		OutDoorLedLDR_Control_Flag = FALSE;
		CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, LDR_CONTROL);
	}
}


// Fan Speed
static void Fan_Operate(void)
{
	if (TempControlMode_flag)
	{
		if (HomeTemperature >= OPERATE_TEMP)
		{
			SET_BIT(SendSPI_SlaveFrame.MotorsStatus, FAN);
			MOTOR_Speed_dir(FAN_MOTOR, DEFAULT_FAN_SPEED, M_CW);
		}
		if (HomeTemperature <= MIN_TEMP_STOP_OPERATE)
		{
			CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, FAN);
			MOTOR_Speed_dir(FAN_MOTOR, ZERO_SPEED, M_STOP);
		}
	}
	else
	{
		u8 FAN_Rec = READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, FAN);
		u8 Fan_Speed = ReceiveSPI_MasterFrame.FanSpeed;
		if (FAN_Rec == 1)
		{
			SET_BIT(SendSPI_SlaveFrame.MotorsStatus, FAN);
			MOTOR_Speed_dir(FAN_MOTOR, Fan_Speed, M_CW);
		}
		else
		{
			CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, FAN);
			MOTOR_Speed_dir(FAN_MOTOR, ZERO_SPEED, M_STOP);
		}
	}
}



// Set Degree of air Condition.
static void AirCondition(void)
{
	if (TempControlMode_flag)
	{
		if (HomeTemperature >= OPERATE_TEMP)
		{
			SET_BIT(SendSPI_SlaveFrame.MotorsStatus, AIRCONDITION);
			LCD_SetCursor(0, 0);
			LCD_WriteString("Air Condition ON ");
			print_Temp(DEFAULT_AIRCONITION_DGREE * 10, 1, 0);
	
		}
		if (HomeTemperature <= MIN_TEMP_STOP_OPERATE)
		{
			CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, AIRCONDITION);
			LCD_SetCursor(0, 0);
			LCD_WriteString("Air ConditionOFF  ");
			LCD_ClearCell(1, 0, 6);
		}
	}
	else
	{
		u8 AirCondition_Rec = READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, AIRCONDITION);
		u8 AirConditiondegree = ReceiveSPI_MasterFrame.AirConditiondegree;
		if (AirCondition_Rec == 1)
		{
			SET_BIT(SendSPI_SlaveFrame.MotorsStatus, AIRCONDITION);
			LCD_SetCursor(0, 0);
			LCD_WriteString("Air Condition ON ");
			print_Temp(AirConditiondegree * 10, 1, 0);
		}
		else
		{
			CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, AIRCONDITION);
			LCD_SetCursor(0, 0);
			LCD_WriteString("Air ConditionOFF ");
			LCD_ClearCell(1, 0, 6);
		}
	}
}

// Make temp sensor control operation of Air condition and fan.
static void Temp_ControlMode(void)
{
	if (READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, TEMPCONTROL) == 1)
	{
		SET_BIT(SendSPI_SlaveFrame.MotorsStatus, TEMPCONTROL);
		TempControlMode_flag = TRUE;
		
	}
	else if (READ_BIT(ReceiveSPI_MasterFrame.MotorsStatus, TEMPCONTROL) == 0)
	{
		CLR_BIT(SendSPI_SlaveFrame.MotorsStatus, TEMPCONTROL);
		TempControlMode_flag = FALSE;
	}
}



// Print Temp. in LCD
static void print_Temp(u16 temp, u8 line, u8 cell)
{
	LCD_SetCursor(line, cell);
	LCD_WriteNumber(temp / 10);
	LCD_WriteChar('.');
	LCD_WriteNumber(temp % 10);
	LCD_WriteChar(DEGREE);
	LCD_WriteString("C ");
}




/*
LCD_SetCursor(1, 0);
LCD_WriteBinary(ReceiveSPI_MasterFrame.SPI_Array[3]);
LCD_SetCursor(0, 0);
LCD_WriteNumber_3D(ReceiveSPI_MasterFrame.SPI_Array[2]);
LCD_WriteString(" ");
LCD_WriteNumber_3D(ReceiveSPI_MasterFrame.SPI_Array[1]);
LCD_WriteString(" ");
LCD_WriteNumber_3D(ReceiveSPI_MasterFrame.SPI_Array[0]);
LCD_WriteString(" ");


	LCD_SetCursor(1,0);
	LCD_WriteNumber(GarageDoor_Rec);
	LCD_SetCursor(1,3);
	LCD_WriteNumber(GarageDoor_Send);
*/








#define START_FRAME_ADDRESS			0X40
#define START_FRAMESLAVE_ADDRESS	0x10
#define FIRST_FRAME_INDEX			0



static void Save_SystemData(void)
{
	EEPROM_writeBusy(START_FRAMESLAVE_ADDRESS, SendSPI_SlaveFrame.MotorsStatus);
	u16 FrameAddress = START_FRAME_ADDRESS;
	for (u8 Index = FIRST_FRAME_INDEX; Index < SMARTHOME_FRAME_SIZE; Index++)
	{
		EEPROM_writeBusy(FrameAddress, ReceiveSPI_MasterFrame.SPI_Array[Index]);
		FrameAddress++;
	}
	
}




static void Read_SystemData(void)
{

	SendSPI_SlaveFrame.MotorsStatus = EEPROM_readBusy(START_FRAMESLAVE_ADDRESS);
	u16 FrameAddress = START_FRAME_ADDRESS;
	for (u8 Index = FIRST_FRAME_INDEX; Index < SMARTHOME_FRAME_SIZE; Index++)
	{
		ReceiveSPI_MasterFrame.SPI_Array[Index] = EEPROM_readBusy(FrameAddress);
		FrameAddress++;
	}
}
/*
static void EEPROM_reset(void)
{
	EEPROM_writeBusy(START_FRAMESLAVE_ADDRESS, 0);
	u16 FrameAddress = START_FRAME_ADDRESS;
	for (u8 Index = FIRST_FRAME_INDEX; Index < SMARTHOME_FRAME_SIZE; Index++)
	{
		EEPROM_writeBusy(FrameAddress, 0);
		FrameAddress++;
	}
}
*/