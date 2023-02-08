#include "motor_pwm.h"

/* PWM�������ֵ */
#define MOTOR_SPEED_MAX 10000

/**********************************************************************************************************
*�� �� ��: motor_set_speed
*����˵��: ��pwm_pluse��ֵ��motor�ṹ���е�speed ������ͨ��ע�붯��
*��    ��: motor�ṹ��ĵ�ַ   ����
*�� �� ֵ: ��
**********************************************************************************************************/
void set_motor_speed(motor_t *motor, int32_t speed)
{
    int32_t ccr = 0;

    if (speed >= 0)
    {
        if (speed > MOTOR_SPEED_MAX)
            ccr = MOTOR_SPEED_MAX;
        else
            ccr = speed;

        motor->speed = ccr;

        __HAL_TIM_SET_COMPARE(motor->htim_pwm, motor->channel_A, motor->speed);
        __HAL_TIM_SET_COMPARE(motor->htim_pwm, motor->channel_B, 0);
    }

    else if (speed < 0)
    {
        if (speed < -MOTOR_SPEED_MAX)
            ccr = MOTOR_SPEED_MAX;
        else
            ccr = -speed;

        motor->speed = ccr;

        __HAL_TIM_SET_COMPARE(motor->htim_pwm, motor->channel_B, motor->speed);
        __HAL_TIM_SET_COMPARE(motor->htim_pwm, motor->channel_A, 0);
    }
}
