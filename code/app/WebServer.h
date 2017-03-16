/*
 * WebServer.h
 *
 * Заголовочный файл к классу WEB-сервера
 *
 */

#ifndef APP_WEBSERVER_H_
#define APP_WEBSERVER_H_

#include "Config.h"
#include "Settings.h"
#include "Application.h"
#include <SmingCore/SmingCore.h>

class WebServer {
private:
	static HttpServer webServer;

	/* Метод выдачи содержимого страниц */
	static void onWebPage(HttpRequest &request, HttpResponse &response);

	/* Методы обработки событий WebSocket'ов */
	static void webSocketConnected(WebSocket& socket);
	static void webSocketMessageReceived(WebSocket& socket, const String& message);
	static void webSocketBinaryReceived(WebSocket& socket, uint8_t* data, size_t size);
	static void webSocketDisconnected(WebSocket& socket);

	/* Метод для отправки файла */
	static void onFile(HttpRequest &request, HttpResponse &response);

	/* Метод для создания файла в случае его отсутствия */
	static bool fileNotExist(String filename, bool createfile = true, bool deletezerofile = false);

public:
	/* Конструктор по-умолчанию */
	WebServer(uint16_t port);

	/* Методы для выдачи/получения файла с текущей конфигурацией устройства */
	static String onBackup();
	static void onRestore(String filename);

	/* Метод, выполняющий остановку процессов внутри модуля */
	void stopModule();

	/* Метод, выполняющий подготовку модуля для перезагрузки системы */
	void onSystemRestart();
};

#endif /* APP_WEBSERVER_H_ */
