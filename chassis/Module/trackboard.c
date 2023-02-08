#include "trackboard.h"

#define START_BYTE_1 0x55     //֡ͷ1
#define START_BYTE_2 0x55     //֡ͷ2
#define END_BYTE 0x88         //֡β

/* ���ݰ������Ϣ���� */
#define TRACK_Rx_MAX_DATA_SIZE 7

/* ����֡�������ռ佻��ʹ�� */
#define TRACK_Rx_SPACE_SIZE 2

/* ���岢��ʼ������ѭ������Ϣ�ṹ�� */
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

/* ƹ�һ�����*/
uint8_t TRACK_Rx_BUF[TRACK_Rx_SPACE_SIZE][TRACK_Rx_MAX_DATA_SIZE] = {0};
uint8_t TRACK_Rx_BUF_UPADTED = 0;
uint8_t TRACK_Rx_bit = 0;


/**********************************************************************
  * ��������receive_trackboard_Init
  * ����: ��ʼ������2 DMA
  * ������ѭ������Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void receive_trackboard_Init(TRACKINGBOARD_t *trackboard){
	trackboard->huart=&huart2;
	HAL_UARTEx_ReceiveToIdle_DMA(trackboard->huart,(uint8_t *)&TRACK_Rx_BUF, 14);
}


/**********************************************************************
  * ��������receive_trackboard_IRQ
  * ����: ����DMA�ж�  ����ƹ�һ�����������չ�巢������ѭ����Ϣ
  * ������ѭ������Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void receive_trackboard_IRQ(TRACKINGBOARD_t *trackboard){
	TRACK_Rx_bit++;
    TRACK_Rx_bit=TRACK_Rx_bit&1;         //ÿ��DMA�������ݴ洢λ�ö��ڱ仯
	TRACK_Rx_BUF_UPADTED=1;
    HAL_UARTEx_ReceiveToIdle_DMA(trackboard->huart,(uint8_t *)&TRACK_Rx_BUF[TRACK_Rx_bit], 14);
}

/**********************************************************************
  * ��������Get_trackboard
  * ����: ��ȡ����֡�еĵ�ƽ��Ϣ
  * ������ѭ������Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void Get_trackboard(TRACKINGBOARD_t *trackboard)
{
    uint8_t pos = (TRACK_Rx_bit+1)&1;  //�л�����ʱû�н��н������������ռ�
	
    /*���ճɹ�*/
	if(TRACK_Rx_BUF_UPADTED==1){
        /*֡ͷ֡βУ��ͨ��*/
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
			TRACK_Rx_BUF_UPADTED = 0;	//���½����¼�
		}
	}
}

