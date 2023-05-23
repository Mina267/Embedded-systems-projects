

#ifndef KEYPAD_CFG_H_
#define KEYPAD_CFG_H_



#define  FIRST_OUTPUT   PINB4
#define  FIRST_INPUT	PIND2




#define  ROWS   4
#define  COLS   4



const u8 KeysArray[ROWS][COLS]=

/*

{
	{'7', '8', '9', 'X'},
	{'4', '5', '6', 'D'},
	{'1', '2', '3', 'N'},
	{'S', '0', 'K', 'N'}
};

*/

/*

{
	{'7', '8', '9', '/'},
	{'4', '5', '6', '*'},
	{'1', '2', '3', '-'},
	{'c', '0', '=', '+'}	
};
*/


{
	{'7', '8', '9', 'D'},
	{'4', '5', '6', 'N'},
	{'1', '2', '3', '-'},
	{'c', '0', '=', '+'}	
};


#endif /* KEYPAD_CFG_H_ */