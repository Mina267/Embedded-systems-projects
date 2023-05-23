 


#ifndef SEGMENT_H_
#define SEGMENT_H_

/********************* Cfg *************************/
#define SEG_PORT	PA
#define DEG1_CATH	PINC6
#define DEG2_CATH	PINC7

/*BCD		MLX		DEF*/
#define SEGMENT_TYPE	MLX

/******************** Private *********************/

#define BCD	1
#define MLX 2
#define DEF	3


/**************************************************/
#if SEGMENT_TYPE == MLX
// Function has total delay 10ms
void Segment_DisplayMLX(int num);
#elif SEGMENT_TYPE == BCD
void Segment_DisplayBCD(int num);
void segment_disWith2BCD_Hex(u8 n);
#else
void segment_dis(u8 n);
#endif
void Segment_Stop(void);





#endif /* SEGMENT_H_ */