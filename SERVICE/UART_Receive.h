


#ifndef UART_RECEIVE_H_
#define UART_RECEIVE_H_



#define FRAME_SIZE	20

#define QUEUE_REC_SIZE	10

typedef enum
{
	FRAME_RECEIVED,
	NO_FRAME_TORECIVE,
	}UART_Rec_status_t;

void UART_Receive_Init(void);
// If there is Frame to receive from Queue Return FRAME_RECEIVED else return NO_FRAME_TORECIVE
UART_Rec_status_t UART_Receive_GetterFrameAshync(u8* str);



#endif /* UART_RECEIVE_H_ */