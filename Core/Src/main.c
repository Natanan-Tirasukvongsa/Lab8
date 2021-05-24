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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char TxDataBuffer[32] =
{ 0 };
char RxDataBuffer[32] =
{ 0 };

uint8_t STATE_Display = 0;
enum _StateDisplay //enum �?ทนคำเป็นตัวเลข
{
  StateDisplay_MenuRoot_Print =0, //หน้า menu
  StateDisplay_MenuRoot_WaitInput,
  StateDisplay_Menu0_WaitInput,
  StateDisplay_Menu1_WaitInput
};

uint8_t frequency = 1;
int on = 1;
uint64_t _micros = 0;
uint64_t timestamp = 0;
uint32_t LED_TOGGLE_DELAY = 500000; //0.5 second
int check = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
//void UARTRecieveAndResponsePolling();
int16_t UARTRecieveIT();
uint64_t micros();

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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  {
  	  //�?ำหนดข้อความ
  	  char temp[]="HELLO WORLD\r\n please type something to START UART\r\n";
  	  //ไม่ใส่ (uint8_t*) ได้�?ต่ขึ้น warning //1000 ms= timeout ถ้าเ�?ินจา�?นี้จะ fail
  	  HAL_UART_Transmit(&huart2, (uint8_t*) temp, strlen(temp), 1000); //polling
   }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (micros() - timestamp >= LED_TOGGLE_DELAY/frequency && on ==1)
	  			{
	  				timestamp = micros();
	  				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  			}

	  HAL_UART_Receive_IT(&huart2,  (uint8_t*)RxDataBuffer, 32);
	  int16_t inputchar = UARTRecieveIT();
	  if(inputchar!=-1)
	  {
		  sprintf(TxDataBuffer, "ReceivedChar:[%c]\r\n", inputchar);
	  	  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  	  switch(STATE_Display)
	  		{
	  	  	  case StateDisplay_MenuRoot_Print:
				  {
					  char temp1[]="\n"
							  "Frequency default is 1 Hz\r\n"
							  "************\r\n"
							  "   Menu   \r\n"
							  "************\r\n"
							  "Menu 0: LED Control\r\n"
							  "Menu 1: Button Status\r\n"
							  "\n";
					  HAL_UART_Transmit(&huart2, (uint8_t*) temp1, strlen(temp1), 1000);
				  }
	  	  		  STATE_Display = StateDisplay_MenuRoot_WaitInput;
	  	  		  break;

	  	  	  case StateDisplay_MenuRoot_WaitInput:
	  	  		  switch (inputchar)
	  	  		  {

	  	  		  	  case '0':
	  	  		  	  {
	  	  		  	  	 char temp2[]="************\r\n"
	  	  		  	  	  				"Menu 0 : LED Control\r\n"
	  	  		  	  	  				 "************\r\n"
	  	  		  	  	  				"a:Speed Up +1Hz\r\n"
	  	  		  	  	  	  			"************\r\n"
	  	  		  	  	  	  			"s:Speed Down -1Hz\r\n"
	  	  		  	  	  	  			"************\r\n"
	  	  		  	  	  	  			"d:On/Off\r\n"
	  	  		  	  	  	  			"************\r\n"
	  	  		  	  	  	  			"x:Back\r\n"
	  	  		  	  	  				"\n";
	  	  		  	  	  	HAL_UART_Transmit(&huart2, (uint8_t*) temp2, strlen(temp2), 1000);
	  	  		  	  	}
	  	  		  		  STATE_Display = StateDisplay_Menu0_WaitInput;
	  	  		  		  break;

	  	  		  	  case '1':
	  	  		  	  {
	  	  		  		  char temp3[]="************\r\n"
	  	  		  				  	  "Menu 1: Button Status\r\n"
	  	  		  	  	  		  	  "************\r\n"
	  	  		  	  	  		  	  "x:Back\r\n"
	  	  		  	  	  		  	  "************\r\n"
	  	  		  	  	  		  	  "Press/Unpress button to show status\r\n"
	  	  		  	  	  		  	  "************\r\n"
	  	  		  	  	  		  	  "\n";
	  	  		  	  	  HAL_UART_Transmit(&huart2, (uint8_t*) temp3, strlen(temp3), 1000);
	  	  		  	  	}
	  	  		  		  STATE_Display = StateDisplay_Menu1_WaitInput;
	  	  		  		  break;

	  	  		  	  default:
	  	  		  		  sprintf(TxDataBuffer,"unidentified input\r\n");
	  	  		  		  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  	  		  		  STATE_Display = StateDisplay_MenuRoot_WaitInput;
	  	  		  		  break;
	  	  		  }
	  	  		  break;

	  		  	case StateDisplay_Menu0_WaitInput:
	  		  		switch (inputchar)
	  		  		  {

	  		  		  	case 'a':
	  		  		  		  frequency += 1;
	  		  		  		  sprintf(TxDataBuffer,"Frequency is %d Hz\r\n",frequency);
	  		  		  		  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  		  		  		  STATE_Display = StateDisplay_Menu0_WaitInput;
	  		  		  		  break;

	  		  		  	case 's':
	  		  		  		  if (frequency > 1)
	  		  		  		  {
	  		  		  		  	frequency -= 1;
	  		  		  		  	sprintf(TxDataBuffer,"Frequency is %d Hz\r\n",frequency);
	  		  		  		  	HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  		  		  		  }
	  		  		  		  else
								{
									sprintf(TxDataBuffer,"Frequency is limited\r\n");
									HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
								}
	  		  		  		  STATE_Display = StateDisplay_Menu0_WaitInput;
	  		  		  		  break;

	  		  		  	case 'd':
	  		  		  		 if (on == 1)
	  		  		  		 {
	  		  		  		   on =0;
	  		  		  		   sprintf(TxDataBuffer,"LED Off\r\n");
	  		  		  		   HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  		  		  		   HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	  		  		  		 }
	  		  		  		 else
	  		  		  		 {
	  		  		  		  	on = 1;
	  		  		  		  	sprintf(TxDataBuffer,"LED On\r\n");
	  		  		  			 HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  		  		  		  }
	  		  		  		  STATE_Display = StateDisplay_Menu0_WaitInput;
	  		  		  		  break;

	  		  		  	case 'x':
	  		  		  			{
	  		  		  				char temp4[]="\n"
	  		  		  					"************\r\n"
	  		  							"   Menu   \r\n"
	  		  							"************\r\n"
	  		  							"Menu 0: LED Control\r\n"
	  		  							"Menu 1: Button Status\r\n"
	  		  							"\n";
	  		  						HAL_UART_Transmit(&huart2, (uint8_t*) temp4, strlen(temp4), 1000);
	  		  					}
	  		  		  		  STATE_Display = StateDisplay_MenuRoot_WaitInput;
	  		  		  		  break;

	  		  		  	default:
	  		  		  		  sprintf(TxDataBuffer,"unidentified input\r\n");
	  		  		  		  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  		  		  		  STATE_Display = StateDisplay_Menu0_WaitInput;
	  		  		  		  break;
	  		  		  	}
	  		  		break;

	  		  	 case StateDisplay_Menu1_WaitInput:
	  		  		 switch(inputchar)
	  		  		 {
	  		  		 	 case 'x':
	  		  		 	 	 {
	  		  		 	 		 char temp4[]= "\n"
	  		  		 	 				 "************\r\n"
	  		  		 	  		  				"   Menu   \r\n"
	  		  		 	  		  				"************\r\n"
	  		  		 	  		  				"Menu 0: LED Control\r\n"
	  		  		 	  		  				"Menu 1: Button Status\r\n"
	  		  		 	  		  				"\n";
	  		  		 	  		  HAL_UART_Transmit(&huart2, (uint8_t*) temp4, strlen(temp4), 1000);
	  		  		 	  	}
	  		  		 	  	STATE_Display = StateDisplay_MenuRoot_WaitInput;
	  		  		 		 break;

	  		  			default:
	  		  				sprintf(TxDataBuffer,"unidentified input\r\n");
	  		  				HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  		  				STATE_Display = StateDisplay_Menu1_WaitInput;
	  		  				break;
	  		  		}
	  		}

	  }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 99;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
//void UARTRecieveAndResponsePolling()
//{
//	//create buffer
//	char Recieve[32]={0};
//
//	//start recieve in polling mode
//	HAL_UART_Receive(&huart2, (uint8_t*)Recieve, 32, 1000);
//
//	//create feedback text
//	sprintf(TxDataBuffer, "Received:[%s]\r\n", Recieve); //string printf //print ลงใน TxDataBuffer
//
//	//second text //response ว่าได้รับ�?ล้ว
//	HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
//
//}

int16_t UARTRecieveIT()
{
	//store data last position
	static uint32_t dataPos =0;
	//create dummy data
	int16_t data=-1;
	//check pos in buffer vs last position
	//32 size - จำนวนที่เหลือ = ตำ�?หน่งปัจจุบัน
	//มี�?ารพิมพ์ตำ�?หน่งจะไม่เท่า�?ัน
	if(huart2.RxXferSize - huart2.RxXferCount!=dataPos)
	{
		//read data from buffer
		data=RxDataBuffer[dataPos];

		//move to next pos //ตำ�?หน่งข้อมูลล่าสุดที่อ่าน
		dataPos= (dataPos+1)%huart2.RxXferSize;
	}
	return data;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//if(huart == huartxx) ถ้ามีหลายตัว
	sprintf(TxDataBuffer, "Received:[%s]\r\n", RxDataBuffer);
	HAL_UART_Transmit_IT(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer));
	//HAL_UART_Transmit_IT ถ้าข้อมูลส่งเร็วไปข้อมูลแรกยังทำงานไม่เสร็จ ข้อมูลต่อมาจะขาดหาย โค้ดบัค
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim2)
	{
		_micros += 4294967295;
	}
}

uint64_t micros()
{
	return _micros + htim2.Instance->CNT;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) //Interrupt
{
	if (GPIO_Pin == GPIO_PIN_13 && STATE_Display == StateDisplay_Menu1_WaitInput)
	{
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) //falling
		{
			sprintf(TxDataBuffer,"Pressed Button\r\n");
			HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
			STATE_Display = StateDisplay_Menu1_WaitInput;
		}
		else //rising
		{
			sprintf(TxDataBuffer,"Unpressed Button\r\n");
			HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
			STATE_Display = StateDisplay_Menu1_WaitInput;
		}
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
