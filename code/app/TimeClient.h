/*
 * TimeClient.h
 *
 * Заголовочный файл к классу-обертке NTP-клиента
 *
 */

#ifndef APP_TIMECLIENT_H_
#define APP_TIMECLIENT_H_

#include "Settings.h"
#include "Application.h"
#include <SmingCore/SmingCore.h>

class TimeClient {
private:
	NtpClient *ntpClient;

	String ntpServerAddress;
	unsigned int ntpRequestInterval;
	double ntpTimezone;

	/* Метод, отвечающий за обновление даты и времени системы */
	static void updateSystemClock(NtpClient& client, time_t timestamp);

public:
	/* Конструктор по-умолчанию */
	TimeClient(String ntpServerAddress = "pool.ntp.org", unsigned int ntpRequestInterval = 3600, double ntpTimezone = 0);

	/* Метод инициализации системных часов и запуска NTP-клиента */
	void ntpInit();

	/* Методы получения и сохранения конфигурации */
	String getSettings();
	void setSettings(String settings);

	/* Метод применения конфигурации */
	void applySettings();

	/* Метод, выполняющий остановку процессов внутри модуля */
	void stopModule();

	/* Метод, выполняющий сохранение конфигурации модуля в файл */
	void saveConfig();

	/* Метод, выполняющий подготовку модуля для перезагрузки системы */
	void onSystemRestart();
};

#endif /* APP_TIMECLIENT_H_ */
