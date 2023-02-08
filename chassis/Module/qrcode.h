#ifndef __QRCODE_H__
#define __QRCODE_H__

#include "usart.h"
#include <stdbool.h>

typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t message;
    bool status;
} QRCODE_t;

extern QRCODE_t qrcode;

void Get_qrcode(QRCODE_t *qrcode);
void receive_qrcode_IRQ(QRCODE_t *qrcode);
void receive_qrcode_Init(QRCODE_t *qrcode);
void QR_trigger_1(void);
void QR_restart(void);
void QR_restore_default(void);
void QR_trigger_2(void);
void QR_trigger_stop(void);
#endif
