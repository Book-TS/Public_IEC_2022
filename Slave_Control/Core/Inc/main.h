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
#define Ampe_1_Pin GPIO_PIN_1
#define Ampe_1_GPIO_Port GPIOA
#define Ampe_2_Pin GPIO_PIN_2
#define Ampe_2_GPIO_Port GPIOA
#define Ampe_3_Pin GPIO_PIN_3
#define Ampe_3_GPIO_Port GPIOA
#define Ampe_4_Pin GPIO_PIN_4
#define Ampe_4_GPIO_Port GPIOA
#define NSS_Pin GPIO_PIN_0
#define NSS_GPIO_Port GPIOB
#define DIO_0_Pin GPIO_PIN_1
#define DIO_0_GPIO_Port GPIOB
#define DIO_0_EXTI_IRQn EXTI1_IRQn
#define RST_Pin GPIO_PIN_2
#define RST_GPIO_Port GPIOB
#define Buzzer_Pin GPIO_PIN_10
#define Buzzer_GPIO_Port GPIOB
#define Relay_1_Pin GPIO_PIN_12
#define Relay_1_GPIO_Port GPIOB
#define Relay_2_Pin GPIO_PIN_13
#define Relay_2_GPIO_Port GPIOB
#define Relay_3_Pin GPIO_PIN_14
#define Relay_3_GPIO_Port GPIOB
#define Relay_4_Pin GPIO_PIN_15
#define Relay_4_GPIO_Port GPIOB
#define IN_1_Pin GPIO_PIN_8
#define IN_1_GPIO_Port GPIOA
#define IN_1_EXTI_IRQn EXTI9_5_IRQn
#define IN_2_Pin GPIO_PIN_9
#define IN_2_GPIO_Port GPIOA
#define IN_2_EXTI_IRQn EXTI9_5_IRQn
#define IN_3_Pin GPIO_PIN_10
#define IN_3_GPIO_Port GPIOA
#define IN_3_EXTI_IRQn EXTI15_10_IRQn
#define IN_4_Pin GPIO_PIN_11
#define IN_4_GPIO_Port GPIOA
#define IN_4_EXTI_IRQn EXTI15_10_IRQn
#define BT_2_Pin GPIO_PIN_12
#define BT_2_GPIO_Port GPIOA
#define BT_2_EXTI_IRQn EXTI15_10_IRQn
#define BT_1_Pin GPIO_PIN_15
#define BT_1_GPIO_Port GPIOA
#define BT_1_EXTI_IRQn EXTI15_10_IRQn
#define LED_Connect_Pin GPIO_PIN_3
#define LED_Connect_GPIO_Port GPIOB
#define LED_Status_Pin GPIO_PIN_4
#define LED_Status_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
