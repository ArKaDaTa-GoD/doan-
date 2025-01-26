#include "DS3231.h"
extern I2C_HandleTypeDef hi2c1;
extern DS3231 ds3231;
#define ADD_DS3231 0x68<<1
static uint8_t DEC2BCD(uint8_t data)
{
	return (data/10)<<4|(data%10);
}
static uint8_t BCD2DEC(uint8_t data)
{
	return (data>>4)*10+(data&0x0f);
}
void write_ds3231(DS3231 *ds){
	uint8_t data[8];
	data[0]=0x00;
	data[1]=DEC2BCD(ds->Seconds);
	data[2]=DEC2BCD(ds->Minutes);
	data[3]=DEC2BCD(ds->Hours);
	data[4]=DEC2BCD(ds->Day);
	data[5]=DEC2BCD(ds->Date);
	data[6]=DEC2BCD(ds->Month);
	data[7]=DEC2BCD(ds->Year);
	HAL_I2C_Master_Transmit(&hi2c1,ADD_DS3231,data,8,1000);
}
void read_ds3231(DS3231 *ds){
	uint8_t add_ds3231=0x00;
	uint8_t data[7];
	HAL_I2C_Master_Transmit(&hi2c1,ADD_DS3231,&add_ds3231,1,1000);
	HAL_I2C_Master_Receive(&hi2c1,ADD_DS3231,data,7,1000);
	ds->Seconds=BCD2DEC(data[0]);
	ds->Minutes=BCD2DEC(data[1]);
	ds->Hours=BCD2DEC(data[2]);
	ds->Day=BCD2DEC(data[3]);
	ds->Date=BCD2DEC(data[4]);
	ds->Month=BCD2DEC(data[5]);
	ds->Year=BCD2DEC(data[6]);
}

