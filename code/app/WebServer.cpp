/*
 * WebServer.cpp
 *
 * Класс, описывающий WEB-сервер
 *
 */

#include "WebServer.h"

WebServer::WebServer(uint16_t port) {
	/* Конструктор-обертка WEB-сервера по-умолчанию */

	webServer.addPath("/", this->onStatus);
	webServer.addPath("/ajax/status", this->onAjaxStatus);

	webServer.addPath("/config", this->onConfig);
	webServer.addPath("/network", this->onNetwork);
	webServer.addPath("/security", this->onSecurity);
	webServer.addPath("/update", this->onUpdate);
	webServer.addPath("/about", this->onAbout);

	webServer.setDefaultHandler(this->onFile);

	webServer.listen(port);
}

// TODO: Завершить реализацию методов выдачи страниц в соответствии с их специфическими требованиями

void WebServer::onStatus(HttpRequest &request, HttpResponse &response) {
	/* Метод для выдачи страницы мониторинга */

	WebServer::fileNotExist("status.html");
	TemplateFileStream *tmpl = new TemplateFileStream("status.html");

	auto &vars = tmpl->variables();
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
		WebServer::fileNotExist(file);
		response.setCache(86400, true);
		response.sendFile(file);
	}
}

void WebServer::fileNotExist(String name) {
	/* Метод для создания файла в случае его отсутствия */

	if(!fileExist(name))
		fileSetContent(name, "404 Not Found");
}
