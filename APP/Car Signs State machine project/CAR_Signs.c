
#include "StdTypes.h"
#include "Keypad_interface.h"
#include "CAR_Signs.h"
#include "LEDS.h"

static SignState_t Curr_State = IDLE_S;
static SignState_t Prev_State = IDLE_S;
static SignTransition_t trans;



void CAR_Sign_Init(void)
{
	KEYPAD_Init();
}


void (*system_Array[4][3]) (void) = 
{
					/* Left_B	right_B		W8_B*/
	/* Idle state  */{Left, 	 Right ,	 W8},
	/* left state  */{Left,      Idle,		 W8},
	/* right state */{Idle,      Right,      W8},
	/* W8 state    */{W8,		 W8,		 W8},	
} 
;

// Runnable using array of pointer to functions.
void CAR_Sign_Runnable2(void)
{
	trans = Get_Transition();
	if (trans != NO_B)
	{
		system_Array[Curr_State][trans]();
	}
	
}



// Runnable using Switch method
void CAR_Sign_Runnable(void)
{
	trans = Get_Transition();
	
	switch (Curr_State)
	{
		case IDLE_S:
		switch (trans)
		{
			case LEFT_B:
			Left();
			break;
			
			case RIGHT_B:
			Right();
			break;
			
			case W8_B:
			Prev_State = IDLE_S;
			W8();
			break;
			
			case NO_B:
			break;
		}
		break;
		
		case LEFT_S:
		switch (trans)
		{
			case LEFT_B:
			Left();
			break;
			
			case RIGHT_B:
			Idle();
			break;
			
			case W8_B:
			Prev_State = LEFT_S;
			W8();
			break;
			
			case NO_B:
			break;
		}
		break;
		
		case RIGHT_S:
		switch (trans)
		{
			case LEFT_B:
			Idle();
			break;
			
			case RIGHT_B:
			Right();
			break;
			
			case W8_B:
			Prev_State = RIGHT_S;
			W8();
			break;
			
			case NO_B:
			break;
		}
		break;
		
		case W8_S:
		switch (trans)
		{
			case LEFT_B:
			Prev_State = LEFT_S;
			break;
			
			case RIGHT_B:
			Prev_State = RIGHT_S;
			break;
			
			case W8_B:
			if (Prev_State == IDLE_S)
			{
				Idle();
			}
			else if (Prev_State == LEFT_S)
			{
				Left();
			}
			else if (Prev_State == RIGHT_S)
			{
				Right();
			}
			
			break;
			
			case NO_B:
			break;
		}
		break;
	}
}


void Idle(void)
{
	Curr_State = IDLE_S;
	
	LEDS_Off(BLU_LED);
	LEDS_Off(GRN_LED);
	
}

void Left(void)
{
	Curr_State = LEFT_S;
	
	LEDS_On(BLU_LED);
	LEDS_Off(GRN_LED);
	
}
void Right(void)
{
	Curr_State = RIGHT_S;
	
	LEDS_Off(BLU_LED);
	LEDS_On(GRN_LED);
	
}

void W8(void)
{
	if (Curr_State != W8_S)
	{
		Prev_State = Curr_State;
		Curr_State = W8_S;
		LEDS_On(BLU_LED);
		LEDS_On(GRN_LED);
	}
	else if (Curr_State == W8_S)
	{
		if (trans == LEFT_B)
		{
			Prev_State = LEFT_S;
		}
		else if (trans == RIGHT_B)
		{
			Prev_State = RIGHT_S;
		}
		else if (trans == W8_B)
		{
			if (Prev_State == IDLE_S)
			{
				Idle();
			}
			else if (Prev_State == LEFT_S)
			{
				Left();
			}
			else if (Prev_State == RIGHT_S)
			{
				Right();
			}
		}
	}
	
	
	
	
}

SignTransition_t Get_Transition(void)
{
	u8 key = KEYPAD_GetKey();
	SignTransition_t T = NO_B;
	if(key != NO_KEY)
	{
		if (key == '1')
		{
			T = LEFT_B;
		}
		else if (key == '2')
		{
			T = RIGHT_B;
		}
		else if (key == '3')
		{
			T = W8_B;
		}
	}
	
	return T;
}