#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"
#include "DIO_Interface.h"
#include "DIO_Private.h"
#include "DIO_cfg.h"


static void DIO_InitPin(DIO_Pin_type Pin, DIO_PinStatus_type status)
{
	DIO_Port_type port =  Pin / MAX_PINS_INPORT;
	u8 pin_num =  Pin % MAX_PINS_INPORT;
	volatile u8 *P_RegDDR = (volatile u8 *)Ports_Array[port][DDR_INDEX];
	volatile u8 *P_RegPort = (volatile u8 *)Ports_Array[port][PORT_INDEX];
	
	switch (status)
	{
		case OUTPUT:
		SET_BIT(*P_RegDDR, pin_num);
		CLR_BIT(*P_RegPort, pin_num);
		break;
		
		case INFREE:
		CLR_BIT(*P_RegDDR, pin_num);
		CLR_BIT(*P_RegPort, pin_num);
		break;
		
		case INPULL:
		CLR_BIT(*P_RegDDR, pin_num);
		SET_BIT(*P_RegPort, pin_num);
		break;
		
	}
}


 void DIO_Init(void)
{
	for (DIO_Pin_type PinIdex = FisrtPIN; PinIdex < TOTAL_PINS; PinIdex++)
	{
		DIO_InitPin(PinIdex, PinsStatusArray[PinIdex]);
	}
}



void DIO_WritePin(DIO_Pin_type Pin, DIO_PinVoltage_type  status)
{
	DIO_Port_type port =  Pin / MAX_PINS_INPORT;
	u8 pin_num =  Pin % MAX_PINS_INPORT;
	
	volatile u8 *P_Reg = (volatile u8 *)Ports_Array[port][PORT_INDEX];
	WRITE_BIT(*P_Reg, pin_num, status);
	
}

DIO_PinVoltage_type DIO_ReadPin(DIO_Pin_type Pin)
{
	DIO_Port_type port =  Pin / MAX_PINS_INPORT;
	u8 pin_num =  Pin % MAX_PINS_INPORT;
	DIO_PinVoltage_type voltage = READ_BIT(*Ports_Array[port][PIN_INDEX], pin_num);
	return voltage;
}
void DIO_TogglePin(DIO_Pin_type Pin)
{
	DIO_Port_type port =  Pin / MAX_PINS_INPORT;
	u8 pin_num =  Pin % MAX_PINS_INPORT;
	volatile u8 *P_Reg = (volatile u8 *)Ports_Array[port][PORT_INDEX];
	TOG_BIT(*P_Reg, pin_num);
}





void DIO_WritePort(DIO_Port_type port,u8 value)
{
	volatile u8 *P_Reg = (volatile u8 *)Ports_Array[port][PORT_INDEX];
	*P_Reg = value;
}



u8 DIO_ReadPort(DIO_Port_type port)
{
	u8 value = *Ports_Array[port][PIN_INDEX];
	return value;
}









