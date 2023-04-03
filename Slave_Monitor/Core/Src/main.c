/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "Lora.h"
#include "String.h"
#include "CLCD_I2C.h"
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
//
#define address 0x42
#define func		0x30
// LCD
CLCD_I2C_Name mLCD;

// Lora
LoRa mLoRa;
uint8_t LoRa_status = 0;
uint8_t LoraData[128];
uint8_t loraSend[12];
// Time
unsigned long oldtime_1 = 0;
unsigned long oldtime_2 = 0;
unsigned long oldtime_3 = 0;
unsigned long timeDelay_1 = 1000;
unsigned long timeDelay_2 = 500;
unsigned long timeDelay_3 = 50;

// Event
uint8_t loraRCV = 0;
uint8_t rs485RCV = 0;

//RS485
uint8_t dataRS485[7];

// Sensor Value
uint8_t temp_Val[3];
uint8_t WL_Val[3];
uint8_t pH_Val[3];
uint8_t DO_Val[3];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void buzzerRun(void);
void blynkConnect(void);
void loraConnect(void);
void loraInit(void);
void monitorInit(void);
void getTemp(void);
void getWL(void);
void getpH(void);
void getDO(void);
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	monitorInit();
	CLCD_I2C_Clear(&mLCD);
	CLCD_I2C_SetCursor(&mLCD, 1, 0);
	CLCD_I2C_WriteString(&mLCD, "SLAVE MONITOR");
	CLCD_I2C_SetCursor(&mLCD, 3, 1);
	CLCD_I2C_WriteString(&mLCD, "MDF GROUP");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if ((unsigned long) (HAL_GetTick() - oldtime_1) > timeDelay_1) {
			loraConnect();
			HAL_GPIO_TogglePin(LED_Debug_GPIO_Port, LED_Debug_Pin);
			oldtime_1 = HAL_GetTick();
		}
		if ((unsigned long) (HAL_GetTick() - oldtime_2) > timeDelay_2) {
			if (loraRCV) {
				blynkConnect();
				CLCD_I2C_Clear(&mLCD);
				CLCD_I2C_SetCursor(&mLCD, 1, 0);
				CLCD_I2C_WriteString(&mLCD, "SLAVE MONITOR");
				CLCD_I2C_SetCursor(&mLCD, 0, 1);
				CLCD_I2C_WriteString(&mLCD, "Read Sensor...");
				
				uint8_t data_3[] = "B0\n";
				HAL_UART_Transmit(&huart1, data_3, sizeof(data_3), 10);
				HAL_Delay(50);
				loraRCV = 0;
			}
			if (rs485RCV) {
				CLCD_I2C_Clear(&mLCD);
				CLCD_I2C_SetCursor(&mLCD, 1, 0);
				CLCD_I2C_WriteString(&mLCD, "SLAVE MONITOR");
				CLCD_I2C_SetCursor(&mLCD, 0, 1);
				CLCD_I2C_WriteString(&mLCD, "Send Data...");
				getWL();
				getpH();
				loraSend[0] = address;
				loraSend[1] = func;
				loraSend[2] = pH_Val[0];
				loraSend[3] = pH_Val[1];
				loraSend[4] = pH_Val[2];
				loraSend[5] = temp_Val[0];
				loraSend[6] = temp_Val[1];
				loraSend[7] = temp_Val[2];
				loraSend[8] = WL_Val[0];
				loraSend[9] = WL_Val[1];
				loraSend[10] = WL_Val[2];
				loraSend[11] = '1';
				LoRa_transmit(&mLoRa, loraSend, sizeof(loraSend), 500);
				LoRa_transmit(&mLoRa, loraSend, sizeof(loraSend), 500);
				rs485RCV = 0;
				HAL_Delay(1000);
				CLCD_I2C_Clear(&mLCD);
				CLCD_I2C_SetCursor(&mLCD, 1, 0);
				CLCD_I2C_WriteString(&mLCD, "SLAVE MONITOR");
				CLCD_I2C_SetCursor(&mLCD, 3, 1);
				CLCD_I2C_WriteString(&mLCD, "MDF GROUP");
			}
			oldtime_2 = HAL_GetTick();
		}
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
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
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

/* USER CODE BEGIN 4 */
// Get Temp
void getTemp(void) {
	int valTemp = 295+rand()%5;
	temp_Val[0] = (int)valTemp/100 + 48;
	temp_Val[1] = (int)((valTemp/10)%10) + 48;
	temp_Val[2] = (int)valTemp%10 + 48;
}
// Get WL
void getWL(void) {
	int valWL = 170+rand()%5;
	WL_Val[0] = (int)valWL/100 + 48;
	WL_Val[1] = (int)((valWL/10)%10) + 48;
	WL_Val[2] = (int)valWL%10 + 48;
}
// Get PH
void getpH(void) {
	int valpH = 77+rand()%3;
	pH_Val[0] = 48;
	pH_Val[1] = (int)valpH/10 + 48;
	pH_Val[2] = (int)valpH%10 + 48;
}

void getDO(void) {
	int doVal = 55+rand()%3;
	DO_Val[0] = 48;
	DO_Val[1] = (int)doVal/10 + 48;
	DO_Val[2] = (int)doVal%10 + 48;
}

// Monitor Init
void monitorInit(void) {
	temp_Val[0] = '2';
	temp_Val[1] = '7';
	temp_Val[2] = '5';
	
	WL_Val[0] = '1';
	WL_Val[1] = '7';
	WL_Val[2] = '2';
	
	pH_Val[0] = '0';
	pH_Val[1] = '7';
	pH_Val[2] = '8';
	
	DO_Val[0] = '0';
	DO_Val[1] = '5';
	DO_Val[2] = '7';
	
	CLCD_I2C_Init(&mLCD,&hi2c1, 0x4E, 16,2);
	
	HAL_UART_Receive_DMA(&huart1, dataRS485, 7);
	
	loraInit();
	loraConnect();
	HAL_Delay(200);
	
	CLCD_I2C_Clear(&mLCD);
	CLCD_I2C_SetCursor(&mLCD, 0, 0);
	CLCD_I2C_WriteString(&mLCD, "Init Done!");
	
	buzzerRun();
}
// Interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == mLoRa.DIO0_pin) {
		LoRa_receive(&mLoRa, LoraData, 128);
		if((LoraData[0] == 'B') && (LoraData[1] == '0'))  loraRCV = 1;			// Kiem tra nhan du lieu
	}
}
// RS485 Receive
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if(huart->Instance == USART1) {
		if((dataRS485[0] == 'A') && (dataRS485[1]=='0')) {
			pH_Val[0] = dataRS485[3];
			pH_Val[1] = dataRS485[4];
			pH_Val[2] = dataRS485[5];
			rs485RCV = 1;
		}
		else if ((dataRS485[0] == 'B') && (dataRS485[1]=='0')) {
			temp_Val[0] = dataRS485[3];
			temp_Val[1] = dataRS485[4];
			temp_Val[2] = dataRS485[5];
			rs485RCV = 1;
		}
	}
}
// Buzzer run
void buzzerRun(void) {
	for(int i = 0; i < 3; i++) {
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
		HAL_Delay(100);
	}
}
//Blynk
void blynkConnect(void) {
	for(int i = 0; i < 5; i++) {
		HAL_GPIO_WritePin(LED_Status_GPIO_Port, LED_Status_Pin, GPIO_PIN_RESET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(LED_Status_GPIO_Port, LED_Status_Pin, GPIO_PIN_SET);
		HAL_Delay(50);
	}
}
// ConnectLoRa
void loraConnect(void) {
	if(LoRa_init(&mLoRa) == LORA_OK) {
		LoRa_status = 1;
		HAL_GPIO_WritePin(LED_Connect_GPIO_Port, LED_Connect_Pin, GPIO_PIN_RESET);
	}
	else HAL_GPIO_WritePin(LED_Connect_GPIO_Port, LED_Connect_Pin, GPIO_PIN_SET);
	
	LoRa_startReceiving(&mLoRa);
}
// Lora Init
void loraInit(void) {
	mLoRa = newLoRa();
	
	mLoRa.CS_port         = LoRa_NSS_GPIO_Port;
	mLoRa.CS_pin          = LoRa_NSS_Pin;
	mLoRa.reset_port      = LoRa_RST_GPIO_Port;
	mLoRa.reset_pin       = LoRa_RST_Pin;
	mLoRa.DIO0_port       = LoRa_DIO0_GPIO_Port;
	mLoRa.DIO0_pin        = LoRa_DIO0_Pin;
	mLoRa.hSPIx           = &hspi1;
	
	mLoRa.frequency             = 433;             // default = 433 MHz
	mLoRa.spredingFactor        = SF_10;           // default = SF_7
	mLoRa.bandWidth             = BW_125KHz;       // default = BW_125KHz
	mLoRa.crcRate               = CR_4_5;          // default = CR_4_5
	mLoRa.power                 = POWER_17db;      // default = 20db
	mLoRa.overCurrentProtection = 100;             // default = 100 mA
	mLoRa.preamble              = 8;               // default = 8;
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
