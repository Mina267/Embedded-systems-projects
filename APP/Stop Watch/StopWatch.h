
#ifndef STOPWATCH_H_
#define STOPWATCH_H_


/************************************ Config Stop Watch *********************************/
#define WATCH_DEFUALT_TIME	30
#define TIME_FINSH			0

typedef enum
{
	MOD1,
	MOD2,
}WatchMod_t;

typedef enum
{
	RESUME,
	START,
	FINSH,
	EDIT,
	PAUSE,
}watchStatus_t;


/*************** Stop watch *********************/

void StopWatch_Init(void);
void StopWatch_Runnable(void);






#endif /* STOPWATCH_H_ */