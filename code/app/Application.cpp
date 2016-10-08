
/* Подключение заголовочных файлов Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* TEST УЧАСТОК */
#include "Thermo.h"
#include "AirCond.h"
/* TEST УЧАСТОК */

void init()
{
	/* Монтирование файловой системы */
	spiffs_mount();

	/* TEST УЧАСТОК */

		Thermo sensor(0, 0x5F);
		float temp = sensor.getTemp();

		AirCond airCond;
		String settings = airCond.getSettings();

	/* TEST УЧАСТОК */
}
