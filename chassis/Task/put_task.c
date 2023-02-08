#include "put_task.h"
#include "turntable_task.h"
#include "lsc.h"
#include "openmv.h"
#include "bsp_delay.h"
#include "walk_task.h"
#include "qrcode.h"

/*小球放入仓库顺序*/
uint8_t putBallOrder[9] = {0x33,0x23,0x13,0x32,0x22,0x12,0x31,0x21,0x11};

extern uint8_t IDLocation[12];

/*找到小球信息标志位*/
bool found = false;

uint8_t idx=1;

/**********************************************************************
  * 函数名：scan_qrcode
  * 描述: 扫描二维码 重新定义第一行货仓
  * 参数：无    
  * 返回值:无
***********************************************************************/
void scan_qrcode()
{
  
	
/************这一段千万不要不要不要不要加****************/
	//QR_trigger_1();
	//delay_ms(2);
	//QR_trigger_stop();
  //Get_qrcode(&qrcode);
/************************************/
  switch(idx){
      case 1:putBallOrder[2] = qrcode.message - 0x30 + 0x10;break;
      case 2:putBallOrder[5] = qrcode.message - 0x30 + 0x10;break;
      case 3:putBallOrder[8] = qrcode.message - 0x30 + 0x10;break;
  }
  idx++;
	if((putBallOrder[2]!=0x11)&&(putBallOrder[2]!=0x12)&&(putBallOrder[2]!=0x13)) putBallOrder[2] = 0x13; 
	if((putBallOrder[5]!=0x11)&&(putBallOrder[5]!=0x12)&&(putBallOrder[5]!=0x13)) putBallOrder[5] = 0x12;
	if((putBallOrder[8]!=0x11)&&(putBallOrder[8]!=0x12)&&(putBallOrder[8]!=0x13)) putBallOrder[8] = 0x11;
	qrcode.message = 0;
}  

/**********************************************************************
  * 函数名：look_for_location
  * 描述: 寻找IC卡信息对应的小球坑位
  * 参数：无    
  * 返回值:无
***********************************************************************/
int look_for_location(int z)
{
	int i,j;
	j = -1;
	found = false;
	for(i=0;i<12;i++)
	{
		if(IDLocation[i] == z)
		{
			IDLocation[i] = 0;
			j = i;
			i++;
			found = true;
			break;
		}
	}
	return j;
}

/**********************************************************************
  * 函数名：put_3th
  * 描述: 放置立体仓库第三行的小球
  * 参数：小球IC卡信息的索引    
  * 返回值:无
***********************************************************************/
void put_3th(int i){
	int j,k;
	k=0;
	do{
		j = look_for_location(putBallOrder[i]);
		if(j!=-1)
			{
				span(j);
				//servogroup_catch_storehouse();
        //HAL_Delay(200);
				tell_claw_to_catch();
//				if(k==0){
//					k++;
//					action_group_run(0);
//				}
        //HAL_Delay(200);
				servogroup_storehouse_to_floor_3();
				HAL_Delay(2000); //5和6之间时间一定要给够
				tell_openmv_to_loosen();
        HAL_Delay(100);
				servogroup_floor_3_to_storehouse();
        HAL_Delay(2000);
			}
	}while(found);
}

/**********************************************************************
  * 函数名：put_2th
  * 描述: 放置立体仓库第二行的小球
  * 参数：小球IC卡信息的索引    
  * 返回值:无
***********************************************************************/
void put_2th(int i){
	int j,k;
	k=0;
	do{
		j = look_for_location(putBallOrder[i]);
		if(j!=-1)
			{
				span(j);
				//servogroup_catch_storehouse();
        //HAL_Delay(200);
				tell_claw_to_catch();
//				if(k==0){
//					k++;
//					action_group_run(0);
//				}
        //HAL_Delay(200);
				servogroup_storehouse_to_floor_2();
				HAL_Delay(2000); //5和6之间时间一定要给够
				tell_openmv_to_loosen();
        HAL_Delay(100);
				servogroup_floor_2_to_storehouse();
        HAL_Delay(2000);
			}
	}while(found);
}

/**********************************************************************
  * 函数名：put_1th
  * 描述: 放置立体仓库第一行的小球
  * 参数：小球IC卡信息的索引    
  * 返回值:无
***********************************************************************/
void put_1th(int i){
	int j,k;
	k=0;
	do{
		j = look_for_location(putBallOrder[i]);
		if(j!=-1)
			{
				span(j);
				//servogroup_catch_storehouse();
        //HAL_Delay(200);
				tell_claw_to_catch();
//				if(k==0){
//					k++;
//					action_group_run(0);
//				}
        //HAL_Delay(200);
				servogroup_storehouse_to_floor_1();
				HAL_Delay(2000); //5和6之间时间一定要给够
				tell_openmv_to_loosen();
        HAL_Delay(100);
				servogroup_floor_1_to_storehouse();
        HAL_Delay(2000);
			}
	}while(found);
}

/**********************************************************************
* 函数名：pre_lay()
* 描述: 预赛 将小球存入立体仓库的过程
* 参数：无
* 返回值:无
***********************************************************************/
void pre_lay(){
	int i;	
	walk_one_warehouse_red();
	for(i=0;i<3;i++){
		if(i==0) put_3th(i);
		if(i==1) put_2th(i);
		if(i==2) put_1th(i);			
	}
	walk_one_warehouse_red();  //用速度和时间跑一格仓库
	for(i=3;i<6;i++){
		if(i==3) put_3th(i);
		if(i==4) put_2th(i);
		if(i==5) put_1th(i);			
	}
	walk_line2_to_line_1_red();
	for(i=6;i<9;i++){
		if(i==6) put_3th(i);
		if(i==7) put_2th(i);
		if(i==8) put_1th(i);			
	}
}

/**********************************************************************
  * 函数名：re_and_final_lay
  * 描述: 复赛和决赛 将小球存入立体仓库的过程（使用二维码定义方式）
  * 参数：无
  * 返回值:无
***********************************************************************/
void re_and_final_lay_red()
{
  int i;	
  walk_two_warehouse_red();
//	while(1){
//		if(qrcode.message == 0x31||qrcode.message == 0x32||qrcode.message == 0x33) break;
//	}
	//delay_ms(500);
  //scan_qrcode();
	for(i=0;i<3;i++){
		if(i==0) put_3th(i);
		if(i==1) put_2th(i);
		if(i==2) put_1th(i);			
	}
	walk_line3_to_line2_red();

//	while(1){
//		if(qrcode.message == 0x31||qrcode.message == 0x32||qrcode.message == 0x33) break;
//	}
	//delay_ms(500);
  //scan_qrcode();
	for(i=3;i<6;i++){
		if(i==3) put_3th(i);
		if(i==4) put_2th(i);
		if(i==5) put_1th(i);			
	}
  walk_line2_to_line_1_red();
//	while(1){
//		if(qrcode.message == 0x31||qrcode.message == 0x32||qrcode.message == 0x33) break;
//	}
	//delay_ms(500);
  //scan_qrcode();
	for(i=6;i<9;i++){
		if(i==6) put_3th(i);
		if(i==7) put_2th(i);
		if(i==8) put_1th(i);			
	}
}

/**********************************************************************
  * 函数名：re_and_final_lay
  * 描述: 复赛和决赛 将小球存入立体仓库的过程（使用二维码定义方式）
  * 参数：无
  * 返回值:无
***********************************************************************/
void re_and_final_lay_blue()
{
  int i;	
  walk_two_warehouse_blue();
//	while(1){
//		if(qrcode.message == 0x31||qrcode.message == 0x32||qrcode.message == 0x33) break;
//	}
	//delay_ms(500);
  //scan_qrcode();
	for(i=0;i<3;i++){
		if(i==0) put_3th(i);
		if(i==1) put_2th(i);
		if(i==2) put_1th(i);			
	}
	walk_one_warehouse_blue();  //用速度和时间跑一格仓库
//	while(1){
//		if(qrcode.message == 0x31||qrcode.message == 0x32||qrcode.message == 0x33) break;
//	}
	//delay_ms(500);
 // scan_qrcode();
	for(i=3;i<6;i++){
		if(i==3) put_3th(i);
		if(i==4) put_2th(i);
		if(i==5) put_1th(i);			
	}
  walk_line2_to_line_1_blue();
//	while(1){
//		if(qrcode.message == 0x31||qrcode.message == 0x32||qrcode.message == 0x33) break;
//	}
	//delay_ms(500);
  //scan_qrcode();
	for(i=6;i<9;i++){
		if(i==6) put_3th(i);
		if(i==7) put_2th(i);
		if(i==8) put_1th(i);			
	}
}


