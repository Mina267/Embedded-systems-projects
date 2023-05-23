


#ifndef SMARTHOME_CFG_H_
#define SMARTHOME_CFG_H_


/**************************** Task ********************************************/

// Temperature in Celsius * 10
#define OPERATE_TEMP				330
// Temperature C * 10
#define MIN_TEMP_STOP_OPERATE		180
// Speed in percentage
#define DEFAULT_FAN_SPEED			80
// Speed in percentage
#define ZERO_SPEED					0
// Temperature in Celsius
#define DEFAULT_AIRCONITION_DGREE	24
// Pin control out door leds
#define OUTDOORLEDS_PIN				PINC6

#define MIN_LIGHT		2500
#define MAX_LIGHT		3000



/**********  relays. *************************/
#define _1_GRAGEDOOR_RELAY			_1_RELAY
#define _2_WINDOW_CURTION_RELAY		_2_RELAY

/******** Motors *****************************/
#define GRAGEDOOR_MOTOR			M2
#define WINDOW_CURTION_MOTOR	M3
#define FAN_MOTOR				M1

/********** SPI Frame ************************/
#define SMARTHOME_FRAME_SIZE	4
#define SPI_FRAME_TIME			200
/********** SPI Motor Status bits ***********/
#define MOTORS_STATUS			3
#define FAN						0
#define GRAGEDOOR				1
#define WINDOW_CURTION			2
#define AIRCONDITION			3
#define TEMPCONTROL				4
#define OUTDOORLEDS				5
#define LDR_CONTROL				6


#endif /* SMARTHOME_CFG_H_ */