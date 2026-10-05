/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


typedef struct {
    float kp, ki, kd;
    float ErroAnterior;
    float Integral;
} PIDController;

typedef struct{

	bool Prep;
	bool Sinc;
	bool Run;

}SemaphoreState;

typedef enum {

    State_Ready,
	State_Sync,
	State_Go,

} StateMachin;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define PID_INTEGRAL_MAX 200.0f
#define LOOP_MS 2
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t Leituras[8];
uint8_t  SensorAtivo[8];

const int pesos[8] = {-100, -50, -35, -25, 25, 35, 50, 100};
uint16_t LimiarSensor = 2800;

float posicaoLinha = 0.0f;      // posição atual (-100 a +100)
float VELOCIDADE_BASE = 300.0f; // ajuste conforme o ARR do TIM1
float velocidadeEsq, velocidadeDir;

PIDController ControlPID;
SemaphoreState Memorys;
StateMachin Actions;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void atualizarEstado(SemaphoreState *semaforo, StateMachin estadoAtual);

void pid_init(PIDController *pid, float kp, float ki, float kd);

void Leitura();
float pid_calcula(PIDController *pid, float setpoint, float atual, float dt);
void AcionarMotores(float correcao);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

	if(GPIO_Pin == Btn1_Pin){

		Memorys.Prep=false;
		Memorys.Sinc=false;
		Memorys.Run =false;

		HAL_GPIO_WritePin(MotorA_GPIO_Port, MotorA_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MotorB_GPIO_Port, MotorB_Pin, GPIO_PIN_RESET);

		HAL_GPIO_TogglePin(Led1_GPIO_Port, Led1_Pin);
		HAL_Delay(15);
		Memorys.Prep=true;
		Memorys.Sinc=false;
		Memorys.Run =false;

	}

	if((GPIO_Pin == Btn3_Pin && Memorys.Prep == true) ){

		Memorys.Sinc=false;
		Memorys.Run =false;

		HAL_GPIO_WritePin(MotorA_GPIO_Port, MotorA_Pin, GPIO_PIN_SET);
		HAL_Delay(15);
		HAL_GPIO_WritePin(MotorA_GPIO_Port, MotorA_Pin, GPIO_PIN_RESET);
		HAL_Delay(15);
		HAL_GPIO_WritePin(MotorB_GPIO_Port, MotorB_Pin, GPIO_PIN_SET);
		HAL_Delay(15);
		HAL_GPIO_WritePin(MotorA_GPIO_Port, MotorA_Pin, GPIO_PIN_RESET);
		HAL_GPIO_TogglePin(Led1_GPIO_Port, Led1_Pin);
		HAL_Delay(15);

		Memorys.Sinc= true;
		Memorys.Run =false;

	}

	if((GPIO_Pin == Btn2_Pin && Memorys.Prep == true && Memorys.Sinc == true)){


		Memorys.Run =false;


		HAL_GPIO_TogglePin(Led1_GPIO_Port, Led1_Pin);
		HAL_Delay(15);


		Memorys.Run = true;

	}

}





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
  MX_TIM1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, Leituras, 8);

  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);



  pid_init(&ControlPID, 2.5f, 0.0f, 0.5f);
  uint32_t tempo_anterior = HAL_GetTick();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  uint32_t tempo_atual = HAL_GetTick();

	     if (tempo_atual - tempo_anterior >= LOOP_MS)
	     {
	         float dt = (tempo_atual - tempo_anterior) / 1000.0f;
	         tempo_anterior = tempo_atual;

	         Leitura();
	         float correcao = pid_calcula(&ControlPID, 0.0f, posicaoLinha, dt);
	         AcionarMotores(correcao);
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void atualizarEstado(SemaphoreState *semaforo, StateMachin estadoAtual) {
    switch (estadoAtual) {
        case State_Ready:
            semaforo->Prep = true;
            semaforo->Sinc = false;
            semaforo->Run  = false;
            break;

        case State_Sync:
            semaforo->Prep = false;
            semaforo->Sinc = true;
            semaforo->Run  = false;
            break;

        case State_Go:
            semaforo->Prep = false;
            semaforo->Sinc = false;
            semaforo->Run  = true;
            break;
    }
}

void pid_init(PIDController *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->ErroAnterior = 0.0f;
    pid->Integral = 0.0f;
}

void Leitura(void)
{
    int soma = 0;
    int ativos = 0;

    for (int i = 0; i < 8; i++)
    {
        SensorAtivo[i] = (Leituras[i] < LimiarSensor) ? 1 : 0;
        if (SensorAtivo[i])
        {
            soma += pesos[i];
            ativos++;
        }
    }

    // Média ponderada; se perdeu a linha, mantém a última posição
    if (ativos > 0)
        posicaoLinha = (float)soma / ativos;
}

float pid_calcula(PIDController *pid, float setpoint, float atual, float dt)
{
    if (dt <= 0.0f) dt = 0.001f;

    float erro = setpoint - atual;

    pid->Integral += erro * dt;
    if (pid->Integral >  PID_INTEGRAL_MAX) pid->Integral =  PID_INTEGRAL_MAX;
    if (pid->Integral < -PID_INTEGRAL_MAX) pid->Integral = -PID_INTEGRAL_MAX;

    float derivativo = (erro - pid->ErroAnterior) / dt;
    pid->ErroAnterior = erro;

    return (pid->kp * erro) + (pid->ki * pid->Integral) + (pid->kd * derivativo);
}

void AcionarMotores(float correcao)
{
    float max = (float)__HAL_TIM_GET_AUTORELOAD(&htim1);

    // erro negativo = linha à direita -> esquerda acelera, direita desacelera
    velocidadeEsq = VELOCIDADE_BASE - correcao;
    velocidadeDir = VELOCIDADE_BASE + correcao;

    if (velocidadeEsq < 0) velocidadeEsq = 0;
    if (velocidadeEsq > max) velocidadeEsq = max;
    if (velocidadeDir < 0) velocidadeDir = 0;
    if (velocidadeDir > max) velocidadeDir = max;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)velocidadeEsq);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)velocidadeDir);
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
#ifdef USE_FULL_ASSERT
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
