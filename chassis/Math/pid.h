#ifndef _PID_H
#define _PID_H

#include <stdint.h>
#include "time_cnt.h"

#define NULL 0

typedef struct pid_paramer {
    //�����޷�ֵ
    float integrate_max;
    //���Ʋ���kp
    float kp;
    //���Ʋ���ki
    float ki;
    //���Ʋ���kd
    float kd;
    //����޷�
    float control_output_limit;
} pid_paramer_t;

typedef struct pid_data{
    //����
    volatile float expect;
    //����ֵ
    float feedback;
    //ƫ��
    float err;
    //�ϴ�ƫ��
    float last_err;
    //���ϴ�ƫ��
    float last2_err;
    //����ֵ
    float integrate;
    //����ƫ��
    float delta;    
    //ƫ��΢��
    float dis_err;
    //�����������
    float control_output;
    //���ʱ�����
    Testime pid_controller_dt;
    //˽������
    void *pri_data;
    //�Զ������ƫ�ƫ����ֻص�
    void (*err_callback)(struct pid_data *, struct pid_paramer *);
    //��·��־
    uint8_t short_circuit_flag;
} pid_data_t;

extern float  Iout, Pout, Dout;


typedef void (*pid_callback_t)(struct pid_data *, struct pid_paramer *);

float pid_positional(pid_data_t *data, pid_paramer_t *para);
float pid_incremental(pid_data_t *data, pid_paramer_t *para);

#endif
