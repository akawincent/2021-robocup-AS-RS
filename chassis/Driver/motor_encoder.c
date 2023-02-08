#include "motor_encoder.h"
#include "motor_config.h"

#define TIM_PRESCALER 108-1  //��Ƶ
#define MAX_TIM_CNT 65535    //��ʱ��������ֵ
#define TIM_IC_FREQ 1000000  //Ƶ��
#define SPEED_PARAM TIM_IC_FREQ/20    //�ٶ�����
#define FILTER 4            //��ֵ�˲� ��������    
 
#define IC_RISE_EDGE 0     //��һ��������
#define IC_FALL_EDGE 1     //�ڶ���������
#define FORWARD    1         //ǰ������
#define BACKWARD  -1        //���˷���

#define POLE 2               //MG540�����ת�ų��ļ�����

extern motor_t motor1;
extern motor_t motor2;
extern motor_t motor3;
extern motor_t motor4;

int encoder = 0;

/**********************************************************************************************************
*�� �� ��: TIM5CaptureChannel1Callback
*����˵��: ͨ�����벶���ܼ�����1ת����Ƶ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void TIM5CaptureChannel1Callback()
{
    static uint8_t ic_edge = IC_RISE_EDGE;
    static uint32_t cap_val_1 = 0;             //��һ��������CCR�Ĵ�����ֵ
    static uint32_t cap_val_2 = 0;             //�ڶ���������CCR�Ĵ�����ֵ
    static int32_t sum_speed = 0;              //��ֵ�˲������ٶ�ֵ�ܺ�
    static int32_t single_speed = 0;           //�����ٶ�ֵ
    static int32_t average_speed = 0;          //�˲�������ƽ���ٶ�
    static int32_t last_average_speed = 0;     //�洢��һ���˲��ó���ƽ���ٶ�
    static uint32_t cap_cnt = 0;               //��¼��������
    static int8_t direct = 0;                  //����
    static uint8_t flag_first = 0;             //��־�Ƿ��ǵ�һ���˲�

    if(ic_edge == IC_RISE_EDGE)                //����⵽������
    {
        motor1.updata = 0;                     //�����ж��¼���¼
        cap_val_1 = HAL_TIM_ReadCapturedValue(motor1.htim_ic, motor1.ic_channel);   //��ȡTIM3 CCR1��ֵ
        ic_edge = IC_FALL_EDGE;                                                     //�ȴ��½���
        
        /* �����һ��GPIO�ڵĵ�ƽ�仯���жϷ���*/
        if(HAL_GPIO_ReadPin(motor1.IC_GPIO_Port_v, motor1.IC_Pin_v) == GPIO_PIN_RESET)
        {
            direct = BACKWARD;
        }
        else
        {
            direct = FORWARD;
        }
        __HAL_TIM_SET_CAPTUREPOLARITY(motor1.htim_ic, motor1.ic_channel, TIM_ICPOLARITY_FALLING);     //ת�����벶����Ϊ�½���
    }
    
    else                                                                                              //��⵽�½���
    {
        cap_val_2 = HAL_TIM_ReadCapturedValue(motor1.htim_ic, motor1.ic_channel);                     //�ٴζ�ȡTIM3 CCR1��ֵ
        if(motor1.updata == 0)                                                                        //�����β���֮�������δ���
        {
            single_speed = SPEED_PARAM / (cap_val_2 - cap_val_1) * direct;
        }
        else if(motor1.updata == 1)                                                                   //�����β���֮�����һ����������
        {
            single_speed = SPEED_PARAM / ((MAX_TIM_CNT - cap_val_1) + cap_val_2) * direct;
        }
        else                                                                                          //�����β���֮�����N����������
        {
            single_speed = SPEED_PARAM / ((MAX_TIM_CNT - cap_val_1) + MAX_TIM_CNT * (motor1.updata - 1) + cap_val_2) * direct;
        }
        
        sum_speed += single_speed;                              //���������ۼ�
        cap_cnt++;                                              //�ۻ���������
        
        if(cap_cnt == FILTER) {                                 //�������                                   
           cap_cnt = 0;                                     
           last_average_speed = average_speed;                  //������һ���˲�����ٶ�ֵ  
           if(flag_first == 0) {
               average_speed = (sum_speed+last_average_speed) / FILTER; //��һ���˲�ʱ last_average_speed=0
               flag_first = 1;
           }
           else {
               average_speed = (sum_speed-single_speed+last_average_speed) / FILTER;    //������˲����������һ���ɼ����ٶ� �滻Ϊ��һ���˲�����ٶ�
           }
           
           if(!(__fabs(average_speed + last_average_speed) < 5)) {              //����һ������һ�β�ֵ��������Ӻ�ӽ�0 ˵������ֵ��������
                motor1.freq =  average_speed ;
           } 
           cap_cnt = 0;             //�����������  ��ͳ�Ʋ���������ֵ��0 �ȴ���һ�β�������
           sum_speed = 0;           //�����������  �������ܺ͵�ֵ��0 �ȴ���һ�β�������
        }
        __HAL_TIM_SET_CAPTUREPOLARITY(motor1.htim_ic, motor1.ic_channel, TIM_ICPOLARITY_RISING);      //ת�����벶����Ϊ�����أ��ȴ���һ�β���
        ic_edge = IC_RISE_EDGE;     //�ȴ���һ�����ڵĵ�һ��������
    }
}

/**********************************************************************************************************
*�� �� ��: TIM5CaptureChannel3Callback
*����˵��: ͨ�����벶���ܼ�����2ת����Ƶ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void TIM5CaptureChannel3Callback()
{
    static uint8_t ic_edge = IC_RISE_EDGE;
    static uint32_t cap_val_1 = 0;             //��һ��������CCR�Ĵ�����ֵ
    static uint32_t cap_val_2 = 0;             //�ڶ���������CCR�Ĵ�����ֵ
    static int32_t sum_speed = 0;              //��ֵ�˲������ٶ�ֵ�ܺ�
    static int32_t single_speed = 0;           //�����ٶ�ֵ
    static int32_t average_speed = 0;          //�˲�������ƽ���ٶ�
    static int32_t last_average_speed = 0;     //�洢��һ���˲��ó���ƽ���ٶ�
    static uint32_t cap_cnt = 0;               //��¼��������
    static int8_t direct = 0;                  //����
    static uint8_t flag_first = 0;             //��־�Ƿ��ǵ�һ���˲�

    if(ic_edge == IC_RISE_EDGE)                //����⵽������
    {
        motor2.updata = 0;                     //�����ж��¼���¼
        cap_val_1 = HAL_TIM_ReadCapturedValue(motor2.htim_ic, motor2.ic_channel);   //��ȡTIM3 CCR1��ֵ
        ic_edge = IC_FALL_EDGE;                                                     //�ȴ��½���

        /* �����һ��GPIO�ڵĵ�ƽ�仯���жϷ���*/
        if(HAL_GPIO_ReadPin(motor2.IC_GPIO_Port_v, motor2.IC_Pin_v) == GPIO_PIN_RESET)
        {
            direct = BACKWARD;
        }
        else
        {
            direct = FORWARD;
        }
        __HAL_TIM_SET_CAPTUREPOLARITY(motor2.htim_ic, motor2.ic_channel, TIM_ICPOLARITY_FALLING);     //ת�����벶����Ϊ�½���
    }
    
    else                                                                                              //��⵽�½���
    {
        cap_val_2 = HAL_TIM_ReadCapturedValue(motor2.htim_ic, motor2.ic_channel);                     //�ٴζ�ȡTIM3 CCR1��ֵ
        if(motor2.updata == 0)                                                                        //�����β���֮�������δ���
        {
            single_speed = SPEED_PARAM / (cap_val_2 - cap_val_1) * direct;
        }
        else if(motor2.updata == 1)                                                                   //�����β���֮�����һ����������
        {
            single_speed = SPEED_PARAM / ((MAX_TIM_CNT - cap_val_1) + cap_val_2) * direct;
        }
        else                                                                                          //�����β���֮�����N����������
        {
            single_speed = SPEED_PARAM / ((MAX_TIM_CNT - cap_val_1) + MAX_TIM_CNT * (motor2.updata - 1) + cap_val_2) * direct;
        }
        
        sum_speed += single_speed;                              //���������ۼ�
        cap_cnt++;                                              //�ۻ���������
        
        if(cap_cnt == FILTER) {                                 //�������                                   
           cap_cnt = 0;                                     
           last_average_speed = average_speed;                  //������һ���˲�����ٶ�ֵ
           if(flag_first == 0) {
               average_speed = (sum_speed+last_average_speed) / FILTER; //��һ���˲�ʱ last_average_speed=0
               flag_first = 1;
           }
           else {
               average_speed = (sum_speed-single_speed+last_average_speed) / FILTER;    //������˲����������һ���ɼ����ٶ� �滻Ϊ��һ���˲�����ٶ�
           }
           
           if(!(__fabs(average_speed + last_average_speed) < 5)) {              //����һ������һ�β�ֵ��������Ӻ�ӽ�0 ˵������ֵ��������
                motor2.freq =  average_speed ;
             
           } 
           cap_cnt = 0;             //�����������  ��ͳ�Ʋ���������ֵ��0 �ȴ���һ�β�������
           sum_speed = 0;           //�����������  �������ܺ͵�ֵ��0 �ȴ���һ�β�������
        }
        __HAL_TIM_SET_CAPTUREPOLARITY(motor2.htim_ic, motor2.ic_channel, TIM_ICPOLARITY_RISING);      //ת�����벶����Ϊ�����أ��ȴ���һ�β���
        ic_edge = IC_RISE_EDGE;     //�ȴ���һ�����ڵĵ�һ��������
        
    }
}

/**********************************************************************************************************
*�� �� ��: TIM3CaptureChannel1Callback
*����˵��: ͨ�����벶���ܼ�����3ת����Ƶ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void TIM3CaptureChannel1Callback()
{
   static uint8_t ic_edge = IC_RISE_EDGE;
    static uint32_t cap_val_1 = 0;             //��һ��������CCR�Ĵ�����ֵ
    static uint32_t cap_val_2 = 0;             //�ڶ���������CCR�Ĵ�����ֵ
    static int32_t sum_speed = 0;              //��ֵ�˲������ٶ�ֵ�ܺ�
    static int32_t single_speed = 0;           //�����ٶ�ֵ
    static int32_t average_speed = 0;          //�˲�������ƽ���ٶ�
    static int32_t last_average_speed = 0;     //�洢��һ���˲��ó���ƽ���ٶ�
    static uint32_t cap_cnt = 0;               //��¼��������
    static int8_t direct = 0;                  //����
    static uint8_t flag_first = 0;             //��־�Ƿ��ǵ�һ���˲�

    if(ic_edge == IC_RISE_EDGE)                //����⵽������
    {
        motor3.updata = 0;                     //�����ж��¼���¼
        cap_val_1 = HAL_TIM_ReadCapturedValue(motor3.htim_ic, motor3.ic_channel);   //��ȡTIM3 CCR1��ֵ
        ic_edge = IC_FALL_EDGE;                                                     //�ȴ��½���

        /* �����һ��GPIO�ڵĵ�ƽ�仯���жϷ���*/
        if(HAL_GPIO_ReadPin(motor3.IC_GPIO_Port_v, motor3.IC_Pin_v) == GPIO_PIN_RESET)
        {
            direct = BACKWARD;
        }
        else
        {
            direct = FORWARD;
        }
        __HAL_TIM_SET_CAPTUREPOLARITY(motor3.htim_ic, motor3.ic_channel, TIM_ICPOLARITY_FALLING);     //ת�����벶����Ϊ�½���
    }
    
    else                                                                                              //��⵽�½���
    {
        cap_val_2 = HAL_TIM_ReadCapturedValue(motor3.htim_ic, motor3.ic_channel);                     //�ٴζ�ȡTIM3 CCR1��ֵ
        if(motor3.updata == 0)                                                                        //�����β���֮�������δ���
        {
            single_speed = SPEED_PARAM / (cap_val_2 - cap_val_1) * direct;
        }
        else if(motor3.updata == 1)                                                                   //�����β���֮�����һ����������
        {
            single_speed = SPEED_PARAM / ((MAX_TIM_CNT - cap_val_1) + cap_val_2) * direct;
        }
        else                                                                                          //�����β���֮�����N����������
        {
            single_speed = SPEED_PARAM / ((MAX_TIM_CNT - cap_val_1) + MAX_TIM_CNT * (motor3.updata - 1) + cap_val_2) * direct;
        }
        
        sum_speed += single_speed;                              //���������ۼ�
        cap_cnt++;                                              //�ۻ���������
        
        if(cap_cnt == FILTER) {                                 //�������                                   
           cap_cnt = 0;                                     
           last_average_speed = average_speed;                  //������һ���˲�����ٶ�ֵ
           if(flag_first == 0) {
               average_speed = (sum_speed+last_average_speed) / FILTER; //��һ���˲�ʱ last_average_speed=0
               flag_first = 1;
           }
           else {
               average_speed = (sum_speed-single_speed+last_average_speed) / FILTER;    //������˲����������һ���ɼ����ٶ� �滻Ϊ��һ���˲�����ٶ�
           }
           
           if(!(__fabs(average_speed + last_average_speed) < 5)) {              //����һ������һ�β�ֵ��������Ӻ�ӽ�0 ˵������ֵ��������
                motor3.freq =  average_speed ;
           } 
           cap_cnt = 0;             //�����������  ��ͳ�Ʋ���������ֵ��0 �ȴ���һ�β�������
           sum_speed = 0;           //�����������  �������ܺ͵�ֵ��0 �ȴ���һ�β�������
        }
        __HAL_TIM_SET_CAPTUREPOLARITY(motor3.htim_ic, motor3.ic_channel, TIM_ICPOLARITY_RISING);      //ת�����벶����Ϊ�����أ��ȴ���һ�β���
        ic_edge = IC_RISE_EDGE;     //�ȴ���һ�����ڵĵ�һ��������
        
    }
}

/**********************************************************************************************************
*�� �� ��: TIM3CaptureChannel3Callback
*����˵��: ͨ�����벶���ܼ�����4ת����Ƶ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void TIM3CaptureChannel3Callback()
{
   static uint8_t ic_edge = IC_RISE_EDGE;
    static uint32_t cap_val_1 = 0;             //��һ��������CCR�Ĵ�����ֵ
    static uint32_t cap_val_2 = 0;             //�ڶ���������CCR�Ĵ�����ֵ
    static int32_t sum_speed = 0;              //��ֵ�˲������ٶ�ֵ�ܺ�
    static int32_t single_speed = 0;           //�����ٶ�ֵ
    static int32_t average_speed = 0;          //�˲�������ƽ���ٶ�
    static int32_t last_average_speed = 0;     //�洢��һ���˲��ó���ƽ���ٶ�
    static uint32_t cap_cnt = 0;               //��¼��������
    static int8_t direct = 0;                  //����
    static uint8_t flag_first = 0;             //��־�Ƿ��ǵ�һ���˲�

    if(ic_edge == IC_RISE_EDGE)                //����⵽������
    {
        motor4.updata = 0;                     //�����ж��¼���¼
        cap_val_1 = HAL_TIM_ReadCapturedValue(motor4.htim_ic, motor4.ic_channel);   //��ȡTIM3 CCR1��ֵ
        ic_edge = IC_FALL_EDGE;                                                     //�ȴ��½���

        /* �����һ��GPIO�ڵĵ�ƽ�仯���жϷ���*/
        if(HAL_GPIO_ReadPin(motor4.IC_GPIO_Port_v, motor4.IC_Pin_v) == GPIO_PIN_RESET)
        {
            direct = BACKWARD;
        }
        else
        {
            direct = FORWARD;
        }
        __HAL_TIM_SET_CAPTUREPOLARITY(motor4.htim_ic, motor4.ic_channel, TIM_ICPOLARITY_FALLING);     //ת�����벶����Ϊ�½���
    }
    
    else                                                                                              //��⵽�½���
    {
        cap_val_2 = HAL_TIM_ReadCapturedValue(motor4.htim_ic, motor4.ic_channel);                     //�ٴζ�ȡTIM3 CCR1��ֵ
        if(motor4.updata == 0)                                                                        //�����β���֮�������δ���
        {
            single_speed = SPEED_PARAM / (cap_val_2 - cap_val_1) * direct;
        }
        else if(motor4.updata == 1)                                                                   //�����β���֮�����һ����������
        {
            single_speed = SPEED_PARAM / ((MAX_TIM_CNT - cap_val_1) + cap_val_2) * direct;
        }
        else                                                                                          //�����β���֮�����N����������
        {
            single_speed = SPEED_PARAM / ((MAX_TIM_CNT - cap_val_1) + MAX_TIM_CNT * (motor4.updata - 1) + cap_val_2) * direct;
        }
        
        sum_speed += single_speed;                              //���������ۼ�
        cap_cnt++;                                              //�ۻ���������
        
        if(cap_cnt == FILTER) {                                 //�������                                   
           cap_cnt = 0;                                     
           last_average_speed = average_speed;                  //������һ���˲�����ٶ�ֵ
           if(flag_first == 0) {
               average_speed = (sum_speed+last_average_speed) / FILTER; //��һ���˲�ʱ last_average_speed=0
               flag_first = 1;
           }
           else {
               average_speed = (sum_speed-single_speed+last_average_speed) / FILTER;    //������˲����������һ���ɼ����ٶ� �滻Ϊ��һ���˲�����ٶ�
           }
           
           if(!(__fabs(average_speed + last_average_speed) < 5)) {              //����һ������һ�β�ֵ��������Ӻ�ӽ�0 ˵������ֵ��������
                motor4.freq =  average_speed ;
           } 
           cap_cnt = 0;             //�����������  ��ͳ�Ʋ���������ֵ��0 �ȴ���һ�β�������
           sum_speed = 0;           //�����������  �������ܺ͵�ֵ��0 �ȴ���һ�β�������
        }
        __HAL_TIM_SET_CAPTUREPOLARITY(motor4.htim_ic, motor4.ic_channel, TIM_ICPOLARITY_RISING);      //ת�����벶����Ϊ�����أ��ȴ���һ�β���
        ic_edge = IC_RISE_EDGE;     //�ȴ���һ�����ڵĵ�һ��������
        
    }
}

/**********************************************************************************************************
*�� �� ��: TIM5_IcOverflowCntCallback
*����˵��: TIM5��ʱ����������� �����ж��¼�����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void TIM5_IcOverflowCntCallback(void)
{
    if(motor1.updata < MAX_TIM_CNT)
    {
        motor1.updata++;
    }
    else
    {
        motor1.updata = 0;
        motor1.freq = 0;
    }

    if(motor2.updata < MAX_TIM_CNT)
    {
        motor2.updata++;
    }
    else
    {
        motor2.updata = 0;
        motor2.freq = 0;
    }
}

/**********************************************************************************************************
*�� �� ��: TIM3_IcOverflowCntCallback
*����˵��: TIM3��ʱ����������� �����ж��¼�����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void TIM3_IcOverflowCntCallback()
{
    if(motor3.updata < MAX_TIM_CNT)
    {
        motor3.updata++;
    }
    else
    {
        motor3.updata = 0;
        motor3.freq = 0;
    }

    if(motor4.updata < MAX_TIM_CNT)
    {
        motor4.updata++;
    }
    else
    {
        motor4.updata = 0;
        motor4.freq = 0;
    }
}


/**********************************************************************************************************
*�� �� ��: HAL_TIM_PeriodElapsedCallback
*����˵��: �жϻص�����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
        TIM3_IcOverflowCntCallback();
    }

    if(htim->Instance == TIM5)
    {
        TIM5_IcOverflowCntCallback();
    }
}

/**********************************************************************************************************
*�� �� ��: HAL_TIM_IC_CaptureCallback
*����˵��: ���벶���¼��ص�����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    static int count = 0;
    if(htim->Instance == TIM5)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            TIM5CaptureChannel1Callback();           //���1���벶��
        }
        else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {  
            TIM5CaptureChannel3Callback();           //���2���벶��
        }
    }

    else if(htim->Instance == TIM3)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            TIM3CaptureChannel1Callback();           //���3���벶��
        }
        else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            TIM3CaptureChannel3Callback();           //���4���벶��
        }
    }
    
    count++;
    
    if(count == 400) {
        count = 0;
        encoder += (__fabs(motor1.freq)+__fabs(motor2.freq)+__fabs(motor3.freq)+__fabs(motor4.freq))/4.0;   //����������·��
    }
}


