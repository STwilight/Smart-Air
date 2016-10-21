
/* Определение типа используемого модуля */
#define ESP01

/* Подключение заголовочных файлов Sming */
#include "Definitions.h"
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* Подключение заголовочного файла для модуля управления кондиционером */
#include "AirConditioner.h"

/* DEBUG */ Timer repeater;
/* DEBUG */ AirConditioner *aircond;

void repeatAction() {
	/* DEBUG */ Serial.println(aircond->getSettings());
}

void init()
{
	/* Монтирование файловой системы */
	// spiffs_mount();

	/* Инициализация UART */
	/* DEBUG */ Serial.systemDebugOutput(false);
	/* DEBUG */ aircond = new AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);
	/* DEBUG */ Serial.begin(115200);
	/* DEBUG */ repeater.initializeMs(1000, repeatAction).start(true);

	/* Создание объекта типа "Кондиционер" */
	// AirConditioner AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);

	/* Вывод настроек кондиционера в UART */
	/* DEBUG */
}
