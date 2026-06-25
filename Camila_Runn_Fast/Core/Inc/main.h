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
#define Sensor01_Pin GPIO_PIN_0
#define Sensor01_GPIO_Port GPIOA
#define Sensor02_Pin GPIO_PIN_1
#define Sensor02_GPIO_Port GPIOA
#define Sensor03_Pin GPIO_PIN_2
#define Sensor03_GPIO_Port GPIOA
#define Sensor04_Pin GPIO_PIN_3
#define Sensor04_GPIO_Port GPIOA
#define Sensor05_Pin GPIO_PIN_4
#define Sensor05_GPIO_Port GPIOA
#define Sensor06_Pin GPIO_PIN_5
#define Sensor06_GPIO_Port GPIOA
#define Sensor07_Pin GPIO_PIN_6
#define Sensor07_GPIO_Port GPIOA
#define Sensor08_Pin GPIO_PIN_7
#define Sensor08_GPIO_Port GPIOA
#define SensorL_Pin GPIO_PIN_0
#define SensorL_GPIO_Port GPIOB
#define SensorR_Pin GPIO_PIN_1
#define SensorR_GPIO_Port GPIOB
#define Btn_Control_Pin GPIO_PIN_2
#define Btn_Control_GPIO_Port GPIOB
#define STBY_Pin GPIO_PIN_8
#define STBY_GPIO_Port GPIOA
#define BIN2_Pin GPIO_PIN_9
#define BIN2_GPIO_Port GPIOA
#define BIN1_Pin GPIO_PIN_10
#define BIN1_GPIO_Port GPIOA
#define AIN2_Pin GPIO_PIN_11
#define AIN2_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_12
#define AIN1_GPIO_Port GPIOA
#define PWM_MotorB_Pin GPIO_PIN_15
#define PWM_MotorB_GPIO_Port GPIOA
#define PWM_MotorA_Pin GPIO_PIN_3
#define PWM_MotorA_GPIO_Port GPIOB
#define Enc2_MotorB_Pin GPIO_PIN_4
#define Enc2_MotorB_GPIO_Port GPIOB
#define Enc1_MotorB_Pin GPIO_PIN_5
#define Enc1_MotorB_GPIO_Port GPIOB
#define Enc2_MotorA_Pin GPIO_PIN_6
#define Enc2_MotorA_GPIO_Port GPIOB
#define Enc1_MotorA_Pin GPIO_PIN_7
#define Enc1_MotorA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
