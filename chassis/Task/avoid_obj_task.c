#include "avoid_obj_task.h"
#include "motor_task.h"
#include "motor.h"

#define front_left_IRsensor  HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10)   //«∞∑Ω◊Û≤‡∫ÏÕ‚
#define front_right_IRsensor HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)   //«∞∑Ω”“≤‡∫ÏÕ‚

#define NO_OBJ  1       //Œ¥ºÏ≤‚µΩ’œ∞≠
#define IS_OBJ  0       //ºÏ≤‚µΩ’œ∞≠


void avoid_obj()
{
    if((front_left_IRsensor == IS_OBJ)||(front_right_IRsensor ==IS_OBJ)) {
        set_chassis_speed(0, 0, 0);
        HAL_Delay(300);
        set_chassis_speed(80,0,0);
        HAL_Delay(900);
        set_chassis_speed(0, 0, 0);
        HAL_Delay(300);
        set_chassis_speed(0,80,0);
        HAL_Delay(1500);
        set_chassis_speed(0, 0, 0);
        HAL_Delay(300);
        set_chassis_speed(-80,0,0);
        HAL_Delay(800);
        set_chassis_speed(0, 0, 0);
        HAL_Delay(300);
    }

}
