#include "stack_transfer.h"
#include "lsc.h"
#include "walk_task.h"
#include "openmv.h"
#include "bsp_delay.h"
#include "turntable_task.h"
#include "motor.h"
#include "time_cnt.h"
#include "put_task.h"
//#include "motor.h"

#define yes 1       //找到小球
#define no  0       //未找到

bool if_ball = false;

extern uint32_t waittime;

extern uint8_t putBallOrder[9];

int cnt = 0;

/*定义仓库中随机工件的状态*/
int storage_state[4][5] = {no};

/*第四列工件不需要被倒垛的标志位*/
int need_to_transfer[4]={0};

/**********************************************************************
  * 函数名：scan_ball()
  * 描述: 倒垛时的扫描识别
  * 参数：无
  * 返回值:无
***********************************************************************/
void scan_ball()
{
    waittime = 400;
    while(waittime!=0) {
        Get_openmv(&openmv);
        if(openmv.message ==0x01) {
            if_ball = true;
            break;
        }
    }
}

/**********************************************************************
  * 函数名：stack_transfer
  * 描述: 倒垛全过程
  * 参数：无
  * 返回值:无
***********************************************************************/

int error_red_1 = 0;
int error_red_2 = 0;
int error_red_3 = 0;

void stack_transfer_red()
{
	
	tell_openmv_to_transfer_red();
	tell_openmv_to_transfer_red();
	tell_openmv_to_transfer_red();
	tell_openmv_to_transfer_red();
	tell_openmv_to_transfer_red();
	tell_openmv_to_transfer_red();
	tell_openmv_to_transfer_red();
	tell_openmv_to_transfer_red();
	for(int i=0;i<3;i++){
		openmv.message = 0;
		switch(i){
			case 0:
				stack_transfer_distinguish_1();delay_ms(2000);break;
		  case 1:
				stack_transfer_distinguish_2();delay_ms(2000);break;
			case 2:
				stack_transfer_distinguish_3();delay_ms(2000);break;
		}
		
		baohu_transfer:
		if(error_red_1>=2) continue;
		if(error_red_2>=2) continue;
		if(error_red_3>=2) break;
		tell_openmv_to_transfer();
		tell_openmv_to_transfer();
		tell_openmv_to_transfer();
		tell_openmv_to_transfer();
		tell_openmv_to_transfer();
		
		//waittime = 200;
		while(1){
			set_chassis_speed(2,12,0);
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 1&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 0){
				hit_baffle_red();
				if(i == 0) error_red_1++;
				if(i == 1) error_red_2++;
				if(i == 2) error_red_3++;
				goto baohu_transfer;
			}
			if(openmv.message == 0x01) break;
		}
		set_chassis_speed(0,0,0);
		openmv.message = 0;
		
//		baohu_tansfer:
//		 continue;
		//if(waittime != 0) continue;
		
	  switch(i){
			case 0:
				stack_transfer_catch_1();delay_ms(2500);break;
		  case 1:
				stack_transfer_catch_2();delay_ms(2700);break;
			case 2:
				stack_transfer_catch_3();delay_ms(2700);break;
		}
		
		tell_claw_to_catch();
		delay_ms(100);
		
		switch(i){
			case 0:
				stack_transfer_back_1();delay_ms(2500);break;
			case 1:
				stack_transfer_back_2();delay_ms(2700);break;
			case 2:
				stack_transfer_back_3();delay_ms(2700);break;
		}
		hit_baffle_red();
	  walk_to_warehouse_column4_red();
		
		switch(i){
			case 0:
				stack_transfer_put_1();delay_ms(2500);break;
		  case 1:
				stack_transfer_put_2();delay_ms(2700);break;
			case 2:
				stack_transfer_put_3();delay_ms(2700);break;
		}
		tell_openmv_to_loosen();
		delay_ms(500);
		switch(i){
			case 0:
				stack_transfer_back_1();delay_ms(2500);break;
		  case 1:
				stack_transfer_back_2();delay_ms(2700);break;
			case 2:
				stack_transfer_back_3();delay_ms(2700);break;
		}
		//stack_transfer_back_1();delay_ms(2500);
	}
	servogroup_switch();
	delay_ms(2500);
//	int j;
//	j = look_for_location(putBallOrder[0]);
//	if(j!=-1)
//	{
//		span(j);
//	}
	//put_3th(0);
	
}



/**********************************************************************
  * 函数名：stack_transfer
  * 描述: 倒垛全过程
  * 参数：无
  * 返回值:无
***********************************************************************/
//void stack_transfer()
//{
//    servogroup_floor_2_to_storehouse();
//    delay_ms(3000);
//    int i,j;
//    for(i=1;i<=4;i++){
//        
//        walk_one_warehouse();
//        
//        for(j=1;j<=3;j++){
//            
//            if(storage_state[j][i] == no) {
//                switch(j){
//                    case 1:stack_transfer_distinguish_1();delay_ms(3000);break;
//                    case 2:stack_transfer_distinguish_2();delay_ms(3000);break;
//                    case 3:stack_transfer_distinguish_3();delay_ms(3000);break;
//                }
//            
//                tell_openmv_to_transfer();
//                
//                scan_ball();
//                
//                if(if_ball){
//                    if_ball = false;
//                    cnt++;
//                    for(int k=1;k<=4;k++){
//                       storage_state[j][k] = yes;
//                    }
//                    
//                    if(i ==1) {
//                        switch(j){
//                            case 1:servogroup_floor_1_to_storehouse();delay_ms(3000);need_to_transfer[1]=1;break;
//                            case 2:servogroup_floor_2_to_storehouse();delay_ms(3000);need_to_transfer[2]=1;break;
//                            case 3:servogroup_floor_3_to_storehouse();delay_ms(3000);need_to_transfer[3]=1;break;
//                        }
//                        continue;
//                    }
//                    
//                    else{
//                        
//                        span(8+j);
//                        
//                        switch(j){
//                            case 1:stack_transfer_catch_1();delay_ms(3000);break;
//                            case 2:stack_transfer_catch_2();delay_ms(3000);break;
//                            case 3:stack_transfer_catch_3();delay_ms(3000);break;
//                        }
//                        
//                        tell_claw_to_catch();
//                        
//                        while(1) {
//                            Get_openmv(&openmv);
//                            if (openmv.message == 0x02) break;
//                        }
//                        
//                        switch(j){
//                            case 1:servogroup_floor_1_to_storehouse();delay_ms(3000);break;
//                            case 2:servogroup_floor_2_to_storehouse();delay_ms(3000);break;
//                            case 3:servogroup_floor_3_to_storehouse();delay_ms(3000);break;
//                        }
//                      
//                        tell_openmv_to_loosen();
//                        delay_ms(1000);
//                    }
//                }
//                
//                else{
//                    switch(j){
//                        case 1:servogroup_floor_1_to_storehouse();delay_ms(3000);break;
//                        case 2:servogroup_floor_2_to_storehouse();delay_ms(3000);break;
//                        case 3:servogroup_floor_3_to_storehouse();delay_ms(3000);break;
//                    }
//                }      
//            }
//            if(cnt == 3) goto here;
//        }
//    }
//    
//    here:hit_baffle();
//    
//    walk_one_warehouse();
//    
//    for(j=1;j<=3;j++){
//        if(need_to_transfer[j]!=1){
//            span(8+j);
//            delay_ms(200);
//            tell_claw_to_catch();
//            delay_ms(200);
//            
//            switch(j) {
//                case 1:servogroup_storehouse_to_floor_1();delay_ms(3000);break;
//                case 2:servogroup_storehouse_to_floor_2();delay_ms(3000);break;
//                case 3:servogroup_storehouse_to_floor_3();delay_ms(3000);break;
//            }
//            
//            tell_openmv_to_loosen();
//            delay_ms(500);
//            
//            switch(j) {
//                case 1:servogroup_floor_1_to_storehouse();delay_ms(3000);break;
//                case 2:servogroup_floor_2_to_storehouse();delay_ms(3000);break;
//                case 3:servogroup_floor_3_to_storehouse();delay_ms(3000);break;
//            }
//        }  	  
//    } 
//}
/**********************************************************************
  * 函数名：stack_transfer
  * 描述: 倒垛全过程
  * 参数：无
  * 返回值:无
***********************************************************************/
//void stack_transfer()
//{
//	for(int i=0;i<3;i++){
//		
//		switch(i){
//			case 0:
//				stack_transfer_distinguish_1();delay_ms(2000);break;
//		  case 1:
//				stack_transfer_distinguish_2();delay_ms(2000);break;
//			case 2:
//				stack_transfer_distinguish_3();delay_ms(2000);break;
//		}
//		
//		tell_openmv_to_transfer();
//		tell_openmv_to_transfer();
//		tell_openmv_to_transfer();
//		tell_openmv_to_transfer();
//		tell_openmv_to_transfer();
//		
//		while(1){
//			set_chassis_speed(2,7,0);
//			if(openmv.message == 0x01) break;
//		}
//		set_chassis_speed(0,0,0);
//		
//	  switch(i){
//			case 0:
//				stack_transfer_catch_1();delay_ms(2000);break;
//		  case 1:
//				stack_transfer_catch_2();delay_ms(2000);break;
//			case 2:
//				stack_transfer_catch_3();delay_ms(2000);break;
//		}
//		
//		tell_claw_to_catch();
//		delay_ms(100);
//		
//		switch(i){
//			case 0:
//				stack_transfer_back_1();delay_ms(2000);break;
//			case 1:
//				stack_transfer_back_2();delay_ms(2000);break;
//			case 2:
//				stack_transfer_back_3();delay_ms(2000);break;
//		}
//		hit_baffle();
//	  walk_to_warehouse_column4();
//		
//		switch(i){
//			case 0:
//				stack_transfer_catch_1();delay_ms(1500);break;
//		  case 1:
//				stack_transfer_catch_2();delay_ms(1500);break;
//			case 2:
//				stack_transfer_catch_3();delay_ms(1500);break;
//		}
//		tell_openmv_to_loosen();
//		delay_ms(500);
//		switch(i){
//			case 0:
//				stack_transfer_back_1();delay_ms(1500);break;
//		  case 1:
//				stack_transfer_back_2();delay_ms(1500);break;
//			case 2:
//				stack_transfer_back_3();delay_ms(1500);break;
//		}
//		//stack_transfer_back_1();delay_ms(2500);
//	}
//}

//void stack_transfer()
//{
//	int i,j,k;
//	stack_transfer_distinguish_1();
//	tell_openmv_to_transfer();
//	tell_openmv_to_transfer();
//	tell_openmv_to_transfer();
//	tell_openmv_to_transfer();
//	tell_openmv_to_transfer();
//	while(1){
//		set_chassis_speed(0,10,0);
//		while(1){
//			if(openmv.message ==0x01) break;
//		}
//		//scan_ball();
//			
//			if(if_ball){
//				if_ball = false;
//        if(i ==4) break; 
//				else{
//						stack_transfer_catch_1();
//						delay_ms(2500);
//						tell_claw_to_catch();
//						delay_ms(100);
////						while(1) {
////								Get_openmv(&openmv);
////								if (openmv.message == 0x02) break;
////						}
//						stack_transfer_back_1();delay_ms(1500);
//						hit_baffle();
//						walk_to_warehouse_column4();
//						stack_transfer_put_1();delay_ms(2500);
//						tell_openmv_to_loosen();
//						delay_ms(500);
//						stack_transfer_back_1();delay_ms(2500);
//						break;
//			   }
//	    }
//	
//	}
//	for(i=4;i>=1;i--){
//			switch(i){
//				case 4:
//					walk_to_warehouse_column4();
//					stack_transfer_distinguish_1();
//					delay_ms(2500);
//					break;
//				case 3:
//					walk_line4_to_line_3();
//					break;
//				case 2:
//					walk_one_warehouse();
//					break;
//				case 1:
//					walk_line2_to_line_1();
//					break;
//			}
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			scan_ball();
//			
//			if(if_ball){
//				if_ball = false;
//        if(i ==4) break; 
//				else{
//						stack_transfer_catch_1();
//						delay_ms(2500);
//						tell_claw_to_catch();
//						delay_ms(100);
////						while(1) {
////								Get_openmv(&openmv);
////								if (openmv.message == 0x02) break;
////						}
//						stack_transfer_back_1();delay_ms(1500);
//						hit_baffle();
//						walk_to_warehouse_column4();
//						stack_transfer_put_1();delay_ms(2500);
//						tell_openmv_to_loosen();
//						delay_ms(500);
//						stack_transfer_back_1();delay_ms(2500);
//						break;
//			   }
//	    }		
//		}
//	
//	for(j=4;j>=1;j--){
//			switch(j){
//				case 4:
//					//walk_to_warehouse_column4();
//					stack_transfer_distinguish_2();
//					delay_ms(2500);
//					break;
//				case 3:
//					walk_line4_to_line_3();
//					break;
//				case 2:
//					walk_one_warehouse();
//					break;
//				case 1:
//					walk_line2_to_line_1();
//					break;
//			}
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			scan_ball();
//			
//			if(if_ball){
//				if_ball = false;
//        if(j ==3) continue; 
//				else{
//			    stack_transfer_catch_2();
//					delay_ms(2500);
//					tell_claw_to_catch();
//					delay_ms(100);
////          while(1) {
////              Get_openmv(&openmv);
////              if (openmv.message == 0x02) break;
////          }
//					stack_transfer_back_2();delay_ms(1500);
//					hit_baffle();
//					walk_to_warehouse_column4();
//					stack_transfer_put_2();delay_ms(2500);
//					tell_openmv_to_loosen();
//          delay_ms(500);
//					stack_transfer_back_2();delay_ms(2500);
//					break;
//			  }
//	   }
//	 }
//	
//	for(k=4;k>=1;k--){
//			switch(k){
//				case 4:
//					//walk_to_warehouse_column4();
//					stack_transfer_distinguish_3();
//					delay_ms(2500);
//					break;
//				case 3:
//					walk_line4_to_line_3();
//					break;
//				case 2:
//					walk_one_warehouse();
//					break;
//				case 1:
//					walk_line2_to_line_1();
//					break;
//			}
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			tell_openmv_to_transfer();
//			scan_ball();
//			
//			if(if_ball){
//				if_ball = false;
//        if(k ==3) continue; 
//				else{
//						stack_transfer_catch_3();
//						delay_ms(2500);
//						tell_claw_to_catch();
//						delay_ms(100);
////						while(1) {
////								Get_openmv(&openmv);
////								if (openmv.message == 0x02) break;
////						}
//            stack_transfer_back_3();delay_ms(2500);
//						hit_baffle();
//						walk_to_warehouse_column4();
//						stack_transfer_put_3();delay_ms(2500);
//						tell_openmv_to_loosen();
//						delay_ms(500);
//						stack_transfer_back_3();delay_ms(2500);
//						break;
//			  }
//	    }
//   }
//}

/**********************************************************************
  * 函数名：stack_transfer
  * 描述: 倒垛全过程
  * 参数：无
  * 返回值:无
***********************************************************************/

int error_blue_1 = 0;
int error_blue_2 = 0;
int error_blue_3 = 0;

void stack_transfer_blue()
{
	openmv.message = 0;
	tell_openmv_to_transfer_blue();
	tell_openmv_to_transfer_blue();
	tell_openmv_to_transfer_blue();
	tell_openmv_to_transfer_blue();
	tell_openmv_to_transfer_blue();
	for(int i=0;i<3;i++){
		openmv.message = 0;  
		switch(i){
			case 0:
				stack_transfer_distinguish_1();delay_ms(2000);break;
		  case 1:
				stack_transfer_distinguish_2();delay_ms(2000);break;
			case 2:
				stack_transfer_distinguish_3();delay_ms(2000);break;
		}
		
		baohu_transfer_blue:
		if(error_blue_1>=2) continue;
		if(error_blue_2>=2) continue;
		if(error_blue_3>=2) break;
		tell_openmv_to_transfer();
		tell_openmv_to_transfer();
		tell_openmv_to_transfer();
		tell_openmv_to_transfer();
		tell_openmv_to_transfer();
		
		//waittime = 200;
		while(1){
			set_chassis_speed(2,8,0);
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 1&&HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == 0){
				hit_baffle_red();
				walk_to_warehouse_column4_red();
				if(i == 0)error_blue_1++;
				if(i == 1)error_blue_2++;
				if(i == 2)error_blue_3++;
				goto baohu_transfer_blue;
			}
			if(openmv.message == 0x01) break;
		}
		set_chassis_speed(0,0,0);
		openmv.message = 0;
		//if(waittime != 0) continue;
		
	  switch(i){
			case 0:
				stack_transfer_catch_1();delay_ms(2500);break;
		  case 1:
				stack_transfer_catch_2();delay_ms(2700);break;
			case 2:
				stack_transfer_catch_3();delay_ms(2700);break;
		}
		   
		tell_claw_to_catch();
		delay_ms(100);
		
		switch(i){
			case 0:
				stack_transfer_back_1();delay_ms(2500);break;
			case 1:
				stack_transfer_back_2();delay_ms(2700);break;
			case 2:
				stack_transfer_back_3();delay_ms(2700);break;
		}
		hit_baffle_red();
	  walk_to_warehouse_column4_red();
		
		switch(i){
			case 0:
				stack_transfer_put_1();delay_ms(2500);break;
		  case 1:
				stack_transfer_put_2();delay_ms(2700);break;
			case 2:
				stack_transfer_put_3();delay_ms(2700);break;
		}
		tell_openmv_to_loosen();
		delay_ms(100);
		switch(i){
			case 0:
				stack_transfer_back_1();delay_ms(2500);break;
		  case 1:
				stack_transfer_back_2();delay_ms(2700);break;
			case 2:
				stack_transfer_back_3();delay_ms(2700);break;
		}
		//stack_transfer_back_1();delay_ms(2500);
	}
	servogroup_switch();
	delay_ms(2500);

}

