#include "turntable_task.h"
#include "usart.h"
#include "bsp_delay.h"

/*����ż��*/
#define photogate_left HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)
#define photogate_right HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1)

/*�Ƿ����ת����־λ*/
bool span_flag = true;

/**********************************************************************
  * ��������span
  * ����: Բ��ת������Ӧ��ſ�λ
  * ��������  λ    
  * ����ֵ:��
***********************************************************************/
void span(int store_id)
{
  static int zm_now = 0;
	int zm_count,zm_dir;
	//-----------------------------
	if(span_flag && zm_now == 0 && store_id == 0)
	{
		span_flag = false;
		Set_Steering_anticlockwise(&turnover_steering);
		while((photogate_left== 0) || (photogate_right== 0));
		Set_Steering_stop(&turnover_steering);
		return;
	}//��ʼ��span(0)������Խ��
	//-----------------------------
	zm_count = store_id - zm_now;	
	if(zm_count > 6) zm_count -= 12;
	else if(zm_count < -6) zm_count += 12;
	zm_dir = zm_count > 0? 1:-1;
	zm_now += zm_count;
	if(zm_now >= 12)  zm_now -= 12;
	else if(zm_now < 0)	zm_now += 12;
	if(zm_dir == 1)	Set_Steering_clockwise(&turnover_steering);     //pan = TIM->CCR1 //���ƶ�����ٶ�  90  210
	else 	Set_Steering_anticlockwise(&turnover_steering);
	if( (photogate_left== 1) && (photogate_right == 1))zm_count += zm_dir;    //photo �Ǻ��������  &&->||
	//if(photo1 == 1 )zm_count += zm_dir;
	while(zm_count != 0)
	{
    
		if((photogate_left == 1) && (photogate_right == 1))
		{
			//PGout(15) = 0;
			zm_count -= zm_dir;
			if(zm_count == 0)	{
				 Set_Steering_stop(&turnover_steering);
			   return;
		  }	//���
			while((photogate_left == 1) || (photogate_right == 1));
		}
	}
}
