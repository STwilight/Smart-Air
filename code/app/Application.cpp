
/* ����������� ������������ ������ Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* TEST ������� */
#include "Thermo.h"
#include "AirCond.h"
/* TEST ������� */

void init()
{
	/* ������������ �������� ������� */
	spiffs_mount();

	/* TEST ������� */

		Thermo sensor(0, 0x5F);
		float temp = sensor.getTemp();

		AirCond airCond;
		String settings = airCond.getSettings();

	/* TEST ������� */
}
