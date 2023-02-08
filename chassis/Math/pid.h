#ifndef _PID_H
#define _PID_H

#include <stdint.h>
#include "time_cnt.h"

#define NULL 0

typedef struct pid_paramer {
    //积分限幅值
    float integrate_max;
    //控制参数kp
    float kp;
    //控制参数ki
    float ki;
    //控制参数kd
    float kd;
    //输出限幅
    float control_output_limit;
} pid_paramer_t;

typedef struct pid_data{
    //期望
    volatile float expect;
    //反馈值
    float feedback;
    //偏差
    float err;
    //上次偏差
    float last_err;
    //上上次偏差
    float last2_err;
    //积分值
    float integrate;
    //单次偏差
    float delta;    
    //偏差微分
    float dis_err;
    //控制器总输出
    float control_output;
    //间隔时间计算
    Testime pid_controller_dt;
    //私有数据
    void *pri_data;
    //自定义计算偏差，偏差积分回调
    void (*err_callback)(struct pid_data *, struct pid_paramer *);
    //短路标志
    uint8_t short_circuit_flag;
} pid_data_t;

extern float  Iout, Pout, Dout;


typedef void (*pid_callback_t)(struct pid_data *, struct pid_paramer *);

float pid_positional(pid_data_t *data, pid_paramer_t *para);
float pid_incremental(pid_data_t *data, pid_paramer_t *para);

#endif
