/*
 * WebServer.cpp
 *
 * Класс, описывающий WEB-сервер
 *
 */

#include "WebServer.h"

/* Декларирование static-переменных класса WebServer */
	HttpServer WebServer::webServer;

WebServer::WebServer(uint16_t port) {
	/* Конструктор-обертка WEB-сервера по-умолчанию */

	this->webServer.addPath("/", this->onStatus);
	this->webServer.addPath("/ajax/status", this->onAjaxStatus);

	this->webServer.addPath("/config", this->onConfig);
	this->webServer.addPath("/network", this->onNetwork);
	this->webServer.addPath("/security", this->onSecurity);
	this->webServer.addPath("/update", this->onUpdate);
	this->webServer.addPath("/about", this->onAbout);

	/* DEBUG - WebSockets */
		this->webServer.addPath("/debug", this->onDebug);
		this->webServer.enableWebSockets(true);
		this->webServer.setWebSocketConnectionHandler(this->webSocketConnected);
		this->webServer.setWebSocketMessageHandler(this->webSocketMessageReceived);
		this->webServer.setWebSocketBinaryHandler(this->webSocketBinaryReceived);
		this->webServer.setWebSocketDisconnectionHandler(this->webSocketDisconnected);
	/* DEBUG - WebSockets */

	this->webServer.setDefaultHandler(this->onFile);

	this->webServer.listen(port);
}

/* DEBUG - WebSockets */
	void WebServer::onDebug(HttpRequest &request, HttpResponse &response) {
		/* Метод для выдачи страницы отладки */

		WebServer::fileNotExist("debug.html");
		TemplateFileStream *tmpl = new TemplateFileStream("debug.html");

		auto &vars = tmpl->variables();
		response.sendTemplate(tmpl);
	}

	// TODO: Завершить реализацию методов обработки событий WebSocket'ов
	void WebServer::webSocketConnected(WebSocket& socket) {
		/* Метод обработки события установления нового socket-подключения */

		/* DEBUG */ /*
			WebSocketsList &clients = WebServer::webServer.getActiveWebSockets();
			for(int i=0; i<clients.count(); i++)
				clients[i].sendString("Connected! Total connections: " + String(WebServer::webServer.getActiveWebSockets().count()));
		*/
	}
	void WebServer::webSocketMessageReceived(WebSocket& socket, const String& message) {
		/* Метод обработки полученной информации в текстовом формате */

		socket.sendString(getData(message.toInt()));
		/* DEBUG */ // String response = "Message received: " + message;
		/* DEBUG */ // socket.sendString(response);
	}
	void WebServer::webSocketBinaryReceived(WebSocket& socket, uint8_t* data, size_t size) {
		/* Метод обработки полученной информации в двоичном формате */

		/* DEBUG */ /*
			String response = "Binary data recieved, size: " + size;
			socket.sendString(response);
		*/
	}
	void WebServer::webSocketDisconnected(WebSocket& socket) {
		/* Метод обработки разрыва существующего socket-подключения */

		/* DEBUG */ /*
			WebSocketsList &clients = WebServer::webServer.getActiveWebSockets();
			for (int i=0; i<clients.count(); i++)
				clients[i].sendString("Disconnected! Total connections: " + String(WebServer::webServer.getActiveWebSockets().count()));
		*/
	}
/* DEBUG - WebSockets */

// TODO: Завершить реализацию методов выдачи страниц в соответствии с их специфическими требованиями
void WebServer::onStatus(HttpRequest &request, HttpResponse &response) {
	/* Метод для выдачи страницы мониторинга */

	WebServer::fileNotExist("status.html");
	TemplateFileStream *tmpl = new TemplateFileStream("status.html");

	auto &vars = tmpl->variables();

	vars["author_name"] = AUTHOR_NAME;
	vars["project_name"] = PROJECT_NAME;
	vars["device_name"] = device_name;
	vars["device_class"] = DEVICE_CLASS;
	vars["start_year"] = START_YEAR;
	vars["current_year"] = current_year;
	vars["company_name"] = COMPANY_NAME;

	response.sendTemplate(tmpl);
}
void WebServer::onAjaxStatus(HttpRequest &request, HttpResponse &response) {
	/* Метод для передачи значений для страницы мониторинга в режиме реального времени */

	// TODO: Реализовать метод для передачи значений для страницы мониторинга в режиме реального времени
}

void WebServer::onConfig(HttpRequest &request, HttpResponse &response) {
	/* Метод для выдачи страницы настройки системы */

	WebServer::fileNotExist("config.html");
	TemplateFileStream *tmpl = new TemplateFileStream("config.html");

	auto &vars = tmpl->variables();
	response.sendTemplate(tmpl);
}
void WebServer::onNetwork(HttpRequest &request, HttpResponse &response) {
	/* Метод для выдачи страницы настройки сетевых параметров */

	WebServer::fileNotExist("network.html");
	TemplateFileStream *tmpl = new TemplateFileStream("network.html");

	auto &vars = tmpl->variables();
	response.sendTemplate(tmpl);
}
void WebServer::onSecurity(HttpRequest &request, HttpResponse &response) {
	/* Метод для выдачи страницы настройки опций безопасности */

	WebServer::fileNotExist("security.html");
	TemplateFileStream *tmpl = new TemplateFileStream("security.html");

	auto &vars = tmpl->variables();
	response.sendTemplate(tmpl);
}
void WebServer::onUpdate(HttpRequest &request, HttpResponse &response) {
	/* Метод для выдачи страницы обновления системы */

	WebServer::fileNotExist("update.html");
	TemplateFileStream *tmpl = new TemplateFileStream("update.html");

	auto &vars = tmpl->variables();
	response.sendTemplate(tmpl);
}
void WebServer::onBackup(HttpRequest &request, HttpResponse &response) {
	/* Метод для выдачи файла с текущей конфигурацией устройства */

	// TODO: Реализовать метод для выдачи файла с текущей конфигурацией устройства
}
void WebServer::onRestore(HttpRequest &request, HttpResponse &response) {
	/* Метод для получения файла с конфигурацией устройства */

	// TODO: Реализовать метод для получения файла с конфигурацией устройства
}
void WebServer::onAbout(HttpRequest &request, HttpResponse &response) {
	/* Метод для выдачи страницы с информацией о системе */

	WebServer::fileNotExist("about.html");
	TemplateFileStream *tmpl = new TemplateFileStream("about.html");

	auto &vars = tmpl->variables();
	response.sendTemplate(tmpl);
}

void WebServer::onFile(HttpRequest &request, HttpResponse &response)
{
	/* Метод для отправки файла */

	String file = request.getPath();

	if(file[0] == '/')
		file = file.substring(1);

	if(file[0] == '.')
		response.forbidden();
	else
	{
		WebServer::fileNotExist(file, false);
		response.setCache(86400, true);
		response.sendFile(file);
	}
}

void WebServer::fileNotExist(String name, bool createfile) {
	/* Метод для создания файла в случае его отсутствия */

	if(!fileExist(name) && createfile)
		fileSetContent(name, "404 Not Found");
}
