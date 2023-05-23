#include "StdTypes.h"
#include "RGB_Cfg.h"
#include "RGB_Interface.h"



#define RGB_NUM			3

const u8 RGB_ColorArray[COLOR_NUM][RGB_NUM] = 
												/* RED / GREEN / BLUE */
{			/* RED					,	*/				{255,0,	0},
			/* GREEN				,	*/				{0,255,0},
			/* BLUE					,	*/				{0,0,255},				
			/* BLACK				,	*/				{0,0,0},				
			/* WHITE				,	*/				{255,255,255},				
			/* YELLOW				,	*/				{255,255,0},				
			/* CYAN					,	*/				{0,255,255},				
			/* MAGENTA				,	*/				{255,0,	255},				
			/* SILVER				,	*/				{192,192,192},				
			/* GRAY					,	*/				{128,128,128},				
			
			/*
			/ * MAROON				,	* /				{128,0,0},				
			/ * OLIVE				,	* /				{128,128,0},				
			/ * PURPLE				,	* /				{128,0,128},				
			/ * TEAL					,	* /				{0,128,128},				
			/ * NAVY					,	* /				{0,0,128},
			/ *CRIMSON				,	* /				{220,20,60  },
			/ *TOMATO				,	* / 				{255,99,71  },
			/ *CORAL		 			,	* /				{255,127,80 },
			/ *IVORY					,	* /				{255,255,240},
			/ *AZURE					,	* /				{240,255,255},	
			/ *LIGHTBLUE				,	* /				{173,216,230},
			/ *SKYBLUE				,	* /				{135,206,235},
			/ *ORANGE				,	* /				{255,165,0  },
			/ *GOLD					,	* /				{255,215,0  },
			/ *PINK					,	* /				{255,51,153},
			/ *PLUM					,	* /				{221,160,221},
			/ *VIOLET				,	* /				{238,130,238},
			/ *SALMON				,	* /				{250,128,114},
			/ *LAVENDERBLUSH			,	* /				{255,240,245},
			/ *LINEN					,	* /				{250,240,230},
			/ *OLDLACE				,	* /				{253,245,230},	
			/ *LAVENDER				,	* /				{230,230,250},
			/ *ROYALBLUE				,	* /				{65,105,225},
			/ *BLUEVIOLET			,	* /				{138,43,226},
			/ *INDIGO				,	* /				{75,0,130},
			/ *DARKSLATEBLUE			,	* /				{72,61,139},
			/ *SLATEBLUE				,	* /				{106,90,205},
			/ *MAGENTA/FUCHSIA		,	* /				{255,0,255},
			/ *ORCHID				,	* /				{218,112,214},
			/ *LIGHT_CORA			,	* /				{240,128,128},
			/ *DARK_SALMON			,	* /				{233,150,122},
			/ *LIGHT_SALMON			,	* /				{255,160,122},
			/ *ORANGE_RED			,	* /				{255,69,0   },
			/ *DARK_ORANGE			,	* /				{255,140,0  },
			/ *DARK_GOLDEN_ROD		,	* /				{184,134,11 },
			/ *GOLDEN_ROD			,	* /				{218,165,32 },
			/ *PALE_GOLDEN ROD		,	* /				{238,232,170},
			/ *DARK_KHAKI			,	* /				{189,183,107},
			/ *INDIAN_RED			,	* /				{205,92,92  },
			/ *KHAKI					,	* /				{240,230,140},
			/ *YELLOW GREEN			,	* /				{154,205,50} ,
			/ *DARK OLIVE GREEN		,	* /				{85,107,47 } ,
			/ *OLIVE_DRAB			,	* /				{107,142,35} ,
			/ *LAWN_GREEN			,	* /				{124,252,0 } ,
			/ *CHARTREUSE			,	* /				{127,255,0 } ,
			/ *GREEN_YELLOW			,	* /				{173,255,47} ,
			/ *LIGHTCYAN				,	* /				{224,255,255},
			/ *DARKTURQUOISE			,	* /				{0,206,209},
			/ *TURQUOISE				,	* /				{64,224,208},
			/ *MEDIUMTURQUOISE		,	* /				{72,209,204},
			/ *PALETURQUOISE			,	* /				{175,238,238},
			/ *AQUAMARINE			,	* /				{127,255,212},
			/ *POWDERBLUE			,	* /				{176,224,230},
			/ *CADETBLUE				,	* /				{95,158,160},
			/ *STEELBLUE				,	* /				{70,130,180},
			/ *CORNFLOWERBLUE		,	* /				{100,149,237},
			/ *DEEPSKYBLUE			,	* /				{0,191,255},
			/ *DODGERBLUE			,	* /				{30,144,255},
			/ *LIGHTSKYBLUE			,	* /				{135,206,250},
			/ *MIDNIGHTBLUE			,	* /				{25,25,112},
			/ *DARKBLUE				,	* /				{0,0,139},
			/ *MEDIUMBLUE			,	* /				{0,0,205},
			/ *MEDIUMSLATEBLUE		,	* /				{123,104,238},
			/ *MEDIUMPURPLE			,	* /				{147,112,219},
			/ *DARKMAGENTA			,	* /				{139,0,139},
			/ *DARKVIOLET			,	* /				{148,0,211},
			/ *DARKORCHID			,	* /				{153,50,204},
			/ *MEDIUMORCHID			,	* /				{186,85,211},
			/ *THISTLE				,	* /				{216,191,216},
			/ *MEDIUMVIOLETRED		,	* /				{199,21,133},
			/ *PALEVIOLETRED			,	* /				{219,112,147},
			/ *DEEPPINK				,	* /				{255,20,147},
			/ *HOTPINK				,	* /				{255,105,180},
			/ *LIGHTPINK				,	* /				{255,182,193},
			/ *ANTIQUEWHITE			,	* /				{250,235,215},
			/ *BEIGE					,	* /				{245,245,220},
			/ *BISQUE				,	* /				{255,228,196},
			/ *BLANCHEDALMOND		,	* /				{255,235,205},
			/ *WHEAT					,	* /				{245,222,179},
			/ *CORNSILK				,	* /				{255,248,220},
			/ *LEMONCHIFFON			,	* /				{255,250,205},
			/ *LIGHTGOLDENRODYELLOW	,	* /				{250,250,210},
			/ *LIGHTYELLOW			,	* /				{255,255,224},
			/ *SADDLEBROWN			,	* /				{139,69,19},
			/ *SIENNA				,	* /				{160,82,45},
			/ *CHOCOLATE				,	* /				{210,105,30},
			/ *PERU					,	* /				{205,133,63},
			/ *SANDYBROWN			,	* /				{244,164,96},
			/ *BURLYWOOD				,	* /				{222,184,135},
			/ *TAN					,	* /				{210,180,140},
			/ *ROSYBROWN				,	* /				{188,143,143},
			/ *MOCCASIN				,	* /				{255,228,181},
			/ *NAVAJOWHITE			,	* /				{255,222,173},
			/ *PEACHPUFF				,	* /				{255,218,185},
			/ *MISTYROSE				,	* /				{255,228,225},
			/ *PAPAYAWHIP			,	* /				{255,239,213},
			/ *SEASHELL				,	* /				{255,245,238},
			/ *MINTCREAM				,	* /				{245,255,250},
			/ *SLATEGRAY				,	* /				{112,128,144},
			/ *LIGHTSLATEGRAY		,	* /				{119,136,153},
			/ *LIGHTSTEELBLUE		,	* /				{176,196,222},
			/ *FLORALWHITE			,	* /				{255,250,240},
			/ *ALICEBLUE				,	* /				{240,248,255},
			/ *GHOSTWHITE			,	* /				{248,248,255},
			/ *HONEYDEW				,	* /				{240,255,240},
			*/
			
						
							};
							
							



const c8 *RGB_NamesColorArray[COLOR_NUM] =
{
	 "RED",
	 "GREEN",
	 "BLUE"	,
	 "BLACK",
	 "WHITE",
	 "YELLOW",
	 "CYAN"	,
	 "MAGENTA",
	 "SILVER",
	 "GRAY"	,


} ;