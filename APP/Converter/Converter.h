
#ifndef CONVERTER_H_
#define CONVERTER_H_


typedef enum
{
	NO_CHOICE,
	FIRST,
	SECOND,
	THIRD,
	}Converter_choice_type;
	
void Converter_Init(void);
void Converter_Runnable(void);

#endif /* CONVERTER_H_ */