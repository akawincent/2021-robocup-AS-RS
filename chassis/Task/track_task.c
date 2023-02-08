#include "track_task.h"

pid_data_t track_pid_data;

/* 循迹pid控制参数 */
pid_paramer_t track_pid_paramer = {
    .integrate_max = 5,
    .kp = 5,
    .ki = 0,
    .kd = 0,
    .control_output_limit = 40
};

/* 计算循迹偏差值所用权重*/
float front_weight[9] = {0,-4,-3,-2,-1,1,2.5,3.5,4.5};   //左偏为负  右偏为正
float back_weight[9] = {-4.5,-3.5,-2.5,-1,1,2,3,4,0};    //左偏为负  右偏为正
												
/**********************************************************************
  * 函数名：set_track_status
  * 描述: 设置是否开启循迹
  * 参数：track_pid_data结构体   状态
  * 返回值:无
***********************************************************************/
void set_track_status(TRACKINGBOARD_t *trackboard,bool status)
{
    trackboard->status = status;
}

/**********************************************************************
  * 函数名：track_pid_data_init
  * 描述: 初始化循迹pid数据
  * 参数：track_pid_data结构体
  * 返回值:无
***********************************************************************/
void track_pid_data_init(pid_data_t *track_pid_data)
{
    track_pid_data->expect = 0;
    track_pid_data->feedback = 0;

    track_pid_data->err = 0;
    track_pid_data->last_err = 0;
    track_pid_data->last2_err=0;
    track_pid_data->integrate = 0;
    track_pid_data->delta = 0;
    track_pid_data->dis_err = 0;

    track_pid_data->control_output = 0;

    track_pid_data->short_circuit_flag = 0;
    track_pid_data->err_callback = NULL;
    track_pid_data->pri_data = NULL;
}

/**********************************************************************
  * 函数名：calculate_diviation
  * 描述: 计算循迹板的偏离值
  * 参数：电平信息数组   配套权重
  * 返回值:偏离路径值
***********************************************************************/
int calculate_diviation(TRACKINGBOARD_t *trackboard,float weight[])
{
    int div = 0;
    for(int i=1;i<=8;i++){
        div += trackboard->ttl_msg[i] * weight[i];
    }
    return div;
}

/**********************************************************************
  * 函数名：track_pid()
  * 描述: 循迹pid
  * 参数：无
  * 返回值:无
***********************************************************************/
int track_pid(TRACKINGBOARD_t *trackboard,float weight[])
{
    if(trackboard->status == true) {
        track_pid_data.expect = 0;
        track_pid_data.feedback = calculate_diviation(trackboard,weight);        
        return pid_positional(&track_pid_data, &track_pid_paramer);    
    }
    else return 0;
}
