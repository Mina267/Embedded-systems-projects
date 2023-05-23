# define F_CPU 8000000
#include <util/delay.h>


#include "StdTypes.h"
#include "Segment.h"
#include "StopWatch.h"
#include "StopWatch_private.h"
#include "Butthon_interface.h"
#include "BUZZ_interface.h"



static u8 cnt = WATCH_DEFUALT_TIME;



void StopWatch_Init(void)
{
	/*Do Nothing*/
}



void StopWatch_Runnable(void)
{
	
	static watchStatus_t buttonStatus;
	
	for (int i = 1; i <= 10; i++)
	{
		buttonStatus = button_status();
		Segment_DisplayMLX(cnt);
	}
	
	if (cnt == TIME_FINSH && buttonStatus == RESUME)
	{
		buttonStatus = FINSH;
	}
	
	
	switch(buttonStatus)
	{
		case RESUME:
		cnt--;
		break;
		
		case START:
		cnt = WATCH_DEFUALT_TIME;
		break;

		case FINSH:
		flashBuz();
		cnt = WATCH_DEFUALT_TIME;
		buttonStatus = START;
		break;
		
		case EDIT:
		break;
		
		case PAUSE:
		break;
	}
	
}




static watchStatus_t button_status(void)
{
	static watchStatus_t Status = START;
	
	
	static bool_t PauseFlag = FALSE;
	static WatchMod_t flagMOD= MOD1;
	
	// reset --> change 1st digit
	// Button 1
	
	if (BUTTON_Read(BUTTON1) == PRESSED)
	{
		if (flagMOD == MOD1)
		{
			Status = START;
			PauseFlag = FALSE;
		}
		else
		{
			cnt = INC_TENS(cnt);
			Status = EDIT;
		}
	}

	// Pause/Resume --> change 2nd digit
	// Button 2
	
	
	if (BUTTON_Read(BUTTON2) == PRESSED)
	{
		
		if (flagMOD == MOD1)
		{
			if (PauseFlag)
			{
				Status = PAUSE;
				PauseFlag = FALSE;
			}
			else
			{
				Status = RESUME;
				PauseFlag = TRUE;
			}
		}
		else
		{
			cnt = INC_UNITS(cnt);
			Status = EDIT;
		}
	}

	
	// Mode (calibration/run)
	// Button 3
	
	if (BUTTON_Read(BUTTON3) == PRESSED && Status != RESUME)
	{
		if (flagMOD == MOD1)
		{
			flagMOD = MOD2;
		}
		else if (flagMOD == MOD2)
		{
			flagMOD = MOD1;
		}
	}
	
	
	return Status;
}



static void flashBuz(void)
{
	while (BUTTON_ReadBussy(BUTTON1) == NOT_PRESSED)
	{
		BuzzOn(); // turn on Buz.
		// write 0 in segment
		for (u8 i = 0; i < 10; i++)
		{
			Segment_DisplayMLX(0);
		}
		for (u8 i = 0; i < 10; i++)
		{
			BuzzOff();		// Turn off Buzz.
			Segment_Stop(); // Turn off Dig 1&2
			_delay_ms(10);
		}
	}
}


