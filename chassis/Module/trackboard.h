#ifndef __TRACKBOARD_H__
#define __TRACKBOARD_H__

#include "usart.h"
#include <stdbool.h>

/* 循迹板id信息 */
typedef enum {      
    FRONT_BAR,
    SIDE_BAR,
    BACK_BAR,    
} bar_id_t;

/* 循迹板总信息 */
typedef struct {
  UART_HandleTypeDef *huart;
  int16_t div;
  bar_id_t id;
  bool ttl_msg[9];
  bool status;
} TRACKINGBOARD_t;

extern TRACKINGBOARD_t front_bar;
extern TRACKINGBOARD_t side_bar;
extern TRACKINGBOARD_t back_bar;

void receive_trackboard_Init(TRACKINGBOARD_t *trackboard);
void receive_trackboard_IRQ(TRACKINGBOARD_t *trackboard);
void Get_trackboard(TRACKINGBOARD_t *trackboard);
#endif

