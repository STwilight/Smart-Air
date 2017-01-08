/*
 * Functions.h
 *
 * Заголовочный файл, содержащий реализацию специфических функций
 *
 */

#ifndef APP_FUNCTIONS_H_
#define APP_FUNCTIONS_H_

String setDefaultValue(String value, String default_value) {
	if(value.length() != 0)
		return value;
	else
		return default_value;
}

#endif /* APP_FUNCTIONS_H_ */
