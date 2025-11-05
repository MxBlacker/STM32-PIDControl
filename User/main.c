#include "stm32f10x.h"                  // Device header
#include "Initialization.h"
#include "Serial.h"
#include "Timer.h"
#include "Motor.h"

extern uint8_t Serial_RxData[];
extern uint8_t Freq_Counter;

uint16_t Motor_Speed;

/*
	1. 串口传信息 √
	2. PID
	
		2.1 学习PID √
		2.2 写PID程序 
		2.3 将PID程序应用于左轮 控制量CCR
		
	3. 左右轮同转
	
		3.1 读取左轮Freq和占空比
		3.2 据此输出方波给右轮
*/

int main(void){
	
	USART1_Serial_Init();
	//TIM2同时在计时
	TIMx_Init(TIM2 , 100 , 720 , OC_MODE , 3);				//输出左轮
	TIMx_Init(TIM2 , 1000 , 720 , OC_MODE , 4);				//输出右轮
	TIMx_Init(TIM3 , 65535 , 1 , ENCODER_MODE , 0);			//读左轮
	TIMx_Init(TIM4 , 65535 , 1 , ENCODER_MODE , 0);			//读右轮
	
	while(1){
		
		if(Freq_Counter >= 10){								//10ms传输一次
			Freq_Counter = 0;
			Motor_Speed = (int16_t)Motor_Get_Frequency();
			Serial_Printf("Speed = %d\r\n",Motor_Speed);	
		}
		
		if(Serial_GetRxFlag() == 1){						//接收到数据了
			Motor_Set_Target_Speed(Transfer_RxData());
		}
		
	}
}
