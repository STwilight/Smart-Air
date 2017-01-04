/*
 * Definitions.h
 *
 * Заголовочный файл, описывающий специфические определения.
 *
 */

#ifndef APP_DEFINITIONS_H_
#define APP_DEFINITIONS_H_

/* Определение состояния вывода порта */
#define On		true
#define Off		false

/* Определения типов настроек для выдачи в формате JSON */
#define AIR_SET  0x00
#define AIR_STA  0x01
#define NTP_SET  0x02
#define FTP_SET  0x03
#define WIFI_SET 0x04
#define WIFI_STA 0x05
#define WIFI_HW	 0x06
#define SCH_SET  0x07

/* Определения типов настроек для получения в формате JSON */
#define AIR_CFG  0x10
#define NTP_CFG  0x12
#define FTP_CFG  0x13
#define WIFI_CFG 0x14
#define SCH_CFG  0x17

#endif /* APP_DEFINITIONS_H_ */
