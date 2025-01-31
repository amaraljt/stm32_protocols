/* GPIO.h - Jared Amaral */


#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"

#define PIN5 5

void GPIO_Init(); /* Initialize GPIO at Pin 5 */

void GPIO_Set_Pin();
void GPIO_Reset_Pin();


#endif /* GPIO_H */