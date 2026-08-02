## System Architecture

The system consists of two microcontrollers communicating through UART:

- Arduino UNO:
  - Reads distance from HC-SR04 ultrasonic sensor
  - Generates measurement frame
  - Sends data through UART

- Arduino Mega 2560:
  - Receives UART frame
  - Checks checksum validity
  - Displays distance on multiplexed 7-segment display

