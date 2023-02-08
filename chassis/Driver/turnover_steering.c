#include "turnover_steering.h"

/* 定义周转舵机的结构体数据类型 */
Steering_t turnover_steering;


/**********************************************************************************************************
*函 数 名: Steering_Init
*功能说明: 初始化周转舵机
*形    参: 舵机结构体    
*返 回 值: 无
**********************************************************************************************************/
void Steering_Init(Steering_t *turnover_steering){
	turnover_steering->htim= &htim4;
	turnover_steering->channel = TIM_CHANNEL_4;
	turnover_steering->Pulse=0;
	HAL_TIM_PWM_Start(turnover_steering->htim,turnover_steering->channel);
}

/**********************************************************************************************************
*函 数 名: Set_Steering_clockwise
*功能说明: 设定周转舵机顺时针转动
*形    参: 舵机结构体   
*返 回 值: 无
**********************************************************************************************************/
void Set_Steering_clockwise(Steering_t *turnover_steering){
	turnover_steering->Pulse=1850;
	__HAL_TIM_SET_COMPARE(turnover_steering->htim,turnover_steering->channel,turnover_steering->Pulse);
}

/**********************************************************************************************************
*函 数 名: Set_Steering_clockwise
*功能说明: 设定周转舵机逆时针转动  
*形    参: 舵机结构体   
*返 回 值: 无
**********************************************************************************************************/
void Set_Steering_anticlockwise(Steering_t *turnover_steering){
	turnover_steering->Pulse=1150;
	__HAL_TIM_SET_COMPARE(turnover_steering->htim,turnover_steering->channel,turnover_steering->Pulse);
}

/**********************************************************************************************************
*函 数 名: Set_Steering_clockwise
*功能说明: 设定周转舵机停止
*形    参: 舵机结构体   
*返 回 值: 无
**********************************************************************************************************/
void Set_Steering_stop(Steering_t *turnover_steering){
	turnover_steering->Pulse=1500;
	__HAL_TIM_SET_COMPARE(turnover_steering->htim,turnover_steering->channel,turnover_steering->Pulse);
}



