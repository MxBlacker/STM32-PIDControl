#include "stm32f10x.h"                  // Device header
#include "Menu.h"
#include "OLED.h"

void TIM2_Init(void){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //开启时钟
	TIM_InternalClockConfig(TIM2); //可以不写，因为默认就是使用内部时钟

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //分频数，可以是1,2,4
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //Counter模式
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; //这个和下一个都是0~65535的取值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; //参数要记得 - 1
	//这样就是1ms一次判断了
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //仅高级计时器有用 
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseInitStructure);
	
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //开启中断输出控制

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //PB14
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

/*
	LED流水灯
*/

int LED_counter;
int LED_lit_pin;

void reset_LED(){
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	
}

void LED_flow(){
	
	LED_counter++;
	
	short speed_tier = get_value(MENU_LED,0);
	short dir = get_value(MENU_LED,1);
	
	int speed = 500;
	switch(speed_tier){
		case 0:
			speed = 500;
			break;
		case 1:
			speed = 1000;
			break;
		case 2:
			speed = 200;
			break;
	}
	
	if(LED_counter >= speed){
		LED_counter = 0;
		
		if(dir == 0) LED_lit_pin = (LED_lit_pin + 1) % 4;
		else LED_lit_pin = ((LED_lit_pin - 1) + 4) % 4;
		
		reset_LED();
		switch(LED_lit_pin){
			case 0:
				GPIO_ResetBits(GPIOB,GPIO_Pin_12);
				break;
			case 1:
				GPIO_ResetBits(GPIOB,GPIO_Pin_13);
				break;
			case 2:
				GPIO_ResetBits(GPIOB,GPIO_Pin_14);
				break;			
			case 3:
				GPIO_ResetBits(GPIOB,GPIO_Pin_15);
				break;
		}
		
	}
}

/*
	按钮及其消抖，以及长按
*/

int button_counter;
int cur_state[4] = {0,0,0,0};
int prev_state[4] = {0,0,0,0};
int hold_counter[4] = {0,0,0,0};


void button_check(){	//以上升沿为界
	
	button_counter++;
	
	if(button_counter >= 20){	//消抖
		button_counter++;
		for(int i = 0;i < 4;i++){
			switch(i){
				case 0:
					prev_state[i] = cur_state[i];
					cur_state[i] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2);
					break;
				case 1:
					prev_state[i] = cur_state[i];
					cur_state[i] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
					break;
				case 2:
					prev_state[i] = cur_state[i];
					cur_state[i] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);	
					break;
				case 3:
					prev_state[i] = cur_state[i];
					cur_state[i] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14);	
					break;
			}
		}
	}

	for(int i = 0;i < 4;i++){
		if(prev_state[i] == 0 && cur_state[i] == 1){
			hold_counter[i] = 0;
			switch(i){
				case 0:
					up();
					break;
				case 1:
					down();
					break;
				case 2:
					confirm();
					break;
				case 3:
					backward();
					break;
			}
		}
		else if(prev_state[i] == 0 && cur_state[i] == 0){	//hold的检测
			hold_counter[i]++;
			
			if(hold_counter[i] < 1000 || hold_counter[i] % 100 != 0) break;
			
			switch(i){
				case 0:
					up();
					break;
				case 1:
					down();
					break;
				case 2:
					confirm();
					break;
				case 3:
					backward();
					break;
			}
		}
	}

}

/*
	旋钮
*/

void EXTI15_10_IRQHandler(){
	if((EXTI_GetITStatus(EXTI_Line10) == SET || EXTI_GetITStatus(EXTI_Line11) == SET)){
	
		if(EXTI_GetITStatus(EXTI_Line10) == SET){
			
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == 0)
				up();
		
			EXTI_ClearITPendingBit(EXTI_Line10);
	
		}
	
		if(EXTI_GetITStatus(EXTI_Line11) == SET){
		
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 0)
				down();
		
			EXTI_ClearITPendingBit(EXTI_Line11);
	
		}
	}
}

int test_num;

void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
		LED_flow();
		button_check();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
