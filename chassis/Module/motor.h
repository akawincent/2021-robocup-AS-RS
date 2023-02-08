#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdbool.h>
#include "motor_config.h"
#include "motor_pwm.h"

#define X_direct 1
#define Y_direct 2

/*底盘信息结构体数据类型*/
typedef struct
{
    bool status;        //是否使能底盘
    float x_speed;      //x方向底盘速度
    float y_speed;      //y方向底盘速度
    float w_speed;      //w方向底盘角速度
} chassis_t;




void set_chassis_speed (float x, float y, float w);
void change_chassis_status(bool status);
void chassis_synthetic_control(void);
void move_by_encoder(int  direct, int val);

#endif
