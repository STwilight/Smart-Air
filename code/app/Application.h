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
extern String getData(byte type);

/* Объявление системных функций */
void vars_init();

#endif /* APP_APPLICATION_H_ */
