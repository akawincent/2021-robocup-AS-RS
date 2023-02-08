#ifndef __PUT_TASK_H__
#define __PUT_TASK_H__

#include <stdbool.h>
#include "stm32f7xx_hal.h"

void scan_qrcode(void);
int look_for_location(int z);
void put_3th(int i);
void put_2th(int i);
void put_1th(int i);
void pre_lay(void);
void re_and_final_lay_red(void);
void re_and_final_lay_blue(void);
#endif
