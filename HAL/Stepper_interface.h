
#ifndef STEPPER_INTERFACE_H_
#define STEPPER_INTERFACE_H_


typedef enum
{
	CCW,
	CW,
	
	}Stepper_Dir_type;
	
/****************************  Stepper cfg ****************************/

#define COIL1A  PINC0
#define COIL1B  PINC1
#define COIL2A  PINC2
#define COIL2B  PINC3
#define DELAY	3  // minimum 3 ms

/*********************************/

#define STEP_PERONE_ROT 512

/*********************************/

void Stepper_Init(void);

void Stepper_Pipolar_CW(void);
void Stepper_Pipolar_CCW(void);

void Stepper_Unipolar_CW(void);
void Stepper_Unipolar_CCW(void);

void Stepper_Unipolar_CW_HS(void);
void Stepper_Unipolar_CCW_HS(void);

void Stepper_ByAngle(u16 angle, Stepper_Dir_type dir);
void Stepper_ByRotate(u16 rot, Stepper_Dir_type dir);

void Stepper_ByAngle(u16 angle, Stepper_Dir_type dir);
void Stepper_ByAngle_HS(u16 angle, Stepper_Dir_type dir);
void Stepper_ByRotate(u16 rot, Stepper_Dir_type dir);
void Stepper_ByRotate_HS(u16 rot, Stepper_Dir_type dir);

void Stepper_AngleSetter(u16 angle);
u8 Stepper_AngleRunnable(void);
void Stepper_PosMatch(void);
#endif /* STEPPER_INTERFACE_H_ */