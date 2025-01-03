#include <stdio.h>
//#include "iot_button.h"
//#include "button_matrix.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// GPIO 15 - button
// GPIO 2 - led

#define LED    GPIO_NUM_2
#define BUTTON GPIO_NUM_15

/*void button_init(){
  // create gpio button
  button_config_t gpio_btn_cfg = {
    .type = BUTTON_TYPE_GPIO,
    .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
    .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
    .gpio_button_config = {
      .gpio_num = 15,
      .active_level = 0,
    },
  };
  button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
  if(NULL == gpio_btn) {
    ESP_LOGE(TAG, "Button create failed");
  }
}

void matrix_button_init(){
// create matrix keypad button
  button_config_t matrix_button_cfg = {
    .type = BUTTON_TYPE_MATRIX,
    .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
    .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
    .matrix_button_config = {
        .row_gpio_num = 0,
        .col_gpio_num = 1,
    }
  };
  button_handle_t matrix_button = iot_button_create(&matrix_button_cfg);
  if(NULL == matrix_button) {
    ESP_LOGE(TAG, "Button create failed");
  }
}*/

void app_main(void)
{
  //button_init();

  gpio_set_direction(LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

  while(1){
    if(gpio_get_level(BUTTON) == 1){
      gpio_set_level(LED, 1);
    } else{
      gpio_set_level(LED, 0);
    }
    vTaskDelay(1);
  }
}
