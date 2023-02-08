#ifndef __IMU_TASK_H__
#define __IMU_TASK_H__

#include "pid.h"
#include "imu.h"

void self_turn(int angle);
void imu_calibration(void);
void set_imu_status(ATTITUDE_t *attitude,bool status);
void set_turn_angle(ATTITUDE_t *attitude,int target_angle);
void imu_pid_data_init(pid_data_t *imu_pid_data);
int imu_pid(ATTITUDE_t *attitude);

#endif
