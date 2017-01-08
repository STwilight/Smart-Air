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

	this->webServer.addPath("/", this->onWebPage);
	this->webServer.addPath("/config", this->onWebPage);
	this->webServer.addPath("/network", this->onWebPage);
	this->webServer.addPath("/services", this->onWebPage);
	this->webServer.addPath("/security", this->onWebPage);
	this->webServer.addPath("/update", this->onWebPage);
	this->webServer.addPath("/about", this->onWebPage);

	this->webServer.enableWebSockets(true);
	this->webServer.setWebSocketConnectionHandler(this->webSocketConnected);
	this->webServer.setWebSocketMessageHandler(this->webSocketMessageReceived);
	this->webServer.setWebSocketBinaryHandler(this->webSocketBinaryReceived);
	this->webServer.setWebSocketDisconnectionHandler(this->webSocketDisconnected);

	this->webServer.setDefaultHandler(this->onFile);

	this->webServer.listen(port);
}

void WebServer::onWebPage(HttpRequest &request, HttpResponse &response) {
	/* Метод выдачи содержимого страниц */

	String path = request.getPath();
	if(path[0] == '/')
		path = path.substring(1);

	if(path.length() == 0) {
		path = "status";
	}
	path.concat(".html");

	if(!WebServer::fileNotExist(path, false, false)) {
		TemplateFileStream *tmpl = new TemplateFileStream(path);
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
	else
		response.notFound();
}

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

	if(message.length() != 0) {
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(message);
		byte type = root["type"];
		socket.sendString(processData(type, message));
	}
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
		response.setCache(86400, true);
		response.sendFile(file);
	}
}

bool WebServer::fileNotExist(String name, bool createfile, bool deletezerofile) {
	/* Метод проверки существования файла и его создания/удаления в случае его отсутствия/нулевого размера */

	if(!fileExist(name)) {
		if(createfile)
			fileSetContent(name, "404 Not Found");
		return true;
	}
	else {
		if(fileGetSize(name) == 0) {
			if(deletezerofile)
				fileDelete(name);
			return true;
		}
		else
			return false;
	}
}

void WebServer::onBackup(HttpRequest &request, HttpResponse &response) {
	/* Метод для выдачи файла с текущей конфигурацией устройства */

	// TODO: Реализовать метод для выдачи файла с текущей конфигурацией устройства
}
void WebServer::onRestore(HttpRequest &request, HttpResponse &response) {
	/* Метод для получения файла с конфигурацией устройства */

	// TODO: Реализовать метод для получения файла с конфигурацией устройства
}
