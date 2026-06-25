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
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
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
/* Definitions for Read_sensors */
osThreadId_t Read_sensorsHandle;
uint32_t Read_sensorsBuffer[ 128 ];
osStaticThreadDef_t Read_sensorsControlBlock;
const osThreadAttr_t Read_sensors_attributes = {
  .name = "Read_sensors",
  .cb_mem = &Read_sensorsControlBlock,
  .cb_size = sizeof(Read_sensorsControlBlock),
  .stack_mem = &Read_sensorsBuffer[0],
  .stack_size = sizeof(Read_sensorsBuffer),
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for SensorSample */
osTimerId_t SensorSampleHandle;
osStaticTimerDef_t SensorSampleControlBlock;
const osTimerAttr_t SensorSample_attributes = {
  .name = "SensorSample",
  .cb_mem = &SensorSampleControlBlock,
  .cb_size = sizeof(SensorSampleControlBlock),
};
/* Definitions for PID_Control */
osTimerId_t PID_ControlHandle;
osStaticTimerDef_t PID_ControlControlBlock;
const osTimerAttr_t PID_Control_attributes = {
  .name = "PID_Control",
  .cb_mem = &PID_ControlControlBlock,
  .cb_size = sizeof(PID_ControlControlBlock),
};
/* Definitions for myEvent01 */
osEventFlagsId_t myEvent01Handle;
const osEventFlagsAttr_t myEvent01_attributes = {
  .name = "myEvent01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void fun_read_sensors(void *argument);
void SensorSample_Callback(void *argument);
void PIDControl_Callback(void *argument);

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

  /* Create the timer(s) */
  /* creation of SensorSample */
  SensorSampleHandle = osTimerNew(SensorSample_Callback, osTimerPeriodic, NULL, &SensorSample_attributes);

  /* creation of PID_Control */
  PID_ControlHandle = osTimerNew(PIDControl_Callback, osTimerPeriodic, NULL, &PID_Control_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Read_sensors */
  Read_sensorsHandle = osThreadNew(fun_read_sensors, NULL, &Read_sensors_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of myEvent01 */
  myEvent01Handle = osEventFlagsNew(&myEvent01_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_fun_read_sensors */
/**
  * @brief  Function implementing the Read_sensors thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_fun_read_sensors */
__weak void fun_read_sensors(void *argument)
{
  /* USER CODE BEGIN fun_read_sensors */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END fun_read_sensors */
}

/* SensorSample_Callback function */
void SensorSample_Callback(void *argument)
{
  /* USER CODE BEGIN SensorSample_Callback */

  /* USER CODE END SensorSample_Callback */
}

/* PIDControl_Callback function */
void PIDControl_Callback(void *argument)
{
  /* USER CODE BEGIN PIDControl_Callback */

  /* USER CODE END PIDControl_Callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

