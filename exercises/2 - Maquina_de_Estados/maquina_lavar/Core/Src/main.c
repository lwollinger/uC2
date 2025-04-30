/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


// Enumeração de estados: Aqui foi definido um tipo chamado Estados que representa os possíveis estados da máquina de estados.
typedef enum { ST_INICIO, ST_MOLHO, ST_LAVAR, ST_ENXAGUE, ST_CENTRIFUGA, ST_DESLIGA } Estados;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//#define GPIO_PIN_RESET 0
//#define GPIO_PIN_SET  1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Estados estado_atual = ST_INICIO;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

void estado_inicio();
void estado_molho();
void estado_lavar();
void estado_enxague();
void estado_centrifuga();
void estado_desliga();
void (*tabela_estados[])();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
 *  A função estado_inicio, vai definir o funcionamento do sistema. É nele que será definido a partir de botões, qual
 *  será a operação de lavagem que o sistema entrará.
 *
 *
 * OBS: SET -> N.L 1 vcc
 * 		RESET -> N.L 0 gnd
 *
 *   "LED -> set indica ligado"
 *   "Botão -> Reset Indica Botão APERTADO, ex: Tampa Fechada"
 *
 */

void tampa_aberta(){
	//HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOA,LED_PARAR_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

void tampa_fechada(){
	//HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOA,LED_PARAR_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
}



void estado_inicio(){

	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOA,LED_INICIO_Pin, GPIO_PIN_SET);	// Liga o LED indicando ponto inicial (esperando escolher modo)

	// Caso tampa aberta.
	if((HAL_GPIO_ReadPin(GPIOB, BOTAO_TAMPA_Pin))){	// Sempre retorna 1, quando for fechada tampa (botão acionado) if(0)
		HAL_Delay(1000);
		tampa_aberta();
	}
	else{
		tampa_fechada();
		HAL_Delay(1000);

		// Seleção dos modos
		if(!(HAL_GPIO_ReadPin(GPIOB, BOTAO_MOLHO_Pin))){ // Botão Apertado (GND), Indica Proximo estado
			HAL_GPIO_WritePin(GPIOA,LED_INICIO_Pin, GPIO_PIN_RESET); 	// Desliga led do ponto inicial
			estado_atual = ST_MOLHO;
		}
		if(!(HAL_GPIO_ReadPin(GPIOB, BOTAO_LAVAR_Pin))){	// Botão Apertado (GND), Indica Proximo estado
			HAL_GPIO_WritePin(GPIOA,LED_INICIO_Pin, GPIO_PIN_RESET);	// Desliga led do ponto inicial
			estado_atual = ST_LAVAR;
		}
		if(!(HAL_GPIO_ReadPin(GPIOB, BOTAO_ENXAGUE_Pin))){	// Botão Apertado (GND), Indica Proximo estado
			HAL_GPIO_WritePin(GPIOA,LED_INICIO_Pin, GPIO_PIN_RESET);	// Desliga led do ponto inicial
			estado_atual = ST_ENXAGUE;
		}
	}
}


void estado_molho(){

	HAL_GPIO_WritePin(GPIOA, LED_MOLHO_Pin, GPIO_PIN_SET); // liga o led indicando estado molho iniciando.
	HAL_Delay(1000);
	// Caso tampa fechada
	if(!(HAL_GPIO_ReadPin(GPIOB, BOTAO_TAMPA_Pin))){ // como pull-up, retorna 0
		tampa_fechada();

		HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOA, LED_MOLHO_Pin, GPIO_PIN_RESET);	// Desliga led indicando estado molho terminado.
		estado_atual = ST_LAVAR;
	}

	// Caso tampa aberta.
	if((HAL_GPIO_ReadPin(GPIOB, BOTAO_TAMPA_Pin))){ // Botão em VCC, Porta Aberta
		tampa_aberta(); // Acende led indicando Porta Aberta
	}

}

void estado_lavar(){
	HAL_GPIO_WritePin(GPIOA,LED_LAVAR_Pin, GPIO_PIN_SET); // liga o led indicando estado lavar iniciando.
	HAL_Delay(1000); // delay de 5 segundos
	HAL_GPIO_WritePin(GPIOA,LED_LAVAR_Pin, GPIO_PIN_RESET);	// Desliga o led indicando estado lavar finalizado.
	estado_atual = ST_ENXAGUE;
}

void estado_enxague(){
	HAL_GPIO_WritePin(GPIOA, LED_ENXAGUE_Pin, GPIO_PIN_SET);	// Liga LED indicando modo enxague.

	if(!(HAL_GPIO_ReadPin(GPIOB, BOTAO_TAMPA_Pin))){ // tampa fechada
		HAL_GPIO_WritePin(GPIOA,LED_PARAR_Pin, GPIO_PIN_RESET); // Desliga led indicando Porta fechada

		HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOA,LED_ENXAGUE_Pin, GPIO_PIN_RESET);	// Desliga o led indicando estado lavar finalizado.
		estado_atual = ST_CENTRIFUGA;
	}

	// Caso tampa aberta.
	if((HAL_GPIO_ReadPin(GPIOB, BOTAO_TAMPA_Pin))){
		tampa_aberta(); // Acende led indicando Porta Aberta
	}

}

void estado_centrifuga(){
	HAL_GPIO_WritePin(GPIOA, LED_CENTRIFUGA_Pin, GPIO_PIN_SET);

	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOA, LED_CENTRIFUGA_Pin, GPIO_PIN_RESET);
	estado_atual = ST_DESLIGA;
}

void estado_desliga(){
	HAL_GPIO_WritePin(GPIOA, LED_DESLIGA_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOA, LED_DESLIGA_Pin, GPIO_PIN_RESET);
	estado_atual = ST_INICIO;
}



// Vetor ponteiro para funções.
void (*tabela_estados[])() = { estado_inicio, estado_molho, estado_lavar, estado_enxague, estado_centrifuga, estado_desliga };

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
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
/*
	  if(!(HAL_GPIO_ReadPin(GPIOB, BOTAO_TAMPA_Pin))){
		  HAL_GPIO_WritePin(GPIOA,LED_PARAR_Pin, GPIO_PIN_SET);
		  HAL_Delay(100);
	  }else{
		  HAL_GPIO_WritePin(GPIOA,LED_PARAR_Pin, GPIO_PIN_RESET);
		  HAL_Delay(100);
	  }
*/
		tabela_estados[estado_atual]();



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

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LED_MOLHO_Pin|LED_LAVAR_Pin|LED_ENXAGUE_Pin|LED_CENTRIFUGA_Pin
			|LED_DESLIGA_Pin|LED_PARAR_Pin|LED_INICIO_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : LED_MOLHO_Pin LED_LAVAR_Pin LED_ENXAGUE_Pin LED_CENTRIFUGA_Pin
                           LED_DESLIGA_Pin LED_PARAR_Pin LED_INICIO_Pin */
	GPIO_InitStruct.Pin = LED_MOLHO_Pin|LED_LAVAR_Pin|LED_ENXAGUE_Pin|LED_CENTRIFUGA_Pin
			|LED_DESLIGA_Pin|LED_PARAR_Pin|LED_INICIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : BOTAO_MOLHO_Pin BOTAO_LAVAR_Pin BOTAO_ENXAGUE_Pin BOTAO_TAMPA_Pin */
	GPIO_InitStruct.Pin = BOTAO_MOLHO_Pin|BOTAO_LAVAR_Pin|BOTAO_ENXAGUE_Pin|BOTAO_TAMPA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


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
