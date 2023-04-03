/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Debug_Pin GPIO_PIN_13
#define LED_Debug_GPIO_Port GPIOC
#define LED_Connect_Pin GPIO_PIN_14
#define LED_Connect_GPIO_Port GPIOC
#define LED_Status_Pin GPIO_PIN_15
#define LED_Status_GPIO_Port GPIOC
#define A4_Pin GPIO_PIN_1
#define A4_GPIO_Port GPIOA
#define A3_Pin GPIO_PIN_2
#define A3_GPIO_Port GPIOA
#define A2_Pin GPIO_PIN_3
#define A2_GPIO_Port GPIOA
#define A1_Pin GPIO_PIN_4
#define A1_GPIO_Port GPIOA
#define LoRa_SCK_Pin GPIO_PIN_5
#define LoRa_SCK_GPIO_Port GPIOA
#define LoRa_MISO_Pin GPIO_PIN_6
#define LoRa_MISO_GPIO_Port GPIOA
#define LoRa_MOSI_Pin GPIO_PIN_7
#define LoRa_MOSI_GPIO_Port GPIOA
#define LoRa_NSS_Pin GPIO_PIN_0
#define LoRa_NSS_GPIO_Port GPIOB
#define LoRa_RST_Pin GPIO_PIN_1
#define LoRa_RST_GPIO_Port GPIOB
#define LoRa_DIO0_Pin GPIO_PIN_2
#define LoRa_DIO0_GPIO_Port GPIOB
#define LoRa_DIO0_EXTI_IRQn EXTI2_IRQn
#define Buzzer_Pin GPIO_PIN_10
#define Buzzer_GPIO_Port GPIOB
#define BT1_Pin GPIO_PIN_12
#define BT1_GPIO_Port GPIOB
#define BT2_Pin GPIO_PIN_13
#define BT2_GPIO_Port GPIOB
#define RS485_TX_Pin GPIO_PIN_9
#define RS485_TX_GPIO_Port GPIOA
#define RS485_RX_Pin GPIO_PIN_10
#define RS485_RX_GPIO_Port GPIOA
#define I2C_SCL_Pin GPIO_PIN_6
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
