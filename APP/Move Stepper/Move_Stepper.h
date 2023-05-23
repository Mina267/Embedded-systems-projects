
#ifndef MOVE_STEPPER_H_
#define MOVE_STEPPER_H_



#define INPUTDONE		'='
#define DELETE			'D'
#define START_AGAIN		'c'
#define MINUS			'-'

#define BUZPIN PINC5

typedef enum
{
	MOVE_NO_CHOICE,
	MOVE_FIRSTCHOICE,
	MOVE_SECONDCHOICE,
	MOVE_THIRDCHOICE,
}MoveStepper_choice_type;

void Move_Stepper_Init(void);
void Move_Stepper_Runnable(void);

#endif /* MOVE_STEPPER_H_ */