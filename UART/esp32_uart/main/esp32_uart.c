#include <stdio.h>
#include "driver/uart.h"
#include "esp_log.h"

#define UART_NUM UART_NUM_2 // Use UART2 for GPIO16 and GPIO17
#define TX_PIN GPIO_NUM_17  // TX2 pin
#define RX_PIN GPIO_NUM_16  // RX2 pin
#define BUF_SIZE 1024

static const char *TAG = "UART Example";

void app_main() {
    // Configure UART parameters
    uart_config_t uart_config = {
        .baud_rate = 115200, // Set baud rate
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Install the driver and configure the pins
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Example: Sending data
    const char *test_str = "Hello UART2!\n";
    uart_write_bytes(UART_NUM, test_str, strlen(test_str));

    // Example: Receiving data
    uint8_t data[BUF_SIZE];
    while (1) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, pdMS_TO_TICKS(1000));
        if (len > 0) {
            data[len] = '\0'; // Null-terminate the string
            ESP_LOGI(TAG, "Received: '%s'", (char *)data);
        }
    }
}

