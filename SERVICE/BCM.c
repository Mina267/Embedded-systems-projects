#include "StdTypes.h"
#include "UART_Interface.h"
#include "BCM.h"
#include "BCM_Private.h"
#include "BCM_Cfg.h"


/*********** Q variables **********/ 
static c8 *Tx_Queue[QUEUE_SEND_SIZE];
static u8 queue_in  = IN_START;
static u8 queue_out = OUT_START;
static u8 Data_cnt;
/*********** Send string variables **********/
static c8 *Tx_Str;
static volatile bool_t Sending_Flag = FALSE;
static volatile bool_t Tx_flag = TRUE;



/******************* Q functions *************/ 

static Queue_status_t Queue_In (c8 *str)
{
	Queue_status_t status = Q_DONE;
	if (Data_cnt == QUEUE_SEND_SIZE)
	{
		status = Q_FULL;
	}
	else
	{
		Tx_Queue[queue_in] = str;
		queue_in = (queue_in + 1) % QUEUE_SEND_SIZE;
		Data_cnt++;
	}
	return status;
}

static Queue_status_t Queue_Out (c8 **str)
{
	Queue_status_t status = Q_DONE;
	if (Data_cnt == Q_EMPTY)
	{
		status = Q_EMPTY;
	}
	else
	{
		*str = Tx_Queue[queue_out];
		queue_out = (queue_out + 1) % QUEUE_SEND_SIZE;
		Data_cnt--;
	}
	return status;
}






/*********** Send string functions **********/


static void BCM_func_tx(void)
{
	Tx_flag = TRUE;
	
}



void BCM_SendStringInit(void)
{
	UART_TX_SetCallBack(BCM_func_tx);
	UART_TX_InterruptEnable();
}


BCM_status_t BCM_SendStringSetter(c8* str)
{
	Queue_status_t Q_status;
	BCM_status_t status = STR_TAKE;
	Q_status = Queue_In(str);
	if (Q_status == Q_FULL)
	{
		status = STR_NTAKE;
	}	
	
	return status;
}



void BCM_SendStringRunnable(void)
{
	
	static bool_t last_flag = FALSE;
	Queue_status_t Q_status;
	if (!Sending_Flag)
	{
		Q_status = Queue_Out(&Tx_Str);
		if (Q_status != Q_EMPTY)
		{
			Sending_Flag = TRUE;
			Tx_flag = TRUE;
		}
	}

	static u8 i = 0;
	if (Tx_flag)
	{
		if (Tx_Str[i] != 0)
		{
			
			UART_SendNoBlock(Tx_Str[i]);
			i++;
			last_flag = TRUE;
			
		}
		#ifdef _2ST_LAST_STR_PATTERN 
		else if (last_flag)
		{
			if (_1ST_LAST_STR_PATTERN != 0)
			{
				UART_SendNoBlock(_1ST_LAST_STR_PATTERN);
			}
			if (_2ST_LAST_STR_PATTERN != 0)
			{
				UART_SendNoBlock(_2ST_LAST_STR_PATTERN);
				
			}

			last_flag = FALSE;
		}
		#endif
		else
		{
			i = 0;
			Sending_Flag = FALSE;
		}
		
		Tx_flag = FALSE;
		
	}
	
}





/**************************************** UART Receive Strings *************************************/




/*********** Q variables **********/
static c8 Rx_QueueStr[QUEUE_RECSTR_SIZE][MAX_STR_SIZE];
static u8 queueStr_in;
static u8 queueStr_out;
static u8 DataStr_cnt;



/*********** Receive string variables **********/
static c8 ReceiveStr_Array[MAX_STR_SIZE];
static u8 StrFinish1;
static u8 StrFinish2;
static volatile bool_t StrFinish_flag = FALSE;
static volatile bool_t StrFinish2_flag = FALSE;


/******************* Q functions *************/

static Queue_Rec_status_t Queue_RecStr_In (c8 *str)
{
	Queue_Rec_status_t status = Q_REC_DONE;
	if (DataStr_cnt == QUEUE_RECSTR_SIZE)
	{
		status = Q_REC_FULL;
	}
	else
	{
		u8 StrIndex;
		for (StrIndex = 0; StrIndex < MAX_STR_SIZE && str[StrIndex]; StrIndex++)
		{
			Rx_QueueStr[queueStr_in][StrIndex] = str[StrIndex];
		}
		Rx_QueueStr[queueStr_in][StrIndex] = 0;
		queueStr_in = (queueStr_in + 1) % QUEUE_RECSTR_SIZE;
		DataStr_cnt++;
		
	}
	return status;
}

static Queue_Rec_status_t Queue_RecStr_Out (c8 *str)
{
	Queue_Rec_status_t status = Q_REC_DONE;
	if (DataStr_cnt == Q_REC_EMPTY)
	{
		status = Q_REC_EMPTY;
	}
	else
	{
		u8 StrIndex;
		for (StrIndex = 0; StrIndex < MAX_STR_SIZE && Rx_QueueStr[queueStr_out][StrIndex]; StrIndex++)
		{
			str[StrIndex] = Rx_QueueStr[queueStr_out][StrIndex]; 
		}
		str[StrIndex] = 0;
		queueStr_out = (queueStr_out + 1) % QUEUE_RECSTR_SIZE;
		DataStr_cnt--;
	}
	return status;
}



/**************** Receive string Detect end of string by pattern ******************/



static void Rx_Func(void)
{
	
	static s16 i = 0;
	ReceiveStr_Array[i] = UART_ReciveNoBlock();
	
	if ((ReceiveStr_Array[i] == StrFinish1) || (StrFinish2_flag == TRUE))
	{
		
		if (ReceiveStr_Array[i] == StrFinish1)
		{
			ReceiveStr_Array[i] = 0;
		}
		
		if ((StrFinish2 == 0) || (ReceiveStr_Array[i] == StrFinish2))
		{
			StrFinish_flag = TRUE;
			i = -1;
		}
		else
		{
			StrFinish2_flag = TRUE;
		}
		
	}
	
	if (i >= MAX_STR_SIZE - 2)
	{
		ReceiveStr_Array[i] = 0;
		StrFinish_flag = TRUE;
		i = -1;
	}
	
	
	i++;
	
	
	
	if (StrFinish_flag)
	{
		Queue_RecStr_In(ReceiveStr_Array);
		StrFinish_flag = FALSE;
		StrFinish2_flag = FALSE;
	}
	
	
	
}

// Take str finish
void BCM_ReceiveStringAshync_Init(void)
{
	UART_RX_SetCallBack(Rx_Func);
	UART_RX_InterruptEnable();
	StrFinish1 = (u8)_1ST_STR_END_PATTERN;
	StrFinish2 = (u8)_2ST_STR_END_PATTERN;
	
}


BCM_RecStr_status_t BCM_ReceiveStringAshync(c8* str)
{
	
	Queue_Rec_status_t Q_Recstatus;
	BCM_RecStr_status_t status =  BCM_STR_RECEVIED;
	Q_Recstatus = Queue_RecStr_Out(str);
	if (Q_Recstatus == Q_REC_EMPTY)
	{
		status = BCM_NO_STR_TORECIVE;
	}
	return status;
}




