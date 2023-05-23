

#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#define MATH_ERROR_DELAY	2000

typedef enum
{
	NO_OP,
	MULT = '*',
	DIV = '/',
	ADD = '+',
	SUBS = '-',
	EQUAL = '=',
}Math_Operations_t;


void Calculator_Init(void);
void Calculator_Runnable(void);

Error_type Mathematical_operations(u32 *num1, u32 num2, Math_Operations_t op);


#endif /* CALCULATOR_H_ */