
#ifndef CONVERTER_PRIVATE_H_
#define CONVERTER_PRIVATE_H_


#define WRITEBIN_LAST(NUM, BIN)		(NUM = ((NUM) << 1) | (((NUM << 1) | (BIN)) & 0X1))
#define WRITEHEX(NUM, HEX)			(NUM = ((NUM) << 4) | ((((NUM) << 4) | (HEX)) & (0XF)))


#define WELCOME_DELAY			2000
#define _1ST_NUMBER_OF_CHOICE	3
#define _2ST_NUMBER_OF_CHOICE	2
#define MAX_8BYTES				8


/**************************** CELLS private **********************/
#define CELL_OUT_LCD	16
#define FIRST_LINE		0
#define SECOND_LINE		1
#define FIRST_CELL		0
#define SECOND_CELL		1
#define LAST_CELL		15



static u32 TakeDec(u8 line, u8 cell);
static u8 Select_hex(u8 line, u8 cell);
static Converter_choice_type takeChoice(u8 numOFchoice);
static u32 TakeHEX(u8 line, u8 cell);
static u32 TakeBin(u8 line, u8 cell);
static void Welcome(void);
static void menu(void);
static void convertionDisplay(void);
static void display_num(u32 num);


#endif /* CONVERTER_PRIVATE_H_ */