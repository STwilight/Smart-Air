/*
 * FileServer.h
 *
 * Заголовочный файл к классу-обертке FTP-сервера
 *
 */

#ifndef APP_FILESERVER_H_
#define APP_FILESERVER_H_

#include <SmingCore/SmingCore.h>

class FileServer {
private:
	FTPServer ftpServer;

public:
	/* Конструктор по-умолчанию */
	FileServer(String name, String pass, uint16_t port);
};

#endif /* APP_FILESERVER_H_ */
