#include <stdio.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>

#define UART_NUM UART_NUM_2 // Use UART2 for GPIO16 and GPIO17
#define TXD_PIN GPIO_NUM_17  // TX2 pin
#define RXD_PIN GPIO_NUM_16  // RX2 pin
#define BUF_SIZE 1024

void app_main() {
  // Configure UART parameters
  uart_config_t uart_config = {
    .baud_rate = 115200, // Set baud rate
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  };
    
  QueueHandle_t uart_queue;
  // Install the driver and configure the pins
  ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 10, &uart_queue, 0));
  ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

  // Example: Sending data
  const char *test_str = "Hello stm32\r\n";

  // Example: Receiving data
  uint8_t data[BUF_SIZE];
  int len = 0;

  while (1) {
    // Example: Sending data
    uart_write_bytes(UART_NUM, test_str, strlen(test_str));

    vTaskDelay(1000);
    ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_NUM, (size_t*)&len));
    len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, 100);
    if (len > 0) {
      data[len] = '\0'; // Null-terminate the string
      printf("Received: '%s'", (char *)data);
    }
  }
}

