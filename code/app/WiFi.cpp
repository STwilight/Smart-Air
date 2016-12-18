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
  	   byte WiFi::ap_wifi_def_channel;
	   bool WiFi::ap_wifi_def_state;
	   bool WiFi::st_wifi_err;

WiFi::WiFi() {
	/* Конструктор по-умолчанию */

	String def_ssid = PROJECT_NAME;
	def_ssid.concat(" [");
	def_ssid.concat(this->convertMAC(WifiAccessPoint.getMAC(), false));
	def_ssid.concat("]");

	this->ap_wifi_ssid			= def_ssid;
	this->ap_wifi_pwd			= DEFAULT_PASS;
	this->ap_wifi_auth_mode		= AUTH_OPEN;
	this->ap_wifi_hidden		= false;
	this->ap_wifi_channel		= 6;
	this->ap_wifi_ip_address	= "10.0.0.1";
	this->ap_wifi_state			= Off;

	this->ap_wifi_def_ssid		= def_ssid;
	this->ap_wifi_def_pwd		= DEFAULT_PASS;
	this->ap_wifi_def_auth_mode = AUTH_OPEN;
	this->ap_wifi_def_channel	= 6;
	this->ap_wifi_def_state		= Off;

	this->st_wifi_ssid			= WIFI_SSID;
	this->st_wifi_pwd			= WIFI_PWD;
	this->st_wifi_autoconnect	= true;
	this->st_wifi_conn_timeout  = 20;
	this->st_wifi_err			= false;
	this->st_wifi_state			= On;
}

void WiFi::wifiInit() {
	/* Метод инициализации Wi-Fi модуля */

	/* Настройка режима "Точка доступа Wi-Fi" */
	WifiAccessPoint.setIP(IPAddress(this->ap_wifi_ip_address));
	WifiAccessPoint.enable(WiFi::ap_wifi_state);
	if(WiFi::ap_wifi_state) {
		WifiAccessPoint.config(this->ap_wifi_ssid, this->ap_wifi_pwd, this->ap_wifi_auth_mode, this->ap_wifi_hidden, this->ap_wifi_channel);
	}

	/* Настройка режима "Клиент Wi-Fi сети" */
	WifiStation.enable(this->st_wifi_state);
	this->wifiConnect(this->st_wifi_ssid, this->st_wifi_pwd, this->st_wifi_autoconnect, false);
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

String WiFi::wifiScan() {
	/* Метод сканирования доступных точек доступа */

	// TODO: Завершить реализацию метода сканирования доступных точек доступа
	BssList networks;
	String jsonString;

	return jsonString;
}

String WiFi::getSettings() {
	/* Получение конфигурации Wi-Fi модуля в формате JSON-строки */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

				   JsonObject& ap_mode = jsonBuffer.createObject();
					   root["ap_mode"] = ap_mode;

			   ap_mode["ap_wifi_ssid"] = this->ap_wifi_ssid;
			    ap_mode["ap_wifi_pwd"] = this->ap_wifi_pwd;
		  ap_mode["ap_wifi_auth_mode"] = this->convertAuthModeToString(this->ap_wifi_auth_mode);
		     ap_mode["ap_wifi_hidden"] = (bool) this->ap_wifi_hidden;
		    ap_mode["ap_wifi_channel"] = (byte) this->ap_wifi_channel;
	     ap_mode["ap_wifi_ip_address"] = this->ap_wifi_ip_address;
			  ap_mode["ap_wifi_state"] = (bool) WiFi::ap_wifi_state;

			   JsonObject& ap_def_mode = jsonBuffer.createObject();
		 	 	   root["ap_def_mode"] = ap_def_mode;

		ap_def_mode["ap_wifi_def_pwd"] = WiFi::ap_wifi_def_pwd;
  ap_def_mode["ap_wifi_def_auth_mode"] = this->convertAuthModeToString(WiFi::ap_wifi_def_auth_mode);
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

	if(jsonString.length() != 0) {
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(jsonString);

			 JsonObject& ap_mode = root["ap_mode"];

			  this->ap_wifi_ssid = ap_mode["ap_wifi_ssid"].asString();
			   this->ap_wifi_pwd = ap_mode["ap_wifi_pwd"].asString();
		 this->ap_wifi_auth_mode = this->convertStringToAuthMode(ap_mode["ap_wifi_auth_mode"].asString());
			this->ap_wifi_hidden = ap_mode["ap_wifi_hidden"];
		   this->ap_wifi_channel = ap_mode["ap_wifi_channel"];
		this->ap_wifi_ip_address = ap_mode["ap_wifi_ip_address"].asString();
			 WiFi::ap_wifi_state = ap_mode["ap_wifi_state"];

		 JsonObject& ap_def_mode = root["ap_def_mode"];

		   WiFi::ap_wifi_def_pwd = ap_def_mode["ap_wifi_def_pwd"].asString();
	 WiFi::ap_wifi_def_auth_mode = this->convertStringToAuthMode(ap_def_mode["ap_wifi_def_auth_mode"].asString());
		 WiFi::ap_wifi_def_state = ap_def_mode["ap_wifi_def_state"];

		 JsonObject& client_mode = root["client_mode"];

			  this->st_wifi_ssid = client_mode["st_wifi_ssid"].asString();
			   this->st_wifi_pwd = client_mode["st_wifi_pwd"].asString();
	   this->st_wifi_autoconnect = client_mode["st_wifi_autoconnect"];
	  this->st_wifi_conn_timeout = client_mode["st_wifi_conn_timeout"];
			   WiFi::st_wifi_err = client_mode["st_wifi_err"];
			 this->st_wifi_state = client_mode["st_wifi_state"];
	}
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
		systemRestart();
	}
}

String WiFi::getState() {
	/* Метод получения текущего состояния модуля */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	String ap_wifi_ssid;
	if(ap_wifi_def_state)
		ap_wifi_ssid = ap_wifi_def_ssid;
	else
		ap_wifi_ssid = this->ap_wifi_ssid;

			 JsonObject& ap_wifi_status = jsonBuffer.createObject();
				 root["ap_wifi_status"] = ap_wifi_status;

	  ap_wifi_status["ap_wifi_enabled"]	= (bool) WifiAccessPoint.isEnabled();
	// функция 'WifiAccessPoint.getSSID()' не работает (не найдена)
		 ap_wifi_status["ap_wifi_ssid"] = ap_wifi_ssid;
		   ap_wifi_status["ap_wifi_ip"]	= WifiAccessPoint.getIP().toString();

			 JsonObject& st_wifi_status = jsonBuffer.createObject();
				 root["st_wifi_status"] = st_wifi_status;

	  st_wifi_status["st_wifi_enabled"]	= (bool) WifiStation.isEnabled();
		 st_wifi_status["st_wifi_ssid"]	= WifiStation.getSSID();
	st_wifi_status["st_wifi_connected"] = (bool) WifiStation.isConnected();
		   st_wifi_status["st_wifi_ip"]	= WifiStation.getIP().toString();
	// функция 'WifiStation.getRssi()' не работает (не найдена)

	String jsonString;
	root.printTo(jsonString);

	return jsonString;
}

AUTH_MODE WiFi::convertStringToAuthMode(String data) {
	/* Метод преобразования строки в тип шифрования точки доступа */

	AUTH_MODE auth_mode;

	if(data.equals("AUTH_OPEN"))
		auth_mode = AUTH_OPEN;
	else if(data.equals("AUTH_WEP"))
		auth_mode = AUTH_WEP;
	else if(data.equals("AUTH_WPA_PSK"))
		auth_mode = AUTH_WPA_PSK;
	else if(data.equals("AUTH_WPA2_PSK"))
		auth_mode = AUTH_WPA2_PSK;
	else if(data.equals("AUTH_WPA_WPA2_PSK"))
		auth_mode = AUTH_WPA_WPA2_PSK;
	else if(data.equals("AUTH_MAX"))
		auth_mode = AUTH_MAX;
	else
		auth_mode = AUTH_OPEN;

	return auth_mode;
}
String WiFi::convertAuthModeToString(AUTH_MODE auth_mode) {
	/* Метод преобразования типа шифрования точки доступа в строку */

	String data;

	switch (auth_mode) {
		case AUTH_OPEN:
			data = "AUTH_OPEN";
			break;
		case AUTH_WEP:
			data = "AUTH_WEP";
			break;
		case AUTH_WPA_PSK:
			data = "AUTH_WPA_PSK";
			break;
		case AUTH_WPA2_PSK:
			data = "AUTH_WPA2_PSK";
			break;
		case AUTH_WPA_WPA2_PSK:
			data = "AUTH_WPA_WPA2_PSK";
			break;
		case AUTH_MAX:
			data = "AUTH_MAX";
			break;
		default:
			data = "AUTH_OPEN";
			break;
	}

	return data;
}

String WiFi::getAccessPointMAC(bool raw) {
	/* Метод получения MAC-адреса точки доступа Wi-Fi модуля */

	String rawMAC = WifiAccessPoint.getMAC();

	if(raw)
		return rawMAC;
	else
		return convertMAC(rawMAC, true);
}
String WiFi::getStationMAC(bool raw) {
	/* Метод получения MAC-адреса клиента Wi-Fi модуля */

	String rawMAC = WifiStation.getMAC();

	if(raw)
		return rawMAC;
	else
		return convertMAC(rawMAC, true);
}
String WiFi::getSN() {
	/* Метод получения серийного номера Wi-Fi модуля на основе MAC адреса его точки доступа */

	return this->convertSN(this->getAccessPointMAC(true));
}

String WiFi::getDefaulDeviceName() {
	/* Метод получения имени устройства по-умолчанию (на основе MAC адреса его точки доступа ) */

	return this->ap_wifi_ssid;
}

String WiFi::convertMAC(String macAddress, bool fullMAC) {
	/* Метод для конвертации MAC адреса с разделением двоеточием.
	 *
	 * При значении входного параметра "fullMAC" равному "true",
	 * 		производится конвертация полного MAC-адреса,
	 * 		в противном случае конвертируются лишь три последних байта.
	 *
	 *  */

	String result = macAddress;
	result.toUpperCase();

	char macChar[macAddress.length() + 1];
	result.toCharArray(macChar, macAddress.length() + 1);

	result = "";
	byte size = sizeof(macChar) - 1;

	int startNUM = 0;
	if(!fullMAC)
		startNUM = 6;

	for(int i=startNUM; i<size; i++) {
		result += macChar[i];
		if((i%2 != 0) & (i != size-1))
			result += ":";
	}

	return result;
}
String WiFi::convertHEX(String hexSN)
{
	/* Метод конвертации HEX строки вида "1a2b3c" в DEC значение */

	const char symbols[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
							  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	String hexNumber = hexSN;
	byte hexLength = hexNumber.length();
	uint32_t decResult = 0;

	hexNumber.toUpperCase();
	for(int i=0; i<hexLength; i++)
		for(int j=0; j<sizeof(symbols); j++) {
			if(hexNumber.charAt(i) == symbols[j])
				decResult += j << 4*(hexLength-i-1);
	}

	return String(decResult);
}
String WiFi::convertSN(String macAddress)
{
	/* Метод конвертации 3 последних байт MAC-адреса в десятичный серийный номер */

	String tmp;

	char macChar[macAddress.length() + 1];
	macAddress.toCharArray(macChar, macAddress.length() + 1);

	byte size = sizeof(macChar) - 1;
	for(int i=6; i<size; i++)
		tmp += macChar[i];

	/* Необходимо использовать конвертацию из String в uint32_t для преобразования HEX > DEC
	 * 		посредством использования недоступной сейчас в Sming функции sscanf().
	 *
	 * Временное решение – самописный конвертер HEX-кода в DEC String значение.
	 * Функция для вызова: (String) convertHEX(String hexSN).
	 *
	 * Правильное решение:
	 * 		sscanf(tmp.c_str(), "%x", &result);
	 * 		return (long) result;
	 *
	 */

	return convertHEX(tmp);
}

void WiFi::onSystemRestart() {
	/* Метод, выполняющий подготовку Wi-Fi модуля для перезагрузки системы */

	if(WifiAccessPoint.isEnabled())
		WifiAccessPoint.enable(false);
	if(WifiStation.isConnected())
		WifiStation.disconnect();
	if(WifiStation.isEnabled())
		WifiStation.enable(false);

	Settings.save(this->getSettings(), WIFI_SETTINGS);
}
