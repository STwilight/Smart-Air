/*
 * FileServer.h
 *
 * Заголовочный файл к классу-обертке FTP-сервера
 *
 */

#ifndef APP_FILESERVER_H_
#define APP_FILESERVER_H_

#include "Config.h"
#include "Settings.h"
#include "Application.h"
#include <SmingCore/SmingCore.h>

class FileServer {
private:
	FTPServer ftpServer;

	String ftpServerName;
	String ftpServerPass;
	uint16_t ftpServerPort;

public:
	/* Конструктор по-умолчанию */
	FileServer();

	/* Метод инициализации и запуска FTP-сервера */
	void ftpInit();

	/* Методы получения и сохранения конфигурации */
	String getSettings();
	void setSettings(String settings);

	/* Метод применения конфигурации */
	void applySettings();

	/* Метод, выполняющий подготовку FTP модуля для перезагрузки системы */
	void onSystemRestart();
};

#endif /* APP_FILESERVER_H_ */
