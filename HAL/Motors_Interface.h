

#ifndef MOTORS_INTERFACE_H_
#define MOTORS_INTERFACE_H_

#define  MOTOR_PIN_NUMBERS   3
#define  IN1   0
#define  IN2   1
#define  EN    2

typedef enum{
	M1,
	M2,
	M3,
	M4,
	NUMBER_MOTORS
	}MOTOR_type;


typedef enum
{
	M_CW,
	M_CCW,
	M_STOP
}Direction_status;

void MOTOR_Init(void);
void MOTOR_Stop(MOTOR_type motor);
void MOTOR_ForceStop(MOTOR_type motor);
void MOTOR_CW(MOTOR_type motor);
void MOTOR_CCW(MOTOR_type motor);

void MOTOR_Speed(MOTOR_type motor,u8 speed);
void MOTOR_Speed_dir(MOTOR_type motor,u8 speed , Direction_status status);
void MOTOR_dir(MOTOR_type motor, Direction_status status);


#endif /* MOTORS_INTERFACE_H_ */