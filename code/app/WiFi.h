/*
 * WiFi.h
 *
 * Заголовочный файл класса, описывающего методы управления Wi-Fi модулем ESP8266
 *
 */

#ifndef APP_WIFI_H_
#define APP_WIFI_H_

#include "Definitions.h"
#include <SmingCore/SmingCore.h>

class WiFi {
private:
	/* Определение значений для режима "Точка доступа Wi-Fi" */
	String ap_wifi_ssid;
	String ap_wifi_pwd;
	  bool ap_wifi_mode;

	/* Определение значений для режима "Ошибка подключения / отсутствие конфигурации Wi-Fi" */
	String ap_wifi_def_ssid;
	  bool ap_wifi_def_mode;

	/* Определение значений для режима "Клиент Wi-Fi сети" */
	String st_wifi_ssid;
	String st_wifi_pwd;
	  bool st_wifi_mode;
	  bool st_wifi_err;

public:
	WiFi();
};

#endif /* APP_WIFI_H_ */
