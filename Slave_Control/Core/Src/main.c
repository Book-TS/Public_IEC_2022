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
#include "iwdg.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Lora.h"
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
// Pin
#define Port_Contactor_1 	Relay_4_GPIO_Port
#define Pin_Concactor_1		Relay_4_Pin
#define Port_Contactor_2 	Relay_3_GPIO_Port
#define Pin_Concactor_2		Relay_3_Pin
#define Port_Buzzer				Relay_2_GPIO_Port
#define Pin_Buzzer				Relay_2_Pin
// Lora
LoRa mLoRa;
uint8_t lora_Stat = 0;
uint8_t loraData[128];
uint8_t loraSend[4];
// Time
unsigned long oldtime_1 = 0;
unsigned long oldtime_2 = 0;
unsigned long oldtime_3 = 0;

unsigned long timeDelay_1 = 1000;
unsigned long timeDelay_2 = 500;
unsigned long timeDelay_3 = 500;
// Status Variable
uint8_t loraRCV = 0;
uint8_t stat_Con1 = 0;			uint8_t status1 = 0;
uint8_t stat_Con2 = 0;			uint8_t status2 = 0;
uint8_t stat_Buzzer = 0;
uint8_t stat_Manual = 0;

// ADC
uint16_t data_ADC[4];
uint16_t temp_ADC[4];
uint16_t volt_ADC[4] = {0, 0, 0, 0};
// Temporary
uint8_t temp_1 = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void buzzerRun(void);
void rstContactor(void);
void blynkConnect(void);
void connectLora(void);
void initLora(void);
void readADC(void);
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
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
	HAL_IWDG_Init(&hiwdg);
	initLora();
	rstContactor();
	buzzerRun();
	
	stat_Manual = HAL_GPIO_ReadPin(IN_1_GPIO_Port, IN_1_Pin);
	if(stat_Manual)		rstContactor();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	
		if((unsigned long) (HAL_GetTick() - oldtime_1) > timeDelay_1) {
			HAL_IWDG_Refresh(&hiwdg);
			connectLora();
			HAL_GPIO_TogglePin(LED_Debug_GPIO_Port, LED_Debug_Pin);
			oldtime_1 = HAL_GetTick();
		}
		
		if ((unsigned long) (HAL_GetTick() - oldtime_2) > timeDelay_2) {
			if (stat_Manual == 0) {		// Mode Auto
				if (loraRCV) {					// Receive Lora
//					blynkConnect();
					buzzerRun();
					loraSend[0] = 'Q';
					loraSend[1] = '0';
					loraSend[2] = stat_Con1 + 0x30;
					loraSend[3] = stat_Con2 + 0x30;
					LoRa_transmit(&mLoRa, loraSend, sizeof(loraSend), 500);
					loraRCV = 0;
				}
				HAL_GPIO_WritePin(Port_Contactor_1, Pin_Concactor_1, !stat_Con1);
				HAL_GPIO_WritePin(Port_Contactor_2, Pin_Concactor_2, !stat_Con2);
				HAL_GPIO_WritePin(Port_Buzzer, Pin_Buzzer, !stat_Buzzer);
			}
			oldtime_2 = HAL_GetTick();
    }
		if((unsigned long) (HAL_GetTick() - oldtime_3) > timeDelay_3) {
			if (stat_Manual == 1) {		// Mode Manual
//				HAL_GPIO_WritePin(Port_Contactor_1, Pin_Concactor_1, status1);
//				HAL_GPIO_WritePin(Port_Contactor_2, Pin_Concactor_2, status2);
				if(temp_1) {
					loraSend[0] = 'Q';
					loraSend[1] = '0';
					loraSend[2] = status1 + 0x30;
					loraSend[3] = status2 + 0x30;
					LoRa_transmit(&mLoRa, loraSend, sizeof(loraSend), 500);
					temp_1 = 0;
				}
			}
			oldtime_3 = HAL_GetTick();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
// Interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == mLoRa.DIO0_pin) {
		LoRa_receive(&mLoRa, loraData, 128);
		
		if((loraData[0] == 'Q')&& (loraData[1] ==	'0')) {
			loraRCV = 1;
			if(loraData[2] == '1') 				stat_Con1 = 1;
			else if (loraData[2] == '0') 	stat_Con1 = 0;

			if(loraData[3] == '1') 				stat_Con2 = 1;
			else if (loraData[3] == '0') 	stat_Con2 = 0;

			if(loraData[4] == '1') 				stat_Buzzer = 1;
			else if (loraData[4] == '0') 	stat_Buzzer = 0;
		}	
	}
	
	if (GPIO_Pin == IN_1_Pin) {
		stat_Manual = HAL_GPIO_ReadPin(IN_1_GPIO_Port, IN_1_Pin);
		if(stat_Manual)		rstContactor();
	}
	if (GPIO_Pin == IN_2_Pin) {
		status1 = !HAL_GPIO_ReadPin(IN_2_GPIO_Port, IN_2_Pin);
		temp_1  = 1;
	}
	if (GPIO_Pin == IN_3_Pin) {
		status2 = !HAL_GPIO_ReadPin(IN_3_GPIO_Port, IN_3_Pin);
		temp_1  = 1;
	}
	
	if(GPIO_Pin == BT_1_Pin) {
		temp_1 = 1;
		status1 = !status1;
	}
	if(GPIO_Pin == BT_2_Pin) {
		temp_1 = 1;
		status2 = !status2;
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

// Reset status
void rstContactor(void) {
	HAL_GPIO_WritePin(Port_Contactor_1, Pin_Concactor_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Port_Contactor_2, Pin_Concactor_2, GPIO_PIN_SET);
}
// Read ADC
void readADC(void) {
	for(int i = 0; i < 50; i++) {
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)data_ADC, 4);
		HAL_Delay(1);
		HAL_ADC_Stop_DMA(&hadc1);
		for(int j = 0; i < 4; j++) {
			temp_ADC[j] = (data_ADC[j]*3300)/4095;
			if(volt_ADC[j] < temp_ADC[j]) volt_ADC[j] = temp_ADC[j];
		}
	}
}

//Blynk
void blynkConnect(void) {
	for(int i = 0; i < 5; i++) {
		HAL_GPIO_WritePin(LED_Status_GPIO_Port, LED_Status_Pin, GPIO_PIN_SET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(LED_Status_GPIO_Port, LED_Status_Pin, GPIO_PIN_RESET);
		HAL_Delay(50);
	}
}

// Connect Lora
void connectLora(void) {
	if(LoRa_init(&mLoRa) == LORA_OK) {
		lora_Stat = 1;
		HAL_GPIO_WritePin(LED_Connect_GPIO_Port, LED_Connect_Pin, GPIO_PIN_SET);
	}
	else HAL_GPIO_WritePin(LED_Connect_GPIO_Port, LED_Connect_Pin, GPIO_PIN_RESET);
	
	LoRa_startReceiving(&mLoRa);
}
// Init Lora
void initLora(void) {
	mLoRa = newLoRa();
	
	mLoRa.CS_port         = NSS_GPIO_Port;
	mLoRa.CS_pin          = NSS_Pin;
	mLoRa.reset_port      = RST_GPIO_Port;
	mLoRa.reset_pin       = RST_Pin;
	mLoRa.DIO0_port       = DIO_0_GPIO_Port;
	mLoRa.DIO0_pin        = DIO_0_Pin;
	mLoRa.hSPIx           = &hspi1;
	
	mLoRa.frequency             = 433;             // default = 433 MHz
	mLoRa.spredingFactor        = SF_10;           // default = SF_7
	mLoRa.bandWidth             = BW_125KHz;       // default = BW_125KHz
	mLoRa.crcRate               = CR_4_5;          // default = CR_4_5
	mLoRa.power                 = POWER_17db;      // default = 20db
	mLoRa.overCurrentProtection = 100;             // default = 100 mA
	mLoRa.preamble              = 8;               // default = 8;
	
	connectLora();
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
