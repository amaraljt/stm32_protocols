#ifndef UART_H
#define UART_H

#include "stm32f4xx.h"

#define GPIOA_PIN5 5

void Delay_ms();
void Clock_Init();
void GPIO_Init();
void GPIO_Set_Pin();
void GPIO_Reset_Pin();
void USART_Init();
void USART1_SendChar(uint8_t c);
void USART1_SendString(char *str);
uint8_t USART1_ReceiveChar();


#endif /* UART_H */
