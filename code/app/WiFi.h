/*
 * WiFi.h
 *
 * Заголовочный файл класса, описывающего методы управления Wi-Fi модулем ESP8266
 *
 */

#ifndef APP_WIFI_H_
#define APP_WIFI_H_

#include "Config.h"
#include "Settings.h"
#include "Application.h"
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

		/* Методы перенастройки Wi-Fi модуля в зависимости от состояния подключения в режиме "Клиент Wi-Fi сети" */
	    static void wifiConnectOK();
		static void wifiConnectFail();

		/* Методы преобразования типа шифрования */
		AUTH_MODE convertStringToAuthMode(String data);
		String convertAuthModeToString(AUTH_MODE auth_mode);

		/* Метод преобразования MAC-адреса с разделением двоеточием */
		String convertMAC(String macAddress, bool fullMAC = true);

		/* Методы преобразования RAW MAC-адреса в серийный номер устройства */
		String convertHEX(String hexSN);
		String convertSN(String macAddress);

		/* Методы получения MAC-адресов точки доступа и клиента Wi-Fi модуля */
		String getAccessPointMAC(bool raw = false);
		String getStationMAC(bool raw = false);

		/* Метод получения серийного номера Wi-Fi модуля на основе MAC адреса его точки доступа */
		String getSN();

public:
	  /* Конструктор для режимов "Точка доступа Wi-Fi" и "Клиент Wi-Fi сети".
	   * Допускается использование нескольких режимов одновременно.
	   *
	   * Параметры по-умолчанию для режима "Точка доступа Wi-Fi":
	   * 		– SSID точки доступа содержит название проекта и 3 последних байта MAC-адреса;
	   * 		– пароль для подключения не задан;
	   * 		– шифрование отсутствует;
	   * 		– скрытие SSID отключено;
	   * 		– задан 6-й канал вещания;
	   * 		– IP-адрес точки доступа равен 10.0.0.1;
	   * 		– режим точки доступа отключен.
	   *
	   * Параметры по-умолчанию для режима "Точка доступа Wi-Fi по-умолчанию":
	   * 		– SSID точки доступа содержит название проекта и 3 последних байта MAC-адреса;
	   * 		– пароль для подключения не задан;
	   * 		– шифрование отсутствует;
	   * 		– задан 6-й канал вещания;
	   * 		– режим точки доступа по-умолчанию отключен.
	   *
	   * Параметры по-умолчанию для режима "Клиент Wi-Fi сети":
	   * 		– SSID для подключения не задан;
	   * 		– пароль для подключения не задан;
	   * 		– автоматическое подключение к сохраненной точке доступа;
	   * 		– тайм-аут подключения к точке доступа – 20 секунд;
	   * 		– флаг ошибки сброшен (нет ошибки подключения);
	   * 		– режим клиента включен.
	   *
	   */
		WiFi();

		/* Метод инициализации Wi-Fi модуля */
		void wifiInit();

		/* Методы получения и сохранения конфигурации */
		String getSettings();
		void setSettings(String settings);

		/* Метод применения конфигурации к аппаратной части */
		void applySettings();

		/* Метод получения текущего состояния модуля */
		String getState();

		/* Метод получения информации об аппаратном обеспечении */
		String getHardwareInfo();

		/* Метод получения имени устройства по-умолчанию (на основе MAC адреса его точки доступа ) */
		String getDefaulDeviceName();

		/* Метод подключения к точке доступа в режиме "Клиент Wi-Fi сети" */
		void wifiConnect(String st_wifi_ssid, String st_wifi_pwd, bool st_wifi_autoconnect, bool store_settings);

		/* Метод сканирования доступных точек доступа */
		String wifiScan();

		/* Метод, выполняющий подготовку Wi-Fi модуля для перезагрузки системы */
		void onSystemRestart();
};

#endif /* APP_WIFI_H_ */
