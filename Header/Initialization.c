#include "stm32f10x.h"                  // Device header
#include <stdio.h>

/*
	AutoInitGPIO
	用于自动初始化GPIOx端口
	输入格式为:
	AutoInitGPIO(GPIOx , Mode , Pin , Speed)
	
	这里我只写了ABC三个端口，因为我STM32就这么多端口赫赫
*/

void AutoInitGPIO(GPIO_TypeDef* GPIOx , GPIOMode_TypeDef Mode , uint16_t Pin , GPIOSpeed_TypeDef Speed){
	
	if(GPIOx == NULL) return; //判断是否合法
	
	if(GPIOx == GPIOA)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(GPIOx == GPIOB)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(GPIOx == GPIOC)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = Mode; 
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_InitStructure.GPIO_Speed = Speed;
	GPIO_Init(GPIOx , &GPIO_InitStructure);
}
