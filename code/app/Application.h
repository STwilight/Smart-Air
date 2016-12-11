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

extern void systemRestart();
extern String getData(byte type);

#endif /* APP_APPLICATION_H_ */
