/*
 * Joystick.h
 *
 * Created: 4/15/2023 12:35:55 AM
 *  Author: mina2
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_


#define VX_CHANNEL	CH_0
#define VY_CHANNEL	CH_7

void Joystick_Init(void);
void Joystick_Runnable(void);
u16 Joystick_GetAngle(void);

void Joystick_Display(void);
#endif /* JOYSTICK_H_ */