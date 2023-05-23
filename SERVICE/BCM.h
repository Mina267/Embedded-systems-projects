

#ifndef BCM_H_
#define BCM_H_


typedef enum
{
	STR_TAKE,
	STR_NTAKE,
	}BCM_status_t;



void BCM_SendStringInit(void);
BCM_status_t BCM_SendStringSetter(c8* str);
void BCM_SendStringRunnable(void);




typedef enum
{
	BCM_STR_RECEVIED,
	BCM_NO_STR_TORECIVE,
	
}BCM_RecStr_status_t;




void BCM_ReceiveStringAshync_Init(void);
BCM_RecStr_status_t BCM_ReceiveStringAshync(c8* str);




#endif /* BCM_H_ */