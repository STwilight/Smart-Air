/*
 * WiFi.cpp
 *
 * Класс, описывающий методы управления Wi-Fi модулем ESP8266
 *
 */

#include "WiFi.h"
#include "Functions.h"

/* Декларирование static-переменных класса WiFi */
	   bool WiFi::ap_wifi_state;
	 String WiFi::ap_wifi_def_ssid;
	 String WiFi::ap_wifi_def_pwd;
  AUTH_MODE WiFi::ap_wifi_def_auth_mode;
  	   byte WiFi::ap_wifi_def_channel;
	   bool WiFi::ap_wifi_def_state;
	   bool WiFi::st_wifi_err;

WiFi::WiFi() {
	/* Конструктор по-умолчанию */

	String def_ssid = PROJECT_NAME;
	def_ssid.concat(" [");
	def_ssid.concat(convertMAC(WifiAccessPoint.getMAC(), false));
	def_ssid.concat("]");

	this->ap_wifi_ssid			= def_ssid;
	this->ap_wifi_pwd			= "";
	this->ap_wifi_auth_mode		= AUTH_OPEN;
	this->ap_wifi_hidden		= false;
	this->ap_wifi_channel		= 6;
	this->ap_wifi_ip_address	= "10.0.0.1";
	this->ap_wifi_state			= Off;

	this->ap_wifi_def_ssid		= def_ssid;
	this->ap_wifi_def_pwd		= "";
	this->ap_wifi_def_auth_mode = AUTH_OPEN;
	this->ap_wifi_def_channel	= 6;
	this->ap_wifi_def_state		= Off;

	this->st_wifi_ssid			= "";
	this->st_wifi_pwd			= "";
	this->st_wifi_autoconnect	= true;
	this->st_wifi_conn_timeout  = 20;
	this->st_wifi_err			= false;
	this->st_wifi_state			= On;

	this->wifiInit();
}

void WiFi::wifiInit() {
	/* Метод инициализации Wi-Fi модуля */

	/* Настройка режима "Клиент Wi-Fi сети" */
	WifiStation.enable(this->st_wifi_state);
	this->wifiConnect(this->st_wifi_ssid, this->st_wifi_pwd, this->st_wifi_autoconnect, false);

	/* Настройка режима "Точка доступа Wi-Fi" */
	WifiAccessPoint.setIP(IPAddress(this->ap_wifi_ip_address));
	WifiAccessPoint.enable(WiFi::ap_wifi_state);
	if(WiFi::ap_wifi_state) {
		WifiAccessPoint.config(this->ap_wifi_ssid, this->ap_wifi_pwd, this->ap_wifi_auth_mode, this->ap_wifi_hidden, this->ap_wifi_channel);
	}
}

void WiFi::wifiConnectOK() {
	/* Метод, выполняющийся при успешном подключении к Wi-Fi точке доступа */

	WiFi::st_wifi_err = false;

	if(WifiAccessPoint.isEnabled() & !WiFi::ap_wifi_state) {
		WifiAccessPoint.enable(Off);
		WiFi::ap_wifi_def_state = false;
	}
}
void WiFi::wifiConnectFail() {
	/* Метод, выполняющийся при отсутствии/ошибке подключения к Wi-Fi точке доступа */

	WiFi::st_wifi_err = true;

	if(WifiStation.isEnabled())
		WifiStation.enable(Off);

	if(!WifiAccessPoint.isEnabled()) {
		WifiAccessPoint.enable(On);
		WifiAccessPoint.config(WiFi::ap_wifi_def_ssid, WiFi::ap_wifi_def_pwd, WiFi::ap_wifi_def_auth_mode, false, WiFi::ap_wifi_def_channel);
		WiFi::ap_wifi_def_state = true;
	}
}
void WiFi::wifiConnect(String st_wifi_ssid, String st_wifi_pwd, bool st_wifi_autoconnect, bool store_settings) {
	/* Метод подключения к точке доступа с указанными параметрами */

	if(store_settings) {
		this->st_wifi_ssid = st_wifi_ssid;
		this->st_wifi_pwd = st_wifi_pwd;
		this->st_wifi_autoconnect = st_wifi_autoconnect;
	}

	if(this->st_wifi_state) {
		if(!WifiStation.isEnabled())
			WifiStation.enable(On);
		WifiStation.config(st_wifi_ssid, st_wifi_pwd, st_wifi_autoconnect);
		WifiStation.waitConnection(WiFi::wifiConnectOK, WiFi::st_wifi_conn_timeout, WiFi::wifiConnectFail);
	}
}

String WiFi::getSettings() {
	/* Получение конфигурации Wi-Fi модуля в формате JSON-строки */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

				   JsonObject& ap_mode = jsonBuffer.createObject();
					   root["ap_mode"] = ap_mode;

			   ap_mode["ap_wifi_ssid"] = this->ap_wifi_ssid;
			    ap_mode["ap_wifi_pwd"] = this->ap_wifi_pwd;
		  ap_mode["ap_wifi_auth_mode"] = convertAuthModeToString(this->ap_wifi_auth_mode);
		     ap_mode["ap_wifi_hidden"] = (bool) this->ap_wifi_hidden;
		    ap_mode["ap_wifi_channel"] = (byte) this->ap_wifi_channel;
	     ap_mode["ap_wifi_ip_address"] = this->ap_wifi_ip_address;
			  ap_mode["ap_wifi_state"] = (bool) WiFi::ap_wifi_state;

			   JsonObject& ap_def_mode = jsonBuffer.createObject();
		 	 	   root["ap_def_mode"] = ap_def_mode;

		ap_def_mode["ap_wifi_def_pwd"] = WiFi::ap_wifi_def_pwd;
  ap_def_mode["ap_wifi_def_auth_mode"] = convertAuthModeToString(WiFi::ap_wifi_def_auth_mode);
  	  ap_def_mode["ap_wifi_def_state"] = (bool) WiFi::ap_wifi_def_state;

  	  	  	   JsonObject& client_mode = jsonBuffer.createObject();
	   	      	   root["client_mode"] = client_mode;

	 	   client_mode["st_wifi_ssid"] = this->st_wifi_ssid;
		    client_mode["st_wifi_pwd"] = this->st_wifi_pwd;
	client_mode["st_wifi_autoconnect"] = (bool) this->st_wifi_autoconnect;
   client_mode["st_wifi_conn_timeout"] = (byte) this->st_wifi_conn_timeout;
			client_mode["st_wifi_err"] = (bool) WiFi::st_wifi_err;
		  client_mode["st_wifi_state"] = (bool) this->st_wifi_state;

	String jsonString;
	root.printTo(jsonString);

	return jsonString;
}
void WiFi::setSettings(String jsonString) {
	/* Сохранение конфигурации Wi-Fi модуля из формата JSON-строки */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(jsonString);

		 JsonObject& ap_mode = root["ap_mode"];

		  this->ap_wifi_ssid = ap_mode["ap_wifi_ssid"].asString();
		   this->ap_wifi_pwd = ap_mode["ap_wifi_pwd"].asString();
	 this->ap_wifi_auth_mode = convertStringToAuthMode(ap_mode["ap_wifi_auth_mode"].asString());
		this->ap_wifi_hidden = ap_mode["ap_wifi_hidden"];
	   this->ap_wifi_channel = ap_mode["ap_wifi_channel"];
	this->ap_wifi_ip_address = ap_mode["ap_wifi_ip_address"].asString();
		 WiFi::ap_wifi_state = ap_mode["ap_wifi_state"];

	 JsonObject& ap_def_mode = root["ap_def_mode"];

	   WiFi::ap_wifi_def_pwd = ap_def_mode["ap_wifi_def_pwd"].asString();
 WiFi::ap_wifi_def_auth_mode = convertStringToAuthMode(ap_def_mode["ap_wifi_def_auth_mode"].asString());
	 WiFi::ap_wifi_def_state = ap_def_mode["ap_wifi_def_state"];

	 JsonObject& client_mode = root["client_mode"];

		  this->st_wifi_ssid = client_mode["st_wifi_ssid"].asString();
		   this->st_wifi_pwd = client_mode["st_wifi_pwd"].asString();
   this->st_wifi_autoconnect = client_mode["st_wifi_autoconnect"];
  this->st_wifi_conn_timeout = client_mode["st_wifi_conn_timeout"];
		   WiFi::st_wifi_err = client_mode["st_wifi_err"];
		 this->st_wifi_state = client_mode["st_wifi_state"];
}
void WiFi::applySettings() {
	/* Метод применения конфигурации к аппаратной части */

	/* Применение параметров для режима "Клиент Wi-Fi сети" */
	if((WifiStation.isEnabled() != this->st_wifi_state) or (WifiStation.getSSID() != this->st_wifi_ssid) or (WifiStation.getPassword() != this->st_wifi_pwd)) {
		WDT.alive();
		WifiStation.enable(this->st_wifi_state);
		if(WifiStation.isConnected())
			WifiStation.disconnect();
		this->wifiConnect(this->st_wifi_ssid, this->st_wifi_pwd, this->st_wifi_autoconnect, false);
	}

	/* Применение параметров для режима "Точка доступа Wi-Fi" */
	if(WifiAccessPoint.isEnabled() == WiFi::ap_wifi_state) {
		if(WiFi::ap_wifi_state) {
			WDT.alive();
			WifiAccessPoint.config(this->ap_wifi_ssid, this->ap_wifi_pwd, this->ap_wifi_auth_mode, this->ap_wifi_hidden, this->ap_wifi_channel);
		}
	}
	else {
		/* Необходимый IP адрес в режиме "Точка доступа Wi-Fi" назначается лишь в случае конфигурации при перезапуске системы.
		 * Временная мера – перезагрузка системы при изменении состояния точки доступа (вкл/выкл). */
		spiffs_unmount();
		System.restart();
	}
}
