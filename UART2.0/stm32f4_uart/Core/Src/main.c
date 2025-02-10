#include "main.h"
#include "stm32f4xx.h"
#include <stdio.h>


// typedef struct
// {
//   __IO uint16_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
//   uint16_t      RESERVED0;  /*!< Reserved, 0x02                                                */
//   __IO uint16_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
//   uint16_t      RESERVED1;  /*!< Reserved, 0x06                                                */
//   __IO uint16_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
//   uint16_t      RESERVED2;  /*!< Reserved, 0x0A                                                */
//   __IO uint16_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
//   uint16_t      RESERVED3;  /*!< Reserved, 0x0E                                                */
//   __IO uint16_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
//   uint16_t      RESERVED4;  /*!< Reserved, 0x12                                                */
//   __IO uint16_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
//   uint16_t      RESERVED5;  /*!< Reserved, 0x16                                                */
//   __IO uint16_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
//   uint16_t      RESERVED6;  /*!< Reserved, 0x1A                                                */
// } USART_TypeDef;

// typedef struct
// {
//   __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
//   __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
//   __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
//   __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
//   __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
//   __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
//   __IO uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
//   __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
//   __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
// } GPIO_TypeDef;


int main(){
  // Step 0. Initialize GPIOA & USART1 clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  // Step 1. Initalize I/O Pins as alternate mode : PA9 - USART1_TX & PA10 - USART1_RX
  GPIOA->MODER |= (2 << 18); // alternate function for PA9
  GPIOA->MODER |= (2 << 20); // alternate function for PA10

  GPIOA->OSPEEDR |= ((3 << 18) | (3 << 20)); // High Speed for PA9 & PA10

  // Step 2. Set alternate functions for PA9 & PA10 : 0111 <- USART
  GPIOA->AFR[1] |= (7 << 4); // PA9  -> AF7 (USART_TX)
  GPIOA->AFR[1] |= (7 << 8); // PA10 -> AF7 (USART_RX)

  // Step 3. Initialize USART
  USART1->CR1 |= (1 << 13); // USART Enable
  USART1->CR1 &= ~(1 << 12);  // 0: 1 Start bit, 8 Data bits, n Stop bit

  USART1->BRR = 782; // Baud Rate of 115200, PCLK2 @ 90MHz max

  USART1->CR1 |= (1 << 3);  // Transmitter Enable


  // Step 4. Send Data
  uint8_t data = 'G';
  while(1){
    while(!(USART1->SR & (1 << 7))); // Wait for Transmit Data Register to be empty
    USART1->DR = data;
    while(!(USART1->SR & (1 << 6))); // Wait for TC to SET
    for (volatile int i = 0; i < 1000000; i++) {}  // Delay
  }

  return 0;
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 90;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
