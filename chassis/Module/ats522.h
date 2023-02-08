#ifndef __ATS522_H__
#define __ATS522_H__

#include "usart.h"

#define FRAMELEN 0    //framelen字段所在位置
#define CMDTYPE 1     //cmdtype字段所在位置
#define COMMAND 2     //cmd字段所在位置
#define STATUS 2      //status字段所在位置
#define LENGTH 3      //length字段所在位置
#define INFO 4        //info字段起始位置

#define ETX 0X03      //数据帧结束符号
#define PRTCLEN 6     //帧长等于信息长度加6
#define ATS522_Tx_MAX_DATA_SIZE 54    //ATS522 传输数据帧最大长度


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

