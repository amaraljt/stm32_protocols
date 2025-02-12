#ifndef UART_H
#define UART_H

#include "stm32f4xx.h"

void Delay_ms();
void Clock_Init();
void GPIO_Init();
void USART_Init();
void USART1_SendChar(uint8_t c);
void USART1_SendString(char *str);


#endif /* UART_H */
