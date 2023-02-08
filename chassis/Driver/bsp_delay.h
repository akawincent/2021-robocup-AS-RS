//
// Created by luohx on 2020/9/25.
//

#ifndef FIRMWARE_CLION_BSP_BSP_DELAY_H_
#define FIRMWARE_CLION_BSP_BSP_DELAY_H_
#include "main.h"

extern void delay_init(void);
extern void delay_us(uint16_t nus);
extern void delay_ms(uint16_t nms);
#endif //FIRMWARE_CLION_BSP_BSP_DELAY_H_
