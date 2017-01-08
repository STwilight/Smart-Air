/*
 * Application.cpp
 *
 * Основной файл приложения
 *
 */

/* Подключение заголовочного файла приложения */
#include "Application.h"

/* Подключение файла с определениями */
#include "Definitions.h"

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

/* Объявление для глобальных внешних (extern) переменных */
String device_name;
uint16_t current_year;


void init()
{
	/* Основной метод */

	// TODO: Добавить класс UART-интерфейса
	/* Инициализация UART */
	/* DEBUG */ Serial.begin(115200);
	/* DEBUG */ Serial.systemDebugOutput(false);
	/* DEBUG */ Serial.commandProcessing(false);

	// TODO: Добавить установку режима сна в класс Wi-Fi модуля
	/* Отключение сна для Wi-Fi модуля */
	/* DEBUG */ wifi_set_sleep_type(NONE_SLEEP_T);

	// TODO: Изучить доступные события Wi-Fi модуля
	/* Класс событий Wi-Fi модуля */
	/* DEBUG */ // WifiEvents

	/* Установка рабочей частоты процессора на 160 МГц */
	System.setCpuFrequency(eCF_160MHz);

	/* DEBUG-переменная для отладки программного обеспечения на плате Smart-Rock */
	#ifdef DEBUG
		/* Создание экземпляра класса модуля кондиционера - отладка на плате Smart-Rock */
		aircond = new AirConditioner(GPIO0, GPIO14, GPIO12, GPIO13, GPIO2, 11);
	#else
		/* Создание экземпляра класса модуля кондиционера */
		aircond = new AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);
	#endif

	/* Монтирование файловой системы */
	spiffs_mount();

	/* Инициализация модуля кондиционера */
	aircond->setSettings(Settings.load(APP_SETTINGS));

	/* Создание экземпляра класса и инициализация Wi-Fi модуля */
	wifi = new WiFi();
	wifi->setSettings(Settings.load(WIFI_SETTINGS));
	wifi->wifiInit();

	/* Инициализация системных переменных */
	vars_init();

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
void vars_init() {
	device_name = wifi->getDefaulDeviceName();
	current_year = START_YEAR;
}
String getSysInfo() {
	/* Метод применения конфигурации системы */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

  JsonObject& sys_info = jsonBuffer.createObject();
	  root["sys_info"] = sys_info;

	sys_info["dev_name"] = device_name;
	sys_info["dev_fw"]   = FIRMWARE_VER;

	String jsonString;
	root.printTo(jsonString);

	return jsonString;
}
void setSysInfo(String jsonString) {
	/* Сохранение конфигурации системы из формата JSON-строки */

	if(jsonString.length() != 0) {
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(jsonString);
		device_name = root["dev_name"].asString();
	}
}

extern void systemRestart() {
	/* Метод, вызываемый внешними модулями при необходимости перезагрузки системы */

	aircond->onSystemRestart();
	ntpclient->onSystemRestart();
	wifi->onSystemRestart();
	ftpserver->onSystemRestart();

	spiffs_unmount();
	System.restart();
}
extern String processData(byte type, String data) {
	/* Метод, вызываемый внешними модулями при необходимости получения/установки настроек в формате JSON */

	String settings;

	switch (type) {
		// Получение настроек
		case AIR_SET:
			settings = aircond->getSettings();
			break;
		case AIR_STA:
			settings = aircond->getState();
			break;
		case NTP_SET:
			settings = ntpclient->getSettings();
			break;
		case FTP_SET:
			settings = ftpserver->getSettings();
			break;
		case WIFI_SET:
			settings = wifi->getSettings();
			break;
		case WIFI_STA:
			settings = wifi->getState();
			break;
		case WIFI_HW:
			settings = wifi->getHardwareInfo();
			break;
		case SCH_SET:
			// Зарезервировано для выдачи настроек планировщика
			break;
		case SYS_INF:
			settings = getSysInfo();
			break;
		// Сохранение настроек
		case AIR_CFG:
			aircond->setSettings(data);
			break;
		case NTP_CFG:
			ntpclient->setSettings(data);
			break;
		case FTP_CFG:
			ftpserver->setSettings(data);
			break;
		case WIFI_CFG:
			wifi->setSettings(data);
			break;
		case SCH_CFG:
			// Зарезервировано для сохранения настроек планировщика
			break;
		case SYS_CFG:
			setSysInfo(data);
			break;
		// Применение настроек
		case NTP_APP:
			ntpclient->setSettings(data);
			ntpclient->applySettings();
			break;
		case FTP_APP:
			ftpserver->setSettings(data);
			ftpserver->applySettings();
			break;
		case WIFI_APP:
			wifi->setSettings(data);
			wifi->applySettings();
			break;
		default:
			settings = "";
			break;
	}

	return settings;
}
