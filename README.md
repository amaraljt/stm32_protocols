# stm32_protocols

This is a testing playground for practicing communication protocols using the STM32 NUCLEO-F446RE.
I will be creating micro projects for each protocol to further my understanding of embedded systems/software.

## UART
For the UART protocol I will be using an stm32 as the transmitter and an esp32 as the receiver.


## SPI


## I2C
Connected an lcd1602 with an I2C backpack to an esp32 for displaying messages to the screen. This will be further developed where a button will initiate data transfer to the lcd.

## CAN


## USB


## Wi-Fi
One esp32 will be setup in Access Point mode, acting as a hotspot waiting for connections and the other esp32 will be setup in station mode, looking to connect to the esp32 access point.

### UDP Server
A UDP server will be created to communicate between the client and server boards. I chose to use a UDP server instead of TCP for it's speed and me not needing reliable data transfers.
