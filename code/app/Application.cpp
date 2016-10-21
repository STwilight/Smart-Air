
/* Определение типа используемого модуля */
#define ESP07

/* Подключение заголовочных файлов Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* Подключение заголовочного файла для модуля управления кондиционером */
#include "AirConditioner.h"

void init()
{
	/* Монтирование файловой системы */
	// spiffs_mount();

	AirConditioner AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);
	// String settings = AirConditioner.getSettings();
}
