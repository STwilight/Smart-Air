/*
 * WiFi.cpp
 *
 * Класс, описывающий методы управления Wi-Fi модулем ESP8266
 *
 */

// TODO: События Wi-Fi модуля на уровне Sming доступны через WifiEvents

#include "WiFi.h"

/* Декларирование static-переменных класса WiFi */
	   bool WiFi::ap_wifi_state;
	 String WiFi::ap_wifi_def_ssid;
	 String WiFi::ap_wifi_def_pwd;
  AUTH_MODE WiFi::ap_wifi_def_auth_mode;
  	   byte WiFi::ap_wifi_def_channel;
	   bool WiFi::ap_wifi_def_state;
	   bool WiFi::st_wifi_err;
	   BssList WiFi::networks;
	    String WiFi::json_networks_string;

WiFi::WiFi() {
	/* Конструктор по-умолчанию */

	default_device_name = PROJECT_NAME;
	default_device_name.concat(" [");
	default_device_name.concat(this->convertMAC(WifiAccessPoint.getMAC(), false));
	default_device_name.concat("]");

	this->ap_wifi_ssid			= default_device_name;
	this->ap_wifi_pwd			= DEFAULT_PASS;
	this->ap_wifi_auth_mode		= AUTH_OPEN;
	this->ap_wifi_hidden		= false;
	this->ap_wifi_channel		= 6;
	this->ap_wifi_ip_address	= "10.0.0.1";
	this->ap_wifi_state			= Off;

	this->ap_wifi_def_ssid		= default_device_name;
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

	this->wifi_sleep_type		= NONE_SLEEP_T;
}

void WiFi::wifiInit() {
	/* Метод инициализации Wi-Fi модуля */

	/* Установка режима сна для Wi-Fi модуля */
	wifi_set_sleep_type(this->wifi_sleep_type);

	/* Настройка режима "Точка доступа Wi-Fi" */
	WifiAccessPoint.setIP(IPAddress(this->ap_wifi_ip_address));
	WifiAccessPoint.enable(WiFi::ap_wifi_state);
	if(WiFi::ap_wifi_state) {
		WifiAccessPoint.config(this->ap_wifi_ssid, this->ap_wifi_pwd, this->ap_wifi_auth_mode, this->ap_wifi_hidden, this->ap_wifi_channel);
	}

	/* Настройка режима "Клиент Wi-Fi сети" */
	WifiStation.enable(On);
	this->scanWiFiNetworks(false);
	if(this->st_wifi_state)
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

	if(WifiStation.isEnabled()) {
		WifiStation.config("", "", false);
		WifiStation.disconnect();
	}
	else
		WifiStation.enable(On);

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

String WiFi::scanWiFiNetworks(bool makeJSON) {
	/* Метод сканирования доступных точек доступа */

	if(!WifiStation.isEnabled())
		WifiStation.enable(On);

	WifiStation.startScan(this->processScanningResults);

	if(makeJSON)
		return this->json_networks_string;
	else
		return "";
}
void WiFi::processScanningResults(bool succeeded, BssList list) {
	/* Метод обработки результатов сканирования доступных точек доступа */

	if(succeeded)
	{
		for(int i=0; i<list.count(); i++)
			if(!list[i].hidden && list[i].ssid.length()>0)
				WiFi::networks.add(list[i]);
	}
	list.clear();
	WiFi::networks.sort([](const BssInfo& a, const BssInfo& b) {
		return b.rssi - a.rssi;
	});
	WiFi::json_networks_string = WiFi::getWiFiNetworks();
	WiFi::networks.clear();
}
String WiFi::getWiFiNetworks() {
	/* Метод формирования JSON-строки со списком доступных точек доступа */

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& root = stream->getRoot();

	JsonArray& netlist = root.createNestedArray("networks");

	for(int i=0; i<networks.count(); i++)
	{
		if(networks[i].hidden)
			continue;
		else
		{
			  JsonObject &item = netlist.createNestedObject();
					item["id"] = (int)networks[i].getHashId();
				  item["ssid"] = networks[i].ssid;
				  item["rssi"] = networks[i].rssi;
			item["encryption"] = networks[i].getAuthorizationMethodName();
		}
	}

	String jsonString;
	root.printTo(jsonString);

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

		if(root.containsKey("ap_mode") && root.get("ap_mode").size() != 0) {
			JsonObject& ap_mode = root["ap_mode"];
			if(ap_mode.containsKey("ap_wifi_ssid"))
				this->ap_wifi_ssid = ap_mode["ap_wifi_ssid"].asString();
			if(ap_mode.containsKey("ap_wifi_pwd"))
				this->ap_wifi_pwd = ap_mode["ap_wifi_pwd"].asString();
			if(ap_mode.containsKey("ap_wifi_auth_mode"))
				this->ap_wifi_auth_mode = this->convertStringToAuthMode(ap_mode["ap_wifi_auth_mode"].asString());
			if(ap_mode.containsKey("ap_wifi_hidden"))
				this->ap_wifi_hidden = ap_mode["ap_wifi_hidden"];
			if(ap_mode.containsKey("ap_wifi_channel"))
				this->ap_wifi_channel = ap_mode["ap_wifi_channel"];
			if(ap_mode.containsKey("ap_wifi_ip_address"))
				this->ap_wifi_ip_address = ap_mode["ap_wifi_ip_address"].asString();
			if(ap_mode.containsKey("ap_wifi_state"))
				WiFi::ap_wifi_state = ap_mode["ap_wifi_state"];
		}
		if(root.containsKey("ap_def_mode") && root.get("ap_def_mode").size() != 0) {
			JsonObject& ap_def_mode = root["ap_def_mode"];
			if(ap_def_mode.containsKey("ap_wifi_def_pwd"))
				WiFi::ap_wifi_def_pwd = ap_def_mode["ap_wifi_def_pwd"].asString();
			if(ap_def_mode.containsKey("ap_wifi_def_auth_mode"))
				WiFi::ap_wifi_def_auth_mode = this->convertStringToAuthMode(ap_def_mode["ap_wifi_def_auth_mode"].asString());
			if(ap_def_mode.containsKey("ap_wifi_def_state"))
				WiFi::ap_wifi_def_state = ap_def_mode["ap_wifi_def_state"];
		}
		if(root.containsKey("client_mode") && root.get("client_mode").size() != 0) {
			JsonObject& client_mode = root["client_mode"];
			if(client_mode.containsKey("st_wifi_ssid"))
				this->st_wifi_ssid = client_mode["st_wifi_ssid"].asString();
			if(client_mode.containsKey("st_wifi_pwd"))
				this->st_wifi_pwd = client_mode["st_wifi_pwd"].asString();
			if(client_mode.containsKey("st_wifi_autoconnect"))
				this->st_wifi_autoconnect = client_mode["st_wifi_autoconnect"];
			if(client_mode.containsKey("st_wifi_conn_timeout"))
				this->st_wifi_conn_timeout = client_mode["st_wifi_conn_timeout"];
			if(client_mode.containsKey("st_wifi_err"))
				WiFi::st_wifi_err = client_mode["st_wifi_err"];
			if(client_mode.containsKey("st_wifi_state"))
				this->st_wifi_state = client_mode["st_wifi_state"];
		}
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

String WiFi::getHardwareInfo() {
	/* Метод получения информации об аппаратном обеспечении */

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

  JsonObject& dev_info = jsonBuffer.createObject();
	  root["dev_info"] = dev_info;

	dev_info["dev_sn"] = this->getSN();
	dev_info["ap_mac"] = this->getAccessPointMAC();
	dev_info["st_mac"] = this->getStationMAC();

	String jsonString;
	root.printTo(jsonString);

	return jsonString;
}
String WiFi::getSN() {
	/* Метод получения серийного номера Wi-Fi модуля на основе MAC адреса его точки доступа */

	return this->convertSN(this->getAccessPointMAC(true));
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

String WiFi::getDefaulDeviceName() {
	/* Метод получения имени устройства по-умолчанию (на основе MAC адреса его точки доступа ) */

	return this->default_device_name;
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

void WiFi::stopModule() {
	/* Метод, выполняющий остановку процессов внутри модуля */

	if(WifiStation.isConnected())
		WifiStation.disconnect();
	if(WifiStation.isEnabled())
		WifiStation.enable(false);
	if(WifiAccessPoint.isEnabled())
		WifiAccessPoint.enable(false);
}
void WiFi::saveConfig() {
	/* Метод, выполняющий сохранение конфигурации модуля в файл */

	Settings.save(this->getSettings(), WIFI_SETTINGS);
}
void WiFi::onSystemRestart() {
	/* Метод, выполняющий подготовку модуля для перезагрузки системы */

	this->stopModule();
	this->saveConfig();
}
