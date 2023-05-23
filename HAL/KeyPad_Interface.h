

#ifndef KEYPAD_INTERFACE_H_
#define KEYPAD_INTERFACE_H_


#define COLS	4
#define ROWS	4


#ifdef _KEYSARRAY
static const u8 KeysArray[COLS][ROWS] =   {	{'7', '8', '9', 'S'},
											{'4', '5', '6', 'y'},
											{'1', '2', '3', '-'},
											{'c', '0', '=', 'm'},
	};
#endif // _KEYSARRAY

#define FIRST_OUTPUT	PINB4		// KIT Keypad PINB4	
#define FIRST_INPUT		PIND2		// KIT Keypad PIND2
#define NO_KEY			'`'
	
void KEYPAD_Init(void);
u8 KEYPAD_GetKey(void);


#endif /* KEYPAD_INTERFACE_H_ */