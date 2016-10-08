/*
 * Thermo.cpp
 *
 * �����, ����������� ��������� �� ������ IC DS18B20+.
 *
 */

#include "Thermo.h"

Thermo::Thermo(byte TempSensorPin, byte SensorResolution) {
	/* ����������� ��-���������.
	 * �� ���� ����������� ����� ������,
	 * � �������� ��������� ������ �����������. */

	this->addr[8]		= {0};
	this->type_s		= 0x00;
	this->sens_res		= SensorResolution;
	this->sensor_error	= false;

	this->sensor = new OneWire(TempSensorPin);
	this->sensor->begin();

	this->searchSensor();
}

void Thermo::searchSensor() {
	/* ����� � ����������� ������� �� ���� 1-Wire */

	/* ������� ������� ������ ������ ������� */
	byte scan_attempt = 0;

	/* ����� ��� �������� � ������, ���� ROM ������� ������ ������� */
	rescan:

	/* �������� ������� ������ ����� 1-Wire */
	this->sensor->reset_search();

	/* ����� ��������� �� ���� 1-Wire */
	while(!this->sensor->search(this->addr))
	{
		if(scan_attempt >= 10) {
			this->sensor_error = true;
			break;
		}
		this->sensor->reset_search();
		scan_attempt++;
		WDT.alive();
		delay(500);
	}

	/* �������� ������������ ������ ������ ������� */
	if(OneWire::crc8(this->addr, 7) != this->addr[7])
	{
		if(!this->sensor_error) {
			scan_attempt = 0;
			goto rescan;
		}
	}
	else
	{
		if(!this->sensor_error)
		{
			/* ����������� ���� ������� */
			switch (this->addr[0])
			{
				case 0x10:
					this->type_s = 1;
					break;
				case 0x28:
					this->type_s = 0;
					break;
				case 0x22:
					this->type_s = 0;
					break;
				default:
					break;
			}
			this->setResolution();
		}
	}
}
void Thermo::setResolution() {
	/* ����� ���������� ������� */

	WDT.enable(false);

	this->sensor->reset();
	this->sensor->select(this->addr);
	this->sensor->write(0x4E);
	this->sensor->write(0x00);
	this->sensor->write(0x00);
	this->sensor->write(this->sens_res);
	this->sensor->reset();

    WDT.alive();
}

float Thermo::getTemp() {
	/* ��������� ����������� (� �������� �������) �� ������� */

	float temp = 0;
	byte data[12];

	if(!this->sensor_error)
	{
		/* �������� ������� ������� �������������� (���������� �������) */
		this->sensor->reset();
		this->sensor->select(this->addr);
		this->sensor->write(0x44, 1);

		/* ����� ��� ���������� �������� ��������� */
		WDT.alive();
		delay(450);

		/* �������� ������� ������ ����������� � ������� */
		this->sensor->reset();
		this->sensor->select(this->addr);
		this->sensor->write(0xBE);

		/* ������ ������ � ����������� */
		for (byte i = 0; i < 9; i++)
			data[i] = this->sensor->read();

		/* ����������� ����������� */
		int16_t raw = (data[1] << 8) | data[0];
		if(this->type_s)
		{
			raw = raw << 3;
			if(data[7] == 0x10)
				raw = (raw & 0xFFF0) + 12 - data[6];
		}
		else
		{
			byte cfg = (data[4] & 0x60);
			if(cfg == 0x00) raw = raw & ~7;  		// ���������� � 9 ���,  93.75 ms
			else if(cfg == 0x20) raw = raw & ~3; 	// ���������� � 10 ���, 187.5 ms
			else if(cfg == 0x40) raw = raw & ~1; 	// ���������� � 11 ���, 375 ms
													// ���������� � 12 ���, 750 ms (��-���������)
		}

		/* �������� �������������� */
		temp = (float)raw / 16.0;
	}

	return temp;
}
