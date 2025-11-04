
/*
	这个头文件主要是TIM的初始化和TIM中断函数
*/

#ifndef __TIMER_FUCK
#define __TIMER_FUCK

void TIMx_Init(TIM_TypeDef * TIMx , uint16_t Period,uint16_t Prescaler);
void TIM2_EncoderInterfaceInit(void);
	
#endif
