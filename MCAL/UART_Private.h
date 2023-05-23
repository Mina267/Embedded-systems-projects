


#ifndef UART_PRIVATE_H_
#define UART_PRIVATE_H_



typedef union
{
	u8 UCSRC_reg;
	struct
	{
		u16 UCPOL_bit:	1;
		u16 UCSZ_bit:	2;
		u16 USBS_bit:	1;
		u16 UPM_bit:	2;
		u16 UMSEL_bit:	1;
		u16 URSEL_bit:	1;
	};
}USART_UCSRC_t;


#endif /* UART_PRIVATE_H_ */