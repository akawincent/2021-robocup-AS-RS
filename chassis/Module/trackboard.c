#include "trackboard.h"

#define START_BYTE_1 0x55     //帧头1
#define START_BYTE_2 0x55     //帧头2
#define END_BYTE 0x88         //帧尾

/* 数据包最大信息长度 */
#define TRACK_Rx_MAX_DATA_SIZE 7

/* 数据帧缓冲区空间交替使用 */
#define TRACK_Rx_SPACE_SIZE 2

/* 定义并初始化接收循迹板信息结构体 */
TRACKINGBOARD_t front_bar = {
    .id = FRONT_BAR,
    .ttl_msg = {0},
    .div = 0,
};

TRACKINGBOARD_t side_bar = {
    .id = SIDE_BAR,
    .ttl_msg = {0},
    .div = 0,
};

TRACKINGBOARD_t back_bar = {
    .id = BACK_BAR,
    .ttl_msg = {0},
    .div = 0,
};

/* 乒乓缓冲区*/
uint8_t TRACK_Rx_BUF[TRACK_Rx_SPACE_SIZE][TRACK_Rx_MAX_DATA_SIZE] = {0};
uint8_t TRACK_Rx_BUF_UPADTED = 0;
uint8_t TRACK_Rx_bit = 0;


/**********************************************************************
  * 函数名：receive_trackboard_Init
  * 描述: 初始化串口2 DMA
  * 参数：循迹板信息结构体
  * 返回值:无
***********************************************************************/
void receive_trackboard_Init(TRACKINGBOARD_t *trackboard){
	trackboard->huart=&huart2;
	HAL_UARTEx_ReceiveToIdle_DMA(trackboard->huart,(uint8_t *)&TRACK_Rx_BUF, 14);
}


/**********************************************************************
  * 函数名：receive_trackboard_IRQ
  * 描述: 串口DMA中断  利用乒乓缓冲区接收拓展板发送来的循迹信息
  * 参数：循迹板信息结构体
  * 返回值:无
***********************************************************************/
void receive_trackboard_IRQ(TRACKINGBOARD_t *trackboard){
	TRACK_Rx_bit++;
    TRACK_Rx_bit=TRACK_Rx_bit&1;         //每次DMA接收数据存储位置都在变化
	TRACK_Rx_BUF_UPADTED=1;
    HAL_UARTEx_ReceiveToIdle_DMA(trackboard->huart,(uint8_t *)&TRACK_Rx_BUF[TRACK_Rx_bit], 14);
}

/**********************************************************************
  * 函数名：Get_trackboard
  * 描述: 获取数据帧中的电平信息
  * 参数：循迹板信息结构体
  * 返回值:无
***********************************************************************/
void Get_trackboard(TRACKINGBOARD_t *trackboard)
{
    uint8_t pos = (TRACK_Rx_bit+1)&1;  //切换到此时没有进行接收任务的数组空间
	
    /*接收成功*/
	if(TRACK_Rx_BUF_UPADTED==1){
        /*帧头帧尾校验通过*/
		if((TRACK_Rx_BUF[pos][0]==START_BYTE_1)&&(TRACK_Rx_BUF[pos][1]==START_BYTE_2)&&(TRACK_Rx_BUF[pos][5]==END_BYTE)){			
            
            if(trackboard->id == FRONT_BAR) {
                trackboard->ttl_msg[1] = (TRACK_Rx_BUF[pos][2] & 0x80);
                trackboard->ttl_msg[2] = (TRACK_Rx_BUF[pos][2] & 0x40);
                trackboard->ttl_msg[3] = (TRACK_Rx_BUF[pos][2] & 0x20);
                trackboard->ttl_msg[4] = (TRACK_Rx_BUF[pos][2] & 0x10);
                trackboard->ttl_msg[5] = (TRACK_Rx_BUF[pos][2] & 0x08);
                trackboard->ttl_msg[6] = (TRACK_Rx_BUF[pos][2] & 0x04);
                trackboard->ttl_msg[7] = (TRACK_Rx_BUF[pos][2] & 0x02);
                trackboard->ttl_msg[8] = (TRACK_Rx_BUF[pos][2] & 0x01);            
            }
            
            if(trackboard->id == BACK_BAR) {
                trackboard->ttl_msg[1] = (TRACK_Rx_BUF[pos][3] & 0x80);
                trackboard->ttl_msg[2] = (TRACK_Rx_BUF[pos][3] & 0x40);
                trackboard->ttl_msg[3] = (TRACK_Rx_BUF[pos][3] & 0x20);
                trackboard->ttl_msg[4] = (TRACK_Rx_BUF[pos][3] & 0x10);
                trackboard->ttl_msg[5] = (TRACK_Rx_BUF[pos][3] & 0x08);
                trackboard->ttl_msg[6] = (TRACK_Rx_BUF[pos][3] & 0x04);
                trackboard->ttl_msg[7] = (TRACK_Rx_BUF[pos][3] & 0x02);
                trackboard->ttl_msg[8] = (TRACK_Rx_BUF[pos][3] & 0x01);            
            }
            
            if(trackboard->id == SIDE_BAR) {
                trackboard->ttl_msg[1] = (TRACK_Rx_BUF[pos][4] & 0x80);
                trackboard->ttl_msg[2] = (TRACK_Rx_BUF[pos][4] & 0x40);
                trackboard->ttl_msg[3] = (TRACK_Rx_BUF[pos][4] & 0x20);
                trackboard->ttl_msg[4] = (TRACK_Rx_BUF[pos][4] & 0x10);
                trackboard->ttl_msg[5] = (TRACK_Rx_BUF[pos][4] & 0x08);
                trackboard->ttl_msg[6] = (TRACK_Rx_BUF[pos][4] & 0x04);
                trackboard->ttl_msg[7] = (TRACK_Rx_BUF[pos][4] & 0x02);
                trackboard->ttl_msg[8] = (TRACK_Rx_BUF[pos][4] & 0x01);            
            }
			TRACK_Rx_BUF_UPADTED = 0;	//更新接收事件
		}
	}
}

