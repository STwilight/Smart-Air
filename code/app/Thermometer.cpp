/*
 * Thermometer.cpp
 *
 * Класс, описывающий термометр на основе IC DS18B20+.
 *
 */

#include "Thermometer.h"

Thermometer::Thermometer(byte SensorPin, byte SensorResolution) {
	/* Конструктор по-умолчанию.
	 *
	 * На вход принимается номер вывода,
	 * к которому подключен датчик температуры,
	 * и разрешение получаемых измерений (точность) в битах.
	 *
	 */

	this->addr[8]		= {0};
	this->type_s		= 0x00;
	this->sens_res		= 0x7F;
	this->sensor_error	= false;

	this->sensor = new OneWire(SensorPin);
	this->sensor->begin();

	this->searchSensor();
	this->setResolution(SensorResolution);
}

void Thermometer::searchSensor() {
	/* Поиск и обнаружение датчика на шине 1-Wire */

	/* Счетчик попыток чтения адреса датчика */
	byte scan_attempt = 0;

	/* Метка для возврата в случае, если ROM датчика принят неверно */
	rescan:

	/* Отправка команды сброса линии 1-Wire */
	this->sensor->reset_search();

	/* Поиск устройств на шине 1-Wire */
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

	/* Проверка правильности приема адреса датчика */
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
			/* Определение типа датчика */
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
		}
	}
}
void Thermometer::setResolution(byte SensorResolution) {
	/* Смена разрешения датчика */

	switch (SensorResolution) {
		case 9:
			this->sens_res = 0x1F;
			break;
		case 10:
			this->sens_res = 0x3F;
			break;
		case 11:
			this->sens_res = 0x5F;
			break;
		default:
			this->sens_res = 0x7F;
			break;
	}

	/* Разрешение датчика температуры:
	 * 		12 бит (750 мс,   0.0625): RES = 0x7F
	 * 		11 бит (375 мс,   0.125):  RES = 0x5F
	 * 		10 бит (187.5 мс, 0.25):   RES = 0x3F
	 *  	 9 бит (93.75 мс, 0.5):    RES = 0x1F
	 */

	if(!this->sensor_error) {
		WDT.alive();
		this->sensor->reset();
		this->sensor->select(this->addr);
		this->sensor->write(0x4E);
		this->sensor->write(0x00);
		this->sensor->write(0x00);
		this->sensor->write(this->sens_res);
		this->sensor->reset();
	}
}

float Thermometer::getTemp() {
	/* Получения температуры (в градусах Цельсия) от дачтика */

	float temp = 0;
	byte data[12];

	if(!this->sensor_error)
	{
		/* Отправка команды запуска преобразования (паразитное питание) */
		this->sensor->reset();
		this->sensor->select(this->addr);
		this->sensor->write(0x44, 1);

		/* Пауза для выполнения датчиком измерений */
		WDT.alive();
		delay(450);

		/* Отправка команды чтения температуры с датчика */
		this->sensor->reset();
		this->sensor->select(this->addr);
		this->sensor->write(0xBE);

		/* Чтение данных о температуре */
		for (byte i = 0; i < 9; i++)
			data[i] = this->sensor->read();

		/* Конвертация температуры */
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
			if(cfg == 0x00) raw = raw & ~7;  		// Разрешение в 9 бит,  93.75 ms
			else if(cfg == 0x20) raw = raw & ~3; 	// Разрешение в 10 бит, 187.5 ms
			else if(cfg == 0x40) raw = raw & ~1; 	// Разрешение в 11 бит, 375 ms
													// Разрешение в 12 бит, 750 ms (по-умолчанию)
		}

		/* Конечное преобразование */
		temp = (float)raw / 16.0;
	}

	return temp;
}
