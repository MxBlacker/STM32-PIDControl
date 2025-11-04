#include "stm32f10x.h"                  // Device header
#include "Initialization.h"

void TIMx_Init(TIM_TypeDef * TIMx , uint16_t Period , uint16_t Prescaler){
	
	if(TIMx == TIM1)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if (TIMx == TIM2) 
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if (TIMx == TIM3) 
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if (TIMx == TIM4) 
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_InternalClockConfig(TIMx);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = Period - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler - 1;	
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIMx , &TIM_TimeBaseInitStructure);
	
	
	TIM_ClearITPendingBit(TIMx , TIM_IT_Update);
	TIM_ITConfig(TIMx , TIM_IT_Update,ENABLE); //开启中断输出控制

	IRQn_Type IRQn;
	if (TIMx == TIM1) 
        IRQn = TIM1_CC_IRQn;
    else if (TIMx == TIM2) 
        IRQn = TIM2_IRQn;
    else if (TIMx == TIM3) 
        IRQn = TIM3_IRQn;
    else if (TIMx == TIM4) 
        IRQn = TIM4_IRQn;
    
	AutoInitNVIC(NVIC_PriorityGroup_2 , IRQn , 2 , 1);
	
	TIM_Cmd(TIMx,ENABLE);
	
}

void TIM2_EncoderInterfaceInit(void){
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure); //编码器模式不需要全部配置，后面那些分频器啥的就不用配置了
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //CH1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInit(TIM2 , &TIM_ICInitStructure);

	TIM_ICStructInit(&TIM_ICInitStructure); //配置Channel2，两个都要去到编码器
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //CH2
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInit(TIM2 , &TIM_ICInitStructure);

	TIM_EncoderInterfaceConfig(TIM2 , TIM_EncoderMode_TI12 , TIM_ICPolarity_Rising , TIM_ICPolarity_Rising);

}

void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
		
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
