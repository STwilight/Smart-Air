
/* ����������� ���� ������������� ������ */
#define ESP07

/* ����������� ������������ ������ Sming */
#include <user_config.h>
#include <SmingCore/SmingCore.h>

/* ����������� ������������� ����� ��� ������ ���������� ������������� */
#include "AirConditioner.h"

void init()
{
	/* ������������ �������� ������� */
	// spiffs_mount();

	AirConditioner AirConditioner(GPIO16, GPIO14, GPIO12, GPIO13, GPIO4, 11);
	// String settings = AirConditioner.getSettings();
}
