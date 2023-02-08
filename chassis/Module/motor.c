#include "motor.h"
#include "motor_encoder.h"
#include "math.h"
#include "motor_task.h"
#include "track_task.h"
#include "imu_task.h"
#include "time_cnt.h"

#define CHASSIS_RADIUS 22.0         //�������ĵ��������ľ���
#define WHEEL_ANGLE 45.0            //����С����복������ϵx��н�
#define RADIAN 57.3                 //��λ���ȶ�Ӧ�ĽǶ�
#define MAX_SPEED 110               //���Ƶ����ת����ٶ�
#define TIME_PARAM 2               //�ȴ�ʱ��

#define ENCODE_THRESHOLD 20         //����������·����ֵ
#define ENCODER_FACTOR 4            //����������P���Ŵ�����

uint32_t time = 0;                  //��ȡ��ϵͳ����ʱ��
unsigned short int  time_count=0;   //����
chassis_t chassis_param;            //������̲���

extern motor_t motor1;
extern motor_t motor2;
extern motor_t motor3;
extern motor_t motor4;

double motor_target[5];              //�洢�ĸ�����ٶȵ�Ŀ��ֵ
int val_track_front = 0;				//����ѭ��pid�����ֵ
int val_track_back = 0; 
int val_imu = 0;                //����������pid�����ֵ

extern uint8_t red_mode;
extern uint8_t blue_mode;

/************************************************************
*�� �� ��:set_chassis_speed
*����˵��:ֱ�Ӷ�Ŀ���ٶ�ֵ�����޸�
*��    ��:����������ٶ�
*�� �� ֵ:��
**************************************************************/
void set_chassis_speed (float x, float y, float w)
{
    chassis_param.x_speed = x;
    chassis_param.y_speed = y;
    chassis_param.w_speed = w;
}

/************************************************************
*�� �� ��:change_chassis_status
*����˵��:����ʹ�ܿ���
*��    ��:״̬��boolֵ
*�� �� ֵ:��
**************************************************************/
void change_chassis_status(bool status)
{
    chassis_param.status = status;
}

/************************************************************
*�� �� ��:chassis_synthetic_control
*����˵��:���̵��ۺϿ��ƺ������������ֿ��� �ڲ������PID
*��    ��:��
*�� �� ֵ:��
**************************************************************/
void chassis_synthetic_control(void)
{
    int i;
    double x, y, w; 
    double max_val, factor;
    if(chassis_param.status == false) return;//������̲���ʹ�ܣ���û�к�������

    max_val = 0;        //�����ֵ���ݽ��г�ʼ��
    factor = 1;         //�������ӳ�ʼ��

    time_count++;
    if(time_count == TIME_PARAM) {
        time_count = 0;
        
        /* �ڲ�PID��ִ�� BEGIN */
        if(red_mode){
					val_track_front = track_pid(&front_bar,front_weight);
				}
				else if(blue_mode){
					val_track_front = track_pid(&front_bar,back_weight);
				}
        
        /* �ڲ�PID��ִ�� END */ 
    }  
    
    x = chassis_param.x_speed;
    y = chassis_param.y_speed;
    w = chassis_param.w_speed + val_imu;
        
    /***************************************
     * motor1  ǰ���Ҳ�
     * motor2  ���Ҳ�
     * motor3  ǰ�����
     * motor4  �����
    ****************************************/
//    motor_target[1] = -cos(WHEEL_ANGLE/RADIAN) * x + sin(WHEEL_ANGLE/RADIAN) * y + CHASSIS_RADIUS * w;
//    motor_target[2] = cos(WHEEL_ANGLE/RADIAN) * x + sin(WHEEL_ANGLE/RADIAN) * y + CHASSIS_RADIUS * w;
//    motor_target[3] = -cos(WHEEL_ANGLE/RADIAN) * x - sin(WHEEL_ANGLE/RADIAN) * y + CHASSIS_RADIUS * w;
//    motor_target[4] = cos(WHEEL_ANGLE/RADIAN) * x - sin(WHEEL_ANGLE/RADIAN) * y + CHASSIS_RADIUS * w;

    motor_target[1] = -cos(WHEEL_ANGLE/RADIAN) * x + sin(WHEEL_ANGLE/RADIAN) * y + w;
    motor_target[2] = cos(WHEEL_ANGLE/RADIAN) * x + sin(WHEEL_ANGLE/RADIAN) * y + w;
    motor_target[3] = -cos(WHEEL_ANGLE/RADIAN) * x - sin(WHEEL_ANGLE/RADIAN) * y + w;
    motor_target[4] = cos(WHEEL_ANGLE/RADIAN) * x - sin(WHEEL_ANGLE/RADIAN) * y + w;    
    
   
    
//    /* ����һ���޷����������ⵥ���ٶȹ��ߵ��¿���Ч�������� */
//    for(i = 1; i <= 4; ++i) {                                       //�ҳ����ֵ
//        if(motor_target[i] > max_val) max_val = motor_target[i];
//    }
//    
//    /*���ֵ�Ƿ����ƣ����в�����ȷ�����ֵ���ڷ�Χ����ת�ٱ�������*/
//    if(max_val > MAX_SPEED) {             
//        factor = MAX_SPEED / max_val;
//        for(i = 1; i < 4; ++ i) {
//            motor_target[i] *= factor;
//        }
//    }
    
   /*�������pid����*/
   /*����������Ŀ��ֵ*/
   motor1_pid_data.expect = motor_target[1] + val_track_front;// + val_track_back;
   motor2_pid_data.expect = motor_target[2] + val_track_front;// + val_track_back;
   motor3_pid_data.expect = motor_target[3] + val_track_front;// + val_track_back;
   motor4_pid_data.expect = motor_target[4] + val_track_front;// + val_track_back;
  
   /*��ȡ��ǰ���ת��*/
   motor1_pid_data.feedback = read_freq(&motor1);
   motor2_pid_data.feedback = read_freq(&motor2);
   motor3_pid_data.feedback = read_freq(&motor3);
   motor4_pid_data.feedback = read_freq(&motor4);
  
   /*����pid���� �������ֵע����ͨ��*/
   
   set_motor_speed(&motor1, pid_incremental(&motor1_pid_data,&motor1_pid_paramer));
   set_motor_speed(&motor2, pid_incremental(&motor2_pid_data,&motor2_pid_paramer));
   set_motor_speed(&motor3, pid_incremental(&motor3_pid_data,&motor3_pid_paramer));
   set_motor_speed(&motor4, pid_incremental(&motor4_pid_data,&motor4_pid_paramer));
   //printf("%f  %f  %f  %f\r\n",motor1_pid_data.feedback ,motor2_pid_data.feedback ,motor3_pid_data.feedback ,motor4_pid_data.feedback);
   //printf("%f    %f   %d\r\n",motor2_pid_data.feedback,motor2_pid_data.expect,TIM1->CCR3);
   //motor_pid();
}


/************************************************************
*�� �� ��:move_by_encoder
*����˵��:ͨ���������涨·������
*��    ��:���� ·��
*�� �� ֵ:��
**************************************************************/
void move_by_encoder(int  direct, int val)
{
    time = TIME_ISR_CNT;            //��ȡϵͳʱ��
    double bias = 0, variation;     //��������
    encoder = 0;                    //���������ۼ�ֵ��0
    if(direct == 1)
    {
        if(val < 0)//����
        {
            while(__fabs(val) > encoder )
            {
                if( (TIME_ISR_CNT - time > 100) && ( __fabs( (val - encoder) ) < ENCODE_THRESHOLD)) goto EXIT_FUNC;  //��ʱ�������⿨��
                bias =  -(__fabs(val) - encoder);                       //�õ���ֵ
                variation = bias * ENCODER_FACTOR;                              //����ó����ֵ��P��
                variation = variation < - MAX_SPEED ? -MAX_SPEED : variation;   //�޷�
                set_chassis_speed(variation, 0, 0);                             //�����ٶ�
            }
        }
        else
        {

            while(val > encoder)
            {
                if( (TIME_ISR_CNT - time > 100) && ( (val - encoder ) < ENCODE_THRESHOLD) ) goto EXIT_FUNC;
                bias = val - encoder;
                variation = bias * ENCODER_FACTOR;
                variation = variation > MAX_SPEED ? MAX_SPEED : variation;
                set_chassis_speed(variation, 0, 0);
            }
        }
    }
    else if(direct == 2)
    {	
				if(val < 0)//���
        {
            while(__fabs(val) > encoder )
            {
                if( (TIME_ISR_CNT - time > 100) && ( __fabs( (val - encoder) ) < ENCODE_THRESHOLD)) goto EXIT_FUNC;  //��ʱ�������⿨��
                bias =  -(__fabs(val) - encoder);                       //�õ���ֵ
                variation = bias * ENCODER_FACTOR;                              //����ó����ֵ��P��
                variation = variation < - MAX_SPEED ? -MAX_SPEED : variation;   //�޷�
                set_chassis_speed(0, variation, 0);                             //�����ٶ�
            }
        }
        else  //��ǰ
        {

            while(val > encoder)
            {
                if( (TIME_ISR_CNT - time > 100) && ( (val - encoder ) < ENCODE_THRESHOLD) ) goto EXIT_FUNC;
                bias = val - encoder;
                variation = bias * ENCODER_FACTOR;
                variation = variation > MAX_SPEED ? MAX_SPEED : variation;
                set_chassis_speed(0, variation, 0);
            }
        }
				
    }
    
    EXIT_FUNC:
        set_chassis_speed(0, 0, 0);//ͣ��
}







