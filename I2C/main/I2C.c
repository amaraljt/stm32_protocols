#include "stdio.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "i2c-lcd.h"

static const char *TAG = "i2c-example";

char buffer[500];
float num = 12.34;


/* i2c master configuration */
void app_main(void){
  i2c_master_init();
  ESP_LOGI(TAG, "I2C initialized successfully");

  lcd_init();
  vTaskDelay(pdMS_TO_TICKS(100));  // Wait a bit after init
  
  lcd_put_cur(0, 0);
  lcd_send_string("Hello esp32!");

  lcd_put_cur(1, 0);
  lcd_send_string("LMAO");

  vTaskDelay(pdMS_TO_TICKS(100));

  lcd_clear();
 
  // print numeric values
  lcd_put_cur(0, 0);
  sprintf(buffer, "val=%f", num);
  lcd_send_string(buffer);
  lcd_put_cur(1, 0);
  lcd_send_string("LOL");
  
  ESP_LOGI(TAG, "Sent data to LCD");
}
