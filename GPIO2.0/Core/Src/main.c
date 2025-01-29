#include "stm32f4xx.h"

void GPIO_Init(void) {
    // Enable clock for GPIOA (this is the port we are using for pin PA5)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  

    // Set PA5 as output (Mode = 01 for General purpose output mode)
    GPIOA->MODER |= GPIO_MODER_MODE5_0;  

    // Set the output type to push-pull (default)
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;  

    // Set the output speed (Low speed)
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;  

    // Disable pull-up/pull-down resistors (default)
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;  
}

void GPIO_SetHigh(void) {
    // Set PA5 high
    GPIOA->ODR |= GPIO_ODR_OD5;  
}

void GPIO_SetLow(void) {
    // Set PA5 low
    GPIOA->ODR &= ~GPIO_ODR_OD5;  
}

int main(void) {
    // Initialize GPIO
    GPIO_Init();

    // Turn on LED (Set PA5 high)
    GPIO_SetHigh();

    // Infinite loop
    while (1) {
        // Toggle LED every 500ms (optional)
        for (volatile int i = 0; i < 1000000; i++) {}  // Delay
        GPIO_SetLow();  // Turn off LED
        for (volatile int i = 0; i < 1000000; i++) {}  // Delay
        GPIO_SetHigh();  // Turn on LED
    }

    return 0;
}
