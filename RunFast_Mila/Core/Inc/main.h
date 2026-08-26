/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define Led_Pin GPIO_PIN_13
#define Led_GPIO_Port GPIOC
#define Sensor0_Pin GPIO_PIN_0
#define Sensor0_GPIO_Port GPIOA
#define Sensor1_Pin GPIO_PIN_1
#define Sensor1_GPIO_Port GPIOA
#define Sensor2_Pin GPIO_PIN_2
#define Sensor2_GPIO_Port GPIOA
#define Sensor3_Pin GPIO_PIN_3
#define Sensor3_GPIO_Port GPIOA
#define Sensor4_Pin GPIO_PIN_4
#define Sensor4_GPIO_Port GPIOA
#define Sensor5_Pin GPIO_PIN_5
#define Sensor5_GPIO_Port GPIOA
#define Sensor6_Pin GPIO_PIN_6
#define Sensor6_GPIO_Port GPIOA
#define Sensor7_Pin GPIO_PIN_7
#define Sensor7_GPIO_Port GPIOA
#define Sle_Pin GPIO_PIN_0
#define Sle_GPIO_Port GPIOB
#define Sld_Pin GPIO_PIN_1
#define Sld_GPIO_Port GPIOB
#define Btn1_Pin GPIO_PIN_2
#define Btn1_GPIO_Port GPIOB
#define Ain1_Pin GPIO_PIN_10
#define Ain1_GPIO_Port GPIOB
#define Ain2_Pin GPIO_PIN_11
#define Ain2_GPIO_Port GPIOB
#define Bin1_Pin GPIO_PIN_12
#define Bin1_GPIO_Port GPIOB
#define Bin2_Pin GPIO_PIN_13
#define Bin2_GPIO_Port GPIOB
#define Stby_Pin GPIO_PIN_14
#define Stby_GPIO_Port GPIOB
#define Motor2_Pin GPIO_PIN_15
#define Motor2_GPIO_Port GPIOA
#define Motor1_Pin GPIO_PIN_3
#define Motor1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
