#include "qrcode.h"

QRCODE_t qrcode;

/* 数据包最大信息长度 */
#define QRCODE_Rx_MAX_DATA_SIZE 9

/* 数据帧缓冲区空间交替使用 */
#define QRCODE_Rx_SPACE_SIZE 2

/* 乒乓缓冲区*/
uint8_t QRCODE_Rx_BUF[QRCODE_Rx_SPACE_SIZE][QRCODE_Rx_MAX_DATA_SIZE] = {0};
uint8_t QRCODE_Rx_BUF_UPADTED = 0;
uint8_t QRCODE_Rx_bit = 0;


/**********************************************************************
  * 函数名：receive_qrcode_Init
  * 描述: 初始化串口4 DMA
  * 参数：二维码信息结构体
  * 返回值:无
***********************************************************************/
void receive_qrcode_Init(QRCODE_t *qrcode){
	qrcode->huart=&huart4;
	HAL_UARTEx_ReceiveToIdle_DMA(qrcode->huart,(uint8_t *)&QRCODE_Rx_BUF, 18);
}


/**********************************************************************
  * 函数名：receive_qrcode_IRQ
  * 描述: 串口DMA中断  利用乒乓缓冲区接收二维码传回信息
  * 参数：二维码信息结构体
  * 返回值:无
***********************************************************************/
void receive_qrcode_IRQ(QRCODE_t *qrcode){
	QRCODE_Rx_bit++;
  QRCODE_Rx_bit = QRCODE_Rx_bit&1;         //每次DMA接收数据存储位置都在变化
	QRCODE_Rx_BUF_UPADTED=1;
  HAL_UARTEx_ReceiveToIdle_DMA(qrcode->huart,(uint8_t *)&QRCODE_Rx_BUF[QRCODE_Rx_bit], 18);
}

/**********************************************************************
  * 函数名：Get_qrcode
  * 描述: 解析出二维码发回数据
  * 参数：二维码信息结构体
  * 返回值:无
***********************************************************************/
void Get_qrcode(QRCODE_t *qrcode)
{
    uint8_t pos = (QRCODE_Rx_bit+1)&1;  //切换到此时没有进行接收任务的数组空间
	
    if(QRCODE_Rx_BUF_UPADTED == 1) {
        if(QRCODE_Rx_BUF[pos][0] == 'H'&&QRCODE_Rx_BUF[pos][1] == 'E'&&QRCODE_Rx_BUF[pos][2] == 'A'&&QRCODE_Rx_BUF[pos][3] == 'D') {
            if(QRCODE_Rx_BUF[pos][5] == 'T'&&QRCODE_Rx_BUF[pos][6] == 'A'&&QRCODE_Rx_BUF[pos][7] == 'I'&&QRCODE_Rx_BUF[pos][8] == 'L') {
                qrcode->message = QRCODE_Rx_BUF[pos][4];        //二维码传回的是字符类型
                QRCODE_Rx_BUF_UPADTED = 0;	//更新接收事件
            }
        }  
    }
}

/**********************************************************************
  * 函数名：QR_trigger_1
  * 描述: 发送设置码 命令触发1模式（发送命令后触发识读，当识读成功或者
          超过“单次识读时长”或者收到“命令触发停止”后，设备停止识读）
  * 参数：无
  * 返回值:无
***********************************************************************/
void QR_trigger_1(){  
	uint8_t command[12]={'$','1','0','8','0','0','0','-','A','D','B','0'};      //发送二维码请求命令$108000-ADB0
    HAL_UART_Transmit(&huart4,command,12,500);
}

/**********************************************************************
  * 函数名：QR_trigger_2
  * 描述: 发送命令后触发持续识读，只有当发送“命令触发停止”后设备才会停止识读；
  * 参数：无
  * 返回值:无
***********************************************************************/
void QR_trigger_2(){
	uint8_t command[12]={'$','1','0','8','0','0','1','-','9','E','8','0'};      //发送二维码请求命令$108000-ADB0
    HAL_UART_Transmit(&huart4,command,12,500);
}

/**********************************************************************
  * 函数名：QR_trigger_2
  * 描述: 发送命令后触发持续识读，只有当发送“命令触发停止”后设备才会停止识读；
  * 参数：无
  * 返回值:无
***********************************************************************/
void QR_trigger_stop(){
	uint8_t command[12]={'$','1','0','8','0','0','3','-','F','8','E','3'};      //发送二维码请求命令$108000-ADB0
    HAL_UART_Transmit(&huart4,command,12,500);
}

/**********************************************************************
  * 函数名：QR_restart
  * 描述: 重启二维码设备
  * 参数：无
  * 返回值:无
***********************************************************************/
void QR_restart(){  
	uint8_t command[12]={'$','0','1','0','5','0','0','-','E','E','1','9'};      //发送二维码重启命令
    HAL_UART_Transmit(&huart4,command,12,500);
}

/**********************************************************************
  * 函数名：QR_user
  * 描述: 恢复用户默认设置
  * 参数：无
  * 返回值:无
***********************************************************************/
void QR_restore_default(){     
	uint8_t command[12]={'$','0','1','0','2','0','2','-','D','9','5','6'};      //恢复用户设置
    HAL_UART_Transmit(&huart4,command,12,500);
}







