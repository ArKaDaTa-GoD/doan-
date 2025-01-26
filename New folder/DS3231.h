#ifndef __DS3231_H
#define __DS3231_H
#include "stm32f1xx_hal.h"
typedef struct{
	uint8_t Seconds;
	uint8_t Minutes;
	uint8_t Hours;
	uint8_t Day;
	uint8_t Date;
	uint8_t Month;
	uint8_t Year;
}DS3231;
void write_ds3231(DS3231 *ds);
void read_ds3231(DS3231 *ds);
#endif

