#include "turnover_steering.h"

/* ������ת����Ľṹ���������� */
Steering_t turnover_steering;


/**********************************************************************************************************
*�� �� ��: Steering_Init
*����˵��: ��ʼ����ת���
*��    ��: ����ṹ��    
*�� �� ֵ: ��
**********************************************************************************************************/
void Steering_Init(Steering_t *turnover_steering){
	turnover_steering->htim= &htim4;
	turnover_steering->channel = TIM_CHANNEL_4;
	turnover_steering->Pulse=0;
	HAL_TIM_PWM_Start(turnover_steering->htim,turnover_steering->channel);
}

/**********************************************************************************************************
*�� �� ��: Set_Steering_clockwise
*����˵��: �趨��ת���˳ʱ��ת��
*��    ��: ����ṹ��   
*�� �� ֵ: ��
**********************************************************************************************************/
void Set_Steering_clockwise(Steering_t *turnover_steering){
	turnover_steering->Pulse=1850;
	__HAL_TIM_SET_COMPARE(turnover_steering->htim,turnover_steering->channel,turnover_steering->Pulse);
}

/**********************************************************************************************************
*�� �� ��: Set_Steering_clockwise
*����˵��: �趨��ת�����ʱ��ת��  
*��    ��: ����ṹ��   
*�� �� ֵ: ��
**********************************************************************************************************/
void Set_Steering_anticlockwise(Steering_t *turnover_steering){
	turnover_steering->Pulse=1150;
	__HAL_TIM_SET_COMPARE(turnover_steering->htim,turnover_steering->channel,turnover_steering->Pulse);
}

/**********************************************************************************************************
*�� �� ��: Set_Steering_clockwise
*����˵��: �趨��ת���ֹͣ
*��    ��: ����ṹ��   
*�� �� ֵ: ��
**********************************************************************************************************/
void Set_Steering_stop(Steering_t *turnover_steering){
	turnover_steering->Pulse=1500;
	__HAL_TIM_SET_COMPARE(turnover_steering->htim,turnover_steering->channel,turnover_steering->Pulse);
}



