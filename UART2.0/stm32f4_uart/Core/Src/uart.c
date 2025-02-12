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
    Initialize GPIOA port and PINS 9, 10 & 5
*/
void GPIO_Init(){
    // Step 1. Initalize I/O Pins as alternate mode : PA9 - USART1_TX & PA10 - USART1_RX
    GPIOA->MODER |= (2 << 18); // alternate function for PA9
    GPIOA->MODER |= (2 << 20); // alternate function for PA10

    GPIOA->OSPEEDR |= ((3 << 18) | (3 << 20)); // High Speed for PA9 & PA10

    // Step 2. Set alternate functions for PA9 & PA10 : 0111 <- USART
    GPIOA->AFR[1] |= (7 << 4); // PA9  -> AF7 (USART_TX)
    GPIOA->AFR[1] |= (7 << 8); // PA10 -> AF7 (USART_RX)

    // PIN 5 SETUP
    GPIOA->MODER |= (1 << (GPIOA_PIN5 * 2)); // Output Mode for Pin5 : (1 @ bit 2y)
    GPIOA->MODER &= ~(1 << (2 * GPIOA_PIN5 + 1)); // Output Mode for Pin5 : (0 @ bit 2y+1)

    GPIOA->OTYPER &= ~(1 << GPIOA_PIN5); // Output Push-pull : (0 @ bit 5)

    GPIOA->OSPEEDR |= (1 << (2 * GPIOA_PIN5 + 1)); // Fast Speed : (1 @ bit 2y+1)
    GPIOA->OSPEEDR &= ~(1 << (2 * GPIOA_PIN5)); // Fast Speed : (0 @ bit 2y)

    GPIOA->PUPDR &= ~((1 << (2 * GPIOA_PIN5)) | (1 << (2 * GPIOA_PIN5 + 1))); // No Pull-up/down resistor : (00 @ bits 2y:2y+1)
}

void GPIO_Set_Pin(){
    GPIOA->BSRR = (1 << GPIOA_PIN5); // Set Pin PA5 High
}

void GPIO_Reset_Pin(){
    GPIOA->BSRR = (1 << 21); // Reset Pin PA5 Low
}

/*
UART_Init():
    Initialize USART1
*/
void USART_Init(){
    // Step 3. Initialize USART
    USART1->CR1 |= (1 << 13);  // USART Enable
    USART1->CR1 &= ~(1 << 12); // 0: 1 Start bit, 8 Data bits, n Stop bit
    USART1->CR2 &= ~(3 << 12); // 1 stop bit
    USART1->BRR = 782;         // Baud Rate of 115200, PCLK2 @ 90MHz max

    USART1->CR1 |= (1 << 3); // Transmitter Enabled
    USART1->CR1 |= (1 << 2); // Receiver Enabled

}

/*
USART1_SendChar():
    Transmits character over UART
*/
void USART1_SendChar(uint8_t c){
    while(!(USART1->SR & (1 << 7))); // Wait for Transmit Data Register to be empty
    USART1->DR = c;
    while(!(USART1->SR & (1 << 6))); // Wait for TC to SET
}

/*
USART1_SendString():
    Transmits string over UART
*/
void USART1_SendString(char *str){
    while(*str){
        USART1_SendChar(*str++);
    }
}

/*
USART1_ReceiveChar():
    Receives character over UART
*/
uint8_t USART1_ReceiveChar(){
    while(!(USART1->SR & (1 << 5))); // wait for received data
    uint8_t data = USART1->DR; // read data
    return data;
}
