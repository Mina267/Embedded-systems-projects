

#ifndef RGB_INTERFACE_H_
#define RGB_INTERFACE_H_

typedef enum
{
	 RED					,
	 GREEN				,
	 BLUE					,
	 BLACK				,
	 WHITE				,
	 YELLOW				,
	 CYAN					,
	 MAGENTA				,
	 SILVER				,
	 GRAY					,
	COLOR_NUM,
	}RGB_Color_type;

typedef enum
{
	RGB_NDONE,
	RGB_DONE
	}RGB_Status;
	
void RGB_Init(void);
void RGB_LedColor(RGB_Color_type Color);
void RGB_LedColorFullRange(RGB_Color_type Color);
RGB_Status RGB_ShowRoomSetColors(RGB_Color_type Color1, RGB_Color_type Color2);
RGB_Color_type RGB_LedColorIndex(c8 *StrColor);
void RGB_ShowRoom_Enable(void);
void RGB_ShowRoom_Disable(void);

#endif /* RGB_INTERFACE_H_ */