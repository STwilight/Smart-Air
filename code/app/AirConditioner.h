/*
 * AirConditioner.h
 *
 * ������������ ���� ������ AirConditioner, ������������ �����������, ��� ��������� ����������.
 *
 */

#ifndef APP_AirConditioner_H_
#define APP_AirConditioner_H_

#include "Definitions.h"
#include "Thermometer.h"
#include <SmingCore/SmingCore.h>

/* ����������� ��������� ������� ������������ */
#define PowerOn			true
#define PowerOff		false

/* ����������� ������ ������ ��� ������������ */
#define Heating			true
#define Cooling			false

/* ����������� �������� �������� �������� ����������� */
#define Stopped			0x00
#define LowSpeed		0x01
#define MedSpeed		0x02
#define	HiSpeed			0x03

/* ����������� ��������� �������� �������� ����������� */
#define set_temp_min	15
#define set_temp_max	30
#define set_temp_def	20

/* ����������� ��������� �������� ���������� ����������� */
#define delta_temp_min	0
#define delta_temp_max	5
#define delta_temp_def	3

/* �������� ������ "�����������", ��� ��������� ���������� */
class AirConditioner {
private:
	static bool power;		/* ������� �������:
							 *		�������		= true
							 *		��������	= false
							 */

	static bool mode;		/* ����� ������:
							 *		�������		= true
							 *		����������	= false
							 */

	static byte speed;		/* �������� �������� �����������:
							 *		����������	= 0x00
							 *		������		= 0x01
							 *		�������		= 0x02
							 *		�������		= 0x03
							 */

	static byte set_temp;	/* �������� �����������
						 	 *		� ����������� � �������� +15 .. +30 �������� �������,
						 	 *		  ��� ���������� ������� ���������� ���������� �����������.
						 	 */

	static byte delta_temp;	/* ���������� �����������
							 *		� ������� (� �������� 0 .. 5 �������� �������) ����� ��������
							 *		  � �������������� �������������, ��� ���������� �������
							 *		  ���������� ��������� �����������.
							 */

	static float cur_temp;	/* ������� ����������� ���������� ����� */

	/* ����������� ������� ����������� ��� ���������� */
	byte PowerPin;				/* ����� ���������� �������� ������������ */
	static byte LowSpeedPin;	/* ����� ��������� ����������� �� ������ �������� */
	static byte MedSpeedPin;	/* ����� ��������� ����������� �� ������� �������� */
	static byte HiSpeedPin;		/* ����� ��������� ����������� �� ������� �������� */

	/* ������ ���������� ������������� */
	void setPower(bool power);
	void setMode(bool mode);
	static void setSpeed(byte speed);
	void setSetTemp(byte temp);
	void setDeltaTemp(byte temp);

	/* ����� ���������� ������������ � ���������� ����� */
	void applySettings();

	/* ������� ��� ������� ������� */
	Timer executeConditioner;

	/* ���������� ������ ����������� */
	static Thermometer *sensor;
	static void getTemp();

	/* ���������� ����������� ����������� ���������� */
	static void execThermostat();

	/* �������� �������, ����������� ���������� ������������ */
	static void execConditioner();

public:
	/* ����������� ��-��������� */
	AirConditioner(byte PowerPin, byte LowSpeedPin, byte MedSpeedPin, byte HiSpeedPin, byte SensorPin, byte SensorResolution);

	/* ������ ��������� � ���������� ������������ */
	String getSettings();
	void setSettings(String settings);
};

#endif /* APP_AirConditioner_H_ */
