#ifndef __TURNOVER_STEERING_H__
#define __TURNOVER_STEERING_H__

#include "tim.h"

/* ���ƶ�������ݽṹ�� */
typedef struct{
	TIM_HandleTypeDef *htim;          //��ʱ��
	uint32_t channel;                 //ͨ��
	uint16_t Pulse;                   //�Ƚϼ���ֵ
}Steering_t;

extern Steering_t turnover_steering;

void Steering_Init(Steering_t *turnover_steering);
void Set_Steering_clockwise(Steering_t *turnover_steering);
void Set_Steering_anticlockwise(Steering_t *turnover_steering);
void Set_Steering_stop(Steering_t *turnover_steering);


#endif
