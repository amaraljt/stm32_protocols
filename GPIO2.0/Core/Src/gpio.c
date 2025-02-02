/* gpio.c - Jared Amaral */

#include "gpio.h"

void GPIO_Init(){    
    RCC->AHB1ENR |= (1 << 0); // Enable GPIOA clock

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