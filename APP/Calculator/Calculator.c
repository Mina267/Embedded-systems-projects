# define F_CPU 8000000
#include <util/delay.h>

#include "StdTypes.h"
#include "LCD_interface.h"
#include "Keypad_interface.h"
#include "Calculator.h"

void Calculator_Init(void)
{
	/*DO nothing*/
}


void Calculator_Runnable(void)
{
	static u8 key;
	static u32 num = 0;
	static u32 result = 0;
	static u8 cnt = 0;
	static Math_Operations_t op = NO_OP;
	Error_type status = OK;
	
	key = KEYPAD_GetKey();
	if (key != NO_KEY)
	{
		
		
		
		// If user Enter equal and did not want to continuous in previous saved number reset
		// else do Nothing clear LCD only
		if (op == EQUAL && (key >= '0' && key <= '9'))
		{
			LCD_Clear();
			// Cnt track cell.
			cnt = 0;
			result = 0;
			op = NO_OP;
		}
		else if (op == EQUAL)
		{
			LCD_Clear();
			LCD_WriteString("ANS");
			cnt = 3;
		}
		
		
		// Max operation of every number and store in result
		if ((key < '0' || key > '9'))
		{
			// Check for OP first to make time to build number first 
			// Because user enter operation after number EX. (50 + 60)
			if (op != NO_OP)
			{
				status = Mathematical_operations(&result, num, op);
				num = 0;
				// Handle div. by Zero 
				if (status == ERROR)
				{
					LCD_SetCursor(1,0);
					LCD_WriteString("Math ERROR");
					_delay_ms(MATH_ERROR_DELAY);
					LCD_ClearCell(1 ,0, 15);
					
				}
			}
			op = key;
		}
		

		// Build first number in result to avoid problems in mult. and div.
		if ((key >= '0' && key <= '9') && op == NO_OP)
		{
			result = result * 10 + (key - '0'); 
		}
		
		
		// Build next number in operation
		if ((key >= '0' && key <= '9') && op != NO_OP)
		{
			num = num * 10 + (key - '0');
		}
		// IF user Press Equal print result in LCD 
		if (key == '=' && status != ERROR)
		{
			LCD_SetCursor(1,0);
			LCD_WriteNumber(result);
		}
		
		// Print number and operation on screen and ignore zero
		if (key != '=')
		{
			LCD_SetCursor(0, cnt);
			LCD_WriteChar(key);
		}
		cnt++;
		
		
		// when press c reset all 
		if (key == 'c')
		{
			LCD_Clear();
			cnt = 0;
			result = 0;
			op = NO_OP;
		}
	}
}




Error_type Mathematical_operations(u32 *num1, u32 num2, Math_Operations_t op)
{
	Error_type status = OK;
	switch(op)
	{
		case EQUAL:
		break;
		
		case ADD:
		*num1 += num2;
		break;
		
		case SUBS:
		*num1 -= num2;
		break;
		
		case MULT:
		*num1 *= num2;
		break;
		
		case DIV:
		if (num2 == 0)
		{
			status = ERROR;
		}
		else
		{
			*num1 /= num2;
		}
		
		break;
		
		default:
		status = OUT_OF_RANGE;
		
	}
	return status;
}


