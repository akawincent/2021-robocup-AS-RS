#include "ats522.h"

/***************************  Author:wincent  *************************/
/***************************  date:2021 08    *************************/
/*                  */

/* ats522�������ݰ������Ϣ���� */
#define ATS522_Rx_MAX_DATA_SIZE 54

/* ����֡�������ռ佻��ʹ�� */
#define ATS522_Rx_SPACE_SIZE 2

/* ƹ�һ�����*/
uint8_t ATS522_Rx_BUF[ATS522_Rx_SPACE_SIZE][ATS522_Rx_MAX_DATA_SIZE] = {0};
uint8_t ATS522_Rx_BUF_UPADTED = 0;
uint8_t ATS522_Rx_bit = 0;

/* ����ats522��Ϣ�ṹ�� */
ATS522_t ats522;

/* �������ݻ����� */
uint8_t ATS522_Tx_BUF[ATS522_Tx_MAX_DATA_SIZE];

/* ������������ */
static enum {
    Request,
    Anticoll,
    Select,
    Authkey,
    Read,
}Txstate = Request;

/**********************************************************************
  * ��������receive_ats522_Init
  * ����: ��ʼ������8 DMA
  * ������ats522��Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void receive_ats522_Init(ATS522_t *ats522){
	ats522->huart=&huart8;
	HAL_UARTEx_ReceiveToIdle_DMA(ats522->huart,(uint8_t *)&ATS522_Rx_BUF, 108);
}

/**********************************************************************
  * ��������receive_ats522_IRQ
  * ����: ����DMA�ж�  ����ƹ�һ���������ats522������Ϣ
  * ������ats522��Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void receive_ats522_IRQ(ATS522_t *ats522){
	ATS522_Rx_bit++;
    ATS522_Rx_bit = ATS522_Rx_bit&1;         //ÿ��DMA�������ݴ洢λ�ö��ڱ仯
	ATS522_Rx_BUF_UPADTED = 1;
    HAL_UARTEx_ReceiveToIdle_DMA(ats522->huart,(uint8_t *)&ATS522_Rx_BUF[ATS522_Rx_bit], 108);
//    for(int i=0;i<(ATS522_Rx_BUF[ATS522_Rx_bit][FRAMELEN]);i++) {
//        printf("%x  ",ATS522_Rx_BUF[ATS522_Rx_bit][i]);
//    }
//    printf("\r\n");
}

/**********************************************************************
  * ��������Get_ats522
  * ����: ����ats522Ӧ�𴫻���Ϣ
  * ������ats522��Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
void Get_ats522(ATS522_t *ats522)
{
    uint8_t pos = (ATS522_Rx_bit+1)&1;  //�л�����ʱû�н��н������������ռ�
	uint8_t bcc = 0;
    if(ATS522_Rx_BUF_UPADTED == 1) {
        if(ATS522_Rx_BUF[pos][ATS522_Rx_BUF[pos][FRAMELEN]-1] == ETX) {     //֡βУ��
            //printf("hhh\r\n");
            if(ATS522_Rx_BUF[pos][FRAMELEN] ==ATS522_Rx_BUF[pos][LENGTH] + PRTCLEN) {       //֡��У��
                //printf("hhh\r\n");
                /* ����У��� */
                for(int i=0;i<(ATS522_Rx_BUF[pos][FRAMELEN]-2);i++) {
                    bcc ^= ATS522_Rx_BUF[pos][i];
                }
                //printf("%x\r\n",(~bcc)&0xFF);
                
                if(ATS522_Rx_BUF[pos][ATS522_Rx_BUF[pos][FRAMELEN]-2] == ((~bcc)&0xFF)) {          //У���ͨ��
                    //printf("hhh\r\n");
                    if(ATS522_Rx_BUF[pos][CMDTYPE] == 0X02) {               //���ص�������״̬����
                        
                        //printf("hhh\r\n");
                        /* ����Ӧ��  �������ATQ */
                        if(ATS522_Tx_BUF[COMMAND] == 'A') {                 
                            ats522->ATQ = (ATS522_Rx_BUF[pos][INFO]) | (ATS522_Rx_BUF[pos][INFO+1]<<8);
                        }
                        
                        /* ����ײӦ�� ����������к�UID */
                        if(ATS522_Tx_BUF[COMMAND] == 'B') {
                            for(int i=0;i<4;i++) {
                                ats522->UID[3-i] = ATS522_Rx_BUF[pos][INFO+3-i];
                            }
                        }
                        
                        /* ѡ��Ӧ�� ��������������� */
                        if(ATS522_Tx_BUF[COMMAND] == 'C') {}
                            
                        /* ֤ʵӦ�� ��������������� */    
                        if(ATS522_Tx_BUF[COMMAND] == 'F') {}
                            
                        /* ��ȡӦ�� ������ÿ����� */    
                        if(ATS522_Tx_BUF[COMMAND] == 'G'&&(ATS522_Rx_BUF[pos][FRAMELEN] == 0x16)) {
                            for(int i=0;i<16;i++) {
                                ats522->blockdata[i] = ATS522_Rx_BUF[pos][INFO+i];      
                            }
                        }

                        ATS522_Rx_BUF_UPADTED = 0;        
                    }
                }
            }
        }
    }
}


/**********************************************************************
  * ��������Clear_SendBUF
  * ����: ��ats522�������ݻ�����ȫ������   
  * ������ats522�������ݻ�����
  * ����ֵ:��
***********************************************************************/
void Clear_SendBUF(uint8_t *buf) 
{
	for(int i=0;i<ATS522_Tx_MAX_DATA_SIZE;i++) {
        buf[i] = 0;
    }
}

/**********************************************************************
  * ��������send_Request
  * ����:��ats522ģ�鷢������ָ��    
  * ��������
  * ����ֵ:��
***********************************************************************/
uint8_t send_Request()
{
    char BCC = 0;
    ATS522_Tx_BUF[CMDTYPE] = 0x02;
    ATS522_Tx_BUF[COMMAND] = 'A';
    ATS522_Tx_BUF[LENGTH] = 0x01;  
    ATS522_Tx_BUF[FRAMELEN] = ATS522_Tx_BUF[LENGTH] + PRTCLEN; 
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-1] = ETX;
    ATS522_Tx_BUF[INFO] = 0x52;
    
    for(int i = 0;i < (ATS522_Tx_BUF[FRAMELEN] - 2);i++) {
        BCC ^= ATS522_Tx_BUF[i];
    }
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-2] = ~BCC;
    
    return HAL_UART_Transmit(&huart8,ATS522_Tx_BUF,ATS522_Tx_BUF[FRAMELEN],1000);
}

/**********************************************************************
  * ��������send_Anticoll
  * ����:��ats522ģ�鷢�ͷ���ײָ��    
  * ��������
  * ����ֵ:��
***********************************************************************/
uint8_t send_Anticoll()
{
    char BCC = 0;
    ATS522_Tx_BUF[CMDTYPE] = 0x02;
    ATS522_Tx_BUF[COMMAND] = 'B';
    ATS522_Tx_BUF[LENGTH] = 0x02;  
    ATS522_Tx_BUF[FRAMELEN] = ATS522_Tx_BUF[LENGTH] + PRTCLEN; 
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-1] = ETX;
    ATS522_Tx_BUF[INFO] = 0x93;
    ATS522_Tx_BUF[INFO+1] = 0x00;
    
    for(int i = 0;i < (ATS522_Tx_BUF[FRAMELEN] - 2);i++) {
        BCC ^= ATS522_Tx_BUF[i];
    }
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-2] = ~BCC;
    
    return HAL_UART_Transmit(&huart8,ATS522_Tx_BUF,ATS522_Tx_BUF[FRAMELEN],1000);
}

/**********************************************************************
  * ��������send_Select
  * ����:��ats522ģ�鷢��ѡ��ָ��    
  * ��������
  * ����ֵ:��
***********************************************************************/
uint8_t send_Select(ATS522_t *ats522)
{
    char BCC = 0;
    ATS522_Tx_BUF[CMDTYPE] = 0x02;
    ATS522_Tx_BUF[COMMAND] = 'C';
    ATS522_Tx_BUF[LENGTH] = 0x05;  
    ATS522_Tx_BUF[FRAMELEN] = ATS522_Tx_BUF[LENGTH] + PRTCLEN; 
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-1] = ETX;
    ATS522_Tx_BUF[INFO] = 0X93;
    
    /* ���к� */
    ATS522_Tx_BUF[INFO+1] = ats522->UID[0];
    ATS522_Tx_BUF[INFO+2] = ats522->UID[1];
    ATS522_Tx_BUF[INFO+3] = ats522->UID[2];
    ATS522_Tx_BUF[INFO+4] = ats522->UID[3];
    
    
    for(int i = 0;i < (ATS522_Tx_BUF[FRAMELEN] - 2);i++) {
        BCC ^= ATS522_Tx_BUF[i];
    }
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-2] = ~BCC;
    
    return HAL_UART_Transmit(&huart8,ATS522_Tx_BUF,ATS522_Tx_BUF[FRAMELEN],1000);
}

/**********************************************************************
  * ��������send_Authkey
  * ����:��ats522ģ�鷢��֤ʵָ��    
  * ������ats522��Ϣ�ṹ��
  * ����ֵ:��
***********************************************************************/
uint8_t send_Authkey(ATS522_t *ats522)
{
    char BCC = 0;
    ATS522_Tx_BUF[CMDTYPE] = 0x02;
    ATS522_Tx_BUF[COMMAND] = 'F';
    ATS522_Tx_BUF[LENGTH] = 0x0C;  
    ATS522_Tx_BUF[FRAMELEN] = ATS522_Tx_BUF[LENGTH] + PRTCLEN; 
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-1] = ETX;
    ATS522_Tx_BUF[INFO] = 0x60;                              //��Կ����A
    
    /* ���к� */
    ATS522_Tx_BUF[INFO+1] = ats522->UID[0];
    ATS522_Tx_BUF[INFO+2] = ats522->UID[1];
    ATS522_Tx_BUF[INFO+3] = ats522->UID[2];
    ATS522_Tx_BUF[INFO+4] = ats522->UID[3];
    
    /* ��ԿA */
    ATS522_Tx_BUF[INFO+5] = 0xFF;
    ATS522_Tx_BUF[INFO+6] = 0xFF;
    ATS522_Tx_BUF[INFO+7] = 0xFF;
    ATS522_Tx_BUF[INFO+8] = 0xFF;
    ATS522_Tx_BUF[INFO+9] = 0xFF;
    ATS522_Tx_BUF[INFO+10] = 0xFF;
    
    /* ���� */
    ATS522_Tx_BUF[INFO+11] = 0x01;
    
    for(int i = 0;i < (ATS522_Tx_BUF[FRAMELEN] - 2);i++) {
        BCC ^= ATS522_Tx_BUF[i];
    }
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-2] = ~BCC;
    
    return HAL_UART_Transmit(&huart8,ATS522_Tx_BUF,ATS522_Tx_BUF[FRAMELEN],1000);
}


/**********************************************************************
  * ��������send_Read
  * ����:��ats522ģ�鷢�Ͷ�ȡָ��    
  * ��������
  * ����ֵ:��
***********************************************************************/
uint8_t send_Read()
{
    char BCC = 0;
    ATS522_Tx_BUF[CMDTYPE] = 0x02;
    ATS522_Tx_BUF[COMMAND] = 'G';
    ATS522_Tx_BUF[LENGTH] = 0x01;  
    ATS522_Tx_BUF[FRAMELEN] = ATS522_Tx_BUF[LENGTH] + PRTCLEN; 
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-1] = ETX;
    
    /* ��ȡ���� */
    ATS522_Tx_BUF[INFO] = 0x01;                              
    
    for(int i = 0;i < (ATS522_Tx_BUF[FRAMELEN] - 2);i++) {
        BCC ^= ATS522_Tx_BUF[i];
    }
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-2] = ~BCC;
    
    return HAL_UART_Transmit(&huart8,ATS522_Tx_BUF,ATS522_Tx_BUF[FRAMELEN],1000);
}

uint8_t get_ball_id()
{
    Clear_SendBUF(ATS522_Tx_BUF);
    send_Request();
    HAL_Delay(110);
    
    Clear_SendBUF(ATS522_Tx_BUF);
    send_Anticoll();
    HAL_Delay(110);
    Get_ats522(&ats522);

    Clear_SendBUF(ATS522_Tx_BUF);
    send_Select(&ats522);
    HAL_Delay(110);
    Get_ats522(&ats522);
    
    Clear_SendBUF(ATS522_Tx_BUF);
    send_Authkey(&ats522);
    HAL_Delay(110);
    Get_ats522(&ats522);
    
    Clear_SendBUF(ATS522_Tx_BUF);
    send_Read();
    HAL_Delay(110);
    Get_ats522(&ats522);
    return ats522.blockdata[0];
}


