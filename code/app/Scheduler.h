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
private:
	static bool scheduler_enabled;	/* Флаг режима работы:
	 	 	 	 	 	 	 	 	 *		Ручной (планировщик запрещен) 		  = false
	 	 	 	 	 	 	 	 	 *		Автоматический (планировщик разрешен) = true
	 	 	 	 	 	 	 	 	 */

	static bool schedule_triggered; /* Флаг срабатывания расписания:
									 *		Работа запрещена = false
									 *		Работа разрешена = true
									 */

	static byte time_from_hours;	/* Время начала (часы)
							 	 	 *		Значение: 0 .. 23
							 	 	 */

	static byte time_from_mins;		/* Время начала (минуты)
							 	 	 *		Значение: 0 .. 59
							 	 	 */

	static byte time_to_hours;		/* Время окончания (часы)
							 	 	 *		Значение: 0 .. 23
							 	  	 */

	static byte time_to_mins;		/* Время окончания (минуты)
							 	 	 *		Значение: 0 .. 59
							 	  	 */

	static byte days_of_week;		/* Дни недели для включения
							 	 	 *		Значение: 0 .. 127 (двоичный паттерн: х СБ ПТ ЧТ СР ВТ ПН ВС)
							 	  	 */

	/* Таймеры для отсчета времени */
	Timer executeScheduler;

	/* Реализация функционала внутреннего планировщика */
	static void execScheduler();

public:
	/* Конструктор по-умолчанию */
	Scheduler();

	/* Метод для запуска планировщика в работу */
	void start();

	/* Методы получения и сохранения конфигурации */
	String getSettings();
	void setSettings(String settings);

	/* Метод получения состояния флага срабатывания расписания */
	bool getStatus();

	/* Метод, выполняющий подготовку модуля планировщика для перезагрузки системы */
	void onSystemRestart();
};

#endif /* APP_SCHEDULER_H_ */
