#include "StdTypes.h"

#include "EX_Interrupt.h"
#include "SPI_Service_Private.h"
#include "SPI_Service.h"
#include "SPI_Interface.h"
#include "SPI_Service_Cfg.h"

# define F_CPU 8000000
#include <util/delay.h>


/********************** Receive and Send Frames using SPI Interrupt ****************************/

/****************************** Q Frame *******************************/
static volatile c8 QFrame_Rec[Q_FRAME_REC_SIZE][SPI_FRAME_SIZE];
static volatile u8 QFrame_Cnt;
static volatile u8 QFrame_In;
static volatile u8 QFrame_Out;
static volatile Q_status_t QFrame_status = Q_EMPTY;

/********************* Frame Send Receive Functions *******************/
static  u8 *SPI_SendFrame/*[SPI_FRAME_SIZE]*/;
static volatile bool_t SendFrame_Falg = FALSE;


void SPI_sendFrameInit(void)
{
	SPI_SetCallBack(Frame_SPICallBack);
	SPI_InterruptEnable();
}




void SPI_SetterSendFrame(u8 *frame)
{
	if (!SendFrame_Falg)
	{
		SPI_SendFrame = frame;
		SPI_SendNoBlock(SPI_FRAME_START_PATTERN);
		SendFrame_Falg = TRUE;
	}
}


Q_status_t SPI_ReceiveFrameGetter(u8 *FrameReceive)
{
	if (QFrame_Cnt == Q_EMPTY)
	{
		QFrame_status = Q_EMPTY;
	}
	else
	{
		u16 FrameIndex;
		// Copy Data from Queue to User Frame
		for (FrameIndex = 0; FrameIndex < SPI_FRAME_SIZE; FrameIndex++)
		{
			FrameReceive[FrameIndex] = QFrame_Rec[QFrame_Out][FrameIndex];
		}
		
		QFrame_Out = (QFrame_Out + 1) % Q_REC_SIZE;
		QFrame_Cnt--;
	}
	return QFrame_status;
}



static void Frame_SPICallBack(void)
{
	static u8 FrameIndex = 0;

	/********************** Receive Frame *******************/
	
	static s16 FrameIndex2 = 0;
	u8 DataRec = SPI_ReceiveNoBlock();
	
	static bool_t IsFrame_flag = FALSE;
	
	if (QFrame_Cnt == Q_FRAME_REC_SIZE)
	{
		QFrame_status = Q_FULL;
	}
	else if (IsFrame_flag)
	{
		if (FrameIndex2 != SPI_FRAME_SIZE )
		{
			QFrame_Rec[QFrame_In][FrameIndex2] = DataRec;
			FrameIndex2++;
		}
		if (FrameIndex2 == SPI_FRAME_SIZE)
		{
			FrameIndex2 = 0;
			QFrame_In = (QFrame_In + 1) % Q_FRAME_REC_SIZE;
			QFrame_Cnt++;
			IsFrame_flag = FALSE;
			
		}
	}
	
	if (DataRec == SPI_FRAME_START_PATTERN)
	{
		IsFrame_flag = TRUE;
	}
	
	/********************** Send Frame *******************/
	
	if (SendFrame_Falg)
	{
		_delay_us(2);
		SPI_SendNoBlock(SPI_SendFrame[FrameIndex]);
		FrameIndex++;
		if (FrameIndex == SPI_FRAME_SIZE)
		{
			SendFrame_Falg = FALSE;
			FrameIndex = 0;
			SPI_SendFrame = NULLPTR;

		}
	}
}















/**********************************************************************************************************************/


/******************** Send String *********************************/
// Master Function
void SPI_SendStringBusy(c8 *strSend)
{
	u16 StrIndex;
	for (StrIndex = 0; strSend[StrIndex]; StrIndex++)
	{
		SPI_SendReceive(strSend[StrIndex]);
	}
	
	#if SPI_1ST_STR_END != 0
	SPI_SendReceive(SPI_1ST_STR_SEND_END);
	#endif
	
	#if SPI_2ST_STR_END != 0
	SPI_SendReceive(SPI_2ST_STR_SEND_END);
	#endif
}





/************************* SPI Queue For data Received *****************************/

static c8 StrRec[Q_REC_SIZE][SPI_STR_REC_MAX_SIZE];
static u8 Qcnt;
static u8 Qin;
static u8 Qout;
static Q_status_t Qstatus = Q_EMPTY;

/***********************************************************************************/


/********************** Receive  Data using External Interrupt ****************************/

void SPI_RecStr_Init(void)
{
	EXI_Init();
	EXI_SetCallBack(EX_INT0, EXI_StrRec_callBack);
	EXI_Enable(EX_INT0);
	
}

static void EXI_StrRec_callBack(void)
{
	if (Qcnt == Q_REC_SIZE)
	{
		Qstatus = Q_FULL;
	}
	else
	{
		SPI_ReceiveStringBusy(StrRec[Qin]);
		
		Qin = (Qin + 1) % Q_REC_SIZE;
		Qcnt++;
	}
}

Q_status_t SPI_ReceiveStringGetter(c8 *Str)
{
	if (Qcnt == Q_EMPTY)
	{
		Qstatus = Q_EMPTY;
	}
	else
	{
		u16 StrIndex;
		// Copy Data from Queue to User string
		for (StrIndex = 0; StrRec[Qout][StrIndex]; StrIndex++)
		{
			Str[StrIndex] = StrRec[Qout][StrIndex];
		}
		Str[StrIndex] = 0;
		
		Qout = (Qout + 1) % Q_REC_SIZE;
		Qcnt--;
	}
	return Qstatus;
}



// Receive data Only and send Dummy
void SPI_ReceiveStringBusy(c8 *strReceive)
{
	u16 StrIndex = 0;
	u8 dataRec;
	
	dataRec = SPI_SendReceive(SPI_DUMMY);
	// Check if data receive is dummy or not
	if (dataRec == SPI_DUMMY)
	{
		strReceive[StrIndex] = 0;
	}
	else
	{
		strReceive[StrIndex] = dataRec;
	}
	
	while(strReceive[StrIndex] != SPI_1ST_STR_REC_END)
	{
		StrIndex++;
		
		dataRec = SPI_SendReceive(SPI_DUMMY);
		// Check if data receive is dummy or not
		if (dataRec == SPI_DUMMY)
		{
			strReceive[StrIndex] = 0;
		}
		else
		{
			strReceive[StrIndex] = dataRec;
		}
		
		// If Data is larger than Receive data array break.
		if (StrIndex == SPI_STR_REC_MAX_SIZE - 2)
		{
			break;
		}
	}
	// Null at end of String
	StrIndex++;
	strReceive[StrIndex] = 0;
	
	
	#if SPI_2ST_STR_REC_END != 0
	SPI_SendReceive(SPI_DUMMY);
	#endif
}





/********************** Receive and Send Data using SPI Interrupt ****************************/

static c8 *SPI_SlaveSendStr;
static bool_t Send_Falg = TRUE; 



void SPI_StringSendReveive_Init(void)
{
	SPI_SetCallBack(Str_SPICallBack);
	SPI_InterruptEnable();
}

static void Str_SPICallBack(void)
{
	static s16 StrIndex = 0;
	
	/********************** Send String *******************/
	if ((SPI_SlaveSendStr[StrIndex] == 0) && (!Send_Falg))
	{
		SPI_SendNoBlock(SPI_1ST_STR_SEND_END);
		StrIndex = 0;
		Send_Falg = TRUE;
	}
	else if (!Send_Falg)
	{
		SPI_SendNoBlock(SPI_SlaveSendStr[StrIndex]);
		StrIndex++;
	}
	else if (Send_Falg)
	{
		SPI_SendNoBlock(SPI_DUMMY);
	}
	
	
	/********************** Receive String *******************/
	
	static s16 StrIndex2 = 0;
	u8 DataRec = SPI_ReceiveNoBlock();;
	if (Qcnt == Q_REC_SIZE)
	{
		Qstatus = Q_FULL;
	}
	else if (DataRec != SPI_DUMMY)
	{
		
		if (StrRec[Qin][StrIndex2] != SPI_1ST_STR_REC_END)
		{
			StrRec[Qin][StrIndex2] = DataRec;
			
			
			if (StrIndex2 == SPI_STR_REC_MAX_SIZE - 1)
			{
				StrIndex2++;
			}
			
		}
		else
		{
			StrIndex2 = 0;
			StrRec[Qin][StrIndex2] = 0;
			Qin = (Qin + 1) % Q_REC_SIZE;
			Qcnt++;
		}


	}
	
}

u8 SPI_SendStringSetter(c8 *strSend)
{
	
	if (Send_Falg)
	{
		SPI_SlaveSendStr = strSend;
		Send_Falg = FALSE;
		return 1;
	}
	return 0;
	
}



/******************************************* Receive and Send Fixed Frame size ***********************************/

void SPI_SendReceiveFrameBusy(u8 *FrameSend, u8 *FrameReceive)
{
	u16 FrameIndex;
	u8 dataRec;
	for (FrameIndex = 0; FrameIndex < SPI_FRAME_SIZE; FrameIndex++)
	{
		dataRec = SPI_SendReceive(FrameSend[FrameIndex]);
		
		if (dataRec != SPI_DUMMY)
		{
			FrameReceive[FrameIndex] = dataRec;
		}
		else
		{
			FrameReceive[FrameIndex] = 0;
		}
	}
}


void SPI_ReceiveFrameBusy(u8 *FrameReceive)
{
	u16 FrameIndex;
	u8 dataRec;
	bool_t Correct_Frame_flag = FALSE;
	
	dataRec = SPI_SendReceive(SPI_DUMMY);
	_delay_ms(1);
	if (dataRec == SPI_FRAME_START_PATTERN)
	{
		Correct_Frame_flag = TRUE;
	}
	for (FrameIndex = 0; FrameIndex < SPI_FRAME_SIZE; FrameIndex++)
	{
		dataRec = SPI_SendReceive(0x33);
		
		if (Correct_Frame_flag)
		{
			FrameReceive[FrameIndex] = dataRec;
			
		}
		_delay_ms(1);

	}
}



void SPI_SendFrameBusy(u8 *FrameSend)
{
	u16 FrameIndex;
	for (FrameIndex = 0; FrameIndex < SPI_FRAME_SIZE; FrameIndex++)
	{
		SPI_SendReceive(FrameSend[FrameIndex]);
	}
}













