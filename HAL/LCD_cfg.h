


#ifndef LCD_CFG_H_
#define LCD_CFG_H_

#define  RS			PINA1
#define  EN			PINA2


/*** if LCD 8 bit EIGHTBITS or 4 bit FOURBITS ***/
#define LCD_MODE     FOURBITS
/*** for LCD 8 bit enter port            ***/
#define  LCD_PORT   PA
/*** for 4 bit enter 4 pins ***/
#define  LCD_HALFPORT   PA_4TO7
#define D7	PINA6
#define D6	PINA5
#define D5	PINA4
#define D4	PINA3



#define INS_FIRST8BIT	0X38
#define INS_FIRST4BIT	0x28
#define CUR_OFF			0X0C
#define CUR_ONFIXED		0X0E
#define CUR_ONBLINK		0X0F
#define CLR_DIS			0x01
#define RIGHT_TO_LEFT	0X06
#define LEFT_TO_RIGHT	0X04


#endif /* LCD_CFG_H_ */