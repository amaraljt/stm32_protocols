#include "main.h"
#include "uart.h"
#include <stdio.h>

int main(){
    Clock_Init();

    GPIO_Init();
    
    USART_Init();
    
    while(1){
        USART1_SendString("to be lame in the eyes of somebody i would never want to be like is nothing but an absolute success");
        Delay_ms(1000000);
    }
    

    return 0;
}

