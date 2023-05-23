

#ifndef SPI_SERVICE_H_
#define SPI_SERVICE_H_


typedef enum
{
	Q_EMPTY,
	Q_FULL,
	Q_DONE,
	}Q_status_t;


/******************** Send String *********************************/

void SPI_SendStringBusy(c8 *strSend);

/********************** Receive  Data using External Interrupt ****************************/

void SPI_RecStr_Init(void);
// Receive data Only and send Dummy
void SPI_ReceiveStringBusy(c8 *strReceive);
/********************** Receive and Send Data using SPI Interrupt ****************************/

void SPI_StringSendReveive_Init(void);
u8 SPI_SendStringSetter(c8 *strSend);


/******************************************* Receive and Send Fixed Frame size ***********************************/
// Receive and Send Fixed frame size
void SPI_SendReceiveFrameBusy(u8 *FrameSend, u8 *FrameReceive);
void SPI_ReceiveFrameBusy(u8 *FrameReceive);
void SPI_SendFrameBusy(u8 *FrameSend);

/********************** Receive and Send Frames using SPI Interrupt ****************************/
void SPI_SetterSendFrame(u8 *frame);
void SPI_sendFrameInit(void);
Q_status_t SPI_ReceiveFrameGetter(u8 *FrameReceive);

#define SPI_FRAME_START_PATTERN		0x55
#define SPI_DUMMY					0XA5

#endif /* SPI_SERVICE_H_ */