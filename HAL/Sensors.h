

#ifndef SENSORS_H_
#define SENSORS_H_

#define LM35_CH		CH_0
#define MPX4115_CH	CH_7
#define LDR_CH		CH_0

u16 LIGHT_Read(void);
// Return temperature in C*10.
u16 TEMP_Read(void);
// Return press in KPA * 10.
u16 PRESS_Read(void);



void TEMP_Convertion(void);
u16 TEMP_GetRead(void);

// Return press in KPA * 10.
void PRESS_Conversion(void);
u16 PRESS_GetRead(void);

#endif /* SENSORS_H_ */