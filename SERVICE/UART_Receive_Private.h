

#ifndef UART_RECEIVE_PRIVATE_H_
#define UART_RECEIVE_PRIVATE_H_


typedef enum{
	Q_REC_EMPTY,
	Q_REC_FULL = 0,
	Q_REC_DONE,
	}Queue_Rec_status_t;


static Queue_Rec_status_t Queue_Rec_In (u8 *str);
static Queue_Rec_status_t Queue_Rec_Out (u8 *str);



#endif /* UART_RECEIVE_PRIVATE_H_ */