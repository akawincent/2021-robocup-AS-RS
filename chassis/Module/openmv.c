#include "openmv.h"
#include "bsp_delay.h"

#define HeadByte1  0x2C      //��������֡֡ͷ1
#define HeadByte2  0x12      //��������֡֡ͷ2
#define CmdByte    0x01      //��������֡����֡
#define StatusByte 0x02      //��������֡״̬֡
#define EndByte    0x5B      //��������֡֡β

/* ����openmv��Ϣ�ṹ�� */
OPENMV_t openmv;

/* ���ݰ������Ϣ���� */
#define OPENMV_Rx_MAX_DATA_SIZE 6

/* ����֡�������ռ佻��ʹ�� */
#define OPENMV_Rx_SPACE_SIZE 2

/* ƹ�һ�����*/
uint8_t OPENMV_Rx_BUF[OPENMV_Rx_SPACE_SIZE][OPENMV_Rx_MAX_DATA_SIZE] = {0};
uint8_t OPENMV_Rx_BUF_UPADTED = 0;
uint8_t OPENMV_Rx_bit = 0;


/**********************************************************************
  * ��������receive_openmv_Init
  * ����: ��ʼ������7 DMA
  * ������openmv��Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void receive_openmv_Init(OPENMV_t *openmv){
	openmv->huart=&huart7;
	HAL_UARTEx_ReceiveToIdle_DMA(openmv->huart,(uint8_t *)&OPENMV_Rx_BUF, 12);
}


/**********************************************************************
  * ��������receive_openmv_IRQ
  * ����: ����DMA�ж�  ����ƹ�һ���������openmv������Ϣ
  * ������openmv��Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void receive_openmv_IRQ(OPENMV_t *openmv){
	OPENMV_Rx_bit++;
    OPENMV_Rx_bit = OPENMV_Rx_bit&1;         //ÿ��DMA�������ݴ洢λ�ö��ڱ仯
	OPENMV_Rx_BUF_UPADTED=1;
    HAL_UARTEx_ReceiveToIdle_DMA(openmv->huart,(uint8_t *)&OPENMV_Rx_BUF[OPENMV_Rx_bit], 12);
}

/**********************************************************************
  * ��������Get_openmv
  * ����: ������openmv���ص�״̬
  * ������openmv��Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void Get_openmv(OPENMV_t *openmv)
{
    uint8_t pos = (OPENMV_Rx_bit+1)&1;  //�л�����ʱû�н��н������������ռ�
	
    if(OPENMV_Rx_BUF_UPADTED == 1) {
        if((OPENMV_Rx_BUF[pos][0] == HeadByte1) && 
            (OPENMV_Rx_BUF[pos][1] == HeadByte2) && 
             (OPENMV_Rx_BUF[pos][2] == StatusByte) && 
              (OPENMV_Rx_BUF[pos][4] == EndByte)) {
                openmv->message = OPENMV_Rx_BUF[pos][3];
                OPENMV_Rx_BUF_UPADTED = 0;	//���½����¼�
        }
    }
}

//void Get_openmv(OPENMV_t *openmv)
//{

//}

/**********************************************************************
  * ��������tell_openmv_to_distinguish_Strip
  * ����: ����openmv ��е���Ѿ����� ���Կ�ʼʶ������ƽ̨
  * ��������
  * ����ֵ:��
***********************************************************************/
void tell_openmv_to_distinguish_Strip()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x01;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
}

/**********************************************************************
  * ��������tell_openmv_to_loosen
  * ����: ����openmv ��е���Ѿ����� �����ɿ�צ��
  * ��������
  * ����ֵ:��
***********************************************************************/
void tell_openmv_to_loosen()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x02;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,200);
}

/**********************************************************************
  * ��������tell_openmv_to_distinguish_Stepped
  * ����: ����openmv ��е���Ѿ����� ���Կ�ʼʶ�����ƽ̨
  * ��������
  * ����ֵ:��
***********************************************************************/
  void tell_openmv_to_distinguish_Stepped()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x03;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
}

/**********************************************************************
  * ��������tell_claw_to_catch()
  * ����: ��е�����½�������ƽ̨��Ӧ�߶� ����צ�ӿ���ץȡ
  * ��������
  * ����ֵ:��
***********************************************************************/
void tell_claw_to_catch()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x04;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
	  delay_ms(100);
}

/**********************************************************************
  * ��������distinguish_red
  * ����: ��֪צ�� ʶ�����
  * ��������
  * ����ֵ:��
***********************************************************************/
void distinguish_red()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x05;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
}

/**********************************************************************
  * ��������distinguish_blue
  * ����: ��֪צ�� ʶ������
  * ��������
  * ����ֵ:��
***********************************************************************/
void distinguish_blue()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x06;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
}

/**********************************************************************
  * ��������tell_openmv_to_transfer
  * ����: ��֪openmv  ��ʼ�����ʶ��
  * ��������
  * ����ֵ:��
***********************************************************************/
void tell_openmv_to_transfer()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x07;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
}

/**********************************************************************
  * ��������tell_openmv_to_catch_stake()
  * ����: ��֪openmv  ��ʼ��׮��ʶ��
  * ��������
  * ����ֵ:��
***********************************************************************/
void tell_openmv_to_catch_stake()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x08;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
}

/**********************************************************************
  * ��������tell_openmv_to_transfer_blue()
  * ����: ��֪openmv  ��ʼ���볡�����ʶ��
  * ��������
  * ����ֵ:��
***********************************************************************/
void tell_openmv_to_transfer_blue()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x09;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
}

/**********************************************************************
  * ��������tell_openmv_to_transfer_red()
  * ����: ��֪openmv  ��ʼ��볡�����ʶ��
  * ��������
  * ����ֵ:��
***********************************************************************/
void tell_openmv_to_transfer_red()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x00;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
}

void tell_openmv_to_catch_red_disc()
{
    uint8_t command[5];
    command[0] = 0x2C;
    command[1] = 0x12;
    command[2] = 0x01;
    command[3] = 0x0A;
    command[4] = 0x5B;
    HAL_UART_Transmit(&huart7,command,5,1000);
}





