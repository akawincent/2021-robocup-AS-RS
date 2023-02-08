#include "imu.h"

#define UP_BYTE1 		0x55        //帧头1
#define UP_BYTE2 		0x55        //帧头2
#define UP_ATTITUDE     0x01		//姿态角ID

#define RECEIVE_OK  1       //成功解析标志

/* 模块上传有效数据长度 */
#define ATKP_MAX_DATA_SIZE 6

/* 模块上传姿态角数据 */
ATTITUDE_t  attitude;		  

/* 串口接收数据包 */
IMU901_t    rxPacket_imu901;

/* 串口接收数据包流程 */
static enum
{
    waitForStartByte1,
    waitForStartByte2,
    waitForMsgID,
    waitForDataLength,
    waitForData,
    waitForChksum1,
} rxState = waitForStartByte1;

/* 中断接收缓冲区 */
uint8_t IMU901_Rx_buffer;

/* 是否接收陀螺仪数据 */
bool if_imu_open = false;

/**********************************************************************
  * 函数名：receive_imu901_Init
  * 描述: 初始化串口5 
  * 参数：循迹板信息结构体
  * 返回值:无
***********************************************************************/
void receive_imu901_Init(ATTITUDE_t *attitude){
	attitude->huart=&huart5;
    HAL_UART_Receive_IT(attitude->huart,&IMU901_Rx_buffer, 1);
}

/**********************************************************************
  * 函数名：receive_imu901_IRQ
  * 描述: 串口5中断接收一个字节
  * 参数：姿态角信息结构体
  * 返回值:无
***********************************************************************/
void receive_imu901_IRQ(ATTITUDE_t *attitude)
{
     if(RECEIVE_OK==unpacked_imu901(IMU901_Rx_buffer)) {
        Get_imu901(&rxPacket_imu901);
     }
     HAL_UART_Receive_IT(attitude->huart,&IMU901_Rx_buffer, 1); 
}

/**********************************************************************
  * 函数名：unpacked_imu901
  * 描述: 解析imu901传回单个字节信息
  * 参数：缓冲区
  * 返回值:解析成功 1      解析失败 0
***********************************************************************/
uint8_t unpacked_imu901(uint8_t ch)
{
    static uint8_t cksum = 0, dataIndex = 0;

    switch (rxState)
    {
        case waitForStartByte1:
            if (ch == UP_BYTE1)
            {
                rxState = waitForStartByte2;
                rxPacket_imu901.startByte1 = ch;
            }

            cksum = ch;
            break;

        case waitForStartByte2:
            if (ch == UP_BYTE2)
            {
                rxState = waitForMsgID;
                rxPacket_imu901.startByte2 = ch;
            }
            else
            {
                rxState = waitForStartByte1;
            }

            cksum += ch;
            break;

        case waitForMsgID:
			rxPacket_imu901.msgID = ch;
            rxState = waitForDataLength;
            cksum += ch;
            break;

        case waitForDataLength:
            if (ch <= ATKP_MAX_DATA_SIZE)
            {
                rxPacket_imu901.dataLen = ch;
                dataIndex = 0;
                rxState = (ch > 0) ? waitForData : waitForChksum1;	/*ch=0,数据长度为0，校验1*/
                cksum += ch;
            }
            else
            {
                rxState = waitForStartByte1;
            }

            break;

        case waitForData:
            rxPacket_imu901.data[dataIndex] = ch;
            dataIndex++;
            cksum += ch;

            if (dataIndex == rxPacket_imu901.dataLen)
            {
                rxState = waitForChksum1;
            }

            break;

        case waitForChksum1:
            if (cksum == ch)	/*!< 校准正确返回1 */
            {
                rxPacket_imu901.checkSum = cksum;

                return 1;
            }
            else	/*!< 校验错误 */
            {
                rxState = waitForStartByte1;
            }

            rxState = waitForStartByte1;
            break;

        default:
            rxState = waitForStartByte1;
            break;
    }
    return 0;
}


/**********************************************************************
  * 函数名：Get_imu901
  * 描述: 获取imu传回数据帧中的姿态角信息
  * 参数：通讯数据结构体
  * 返回值:无
***********************************************************************/
void Get_imu901(IMU901_t *packet)
{
    /* 姿态角 */
    if (packet->msgID == UP_ATTITUDE)
    {
//        int16_t data = (int16_t) (packet->data[1] << 8) | packet->data[0];
//        attitude.roll = (float) data / 32768 * 180;

//        data = (int16_t) (packet->data[3] << 8) | packet->data[2];
//        attitude.pitch = (float) data / 32768 * 180;
        
        int16_t data = (int16_t) (packet->data[5] << 8) | packet->data[4];
        attitude.yaw = (float) data / 32768 * 180;
    }
}
