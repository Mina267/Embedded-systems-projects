
#ifndef SMARTHOME_PRIVATE_H_
#define SMARTHOME_PRIVATE_H_

/********* String received from user Sizes *************/
#define STR_RECIVED_SIZE		40
#define STR_COMMAND_SIZE		25
#define STR_TASK_SIZE			25

/********* General Command ******************************/
#define FIRST_CHAR				0

/********************** Smart home Tasks. **************************/
#define MAX_NUMOF_LED			3
#define MAX_LCD_STR				35

extern const c8 *RGB_MSG[RGB_MSG_SIZE];
extern const c8 *RGB_Commands[RGB_COMMAND_SIZE];
extern const c8 *GeneralCommands_Array[GENERAL_COMMAND_SIZE];


// System Status enum.
typedef enum
{
	NO_MODE,
	RGB_MODE,
	SMARTHOME_MODE,
	}Sytem_mode_t;

// RGB Status mode enum.	
typedef enum
{
	RGB_NOMODE,
	RGB_SETCOLOR,
	RGB_SHOWROOM,
	}RGB_mode_status_t;	

// RGB String analysis enum.
typedef enum
{
	RGB_WEONG_COMMAND,
	RGB_NO_COMMAND,
	RGB_ACCEPTED_COMMAND,
	}RGB_CommandStatus_t;
	
// Smart Home String analysis enum.
typedef enum
{
	SH_WEONG_COMMAND,
	SH_NO_COMMAND,
	SH_ACCEPTED_COMMAND,
	}SH_CommandStatus_t;
	
// Smart Home Task Status.
typedef enum
{
	TASK_NOT_FOUND,
	TASK_FOUND,
	}TASKS_Status_t;
	
// General command enum.
typedef enum
{
	RESET,
	SELECT_SYSTEM,
	GENERAL_COMMAND_NUM,
	}GeneralCommand_t;	
	
// String Compare enum.
typedef enum
{
	STR_NOMATCH,
	STR_MATCH,
	}STR_Status;
	
	
// SPI Frame struct

typedef union
{
	u8 SPI_Array[SMARTHOME_FRAME_SIZE];
	struct
	{
		u8 WindowPostion;
		u8 FanSpeed;
		u8 AirConditiondegree;
		u8 MotorsStatus;
		};
	}Master_Frame_t;


typedef union
{
	u8 SPI_Array[SMARTHOME_FRAME_SIZE];
	struct
	{
		u8 HomeTemp;
		u8 MotorsStatus;
		};
	}Slave_Frame_t;
	
	
typedef union
{
	u8 TasksEEPROM_Array[MASTER_TASKS_NUM];
	struct
	{
		u8 PrevColor;
		u8 TV1;
		u8 TV2;
		u8 Red;
		u8 Grn;
		u8 Blu;
		u8 Room1;
		u8 Room2;
		u8 Room3;
		u8 Room4;
	};
	}Tasks_EEPROM_t;


/*************************************************************************************************/
/******************************** Select Operation Functions *************************************/
/*************************************************************************************************/

// Take System Operating Mode as default.
static void TakeSystemMode(void);
// Reset all System.
static void Sytem_Reset(void);
// Return to select system mode menu.
static void Sytem_SelectMode(void);

/*************************************************************************************************/
/******************************** System General Command *****************************************/
/*************************************************************************************************/

// Can be use in any mode of system operate.
static void StrAnalysis_GeneralCommands(void);
static void GeneralCommands_TakeAction(GeneralCommand_t GeneralCommandNumber);




/************************************ - 1 - ******************************************************/
/**************************** RGB mode Functions *************************************************/
/*************************************************************************************************/

// Function Operate as RGB mode Selected.
static void RGB_SystemMode(void);
// Take RGB mode User Want to operate with (SelectColor/ShowRoom).
static void TakeRGBSystemMode(void);
// Set color of RGB User Entered.
static void SetRGB_color(void);
// Execute String command user Input of set color.
static void RGB_SetColorExecuteCommand(void);
// Show Room color of RGB User Entered.
static void SetRGBShowRoom_color(void);
// Execute String command user Input of show room.
static void RGB_ShowRoomExecuteCommand(void);
// Reset RGB as default
static void RGB_Reset(void);
// Back To select RGB mode;
static void RGB_Back(void);


/************ Analysis string of RGB mode ****************/
// Take String From UART. and analysis Command User send
static RGB_CommandStatus_t StrAnalysisRGBMode(void);




/**************************************** - 2 - **************************************************/
/******************************** Smart Home Mode Functions **************************************/
/*************************************************************************************************/

// Function Operate as Smart Home mode Selected.
static void SmartHome_SystemMode(void);
// Take Task User want to execute.
static void TakeSmartHome_SystemTask(void);
// Search in task array for task order.
static TASKS_Status_t SearchForTaskOrder(u16 *TaskIndex);
// Execute Task of Specific order from array
static void SmartHome_ExecuteCommand(void);
// Reset Smart Home as default
static void SmartHome_Reset(void);
// Back To select system mode;
static void SmartHome_Back(void);

/****** Smart Home Control Functions *******/

// Enter Number of led to turn On
static void Leds_On(u16 LED);
// Enter Number of led to turn OFF
static void Leds_OFF(u16 LED);
// Enter Number of Room to turn its Led On.
// Turn all Rooms LED On but zero or no number.
static void RoomLeds_On(u16 Room);
// Enter Number of Room to turn its Led Off.
// Turn all Rooms LED OFF but zero or no number.
static void RoomLeds_OFF(u16 Room);
// Enter number of TV to turn on
// Turn all TVs on but zero or no number.
static void TV_ON(u16 n);
// Enter number of TV to turn off
// Turn all TVs off but zero or no number.
static void TV_OFF(u16 n);
// Write String on LCD
static void LCD_Write(u16 n);
// The position in degree or windows open
static void ControlWindow(u16 Positon);
// 0 for Close 1 for open
static void GarageDoor(u16 Status);
// Open and close window Curtain
static void WindowCurtain(u16 Status);
// 0 for Close 1 for open
static void OutDoorLeds(u16 Status);
// Fan Speed
static void Fan_ON(u16 Speed);
// Turn Fan off
static void Fan_OFF(u16 Speed);
// Set Degree of air Condition.
static void AirCondition_ON(u16 Degree);
// Close of air Condition.
static void AirCondition_OFF(u16 Degree);
// Send message when task in salve MCU executed.
static void ReceiveFrameStaus_responed(void);
// Make temp sensor control operation of Air condition and fan 
static void Temp_ControlMode(u16 mode);

static void CallBack_transmitAndReceive_SPIframes(void);


/************ Analysis string of Smart home mode ********/
static SH_CommandStatus_t StrAnalysis_SmartHomeMode(void);


/*************************************************************************************************/
/**************************** Utils Fuctions *****************************************************/
/*************************************************************************************************/

/* Received  , Expected   return STR_NOMATCH or STR_MATCH */
static STR_Status Str_cmp(c8 *strReceive, c8 *StrExpected);
// Convert String to Upper case
static void Str_toUpper(c8 *str);
// Send Wrong Choice MSG
static void SendWrongChoice_MSG(void);
// Send Wrong Command MSG
static void SendWrongCommand_MSG(void);


/************************** EEPROM ***************************************/
static void Save_SystemData(void);
static void Read_SystemData(void);
static void EEPROM_reset(void);
static void Restore_System(void);


#endif /* SMARTHOME_PRIVATE_H_ */