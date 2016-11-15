/*
 * AirConditioner.cpp
 *
 * Класс, описывающий функции управления кондиционером.
 *
 * Логика работы:
 *
 * 		Кондиционер считается выключенным, если параметр "power" равен "PowerOff" (false).
 * 		Для включения возможности подачи питания на двигатель необходимо присвоить параметру "power" значение "PowerOn" (true).
 *
 * 		Режим функционирования (тепло/холод) определяется значением параметра "mode".
 * 		Если "mode" равен значению "Heating" (true), то кондиционер работает в режиме обогрева.
 * 		Если "mode" равен значению "Cooling" (false), то кондиционер работает в режиме охлаждения.
 *
 * 		Скорость вращения вентилятора кондиционера (параметр "speed") может принимать четыре значения:
 * 			– Stopped  (0x00) – вентилятор полностью остановлен;
 *			– LowSpeed (0x01) – низкая скорость вращения;
 * 			– MedSpeed (0x02) – средняя скорость вращения;
 * 			– HiSpeed  (0x03) – высокая скорость вращения.
 *		При условии полной остановки вентилятора, сигналы на всех управляющих выводах равны нулю.
 *		В других скоростных режимах один из выводов выдает сигнал единицы, в то время как сигнал на всех остальных равен нулю.
 *
 *		В режиме обогрева включение вентилятора кондиционера после подачи питания осуществляется при условии, что текущая температура
 *			(параметр "cur_temp"), полученная с датчика температуры (Thermometer), будет ниже, чем заданная температура включения
 *			(параметр "set_temp").
 *		Повторное включение вентилятора в режиме обогрева происходит при условии, что текущая температура
 *			(параметр "cur_temp"), полученная с датчика температуры (Thermometer), будет ниже, чем разность заданной температуры включения
 *			(параметр "set_temp") и допустимого отклонения температуры (параметр "delta_temp").
 *			Данный функционал реализован с целью экономии электроэнергии.
 *
 *		В режиме охлаждения включение вентилятора кондиционера после подачи питания осуществляется при условии, что текущая температура
 *			(параметр "cur_temp"), полученная с датчика температуры (Thermometer), будет выше, чем заданная температура включения
 *			(параметр "set_temp").
 *		Повторное включение вентилятора в режиме охлаждения происходит при условии, что текущая температура
 *			(параметр "cur_temp"), полученная с датчика температуры (Thermometer), будет выше, чем сумма заданной температуры включения
 *			(параметр "set_temp") и допустимого отклонения температуры (параметр "delta_temp").
 *			Данный функционал реализован с целью экономии электроэнергии.
 *
 *		В случае возникновения ошибки чтения с датчика (повреждения датчика температуры или его отсутствие), кондиционер автоматически
 *			выключит вентилятор (параметру "speed" будет присвоено значение "Stopped") и отключит свое питание ("power" будет равен "PowerOff").
 *
 */

#include "AirConditioner.h"

/* Декларирование static-переменных класса AirConditioner */
			bool AirConditioner::power;
			bool AirConditioner::mode;
			byte AirConditioner::speed;
			byte AirConditioner::set_temp;
			byte AirConditioner::delta_temp;
		   float AirConditioner::cur_temp;
		    byte AirConditioner::PowerPin;
			byte AirConditioner::LowSpeedPin;
			byte AirConditioner::MedSpeedPin;
			byte AirConditioner::HiSpeedPin;
	Thermometer* AirConditioner::sensor;

AirConditioner::AirConditioner(byte PowerPin, byte LowSpeedPin, byte MedSpeedPin, byte HiSpeedPin, byte SensorPin, byte SensorResolution) {
	/* Конструктор по-умолчанию
	 * На вход принимается следующие парамтеры:
	 * 		– номер вывода для управления питанием;
	 * 		– номер вывода для выбора малой скорости вентилятора;
	 * 		– номер вывода для выбора средней скорости вентилятора;
	 * 		– номер вывода для выбора высокой скорости вентилятора;
	 * 		– номер вывода для подключения датчика температуры;
	 * 		– разрешение датчика температуры;
	 */

	this->PowerPin		= PowerPin;
	this->LowSpeedPin	= LowSpeedPin;
	this->MedSpeedPin	= MedSpeedPin;
	this->HiSpeedPin	= HiSpeedPin;

	this->initGPIO();

	this->sensor		= new Thermometer(SensorPin, SensorResolution);

	this->power			= PowerOff;
	this->mode			= Cooling;
	this->speed			= Stopped;
	this->set_temp		= set_temp_def;
	this->delta_temp	= delta_temp_def;
	this->cur_temp		= 0;

	AirConditioner::executeConditioner.initializeMs(1000, AirConditioner::execConditioner).start();
}

void AirConditioner::initGPIO() {
	pinMode(this->PowerPin, OUTPUT);
	noPullup(this->PowerPin);
	digitalWrite(this->PowerPin, Off);

	pinMode(this->LowSpeedPin, OUTPUT);
	noPullup(this->LowSpeedPin);
	digitalWrite(this->LowSpeedPin, Off);

	pinMode(this->MedSpeedPin, OUTPUT);
	noPullup(this->MedSpeedPin);
	digitalWrite(this->MedSpeedPin, Off);

	pinMode(this->HiSpeedPin, OUTPUT);
	noPullup(this->HiSpeedPin);
	digitalWrite(this->HiSpeedPin, Off);
}

void AirConditioner::setPower(bool power) {
	/* Применение параметра "Питание" */

	digitalWrite(AirConditioner::PowerPin, power);
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
	/* Применение параметра "Заданная температура" */

	this->set_temp = temp;
}
void AirConditioner::setDeltaTemp(byte temp) {
	/* Применение параметра "Отклонение температуры" */

	this->delta_temp = temp;
}

void AirConditioner::updateTemp() {
	/* Получение температуры с датчика */

	AirConditioner::cur_temp = AirConditioner::sensor->getTemp();
}
void AirConditioner::execThermostat() {
	/* Осуществление поддержания постоянной температуры (функционал термостат) */

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
	else if(AirConditioner::power == PowerOn && AirConditioner::sensor->sensor_error) {
		AirConditioner::setSpeed(Stopped);
		AirConditioner::setPower(Off);
	}
}
void AirConditioner::execConditioner() {
	/* Основной исполняемый метод, реализующий функционал кондиционера */

	AirConditioner::updateTemp();
	AirConditioner::execThermostat();
}

float AirConditioner::getTemp() {
	/* Получение текущей температурыа */

	return AirConditioner::cur_temp;
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
	/* Сохранение конфигурации кондиционера из формата JSON-строки */

	if(jsonString.length() != 0) {
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
}
void AirConditioner::applySettings() {
	/* Применение конфигурации кондиционера к аппаратной части */

	this->setPower(this->power);
	this->setMode(this->mode);
	this->setSpeed(this->speed);
	this->setSetTemp(this->set_temp);
	this->setDeltaTemp(this->delta_temp);
}

void AirConditioner::onSystemRestart() {
	/* Метод, выполняющий подготовку модуля кондиционера для перезагрузки системы */

	if(this->executeConditioner.isStarted())
		this->executeConditioner.stop();

	this->setSpeed(Stopped);
	this->setPower(PowerOff);

	Settings.save(this->getSettings(), APP_SETTINGS);
}
