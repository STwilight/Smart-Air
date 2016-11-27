
/*
 * Application.h
 *
 * Основной файл приложения
 *
 */

/* Подключение заголовочных файлов Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* Подключение конфигурационных файлов проекта */
#include "Config.h"
#include "Boards.h"

/* Подключение заголовочного файла хранилища настроек */
#include "Settings.h"

/* Подключение заголовочных файлов модулей */
#include "AirConditioner.h"
#include "TimeClient.h"
#include "FileServer.h"
#include "WebServer.h"
#include "WiFi.h"

/* Ссылки на объекты классов */
AirConditioner *aircond;
TimeClient *ntpclient;
FileServer *ftpserver;
WebServer *webserver;
WiFi *wifi;

void init()
{
	/* Основной метод */

	// TODO: Добавить класс UART-интерфейса
	/* Инициализация UART */
	/* DEBUG */ // Serial.begin(115200);
	/* DEBUG */ // Serial.systemDebugOutput(false);
	/* DEBUG */ // Serial.commandProcessing(false);

	// TODO: Добавить установку режима сна в класс Wi-Fi модуля
	/* Отключение сна для Wi-Fi модуля */
	/* DEBUG */ // wifi_set_sleep_type(NONE_SLEEP_T);

	// TODO: Изучить доступные события Wi-Fi модуля
	/* Класс событий Wi-Fi модуля */
	/* DEBUG */ // WifiEvents

	/* Установка рабочей частоты процессора на 160 МГц */
	System.setCpuFrequency(eCF_160MHz);

	/* Создание экземпляра класса модуля кондиционера */
	// aircond = new AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);

	/* Создание экземпляра класса модуля кондиционера - отладка на плате Smart-Rock */
	/* DEBUG */ aircond = new AirConditioner(GPIO0, GPIO14, GPIO12, GPIO13, GPIO2, 11);

	/* Монтирование файловой системы */
	spiffs_mount();

	/* Инициализация модуля кондиционера */
	aircond->setSettings(Settings.load(APP_SETTINGS));
	aircond->applySettings();

	/* Создание экземпляра класса и инициализация Wi-Fi модуля */
	wifi = new WiFi();
	wifi->setSettings(Settings.load(WIFI_SETTINGS));
	wifi->wifiInit();

	// TODO: Реализовать шифрование хранимых файлов конфигурации с помощью AES
	/* Создание экземпляра класса и инициализация NTP-клиента */
	ntpclient = new TimeClient();
	ntpclient->setSettings(Settings.load(SYS_SETTINGS));
	ntpclient->ntpInit();

	/* Создание экземпляра класса и инициализация FTP-сервера */
	ftpserver = new FileServer();
	ftpserver->setSettings(Settings.load(SEC_SETTINGS));
	ftpserver->ftpInit();

	// TODO: Ознакомиться с SSL, доступным в Sming 2.1.5
	// TODO: Реализовать шифрование передаваемых через WEB-сервер данных с помощью SSL или AES
	/* Создание экземпляра класса и инициализация Web-сервера */
	webserver = new WebServer(WEB_SERVER_PORT);
};

extern void systemRestart() {
	/* Метод, вызываемый внешними модулями при необходимости перезагрузки системы */

	aircond->onSystemRestart();
	ntpclient->onSystemRestart();
	wifi->onSystemRestart();
	ftpserver->onSystemRestart();

	spiffs_unmount();
	System.restart();
}
