/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/* Private variables ---------------------------------------------------------*/
// UART_HandleTypeDef huart2;  // No usaremos la UART en este código.

/* USER CODE BEGIN PV */
bool carrera_iniciada = false;
bool carrera_terminada = false;
int contador_jugador1 = 0;
int contador_jugador2 = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void iniciarCarrera(void);
void incrementarContadorJugador(int jugador);
void declararGanador(int jugador);
void resetearContadores(void);
void resetearGanadores(void);
void Error_Handler(void);  // Añadimos la declaración de Error_Handler

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  while (1)
  {
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_RESET && (!carrera_iniciada || carrera_terminada))
    {
      iniciarCarrera();
    }

    if (carrera_iniciada && !carrera_terminada)
    {
      if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET) // Botón Jugador 1
      {
        incrementarContadorJugador(1);
        HAL_Delay(500);
      }
      if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET) // Botón Jugador 2
      {
        incrementarContadorJugador(2);
        HAL_Delay(500);
      }
    }
  }
}

void iniciarCarrera(void)
{
  resetearContadores();
  resetearGanadores();

  // Secuencia de luces del semáforo
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Semáforo Rojo
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Semáforo Amarillo
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); // Semáforo Verde

  carrera_iniciada = true;
  carrera_terminada = false;
}

void incrementarContadorJugador(int jugador)
{
  if (jugador == 1 && contador_jugador1 < 8)
  {
    GPIO_TypeDef* puerto_led_jugador1[] = {GPIOA, GPIOA, GPIOA, GPIOB, GPIOC, GPIOA, GPIOA, GPIOB};
    uint16_t pin_led_jugador1[] = {GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_10};

    HAL_GPIO_WritePin(puerto_led_jugador1[contador_jugador1], pin_led_jugador1[contador_jugador1], GPIO_PIN_SET);
    contador_jugador1++;

    if (contador_jugador1 == 8)
    {
      declararGanador(1);
    }
  }
  else if (jugador == 2 && contador_jugador2 < 8)
  {
    GPIO_TypeDef* puerto_led_jugador2[] = {GPIOB, GPIOB, GPIOB, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB};
    uint16_t pin_led_jugador2[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_3, GPIO_PIN_10, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1};

    HAL_GPIO_WritePin(puerto_led_jugador2[contador_jugador2], pin_led_jugador2[contador_jugador2], GPIO_PIN_SET);
    contador_jugador2++;

    if (contador_jugador2 == 8)
    {
      declararGanador(2);
    }
  }
}

void declararGanador(int jugador)
{
  carrera_terminada = true;
  if (jugador == 1)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); // Ganador Jugador 1
  }
  else if (jugador == 2)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // Ganador Jugador 2
  }
}

void resetearContadores(void)
{
  contador_jugador1 = 0;
  contador_jugador2 = 0;

  GPIO_TypeDef* puerto_led_jugador1[] = {GPIOA, GPIOA, GPIOA, GPIOB, GPIOC, GPIOA, GPIOA, GPIOB};
  uint16_t pin_led_jugador1[] = {GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_10};

  for (int i = 0; i < 8; i++)
  {
    HAL_GPIO_WritePin(puerto_led_jugador1[i], pin_led_jugador1[i], GPIO_PIN_RESET);
  }

  GPIO_TypeDef* puerto_led_jugador2[] = {GPIOB, GPIOB, GPIOB, GPIOA, GPIOA, GPIOA, GPIOB, GPIOB};
  uint16_t pin_led_jugador2[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_3, GPIO_PIN_10, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1};

  for (int i = 0; i < 8; i++)
  {
    HAL_GPIO_WritePin(puerto_led_jugador2[i], pin_led_jugador2[i], GPIO_PIN_RESET);
  }
}

void resetearGanadores(void)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); // Ganador Jugador 1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // Ganador Jugador 2
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : Semáforo Rojo, Amarillo, Verde */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : Botón Inicio, Jugador 1, Jugador 2 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;  // Habilitar resistencia pull-up
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;  // Habilitar resistencia pull-up
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure LEDs Jugador 1 */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure LEDs Jugador 2 */
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_2 | GPIO_PIN_3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure LEDs Ganadores */
  GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Error Handler Function
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
    // Loop infinito si ocurre un error
  }
}
