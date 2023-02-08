#include "motor_config.h"

/* 定义电机数据类型 */
motor_t motor1;
motor_t motor2;
motor_t motor3;
motor_t motor4;

/**********************************************************************************************************
*函 数 名: motor1_Enable
*功能说明: 初始化motor1结构体 并使能其对应时钟与中断
*形    参: motor结构体的地址
*返 回 值: 无
**********************************************************************************************************/
void motor1_Enable(motor_t *motor)
{
    motor->htim_pwm = &htim1;
    motor->channel_A = TIM_CHANNEL_1;
    motor->channel_B = TIM_CHANNEL_2;
    motor->speed = 0;

    motor->htim_ic = &htim5;
    motor->ic_channel = TIM_CHANNEL_1;
    motor->IC_GPIO_Port_v = MOTOR3_ENCODER_GPIO_Port;
    motor->IC_Pin_v = MOTOR3_ENCODER_Pin;

    motor->updata = 0;
    motor->freq = 0;

    HAL_TIM_PWM_Start(motor->htim_pwm, motor->channel_A);
    HAL_TIM_PWM_Start(motor->htim_pwm, motor->channel_B);
    HAL_TIM_IC_Start_IT(motor->htim_ic, motor->ic_channel);
}


/**********************************************************************************************************
*函 数 名: motor2_Enable
*功能说明: 初始化motor2结构体 并使能其对应时钟与中断
*形    参: motor结构体的地址
*返 回 值: 无
**********************************************************************************************************/

void motor2_Enable(motor_t *motor)
{
    motor->htim_pwm = &htim1;
    motor->channel_A = TIM_CHANNEL_3;
    motor->channel_B = TIM_CHANNEL_4;
    motor->speed = 0;

    motor->htim_ic = &htim5;
    motor->ic_channel = TIM_CHANNEL_3;
    motor->IC_GPIO_Port_v = MOTOR4_ENCODER_GPIO_Port;
    motor->IC_Pin_v = MOTOR4_ENCODER_Pin;

    motor->updata = 0;
    motor->freq = 0;

    HAL_TIM_PWM_Start(motor->htim_pwm, motor->channel_A);
    HAL_TIM_PWM_Start(motor->htim_pwm, motor->channel_B);
    HAL_TIM_IC_Start_IT(motor->htim_ic, motor->ic_channel);
}

/**********************************************************************************************************
*函 数 名: motor3_Enable
*功能说明: 初始化motor2结构体 并使能其对应时钟与中断
*形    参: motor结构体的地址
*返 回 值: 无
**********************************************************************************************************/
void motor3_Enable(motor_t *motor)
{
    motor->htim_pwm = &htim2;
    motor->channel_A = TIM_CHANNEL_4;
    motor->channel_B = TIM_CHANNEL_3;
    motor->speed = 0;

    motor->htim_ic = &htim3;
    motor->ic_channel = TIM_CHANNEL_1;
    motor->IC_GPIO_Port_v = MOTOR1_ENCODER_GPIO_Port;
    motor->IC_Pin_v = MOTOR1_ENCODER_Pin;

    motor->updata = 0;
    motor->freq = 0;

    HAL_TIM_PWM_Start(motor->htim_pwm, motor->channel_A);
    HAL_TIM_PWM_Start(motor->htim_pwm, motor->channel_B);
    HAL_TIM_IC_Start_IT(motor->htim_ic, motor->ic_channel);
}

/**********************************************************************************************************
*函 数 名: motor4_Enable
*功能说明: 初始化motor2结构体 并使能其对应时钟与中断
*形    参: motor结构体的地址
*返 回 值: 无
**********************************************************************************************************/
void motor4_Enable(motor_t *motor)
{
    motor->htim_pwm = &htim2;
    motor->channel_A = TIM_CHANNEL_2;
    motor->channel_B = TIM_CHANNEL_1;
    motor->speed = 0;

    motor->htim_ic = &htim3;
    motor->ic_channel = TIM_CHANNEL_3;
    motor->IC_GPIO_Port_v = MOTOR2_ENCODER_GPIO_Port;
    motor->IC_Pin_v = MOTOR2_ENCODER_Pin;

    motor->updata = 0;
    motor->freq = 0;

    HAL_TIM_PWM_Start(motor->htim_pwm, motor->channel_A);
    HAL_TIM_PWM_Start(motor->htim_pwm, motor->channel_B);
    HAL_TIM_IC_Start_IT(motor->htim_ic, motor->ic_channel);
}
