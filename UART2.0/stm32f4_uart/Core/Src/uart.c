#include "uart.h"

/*
Delay_ms():
    Delay in milliseconds
*/
void Delay_ms(int delay){
    for (volatile int i = 0; i < delay; i++) {}
}

/* 
Clock_Init():
    Initializes GPIOA and USART1 Clock
*/
void Clock_Init(){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
}

/*
GPIO_Init():
    Initialize GPIOA port
*/
void GPIO_Init(){
    // Step 1. Initalize I/O Pins as alternate mode : PA9 - USART1_TX & PA10 - USART1_RX
    GPIOA->MODER |= (2 << 18); // alternate function for PA9
    GPIOA->MODER |= (2 << 20); // alternate function for PA10

    GPIOA->OSPEEDR |= ((3 << 18) | (3 << 20)); // High Speed for PA9 & PA10

    // Step 2. Set alternate functions for PA9 & PA10 : 0111 <- USART
    GPIOA->AFR[1] |= (7 << 4); // PA9  -> AF7 (USART_TX)
    GPIOA->AFR[1] |= (7 << 8); // PA10 -> AF7 (USART_RX)
}

/*
UART_Init():
    Initialize USART1
*/
void USART_Init(){
    // Step 3. Initialize USART
    USART1->CR1 |= (1 << 13); // USART Enable
    USART1->CR1 &= ~(1 << 12);  // 0: 1 Start bit, 8 Data bits, n Stop bit

    USART1->BRR = 782; // Baud Rate of 115200, PCLK2 @ 90MHz max

    USART1->CR1 |= (1 << 3);  // Transmitter Enable
}

void USART1_SendChar(uint8_t c){
    while(!(USART1->SR & (1 << 7))); // Wait for Transmit Data Register to be empty
    USART1->DR = c;
    while(!(USART1->SR & (1 << 6))); // Wait for TC to SET
}

void USART1_SendString(char *str){
    while(*str){
        USART1_SendChar(*str++);
    }
}
