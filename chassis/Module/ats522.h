#ifndef __ATS522_H__
#define __ATS522_H__

#include "usart.h"

#define FRAMELEN 0    //framelen�ֶ�����λ��
#define CMDTYPE 1     //cmdtype�ֶ�����λ��
#define COMMAND 2     //cmd�ֶ�����λ��
#define STATUS 2      //status�ֶ�����λ��
#define LENGTH 3      //length�ֶ�����λ��
#define INFO 4        //info�ֶ���ʼλ��

#define ETX 0X03      //����֡��������
#define PRTCLEN 6     //֡��������Ϣ���ȼ�6
#define ATS522_Tx_MAX_DATA_SIZE 54    //ATS522 ��������֡��󳤶�


typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t blockdata[16];
    uint8_t ATQ;
    uint8_t UID[4];
}ATS522_t;

extern ATS522_t ats522;


void receive_ats522_Init(ATS522_t *ats522);
void receive_ats522_IRQ(ATS522_t *ats522);
void Get_ats522(ATS522_t *ats522);

void Clear_SendBUF(uint8_t *buf);

uint8_t send_Request(void);
uint8_t send_Anticoll(void);
uint8_t send_Select(ATS522_t *ats522);
uint8_t send_Authkey(ATS522_t *ats522);
uint8_t send_Read(void);

uint8_t get_ball_id(void);

#endif

