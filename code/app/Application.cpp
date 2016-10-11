
/* Подключение заголовочных файлов Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* TEST УЧАСТОК */
#include "Thermometer.h"
#include "AirConditioner.h"
/* TEST УЧАСТОК */

void init()
{
	/* Монтирование файловой системы */
	spiffs_mount();

	/* TEST УЧАСТОК */

		AirConditioner AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);
		String settings = AirConditioner.getSettings();

	/* TEST УЧАСТОК */
}
