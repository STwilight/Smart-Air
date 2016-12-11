/*
 * WebServer.h
 *
 * Заголовочный файл к классу WEB-сервера
 *
 */

#ifndef APP_WEBSERVER_H_
#define APP_WEBSERVER_H_

#include "Application.h"
#include <SmingCore/SmingCore.h>

class WebServer {
private:
	static HttpServer webServer;

	/* Методы для выдачи страницы мониторинга и ее содержимого */
	static void onStatus(HttpRequest &request, HttpResponse &response);
	static void onAjaxStatus(HttpRequest &request, HttpResponse &response);

	/* DEBUG - WebSockets */
		/* Метод выдачи страницы отладки */
		static void onDebug(HttpRequest &request, HttpResponse &response);

		/* Методы обработки событий WebSocket'ов */
		static void webSocketConnected(WebSocket& socket);
		static void webSocketMessageReceived(WebSocket& socket, const String& message);
		static void webSocketBinaryReceived(WebSocket& socket, uint8_t* data, size_t size);
		static void webSocketDisconnected(WebSocket& socket);
	/* DEBUG - WebSockets */

	/* Методы для выдачи системных страниц */
	static void onConfig(HttpRequest &request, HttpResponse &response);
	static void onNetwork(HttpRequest &request, HttpResponse &response);
	static void onSecurity(HttpRequest &request, HttpResponse &response);
	static void onUpdate(HttpRequest &request, HttpResponse &response);
	static void onBackup(HttpRequest &request, HttpResponse &response);
	static void onRestore(HttpRequest &request, HttpResponse &response);
	static void onAbout(HttpRequest &request, HttpResponse &response);

	/* Метод для отправки файла */
	static void onFile(HttpRequest &request, HttpResponse &response);

	/* Метод для создания файла в случае его отсутствия */
	static void fileNotExist(String filename, bool createfile = true);

public:
	/* Конструктор по-умолчанию */
	WebServer(uint16_t port);
};

#endif /* APP_WEBSERVER_H_ */
