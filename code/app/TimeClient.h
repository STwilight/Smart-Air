/*
 * TimeClient.h
 *
 * Заголовочный файл к классу-обертке NTP-клиента
 *
 */

#ifndef APP_TIMECLIENT_H_
#define APP_TIMECLIENT_H_

#include <SmingCore/SmingCore.h>

class TimeClient {
private:
	NtpClient *ntpClient;

	String ntpServerAddress;
	unsigned int ntpRequestInterval;
	double ntpTimezone;
	// REQUESTINTERVAL должен быть в секундах
	// TIMEZONE должен быть со знаком минус для правильной работы (баг Sming)!

	/* Метод, отвечающий за обновление даты и времени системы */
	static void updateSystemClock(NtpClient& client, time_t timestamp);

public:
	/* Конструктор по-умолчанию */
	TimeClient(String ntpServerAddress = "pool.ntp.org", unsigned int ntpRequestInterval = 3600, double ntpTimezone = 0);

	/* Метод инициализации системных часов и запуска NTP-клиента */
	void Init();

	/* Методы получения и сохранения конфигурации */
	String getSettings();
	void setSettings(String settings);
};

#endif /* APP_TIMECLIENT_H_ */
