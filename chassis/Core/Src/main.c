/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor_pwm.h"
#include "motor_encoder.h"
#include "motor_config.h"
#include "motor.h"
#include "motor_task.h"
#include "track_task.h"
#include "avoid_obj_task.h"
#include "imu_task.h"
#include "qrcode.h"
#include "ats522.h"
#include "walk_task.h"
#include "turnover_steering.h"
#include "lsc.h"
#include "openmv.h"
#include "turntable_task.h"
#include "catch_task.h"
#include "bsp_delay.h"
#include "stack_transfer.h"
#include "stake_task.h"
 
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint32_t waittime;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_UART5_Init();
  MX_USART3_UART_Init();
  MX_DMA_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  MX_UART7_Init();
  MX_UART8_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  
  /* 初始化系统时钟滴答定时延时功能*/
  delay_init();
  
  /* 初始化时间周期模块*/
  HAL_TIM_Base_Start_IT(&htim6);
  
  /* 初始化周转舵机*/
  Steering_Init(&turnover_steering);
  
  /*初始化串口2 DMA 接收循迹信息*/
  receive_trackboard_Init(&front_bar);
  
  /*初始化串口4 DMA 接收二维码信息*/
  receive_qrcode_Init(&qrcode);
  
  /*初始化串口5  接收imu传回姿态角信息*/
  //receive_imu901_Init(&attitude);
  
  /*初始化串口7  接收openmv传回的动作状态*/
  receive_openmv_Init(&openmv);
  
  /*初始化串口8  接收ats522模块传回IC卡信息*/
  receive_ats522_Init(&ats522);
  
  /* 初始化四个电机并使能*/
  extern motor_t motor1;
  extern motor_t motor2;
  extern motor_t motor3;
  extern motor_t motor4;
 
  motor1_Enable(&motor1);
  motor2_Enable(&motor2);
  motor3_Enable(&motor3);
  motor4_Enable(&motor4);
  
  /* 初始化电机最外层pid数据*/
  extern pid_data_t motor1_pid_data;
  extern pid_data_t motor2_pid_data;
  extern pid_data_t motor3_pid_data;
  extern pid_data_t motor4_pid_data;
  
  motor_pid_data_init(&motor1_pid_data);
  motor_pid_data_init(&motor2_pid_data);
  motor_pid_data_init(&motor3_pid_data);
  motor_pid_data_init(&motor4_pid_data);
  
  /* 初始化循迹pid数据*/
  extern pid_data_t track_pid_data;
  track_pid_data_init(&track_pid_data);
  
  /* 初始化陀螺仪pid数据*/
  //extern pid_data_t imu_pid_data;
  //imu_pid_data_init(&imu_pid_data);
 
  /* 关闭陀螺仪*/
  //set_imu_status(&attitude,false);
  
  /*二维码恢复用户默认设置*/
  QR_restore_default();
	
	/* 使能底盘*/
  change_chassis_status(true);
	
	/* 初始化动作组*/
	servogroup_InitInit(); 
	
	delay_ms(2000);
	/*选择红球还是蓝球*/
	//set_chassis_speed(0,30,0);
	blue_or_red();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART4
                              |RCC_PERIPHCLK_UART5|RCC_PERIPHCLK_UART7
                              |RCC_PERIPHCLK_UART8;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart7ClockSelection = RCC_UART7CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart8ClockSelection = RCC_UART8CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**********************************************************************
  * 函数名：HAL_UARTEx_RxEventCallback
  * 描述: 串口接收中断回调
  * 参数：串口句柄  传输数据大小
  * 返回值:无
***********************************************************************/
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//    
//    /*串口5 中断接收一个陀螺仪传回字节*/
//	if(huart->Instance == attitude.huart->Instance){
//        receive_imu901_IRQ(&attitude); 
//	}
//}

/**********************************************************************
  * 函数名：HAL_UARTEx_RxEventCallback
  * 描述: 串口DMA接收
  * 参数：串口句柄  传输数据大小
  * 返回值:无
***********************************************************************/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	
	/*串口2 DMA模式接受拓展板传回数据*/
		if(huart->Instance == USART2){
			receive_trackboard_IRQ(&front_bar);
			Get_trackboard(&front_bar);
			Get_trackboard(&back_bar);
			//Get_trackboard(&side_bar);
		}
    
    /*串口4 DMA模式接受二维码传回数据*/
    if(huart->Instance == qrcode.huart->Instance) {
			  //printf("%d\r\n",Size);
        receive_qrcode_IRQ(&qrcode);
				Get_qrcode(&qrcode);
    }
    
    /*串口7 DMA模式接受openmv传回数据*/
    if(huart->Instance == openmv.huart->Instance) {
        receive_openmv_IRQ(&openmv);
				Get_openmv(&openmv);
    }
    
    /*串口8 DMA模式接受ats522传回数据*/
    if(huart->Instance == ats522.huart->Instance) {
        receive_ats522_IRQ(&ats522);
    }
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
