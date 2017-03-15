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
#include "Scheduler.h"
#include "WiFi.h"
#include "UART.h"

/* Ссылки на объекты классов */
AirConditioner *aircond;
TimeClient *ntpclient;
FileServer *ftpserver;
WebServer *webserver;
Scheduler *scheduler;
WiFi *wifi;
UART *uart;

/* Объявление для глобальных внешних (extern) переменных */
String device_name;
String device_sn;
uint16_t current_year;


void init()
{
	/* Основной метод */

	/* Инициализация UART */
	uart = new UART();

	/* Установка рабочей частоты процессора на 160 МГц */
	System.setCpuFrequency(eCF_160MHz);

	/* DEBUG-переменная для отладки программного обеспечения на плате Smart-Rock */
	#ifdef DEBUG
		/* Создание экземпляра класса модуля кондиционера - отладка на плате Smart-Rock */
		aircond = new AirConditioner(GPIO0, GPIO14, GPIO12, GPIO13, GPIO2, 11);
	#else
		/* Создание экземпляра класса модуля кондиционера */
		aircond = new AirConditioner(GPIO13, GPIO12, GPIO16, GPIO14, GPIO4, 11);
	#endif

	/* Монтирование файловой системы */
	spiffs_mount();

	/* Создание экземпляра класса и инициализация модуля планировщика */
	scheduler = new Scheduler();
	scheduler->setSettings(Settings.load(SCH_SETTINGS));

	/* Инициализация модуля кондиционера */
	aircond->setSettings(Settings.load(DEV_SETTINGS));

	/* Создание экземпляра класса и инициализация Wi-Fi модуля */
	wifi = new WiFi();
	wifi->setSettings(Settings.load(WIFI_SETTINGS));
	wifi->wifiInit();

	/* Инициализация системных переменных */
	vars_init();

	// TODO: Реализовать шифрование хранимых файлов конфигурации с помощью AES
	/* Создание экземпляра класса и инициализация NTP-клиента */
	ntpclient = new TimeClient();
	ntpclient->setSettings(Settings.load(NTP_SETTINGS));
	ntpclient->ntpInit();

	/* Создание экземпляра класса и инициализация FTP-сервера */
	ftpserver = new FileServer();
	ftpserver->setSettings(Settings.load(FTP_SETTINGS));
	ftpserver->ftpInit();

	// TODO: Реализовать шифрование передаваемых через WEB-сервер данных с помощью SSL или AES
	// TODO: Реализовать websocket-клиент для обмена настройками между различными устройствами
	/* Создание экземпляра класса и инициализация Web-сервера */
	webserver = new WebServer(WEB_SERVER_PORT);

	/* Запуск модулей планировщика и кондиционера */
	scheduler->start();
	aircond->start();
};
void vars_init() {
	setSysInfo(Settings.load(SYS_INFO));
	if(device_name.length() == 0)
		device_name = wifi->getDefaulDeviceName();
	device_sn = wifi->getSN();
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
		if(root.containsKey("sys_info") && root.get("sys_info").size() != 0) {
			JsonObject& sys_info = root["sys_info"];
			if(sys_info.containsKey("dev_name"))
				device_name = sys_info["dev_name"].asString();
		}
	}
}
String debugMethod() {
	/* Метод для выполнения отладки */

	DateTime currentDateTime = SystemClock.now(eTZ_UTC);

	String data = "Time is ";
	data.concat(currentDateTime.Hour);
	data.concat(":");
	data.concat(currentDateTime.Minute);
	data.concat(", day of week is ");
	data.concat(currentDateTime.DayofWeek);

	return data;
}

extern void stopModules() {
	/* Метод, вызываемый внешними модулями при необходимости остановки всех модулей */

	aircond->stopModule();
	scheduler->stopModule();
	ntpclient->stopModule();
	wifi->stopModule();
	ftpserver->stopModule();
}
extern void saveConfigs() {
	/* Метод, вызываемый внешними модулями при необходимости сохранения конфигурации всех модулей */

	aircond->saveConfig();
	scheduler->saveConfig();
	ntpclient->saveConfig();
	wifi->saveConfig();
	ftpserver->saveConfig();
	Settings.save(getSysInfo(), SYS_INFO);
}
extern void systemRestart() {
	/* Метод, вызываемый внешними модулями при необходимости перезагрузки системы */

	aircond->onSystemRestart();
	scheduler->onSystemRestart();
	ntpclient->onSystemRestart();
	wifi->onSystemRestart();
	ftpserver->onSystemRestart();
	Settings.save(getSysInfo(), SYS_INFO);

	spiffs_unmount();
	System.restart();
}
extern String processData(byte type, String data) {
	/* Метод, вызываемый внешними модулями при необходимости получения/установки настроек в формате JSON */

	String message;

	switch (type) {
		// Получение настроек и параметров
		case AIR_SET:
			message = aircond->getSettings();
			break;
		case AIR_STA:
			message = aircond->getState();
			break;
		case NTP_SET:
			message = ntpclient->getSettings();
			break;
		case FTP_SET:
			message = ftpserver->getSettings();
			break;
		case WIFI_SET:
			message = wifi->getSettings();
			break;
		case WIFI_STA:
			message = wifi->getState();
			break;
		case WIFI_HW:
			message = wifi->getHardwareInfo();
			break;
		case WIFI_NET:
			message = wifi->scanWiFiNetworks();
			break;
		case SCH_SET:
			message = scheduler->getSettings();
			break;
		case SYS_INF:
			message = getSysInfo();
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
			scheduler->setSettings(data);
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
		// Глобальный Backup/Restore настроек и остановка модулей
		case SYS_RES:
			systemRestart();
			break;
		case STP_ALL:
			stopModules();
			break;
		case SAV_CFG:
			saveConfigs();
			break;
		case BAK_CFG:
			message = webserver->onBackup();
			break;
		case RES_CFG:
			webserver->onRestore(data);
			break;
		// DEBUG
		case DBG_REQ:
			message = debugMethod();
			break;
		default:
			message = "";
			break;
	}

	return message;
}
extern bool getSchedulerStatus() {
	/* Метод, вызываемый внешним модулем при необходимости получения флага срабатывания расписания планировщика */

	return scheduler->getStatus();
}
