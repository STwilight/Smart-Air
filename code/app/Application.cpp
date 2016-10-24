
/* Подключение заголовочных файлов Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* Подключение конфигурационных файлов проекта */
#include "Config.h"
#include "Boards.h"

/* Подключение заголовочного файла для модуля управления кондиционером */
#include "AirConditioner.h"
#include "WiFi.h"

/* DEBUG */ Timer repeater;
/* DEBUG */ AirConditioner *aircond;

void repeatAction() {
	/* DEBUG */ Serial.println(aircond->getSettings());
}

void init()
{
	/* Инициализация UART */
	/* DEBUG */ Serial.systemDebugOutput(true);
	/* DEBUG */ Serial.begin(115200);

	/* Установка рабочей частоты процессора на 160 МГц */
	System.setCpuFrequency(eCF_160MHz);

	/* Монтирование файловой системы */
	// spiffs_mount();

	/* Инициализация Wi-Fi модуля */
	WiFi WiFi(PROJECT_NAME, "", AUTH_OPEN, false, 6, "10.0.0.1", Off, "", AUTH_OPEN, Off, "", "", true, 20, false, On);

	/* Инициализация кондиционера */
	AirConditioner AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);

	/* DEBUG */ // aircond = new AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);
	/* DEBUG */ // repeater.initializeMs(1000, repeatAction).start(true);
}
