#include "stm32f10x.h"                  // Device header

uint16_t Motor_Get_Frequency(void){
	int Temp_Freq = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3 , 0);
	
	return Temp_Freq;
}

int16_t Target_Speed;

void Motor_Set_Target_Speed(int16_t Target){
	Target_Speed = Target;
}


