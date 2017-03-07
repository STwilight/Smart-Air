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
extern String device_name, device_sn;
extern uint16_t current_year;

/* Объявление внешних методов */
extern void stopModules();
extern void saveConfigs();
extern void systemRestart();
extern String processData(byte type, String data);
extern bool getSchedulerStatus();

/* Объявление системных функций */
void vars_init();

/* Методы получения и сохранения конфигурации */
String getSysInfo();
void setSysInfo(String jsonString);

/* Отладочный метод */
String debugMethod();

#endif /* APP_APPLICATION_H_ */
