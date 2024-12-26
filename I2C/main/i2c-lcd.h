#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void lcd_init(void); // initialize lcd_init

void lcd_send_cmd(char cmd); // send command to lcd

void lcd_send_data(char data); // send data to the lcd
                            
void lcd_send_string(char *str); // send string to the lcd
                                 
void lcd_put_cur(int row, int col) ; // put cursor at desired position

void lcd_clear(void); // clear lcd screen

#endif // !I2C_LCD_H_
