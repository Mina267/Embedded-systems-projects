#include "StdTypes.h"
#include "Utils.h"
#include "DIO_interface.h"
#include "EX_Interrupt.h"
#include "EEPROM_intrface.h"
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



/**************** System Variables ************************/
static Sytem_mode_t SystemMode = NO_MODE;
static	bool_t	SelectSysModeMSG_flag = TRUE;

/************* 1- RGB System variables ********************/
static RGB_mode_status_t RGB_Mode = RGB_NOMODE;
static RGB_Color_type PrevColor = BLACK;
static bool_t RGBSelectMSG_flag = TRUE;
static bool_t SetRGBColorMSG_flag = TRUE;
static bool_t ShowRoomRGBColorMSG_flag = TRUE;

/************* 2- Smart Home System variables *************/
static bool_t SH_EnterTaskMSG_flag = TRUE;
static c8 LCD_Str[MAX_LCD_STR];
static bool_t TempControlMode_flag = FALSE;
static bool_t OutDoorLedMSG_Flag = FALSE;
static bool_t WindowCurtainMSG_Flag = FALSE;
static bool_t GarageDoorMSG_Flag = FALSE;

/**************** String received from user Variables *******************/
static c8 Str_Recived[STR_RECIVED_SIZE];
static c8 StrCommand[STR_COMMAND_SIZE];
static c8 StrTask[STR_TASK_SIZE];
static u32 NumOrder;

/********************** Smart home Tasks TEXT. **************************/
static const c8 WRG_RoomMSG[] = "Wrong Room!.";
static const c8 WRG_TVmsg[] = "Wrong TV!.";
static const c8 AlreadyOn_MSG[] = "Already On";
static const c8 AlreadyOFF_MSG[] = "Already OFF";
static const c8 ErrorTempControlOn_MSG[] = "Error!Temp Ctrl On";

/**************************** Task Array **************************************/

static Master_Frame_t SendSPI_MasterFrame;
static Slave_Frame_t ReceiveSPI_SlaveFrame;
static const SmartHome_task_t TasksArray[TASK_NUM] = {
	{"LEDON", Leds_On},
	{"LEDOFF", Leds_OFF},
	{"ROOMLED ON", RoomLeds_On},
	{"ROOMLED OFF", RoomLeds_OFF},
	{"TVON", TV_ON},
	{"TVOFF", TV_OFF},
	{"LCD", LCD_Write},
	{"CONTROLWINDOW" ,ControlWindow},
	{"GARAGEDOOR" ,GarageDoor},
	{"WINDOWCURTAIN" ,WindowCurtain},
	{"OUTDOORLEDS" ,OutDoorLeds},	
	{"FANON" ,Fan_ON},
	{"FANOFF" ,Fan_OFF},
	{"AIRCONDITIONON" ,AirCondition_ON},
	{"AIRCONDITIONOFF" ,AirCondition_OFF},
	{"TEMPCONTROLMODE", Temp_ControlMode}
};


/************************ eeprom **********************/
Tasks_EEPROM_t Task_save;
static volatile bool_t SaveInEEPROM_Flag = FALSE;

void SmartHome_Init(void)
{
	BCM_ReceiveStringAshync_Init();
	BCM_SendStringInit();
	
	RGB_Init();
	RGB_LedColor(BLACK);
	
	EXI_Init();
	EXI_SetCallBack(EX_INT0, CallBack_transmitAndReceive_SPIframes);
	EXI_Enable(EX_INT0);
	
	
	// Read system data from EEPROM
	Read_SystemData();
	// Restore System Last status
	Restore_System();
	
	

	
}


void SmartHome_Runnable(void)
{
	switch (SystemMode)
	{
		case NO_MODE:
		TakeSystemMode();
		break;
		
		case RGB_MODE:
		RGB_SystemMode();
		break;
		
		case SMARTHOME_MODE:
		SmartHome_SystemMode();
		break;
	}
	
	ReceiveFrameStaus_responed();
	
	BCM_SendStringRunnable();
	
	
	if (SaveInEEPROM_Flag)
	{
		Save_SystemData();
		SaveInEEPROM_Flag = FALSE;
	}
	
}




/*************************************************************************************************/
/******************************** Select Operation Functions *************************************/
/*************************************************************************************************/

// Take System Operating Mode as default.
static void TakeSystemMode(void)
{
	if (SelectSysModeMSG_flag)
	{
		BCM_SendStringSetter("1-RGB/2-Home");
		SelectSysModeMSG_flag = FALSE;
	}
	
	BCM_RecStr_status_t Rec_status = BCM_ReceiveStringAshync(Str_Recived);
	
	if (Rec_status == BCM_STR_RECEVIED)
	{
		if (Str_cmp(Str_Recived, "1") == STR_MATCH)
		{
			
			SystemMode = RGB_MODE;
		}
		else if (Str_cmp(Str_Recived, "2") == STR_MATCH)
		{
			
			SystemMode = SMARTHOME_MODE;
		}
		else
		{
			SendWrongChoice_MSG();
		}
	}
}

// Reset all System.
static void Sytem_Reset(void)
{
	// Select operation variables Reset
	SystemMode = NO_MODE;
	SelectSysModeMSG_flag = TRUE;
	
	// RGB Reset
	RGB_Reset();
	
	// Smart Home Reset
	SmartHome_Reset();
	
	// EEPROM Reset
	EEPROM_reset();
}
// Return to select system mode menu.
static void Sytem_SelectMode(void)
{
	// Select operation variables Reset
	SystemMode = NO_MODE;
	SelectSysModeMSG_flag = TRUE;
	
	// RGB Back
	RGB_Back();
	
	// Smart Home Back
	SmartHome_Back();
}



/************************************************ - 1 - **********************************************/
/***************************************** RGB Mode Functions ****************************************/
/*****************************************************************************************************/


// Function Operate as RGB mode Selected.
static void RGB_SystemMode(void)
{
	switch (RGB_Mode)
	{
		case RGB_NOMODE:
		TakeRGBSystemMode();
		break;
		
		case RGB_SETCOLOR:
		SetRGB_color();
		break;
		
		case RGB_SHOWROOM:
		SetRGBShowRoom_color();
		break;
	}
}

static void TakeRGBSystemMode(void)
{
	if (RGBSelectMSG_flag)
	{
		BCM_SendStringSetter("1-Color/2-ShowRoom");
		RGBSelectMSG_flag = FALSE;
	}
	
	BCM_RecStr_status_t Rec_status = BCM_ReceiveStringAshync(Str_Recived);
	if (Rec_status == BCM_STR_RECEVIED)
	{
		if (Str_cmp(Str_Recived, "1") == STR_MATCH)
		{
			RGB_Mode = RGB_SETCOLOR;
		}
		else if (Str_cmp(Str_Recived, "2") == STR_MATCH)
		{
			RGB_Mode = RGB_SHOWROOM;
		}
		else
		{
			SendWrongChoice_MSG();
		}
	}
}

/************************************** RGB set Color ********************************************/
// Set color of RGB User Entered.
static void SetRGB_color(void)
{
	if (SetRGBColorMSG_flag)
	{
		BCM_SendStringSetter((c8*)RGB_MSG[RGB_ENTER_COLOR_MSG]);
		SetRGBColorMSG_flag = FALSE;
	}
	
	BCM_RecStr_status_t Rec_status = BCM_ReceiveStringAshync(Str_Recived);
	// Check if user first Enter General command by check for '/'.
	if (Rec_status == BCM_STR_RECEVIED)
	{
		if (Str_Recived[FIRST_CHAR] == '/')
		{
			StrAnalysis_GeneralCommands();
		}
		else
		{
			RGB_CommandStatus_t CommandStatus = StrAnalysisRGBMode();
			if (CommandStatus == RGB_WEONG_COMMAND)
			{
				SendWrongCommand_MSG();
			}
			else
			{
				RGB_SetColorExecuteCommand();
			}
		}
	}
}



// Execute String command user Input of set color.
static void RGB_SetColorExecuteCommand(void)
{
	// If user Want To Go back or Reset RGB Setting
	// By Enter "RGB_BACK" or "RGB_RESET"
	if (Str_cmp(StrTask, (c8*)RGB_Commands[RGB_RESET_CMD]) == STR_MATCH)
	{
		RGB_Reset();
	}
	else if (Str_cmp(StrTask, (c8*)RGB_Commands[RGB_BACK_CMD]) == STR_MATCH)
	{
		RGB_Back();
	}
	else
	{
		// Convert User color name string to its index in RGB Drive.
		RGB_Color_type color = RGB_LedColorIndex(StrTask);
		// If this color is not Existing in RGB color "Send Wrong! Color."	
		if (color == COLOR_NUM)
		{
			color = PrevColor;
			
			BCM_SendStringSetter((c8*)RGB_MSG[RGB_WRG_COLOR_MSG]);
		}
		else
		{
			BCM_SendStringSetter((c8*)RGB_MSG[RGB_COLOR_CHG_MSG]);
		}
		RGB_LedColor(color);
		SetRGBColorMSG_flag = TRUE;
		PrevColor = color;
		Task_save.PrevColor = PrevColor;
	}
}


/************************************** RGB Show Room Color ********************************************/



// Show Room color of RGB User Entered.
static void SetRGBShowRoom_color(void)
{
	if (ShowRoomRGBColorMSG_flag)
	{
		BCM_SendStringSetter((c8*)RGB_MSG[RGB_ENTER_COLOR_MSG]);
		ShowRoomRGBColorMSG_flag = FALSE;
	}
	
	BCM_RecStr_status_t Rec_status = BCM_ReceiveStringAshync(Str_Recived);
	if (Rec_status == BCM_STR_RECEVIED)
	{
		if (Str_Recived[FIRST_CHAR] == '/')
		{
			StrAnalysis_GeneralCommands();
		}
		else
		{
			RGB_CommandStatus_t CommandStatus = StrAnalysisRGBMode();
			if (CommandStatus == RGB_WEONG_COMMAND)
			{
				SendWrongCommand_MSG();
			}
			else
			{
				RGB_ShowRoomExecuteCommand();
			}
		}
	}
}
// Execute String command user Input of show room.
static void RGB_ShowRoomExecuteCommand(void)
{
	// If user Want To Go back or Reset RGB Setting
	// By Enter "RGB_BACK" or "RGB_RESET"
	if (Str_cmp(StrTask, (c8*)RGB_Commands[RGB_RESET_CMD]) == STR_MATCH)
	{
		RGB_Reset();
	}
	else if (Str_cmp(StrTask, (c8*)RGB_Commands[RGB_BACK_CMD]) == STR_MATCH)
	{
		RGB_Back();
	}
	else
	{
		// Convert User color name string to its index in RGB Drive.
		RGB_Color_type color = RGB_LedColorIndex(StrTask);
		// If this color is not Existing in RGB color "Send Wrong! Color."
		if (color == COLOR_NUM)
		{
			color = PrevColor;
			BCM_SendStringSetter((c8*)RGB_MSG[RGB_WRG_COLOR_MSG]);
		}
		
		
		RGB_Status showStatus = RGB_ShowRoomSetColors(PrevColor ,color);
		// If show Room did not finish previous Work.
		if ((showStatus == RGB_NDONE) && (color != PrevColor))
		{
			BCM_SendStringSetter((c8*)RGB_MSG[RGB_COLOR_NOTCHG_MSG]);
		}
		else if (color != PrevColor)
		{
			BCM_SendStringSetter((c8*)RGB_MSG[RGB_COLOR_CHG_MSG]);
		}
		
		ShowRoomRGBColorMSG_flag = TRUE;
		PrevColor = color;
		Task_save.PrevColor = PrevColor;
	}
}



// Reset RGB as default
static void RGB_Reset(void)
{
	RGB_LedColor(BLACK);
	RGB_Mode = RGB_NOMODE;
	PrevColor = BLACK;
	Task_save.PrevColor = PrevColor;
	RGBSelectMSG_flag = TRUE;
	SetRGBColorMSG_flag = TRUE;
	ShowRoomRGBColorMSG_flag = TRUE;
}
// Back To select RGB mode;
static void RGB_Back(void)
{
	RGB_Mode = RGB_NOMODE;
	RGBSelectMSG_flag = TRUE;
	SetRGBColorMSG_flag = TRUE;
	ShowRoomRGBColorMSG_flag = TRUE;
	
}



/************ Analysis string of RGB mode ****************/
// Take String From UART. and analysis Command User send
static RGB_CommandStatus_t StrAnalysisRGBMode(void)
{
	RGB_CommandStatus_t CommandStatus = RGB_ACCEPTED_COMMAND;
	if (Str_Recived[FIRST_CHAR] == NULL)
	{
		CommandStatus = RGB_NO_COMMAND;
	}
	else
	{
		u8 ColorIndex = 0;
		u8 StrIndex;
		// Take First Command "RGB"
		for (StrIndex = 0; Str_Recived[StrIndex] != '_'; StrIndex++)
		{
			// If did not Found '_' and found NULL Exit.
			if (Str_Recived[StrIndex] == NULL)
			{
				CommandStatus = RGB_WEONG_COMMAND;
				break;
			}
			StrCommand[StrIndex] = Str_Recived[StrIndex];
		}
		StrCommand[StrIndex] = NULL;
		StrIndex++;
		
		// Check if First Command is RGB
		if (Str_cmp(StrCommand, "RGB") == STR_NOMATCH)
		{
			CommandStatus = RGB_WEONG_COMMAND;
		}
		
		// Take Name of color 
		while (Str_Recived[StrIndex])
		{
			StrTask[ColorIndex] = Str_Recived[StrIndex];

			ColorIndex++;
			StrIndex++;
		}
		StrTask[ColorIndex] = 0;
		
		// If StrTask Still NULL then there is no color
		if (StrTask == NULL)
		{
			CommandStatus = RGB_WEONG_COMMAND;
		}
	}
	
	return CommandStatus;

}



/********************************************** - 2 - ************************************************/
/************************************ Smart Home Mode Functions **************************************/
/*****************************************************************************************************/

// Function Operate as Smart Home mode Selected.
static void SmartHome_SystemMode(void)
{
	TakeSmartHome_SystemTask();
}

// Take Task User want to execute.
static void TakeSmartHome_SystemTask(void)
{
	if (SH_EnterTaskMSG_flag)
	{
		BCM_SendStringSetter("Enter Task.");
		SH_EnterTaskMSG_flag = FALSE;
	}
	
	
	BCM_RecStr_status_t Rec_status = BCM_ReceiveStringAshync(Str_Recived);
	// Check if user first Enter General command by check for '/'.
	if (Rec_status == BCM_STR_RECEVIED)
	{
		if (Str_Recived[FIRST_CHAR] == '/')
		{
			StrAnalysis_GeneralCommands();
		}
		else
		{
			
			SH_EnterTaskMSG_flag = TRUE;
			SH_CommandStatus_t CommandStatus = StrAnalysis_SmartHomeMode();
			
			if (CommandStatus == SH_WEONG_COMMAND)
			{
				
				SendWrongCommand_MSG();
			}
			else
			{
				SmartHome_ExecuteCommand();
			}
			
		}
	}
}

// Search in task array for task order.
static TASKS_Status_t SearchForTaskOrder(u16 *TaskIndex)
{
	Str_toUpper(StrCommand);
	TASKS_Status_t taskStatus = TASK_NOT_FOUND;
	u8 Index;
	for (Index = 0; Index < TASK_NUM; Index++)
	{
		if (Str_cmp(StrCommand, (c8*)TasksArray[Index].str) == STR_MATCH)
		{
			*TaskIndex = Index;
			taskStatus = TASK_FOUND;
		}
	}
	return taskStatus;
	
}
// Execute Task of Specific order from array
static void SmartHome_ExecuteCommand(void)
{
	u16 TaskIndex;
	TASKS_Status_t taskStatus = SearchForTaskOrder(&TaskIndex);
	if (taskStatus == TASK_FOUND)
	{
		TasksArray[TaskIndex].Fptr(NumOrder);
		BCM_SendStringSetter("Task Executed.");
	}
	else if (taskStatus == TASK_NOT_FOUND)
	{
		BCM_SendStringSetter("Wrong! Task.");
	}
}


// Reset Smart Home as default
static void SmartHome_Reset(void)
{
	SH_EnterTaskMSG_flag = TRUE;
	
	SendSPI_MasterFrame.AirConditiondegree = 0;
	SendSPI_MasterFrame.FanSpeed = 0;
	SendSPI_MasterFrame.MotorsStatus = 0;
	SendSPI_MasterFrame.WindowPostion = 0;
	// close all LEDs
	Leds_OFF(0);
	// close all room 
	RoomLeds_OFF(0);
	// close all TV
	TV_OFF(0);
	// Clear LCD
	LCD_Clear();
	
	Temp_ControlMode(0);
	CLR_BIT(SendSPI_MasterFrame.MotorsStatus, LDR_CONTROL);
	ControlWindow(0);
	OutDoorLeds(0);
	AirCondition_OFF(0);
	Fan_OFF(0);
	
	
	Str_Recived[FIRST_CHAR] = NULL;
	StrCommand[FIRST_CHAR] = NULL;
	StrTask[FIRST_CHAR] = NULL;
	
}
// Back To select system mode;
static void SmartHome_Back(void)
{
	SH_EnterTaskMSG_flag = TRUE;
	Str_Recived[FIRST_CHAR] = NULL;
	StrCommand[FIRST_CHAR] = NULL;
	StrTask[FIRST_CHAR] = NULL;
}



/************ Analysis string of Smart home mode ********/
static SH_CommandStatus_t StrAnalysis_SmartHomeMode(void)
{
	SH_CommandStatus_t CommandStatus = SH_ACCEPTED_COMMAND;
	bool_t Num_flag = TRUE;
	u8 LCD_flag = 0;
	u8 LCD_StrIndex = 0;
	u8 StrIndex;
	
	
	if (Str_Recived[FIRST_CHAR] == NULL)
	{
		
		CommandStatus = SH_WEONG_COMMAND;
	}
	else
	{
		
		// Take Task name.
		for (StrIndex = 0; Str_Recived[StrIndex] != '_'; StrIndex++)
		{
			// If found NULL Break
			if (Str_Recived[StrIndex] == NULL)
			{
				Num_flag = FALSE;
				break;
			}
			// If Found '-' Break and set LCD_flag to 1.
			else if (Str_Recived[StrIndex] == '-')
			{
				LCD_flag = 1;
				Num_flag = FALSE;
				break;
			}
			// Store task name in strCommand
			StrCommand[StrIndex] = Str_Recived[StrIndex];
		}
		StrCommand[StrIndex] = 0;
		StrIndex++;
		
		if (Str_cmp(StrCommand, "LCD") == STR_MATCH)
		{
			LCD_flag = 2;
		}
		
		NumOrder = 0;
		
		
		// Take Task number if can
		if (Num_flag)
		{
			while (Str_Recived[StrIndex])
			{
				if ((Str_Recived[StrIndex] >= '0' && Str_Recived[StrIndex] <= '9'))
				{
					NumOrder = (NumOrder * 10) + (Str_Recived[StrIndex] - '0');
				}
				else
				{
					CommandStatus = SH_WEONG_COMMAND;
				}
				StrIndex++;
			}
		}
		
		// To Write Strings in LCD
		if (LCD_flag == 2)
		{
			while (Str_Recived[StrIndex])
			{
				LCD_Str[LCD_StrIndex] = Str_Recived[StrIndex];
				LCD_StrIndex++;
				StrIndex++;
			}
			LCD_Str[LCD_StrIndex] = 0;
		}
		
	}
	
	
	return CommandStatus;
}



/********************** Smart home Tasks Functions. **************************/


// Enter Number of led to turn On
static void Leds_On(u16 LED)
{
	LEDS_types ledColor = LED - 1;
	if ((LED <= MAX_NUMOF_LED) && LED != 0)
	{
		LEDS_On(ledColor);
		switch(ledColor)
		{
			case RED_LED:
			BCM_SendStringSetter("RedON.");
			Task_save.Red = 1;
			break;
			
			case GRN_LED:
			BCM_SendStringSetter("GrnON.");
			Task_save.Grn = 1;
			break;
			
			case BLU_LED:
			BCM_SendStringSetter("BlueON.");
			Task_save.Blu = 1;
			break;
			
			default:
			;
		}
	}
	else if (LED == 0)
	{
		LEDS_On(RED_LED);
		LEDS_On(GRN_LED);
		LEDS_On(BLU_LED);
		BCM_SendStringSetter("LEDs ON.");
		Task_save.Red = 1;
		Task_save.Grn = 1;
		Task_save.Blu = 1;
	}
	else
	{
		BCM_SendStringSetter("Wrong!Led.");
	}
}





// Enter Number of led to turn OFF
static void Leds_OFF(u16 LED)
{
	LEDS_types ledColor = LED - 1;
	if ((LED <= MAX_NUMOF_LED) && LED != 0)
	{
		LEDS_Off(ledColor);
		switch(ledColor)
		{
			case RED_LED:
			BCM_SendStringSetter("RedOFF.");
			Task_save.Red = 0;
			break;
			
			case GRN_LED:
			BCM_SendStringSetter("GrnOFF.");
			Task_save.Grn = 0;
			break;
			
			case BLU_LED:
			BCM_SendStringSetter("BlueOFF.");
			Task_save.Blu = 0;
			break;
			
			default:
			;
		}
	}
	else if (LED == 0)
	{
		LEDS_Off(RED_LED);
		LEDS_Off(GRN_LED);
		LEDS_Off(BLU_LED);
		BCM_SendStringSetter("LEDs OFF.");
		Task_save.Red = 0;
		Task_save.Grn = 0;
		Task_save.Blu = 0;
	}
	else
	{
		BCM_SendStringSetter("Wrong!Led.");
	}
}


// Enter Number of Room to turn its Led On.
// Turn all Rooms LED On but zero or no number.
static void RoomLeds_On(u16 Room)
{
	switch(Room)
	{
		case 0:
		DIO_WritePin(ROOM1_LEDS, HIGH);
		DIO_WritePin(ROOM2_LEDS, HIGH);
		DIO_WritePin(ROOM3_LEDS, HIGH);
		DIO_WritePin(ROOM4_LEDS, HIGH);
		BCM_SendStringSetter("Rooms On.");
		Task_save.Room1 = 1;
		Task_save.Room2 = 1;
		Task_save.Room3 = 1;
		Task_save.Room4 = 1;
		break;
		
		case 1:
		DIO_WritePin(ROOM1_LEDS, HIGH);
		BCM_SendStringSetter("Room1 On.");
		Task_save.Room1 = 1;
		break;
		
		case 2:
		DIO_WritePin(ROOM2_LEDS, HIGH);
		BCM_SendStringSetter("Room2 On.");
		Task_save.Room2 = 1;
		break;
		
		case 3:
		DIO_WritePin(ROOM3_LEDS, HIGH);
		BCM_SendStringSetter("Room3 On.");
		Task_save.Room3 = 1;
		break;
		
		case 4:
		DIO_WritePin(ROOM4_LEDS, HIGH);
		BCM_SendStringSetter("Room4 On.");
		Task_save.Room4 = 1;
		break;
		
		default:
		BCM_SendStringSetter((c8*)WRG_RoomMSG);
	}
}


// Enter Number of Room to turn its Led Off.
// Turn all Rooms LED OFF but zero or no number.
static void RoomLeds_OFF(u16 Room)
{
	switch(Room)
	{
		case 0:
		DIO_WritePin(ROOM1_LEDS, LOW);
		DIO_WritePin(ROOM2_LEDS, LOW);
		DIO_WritePin(ROOM3_LEDS, LOW);
		DIO_WritePin(ROOM4_LEDS, LOW);
		Task_save.Room1 = 0;
		Task_save.Room2 = 0;
		Task_save.Room3 = 0;
		Task_save.Room4 = 0;

		BCM_SendStringSetter("Rooms Off.");
		break;
		
		case 1:
		DIO_WritePin(ROOM1_LEDS, LOW);
		BCM_SendStringSetter("Room1 Off.");
		Task_save.Room1 = 0;
		break;
		
		case 2:
		DIO_WritePin(ROOM2_LEDS, LOW);
		BCM_SendStringSetter("Room2 Off.");
		Task_save.Room2 = 0;
		break;
		
		case 3:
		DIO_WritePin(ROOM3_LEDS, LOW);
		BCM_SendStringSetter("Room3 Off.");
		Task_save.Room3 = 0;
		break;
		
		case 4:
		DIO_WritePin(ROOM4_LEDS, LOW);
		BCM_SendStringSetter("Room 4 Off.");
		Task_save.Room4 = 0;
		break;
		
		default:
		BCM_SendStringSetter((c8*)WRG_RoomMSG);
	}
}





// Enter number of TV to turn on
// Turn all TVs on but zero or no number.
static void TV_ON(u16 TV)
{
	RELAY_t Tv_Relay = TV;
	
	if ((TV > 0) && (TV <= TV_NUM))
	{
		RELAY_On(Tv_Relay);
		switch(Tv_Relay)
		{
			case _1_TV_RELAY:
			BCM_SendStringSetter("TV1On.");
			Task_save.TV1 = 1;
			break;
			
			case _2_TV_RELAY:
			BCM_SendStringSetter("TV2On.");
			Task_save.TV2 = 1;
			break;
			
			default:
			;
		}
	}
	else if (TV == 0)
	{
		RELAY_On(RELAY_ALL);
		BCM_SendStringSetter("TVs On.");
		Task_save.TV1 = 1;
		Task_save.TV2 = 1;
	}
	else
	{
		BCM_SendStringSetter((c8*)WRG_TVmsg);
	}
}

// Enter number of TV to turn off
// Turn all TVs off but zero or no number.
static void TV_OFF(u16 TV)
{
	RELAY_t Tv_Relay = TV;
	
	if ((TV > 0) && (TV <= TV_NUM))
	{
		RELAY_Off(Tv_Relay);
		switch(Tv_Relay)
		{
			case _1_TV_RELAY:
			BCM_SendStringSetter("TV1Off.");
			Task_save.TV1 = 0;
			break;
			
			case _2_TV_RELAY:
			BCM_SendStringSetter("TV2Off.");
			Task_save.TV2 = 0;
			break;
			
			default:
			;
		}
	}
	else if (TV == 0)
	{
		RELAY_Off(RELAY_ALL);
		BCM_SendStringSetter("TVs Off.");
		Task_save.TV1 = 0;
		Task_save.TV2 = 0;
	}
	else
	{
		BCM_SendStringSetter((c8*)WRG_TVmsg);
	}
	
	
}



// Write String on LCD
static void LCD_Write(u16 n)
{
	if (n == 0)
	{
		LCD_Clear();
		LCD_WriteStringOverFlow(0 ,0, LCD_Str);
	}
	
}

// The position in degree or windows open
static void ControlWindow(u16 Positon)
{
	if (Positon <= 180)
	{
		SendSPI_MasterFrame.WindowPostion = Positon;
	}
	else
	{
		SendSPI_MasterFrame.WindowPostion = 180;
	}
	
}




// 0 for Close 1 for open
static void GarageDoor(u16 Status)
{	
	u8 GarageDoor_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, GRAGEDOOR);
	if (Status == 0)
	{
		if (GarageDoor_rec == 0)
		{
			BCM_SendStringSetter((c8*)AlreadyOFF_MSG);
		}
		else
		{
			CLR_BIT(SendSPI_MasterFrame.MotorsStatus, GRAGEDOOR);
			GarageDoorMSG_Flag = TRUE;
		}
	}
	else if (Status == 1)
	{
		if (GarageDoor_rec == 1)
		{
			BCM_SendStringSetter((c8*)AlreadyOn_MSG);
		}
		else
		{
			SET_BIT(SendSPI_MasterFrame.MotorsStatus, GRAGEDOOR);
			GarageDoorMSG_Flag = TRUE;
		}
	}
}



// Open and close window Curtain
static void WindowCurtain(u16 Status)
{
	u8 WindowCurtain_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, WINDOW_CURTION);
	if (Status == 0)
	{
		if (WindowCurtain_rec == 0)
		{
			BCM_SendStringSetter((c8*)AlreadyOFF_MSG);
		}
		else
		{
			CLR_BIT(SendSPI_MasterFrame.MotorsStatus, WINDOW_CURTION);
			WindowCurtainMSG_Flag = TRUE;
		}
	}
	else if (Status == 1)
	{
		if (WindowCurtain_rec == 1)
		{
			BCM_SendStringSetter((c8*)AlreadyOn_MSG);
		}
		else
		{
			SET_BIT(SendSPI_MasterFrame.MotorsStatus, WINDOW_CURTION);
			WindowCurtainMSG_Flag = TRUE;
		}
	}
}



static const c8 LCRCntrlOn_MSG[] = "LDR!CtrlOn.";

// 0 for Close 1 for open
static void OutDoorLeds(u16 Status)
{
	u8 Outdoorled_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, OUTDOORLEDS);
	u8 LDR_control_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, LDR_CONTROL);
	
	if ((LDR_control_rec == 1) && (Status != 2))
	{
		BCM_SendStringSetter((c8*)LCRCntrlOn_MSG);
	}
	else if (Status == 0)
	{
		if (Outdoorled_rec == 0)
		{
			BCM_SendStringSetter((c8*)AlreadyOFF_MSG);
		}
		else
		{
			CLR_BIT(SendSPI_MasterFrame.MotorsStatus, OUTDOORLEDS);
			OutDoorLedMSG_Flag = TRUE;
		}
		
	}
	else if (Status == 1)
	{
		if (Outdoorled_rec == 1)
		{
			BCM_SendStringSetter((c8*)AlreadyOn_MSG);
		}
		else
		{
			SET_BIT(SendSPI_MasterFrame.MotorsStatus, OUTDOORLEDS);
			OutDoorLedMSG_Flag = TRUE;
		}
		
	}
	else if (Status == 2)
	{
		if (LDR_control_rec == 0)
		{
			SET_BIT(SendSPI_MasterFrame.MotorsStatus, LDR_CONTROL);
			BCM_SendStringSetter((c8*)LCRCntrlOn_MSG);
		}
		else
		{
			CLR_BIT(SendSPI_MasterFrame.MotorsStatus, LDR_CONTROL);
			BCM_SendStringSetter("LCD CtrlOFF.");
		}
		
	}
	
}





// Fan Speed
static void Fan_ON(u16 Speed)
{
	u8 TempControlMode_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, TEMPCONTROL);
	if (TempControlMode_rec == 1)
	{
		BCM_SendStringSetter((c8*) ErrorTempControlOn_MSG);
	}
	else
	{
		SET_BIT(SendSPI_MasterFrame.MotorsStatus, FAN);
		
		if (Speed <= 100)
		{
			SendSPI_MasterFrame.FanSpeed = Speed;
		}
		else
		{
			SendSPI_MasterFrame.FanSpeed = 100;
		}
	}
	
	
}


// Turn Fan off
static void Fan_OFF(u16 Speed)
{
	u8 TempControlMode_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, TEMPCONTROL);
	if (TempControlMode_rec == 1)
	{
		BCM_SendStringSetter((c8*) ErrorTempControlOn_MSG);
	}
	else
	{
		// If air condition already on do nothing
		if (READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, FAN) == 0)
		{
			BCM_SendStringSetter((c8*)AlreadyOFF_MSG);
		}
		else if (Speed == 0)
		{
			CLR_BIT(SendSPI_MasterFrame.MotorsStatus, FAN);
			SendSPI_MasterFrame.FanSpeed = 0;
		}
	}
}

// Set Degree of air Condition.
static void AirCondition_ON(u16 Degree)
{
	u8 TempControlMode_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, TEMPCONTROL);
	if (TempControlMode_rec == 1)
	{
		BCM_SendStringSetter((c8*) ErrorTempControlOn_MSG);
	}
	else
	{
	
		SET_BIT(SendSPI_MasterFrame.MotorsStatus, AIRCONDITION);
		if ((Degree >= 18) && (Degree <= 28))
		{
			SendSPI_MasterFrame.AirConditiondegree = Degree;
		}
		else if ((Degree < 18))
		{
			SendSPI_MasterFrame.AirConditiondegree = 18;
		}
		else
		{
			SendSPI_MasterFrame.AirConditiondegree = 28;
		}
	}
}

// Close of air Condition.
static void AirCondition_OFF(u16 Degree)
{
	u8 TempControlMode_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, TEMPCONTROL);
	if (TempControlMode_rec == 1)
	{
		BCM_SendStringSetter((c8*) ErrorTempControlOn_MSG);
	}
	else
	{
			
		if (Degree == 0)
		{
			// If air condition already on do nothing
			if (READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, AIRCONDITION) == 0)
			{
				BCM_SendStringSetter((c8*)AlreadyOFF_MSG);
			}
			else
			{
				CLR_BIT(SendSPI_MasterFrame.MotorsStatus, AIRCONDITION);
				SendSPI_MasterFrame.AirConditiondegree = 0;
			}
		}
	}
}

static const c8 Tmpctrloff_MSG[] = "Temp ctrl On.";
// Make temp sensor control operation of Air condition and fan
static void Temp_ControlMode(u16 mode)
{
	u8 TempControlMode_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, TEMPCONTROL); 
	if (mode == 1)
	{
		if (TempControlMode_rec == 1)
		{
			BCM_SendStringSetter((c8*)AlreadyOn_MSG);
		}
		else
		{
			TempControlMode_flag = TRUE;
			SET_BIT(SendSPI_MasterFrame.MotorsStatus, TEMPCONTROL);
			BCM_SendStringSetter((c8*)Tmpctrloff_MSG);
		}
		
	}
	else if (mode == 0)
	{
		if (TempControlMode_rec == 0)
		{
			BCM_SendStringSetter((c8*)AlreadyOFF_MSG);
		}
		else
		{
			TempControlMode_flag = FALSE;
			CLR_BIT(SendSPI_MasterFrame.MotorsStatus, TEMPCONTROL);
			BCM_SendStringSetter((c8*)Tmpctrloff_MSG);
		}
		
	}
}


// Send message when task in salve MCU executed.
static void ReceiveFrameStaus_responed(void)
{

	if (OutDoorLedMSG_Flag)
	{
		u8 Outdoorled_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, OUTDOORLEDS);
		u8 Outdoorled_Send = READ_BIT(SendSPI_MasterFrame.MotorsStatus, OUTDOORLEDS);
		if ((Outdoorled_rec == 1) && (Outdoorled_Send == 1))
		{
			BCM_SendStringSetter("OutLeds On");
			OutDoorLedMSG_Flag = FALSE;
		}
		else if ((Outdoorled_rec == 0) && (Outdoorled_Send == 0))
		{
			BCM_SendStringSetter("OutLeds Off");
			OutDoorLedMSG_Flag = FALSE;
		}
	}
	
	if (WindowCurtainMSG_Flag)
	{
		u8 WindowCurtain_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, WINDOW_CURTION);
		u8 WindowCurtain_Send = READ_BIT(SendSPI_MasterFrame.MotorsStatus, WINDOW_CURTION);
		if ((WindowCurtain_rec == 1) && (WindowCurtain_Send == 1))
		{
			BCM_SendStringSetter("Curtain Open");
			WindowCurtainMSG_Flag = FALSE;
		}
		else if ((WindowCurtain_rec == 0) && (WindowCurtain_Send == 0))
		{
			BCM_SendStringSetter("Curtain close");
			WindowCurtainMSG_Flag = FALSE;
		}
	}

	if (GarageDoorMSG_Flag)
	{
		u8 GarageDoor_rec = READ_BIT(ReceiveSPI_SlaveFrame.MotorsStatus, GRAGEDOOR);
		u8 GarageDoor_Send = READ_BIT(SendSPI_MasterFrame.MotorsStatus, GRAGEDOOR);
		if ((GarageDoor_rec == 1) && (GarageDoor_Send == 1))
		{
			BCM_SendStringSetter("Garage Open");
			GarageDoorMSG_Flag = FALSE;
		}
		else if ((GarageDoor_rec == 0) && (GarageDoor_Send == 0))
		{
			BCM_SendStringSetter("Garage close");
			GarageDoorMSG_Flag = FALSE;
		}
	}
	
	
	
}



static void CallBack_transmitAndReceive_SPIframes(void)
{
	static u16 SPI_TimeCnt = SPI_FRAME_TIME - 1;
	SPI_TimeCnt++;
	if (SPI_TimeCnt == SPI_FRAME_TIME)
	{
		SPI_SetterSendFrame(SendSPI_MasterFrame.SPI_Array);
		SPI_ReceiveFrameGetter(ReceiveSPI_SlaveFrame.SPI_Array);
		SPI_TimeCnt = 0;
	}
	
	
	static u16 EEPROM_TimeCnt = 0;
	EEPROM_TimeCnt++;
	if (EEPROM_TimeCnt == EEPROM_TIME)
	{
		SaveInEEPROM_Flag = TRUE;
		EEPROM_TimeCnt = 0;
	}
}





/*************************************************************************************************/
/******************************** System General Command *****************************************/
/*************************************************************************************************/
// Can be use in any mode of system operate.
static void StrAnalysis_GeneralCommands(void)
{
	GeneralCommand_t CommandIndex;
	bool_t WrongCommad_flag = TRUE;
	for (CommandIndex = 0; CommandIndex < GENERAL_COMMAND_NUM; CommandIndex++)
	{
		STR_Status StrStatus = Str_cmp(Str_Recived + 1 , (c8*)GeneralCommands_Array[CommandIndex]);
		if (StrStatus == STR_MATCH)
		{
			WrongCommad_flag = FALSE;
			BCM_SendStringSetter("Gnrl command Executed");
			GeneralCommands_TakeAction(CommandIndex);
		}
	}
	
	if (WrongCommad_flag)
	{
		SendWrongCommand_MSG();
	}
}

static void GeneralCommands_TakeAction(GeneralCommand_t GeneralCommandNumber)
{
	switch (GeneralCommandNumber)
	{
		case RESET:
		Sytem_Reset();
		break;
		
		case SELECT_SYSTEM:
		Sytem_SelectMode();
		break;
		
		case GENERAL_COMMAND_NUM:
		break;
	}
}











/*************************************************************************************************/
/**************************** Utils Fuctions *****************************************************/
/*************************************************************************************************/

/* Received  , Expected return STR_NOMATCH or STR_MATCH */
static STR_Status Str_cmp(c8 *strReceive, c8 *StrExpected)
{
	Str_toUpper(strReceive);
	for (u8 Index = 0; strReceive[Index] || StrExpected[Index]; Index++)
	{
		if (strReceive[Index] != StrExpected[Index])
		{
			return STR_NOMATCH;
		}
	}
	return STR_MATCH;
}

static void Str_toUpper(c8 *str)
{
	for (u8 Index = 0; str[Index]; Index++)
	{
		if (str[Index] >= 'a' && str[Index] <= 'z')
		{
			str[Index] = str[Index] - 'a' + 'A';
		}
	}
}



static void SendWrongChoice_MSG(void)
{
	BCM_SendStringSetter("Wrong! Choice");
}

static void SendWrongCommand_MSG(void)
{
	BCM_SendStringSetter("Wrong! Command");
}





/***************************** Save system in EEPROM Functions **********************************/




static void Save_SystemData(void)
{
	EEPROM_writeBusy(START_SLAVE_ADDRESS, ReceiveSPI_SlaveFrame.MotorsStatus);
	u16 FrameAddress = START_FRAME_ADDRESS;
	for (u8 Index = FIRST_FRAME_INDEX; Index < SMARTHOME_FRAME_SIZE; Index++)
	{
		EEPROM_writeBusy(FrameAddress, SendSPI_MasterFrame.SPI_Array[Index]);
		FrameAddress++;
	}
	
	
	u16 SystemAddress = START_SYSTEM_ADDRESS;
	for (u8 Index = FIRST_SYSTEM_INDEX; Index < MASTER_TASKS_NUM; Index++)
	{
		EEPROM_writeBusy(SystemAddress, Task_save.TasksEEPROM_Array[Index]);
		SystemAddress++;
	}
}




static void Read_SystemData(void)
{
	
	ReceiveSPI_SlaveFrame.MotorsStatus = EEPROM_readBusy(START_SLAVE_ADDRESS);
	
	u16 FrameAddress = START_FRAME_ADDRESS;
	for (u8 Index = FIRST_FRAME_INDEX; Index < SMARTHOME_FRAME_SIZE; Index++)
	{
		SendSPI_MasterFrame.SPI_Array[Index] = EEPROM_readBusy(FrameAddress); 
		FrameAddress++;
	}
	
	
	u16 SystemAddress = START_SYSTEM_ADDRESS;
	for (u8 Index = FIRST_SYSTEM_INDEX; Index < MASTER_TASKS_NUM; Index++)
	{
		Task_save.TasksEEPROM_Array[Index] = EEPROM_readBusy(SystemAddress);
		SystemAddress++;
	}
	
	
	
}

static void EEPROM_reset(void)
{
	
	EEPROM_writeBusy(START_SLAVE_ADDRESS, 0);
	
	
	
	u16 SystemAddress = START_SYSTEM_ADDRESS;
	for (u8 Index = FIRST_SYSTEM_INDEX; Index < MASTER_TASKS_NUM; Index++)
	{
		EEPROM_writeBusy(SystemAddress, 0);
		SystemAddress++;
	}
}

static void Restore_System(void)
{
	PrevColor = Task_save.PrevColor;
	RGB_LedColor(PrevColor);
	
	// TVs
	if (Task_save.TV1 == 1)
	{
		TV_ON(1);
	}
	if (Task_save.TV2 == 1)
	{
		TV_ON(2);
	}
	
	// LEDs
	if (Task_save.Red == 1)
	{
		Leds_On(1);
	}
	if (Task_save.Grn== 1)
	{
		Leds_On(2);
	}
	if (Task_save.Blu == 1)
	{
		Leds_On(3);
	}
	
	// Rooms
	if (Task_save.Room1 == 1)
	{
		RoomLeds_On(1);
	}
	if (Task_save.Room3 == 1)
	{
		RoomLeds_On(2);
	}
	if (Task_save.Room3 == 1)
	{
		RoomLeds_On(3);
	}
	if (Task_save.Room4 == 1)
	{
		RoomLeds_On(4);
	}

	
}