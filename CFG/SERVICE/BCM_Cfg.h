

#ifndef BCM_CFG_H_
#define BCM_CFG_H_


/************************ Send String ******************************/
#define QUEUE_SEND_SIZE		3

// define 1st last string pattern if you did not want set it to Zero
#define _1ST_LAST_STR_PATTERN	'\n'
// define 2nd last string pattern if you did not want set it to Zero
#define _2ST_LAST_STR_PATTERN	 0


/************************ Recieve String ******************************/

#define QUEUE_RECSTR_SIZE	3
#define MAX_STR_SIZE		35

// Write the end of String you want to receive
// If you Did not want Second Pattern put Zero
// DockLight 1st 0x0D --> 2nd 0x0A
#define _1ST_STR_END_PATTERN	0x0d
#define _2ST_STR_END_PATTERN	0x00




#endif /* BCM_CFG_H_ */