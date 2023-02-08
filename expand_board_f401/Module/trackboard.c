#include "trackboard.h"

/* 存储循迹板返回电平状态*/
bool front_bar[9];
bool back_bar[9];
bool side_bar[9];

/* 计算循迹偏差值所用权重*/
float weight[9] = {0,-4,-3,-2,-1,1,2,3,4};   //左偏为负  右偏为正


/* 前方循迹板所需GPIO口*/
#define    read_PB15_in      HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)          //D1
#define    read_PB14_in      HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)          //D2
#define    read_PB13_in      HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)          //D3
#define    read_PB12_in      HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)          //D4
#define    read_PB10_in      HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)          //D5
#define    read_PB1_in       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)           //D6
#define    read_PB0_in       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)           //D7
#define    read_PA7_in       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)           //D8

/* 后方循迹板所需GPIO口*/
#define    read_PC15_in      HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_15)          //D1
#define    read_PC14_in      HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14)          //D2
#define    read_PC13_in      HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)          //D3
#define    read_PB9_in       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)           //D4
#define    read_PB8_in       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)           //D5
#define    read_PB7_in       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)           //D6
#define    read_PB6_in       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)           //D7
#define    read_PB5_in       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)           //D8

/* 侧方循迹板所需GPIO口*/
#define    read_PB4_in       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)           //D1
#define    read_PB3_in       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)           //D2
#define    read_PA15_in      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)          //D3
#define    read_PA12_in      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)          //D4
#define    read_PA11_in      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)          //D5
#define    read_PA10_in      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10)          //D6
#define    read_PA9_in       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9)           //D7
#define    read_PA8_in       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)           //D8

/**********************************************************************
  * 函数名：read_frontbar
  * 描述: 读取前方循迹板激光检测返回电平状态 ###黑色返回0 白色返回1###
  * 参数：无
  * 返回值:无
***********************************************************************/
void read_frontbar(){
    front_bar[1] = !read_PB15_in;                  
    front_bar[2] = !read_PB14_in;
    front_bar[3] = !read_PB13_in;
    front_bar[4] = !read_PB12_in;
    front_bar[5] = !read_PB10_in;
    front_bar[6] = !read_PB1_in;
    front_bar[7] = !read_PB0_in;
    front_bar[8] = !read_PA7_in;
}

/**********************************************************************
  * 函数名：read_backbar
  * 描述: 读取后方循迹板激光检测返回电平状态 ###黑色返回0 白色返回1###
  * 参数：无
  * 返回值:无
***********************************************************************/
void read_backbar(){
    back_bar[1] = !read_PC15_in;                  
    back_bar[2] = !read_PC14_in;
    back_bar[3] = !read_PC13_in;
    back_bar[4] = !read_PB9_in;
    back_bar[5] = !read_PB8_in;
    back_bar[6] = !read_PB7_in;
    back_bar[7] = !read_PB6_in;
    back_bar[8] = !read_PB5_in;
}

/**********************************************************************
  * 函数名：read_sidebar
  * 描述: 读取侧方循迹板激光检测返回电平状态 ###黑色返回0 白色返回1###
  * 参数：无
  * 返回值:无
***********************************************************************/
void read_sidebar(){
    side_bar[1] = !read_PB4_in;                  
    side_bar[2] = !read_PB3_in;
    side_bar[3] = !read_PA15_in;
    side_bar[4] = !read_PA12_in;
    side_bar[5] = !read_PA11_in;
    side_bar[6] = !read_PA10_in;
    side_bar[7] = !read_PA9_in;
    side_bar[8] = !read_PA8_in;
}

/**********************************************************************
  * 函数名：calculate_diviation
  * 描述: 计算循迹板的偏离值
  * 参数：电平信息数组   配套权重
  * 返回值:偏离路径值
***********************************************************************/
int calculate_diviation(bool bar[],float weight[])
{
    int div=0;
    for(int i=1;i<=8;i++){
        div+=bar[i]*weight[i];
    }
    return div;
}
