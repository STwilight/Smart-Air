/*
 * Scheduler.h
 *
 * Заголовочный файл к классу планировщика
 *
 */

#ifndef APP_SCHEDULER_H_
#define APP_SCHEDULER_H_

#include "Settings.h"
#include "Application.h"
#include <SmingCore/SmingCore.h>

class Scheduler {
public:
	/* Конструктор по-умолчанию */
	Scheduler();

	/* Методы получения и сохранения конфигурации */
	String getSettings();
	void setSettings(String settings);
};

#endif /* APP_SCHEDULER_H_ */
