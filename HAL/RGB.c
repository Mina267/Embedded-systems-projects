#include "StdTypes.h"
#include "DIO_interface.h"
#include "Timers.h"
#include "RGB_Cfg.h"
#include "RGB_Interface.h"
#include "RGB_Private.h"
#include "MemMap.h"

#include "LCD_interface.h"



static volatile bool_t EnableChange_Flag = TRUE;
static volatile bool_t ChangeColor_Flag = TRUE;
static volatile RGB_Color_type FirstColor;
static volatile RGB_Color_type SecondColor;
static u8 TmpColor[RGB_NUM];

void RGB_Init(void)
{
	TIMER0_Init(  TIMER0_PHASECORRECT_MODE, TIMER0_SCALER_8);
	TIMER0_OC0Mode(OC0_NON_INVERTING);
	OCR0 = 0;
	
	Timer1_Init(  TIMER1_PHASE_ICR_TOP_MODE, TIMER1_SCALER_8);
	Timer1_OCRA1Mode(OCRA_NON_INVERTING);
	Timer1_OCRB1Mode(OCRB_NON_INVERTING);
	ICR1 = 255;
	OCR1A = 0;
	OCR1B = 0;
	
	
	TIMER2_Init(TIMER2_NORMAL_MODE, TIMER2_SCALER_64);
	TIMER2_OV_SetCallBack(CallBackFunc_timer2_ShowRoom);
	TCNT2 = 131;
	
}



void RGB_LedColor(RGB_Color_type Color)
{
	
	RED_PWM = RGB_ColorArray[Color][RED_INDEX];
	GREEN_PWM = RGB_ColorArray[Color][GREEN_INDEX];
	BLUE_PWM = RGB_ColorArray[Color][BLUE_INDEX];
	/*
	LCD_SetCursor(0,0);
	LCD_WriteNumber(Color);
	
	LCD_SetCursor(0,0);
	LCD_WriteNumber_4D(RGB_ColorArray[Color][RED_INDEX]);
	LCD_SetCursor(0,6);
	LCD_WriteNumber_4D(RGB_ColorArray[Color][GREEN_INDEX]);
	LCD_SetCursor(0,12);
	LCD_WriteNumber_4D(RGB_ColorArray[Color][BLUE_INDEX]);
	
	LCD_SetCursor(1,0);
	LCD_WriteNumber_4D(RED_PWM);
	LCD_SetCursor(1,6);
	LCD_WriteNumber_4D(GREEN_PWM);
	LCD_SetCursor(1,12);
	LCD_WriteNumber_4D(BLUE_PWM);
	*/
}



void RGB_LedMakeColor(u8 red, u8 green, u8 blue)
{
	RED_PWM = red;
	GREEN_PWM = green;
	BLUE_PWM = blue;
}

RGB_Status RGB_ShowRoomSetColors(RGB_Color_type Color1, RGB_Color_type Color2)
{
	
	RGB_Status Status = RGB_NDONE;
	
	if (EnableChange_Flag)
	{
		RGB_LedColor(Color1);
		FirstColor = Color1;
		SecondColor = Color2;
		EnableChange_Flag = FALSE;
		
		TmpColor[RED_INDEX] = RGB_ColorArray[FirstColor][RED_INDEX];
		TmpColor[GREEN_INDEX] = RGB_ColorArray[FirstColor][GREEN_INDEX];
		TmpColor[BLUE_INDEX] = RGB_ColorArray[FirstColor][BLUE_INDEX];
		Status = RGB_DONE;
		RGB_ShowRoom_Enable();
	}
	
	return Status;
}



static void RGB_ShowRoom (void)
{
	if (ChangeColor_Flag)
	{
		bool_t IsChange_Flag = FALSE;
		
		if (TmpColor[RED_INDEX] < RGB_ColorArray[SecondColor][RED_INDEX])
		{
			TmpColor[RED_INDEX]++;
			RED_PWM = TmpColor[RED_INDEX];
			IsChange_Flag = TRUE;
		}
		else if (TmpColor[RED_INDEX] > RGB_ColorArray[SecondColor][RED_INDEX])
		{
			TmpColor[RED_INDEX]--;
			RED_PWM = TmpColor[RED_INDEX];
			IsChange_Flag = TRUE;
		}
		
		if (TmpColor[GREEN_INDEX] < RGB_ColorArray[SecondColor][GREEN_INDEX])
		{
			TmpColor[GREEN_INDEX]++;
			GREEN_PWM = TmpColor[GREEN_INDEX];
			IsChange_Flag = TRUE;
		}
		else if (TmpColor[GREEN_INDEX] > RGB_ColorArray[SecondColor][GREEN_INDEX])
		{
			TmpColor[GREEN_INDEX]--;
			GREEN_PWM = TmpColor[GREEN_INDEX];
			IsChange_Flag = TRUE;
		}
		
		if (TmpColor[BLUE_INDEX] < RGB_ColorArray[SecondColor][BLUE_INDEX])
		{
			TmpColor[BLUE_INDEX]++;
			BLUE_PWM = TmpColor[BLUE_INDEX];
			IsChange_Flag = TRUE;
		}
		else if (TmpColor[BLUE_INDEX] > RGB_ColorArray[SecondColor][BLUE_INDEX])
		{
			TmpColor[BLUE_INDEX]--;
			BLUE_PWM = TmpColor[BLUE_INDEX];
			IsChange_Flag = TRUE;
		}
		
		if (IsChange_Flag == FALSE)
		{
			EnableChange_Flag = TRUE;
			RGB_ShowRoom_Disable();
		}
		/*
		LCD_SetCursor(0,0);
		LCD_WriteNumber(TmpColor[RED_INDEX]);
		LCD_SetCursor(0,5);
		LCD_WriteNumber(TmpColor[GREEN_INDEX]);
		LCD_SetCursor(0,10);
		LCD_WriteNumber(TmpColor[BLUE_INDEX]);
		
		LCD_SetCursor(1,0);
		LCD_WriteNumber(RGB_ColorArray[SecondColor][RED_INDEX]);
		LCD_SetCursor(1,5);
		LCD_WriteNumber(RGB_ColorArray[SecondColor][GREEN_INDEX]);
		LCD_SetCursor(1,10);
		LCD_WriteNumber(RGB_ColorArray[SecondColor][BLUE_INDEX]);
		*/
		ChangeColor_Flag = FALSE;
	}
}


void CallBackFunc_timer2_ShowRoom(void)
{
	TCNT2 = 131;
	static u8 Cnt = 0;
	Cnt++;
	
	if (Cnt == SHOW_ROOM_TIME_MS / 2)
	{
		ChangeColor_Flag = TRUE;
	}
	
	RGB_ShowRoom();

	
}

void RGB_ShowRoom_Enable(void)
{
	TIMER2_OV_InterruptEnable();
}

void RGB_ShowRoom_Disable(void)
{
	TIMER2_OV_InterruptDisable();
}






RGB_Color_type RGB_LedColorIndex(c8 *StrColor)
{
	
	RGB_Color_type ColorIndex;
	for (ColorIndex = 0; ColorIndex < COLOR_NUM; ColorIndex++)
	{
		STR_Status StrStatus = Str_cmp(StrColor, (c8*)RGB_NamesColorArray[ColorIndex]);
		if (StrStatus == STR_MATCH)
		{
			return ColorIndex;
		}
	}
	return COLOR_NUM;
}


static void Str_toUpper(c8 *str)
{
	for (u8 Index = 0; str[Index]; Index++)
	{
		if (str[Index] >= 'a' && str[Index] <= 'z')
		{
			str[Index] = str[Index] - 'a' + 'A';
		}
	}
}

static STR_Status Str_cmp(c8 *strReceive, c8 *StrExpected)
{
	Str_toUpper(strReceive);
	for (u8 Index = 0; strReceive[Index] || StrExpected[Index]; Index++)
	{
		if (strReceive[Index] != StrExpected[Index])
		{
			return STR_NOMATCH;
		}
	}
	return STR_MATCH;
}









void RGB_LedColorFullRange(RGB_Color_type Color)
{
	
	if (RGB_ColorArray[Color][RED_INDEX] == 255)
	{
		TIMER0_OC0Mode(OC0_DISCONNECTED);
		DIO_WritePin(RED_PIN, HIGH);
	}
	else
	{
		TIMER0_OC0Mode(OC0_INVERTING);
		RED_PWM = 255 - RGB_ColorArray[Color][RED_INDEX];
	}
	
	
	if (RGB_ColorArray[Color][GREEN_INDEX] == 255)
	{
		Timer1_OCRA1Mode(OC0_DISCONNECTED);
		DIO_WritePin(GREEN_PIN, HIGH);
	}
	else
	{
		Timer1_OCRA1Mode(OCRA_INVERTING);
		GREEN_PWM = 255 - RGB_ColorArray[Color][GREEN_INDEX];
	}
	
	
	if (RGB_ColorArray[Color][BLUE_INDEX] == 255)
	{
		Timer1_OCRB1Mode(OC0_DISCONNECTED);
		DIO_WritePin(BLUE_PIN, HIGH);
	}
	else
	{
		Timer1_OCRB1Mode(OCRA_INVERTING);
		BLUE_PWM = 255 - RGB_ColorArray[Color][BLUE_INDEX];
		
	}
}

