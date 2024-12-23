#include "i2c-lcd.h"

#define SLAVE_ADDRESS_LCD 0x4E

esp_err_t err;

static const char *TAG = "LCD";

void lcd_send_cmd(char cmd){
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (cmd & 0xf0);
  data_l = ((cmd << 4) & 0xf0);
  data_t[0] = data_u | 0x0C; // en = 1, rs = 0
  data_t[1] = data_u | 0x08; // en = 0, rs = 0
  data_t[2] = data_l | 0x0C; // en = 1, rs = 0
  data_t[3] = data_l | 0x08; // en = 0, rs = 0
  
  err = i2c_master_write_to_device(I2C_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
  if(err != 0) ESP_LOGI(TAG, "Error sending command");
}

void lcd_send_data(char data){
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (data & 0xf0);
  data_l = ((data << 4) & 0xf0);
  data_t[0] = data_u | 0x0D; // en = 1, rs = 0
  data_t[1] = data_u | 0x09; // en = 0, rs = 0
  data_t[2] = data_l | 0x0D; // en = 1, rs = 0
  data_t[3] = data_l | 0x09; // en = 0, rs = 0
  
  err = i2c_master_write_to_device(I2C_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
  if(err != 0) ESP_LOGI(TAG, "Error in sending data");
}

void lcd_clear(void){
  lcd_send_cmd(0x80);
  for(int i = 0; i < 70; i++){
    lcd_send_data(' ');
  }
}

void lcd_put_cur(int row, int col){
  switch(row){
    case 0:
      col |= 0x80;
      break;
    case 1:
      col |= 0xC0;
      break;
  }
}

void lcd_init(void){
  
}

