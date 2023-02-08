#include "imu_task.h"
#include "bsp_delay.h"

pid_data_t imu_pid_data;

/* 陀螺仪pid控制参数 */
pid_paramer_t imu_pid_paramer = {
    .integrate_max = 10,
    .kp = 10,
    .ki = 0.1,
    .kd = 1,
    .control_output_limit = 60
};

/**********************************************************************
  * 函数名：set_track_status
  * 描述: 设置是否开启陀螺仪
  * 参数：imu_pid_data结构体   状态
  * 返回值:无
***********************************************************************/
void set_imu_status(ATTITUDE_t *attitude,bool status)
{
    attitude->status = status;
}

/**********************************************************************
  * 函数名：imu_pid_data_init
  * 描述: 初始陀螺仪pid数据
  * 参数：imu_pid_data结构体
  * 返回值:无
***********************************************************************/
void imu_pid_data_init(pid_data_t *imu_pid_data)
{
    imu_pid_data->expect = 0;
    imu_pid_data->feedback = 0;

    imu_pid_data->err = 0;
    imu_pid_data->last_err = 0;
    imu_pid_data->last2_err=0;
    imu_pid_data->integrate = 0;
    imu_pid_data->delta = 0;
    imu_pid_data->dis_err = 0;

    imu_pid_data->control_output = 0;

    imu_pid_data->short_circuit_flag = 0;
    imu_pid_data->err_callback = NULL;
    imu_pid_data->pri_data = NULL;
}

/**********************************************************************
  * 函数名：set_turn_angle
  * 描述: 设置目标角度
  * 参数：姿态角数据结构体    目标角度
  * 返回值:无
***********************************************************************/
void set_turn_angle(ATTITUDE_t *attitude,int target_angle)
{
    attitude->target_angle = target_angle;
}

/**********************************************************************
  * 函数名：imu_calibration
  * 描述: 校准当前参考系z轴
  * 参数：无    
  * 返回值:无
***********************************************************************/
void imu_calibration()
{
    float current_yaw = 0, last_yaw = 0;
    uint8_t init_times = 0;
    while(init_times <= 100) {
        current_yaw = attitude.yaw;
        if (__fabs(current_yaw-last_yaw)<10) {
            HAL_Delay(10);
            init_times++;
        } else {
            init_times = 0;
        }
        last_yaw = current_yaw;
    }
    
    attitude.refer_angle = current_yaw;    //基准角度
    //imu.init_ = current_yaw <= 180 ? -current_yaw : 360 - current_yaw;
    HAL_Delay(500);
}

/**********************************************************************
  * 函数名：imu_pid
  * 描述: 陀螺仪pid
  * 参数：姿态角数据结构体    
  * 返回值:无
***********************************************************************/
int imu_pid(ATTITUDE_t *attitude)
{
    if(attitude->status == true) {
        imu_pid_data.expect = attitude->target_angle;
        if ((attitude->yaw - attitude->refer_angle) > 180)
            attitude->yaw -= 360;
        else if ((attitude->yaw - attitude->refer_angle) < -180)
            attitude->yaw += 360;

        imu_pid_data.feedback = attitude->yaw - attitude->refer_angle;
        
        if(__fabs(imu_pid_data.feedback-imu_pid_data.expect)<=3) return 0;   //3度范围内认为已达到目标值
        else return pid_positional(&imu_pid_data, &imu_pid_paramer);
    }
    else return 0;
}

/**********************************************************************
  * 函数名：self_turn
  * 描述: 车体自转
  * 参数：目标转动角度  逆时针正 顺时针负   
  * 返回值:无
***********************************************************************/
void self_turn(int angle)
{
    imu_calibration();
    set_turn_angle(&attitude,angle);  
    set_imu_status(&attitude,true);
    delay_ms(3000);
    set_imu_status(&attitude,false);
    imu_pid_data_init(&imu_pid_data);
}




