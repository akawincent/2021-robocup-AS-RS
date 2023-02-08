#include "walk_task.h"
#include "track_task.h"
#include "imu_task.h"
#include "motor.h"
#include "time_cnt.h"
#include "catch_task.h"
#include "lsc.h"
#include "bsp_delay.h"
#include "put_task.h"
#include "openmv.h"
#include "stack_transfer.h"
#include "turntable_task.h"
#include "qrcode.h"

uint8_t red_mode = 0;
uint8_t blue_mode = 0;

int front_bar_sum = 0;
int side_bar_sum = 0;
int back_bar_sum = 0;
//uint8_t IDLocation[12] = {0};

extern uint32_t waittime;

Testime Timer;

extern uint8_t putBallOrder[9];

/**********************************************************************
  * 函数名：blue_or_red
  * 描述: 选择红蓝模式
  * 参数：无
  * 返回值:无
***********************************************************************/
void blue_or_red()
{
    while(1){
        if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == 1) {
            red_mode = 1;
            break;
        }
        else if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15) == 1) {
            blue_mode = 1;
            break;
        }        
    }
    
    if(red_mode) {
        distinguish_red();
				//delay_ms(2000);
				//final_Catch_disc_red();
				path_plan_red();
				//go_home_red();
				//hit_baffle_red();
				//stack_transfer_red();
				//servogroup_Stepped_distinguish();
				//delay_ms(2000);
				//tell_openmv_to_distinguish_Stepped();
    }
    else if(blue_mode) {
        distinguish_blue();
				//delay_ms(2000);
				//final_Catch_disc_b   lue(); 
				//hit_baffle_red ();
				//
			  //delay_ms(2000);
				//
				//go_home_blue();
				path_plan_blue();
				//
				//stack_transfer_blue();
    }
}

void walk_two_warehouse_blue()
{
		waittime =295;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}

void walk_two_warehouse_red()
{
		waittime =295;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}


/**********************************************************************
  * 函数名：walk_to_warehouse_column3
  * 描述: 从仓库边缘走到第三列
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_to_warehouse_column3()
{
    waittime =370;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}

/**********************************************************************
  * 函数名：walk_to_warehouse_column4
  * 描述: 从仓库边缘走到第四列
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_to_warehouse_column4_red()
{
    waittime =160;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}

/**********************************************************************
  * 函数名：walk_to_warehouse_column4
  * 描述: 从仓库边缘走到第四列
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_to_warehouse_column4_blue()
{
    waittime =150;
    while (waittime != 0) {
        set_chassis_speed(0,-20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}


/**********************************************************************
  * 函数名：walk_one_warehouse
  * 描述: 仓库定位（行走一格仓库的距离）
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_one_warehouse_red()
{
    waittime =158;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}

void walk_line3_to_line2_red()
{
	  waittime =158;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}


/**********************************************************************
  * 函数名：walk_one_warehouse
  * 描述: 仓库定位（行走一格仓库的距离）
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_one_warehouse_blue()
{
    waittime =162;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}

/**********************************************************************
  * 函数名：walk_line4_to_line_3
  * 描述: 仓库定位（从第四列走到第三列）
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_line4_to_line_3(){
	waittime =155;
  while (waittime != 0) {
      set_chassis_speed(0,20,0);
  }
  set_chassis_speed(0,0,0);
  delay_ms(500);
}

/**********************************************************************
  * 函数名：walk_to_line_1
  * 描述: 回到第一列仓库
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_to_line_1()
{
    waittime =500;
    while (waittime != 0) {
        set_chassis_speed(0,-20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);

}

/**********************************************************************
  * 函数名：walk_to_line_1
  * 描述: 回到第一列仓库
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_line2_to_line_1_red()
{ 
    waittime =161;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);

}

/**********************************************************************
  * 函数名：walk_to_line_1
  * 描述: 回到第一列仓库
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_line2_to_line_1_blue()
{
    waittime =163;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);

}

/**********************************************************************
  * 函数名：walk_two_warehouse
  * 描述: 仓库定位（行走两格仓库的距离）
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_two_warehouse()
{
    waittime =285;
    while (waittime != 0) {
        set_chassis_speed(0,20,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}

/**********************************************************************
  * 函数名：hit_baffle_red
  * 描述: 撞击挡板
  * 参数：无
  * 返回值:无
***********************************************************************/
void hit_baffle_red()
{
    while(1) {
        set_chassis_speed(30, 0, 0);
        if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == 1&&HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15) == 0) {
            set_chassis_speed(15, 0, 10);
        }
        
        if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == 0&&HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15) == 1) {
            set_chassis_speed(15, 0, -10);
        }
        if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == 1&&HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15) == 1) break;
    }
    set_chassis_speed(0, 0, 0);
    //delay_ms(500);
    
    /* 向后退  直到平台边缘 */
    while(1) {
        set_chassis_speed(2, -45, 0);
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 1) break;
    }
    set_chassis_speed(0, 0, 0);
}

/**********************************************************************
  * 函数名：hit_baffle_blue
  * 描述: 撞击挡板
  * 参数：无
  * 返回值:无
***********************************************************************/
  void hit_baffle_blue()
{
    while(1) {
        set_chassis_speed(30, 0, 0);
        if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == 1&&HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15) == 0) {
            set_chassis_speed(15, 0, 10);
        }
        
        if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == 0&&HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15) == 1) {
            set_chassis_speed(15, 0, -10);
        }
        if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == 1&&HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15) == 1) break;
    }
    set_chassis_speed(0, 0, 0);
    //delay_ms(500);
    
    /* 向后退  直到平台边缘 */
    while(1) {
        set_chassis_speed(2, 45, 0);
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 1&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 0) break;
    }
    set_chassis_speed(0, 0, 0);
}

/**********************************************************************
  * 函数名：walk_to_workspace
  * 描述: 到圆盘机作业区
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_to_workspace_red()
{
		waittime =128;
    while (waittime != 0) {
        set_chassis_speed(0,40,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}

/**********************************************************************
  * 函数名：walk_to_workspace
  * 描述: 到圆盘机作业区
  * 参数：无
  * 返回值:无
***********************************************************************/
void walk_to_workspace_blue()
{
		waittime =150;
    while (waittime != 0) {
        set_chassis_speed(0,-30,0);
    }
    set_chassis_speed(0,0,0);
    delay_ms(500);
}

/**********************************************************************
  * 函数名：go_home
  * 描述: 回家
  * 参数：无
  * 返回值:无
***********************************************************************/

#define front_left_IRsensor  HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10)   //前方左侧红外
#define front_right_IRsensor HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)   //前方右侧红外

#define NO_OBJ  1       //未检测到障碍
#define IS_OBJ  0       //检测到障碍



void go_home_red()
{
	int front_sum = 0;
		set_track_status(&front_bar,false);
		while(1){
			set_chassis_speed(-30,0,0);
			front_sum = front_bar.ttl_msg[3] + front_bar.ttl_msg[4] +front_bar.ttl_msg[5] +front_bar.ttl_msg[6];
			if(front_sum >= 2) break;
		}
		set_chassis_speed(0,0,0);
		delay_ms(500);
		
//		set_chassis_speed(0,85,0);
//		delay_ms(750);
		
		set_track_status(&front_bar,true);

	  while(1){
			set_chassis_speed(0,30,0);
			//front_sum = front_bar.ttl_msg[1]+front_bar.ttl_msg[2]+front_bar.ttl_msg[3]+front_bar.ttl_msg[4]+front_bar.ttl_msg[5]+front_bar.ttl_msg[6]+front_bar.ttl_msg[7]+front_bar.ttl_msg[8];
			
			if(front_left_IRsensor==0||front_right_IRsensor == 0){
				set_track_status(&front_bar,false);
				break;
			}
		}
		set_chassis_speed(0,0,0);
		delay_ms(100);
		
		servogroup_InitInit();
		
		
		//delay_ms(2000);
		
		waittime = 243;
		while(waittime != 0)
		{
			set_chassis_speed(-55,0,0); 
		}
		
		set_chassis_speed(0,0,0);
		delay_ms(500);
		
		waittime = 515;        //205
		while(waittime != 0)
		{
			set_chassis_speed(0,55,0);	
		}
		set_chassis_speed(0,0,0);
}


void go_home_blue()
{
	
	//int back_sum = 0;
	int front_sum_x = 0;
		set_track_status(&front_bar,false);
		while(1){
			set_chassis_speed(-30,0,0);  
			front_sum_x = front_bar.ttl_msg[3] + front_bar.ttl_msg[4] +front_bar.ttl_msg[5] +front_bar.ttl_msg[6];
			if(front_sum_x >= 2) break;
		}
		set_chassis_speed(0,0,0);
		delay_ms(500);
		
		set_chassis_speed(0,0,60);
		delay_ms(2500);
		
		set_track_status(&front_bar,true);
		
//		set_chassis_speed(0,50,0);
//		delay_ms(3000);
		
		
		
		front_sum_x = 0;
		
	  while(1){
			set_chassis_speed(0,30,0);
			if(front_left_IRsensor==0||front_right_IRsensor == 0){
				set_track_status(&front_bar,false);
				break;
			}
		}
		set_chassis_speed(0,0,0);
		delay_ms(100);
		
		servogroup_InitInit();
		//delay_ms(2000);
		
		waittime = 242;
		while(waittime != 0)
		{
			set_chassis_speed(55,0,0);
		}
		
		set_chassis_speed(0,0,0);
		delay_ms(500);
		
		waittime = 520;
		while(waittime != 0)
		{
			set_chassis_speed(0,55,0);	
		}
		set_chassis_speed(0,0,0);
}

/**********************************************************************
  * 函数名：path_plan_red
  * 描述: 路径规划(红半场)
  * 参数：无
  * 返回值:无
***********************************************************************/
void path_plan_red()
{
		set_track_status(&front_bar,false);
	  servogroup_Init();
		delay_ms(1000);
		//servogroup_Init();
    //delay_ms(2000);
	  span(0);
	
		move_by_encoder(Y_direct,1050);
		
		int front_gouba = 0;
	
		while(1){
			set_chassis_speed(0,50,0);
			front_gouba = front_bar.ttl_msg[1]+front_bar.ttl_msg[2]+front_bar.ttl_msg[3]+front_bar.ttl_msg[4]+front_bar.ttl_msg[5]+front_bar.ttl_msg[6]+front_bar.ttl_msg[7]+front_bar.ttl_msg[8];
			if(front_gouba>=6) break;
		}
		
		move_by_encoder(Y_direct,1350);
	
		delay_ms(500);
		move_by_encoder(X_direct,900);
	
	  hit_baffle_red();
		Catch_Steppedplatform_red();
	
	  move_by_encoder(X_direct,-1100);
		delay_ms(500);
		move_by_encoder(Y_direct,500);

		
		int front_1 = 0;
		while(1){
			set_chassis_speed(0,70,0);
			front_1 = front_bar.ttl_msg[1]+front_bar.ttl_msg[2]+front_bar.ttl_msg[3]+front_bar.ttl_msg[4]+front_bar.ttl_msg[5]+front_bar.ttl_msg[6]+front_bar.ttl_msg[7]+front_bar.ttl_msg[8];
			if(front_1>=6) break;
		}
		set_chassis_speed(0,0,0);
		delay_ms(500);
		move_by_encoder(Y_direct,100);
		delay_ms(500);
	  move_by_encoder(X_direct,-600);
		// delay_ms(500);
		set_chassis_speed(0,0,60);
	  delay_ms(2500);
		
		hit_baffle_red();
		walk_to_workspace_red();
		final_Catch_disc_red();
		//Catch_disc();
		
		move_by_encoder(X_direct, -200);
		delay_ms(500);
		
	  waittime = 390;
		while(waittime!=0){
			set_chassis_speed(0,90,0);
		}

		//move_by_encoder(Y_direct, 4500);
		
		hit_baffle_red();
		
		stack_transfer_red();
		hit_baffle_red();
		
		
		qrcode.message = 0;
		int cnt = 0;
		uint8_t last_qrcode = 0;
		uint8_t message = 0;
		set_chassis_speed(2,50,0);
		delay_ms(1000);
		set_chassis_speed(2,12,0);
		while(1){
			message = qrcode.message;
			if(message == 0x31 || message == 0x32 || message == 0x33) {
				if(message != last_qrcode){
					last_qrcode = message;
					if (cnt == 0) {
						putBallOrder[2] = message - 0x20;
					} else if (cnt == 1) {
						putBallOrder[5] = message - 0x20;
					} else if (cnt == 2) {
						putBallOrder[8] = message - 0x20;
					}
					cnt++;
				}
			}
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 1&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 0){
				if (cnt != 3) {
					//没找齐
						putBallOrder[2] = 0x13;
						putBallOrder[5] = 0x12;
						putBallOrder[8] = 0x11;
					
				}
				set_chassis_speed(0,0,0);
				break;
			}	
		}
		
		hit_baffle_red();
		
		//pre_lay();    
		//walk_one_warehouse_red();		
		re_and_final_lay_red();
		
		go_home_red();
}

/**********************************************************************
  * 函数名：path_plan_blue
  * 描述: 路径规划(蓝半场)
  * 参数：无
  * 返回值:无
***********************************************************************/
void path_plan_blue()
{
	set_track_status(&front_bar,false);
	servogroup_Init();
	delay_ms(2000);
	//servogroup_Init();
  //delay_ms(2000);
	span(0);
	
	move_by_encoder(Y_direct,-1500);
	
	int back_gouba = 0;
	while(1){
		set_chassis_speed(0,-50,0);
		back_gouba = front_bar.ttl_msg[1]+front_bar.ttl_msg[2]+ front_bar.ttl_msg[3]+ front_bar.ttl_msg[4]+ front_bar.ttl_msg[5]+ front_bar.ttl_msg[6]+ front_bar.ttl_msg[7]+ front_bar.ttl_msg[8];
		if(back_gouba>=6) break;
	}
	
	move_by_encoder(Y_direct,-800);
	
	
	delay_ms(500);
	move_by_encoder(X_direct,900);

	hit_baffle_blue();
	Catch_Steppedplatform_blue();
	 
	
	move_by_encoder(X_direct,-800);
	delay_ms(500);
	move_by_encoder(Y_direct,-900);
	
	int front_2 = 0;
		while(1){
			set_chassis_speed(0,-70,0);
			front_2 = front_bar.ttl_msg[1]+front_bar.ttl_msg[2]+front_bar.ttl_msg[3]+front_bar.ttl_msg[4]+front_bar.ttl_msg[5]+front_bar.ttl_msg[6]+front_bar.ttl_msg[7]+front_bar.ttl_msg[8];
			if(front_2>=6) break;
		}
	
	move_by_encoder(Y_direct,100);
	
	move_by_encoder(X_direct,-700);
	
	set_chassis_speed(0,0,60);
	delay_ms(2500);
	
	hit_baffle_blue();
	walk_to_workspace_blue();
	final_Catch_disc_blue();
	
	move_by_encoder(X_direct, -200);
	delay_ms(500);
	//move_by_encoder(Y_direct, -4500);
	 
	waittime = 390;
	while(waittime!=0){
		set_chassis_speed(0,-90,0);
	}
	
	
	hit_baffle_red();
	stack_transfer_blue(); 
	hit_baffle_red();
	qrcode.message = 0;
	int cnt = 0;
	uint8_t last_qrcode = 0;
	uint8_t message = 0;
	set_chassis_speed(2,50,0);
	delay_ms(1000);
	set_chassis_speed(2,12,0);
	while(1){
		message = qrcode.message;
		if(message == 0x31 || message == 0x32 || message == 0x33) {
			if(message != last_qrcode){
				last_qrcode = message;
				if (cnt == 0) {
					putBallOrder[2] = message - 0x20;
				} else if (cnt == 1) {
					putBallOrder[5] = message - 0x20;
				} else if (cnt == 2) {
					putBallOrder[8] = message - 0x20;
				}
				cnt++;
			}
		}
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 1 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 0){
			if (cnt != 3) {
				//没找齐
				putBallOrder[2] = 0x13;
				putBallOrder[5] = 0x12;
				putBallOrder[8] = 0x11;
				
			}
			set_chassis_speed(0,0,0);
			break;
		}	
	}
	
	hit_baffle_red();
	
	re_and_final_lay_blue();
	//re_and_final_lay_red();
	hit_baffle_red();
	walk_to_warehouse_column4_red();
	
	go_home_blue();
}
