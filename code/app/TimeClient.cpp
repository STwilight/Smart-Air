/*
 * TimeClient.cpp
 *
 * Класс-обертка, описывающий NTP-клиент
 *
 */

#include "TimeClient.h"

TimeClient::TimeClient(String ntpServerAddress, unsigned int ntpRequestInterval, double ntpTimezone) {
	/* Конструктор по-умолчанию */

	this->ntpServerAddress = ntpServerAddress;
	this->ntpRequestInterval = ntpRequestInterval;
	this->ntpTimezone = ntpTimezone;
	this->ntpClient = null;
}

void TimeClient::ntpInit() {
	/* Метод инициализации системных часов и запуска NTP-клиента */

	SystemClock.setTimeZone(this->ntpTimezone);
	this->ntpClient = new NtpClient(this->ntpServerAddress, this->ntpRequestInterval, TimeClient::updateSystemClock);
}

void TimeClient::updateSystemClock(NtpClient& client, time_t timestamp) {
	/* Метод, отвечающий за обновление даты и времени системы */

	SystemClock.setTime(timestamp);

	DateTime currentDateTime = SystemClock.now(eTZ_UTC);
	current_year = currentDateTime.Year;

	/* DEBUG */
		int8_t day_of_week = currentDateTime.DayofWeek;
		int8_t hour = currentDateTime.Hour;
		int8_t minute = currentDateTime.Minute;
		String data = "Time is ";
		data.concat(hour);
		data.concat(":");
		data.concat(minute);
		data.concat(", day of week is ");
		data.concat(day_of_week);
		Serial.println(data);
	/* DEBUG */
}

String TimeClient::getSettings() {
	/* Получение конфигурации NTP-клиента в формате JSON-строки */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

				JsonObject& ntp_client = jsonBuffer.createObject();
					root["ntp_client"] = ntp_client;

	  ntp_client["ntp_server_address"] = this->ntpServerAddress;
	ntp_client["ntp_request_interval"] = (unsigned int) this->ntpRequestInterval;
			ntp_client["ntp_timezone"] = (double) this->ntpTimezone;

	String jsonString;
	root.printTo(jsonString);

	return jsonString;
}
void TimeClient::setSettings(String jsonString) {
	/* Сохранение конфигурации NTP-клиента из формата JSON-строки */

	if(jsonString.length() != 0) {
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(jsonString);

		  JsonObject& ntp_client = root["ntp_client"];
		  this->ntpServerAddress = ntp_client["ntp_server_address"].asString();
		this->ntpRequestInterval = ntp_client["ntp_request_interval"];
			   this->ntpTimezone = ntp_client["ntp_timezone"];
	}
}
void TimeClient::applySettings() {
	/* Метод применения конфигурации */

	systemRestart();
}

void TimeClient::onSystemRestart() {
	/* Метод, выполняющий подготовку NTP модуля для перезагрузки системы */

	this->ntpClient->setAutoQuery(false);

	Settings.save(this->getSettings(), SYS_SETTINGS);
}
