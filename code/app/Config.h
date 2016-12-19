/*
 * Config.h
 *
 * Заголовочный файл, служающий для конфигурации проекта
 *
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

/* Определение переменных среды IDE Eclipse для обратной совместимости
 *
 * Определения WIFI_SSID и WIFI_PWD хранят SSID и пароль точки доступа Wi-Fi для подключения в режиме "Клиент Wi-Fi сети".
 * Их значения задаются в свойствах проекта (Project > Properties > C/C++ Build > Environment > Environment variables to set).
 *
 */
#if !defined(WIFI_SSID) || !defined(WIFI_PWD)
	#define WIFI_SSID ""
	#define WIFI_PWD  ""
#endif

/* DEBUG-переменная для отладки программного обеспечения на плате Smart-Rock */
#define DEBUG

/* Определение типа платы используемого модуля ESP8266 */
#define ESP07

/* Определение специальных переменных проекта */
#define AUTHOR_NAME	 "Symrak"
#define PROJECT_NAME "Smart Air"
#define DEVICE_CLASS "Air conditioner"
#define START_YEAR	 2016
#define COMPANY_NAME "ITPROM LLC"

/* Определение логина и пароля по-умолчанию */
#define DEFAULT_LOGIN PROJECT_NAME
#define DEFAULT_PASS  ""

/* Определение версии программного обеспечения */
#define FIRMWARE_VER "1.0 Alpha"

/* Определения портов для серверов */
#define WEB_SERVER_PORT 80
#define FTP_SERVER_PORT 21
#define TCP_SERVER_PORT 9000

#endif /* APP_CONFIG_H_ */
