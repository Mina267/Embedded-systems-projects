


#ifndef CAR_SIGNS_H_
#define CAR_SIGNS_H_




typedef enum
{
	IDLE_S,
	LEFT_S,
	RIGHT_S,
	W8_S
	}SignState_t;


typedef enum
{
	LEFT_B,
	RIGHT_B,
	W8_B,
	NO_B
	}SignTransition_t;


void CAR_Sign_Init(void);
void CAR_Sign_Runnable(void);
void CAR_Sign_Runnable2(void);

void Left(void);
void Right(void);
void Idle(void);
void W8(void);

SignTransition_t Get_Transition(void);


#endif /* CAR_SIGNS_H_ */