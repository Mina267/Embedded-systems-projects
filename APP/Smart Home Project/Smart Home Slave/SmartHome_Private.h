
#ifndef SMARTHOME_PRIVATE_H_
#define SMARTHOME_PRIVATE_H_


/********* General Command ******************************/
#define FIRST_CHAR				0
#define DEGREE				1


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

// The position in degree or windows open
static void ControlWindow(void);
// 0 for Close 1 for open
static void GarageDoor(void);
// External interrupt coming from Proximity Sensor when Garage door is closed and opened
// When it close or open change condition in SPI frame from open to close
static void EX_INT1_CallBack_GarageProximity(void);
// Open and close window Curtain
static void WindowCurtain(void);
// External interrupt coming from Proximity Sensor when Curtain is closed and opened
// When it close or open change condition in SPI frame from open to close
static void EX_INT0_CallBack_CurtainProximity(void);
// 0 for Close 1 for open
static void OutDoorLeds(void);
static void LDR_Control(void);
// Fan Speed
static void Fan_Operate(void);
// Set Degree of air Condition. Close of air Condition.
static void AirCondition(void);
// Make temp sensor control operation of Air condition and fan 
static void Temp_ControlMode(void);
// Print Temp. in LCD
static void print_Temp(u16 temp, u8 line, u8 cell);


static void Timer2CallBackOC_SPIframes(void);


/************************** EEPROM ***************************************/
static void Save_SystemData(void);
static void Read_SystemData(void);
//static void EEPROM_reset(void);

#endif /* SMARTHOME_PRIVATE_H_ */