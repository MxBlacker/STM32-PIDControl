#include "stm32f10x.h"                  // Device header

/*
GPIO口我们默认上拉输入，所以低电平为按下，我们以低电平变高电平的瞬间为
*/

void Button_Check(GPIO_TypeDef * GPIOx , uint16_t Pin , uint16_t EVENT_ID){ 		//有了就返回1
	
	static int Button_Counter = 0;
	static int PrevState = SET , CurState = SET;
	Button_Counter++;
	
	//10ms消抖
	if(Button_Counter >= 10){
		PrevState = CurState;
		CurState = GPIO_ReadInputDataBit(GPIOx , Pin);
		
		if(PrevState == RESET && CurState == SET){	//按钮松开
			//执行某些操作，这个到时候用EVENT_ID来记录
		}
		
		Button_Counter = 0;
	}
}

void EVENT(uint16_t EVENT_ID){
	//到时候再说
}