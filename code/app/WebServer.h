/*
 * WebServer.h
 *
 * Заголовочный файл к классу WEB-сервера
 *
 */

#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include "Config.h"
#include <SmingCore/SmingCore.h>

class WebServer {
private:
	HttpServer webServer;

	/* Методы для выдачи Status-страницы и ее содержимого */
	void onStatus(HttpRequest &request, HttpResponse &response);
	void onAjaxStatus(HttpRequest &request, HttpResponse &response);

	/* Методы для выдачи системных страниц */
	void onConfig(HttpRequest &request, HttpResponse &response);
	void onNetwork(HttpRequest &request, HttpResponse &response);
	void onSecurity(HttpRequest &request, HttpResponse &response);
	void onUpdate(HttpRequest &request, HttpResponse &response);
	void onAbout(HttpRequest &request, HttpResponse &response);

	/* Метод для отправки файла */
	void onFile(HttpRequest &request, HttpResponse &response);

public:
	WebServer();
};

#endif /* WEBSERVER_H_ */
