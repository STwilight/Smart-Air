/*
 * Thermo.h
 *
 * ������������ ���� ������ Thermo, ������������ ���������, ��� ��������� ����������.
 *
 */

#ifndef APP_THERMO_H_
#define APP_THERMO_H_

#include <Libraries/OneWire/OneWire.h>


class Thermo {
private:
	byte addr[8];		/* ����� ������� �����������, 8 ���� */
	byte type_s;		/* ��� ������� ����������� */

	byte sens_res;		/* ���������� ������� �����������:
	 	 	 	 	 	 * 		12 ��� (750 ��,   0.0625): RES = 0x7F
	 	 	 	 	 	 * 		11 ��� (375 ��,   0.125):  RES = 0x5F
	 	 	 	 	 	 * 		10 ��� (187.5 ��, 0.25):   RES = 0x3F
	 	 	 	 	 	 *  	 9 ��� (93.75 ��, 0.5):    RES = 0x1F
	 	 	 	 	 	 */

	bool sensor_error;	/* ���� ������ ������ ������� � �������� */
	OneWire *sensor;	/* ������ �� ������ ��� ���� 1-Wire */

	/* ������ ��� ��������� ������� */
	void searchSensor();
	void setResolution();


public:
	/* ����������� ��-��������� */
	Thermo(byte TempSensorPin, byte SensorResolution);

	/* ����� ��������� ����������� */
	float getTemp();
};

#endif /* APP_THERMO_H_ */
