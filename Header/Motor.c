#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Motor.h"
#include "Serial.h"

#define PULSES_PER_REV 2496				//2496脉冲每圈
#define MAX_RPS 9.9f      				//实测9.22圈每秒

uint16_t Motor_Get_Left_Frequency(void){
	return TIM_GetCounter(TIM3);
}

uint16_t Motor_Get_Right_Frequency(void){;
	return TIM_GetCounter(TIM4);
}

void Motor_Set_Target_Speed(MotorTypeDef * Motor , float Target){
	Motor->Target_Speed = (int)Target;
}

//以下是PID部分，这里定了两个结构体来分左右

void Cal_Current_Speed(MotorTypeDef * Motor , TIM_TypeDef * TIMx){
		
	Motor->Prev_Count = Motor->Cur_Count;
	Motor->Cur_Count = TIM_GetCounter(TIMx);
        
	// 计算脉冲差值（处理溢出）
	int32_t pulse_diff = (int32_t)Motor->Cur_Count - (int32_t)Motor->Prev_Count;
	if(pulse_diff > 32767) pulse_diff -= 65536;
	else if(pulse_diff < -32768) pulse_diff += 65536;
        
	// 转换为实际转速（转/秒）
	// 10ms内的脉冲数 → 1秒内的脉冲数 → 转数
	Motor->Cur_Speed = (( (float)pulse_diff * 100 / PULSES_PER_REV ) / MAX_RPS) * 1000;
        
	//Serial_Printf("Pulses: %d, CRR: %.2f", pulse_diff, Motor->Cur_Speed);
	
}

float PID_Control(MotorTypeDef * Motor , PIDTypeDef * PID){
	
	PID->error = Motor->Target_Speed - Motor->Cur_Speed;
	
	// 比例
	float proportional_score = PID->KP * PID->error;
    
	// 积分
	PID->I += PID->error;
	// 积分限幅
	if(PID->I > PID->I_Lim) PID->I = PID->I_Lim;
	if(PID->I < -PID->I_Lim) PID->I = -PID->I_Lim;
	float integral_score = PID->KI * PID->I;
	
	// 微分
	float derivative_score = PID->KD * (PID->error - PID->prev_error);
	
	// 总输出
	float output = proportional_score + integral_score + derivative_score;
		
	// 输出限幅
	if(output > PID->Output_Lim) output = PID->Output_Lim;
	if(output < -PID->Output_Lim) output = -PID->Output_Lim;
	
	PID->prev_error = PID->error;
	
    return output;
}

void Set_Left_Motor_Speed(float Speed){		//单位RPS
	
	if(Speed < 0){
		GPIO_WriteBit(GPIOB , GPIO_Pin_12 , Bit_RESET);
		GPIO_WriteBit(GPIOB , GPIO_Pin_13 , Bit_SET);
		Speed = -Speed;
	}else{
		GPIO_WriteBit(GPIOB , GPIO_Pin_12 , Bit_SET);
		GPIO_WriteBit(GPIOB , GPIO_Pin_13 , Bit_RESET);
	}
	
	if(Speed > 1000) Speed = 1000;
	//Serial_Printf("Speed = %.2f",Speed);
	
	Set_OC_value(TIM2 , 3 , (int)Speed);
	
}

void Set_Right_Motor_Speed(float Speed){
	
	if(Speed < 0){
		GPIO_WriteBit(GPIOB , GPIO_Pin_14 , Bit_RESET);
		GPIO_WriteBit(GPIOB , GPIO_Pin_15 , Bit_SET);
		Speed = -Speed;
	}else{
		GPIO_WriteBit(GPIOB , GPIO_Pin_14 , Bit_SET);
		GPIO_WriteBit(GPIOB , GPIO_Pin_15 , Bit_RESET);
	}
	
	if(Speed > 1000) Speed = 1000;
	//Serial_Printf("Speed = %f",Speed);
	
	Set_OC_value(TIM2 , 4 , (int)Speed);
	
}

