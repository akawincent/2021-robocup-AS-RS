#ifndef __MOTOR_PWM_H__
#define __MOTOR_PWM_H__

#include "tim.h"
#include "motor_config.h"


void set_motor_speed(motor_t *motor, int32_t speed);

#endif

