#ifndef __LSC_H__
#define __LSC_H__

#include "usart.h"

void cmd_action_group_run(uint8_t action_group,uint16_t number);
void cmd_action_group_speed(uint8_t action_group,uint16_t speed);
void cmd_action_group_stop(void);
void servogroup_InitInit(void);
void servogroup_Init(void);
void servogroup_switch(void);
void servogroup_catch_Stripplatform(void);
void servogroup_Stripplatform_to_storehouse(void);

void servogroup_Stepped_distinguish(void);
void servogroup_catch_Steppedplatform_3(void);
void servogroup_Steppedplatform_3_to_storehouse(void);
void servogroup_catch_Steppedplatform_2(void);
void servogroup_Steppedplatform_2_to_storehouse(void);
void servogroup_catch_Steppedplatform_1(void);
void servogroup_Steppedplatform_1_to_storehouse(void);

void servogroup_catch_storehouse(void);
void servogroup_storehouse_to_floor_1(void);
void servogroup_floor_1_to_storehouse(void);
void servogroup_storehouse_to_floor_2(void);
void servogroup_floor_2_to_storehouse(void);
void servogroup_storehouse_to_floor_3(void);
void servogroup_floor_3_to_storehouse(void);

void stack_transfer_distinguish_3(void);
void stack_transfer_distinguish_2(void);
void stack_transfer_distinguish_1(void);
void stack_transfer_catch_3(void);
void stack_transfer_catch_2(void);
void stack_transfer_catch_1(void);
void stack_transfer_put_3(void);
void stack_transfer_put_2(void);
void stack_transfer_put_1(void);
void stack_transfer_back_3(void);
void stack_transfer_back_2(void);
void stack_transfer_back_1(void);


void distinguish_staketask(void);
void catch_staketask_A(void);
void put_staketask_A(void);
void catch_staketask_B(void);
void put_staketask_B(void);
void catch_staketask_C(void);
void put_staketask_C(void);
void catch_staketask_D(void);
void put_staketask_D(void);
void catch_staketask_E(void);
void put_staketask_E(void);
void catch_staketask_F(void);
void put_staketask_F(void);
void catch_staketask_G(void);
void put_staketask_G(void);

void servogroup_disc_to_storehouse_blue(void);
void servogroup_catch_disc_blue(void);

#endif 
