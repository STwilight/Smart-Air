/*
 * WebServer.h
 *
 * Заголовочный файл к классу WEB-сервера
 *
 */

#ifndef APP_WEBSERVER_H_
#define APP_WEBSERVER_H_

#include <SmingCore/SmingCore.h>

class WebServer {
private:
	HttpServer webServer;

	/* Методы для выдачи страницы мониторинга и ее содержимого */
	static void onStatus(HttpRequest &request, HttpResponse &response);
	static void onAjaxStatus(HttpRequest &request, HttpResponse &response);

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
	static void fileNotExist(String filename);

public:
	/* Конструктор по-умолчанию */
	WebServer(uint16_t port);
};

#endif /* APP_WEBSERVER_H_ */
