/*
 * Settings.h
 *
 * Заголовочный файл, содержащий реализацию хранилища настроек
 *
 */

#include <SmingCore/SmingCore.h>

#ifndef APP_SETTINGS_H_
#define APP_SETTINGS_H_

/* Определения названий файлов с настройками */
#define SYS_SETTINGS_FILE  ".system.conf"
#define SEC_SETTINGS_FILE  ".security.conf"
#define APP_SETTINGS_FILE  ".application.conf"
#define WIFI_SETTINGS_FILE ".wireless.conf"

/* Определения типов настроек */
#define SYS_SETTINGS  0x00
#define SEC_SETTINGS  0x01
#define APP_SETTINGS  0x02
#define WIFI_SETTINGS 0x03

struct SettingsStorage
{
	/* Структура для выполнения сохранения и загрузки настроек в/из файла в формате JSON */

	String load(byte type) {
		/* Метод загрузки настроек в формате JSON */

		String settings;

		switch (type) {
			case SYS_SETTINGS:
				settings = readJSONFile(SYS_SETTINGS_FILE);
				break;
			case SEC_SETTINGS:
				settings = readJSONFile(SEC_SETTINGS_FILE);
				break;
			case APP_SETTINGS:
				settings = readJSONFile(APP_SETTINGS_FILE);
				break;
			case WIFI_SETTINGS:
				settings = readJSONFile(WIFI_SETTINGS_FILE);
				break;
			default:
				settings = "";
				break;
		}

		return settings;
	}
	bool save(String settings, byte type) {
		/* Метод сохранения настроек */

		switch (type) {
			case SYS_SETTINGS:
				return writeJSONFile(settings, SYS_SETTINGS_FILE);
				break;
			case SEC_SETTINGS:
				return writeJSONFile(settings, SEC_SETTINGS_FILE);
				break;
			case APP_SETTINGS:
				return writeJSONFile(settings, APP_SETTINGS_FILE);
				break;
			case WIFI_SETTINGS:
				return writeJSONFile(settings, WIFI_SETTINGS_FILE);
				break;
			default:
				return false;
				break;
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
