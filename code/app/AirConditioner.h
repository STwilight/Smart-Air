/*
 * AirConditioner.h
 *
 * Заголовочный файл класса AirConditioner, описывающего кондиционер, как целостное устройство.
 *
 */

#ifndef APP_AIRCONDITIONER_H_
#define APP_AIRCONDITIONER_H_

#include "Settings.h"
#include "Application.h"
#include "Definitions.h"
#include "Thermometer.h"
#include <SmingCore/SmingCore.h>

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
class AirConditioner {
private:
	static bool power;		/* Питание прибора:
							 *		Включен		= true
							 *		Выключен	= false
							 */

	static bool mode;		/* Режим работы:
							 *		Обогрев		= true
							 *		Охлаждение	= false
							 */

	static byte speed;		/* Скорость вращения вентилятора:
							 *		Остановлен	= 0x00
							 *		Низкая		= 0x01
							 *		Средняя		= 0x02
							 *		Высокая		= 0x03
							 */

	static byte set_temp;	/* Заданная температура
						 	 *		– температура в пределах +15 .. +30 градусов Цельсия,
						 	 *		  при достижении которой происходит отключение вентиялтора.
						 	 */

	static byte delta_temp;	/* Отклонение температуры
							 *		– разница (в пределах 0 .. 5 градусов Цельсия) между заданной
							 *		  и действительной температурами, при достижении которой
							 *		  происходит включение вентилятора.
							 */

	static float cur_temp;	/* Текущая температура окружающей среды */

	/* Определение выводов контроллера для управления */
	static byte PowerPin;		/* Вывод управления питанием кондиционера */
	static byte LowSpeedPin;	/* Вывод включения вентилятора на низкой скорости */
	static byte MedSpeedPin;	/* Вывод включения вентилятора на средней скорости */
	static byte HiSpeedPin;		/* Вывод включения вентилятора на высокой скорости */

	/* Метод инициализации периферии GPIO */
	void initGPIO();

	/* Методы управления кондиционером */
	static void setPower(bool power);
	static void setSpeed(byte speed);

	/* Таймеры для отсчета времени */
	Timer executeConditioner;

	/* Интеграция датчиа температуры */
	static Thermometer *sensor;
	static void updateTemp();

	/* Реализация функционала внутреннего термостата */
	static void execThermostat();

	/* Основная функция, реализующая функционал кондиционера */
	static void execConditioner();

public:
	/* Конструктор по-умолчанию */
	AirConditioner(byte PowerPin, byte LowSpeedPin, byte MedSpeedPin, byte HiSpeedPin, byte SensorPin, byte SensorResolution);

	/* Метод для запуска кондиционера в работу */
	void start();

	/* Метод получения текущей температуры */
	float getTemp();

	/* Методы получения и сохранения конфигурации */
	String getSettings();
	void setSettings(String settings);

	/* Метод получения текущего состояния модуля */
	String getState();

	/* Метод, выполняющий остановку процессов внутри модуля */
	void stopModule();

	/* Метод, выполняющий сохранение конфигурации модуля в файл */
	void saveConfig();

	/* Метод, выполняющий подготовку модуля для перезагрузки системы */
	void onSystemRestart();
};

#endif /* APP_AIRCONDITIONER_H_ */
