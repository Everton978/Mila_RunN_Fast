/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Sensor_Read */
osThreadId_t Sensor_ReadHandle;
const osThreadAttr_t Sensor_Read_attributes = {
  .name = "Sensor_Read",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for CtrMotors */
osThreadId_t CtrMotorsHandle;
const osThreadAttr_t CtrMotors_attributes = {
  .name = "CtrMotors",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for SensorQueue */
osMessageQueueId_t SensorQueueHandle;
const osMessageQueueAttr_t SensorQueue_attributes = {
  .name = "SensorQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Fun_Sensor_Read(void *argument);
void Fun_ctrMotors(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of SensorQueue */
  SensorQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &SensorQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Sensor_Read */
  Sensor_ReadHandle = osThreadNew(Fun_Sensor_Read, NULL, &Sensor_Read_attributes);

  /* creation of CtrMotors */
  CtrMotorsHandle = osThreadNew(Fun_ctrMotors, NULL, &CtrMotors_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Fun_Sensor_Read */
/**
  * @brief  Function implementing the Sensor_Read thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Fun_Sensor_Read */
void Fun_Sensor_Read(void *argument)
{
  /* USER CODE BEGIN Fun_Sensor_Read */

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Fun_Sensor_Read */
}

/* USER CODE BEGIN Header_Fun_ctrMotors */
/**
* @brief Function implementing the CtrMotors thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Fun_ctrMotors */
void Fun_ctrMotors(void *argument)
{
  /* USER CODE BEGIN Fun_ctrMotors */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Fun_ctrMotors */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

