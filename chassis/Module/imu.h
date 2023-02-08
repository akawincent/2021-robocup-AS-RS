#ifndef __IMU_H__
#define __IMU_H__

#include "usart.h"
#include <stdbool.h>

/* 模块上传有效数据长度 */
#define ATKP_MAX_DATA_SIZE 6

/*通讯数据结构*/
typedef struct
{
    uint8_t startByte1;
    uint8_t startByte2;
    uint8_t msgID;
    uint8_t dataLen;
    uint8_t data[ATKP_MAX_DATA_SIZE];
    uint8_t checkSum;
} IMU901_t;

extern IMU901_t rxPacket_imu901;

/* 姿态角数据结构体 */
typedef struct
{
    UART_HandleTypeDef *huart;
    float roll;
    float pitch;
    float yaw;
    float target_angle;
    float refer_angle;
    bool status;
} ATTITUDE_t ;

extern bool if_imu_open;

extern ATTITUDE_t attitude;		

void receive_imu901_IRQ(ATTITUDE_t *attitude);
void receive_imu901_Init(ATTITUDE_t *attitude);
uint8_t unpacked_imu901(uint8_t ch);
void Get_imu901(IMU901_t *packet);
#endif
