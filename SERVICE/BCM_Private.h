

#ifndef BCM_PRIVATE_H_
#define BCM_PRIVATE_H_

/************* Send *****************/
typedef enum{
	Q_EMPTY,
	Q_FULL = 0,
	Q_DONE,
}Queue_status_t;

#define IN_START	0
#define OUT_START	0


static Queue_status_t Queue_In (c8 *str);
static Queue_status_t Queue_Out (c8 **str);


/************* Recieve *****************/

typedef enum{
	Q_REC_EMPTY = 0,
	Q_REC_FULL = 0,
	Q_REC_DONE,
	}Queue_Rec_status_t;


static Queue_Rec_status_t Queue_RecStr_Out (c8 *str);
static Queue_Rec_status_t Queue_RecStr_In (c8 *str);
static void Rx_Func(void);

#endif /* BCM_PRIVATE_H_ */