/*
 * UART.h
 *
 * Заголовочный файл класса, описывающего методы управления UART модулем ESP8266
 *
 */

#ifndef APP_UART_H_
#define APP_UART_H_

#include <SmingCore/SmingCore.h>

class UART {
public:
	UART(uint32_t baud_rate = 115200, bool debug_output = false, bool command_processing = false);
};

#endif /* APP_UART_H_ */
