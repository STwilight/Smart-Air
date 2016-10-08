/*
 * AirCond.cpp
 *
 * Класс, описывающий функции управления кондиционером.
 *
 */

#include "AirCond.h"


AirCond::AirCond() {
	/* Конструктор по-умолчанию */

	this->power			= PowerOff;
	this->mode			= Cooling;
	this->speed			= Stopped;
	this->set_temp		= set_temp_def;
	this->delta_temp	= delta_temp_def;
}

void AirCond::setPower(bool power) {
	digitalWrite(PowerPin, power);
}
void AirCond::setMode(bool mode) {
	// TODO: Написать реализацию метода для управления режимами.
}
void AirCond::setSpeed(byte speed) {
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
void AirCond::setSetTemp(byte temp) {
	// TODO: Написать реализацию метода для установки целевой температуры.
}
void AirCond::setDeltaTemp(byte temp) {
	// TODO: Написать реализацию метода для установки отклонения температуры.
}

void AirCond::applySettings() {
	/* Применение конфигурации кондиционера к аппаратной части */

	this->setPower(this->power);
	this->setMode(this->mode);
	this->setSpeed(this->speed);
	this->setSetTemp(this->set_temp);
	this->setDeltaTemp(this->delta_temp);
}

// TODO: Описать методы для работы с оберткой датчика температуры.

String AirCond::getSettings() {
	/* Получение конфигурации кондиционера в формате JSON-строки */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	JsonObject& settings = jsonBuffer.createObject();
	root["settings"] = settings;

	settings["power"]		= this->power;
	settings["mode"]		= this->mode;
	settings["speed"]		= this->speed;
	settings["set_temp"]	= this->set_temp;
	settings["delta_temp"]	= this->delta_temp;

	String jsonString;
	root.printTo(jsonString);

	return jsonString;
}
void AirCond::setSettings(String jsonString) {
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

	this->applySettings();
}
