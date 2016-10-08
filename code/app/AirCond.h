/*
 * AirCond.h
 *
 * ������������ ���� ������ AirCond, ������������ �����������, ��� ��������� ����������.
 *
 */

#ifndef APP_AIRCOND_H_
#define APP_AIRCOND_H_

#include "Definitions.h"
#include <SmingCore/SmingCore.h>


/* ����������� ������� ����������� ��� ���������� */
// TODO: �������� ������ GPIO �� ��������������� ������!
#define PowerPin		0
#define LowSpeedPin		0
#define MedSpeedPin		0
#define	HiSpeedPin		0

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
class AirCond {
private:
	bool power;			/* ������� �������:
						 *		�������		= true
						 *		��������	= false
						 */

	bool mode;			/* ����� ������:
						 *		�������		= true
						 *		����������	= false
						 */

	byte speed;			/* �������� �������� �����������:
						 *		����������	= 0x00
						 *		������		= 0x01
						 *		�������		= 0x02
						 *		�������		= 0x03
						 */

	byte set_temp;		/* �������� �����������
						 *		� ����������� � �������� +15 .. +30 �������� �������,
						 *		  ��� ���������� ������� ���������� ���������� �����������.
						 */

	byte delta_temp;	/* ���������� �����������
					 	 *		� ������� (� �������� 0 .. 5 �������� �������) ����� ��������
					 	 *		  � �������������� �������������, ��� ���������� �������
					 	 *		  ���������� ��������� �����������.
					 	 */

	/* ������ ���������� ������������� */
	void setPower(bool power);
	void setMode(bool mode);
	void setSpeed(byte speed);
	void setSetTemp(byte temp);
	void setDeltaTemp(byte temp);

	/* ����� ���������� ������������ � ���������� ����� */
	void applySettings();

	// TODO: ������� �����-������� ��� ������� �����������.
	// TODO: ������������� ������ ��� ������ � �������� ������� �����������.


public:
	/* ����������� ��-��������� */
	AirCond();

	/* ������ ��������� � ���������� ������������ */
	String getSettings();
	void setSettings(String settings);
};

#endif /* APP_AIRCOND_H_ */
