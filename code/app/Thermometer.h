/*
 * Thermometer.h
 *
 * ������������ ���� ������ Thermometer, ������������ ���������, ��� ��������� ����������.
 *
 */

#ifndef APP_Thermometer_H_
#define APP_Thermometer_H_

#include <Libraries/OneWire/OneWire.h>

class Thermometer {
private:
	byte addr[8];		/* ����� ������� �����������, 8 ���� */
	byte type_s;		/* ��� ������� ����������� */

	byte sens_res;		/* ���������� ������� �����������:
	 	 	 	 	 	 * 		12 ��� (750 ��,   0.0625): RES = 0x7F
	 	 	 	 	 	 * 		11 ��� (375 ��,   0.125):  RES = 0x5F
	 	 	 	 	 	 * 		10 ��� (187.5 ��, 0.25):   RES = 0x3F
	 	 	 	 	 	 *  	 9 ��� (93.75 ��, 0.5):    RES = 0x1F
	 	 	 	 	 	 */

	OneWire *sensor;	/* ������ �� ������ ��� ���� 1-Wire */

	/* ������ ��� ��������� ������� */
	void searchSensor();
	void setResolution(byte bitResolution);

public:
	bool sensor_error;	/* ���� ������ ������ ������� � �������� */

	/* ����������� ��-��������� */
	Thermometer(byte TempSensorPin, byte SensorResolution);

	/* ����� ��������� ����������� */
	float getTemp();
};

#endif /* APP_Thermometer_H_ */
