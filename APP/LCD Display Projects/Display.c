
# define F_CPU 8000000
#include <util/delay.h>

#include "StdTypes.h"
#include "LCD_interface.h"
#include "Display.h"



void ManAndArrow_Init(void)
{
	// MakeAllShapes
	// u8 arr[8] = {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x};
	// Man
	u8 arr1[8] = {0x04, 0x0e, 0x0e, 0x0e, 0x04, 0x04, 0x1f, 0x15};
	u8 arr2[8] = {0x15, 0x04, 0x04, 0x0a, 0x11, 0x00, 0x00, 0x00};
	//	bow
	u8 arr3[8] = {0x00, 0x00, 0x00, 0x00, 0x08, 0x0c, 0x0a, 0x19};
	u8 arr4[8] = {0x09, 0x1a, 0x0c, 0x08, 0x00, 0x00, 0x00, 0x00};
	// arrow
	u8 arr5[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1f};
	u8 arr6[8] = {0x1f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	// dead man (horizontal)
	u8 arr7[8] = {0x00, 0x00, 0x00, 0x01, 0x0d, 0x1f, 0x0d, 0x01};
	u8 arr8[8] = {0x00, 0x00, 0x00, 0x10, 0x0b, 0x1c, 0x02, 0x11};
	
	
	LCD_makeNewShape(arr1, MAN1);
	LCD_makeNewShape(arr2, MAN2);
	LCD_makeNewShape(arr3, BOW1);
	LCD_makeNewShape(arr4, BOW2);
	LCD_makeNewShape(arr5, ARROW1);
	LCD_makeNewShape(arr6, ARROW2);
	LCD_makeNewShape(arr7, DEADMAN1);
	LCD_makeNewShape(arr8, DEADMAN2);
}


void ManAndArrow_Runnable(void)
{
	LCD_Clear();
	u8 ArrowCnt = ARROW_CNT_START;
	u8 DeleteCell = DELETECELL_START;
	
	LCD_SetCursor(FIRST_LINE, FIRST_CELL);
	LCD_WriteChar(MAN1);
	LCD_SetCursor(SECOND_LINE, FIRST_CELL);
	LCD_WriteChar(MAN2);
	
	LCD_SetCursor(FIRST_LINE, LAST_CELL);
	LCD_WriteChar(MAN1);
	LCD_SetCursor(SECOND_LINE, LAST_CELL);
	LCD_WriteChar(MAN2);

	LCD_SetCursor(FIRST_LINE, SECOND_CELL);
	LCD_WriteChar(BOW1);
	LCD_SetCursor(SECOND_LINE, SECOND_CELL);
	LCD_WriteChar(BOW2);
	
	while (ArrowCnt < LAST_CELL)
	{
		
		LCD_SetCursor(FIRST_LINE, ArrowCnt);
		LCD_WriteChar(ARROW1);
		LCD_SetCursor(SECOND_LINE, ArrowCnt);
		LCD_WriteChar(ARROW2);
		if (DeleteCell != 1)
		{
			LCD_ClearCell(FIRST_LINE, DeleteCell, ONE_CELL);
			LCD_ClearCell(SECOND_LINE, DeleteCell, ONE_CELL);
		}
		
		
		ArrowCnt++;
		DeleteCell++;
		_delay_ms(ARROW_DELAY);
	}
	
	LCD_ClearCell(FIRST_LINE, _12TH_CELL, FOUR_CELL);
	LCD_ClearCell(FIRST_LINE, _12TH_CELL, FOUR_CELL);
	
	LCD_SetCursor(SECOND_CELL, PRELAST_CELL);
	LCD_WriteChar(DEADMAN1);
	LCD_SetCursor(SECOND_CELL, LAST_CELL);
	LCD_WriteChar(DEADMAN2);
	_delay_ms(MAN_DELAY);
}



void PrintMoveString(c8 *str)
{
	static u8 cnt = CNT_START;
	static u8 line = FIRST_LINE;
	
;
	
	LCD_WriteStringOverFlow(line ,cnt, str);
	cnt++;
	
	if(cnt == CELL_OUT_LCD)
	{
		if (line == FIRST_LINE)
		{
			line = SECOND_LINE;
		}
		else if (line == SECOND_LINE)
		{
			line = FIRST_LINE;
		}
		cnt = CNT_START;
	}
	
	
	_delay_ms(MOVESTR_DELAY);
	LCD_Clear();
}


