#include "time_cnt.h"
#include "motor.h"

volatile uint32_t TIME_ISR_CNT;

//���ڶ�ʱ���������߾���
uint32_t waittime = 0;

//ϵͳʱ��
Time_t Time_Sys;

/**********************************************************************************************************
*�� �� ��: Get_Time_Init
*����˵��: ʱ�����ڼ���ģ���ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//void Get_Time_Init(void)
//{
//	//ʹ�ܶ�ʱ��ʱ��
//	__HAL_RCC_TIM6_CLK_ENABLE();
//
//	//��ʱ����ʼ��
//	htim6.Instance = TIM6;
//	htim6.Init.Prescaler = 108 - 1;
//	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
//	htim6.Init.Period = 10000 - 1;
//	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
//	HAL_TIM_Base_Init(&htim6);
//
//	//ʹ�ܶ�ʱ���ж�
//	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 5, 0);
//    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
//
//	//������ʱ��
//	HAL_TIM_Base_Start_IT(&htim6);
//}

/**********************************************************************************************************
*�� �� ��: TIM6_IRQHandler
*����˵��: ��ʱ��6�жϺ��� �����¼ϵͳ����ʱ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void TIM6_DAC_IRQHandler(void)
{
    static uint16_t Microsecond_Cnt = 0;
    if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)
    {
        //ÿ10ms�Լ�
        TIME_ISR_CNT++;
        Microsecond_Cnt += 10;
        
        chassis_synthetic_control();
        
        if(waittime>0) waittime--;
        
        //1��
        if (Microsecond_Cnt >= 1000)
        {
            
            Microsecond_Cnt = 0;
            Time_Sys.second++;
            //1����
            if (Time_Sys.second >= 60)
            {
                Time_Sys.second = 0;
                Time_Sys.minute++;
                //1Сʱ
                if (Time_Sys.minute >= 60)
                {
                    Time_Sys.minute = 0;
                    Time_Sys.hour++;
                }
            }
        }
        Time_Sys.microsecond = Microsecond_Cnt;
        __HAL_TIM_CLEAR_IT(&htim6, TIM_FLAG_UPDATE);
    }
}

/**********************************************************************************************************
*�� �� ��: Get_Period
*����˵��: ��ȡʱ������
*��    ��: ʱ�����ڽṹ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Get_Time_Period(Testime *Time_Lab)
{
    //�����δ��ʼ��
    if (Time_Lab->inited == 0)
    {
        Time_Lab->inited = 1;
        Time_Lab->Last_Time = Time_Lab->Now_Time = 10000 * TIME_ISR_CNT + TIM6->CNT;
        Time_Lab->Time_Delta = 0;
    }
    Time_Lab->Last_Time = Time_Lab->Now_Time;
    //��λus
    Time_Lab->Now_Time = 10000 * TIME_ISR_CNT + TIM6->CNT;
    Time_Lab->Time_Delta = Time_Lab->Now_Time - Time_Lab->Last_Time;
}
