/*
 * FileServer.cpp
 *
 * Класс-обертка, описывающий FTP-сервер
 *
 */

#include "FileServer.h"

FileServer::FileServer() {
	/* Конструктор по-умолчанию */

	this->ftpServerName = DEFAULT_LOGIN;
	this->ftpServerPass = DEFAULT_PASS;
	this->ftpServerPort = FTP_SERVER_PORT;
}

void FileServer::ftpInit() {
	/* Метод инициализации и запуска FTP-сервера */

	ftpServer.addUser(this->ftpServerName, this->ftpServerPass);
	ftpServer.listen(this->ftpServerPort);
}

String FileServer::getSettings() {
	/* Получение конфигурации FTP-сервера в формате JSON-строки */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

		   JsonObject& ftp_server = jsonBuffer.createObject();
			   root["ftp_server"] = ftp_server;

	ftp_server["ftp_server_name"] = this->ftpServerName;
	ftp_server["ftp_server_pass"] = this->ftpServerPass;

	String jsonString;
	root.printTo(jsonString);

	return jsonString;
}
void FileServer::setSettings(String jsonString) {
	/* Сохранение конфигурации FTP-сервера из формата JSON-строки */

	if(jsonString.length() != 0) {
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(jsonString);

		JsonObject& ftp_server = root["ftp_server"];
		   this->ftpServerName = ftp_server["ftp_server_name"].asString();
		   this->ftpServerPass = ftp_server["ftp_server_pass"].asString();
	}
}
void FileServer::applySettings() {
	/* Метод применения конфигурации */

	systemRestart();
}

void FileServer::stopModule() {
	/* Метод, выполняющий остановку процессов внутри модуля */

	// Тут нечего делать. Пока.
}
void FileServer::saveConfig() {
	/* Метод, выполняющий сохранение конфигурации модуля в файл */

	Settings.save(this->getSettings(), FTP_SETTINGS);
}
void FileServer::onSystemRestart() {
	/* Метод, выполняющий подготовку модуля для перезагрузки системы */

	this->stopModule();
	this->saveConfig();
}
