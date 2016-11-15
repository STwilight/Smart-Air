/*
 * Config.h
 *
 * Заголовочный файл, служающий для конфигурации проекта
 *
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

/* Определение типа платы используемого модуля ESP8266 */
#define ESP07

/* Определение имени проекта */
#define PROJECT_NAME "Smart Air"

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
