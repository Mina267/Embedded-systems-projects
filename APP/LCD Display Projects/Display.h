
#ifndef DISPLAY_H_
#define DISPLAY_H_
/**************************** Man and arrow cfg **********************/
#define ARROW_DELAY 300
#define MAN_DELAY   3000
/**************************** Move str cfg **********************/
#define MOVESTR_DELAY 300


/**************************** Man and arrow private **********************/
#define MAN1		1
#define MAN2		2
#define DEADMAN1	7
#define DEADMAN2	8
#define BOW1		3
#define BOW2		4
#define ARROW1		5
#define ARROW2		6
#define ARROW_CNT_START		2
#define DELETECELL_START	1
/**************************** Move str private **********************/
#define CNT_START		0
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





void ManAndArrow_Runnable(void);
void ManAndArrow_Init(void);

void PrintMoveString(c8 *str);


#endif /* DISPLAY_H_ */