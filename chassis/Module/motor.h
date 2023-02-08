#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdbool.h>
#include "motor_config.h"
#include "motor_pwm.h"

#define X_direct 1
#define Y_direct 2

/*������Ϣ�ṹ����������*/
typedef struct
{
    bool status;        //�Ƿ�ʹ�ܵ���
    float x_speed;      //x��������ٶ�
    float y_speed;      //y��������ٶ�
    float w_speed;      //w������̽��ٶ�
} chassis_t;




void set_chassis_speed (float x, float y, float w);
void change_chassis_status(bool status);
void chassis_synthetic_control(void);
void move_by_encoder(int  direct, int val);

#endif
