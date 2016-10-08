/*
 * AirCond.h
 *
 * Заголовочный файл класса AirCond, описывающего кондиционер, как целостное устройство.
 *
 */

#ifndef APP_AIRCOND_H_
#define APP_AIRCOND_H_

#include "Definitions.h"
#include <SmingCore/SmingCore.h>


/* Определение выводов контроллера для управления */
// TODO: Изменить номера GPIO на соответствующие железу!
#define PowerPin		0
#define LowSpeedPin		0
#define MedSpeedPin		0
#define	HiSpeedPin		0

/* Определение состояния питания кондиционера */
#define PowerOn			true
#define PowerOff		false

/* Определение режима работы для кондиционера */
#define Heating			true
#define Cooling			false

/* Определение значений скорости вращения вентилятора */
#define Stopped			0x00
#define LowSpeed		0x01
#define MedSpeed		0x02
#define	HiSpeed			0x03

/* Определение диапазона значений заданной температуры */
#define set_temp_min	15
#define set_temp_max	30
#define set_temp_def	20

/* Определение диапазона значений отклонения температуры */
#define delta_temp_min	0
#define delta_temp_max	5
#define delta_temp_def	3


/* Описание класса "кондиционер", как конечного устройства */
class AirCond {
private:
	bool power;			/* Питание прибора:
						 *		Включен		= true
						 *		Выключен	= false
						 */

	bool mode;			/* Режим работы:
						 *		Обогрев		= true
						 *		Охлаждение	= false
						 */

	byte speed;			/* Скорость вращения вентилятора:
						 *		Остановлен	= 0x00
						 *		Низкая		= 0x01
						 *		Средняя		= 0x02
						 *		Высокая		= 0x03
						 */

	byte set_temp;		/* Заданная температура
						 *		– температура в пределах +15 .. +30 градусов Цельсия,
						 *		  при достижении которой происходит отключение вентиялтора.
						 */

	byte delta_temp;	/* Отклонение температуры
					 	 *		– разница (в пределах 0 .. 5 градусов Цельсия) между заданной
					 	 *		  и действительной температурами, при достижении которой
					 	 *		  происходит включение вентилятора.
					 	 */

	/* Методы управления кондиционером */
	void setPower(bool power);
	void setMode(bool mode);
	void setSpeed(byte speed);
	void setSetTemp(byte temp);
	void setDeltaTemp(byte temp);

	/* Метод применения конфигурации к аппаратной части */
	void applySettings();

	// TODO: Создать класс-обертку для датчика температуры.
	// TODO: Интегрировать методы для работы с оберткой датчика температуры.


public:
	/* Конструктор по-умолчанию */
	AirCond();

	/* Методы получения и сохранения конфигурации */
	String getSettings();
	void setSettings(String settings);
};

#endif /* APP_AIRCOND_H_ */
