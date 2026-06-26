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
/* Definitions for T_PID_Control */
osThreadId_t T_PID_ControlHandle;
const osThreadAttr_t T_PID_Control_attributes = {
  .name = "T_PID_Control",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for T_StateManager */
osThreadId_t T_StateManagerHandle;
const osThreadAttr_t T_StateManager_attributes = {
  .name = "T_StateManager",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for T_OdomSD */
osThreadId_t T_OdomSDHandle;
const osThreadAttr_t T_OdomSD_attributes = {
  .name = "T_OdomSD",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Sm_DMA_Clpt */
osSemaphoreId_t Sm_DMA_ClptHandle;
const osSemaphoreAttr_t Sm_DMA_Clpt_attributes = {
  .name = "Sm_DMA_Clpt"
};
/* Definitions for State_Flag */
osEventFlagsId_t State_FlagHandle;
const osEventFlagsAttr_t State_Flag_attributes = {
  .name = "State_Flag"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void F_PID_Control(void *argument);
void F_OdomSD(void *argument);

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

  /* Create the semaphores(s) */
  /* creation of Sm_DMA_Clpt */
  Sm_DMA_ClptHandle = osSemaphoreNew(1, 0, &Sm_DMA_Clpt_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of T_PID_Control */
  T_PID_ControlHandle = osThreadNew(F_PID_Control, NULL, &T_PID_Control_attributes);

  /* creation of T_StateManager */
  T_StateManagerHandle = osThreadNew(F_OdomSD, NULL, &T_StateManager_attributes);

  /* creation of T_OdomSD */
  T_OdomSDHandle = osThreadNew(F_OdomSD, NULL, &T_OdomSD_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of State_Flag */
  State_FlagHandle = osEventFlagsNew(&State_Flag_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_F_PID_Control */
/**
  * @brief  Function implementing the T_PID_Control thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_F_PID_Control */
void F_PID_Control(void *argument)
{
  /* USER CODE BEGIN F_PID_Control */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END F_PID_Control */
}

/* USER CODE BEGIN Header_F_OdomSD */
/**
* @brief Function implementing the T_StateManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_F_OdomSD */
__weak void F_OdomSD(void *argument)
{
  /* USER CODE BEGIN F_OdomSD */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END F_OdomSD */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

