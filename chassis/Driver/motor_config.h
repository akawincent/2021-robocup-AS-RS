#ifndef __MOTOR_CONFIG_H__
#define __MOTOR_CONFIG_H__

#include "tim.h"

/* �����Ϣ�ṹ������ */
typedef struct
{
    TIM_HandleTypeDef *htim_pwm;          //�ṩPWM��ʱ��
    uint32_t channel_A;                   //PWM_Aͨ��
    uint32_t channel_B;					  //PWM_Bͨ��
    int32_t speed;                        //PWM��ֵ

    TIM_HandleTypeDef *htim_ic;           //���벶��ʱ��
    uint32_t ic_channel;				  //���벶��ͨ��
    GPIO_TypeDef *IC_GPIO_Port_v;         //����ƽ�仯GPIO��
    uint16_t IC_Pin_v;

    uint16_t updata;                      //��ʱ�������������ͳ��
    int32_t freq;                         //ת��
} motor_t;

extern motor_t motor1;
extern motor_t motor2;
extern motor_t motor3;
extern motor_t motor4;

/* �����ʼ����ʹ�� */
void motor1_Enable(motor_t *motor);
void motor2_Enable(motor_t *motor);
void motor3_Enable(motor_t *motor);
void motor4_Enable(motor_t *motor);

#endif
