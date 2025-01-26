#include "Sieuam.h"
#include "DOAN2.h"
#include "main.h"
uint8_t trangthai2=0;
float distance_cm=0;
uint32_t end_time=0;
extern TIM_HandleTypeDef htim2;
typedef enum{
	HCSR04_IDLE_STATE,
	HCSR04_WAIT_RISING_STATE,
	HCSR04_WAIT_FALLING_STATE,
	HCSR04_COMPLETE_STATE
}HCSR04_State;
HCSR04_State hcsr04_state=HCSR04_IDLE_STATE;
void HCSR04_Start(){
	trangthai2=0;
	HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port,HCSR04_TRIG_Pin,GPIO_PIN_SET);
	delay_ms(1);
	HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port,HCSR04_TRIG_Pin,GPIO_PIN_RESET);
	hcsr04_state=HCSR04_WAIT_RISING_STATE;
}
void HCSR04_Handle(){
	switch(hcsr04_state){
		case HCSR04_WAIT_RISING_STATE:{
			if(HAL_GPIO_ReadPin(HCSR04_ECHO_EXT_GPIO_Port, HCSR04_ECHO_EXT_Pin) == GPIO_PIN_SET){
				__HAL_TIM_SET_COUNTER(&htim2,0);
				HAL_TIM_Base_Start(&htim2);
				hcsr04_state=HCSR04_WAIT_FALLING_STATE;
			}
			break;
		}
		case HCSR04_WAIT_FALLING_STATE:{
			if(HAL_GPIO_ReadPin(HCSR04_ECHO_EXT_GPIO_Port,HCSR04_ECHO_EXT_Pin)==GPIO_PIN_RESET){
				HAL_TIM_Base_Stop(&htim2);
				end_time=__HAL_TIM_GET_COUNTER(&htim2);
				hcsr04_state=HCSR04_COMPLETE_STATE;
				distance_cm=(end_time*0.0343f)/2;
				if(distance_cm>=100){
					distance_cm=99.99;
				}
			}
			break;
		}
		case HCSR04_COMPLETE_STATE:{
			trangthai2=1;
			hcsr04_state=HCSR04_IDLE_STATE;
			break;
		}
		default:
			break;
	}
}
void chaysieuam(void){
	if(hcsr04_state==HCSR04_IDLE_STATE){
		HCSR04_Start();
	}else{
		HCSR04_Handle();
	}
}
void chaysieuam2(void){
	chaysieuam();
	while(trangthai2==0){
		chaysieuam();
	}
}
