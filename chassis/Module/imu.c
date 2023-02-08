#include "imu.h"

#define UP_BYTE1 		0x55        //֡ͷ1
#define UP_BYTE2 		0x55        //֡ͷ2
#define UP_ATTITUDE     0x01		//��̬��ID

#define RECEIVE_OK  1       //�ɹ�������־

/* ģ���ϴ���Ч���ݳ��� */
#define ATKP_MAX_DATA_SIZE 6

/* ģ���ϴ���̬������ */
ATTITUDE_t  attitude;		  

/* ���ڽ������ݰ� */
IMU901_t    rxPacket_imu901;

/* ���ڽ������ݰ����� */
static enum
{
    waitForStartByte1,
    waitForStartByte2,
    waitForMsgID,
    waitForDataLength,
    waitForData,
    waitForChksum1,
} rxState = waitForStartByte1;

/* �жϽ��ջ����� */
uint8_t IMU901_Rx_buffer;

/* �Ƿ�������������� */
bool if_imu_open = false;

/**********************************************************************
  * ��������receive_imu901_Init
  * ����: ��ʼ������5 
  * ������ѭ������Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void receive_imu901_Init(ATTITUDE_t *attitude){
	attitude->huart=&huart5;
    HAL_UART_Receive_IT(attitude->huart,&IMU901_Rx_buffer, 1);
}

/**********************************************************************
  * ��������receive_imu901_IRQ
  * ����: ����5�жϽ���һ���ֽ�
  * ��������̬����Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void receive_imu901_IRQ(ATTITUDE_t *attitude)
{
     if(RECEIVE_OK==unpacked_imu901(IMU901_Rx_buffer)) {
        Get_imu901(&rxPacket_imu901);
     }
     HAL_UART_Receive_IT(attitude->huart,&IMU901_Rx_buffer, 1); 
}

/**********************************************************************
  * ��������unpacked_imu901
  * ����: ����imu901���ص����ֽ���Ϣ
  * ������������
  * ����ֵ:�����ɹ� 1      ����ʧ�� 0
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
                rxState = (ch > 0) ? waitForData : waitForChksum1;	/*ch=0,���ݳ���Ϊ0��У��1*/
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
            if (cksum == ch)	/*!< У׼��ȷ����1 */
            {
                rxPacket_imu901.checkSum = cksum;

                return 1;
            }
            else	/*!< У����� */
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
  * ��������Get_imu901
  * ����: ��ȡimu��������֡�е���̬����Ϣ
  * ������ͨѶ���ݽṹ��
  * ����ֵ:��
***********************************************************************/
void Get_imu901(IMU901_t *packet)
{
    /* ��̬�� */
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
