/*
 * UART.cpp
 *
 * Класс, описывающий методы управления UART модулем ESP8266
 *
 */

#include "UART.h"

UART::UART(uint32_t baud_rate, bool debug_output, bool command_processing) {
	/* Конструктор по-умолчанию */

	Serial.begin(baud_rate);
	Serial.systemDebugOutput(debug_output);
	Serial.commandProcessing(command_processing);
}

