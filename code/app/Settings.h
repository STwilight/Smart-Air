/*
 * Settings.h
 *
 * Заголовочный файл, содержащий реализацию хранилища настроек
 *
 */

#include "Application.h"
#include <SmingCore/SmingCore.h>

#ifndef APP_SETTINGS_H_
#define APP_SETTINGS_H_

/* Определения имен файлов с настройками */
#define SYS_INFO	  ".system.info"
#define DEV_SETTINGS  ".device.conf"
#define SCH_SETTINGS  ".scheduler.conf"
#define FTP_SETTINGS  ".fileserver.conf"
#define TCP_SETTINGS  ".tcpserver.conf"
#define NTP_SETTINGS  ".ntpserver.conf"
#define WIFI_SETTINGS ".wireless.conf"

/* Определение размера массива для хранения имен файлов */
#define FILES_COUNT   7

/* Определение имени файла с комулятивного настройками */
#define BAK_SETTINGS  ".backup"

struct SettingsStorage
{
	/* Структура для выполнения сохранения и загрузки настроек в/из файла в формате JSON */

	String set_files[FILES_COUNT] = {SYS_INFO, DEV_SETTINGS, SCH_SETTINGS, FTP_SETTINGS, TCP_SETTINGS, NTP_SETTINGS, WIFI_SETTINGS};

	String load(String filename) {
		/* Метод загрузки настроек в формате JSON */

		return readJSONFile(filename);
	}
	bool save(String settings, String filename) {
		/* Метод сохранения настроек */

		return writeJSONFile(settings, filename);
	}

	String backup() {
		/* Метод для сбора JSON-конфигураций из файлов в единый backup-файл */

		String year, month, day, hour, minute, second, file, timestamp, content;
		DateTime currentDateTime = SystemClock.now(eTZ_UTC);
		char buffer[5];

		sprintf(buffer, "%04u", currentDateTime.Year);
		year = buffer;
		sprintf(buffer, "%02u", currentDateTime.Month);
		month = buffer;
		sprintf(buffer, "%02u", currentDateTime.Day);
		day = buffer;
		sprintf(buffer, "%02u", currentDateTime.Hour);
		hour = buffer;
		sprintf(buffer, "%02u", currentDateTime.Minute);
		minute = buffer;
		sprintf(buffer, "%02u", currentDateTime.Second);
		second = buffer;

		timestamp = year + "-" + month + "-" + day + ", " + hour + ":" + minute + ":" + second;
			 file = "dev-" + device_sn + BAK_SETTINGS;

		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();

		 JsonObject& info = jsonBuffer.createObject();
		 	 root["info"] = info;
		info["timestamp"] = timestamp;
		 info["dev_name"] = device_name;
		   info["dev_sn"] = device_sn;

	 JsonObject& settings = jsonBuffer.createObject();
		 root["settings"] = settings;

		saveConfigs();

		for(uint8_t i = 0; i < FILES_COUNT; i++) {
			content = readJSONFile(set_files[i]);
			settings[set_files[i]] = content;
		}

		String jsonString;
		root.printTo(jsonString);

		writeJSONFile(jsonString, file);

		return file;
	}
	void restore(String filename) {
		/* Метод для записи JSON-конфигурации из backup-файла в целевые файлы конфигураций модулей */

		String data = readJSONFile(filename);

		if(data.length() != 0) {
			DynamicJsonBuffer jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(data);

			JsonObject& settings = root["settings"];

			for(uint8_t i = 0; i < sizeof(set_files); i++) {
				writeJSONFile(settings[set_files[i]].asString(), set_files[i]);
			}
		}
	}

	String readJSONFile(String filename) {
		/* Метод чтения JSON-конфигурации из файла */

		String data = "";

		WDT.alive();
		if(fileExist(filename) & (fileGetSize(filename) != 0)) {
			int size = fileGetSize(filename);
			char* jsonString = new char[size + 1];
			fileGetContent(filename, jsonString, size + 1);
			data = jsonString;
			delete[] jsonString;
		}

		return data;
	}
	bool writeJSONFile(String data, String filename) {
		/* Метод записи JSON-конфигурации в файл */

		if((data.length() !=0) & (filename.length() != 0)) {
			WDT.alive();
			fileSetContent(filename, data);
		}
		else
			return false;

		WDT.alive();
		if(fileExist(filename) & fileGetContent(filename).equals(data))
			return true;
		else
			return false;
	}
};

static SettingsStorage Settings;

#endif /* APP_SETTINGS_H_ */
