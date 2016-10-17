/*
 * AirConditioner.cpp
 *
 * �����, ����������� ������� ���������� �������������.
 *
 */

#include "AirConditioner.h"

/* �������������� static-���������� ������ AirConditioner */
			bool AirConditioner::power;
			bool AirConditioner::mode;
			byte AirConditioner::speed;
			byte AirConditioner::set_temp;
			byte AirConditioner::delta_temp;
		   float AirConditioner::cur_temp;
			byte AirConditioner::LowSpeedPin;
			byte AirConditioner::MedSpeedPin;
			byte AirConditioner::HiSpeedPin;
	Thermometer* AirConditioner::sensor;

AirConditioner::AirConditioner(byte PowerPin, byte LowSpeedPin, byte MedSpeedPin, byte HiSpeedPin, byte SensorPin, byte SensorResolution) {
	/* ����������� ��-���������
	 * �� ���� ����������� ��������� ���������:
	 * 		� ������ ������ ��� ���������� ��������;
	 * 		� ����� ������ ��� ������ ����� �������� �����������;
	 * 		� ����� ������ ��� ������ ������� �������� �����������;
	 * 		� ����� ������ ��� ������ ������� �������� �����������;
	 * 		� ����� ������ ��� ����������� ������� �����������;
	 * 		� ���������� ������� �����������;
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

	AirConditioner::executeConditioner.initializeMs(1000, AirConditioner::execConditioner).start();
}

void AirConditioner::setPower(bool power) {
	/* ���������� ��������� "�������" */

	digitalWrite(PowerPin, power);
}
void AirConditioner::setMode(bool mode) {
	/* ���������� ��������� "����� ������" */

	if(mode)
		this->mode = Heating;
	else
		this->mode = Cooling;
}
void AirConditioner::setSpeed(byte speed) {
	/* ���������� ��������� "�������� �������� �����������" */

	switch (speed) {
		case 0x01:
			digitalWrite(AirConditioner::LowSpeedPin, On);
			digitalWrite(AirConditioner::MedSpeedPin, Off);
			digitalWrite(AirConditioner::HiSpeedPin,  Off);
			break;
		case 0x02:
			digitalWrite(AirConditioner::LowSpeedPin, Off);
			digitalWrite(AirConditioner::MedSpeedPin, On);
			digitalWrite(AirConditioner::HiSpeedPin,  Off);
			break;
		case 0x03:
			digitalWrite(AirConditioner::LowSpeedPin, Off);
			digitalWrite(AirConditioner::MedSpeedPin, Off);
			digitalWrite(AirConditioner::HiSpeedPin,  On);
			break;
		default:
			digitalWrite(AirConditioner::LowSpeedPin, Off);
			digitalWrite(AirConditioner::MedSpeedPin, Off);
			digitalWrite(AirConditioner::HiSpeedPin,  Off);
			break;
	}
}
void AirConditioner::setSetTemp(byte temp) {
	/* ���������� ��������� "�������� �����������" */

	this->set_temp = temp;
}
void AirConditioner::setDeltaTemp(byte temp) {
	/* ���������� ��������� "���������� �����������" */

	this->delta_temp = temp;
}

void AirConditioner::getTemp() {
	/* ��������� ����������� � ������� */

	AirConditioner::cur_temp = AirConditioner::sensor->getTemp();
}
void AirConditioner::execThermostat() {
	/* ������������� ����������� ���������� ����������� (���������� ���������) */

	if(AirConditioner::power == PowerOn && !AirConditioner::sensor->sensor_error) {
		if(AirConditioner::mode == Cooling){
			if(AirConditioner::cur_temp >= AirConditioner::set_temp + AirConditioner::delta_temp)
				AirConditioner::setSpeed(AirConditioner::speed);
			else
				AirConditioner::setSpeed(Stopped);
		}
		if(AirConditioner::mode == Heating) {
			if(AirConditioner::cur_temp <= AirConditioner::set_temp - AirConditioner::delta_temp)
				AirConditioner::setSpeed(AirConditioner::speed);
			else
				AirConditioner::setSpeed(Stopped);
		}
	}
}
void AirConditioner::execConditioner() {
	/* �������� ����������� �����, ����������� ���������� ������������ */

	AirConditioner::getTemp();
	AirConditioner::execThermostat();
}

String AirConditioner::getSettings() {
	/* ��������� ������������ ������������ � ������� JSON-������ */

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
	/* ���������� ������������ ������������ �� ������� JSON-������ � �� ���������� */

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
	/* ���������� ������������ ������������ � ���������� ����� */

	this->setPower(this->power);
	this->setMode(this->mode);
	this->setSpeed(this->speed);
	this->setSetTemp(this->set_temp);
	this->setDeltaTemp(this->delta_temp);
}
