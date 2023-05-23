

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_



typedef enum
{
	RISINGXCK_EDGE, 
	FALLINGGXCK_EDGE
	}UART_ClockPolarity_t;

typedef enum
{
	_5_BIT,
	_6_BIT,
	_7_BIT,
	_8_BIT,
	_9_BIT = 7, 
	}UART_CharacterSize_t;


typedef enum
{
	STOP_1_BIT,
	STOP_2_BIT,

	}UART_StopBitSelect_t;

typedef enum
{
	
	DISABLE_PARITY,
	EVEN_PARITY = 2,
	ODD_PARITY,
	}UART_ParityModes_t;



typedef enum
{	
	ASYNCHRONOUS,
	SYNCHRONOUS,
	}UART_ModeSelect_t;




typedef struct
{
	UART_ModeSelect_t	mode;
	UART_ParityModes_t	parity;
	UART_StopBitSelect_t Stop;
	UART_CharacterSize_t charSize;
	UART_ClockPolarity_t clk;
	}USART_Control_StatusRegister_C_t;

 
 
typedef enum
{
	_2400_BAUDRATE = 416,
	_4800_BAUDRATE = 207,
	_9600_BAUDRATE = 103,
	_14400_BAUDRATE = 68,
	_19200_BAUDRATE = 51,
	_28800_BAUDRATE = 34,
	}BaudRate_Select_t; 
 
void UART_Init2(USART_Control_StatusRegister_C_t *Pstatus, u8 BaudRate);

void UART_Init(BaudRate_Select_t BaudRate);

void UART_SendData(u8 data);
u8 UART_ReciveData(void);
u8 UART_ReciveDataPerodic(u8 *pdata);
void UART_SendNoBlock(u8 data);
u8 UART_ReciveNoBlock(void);
void UART_RX_InterruptEnable(void);
void UART_RX_InterruptDisable(void);
void UART_RX_SetCallBack(void (*LocalFptr)(void));
void UART_TX_InterruptEnable(void);
void UART_TX_InterruptDisable(void);
void UART_TX_SetCallBack(void (*LocalFptr)(void));


#endif /* UART_INTERFACE_H_ */