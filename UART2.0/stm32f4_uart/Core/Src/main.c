#include "main.h"
#include "stm32f4xx.h"


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
  // Step 0. Initialize GPIOA clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;


  // Step 1. Initalize I/O Pins as alternate mode : PA2 - USART2_TX & PA3 - USART2_RX

  GPIOA->MODER |= (2 << 4); // alternate function for PA2
  GPIOA->MODER |= (2 << 6); // alternate function for PA3


  // Step 3. Set alternate functions for PA2 & PA3
  GPIOA->AF[0] |= (7 << 8);
  GPIOA->AF[0] |= (7 << 12);



  



  return 0;
}