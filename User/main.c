#include "stm32f10x.h"                  // Device header
#include "Initialization.h"
#include "Serial.h"
#include "Timer.h"

extern char Serial_TxData[];
extern uint8_t Serial_RxData[];

int main(void){
	
	USART1_Serial_Init();
	TIMx_Init(TIM2 , 100 , 720); ///1ms一次
	
	
	
	while(1){
		
	}
}
