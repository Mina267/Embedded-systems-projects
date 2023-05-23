

#ifndef SMARTHOME_H_
#define SMARTHOME_H_


typedef struct
{
	c8 *str;
	void (*Fptr) (u16);
	}SmartHome_task_t;


void SmartHome_Init(void);
void SmartHome_Runnable(void);


#endif /* SMARTHOME_H_ */