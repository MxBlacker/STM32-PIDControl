#include "stm32f10x.h"                  // Device header
#include "Initialization.h"
#include "OLED.h"
#include "Delay.h"
#include "EXTI.h"
#include "Menu.h"
#include "Timer.h"

extern int test_num;

int main(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	
	
	//TIM初始化
	TIM2_Init();
	
	
	//LED初始化，默认为上拉输入，所以给高电平
	AutoInitGPIO(GPIOB,GPIO_Mode_Out_PP,GPIO_Pin_12,GPIO_Speed_50MHz);
	AutoInitGPIO(GPIOB,GPIO_Mode_Out_PP,GPIO_Pin_13,GPIO_Speed_50MHz);
	AutoInitGPIO(GPIOB,GPIO_Mode_Out_PP,GPIO_Pin_14,GPIO_Speed_50MHz);
	AutoInitGPIO(GPIOB,GPIO_Mode_Out_PP,GPIO_Pin_15,GPIO_Speed_50MHz);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	
	///按钮初始化，依旧默认上拉，非阻塞+消抖
	AutoInitGPIO(GPIOA,GPIO_Mode_IPU,GPIO_Pin_4,GPIO_Speed_50MHz);
	AutoInitGPIO(GPIOA,GPIO_Mode_IPU,GPIO_Pin_2,GPIO_Speed_50MHz);
	AutoInitGPIO(GPIOA,GPIO_Mode_IPU,GPIO_Pin_0,GPIO_Speed_50MHz);
	AutoInitGPIO(GPIOC,GPIO_Mode_IPU,GPIO_Pin_14,GPIO_Speed_2MHz);
	
	//旋转编码器初始化
	AutoInitEXTI(GPIOA,GPIO_Mode_IPU,GPIO_Pin_10,GPIO_Speed_50MHz,EXTI_Mode_Interrupt,EXTI_Trigger_Falling,NVIC_PriorityGroup_2,EXTI15_10_IRQn,2,2);
	AutoInitEXTI(GPIOA,GPIO_Mode_IPU,GPIO_Pin_11,GPIO_Speed_50MHz,EXTI_Mode_Interrupt,EXTI_Trigger_Falling,NVIC_PriorityGroup_2,EXTI15_10_IRQn,2,2);
	
	//OLED初始化
	OLED_Init();
	OLED_Clear();
	
	//menu初始化
	interface_init();
	show_interface();
	
	while(1){
		
	}
}
