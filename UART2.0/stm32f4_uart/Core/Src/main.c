#include "main.h"
#include "uart.h"
#include <stdio.h>

int main(){
    Clock_Init();

    GPIO_Init();
    
    USART_Init();
    
    //USART1_SendString("to be lame in the eyes of somebody i would never want to be like is nothing but an absolute success");
    USART1_SendChar('A');
    Delay_ms(1000000);

    uint8_t data = USART1_ReceiveChar();

    while(1){
        if(data == 'A'){
            Delay_ms(1000000);
            GPIO_Set_Pin(); // blink LED
            Delay_ms(1000000);
            GPIO_Reset_Pin();
        }    
    }

    return 0;
}

