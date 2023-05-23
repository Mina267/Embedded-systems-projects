

#ifndef DIO_PRIVATE_H_
#define DIO_PRIVATE_H_


#define MAX_PINS_INPORT 8
#define FisrtPIN		0
#define PORT_OFFSET(port) ((3 - port) * 3)

extern const DIO_PinStatus_type PinsStatusArray[TOTAL_PINS];
extern const volatile u8 *Ports_Array[][3];
static void DIO_InitPin(DIO_Pin_type Pin, DIO_PinStatus_type status);


#endif /* DIO_PRIVATE_H_ */