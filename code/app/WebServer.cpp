/*
 * WebServer.cpp
 *
 * Класс, описывающий WEB-сервер
 *
 */

#include "WebServer.h"

WebServer::WebServer() {
	/* Конструктор-обертка WEB-сервера по-умолчанию */

	webServer.listen(WEB_SERVER_PORT);

	webServer.addPath("/", onStatus);
	webServer.addPath("/ajax/status", onAjaxStatus);

	webServer.addPath("/config", onConfig);
	webServer.addPath("/network", onNetwork);
	webServer.addPath("/security", onSecurity);
	webServer.addPath("/update", onUpdate);
	webServer.addPath("/about", onAbout);

	webServer.setDefaultHandler(onFile);
}

void onFile(HttpRequest &request, HttpResponse &response)
{
	/* метод выдачи файла */
	String file = request.getPath();

	if(file[0] == '/')
		file = file.substring(1);

	if(file[0] == '.')
		response.forbidden();
	else
	{
		response.setCache(86400, true);
		response.sendFile(file);
	}
}
