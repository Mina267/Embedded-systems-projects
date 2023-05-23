
#ifndef MOVE_STEPPER_PRIVATE_H_
#define MOVE_STEPPER_PRIVATE_H_


/**************************** CELLS private **********************/
#define CELL_OUT_LCD	16
#define FIRST_LINE		0
#define SECOND_LINE		1
#define FIRST_CELL		0
#define SECOND_CELL		1
#define LAST_CELL		15
#define PRELAST_CELL	14
#define ONE_CELL		1
#define FOUR_CELL		4
#define _12TH_CELL		12
#define CELL_8_			8




static void Welcome(void);
static void menu(void);
static void takeChoice(u8 numOFchoice);
static void MoveStepper_Display(void);
static void Take_SignedDecNum(void);




#endif /* MOVE_STEPPER_PRIVATE_H_ */