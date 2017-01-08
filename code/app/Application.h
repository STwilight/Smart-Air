/*
 * Application.h
 *
 * Основной заголовочный файл приложения
 *
 */

#ifndef APP_APPLICATION_H_
#define APP_APPLICATION_H_

/* Подключение заголовочных файлов Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* Объявление глобальных внешних переменных */
extern String device_name;
extern uint16_t current_year;

/* Объявление внешних методов */
extern void systemRestart();
extern String processData(byte type, String data);

/* Объявление системных функций */
void vars_init();

/* Методы получения и сохранения конфигурации */
String getSysInfo();
void setSysInfo(String jsonString);

#endif /* APP_APPLICATION_H_ */
