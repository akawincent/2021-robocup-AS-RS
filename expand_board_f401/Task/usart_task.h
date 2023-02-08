#ifndef __USART_TASK_H__
#define __USART_TASK_H__

#include <stdbool.h>
#include "usart.h"
#include "stm32f4xx_hal.h"

void pack_bar_message(bool frontbar[],bool backbar[],bool sidebar[]);
void send_tracking_message(void);

#endif

