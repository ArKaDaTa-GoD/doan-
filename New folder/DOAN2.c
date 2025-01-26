#include "DOAN2.h"
#include "DS3231.h"
#include "CLCD_I2C.h"
#include "main.h"
#include "Sieuam.h"
#include <string.h>
#include <stdio.h>
int giay;
int phut;
int gio;
int thu;
int ngay;
int thang;
int nam;
int alarm_phut_off=0;
int alarm_gio_off=0;
int alarm_giay_off=0;
int set_distance_cm=30;
uint32_t current_time1;
uint32_t current_time2;
uint8_t trangthai1=0;
uint8_t trangthai3=0;
uint8_t trangthai4=0;
uint8_t trangthai6=0;
uint8_t dem1=0;
uint8_t vc=0;
char buffer[16];
uint8_t status=1;
extern CLCD_I2C_Name LCD1;
extern DS3231 ds3231;
extern float distance_cm;
extern uint8_t trangthai7;
extern TIM_HandleTypeDef htim1;
void delay_ms(uint32_t ms){
	uint32_t start_time=HAL_GetTick();
	while((HAL_GetTick()-start_time)<ms){}
}
uint8_t debounce_button(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin){
	static uint32_t last_press_time[16]={0};   
	static uint8_t button_states[16]={0};      
	uint8_t pin_index=GPIO_Pin%16;           
	uint32_t current_time=HAL_GetTick();
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin)==GPIO_PIN_SET){
		if((current_time-last_press_time[pin_index])>50){  
			button_states[pin_index]=1;       
			last_press_time[pin_index]=current_time;  
		}
	}else{
		button_states[pin_index]=0;
	}
	return button_states[pin_index];
}
void manhinhmot(){
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"1:XEM GIO      ");
	sprintf(buffer,"%01d",dem1);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"2:KHOANG CACH   ");
}
void manhinhhai(){
	giay=ds3231.Seconds;
	phut=ds3231.Minutes;
	gio=ds3231.Hours;
	thu=ds3231.Day;
	ngay=ds3231.Date;
	thang=ds3231.Month;
	nam=ds3231.Year;
	CLCD_I2C_SetCursor(&LCD1,0,0);
	sprintf(buffer,"%02d",gio);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,":");
	sprintf(buffer,"%02d",phut);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,":");
	sprintf(buffer,"%02d       ",giay);
	CLCD_I2C_WriteString(&LCD1,buffer);
	sprintf(buffer,"%01d",trangthai1);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"THU:");
	sprintf(buffer,"%01d",thu);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,",");
	sprintf(buffer,"%02d",ngay);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,"/");
	sprintf(buffer,"%02d",thang);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,"/");
	sprintf(buffer,"%02d  ",nam);
	CLCD_I2C_WriteString(&LCD1,buffer);
}
void manhinhba(void){
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"KC:");
	sprintf(buffer,"%.2fcm      ",distance_cm);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void manhinhbon(void){
	giay=ds3231.Seconds;
	phut=ds3231.Minutes;
	gio=ds3231.Hours;
	thu=ds3231.Day;
	ngay=ds3231.Date;
	thang=ds3231.Month;
	nam=ds3231.Year;
	CLCD_I2C_SetCursor(&LCD1,0,0);
	sprintf(buffer,"%02d",gio);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,":");
	sprintf(buffer,"%02d",phut);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,":");
	sprintf(buffer,"%02d       ",giay);
	CLCD_I2C_WriteString(&LCD1,buffer);
	sprintf(buffer,"%01d",trangthai3);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"THU:");
	sprintf(buffer,"%01d",thu);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,",");
	sprintf(buffer,"%02d",ngay);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,"/");
	sprintf(buffer,"%02d",thang);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_WriteString(&LCD1,"/");
	sprintf(buffer,"%02d  ",nam);
	CLCD_I2C_WriteString(&LCD1,buffer);
}
void manhinhnam(void){ //baothuc
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"ALARM          ");
	sprintf(buffer,"%01d",trangthai4);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void manhinhsau(void){
	gio=ds3231.Hours;
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"VARY HOURS:");
	sprintf(buffer,"%02d   ",gio);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void manhinhbay(void){
	phut=ds3231.Minutes;
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"VARY MINUTES:");
	sprintf(buffer,"%02d ",phut);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void manhinhtam(void){
	giay=ds3231.Seconds;
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"VARY SECONDS:");
	sprintf(buffer,"%02d ",giay);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void manhinhchin(void){
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"ALARM HOURS:");
	sprintf(buffer,"%02d  ",alarm_gio_off);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void manhinhmuoi(void){
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"ALARM MINUTES:");
	sprintf(buffer,"%02d",alarm_phut_off);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void manhinhmuoimot(void){
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"ALARM SECONDS:");
	sprintf(buffer,"%02d",alarm_giay_off);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void manhinhmuoihai(void){
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"SET DIS:");
	sprintf(buffer,"%02dcm    ",set_distance_cm);
	CLCD_I2C_WriteString(&LCD1,buffer);
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void manhinhmuoiba(void){
	CLCD_I2C_SetCursor(&LCD1,0,0);
	CLCD_I2C_WriteString(&LCD1,"NGOI QUA GIO    ");
	CLCD_I2C_SetCursor(&LCD1,0,1);
	CLCD_I2C_WriteString(&LCD1,"                ");
}
void xuly1(void){
	uint32_t current_time=ds3231.Hours*3600+ds3231.Minutes*60+ds3231.Seconds;
	if(distance_cm<set_distance_cm&&distance_cm>0&&trangthai6==0){
		trangthai6=1;
		current_time1=ds3231.Hours*3600+ds3231.Minutes*60+ds3231.Seconds;
	}
	else if(distance_cm>=set_distance_cm&&trangthai6==1){
		status=2;
		trangthai6=0;
	}
	if(trangthai6==1){
		if((current_time>=current_time1)&&(current_time-current_time1>alarm_giay_off)&&(trangthai4==3)){
			status=13;
		}
		else if((current_time>=current_time1)&&(current_time-current_time1>alarm_phut_off*60)&&(trangthai4==2)){
			status=13;
		}
		else if((current_time>=current_time1)&&(current_time-current_time1>alarm_gio_off*3600)&&(trangthai4==1)){
			status=13;
		}
	}
}
void xuly2(void){
	uint32_t current_time=ds3231.Hours*3600+ds3231.Minutes*60+ds3231.Seconds;
	if(distance_cm<set_distance_cm&&distance_cm>0&&trangthai7==0){
		vc=0;
		trangthai7=1;
	}
	else if(distance_cm>=set_distance_cm){
		if(vc==0){
			current_time2=current_time;
			vc=1;
		}
		if((current_time>=current_time2)&&(current_time-current_time2>9)){
			htim1.Instance->CCR1=0;
		}else{
			trangthai7=0;
		}
	}
}
void nutbam(void){
	if(debounce_button(GPIOC,GPIO_PIN_13)&&status==1){
		dem1++;
		if(dem1>=3){
			dem1=1;
		}
		status=1;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_1)&&status==1){
		if(dem1==1){
			status=2;
		}
		else{
			status=3;
		}
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_1)&&status==2){
		if(trangthai1==2){
			status=5;
		}
		else{
			status=4;
		}
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_1)&&status==4){
		if(trangthai3==1){
			status=6;
		}
		else if(trangthai3==2){
			status=7;
		}
		else{
			status=8;
		}
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_1)&&status==5){
		if(trangthai4==1){
			status=9;
		}
		else if(trangthai4==2){
			status=10;
		}
		else{
			status=11;
		}
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_1)&&(status==6||status==7||status==8)){
		status=4;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_1)&&status==3){
		status=12;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==2){
		trangthai1++;
		if(trangthai1>=3){
			trangthai1=1;
		}
		status=2;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==4){
		trangthai3++;
		if(trangthai3>=4){
			trangthai3=1;
		}
		status=4;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==5){
		trangthai4++;
		if(trangthai4>=4){
			trangthai4=1;
		}
		status=5;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==6){
		gio++;
		if(gio>=24){
			gio=0;
		}
		ds3231.Hours=gio;
		write_ds3231(&ds3231);
		status=6;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==9){
		alarm_gio_off++;
		if(alarm_gio_off>=24){
			alarm_gio_off=0;
		}
		status=9;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==7){
		phut++;
		if(phut>=60){
			phut=0;
		}
		ds3231.Minutes=phut;
		write_ds3231(&ds3231);
		status=7;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==10){
		alarm_phut_off++;
		if(alarm_phut_off>=60){
			alarm_phut_off=0;
		}
		status=10;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==8){
		giay++;
		if(giay>=60){
			giay=0;
		}
		ds3231.Seconds=giay;
		write_ds3231(&ds3231);
		status=8;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==11){
		alarm_giay_off++;
		if(alarm_giay_off>=60){
			alarm_giay_off=0;
		}
		status=11;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOC,GPIO_PIN_13)&&status==12){
		set_distance_cm+=5;
		if(set_distance_cm>100){
			set_distance_cm=100;
		}
		status=12;
		while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_0)&&status==6){
		gio--;
		if(gio<=-1){
			gio=23;
		}
		ds3231.Hours=gio;
		write_ds3231(&ds3231);
		status=6;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_0)&&status==9){
		alarm_gio_off--;
		if(alarm_gio_off<=-1){
			alarm_gio_off=23;
		}
		status=9;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_0)&&status==7){
		phut--;
		if(phut<=-1){
			phut=59;
		}
		ds3231.Minutes=phut;
		write_ds3231(&ds3231);
		status=7;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_0)&&status==10){
		alarm_phut_off--;
		if(alarm_phut_off<=-1){
			alarm_phut_off=59;
		}
		status=10;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_0)&&status==8){
		giay--;
		if(giay<=-1){
			giay=59;
		}
		ds3231.Seconds=giay;
		write_ds3231(&ds3231);
		status=8;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_0)&&status==11){
		alarm_giay_off--;
		if(alarm_giay_off<=-1){
			alarm_giay_off=59;
		}
		status=11;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_2)){
		dem1=0;
		trangthai1=0;
		trangthai3=0;
		status=1;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_SET);
		delay_ms(200);
	}
	else if(debounce_button(GPIOA,GPIO_PIN_0)&&status==12){
		set_distance_cm-=5;
		if(set_distance_cm<0){
			set_distance_cm=0;
		}
		status=12;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET);
		delay_ms(200);
	}
}
void chaychuongtrinhchinh(void){
	switch(status){
		case 1:
			manhinhmot();
			break;
		case 2:
			manhinhhai();
			break;
		case 3:
			manhinhba();
			break;
		case 4:
			manhinhbon();
			break;
		case 5:
			manhinhnam();
			break;
		case 6:
			manhinhsau();
			break;
		case 7:
			manhinhbay();
			break;
		case 8:
			manhinhtam();
			break;
		case 9:
			manhinhchin();
			break;
		case 10:
			manhinhmuoi();
			break;
		case 11:
			manhinhmuoimot();
			break;
		case 12:
			manhinhmuoihai();
			break;
		case 13:
			manhinhmuoiba();
			break;
		default:
			break;
	}
}
