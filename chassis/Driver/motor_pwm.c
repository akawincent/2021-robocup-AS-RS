#include "motor_pwm.h"

/* PWM动力最大值 */
#define MOTOR_SPEED_MAX 10000

/**********************************************************************************************************
*函 数 名: motor_set_speed
*功能说明: 将pwm_pluse赋值给motor结构体中的speed 并向其通道注入动力
*形    参: motor结构体的地址   动力
*返 回 值: 无
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
