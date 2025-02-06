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
  printf("Main start\n");
  // Step 0. Initialize GPIOA & USART1 clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  // Step 1. Initalize I/O Pins as alternate mode : PA2 - USART1_TX & PA3 - USART1_RX
  GPIOA->MODER |= (2 << 18); // alternate function for PA9
  GPIOA->MODER |= (2 << 20); // alternate function for PA10

  GPIOA->OSPEEDR |= ((3 << 18) | (3 << 20)); // High Speed for PA2 & PA3

  // Step . Set alternate functions for PA2 & PA3 : 0111 <- USART
  GPIOA->AFR[1] |= (7 << 4); // PA9  -> AF7 (USART_TX)
  GPIOA->AFR[1] |= (7 << 8); // PA10 -> AF7 (USART_RX)

  // Step 3. Initialize USART
  USART1->CR1 = 0x00; // clear
  USART1->CR1 |= (1 << 13); // USART Enable
  USART1->CR1 |= ~(1 << 12); // Define 8 bit word length
  USART1->CR1 |= (1 << 3); // Transmitter Enable
  USART1->CR1 |= (1 << 2); // Receiver Enable

  USART1->CR1 &= ~(USART_CR1_PCE | USART_CR1_PS); // Disable parity
  USART1->CR2 &= ~(USART_CR2_STOP); // 1 stop bit

  USART1->BRR = (84000000 / 115200); // Baud Rate of 115200, PCLK2 @ 45MHz

  // Step 4. Send Data
  char data = 'G';
  while(1){
    while(!(USART1->SR & (1 << 7))); // Wait for Transmit Data Register to be empty
    USART1->DR = data;
    while(!(USART1->SR & (1 << 6))); // Wait for TC to SET
    for (volatile int i = 0; i < 10000000; i++) {}  // Delay
  }

  // Step 5. Receive Data
  // uint8_t tempData;
  // while(!(USART1->SR & (1 << 5))); // wait for RXNE to SET
  // tempData = USART1->DR; // read data
  // printf("Received: %u\n", tempData);



  return 0;
}