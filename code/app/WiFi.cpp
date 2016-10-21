/*
 * WiFi.cpp
 *
 * Класс, описывающий методы управления Wi-Fi модулем ESP8266
 *
 */

#include "WiFi.h"

WiFi::WiFi() {
	this->ap_wifi_ssid = "Smart Air";
	this->ap_wifi_pwd  = "12345678";
	this->ap_wifi_mode = Off;

	this->ap_wifi_def_ssid = "Smart Air [Unconfigured]";
	this->ap_wifi_def_mode = false;

	this->st_wifi_ssid = "";
	this->st_wifi_pwd  = "";
	this->st_wifi_mode = On;
	this->st_wifi_err  = false;
}

