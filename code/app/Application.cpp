
/* ����������� ������������ ������ Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* TEST ������� */
#include "Thermometer.h"
#include "AirConditioner.h"
/* TEST ������� */

void init()
{
	/* ������������ �������� ������� */
	spiffs_mount();

	/* TEST ������� */

		AirConditioner AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);
		String settings = AirConditioner.getSettings();

	/* TEST ������� */
}
