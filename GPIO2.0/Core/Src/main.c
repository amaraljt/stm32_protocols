// Main.c

#include "stm32f4xx.h"

int main(){

    RCC->AHB1ENR |= (1 << 0); // Enable GPIOA clock

    GPIOA->MODER |= (1 << 10); // Output Mode for Pin5 : (1 @ bit 2y)
    GPIOA->MODER &= ~(1 << 11); // Output Mode for Pin5 : (0 @ bit 2y+1)

    GPIOA->OTYPER &= ~(1 << 5); // Output Push-pull : (0 @ bit 5)

    GPIOA->OSPEEDR |= (1 << 11); // Fast Speed : (1 @ bit 2y+1)
    GPIOA->OSPEEDR &= ~(1 << 10); // Fast Speed : (0 @ bit 2y)

    GPIOA->PUPDR &= ~((1 << 10) | (1 << 11)); // No Pull-up/down resistor : (00 @ bits 2y:2y+1)

    while(1){

        for (volatile int i = 0; i < 1000000; i++) {}  // Delay
        GPIOA->BSRR = (1 << 5); // Set Pin PA5 High

        for (volatile int i = 0; i < 1000000; i++) {}  // Delay
        GPIOA->BSRR = (1 << 21); // Reset Pin PA5 Low
    }
    return 0;
}