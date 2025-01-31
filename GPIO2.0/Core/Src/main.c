// Main.c

#include "stm32f4xx.h"
#include "gpio.h"

int main(){

    GPIO_Init();

    while(1){
        for (volatile int i = 0; i < 1000000; i++) {}  // Delay
        // GPIOA->BSRR = (1 << 5); // Set Pin PA5 High
        GPIO_Set_Pin();

        for (volatile int i = 0; i < 1000000; i++) {}  // Delay
        // GPIOA->BSRR = (1 << 21); // Reset Pin PA5 Low
        GPIO_Reset_Pin();
    }
    return 0;
}