#include "ats522.h"

/***************************  Author:wincent  *************************/
/***************************  date:2021 08    *************************/
/*                  */

/* ats522传回数据包最大信息长度 */
#define ATS522_Rx_MAX_DATA_SIZE 54

/* 数据帧缓冲区空间交替使用 */
#define ATS522_Rx_SPACE_SIZE 2

/* 乒乓缓冲区*/
uint8_t ATS522_Rx_BUF[ATS522_Rx_SPACE_SIZE][ATS522_Rx_MAX_DATA_SIZE] = {0};
uint8_t ATS522_Rx_BUF_UPADTED = 0;
uint8_t ATS522_Rx_bit = 0;

/* 定义ats522信息结构体 */
ATS522_t ats522;

/* 发送数据缓冲区 */
uint8_t ATS522_Tx_BUF[ATS522_Tx_MAX_DATA_SIZE];

/* 发送命令流程 */
static enum {
    Request,
    Anticoll,
    Select,
    Authkey,
    Read,
}Txstate = Request;

/**********************************************************************
  * 函数名：receive_ats522_Init
  * 描述: 初始化串口8 DMA
  * 参数：ats522信息结构体
  * 返回值:无
***********************************************************************/
void receive_ats522_Init(ATS522_t *ats522){
	ats522->huart=&huart8;
	HAL_UARTEx_ReceiveToIdle_DMA(ats522->huart,(uint8_t *)&ATS522_Rx_BUF, 108);
}

/**********************************************************************
  * 函数名：receive_ats522_IRQ
  * 描述: 串口DMA中断  利用乒乓缓冲区接收ats522传回信息
  * 参数：ats522信息结构体
  * 返回值:无
***********************************************************************/
void receive_ats522_IRQ(ATS522_t *ats522){
	ATS522_Rx_bit++;
    ATS522_Rx_bit = ATS522_Rx_bit&1;         //每次DMA接收数据存储位置都在变化
	ATS522_Rx_BUF_UPADTED = 1;
    HAL_UARTEx_ReceiveToIdle_DMA(ats522->huart,(uint8_t *)&ATS522_Rx_BUF[ATS522_Rx_bit], 108);
//    for(int i=0;i<(ATS522_Rx_BUF[ATS522_Rx_bit][FRAMELEN]);i++) {
//        printf("%x  ",ATS522_Rx_BUF[ATS522_Rx_bit][i]);
//    }
//    printf("\r\n");
}

/**********************************************************************
  * 函数名：Get_ats522
  * 描述: 解析ats522应答传回信息
  * 参数：ats522信息结构体
  * 返回值:无
***********************************************************************/
void Get_ats522(ATS522_t *ats522)
{
    uint8_t pos = (ATS522_Rx_bit+1)&1;  //切换到此时没有进行接收任务的数组空间
	uint8_t bcc = 0;
    if(ATS522_Rx_BUF_UPADTED == 1) {
        if(ATS522_Rx_BUF[pos][ATS522_Rx_BUF[pos][FRAMELEN]-1] == ETX) {     //帧尾校验
            //printf("hhh\r\n");
            if(ATS522_Rx_BUF[pos][FRAMELEN] ==ATS522_Rx_BUF[pos][LENGTH] + PRTCLEN) {       //帧长校验
                //printf("hhh\r\n");
                /* 计算校验和 */
                for(int i=0;i<(ATS522_Rx_BUF[pos][FRAMELEN]-2);i++) {
                    bcc ^= ATS522_Rx_BUF[pos][i];
                }
                //printf("%x\r\n",(~bcc)&0xFF);
                
                if(ATS522_Rx_BUF[pos][ATS522_Rx_BUF[pos][FRAMELEN]-2] == ((~bcc)&0xFF)) {          //校验和通过
                    //printf("hhh\r\n");
                    if(ATS522_Rx_BUF[pos][CMDTYPE] == 0X02) {               //返回的类型是状态类型
                        
                        //printf("hhh\r\n");
                        /* 请求应答  解析获得ATQ */
                        if(ATS522_Tx_BUF[COMMAND] == 'A') {                 
                            ats522->ATQ = (ATS522_Rx_BUF[pos][INFO]) | (ATS522_Rx_BUF[pos][INFO+1]<<8);
                        }
                        
                        /* 防碰撞应答 解析获得序列号UID */
                        if(ATS522_Tx_BUF[COMMAND] == 'B') {
                            for(int i=0;i<4;i++) {
                                ats522->UID[3-i] = ATS522_Rx_BUF[pos][INFO+3-i];
                            }
                        }
                        
                        /* 选择应答 无需解析返回数据 */
                        if(ATS522_Tx_BUF[COMMAND] == 'C') {}
                            
                        /* 证实应答 无需解析返回数据 */    
                        if(ATS522_Tx_BUF[COMMAND] == 'F') {}
                            
                        /* 读取应答 解析获得块数据 */    
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
  * 函数名：Clear_SendBUF
  * 描述: 将ats522发送数据缓冲区全部清零   
  * 参数：ats522发送数据缓冲区
  * 返回值:无
***********************************************************************/
void Clear_SendBUF(uint8_t *buf) 
{
	for(int i=0;i<ATS522_Tx_MAX_DATA_SIZE;i++) {
        buf[i] = 0;
    }
}

/**********************************************************************
  * 函数名：send_Request
  * 描述:向ats522模块发送请求指令    
  * 参数：无
  * 返回值:无
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
  * 函数名：send_Anticoll
  * 描述:向ats522模块发送防碰撞指令    
  * 参数：无
  * 返回值:无
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
  * 函数名：send_Select
  * 描述:向ats522模块发送选择指令    
  * 参数：无
  * 返回值:无
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
    
    /* 序列号 */
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
  * 函数名：send_Authkey
  * 描述:向ats522模块发送证实指令    
  * 参数：ats522信息结构体
  * 返回值:无
***********************************************************************/
uint8_t send_Authkey(ATS522_t *ats522)
{
    char BCC = 0;
    ATS522_Tx_BUF[CMDTYPE] = 0x02;
    ATS522_Tx_BUF[COMMAND] = 'F';
    ATS522_Tx_BUF[LENGTH] = 0x0C;  
    ATS522_Tx_BUF[FRAMELEN] = ATS522_Tx_BUF[LENGTH] + PRTCLEN; 
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-1] = ETX;
    ATS522_Tx_BUF[INFO] = 0x60;                              //密钥类型A
    
    /* 序列号 */
    ATS522_Tx_BUF[INFO+1] = ats522->UID[0];
    ATS522_Tx_BUF[INFO+2] = ats522->UID[1];
    ATS522_Tx_BUF[INFO+3] = ats522->UID[2];
    ATS522_Tx_BUF[INFO+4] = ats522->UID[3];
    
    /* 密钥A */
    ATS522_Tx_BUF[INFO+5] = 0xFF;
    ATS522_Tx_BUF[INFO+6] = 0xFF;
    ATS522_Tx_BUF[INFO+7] = 0xFF;
    ATS522_Tx_BUF[INFO+8] = 0xFF;
    ATS522_Tx_BUF[INFO+9] = 0xFF;
    ATS522_Tx_BUF[INFO+10] = 0xFF;
    
    /* 块区 */
    ATS522_Tx_BUF[INFO+11] = 0x01;
    
    for(int i = 0;i < (ATS522_Tx_BUF[FRAMELEN] - 2);i++) {
        BCC ^= ATS522_Tx_BUF[i];
    }
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-2] = ~BCC;
    
    return HAL_UART_Transmit(&huart8,ATS522_Tx_BUF,ATS522_Tx_BUF[FRAMELEN],1000);
}


/**********************************************************************
  * 函数名：send_Read
  * 描述:向ats522模块发送读取指令    
  * 参数：无
  * 返回值:无
***********************************************************************/
uint8_t send_Read()
{
    char BCC = 0;
    ATS522_Tx_BUF[CMDTYPE] = 0x02;
    ATS522_Tx_BUF[COMMAND] = 'G';
    ATS522_Tx_BUF[LENGTH] = 0x01;  
    ATS522_Tx_BUF[FRAMELEN] = ATS522_Tx_BUF[LENGTH] + PRTCLEN; 
    ATS522_Tx_BUF[ATS522_Tx_BUF[FRAMELEN]-1] = ETX;
    
    /* 读取块区 */
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


