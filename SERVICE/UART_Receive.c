#include "StdTypes.h"
#include "UART_Interface.h"
#include "UART_Receive.h"
#include "UART_Receive_Private.h"




/*********** Q variables **********/
static u8 Rx_Queue[QUEUE_REC_SIZE][FRAME_SIZE];
static u8 queue_in;
static u8 queue_out;
static u8 Data_cnt;




/*********** Receive Frame variables **********/
static u8 Receive_Array[FRAME_SIZE];


/******************* Q functions *************/

static Queue_Rec_status_t Queue_Rec_In (u8 *str)
{
	Queue_Rec_status_t status = Q_REC_DONE;
	if (Data_cnt == QUEUE_REC_SIZE)
	{
		status = Q_REC_FULL;
	}
	else
	{
		for (u8 FrameIndex = 0; FrameIndex < FRAME_SIZE; FrameIndex++)
		{
			Rx_Queue[queue_in][FrameIndex] = str[FrameIndex];
		}
		queue_in = (queue_in + 1) % QUEUE_REC_SIZE;
		Data_cnt++;
		
	}
	return status;
}

static Queue_Rec_status_t Queue_Rec_Out (u8 *str)
{
	Queue_Rec_status_t status = Q_REC_DONE;
	if (Data_cnt == Q_REC_EMPTY)
	{
		status = Q_REC_EMPTY;
	}
	else
	{
		for (u8 FrameIndex = 0; FrameIndex < FRAME_SIZE; FrameIndex++)
		{
			str[FrameIndex] = Rx_Queue[queue_out][FrameIndex];
		}
		queue_out = (queue_out + 1) % QUEUE_REC_SIZE;
		Data_cnt--;
	}
	return status;
}







static volatile bool_t Receive_Flag = FALSE;

static void func_Rx (void)
{
	
	static u8 i = 0;
	Receive_Array[i] = UART_ReciveNoBlock();
	i++;
	if (i == FRAME_SIZE)
	{
		Queue_Rec_In(Receive_Array);
		i = 0;
	}
	

}

void UART_Receive_Init(void)
{
	UART_RX_SetCallBack(func_Rx);
	UART_RX_InterruptEnable();
}


UART_Rec_status_t UART_Receive_GetterFrameAshync(u8* str)
{
	Queue_Rec_status_t Q_status;
	UART_Rec_status_t status = FRAME_RECEIVED;
	Q_status = Queue_Rec_Out(str);
	if (Q_status == Q_REC_EMPTY)
	{
		status = NO_FRAME_TORECIVE;
	}
	return status;
}

