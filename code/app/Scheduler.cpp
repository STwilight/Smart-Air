/*
 * TimeClient.cpp
 *
 * Класс, описывающий планировщик
 *
 */

#include "Scheduler.h"

/* Декларирование static-переменных класса Scheduler */
	bool Scheduler::scheduler_enabled;
	bool Scheduler::schedule_triggered;
	byte Scheduler::time_from_hours;
	byte Scheduler::time_from_mins;
	byte Scheduler::time_to_hours;
	byte Scheduler::time_to_mins;
	byte Scheduler::days_of_week;

Scheduler::Scheduler() {
	/* Конструктор по-умолчанию */

	this->scheduler_enabled  = false;
	this->schedule_triggered = true;

	this->time_from_hours	 = 0;
	this->time_from_mins	 = 0;
	this->time_to_hours		 = 0;
	this->time_to_mins		 = 0;
	this->days_of_week		 = 0;
}

void Scheduler::start() {
	/* Метод для запуска кондиционера в работу */

	this->executeScheduler.initializeMs(1000, Scheduler::execScheduler).start();
}

void Scheduler::execScheduler() {
	/* Основной исполняемый метод, реализующий функционал планировщика */

	if(Scheduler::scheduler_enabled) {
		DateTime currentDateTime = SystemClock.now(eTZ_UTC);
		byte current_day_of_week = currentDateTime.DayofWeek;
			   byte current_hour = currentDateTime.Hour;
			 byte current_minute = currentDateTime.Minute;

		if(((Scheduler::days_of_week & (1 << current_day_of_week)) >> current_day_of_week) && \
		  ((current_hour >= Scheduler::time_from_hours) && (current_minute >= Scheduler::time_from_mins)) && \
		  ((current_hour <= Scheduler::time_to_hours) && (current_minute < Scheduler::time_to_mins)))
				Scheduler::schedule_triggered = true;
		else
			Scheduler::schedule_triggered = false;
	}
	else
		Scheduler::schedule_triggered = true;
}

String Scheduler::getSettings() {
	/* Получение конфигурации планировщика в формате JSON-строки */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	JsonObject& settings		= jsonBuffer.createObject();
	root["settings"]			= settings;
	settings["scheduler"]		= (bool) this->scheduler_enabled;
	settings["dow"]				= this->days_of_week;
	settings["time_from_hours"]	= this->time_from_hours;
	settings["time_from_mins"]	= this->time_from_mins;
	settings["time_to_hours"]	= this->time_to_hours;
	settings["time_to_mins"]	= this->time_to_mins;

	String jsonString;
	root.printTo(jsonString);

	return jsonString;
}
void Scheduler::setSettings(String jsonString) {
	/* Сохранение конфигурации планировщика из формата JSON-строки */

	if(jsonString.length() != 0) {
		DynamicJsonBuffer jsonBuffer;

		JsonObject& root = jsonBuffer.parseObject(jsonString);
		JsonObject& settings = root["settings"];

		this->scheduler_enabled = settings["scheduler"];
		this->days_of_week		= settings["dow"];
		this->time_from_hours	= settings["time_from_hours"];
		this->time_from_mins	= settings["time_from_mins"];
		this->time_to_hours		= settings["time_to_hours"];
		this->time_to_mins		= settings["time_to_mins"];

		Settings.save(this->getSettings(), SCH_SETTINGS);
	}
}

bool Scheduler::getStatus() {
	/* Метод получения состояния флага срабатывания расписания */

	return this->schedule_triggered;
}

void Scheduler::stopModule() {
	/* Метод, выполняющий остановку процессов внутри модуля */

	if(this->executeScheduler.isStarted())
		this->executeScheduler.stop();
}
void Scheduler::saveConfig() {
	/* Метод, выполняющий сохранение конфигурации модуля в файл */

	Settings.save(this->getSettings(), SCH_SETTINGS);
}
void Scheduler::onSystemRestart() {
	/* Метод, выполняющий подготовку модуля для перезагрузки системы */

	this->stopModule();
	this->saveConfig();
}

