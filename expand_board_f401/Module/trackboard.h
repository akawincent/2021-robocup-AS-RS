#ifndef __TRACKBOARD_H__
#define __TRACKBOARD_H__

#include "stm32f4xx_hal.h"
#include <stdbool.h>

void read_frontbar(void);
void read_backbar(void);
void read_sidebar(void);

int calculate_diviation(bool bar[],float weight[]);

#endif
