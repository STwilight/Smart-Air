/*
 * AirConditioner.cpp
 *
 * Класс, описывающий функции управления кондиционером.
 *
 */

#include "AirConditioner.h"

AirConditioner::AirConditioner(byte PowerPin, byte LowSpeedPin, byte MedSpeedPin, byte HiSpeedPin, byte SensorPin, byte SensorResolution) {
	/* Конструктор по-умолчанию
	 * На вход принимается следующие парамтеры:
	 * 		– номера вывода для управления питанием;
	 * 		– номер вывода для выбора малой скорости вентилятора;
	 * 		– номер вывода для выбора средней скорости вентилятора;
	 * 		– номер вывода для выбора высокой скорости вентилятора;
	 * 		– номер вывода для подключения датчика температуры;
	 * 		– разрешение датчика температуры;
	 */

	this->power			= PowerOff;
	this->mode			= Cooling;
	this->speed			= Stopped;
	this->set_temp		= set_temp_def;
	this->delta_temp	= delta_temp_def;
	this->cur_temp		= 0;

	this->PowerPin		= PowerPin;
	this->LowSpeedPin	= LowSpeedPin;
	this->MedSpeedPin	= MedSpeedPin;
	this->HiSpeedPin	= HiSpeedPin;

	this->sensor		= new Thermometer(SensorPin, SensorResolution);

	//this->getTemperature.initializeMs(1000, this->getTemp).start();
	//this->executeThermostat.initializeMs(60000, this->execThermostat).start();
}

void AirConditioner::setPower(bool power) {
	/* Применение параметра "Питание" */

	digitalWrite(PowerPin, power);
}
void AirConditioner::setMode(bool mode) {
	/* Применение параметра "Режим работы" */

	if(mode)
		this->mode = Heating;
	else
		this->mode = Cooling;
}
void AirConditioner::setSpeed(byte speed) {
	/* Применение параметра "Скорость вращения вентилятора" */

	switch (speed) {
		case 0x01:
			digitalWrite(LowSpeedPin, On);
			digitalWrite(MedSpeedPin, Off);
			digitalWrite(HiSpeedPin,  Off);
			break;
		case 0x02:
			digitalWrite(LowSpeedPin, Off);
			digitalWrite(MedSpeedPin, On);
			digitalWrite(HiSpeedPin,  Off);
			break;
		case 0x03:
			digitalWrite(LowSpeedPin, Off);
			digitalWrite(MedSpeedPin, Off);
			digitalWrite(HiSpeedPin,  On);
			break;
		default:
			digitalWrite(LowSpeedPin, Off);
			digitalWrite(MedSpeedPin, Off);
			digitalWrite(HiSpeedPin,  Off);
			break;
	}
}
void AirConditioner::setSetTemp(byte temp) {
	/* Применение параметра "Заданная температура" */

	this->set_temp = temp;
}
void AirConditioner::setDeltaTemp(byte temp) {
	/* Применение параметра "Отклонение температуры" */

	this->delta_temp = temp;
}

void AirConditioner::getTemp() {
	/* Получение температуры с датчика */

	this->cur_temp = this->sensor->getTemp();
}

void AirConditioner::execThermostat() {
	/* Осуществление поддержания постоянной температуры (термостат) */

	if(this->power == PowerOn && !this->sensor->sensor_error) {
		if(this->mode == Cooling){
			if(this->cur_temp >= this->set_temp + this->delta_temp)
				this->setSpeed(this->speed);
			else
				this->setSpeed(Stopped);
		}
		if(this->mode == Heating) {
			if(this->cur_temp <= this->set_temp - this->delta_temp)
				this->setSpeed(this->speed);
			else
				this->setSpeed(Stopped);
		}
	}
}

String AirConditioner::getSettings() {
	/* Получение конфигурации кондиционера в формате JSON-строки */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	JsonObject& settings 	= jsonBuffer.createObject();
	root["settings"]		= settings;
	settings["power"]		= (bool) this->power;
	settings["mode"]		= (bool) this->mode;
	settings["speed"]		= this->speed;
	settings["set_temp"]	= this->set_temp;
	settings["delta_temp"]	= this->delta_temp;

	JsonObject& info 		= jsonBuffer.createObject();
	root["info"] 			= info;
	info["cur_temp"]		= (float) this->cur_temp;
	info["err_sensor"]		= (bool) this->sensor->sensor_error;

	String jsonString;
	root.printTo(jsonString);

	return jsonString;
}
void AirConditioner::setSettings(String jsonString) {
	/* Сохранение конфигурации кондиционера из формата JSON-строки и ее применение */

	DynamicJsonBuffer jsonBuffer;

	JsonObject& root = jsonBuffer.parseObject(jsonString);
	JsonObject& settings = root["settings"];

	this->power = settings["power"];
	this->mode = settings["mode"];

	if((settings["speed"] >= Stopped) && (settings["speed"] <= HiSpeed))
		this->speed = settings["speed"];
	else
		this->speed = Stopped;

	if((settings["set_temp"] >= set_temp_min) && (settings["set_temp"] <= set_temp_max))
		this->set_temp = settings["set_temp"];
	else
		this->set_temp = set_temp_def;

	if((settings["delta_temp"] >= delta_temp_min) && (settings["delta_temp"] <= delta_temp_max))
		this->delta_temp = settings["delta_temp"];
	else
		this->delta_temp = delta_temp_def;
}
void AirConditioner::applySettings() {
	/* Применение конфигурации кондиционера к аппаратной части */

	this->setPower(this->power);
	this->setMode(this->mode);
	this->setSpeed(this->speed);
	this->setSetTemp(this->set_temp);
	this->setDeltaTemp(this->delta_temp);
}
