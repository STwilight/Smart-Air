/*
 * WiFi.h
 *
 * Заголовочный файл класса, описывающего методы управления Wi-Fi модулем ESP8266
 *
 */

#ifndef APP_WIFI_H_
#define APP_WIFI_H_

#include "Config.h"
#include "Definitions.h"
#include <SmingCore/SmingCore.h>

class WiFi {
private:
	/* Определение значений для режима "Точка доступа Wi-Fi" */
	   String ap_wifi_ssid;
	   String ap_wifi_pwd;
	AUTH_MODE ap_wifi_auth_mode;
		 bool ap_wifi_hidden;
		 byte ap_wifi_channel;
	   String ap_wifi_ip_address;
  static bool ap_wifi_state;

	/* Определение значений для режима "Точка доступа Wi-Fi по-умолчанию" */
	   static String ap_wifi_def_ssid;
	   static String ap_wifi_def_pwd;
	static AUTH_MODE ap_wifi_def_auth_mode;
	     static byte ap_wifi_def_channel;
	     static bool ap_wifi_def_state;

	/* Определение значений для режима "Клиент Wi-Fi сети" */
	   String st_wifi_ssid;
	   String st_wifi_pwd;
	   	 bool st_wifi_autoconnect;
	   	 byte st_wifi_conn_timeout;
  static bool st_wifi_err;
	     bool st_wifi_state;

private:
		/* Методы перенастройки Wi-Fi модуля в зависимости от состояния подключения в режиме "Клиент Wi-Fi сети" */
	    static void wifiConnectOK();
		static void wifiConnectFail();

public:
	  /* Конструктор для режимов "Точка доступа Wi-Fi" и "Клиент Wi-Fi сети".
	   * Режим зависит от переданных в конструктор параметров.
	   * Допускается тспользование нескольких режимов одновременно.
	   * По-умолчанию оба режима отключены.
	   *
	   * Параметры по-умолчанию для режима "Точка доступа Wi-Fi":
	   * 		– SSID точки доступа не задан
	   * 		– пароль для подключения не задан;
	   * 		– шифрование отсутствует;
	   * 		– скрытие SSID отключено;
	   * 		– задан 6-й канал вещания;
	   * 		– IP-адрес точки доступа равен 10.0.0.1;
	   * 		– режим точки доступа отключен.
	   *
	   * Параметры по-умолчанию для режима "Точка доступа Wi-Fi по-умолчанию":
	   * 		– пароль для подключения не задан;
	   * 		– шифрование отсутствует;
	   * 		– режим точки доступа по-умолчанию включен.
	   *
	   * Параметры по-умолчанию для режима "Клиент Wi-Fi сети":
	   * 		– SSID для подключения не задан;
	   * 		– пароль для подключения не задан;
	   * 		– автоматическое подключение к сохраненной точке доступа;
	   * 		– тайм-аут подключения к точке доступа – 20 секунд;
	   * 		– флаг ошибки сброшен (нет ошибки подключения);
	   * 		– режим клиента отключен.
	   *
	   */
	  WiFi(String ap_wifi_ssid, String ap_wifi_pwd = "", AUTH_MODE ap_wifi_auth_mode = AUTH_OPEN, bool ap_wifi_hidden = false, byte ap_wifi_channel = 6, String ap_wifi_ip_address = "10.0.0.1", bool ap_wifi_state = Off, String ap_wifi_def_pwd = "", AUTH_MODE ap_wifi_def_auth_mode = AUTH_OPEN, bool ap_wifi_def_state = Off, String st_wifi_ssid = "", String st_wifi_pwd = "", bool st_wifi_autoconnect = true, byte st_wifi_conn_timeout = 20, bool st_wifi_err = false, bool st_wifi_state = On);

	  /* Метод подключения к точке доступа в режиме "Клиент Wi-Fi сети" */
	  void wifiConnect(String st_wifi_ssid, String st_wifi_pwd);

	  /* Методы инициализации и переинициализации Wi-Fi модуля */
	  void wifiInit();
	  void wifiReInit();
};

#endif /* APP_WIFI_H_ */
