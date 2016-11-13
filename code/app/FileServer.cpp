/*
 * FileServer.cpp
 *
 * Класс-обертка, описывающий FTP-сервер
 *
 */

#include "FileServer.h"

FileServer::FileServer(String name, String pass, uint16_t port) {
	/* Конструктор по-умолчанию */

	ftpServer.addUser(name, pass);
	ftpServer.listen(port);
}

