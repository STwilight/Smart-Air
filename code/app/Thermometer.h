/*
 * Thermometer.h
 *
 * Заголовочный файл класса Thermometer, описывающего термометр, как целостное устройство.
 *
 */

#ifndef APP_Thermometer_H_
#define APP_Thermometer_H_

#include <Libraries/OneWire/OneWire.h>

class Thermometer {
private:
	byte addr[8];		/* Адрес датчика температуры, 8 байт */
	byte type_s;		/* Тип датчика температуры */

	byte sens_res;		/* Разрешение датчика температуры:
	 	 	 	 	 	 * 		12 бит (750 мс,   0.0625): RES = 0x7F
	 	 	 	 	 	 * 		11 бит (375 мс,   0.125):  RES = 0x5F
	 	 	 	 	 	 * 		10 бит (187.5 мс, 0.25):   RES = 0x3F
	 	 	 	 	 	 *  	 9 бит (93.75 мс, 0.5):    RES = 0x1F
	 	 	 	 	 	 */

	OneWire *sensor;	/* Ссылка на объект для шины 1-Wire */

	/* Методы для настройки датчика */
	void searchSensor();
	void setResolution(byte bitResolution);

public:
	bool sensor_error;	/* Флаг ошибки обмена данными с датчиком */

	/* Конструктор по-умолчанию */
	Thermometer(byte SensorPin, byte SensorResolution);

	/* Метод получения температуры */
	float getTemp();
};

#endif /* APP_Thermometer_H_ */
