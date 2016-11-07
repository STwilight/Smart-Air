
/* Подключение заголовочных файлов Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* Подключение конфигурационных файлов проекта */
#include "Config.h"
#include "Boards.h"

/* Подключение заголовочного файла хранилища настроек */
#include "Settings.h"

/* Подключение заголовочных файлов модулей */
#include "AirConditioner.h"
#include "WiFi.h"

/* DEBUG */ // Timer repeater;
AirConditioner *aircond;
WiFi *wifi;

/* DEBUG */ /*
void repeatAction() {
	Serial.println(aircond->getSettings());
}
*/ /* DEBUG */

void init()
{
	/* Инициализация UART */
	/* DEBUG */ // Serial.systemDebugOutput(true);
	/* DEBUG */ // Serial.begin(115200);

	/* Установка рабочей частоты процессора на 160 МГц */
	System.setCpuFrequency(eCF_160MHz);

	/* Монтирование файловой системы */
	spiffs_mount();

	/* Инициализация модуля кондиционера */
	aircond = new AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);
	aircond->setSettings(Settings.load(APP_SETTINGS));
	aircond->applySettings();

	/* Инициализация Wi-Fi модуля */
	wifi = new WiFi();
	wifi->setSettings(Settings.load(WIFI_SETTINGS));
	wifi->applySettings();

	/* DEBUG */ // repeater.initializeMs(1000, repeatAction).start(true);
};
