#include "usart_task.h"
#include "trackboard.h"

#define START_BYTE_1 0x55     //帧头1
#define START_BYTE_2 0x55     //帧头2
#define END_BYTE 0x88         //帧尾

/* 发送给主控的循迹信息*/
uint8_t TRACKING_BOARD_TX_BUF[6];

/**********************************************************************
  * 函数名：pack_bar_message
  * 描述: 打包循迹信息
  * 参数：无
  * 返回值:无
***********************************************************************/
void pack_bar_message(bool frontbar[],bool backbar[],bool sidebar[]){
    uint8_t front_message = 0; 
    uint8_t back_message = 0;
    uint8_t side_message = 0;
	TRACKING_BOARD_TX_BUF[0] = START_BYTE_1;
	TRACKING_BOARD_TX_BUF[1] = START_BYTE_2;
    for(int i=1; i<=8; i++){
		front_message  <<= 1 ;
        front_message |= frontbar[i];
	}

    for(int i=1; i<=8; i++){
		back_message  <<= 1 ;
        back_message |= backbar[i];
	}
    
    for(int i=1; i<=8; i++){
		side_message  <<= 1 ;
        side_message |= sidebar[i];
	}    
     
    TRACKING_BOARD_TX_BUF[2] = front_message;
    TRACKING_BOARD_TX_BUF[3] = back_message;
    TRACKING_BOARD_TX_BUF[4] = side_message;
	TRACKING_BOARD_TX_BUF[5] = END_BYTE;
}

/**********************************************************************
  * 函数名：send_tracking_message
  * 描述: 向主控发送循迹信息
  * 参数：无
  * 返回值:无
***********************************************************************/
void send_tracking_message()
{
    HAL_UART_Transmit(&huart2,TRACKING_BOARD_TX_BUF,6,50);
}

