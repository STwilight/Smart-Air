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
#define DEV_SETTINGS  ".device.conf"
#define SCH_SETTINGS  ".scheduler.conf"
#define FTP_SETTINGS  ".fileserver.conf"
#define TCP_SETTINGS  ".tcpserver.conf"
#define NTP_SETTINGS  ".ntpserver.conf"
#define WIFI_SETTINGS ".wireless.conf"

/* Определение имени файла с комулятивного настройками */
#define BAK_SETTINGS  ".backup"

struct SettingsStorage
{
	/* Структура для выполнения сохранения и загрузки настроек в/из файла в формате JSON */

	String set_files[6] = {DEV_SETTINGS, SCH_SETTINGS, FTP_SETTINGS, TCP_SETTINGS, NTP_SETTINGS, WIFI_SETTINGS};

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

		DateTime currentDateTime = SystemClock.now(eTZ_UTC);
		String file, timestamp;

		timestamp.concat(currentDateTime.Year + "-");
		timestamp.concat(currentDateTime.Month + "-");
		timestamp.concat(currentDateTime.Day + ", ");
		timestamp.concat(currentDateTime.Hour + ".");
		timestamp.concat(currentDateTime.Minute + ".");
		timestamp.concat(currentDateTime.Second);

		file.concat(timestamp + "_" + BAK_SETTINGS);

		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();

		 JsonObject& info = jsonBuffer.createObject();
		 	 root["info"] = info;
		info["timestamp"] = timestamp;
		 info["dev_name"] = device_name;
		   info["dev_sn"] = device_sn;

	 JsonObject& settings = jsonBuffer.createObject();
		 root["settings"] = settings;

		for(uint8_t i = 0; i < sizeof(set_files); i++) {
			settings[set_files[i]] = readJSONFile(set_files[i]);
		}

		String jsonString;
		root.printTo(jsonString);

		writeJSONFile(jsonString, BAK_SETTINGS);

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
