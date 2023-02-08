#ifndef __TURNOVER_STEERING_H__
#define __TURNOVER_STEERING_H__

#include "tim.h"

/* 控制舵机的数据结构体 */
typedef struct{
	TIM_HandleTypeDef *htim;          //定时器
	uint32_t channel;                 //通道
	uint16_t Pulse;                   //比较计数值
}Steering_t;

extern Steering_t turnover_steering;

void Steering_Init(Steering_t *turnover_steering);
void Set_Steering_clockwise(Steering_t *turnover_steering);
void Set_Steering_anticlockwise(Steering_t *turnover_steering);
void Set_Steering_stop(Steering_t *turnover_steering);


#endif
