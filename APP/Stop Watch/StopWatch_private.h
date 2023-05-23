
#ifndef STOPWATCH_PRIVATE_H_
#define STOPWATCH_PRIVATE_H_


#define INC_UNITS(NUM)		(((cnt) / 10) * 10) + ((((cnt) % 10) + 1) % 10)
#define INC_TENS(NUM)		(((((cnt) / 10) + 1) % 10) * 10) + ((cnt) % 10)

static void flashBuz(void);
static watchStatus_t button_status(void);

#endif /* STOPWATCH_PRIVATE_H_ */