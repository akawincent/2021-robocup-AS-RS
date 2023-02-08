#include "catch_task.h"
#include "motor.h"
#include "lsc.h"
#include "openmv.h"
#include "turntable_task.h"
#include "ats522.h"
#include "bsp_delay.h"
#include "walk_task.h"


#define HEIGHT_1 1      //�߶�1
#define HEIGHT_2 2      //�߶�2
#define HEIGHT_3 3      //�߶�3

/*���߶��õĺ���*/
#define SENSOR HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)

/*����ƽ̨�С��͸߶�״̬�л���־λ*/
uint8_t flag = 0;

/*����ƽ̨ץȡ�߶ȱ�־λ*/
uint8_t height = 0; 

/*С��IC���洢˳��*/
uint8_t IDLocation[12] = {0};

extern uint32_t waittime;


/**********************************************************************
  * ��������Catch_Steppedplatform
  * ����: ץȡ����ƽ̨�Ĺ���(��볡)
  * ��������   
  * ����ֵ:��
***********************************************************************/



void Catch_Steppedplatform_red()
{
	//delay_ms(100);
	openmv.message = 0x00;
    for(int i=0;i<2;i++) {
        servogroup_Stepped_distinguish();
				span(i);
        delay_ms(800);
        
			  //delay_ms(1500);
        tell_openmv_to_distinguish_Stepped();
				tell_openmv_to_distinguish_Stepped();
				tell_openmv_to_distinguish_Stepped();
				tell_openmv_to_distinguish_Stepped();
				tell_openmv_to_distinguish_Stepped();
			
			delay_ms(800);
        
        while(1) {
						if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 1&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 0) goto baohu_stepped;
					
            set_chassis_speed(2,12,0);
						if(SENSOR == 0) flag = 1;       //˵�������������Ѿ���������߽���
            //Get_openmv(&openmv);
            if(openmv.message == 0x01) break;
        }
				openmv.message = 0;				
        
        set_chassis_speed(0,0,0);
        
        
        if(SENSOR == 1&&flag == 0){
            height = HEIGHT_2;    
        }
        
        else if(SENSOR == 0) {
            height = HEIGHT_1;
        }   
        
        else if(SENSOR == 1&&flag == 1) {
            height = HEIGHT_3;
        }
         
        switch(height){
            case HEIGHT_1:
                servogroup_catch_Steppedplatform_1();delay_ms(3000);break;
            case HEIGHT_2:
                servogroup_catch_Steppedplatform_2();delay_ms(3000);break;
            case HEIGHT_3:
                servogroup_catch_Steppedplatform_3();delay_ms(3500);break;
        }
				
        tell_claw_to_catch();
        
        switch(height){
            case HEIGHT_1:
                servogroup_Steppedplatform_1_to_storehouse();
                delay_ms(2500);
                break;
            case HEIGHT_2:
                servogroup_Steppedplatform_2_to_storehouse();
                delay_ms(2500);
                break;
            case HEIGHT_3:
                servogroup_Steppedplatform_3_to_storehouse();
                delay_ms(2500);
                break;
        }
        
        tell_openmv_to_loosen();
				delay_ms(500);  
        IDLocation[i]=get_ball_id();  
    }
		
		 
baohu_stepped:
		/* ��ǰ��  ֱ��ƽ̨��Ե */

		
		//delay_ms(2000);
		
    while(1) {
        set_chassis_speed(3, 50, 0);
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 1&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 0){
					set_chassis_speed(0, 0, 0);
					delay_ms(100);
					set_chassis_speed(-20,0,0);
					delay_ms(2000);
					//move_by_encoder(X_direct,-200);
					servogroup_Steppedplatform_3_to_storehouse();	
					break;
				}
			}
		
}


void qiuqiuni_delay()
{
	delay_ms(800);

}

/**********************************************************************
  * ��������Catch_Steppedplatform_blue
  * ����: ץȡ����ƽ̨�Ĺ���(���볡)
  * ��������   
  * ����ֵ:��
***********************************************************************/
void Catch_Steppedplatform_blue()
{
	openmv.message = 0;
	    for(int i=0;i<2;i++) {
				
        servogroup_Stepped_distinguish();
				span(i);
				
				//walk_line2_to_line_1_red();
        
				//qiuqiuni_delay();
			  //delay_ms(100);
        tell_openmv_to_distinguish_Stepped();
				tell_openmv_to_distinguish_Stepped();
				tell_openmv_to_distinguish_Stepped();
				tell_openmv_to_distinguish_Stepped();
				tell_openmv_to_distinguish_Stepped();  
				delay_ms(3000);
				//
        
        while(1) {
            set_chassis_speed(2,-12,0);
						if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 1) goto baohu_stepped_blue;
						if(SENSOR == 0) flag = 1;       //˵�������������Ѿ���������߽���
            //Get_openmv(&openmv);
            if(openmv.message == 0x01) break;
        } 
				openmv.message = 0;				
        
        set_chassis_speed(0,0,0);
        
        if(SENSOR == 1&&flag == 0){
            height = HEIGHT_3;    
        }
        
        else if(SENSOR == 0) {
            height = HEIGHT_1;
        }   
        
        else if(SENSOR == 1&&flag == 1) {
            height = HEIGHT_2;
        }
         
        switch(height){
            case HEIGHT_1:
                servogroup_catch_Steppedplatform_1();delay_ms(3000);break;
            case HEIGHT_2:
                servogroup_catch_Steppedplatform_2();delay_ms(3000);break;
            case HEIGHT_3:
                servogroup_catch_Steppedplatform_3();delay_ms(3500);break;
        }
				
        tell_claw_to_catch();
        
        switch(height){
            case HEIGHT_1:
                servogroup_Steppedplatform_1_to_storehouse();
                delay_ms(2500);
                break;
            case HEIGHT_2:
                servogroup_Steppedplatform_2_to_storehouse();
                delay_ms(2500);
                break;
            case HEIGHT_3:
                servogroup_Steppedplatform_3_to_storehouse();
                delay_ms(2500);
                break;
        }
        
        tell_openmv_to_loosen();
				delay_ms(500);  
        IDLocation[i]=get_ball_id();  
    }
		
		baohu_stepped_blue:
		
		//servogroup_InitInit();
		//delay_ms(2000);
		/* ��ǰ��  ֱ��ƽ̨��Ե */
    while(1) {
        set_chassis_speed(3, -50, 0);
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 1){
					set_chassis_speed(0, 0, 0);
					move_by_encoder(X_direct,-200);
					servogroup_Steppedplatform_2_to_storehouse();
					break;
				}
    }
}


/**********************************************************************
  * ��������Catch_Stripplatform
  * ����: ץȡ����ƽ̨�Ĺ���
  * ��������   
  * ����ֵ:��
***********************************************************************/
void Catch_Stripplatform()
{
    for(int i=4;i<8;i++) {
        servogroup_catch_Stripplatform();
        delay_ms(2900);
        span(i);
        tell_openmv_to_distinguish_Strip();
     
        while(1) {
            set_chassis_speed(0,5,0);
            //Get_openmv(&openmv);
            if (openmv.message == 0x01) break;
        }
  
        set_chassis_speed(0,0,0);
        while(1) {
            //Get_openmv(&openmv);
            if (openmv.message == 0x02) break;
        } 
        servogroup_Stripplatform_to_storehouse();
        delay_ms(1500);
        tell_openmv_to_loosen();
        delay_ms(1500);
        IDLocation[i]=get_ball_id();     
    }
}

/**********************************************************************
  * ��������Catch_disc
  * ����: ����  ץȡԲ�̻�
  * ��������   
  * ����ֵ:��
***********************************************************************/
 void Catch_disc()
{
	//openmv.message = 0;
    for(int i=2;i<7;i++){
			openmv.message = 0;
        servogroup_catch_Stripplatform();
				span(i);
        delay_ms(1500);
        tell_openmv_to_distinguish_Strip();
        
//			  while(1) {
//            //set_chassis_speed(0,5,0);
//            Get_openmv(&openmv);
//            if (openmv.message == 0x01) break;
//        }
			
//				while(1) {
////            //set_chassis_speed(0,5,0);
////            //Get_openmv(&openmv);
//           if (openmv.message == 0x01) break;
//        }
				
				//openmv.message = 0;
			
        while(1) {
            Get_openmv(&openmv);
            if (openmv.message == 0x02) break;
        } 
				openmv.message = 0;
        servogroup_Stripplatform_to_storehouse();
        delay_ms(1500);
        tell_openmv_to_loosen();
        delay_ms(1500);
        IDLocation[i]=get_ball_id();
    }
}

/**********************************************************************
  * ��������final_Catch_disc_red
  * ����: ����  ץȡԲ�̻�
  * ��������   
  * ����ֵ:��
***********************************************************************/
void final_Catch_disc_red()
{
	int delay_wait_red = 2000;
    for(int i=2;i<7;i++){
				openmv.message = 0;
        servogroup_catch_Stripplatform();
				delay_ms(1000);
        span(i);
				delay_ms(delay_wait_red);
        delay_wait_red -= 300;
        tell_openmv_to_distinguish_Strip();
     
        while(1) {
            //set_chassis_speed(0,5,0);
            Get_openmv(&openmv);
            if(openmv.message == 0x01) break;
        }
  
        //set_chassis_speed(0,0,0);
				//delay_ms(100);
        while(1) {
            Get_openmv(&openmv);
            if(openmv.message == 0x02) break;
        } 
				openmv.message = 0;
        servogroup_Stripplatform_to_storehouse();
        delay_ms(1500);
				tell_openmv_to_loosen();
				delay_ms(500);
        IDLocation[i]=get_ball_id();	
    }
}

/**********************************************************************
  * ��������final_Catch_disc_blue
  * ����: ����  ץȡԲ�̻�
  * ��������   
  * ����ֵ:��
***********************************************************************/
void final_Catch_disc_blue()
{
	int delay_wait_blue = 1000;
    for(int i=2;i<7;i++){
				openmv.message = 0;
        servogroup_catch_disc_blue();
        span(i);
				delay_ms(delay_wait_blue);
				delay_wait_blue -=150;
        
        tell_openmv_to_distinguish_Strip();
     
        while(1) {
            //set_chassis_speed(0,5,0);
            Get_openmv(&openmv);
            if(openmv.message == 0x01) break;
        }
  
        //set_chassis_speed(0,0,0);
				//delay_ms(100);
        while(1) {
            Get_openmv(&openmv);
            if(openmv.message == 0x02) break;
        } 
				openmv.message = 0;
        servogroup_disc_to_storehouse_blue();
        delay_ms(2000);
        tell_openmv_to_catch_red_disc();
				delay_ms(500);
        IDLocation[i]=get_ball_id();
    }
}




    