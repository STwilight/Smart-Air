/*
 * WiFi.cpp
 *
 * Класс, описывающий методы управления Wi-Fi модулем ESP8266
 *
 */

#include "WiFi.h"

/* Декларирование static-переменных класса WiFi */
	   bool WiFi::ap_wifi_state;
	 String WiFi::ap_wifi_def_ssid;
	 String WiFi::ap_wifi_def_pwd;
  AUTH_MODE WiFi::ap_wifi_def_auth_mode;
	   bool WiFi::ap_wifi_def_state;
	   bool WiFi::st_wifi_err;

WiFi::WiFi(String ap_wifi_ssid, String ap_wifi_pwd, AUTH_MODE ap_wifi_auth_mode, bool ap_wifi_hidden, byte ap_wifi_channel, String ap_wifi_ip_address, bool ap_wifi_state, String ap_wifi_def_pwd, AUTH_MODE ap_wifi_def_auth_mode, bool ap_wifi_def_state, String st_wifi_ssid, String st_wifi_pwd, bool st_wifi_autoconnect, byte st_wifi_conn_timeout, bool st_wifi_err, bool st_wifi_state) {
	/* Конструктор по-умолчанию
	 * На вход принимается следующие парамтеры:
	 * 		*** Для режима "Точка доступа Wi-Fi": ***
	 * 			– SSID;
	 * 			– пароль;
	 * 			– метод шифрования;
	 * 			– флаг скрытия SSID;
	 * 			– номер канала вещания;
	 * 			– IP-адрес;
	 * 			– флаг разрешения включения режима.
	 *
	 *		*** Для режима "Точка доступа Wi-Fi по-умолчанию": ***
	 *			– пароль;
	 *			– метод шифрования;
	 *			– флаг разрешения включения режима.
	 *
	 *		*** Для режима "Клиент Wi-Fi сети": ***
	 * 			– SSID;
	 * 			– пароль;
	 * 			– флаг разрешения автоматического подключения;
	 * 			– таймаут подключения к сети в секундах;
	 * 			– флаг ошибки подключения;
	 * 			– флаг разрешения включения режима.
	 *
	 */

	this->ap_wifi_ssid			= ap_wifi_ssid;
	this->ap_wifi_pwd			= ap_wifi_pwd;
	this->ap_wifi_auth_mode		= ap_wifi_auth_mode;
	this->ap_wifi_hidden		= ap_wifi_hidden;
	this->ap_wifi_channel		= ap_wifi_channel;
	this->ap_wifi_ip_address	= ap_wifi_ip_address;
	this->ap_wifi_state			= ap_wifi_state;

	String def_ssid = PROJECT_NAME;
	def_ssid.concat(" [");
	def_ssid.concat(WifiAccessPoint.getMAC());
	def_ssid.concat("]");

	this->ap_wifi_def_ssid		= def_ssid;
	this->ap_wifi_def_pwd		= ap_wifi_def_pwd;
	this->ap_wifi_def_auth_mode = ap_wifi_def_auth_mode;
	this->ap_wifi_def_state		= ap_wifi_def_state;

	this->st_wifi_ssid			= st_wifi_ssid;
	this->st_wifi_pwd			= st_wifi_pwd;
	this->st_wifi_autoconnect	= st_wifi_autoconnect;
	this->st_wifi_conn_timeout  = st_wifi_conn_timeout;
	this->st_wifi_err			= st_wifi_err;
	this->st_wifi_state			= st_wifi_state;
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
		WifiAccessPoint.config(WiFi::ap_wifi_def_ssid, WiFi::ap_wifi_def_pwd, WiFi::ap_wifi_def_auth_mode);
		WiFi::ap_wifi_def_state = true;
	}
}

void WiFi::wifiConnect(String st_wifi_ssid, String st_wifi_pwd) {
	this->st_wifi_ssid = st_wifi_ssid;
	this->st_wifi_pwd  = st_wifi_pwd;

	if(!this->st_wifi_state) {
		if(!WifiStation.isEnabled())
			WifiStation.enable(On);
		WifiStation.config(this->st_wifi_ssid, this->st_wifi_pwd);
		WifiStation.waitConnection(WiFi::wifiConnectOK, WiFi::st_wifi_conn_timeout, WiFi::wifiConnectFail);
	}
}

void WiFi::wifiInit() {
	// TODO: wifiInit()
}
void WiFi::wifiReInit() {
	// TODO: wifiReInit()
}
