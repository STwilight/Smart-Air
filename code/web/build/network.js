var output;

function init() {
	// Основной метод
	output = document.getElementById("messages");
	startWebSocket();
}

function startWebSocket() {
	// Инициализация socket-соединения
	var wsUri = "ws://" + location.host + "/";
	websocket = new WebSocket(wsUri);
	websocket.onopen	= function(evt) {onOpen(evt)};
	websocket.onclose   = function(evt) {onClose(evt)};
	websocket.onmessage = function(evt) {onMessage(evt)};
	websocket.onerror   = function(evt) {onError(evt)};
}

function onOpen(evt) {
	// Действие при установлении socket-соединения
	websocket.send(JSON.stringify({type: 0x04}));
	websocket.send(JSON.stringify({type: 0x08}));
}
function onMessage(evt) {
	// Действие при получении информации в текстовом виде
	processJSON(evt.data);
}
function onError(evt) {
	// Действие при возникновении ошибки
	writeToScreen('<div class="alert alert-danger" role="alert">Connection error!</div>');
}
function onClose(evt) {
	// Действие при разъединении socket-соединения
}

function doSend(message) {
	// Метод отправки информации в текстовом виде 
	websocket.send(message);
}
function doDisconnect() {
	// Метод разрыва socket-соединения
	websocket.close();
}

function processJSON(msg) {
	// Метод обработки информации, полученной из JSON
	var data = JSON.parse(msg);
	for (var key in data) {
		var val = data[key];
		for (var subkey in val) {
			var subval = val[subkey];
			switch (subkey) {
				case "ap_wifi_state":
					if(subval)
						document.getElementById("ap-wifi-on").checked = true;
					else
						document.getElementById("ap-wifi-off").checked = true;
					break;
				default:
					if(document.getElementById(subkey) != null)
						document.getElementById(subkey).value = subval;
					break;
			}
		}
	}
}

function getWiFiSettings(type) {
	// Получение конфигурации Wi-Fi модуля в формате JSON-строки
	var root = {};
	root["type"] = type;
	var ap_mode = {};
	ap_mode["ap_wifi_state"] = ((document.getElementById("ap-wifi-on").checked && !document.getElementById("ap-wifi-off").checked) ? true : false);
	ap_mode["ap_wifi_ssid"]  = document.getElementById("ap_wifi_ssid").value;
	ap_mode["ap_wifi_pwd"]   = document.getElementById("ap_wifi_pwd").value;
	if(document.getElementById("ap_wifi_pwd").value.length >= 8)
		ap_mode["ap_wifi_auth_mode"] = "AUTH_WPA2_PSK";
	else
		ap_mode["ap_wifi_auth_mode"] = "AUTH_OPEN";
			root["ap_mode"]  = ap_mode;
	var client_mode = {};
	client_mode["st_wifi_ssid"] = document.getElementById("st_wifi_ssid").value;
	client_mode["st_wifi_pwd"]  = document.getElementById("st_wifi_pwd").value;
			root["client_mode"] = client_mode;
	return JSON.stringify(root);	
}
function getDeviceNameSettings(type) {
	// Получение настройки имени устройства в формате JSON-строки
	var root = {};
	root["type"] = type;
	root["dev_name"] = document.getElementById("dev_name").value;
	return JSON.stringify(root);	
}

function onSaveButton() {
	// Метод обработки данных при нажатии на кнопку "Save"
	websocket.send(getWiFiSettings(0x14));
	websocket.send(getDeviceNameSettings(0x18));
}
function onApplyButton() {
	// Метод обработки данных при нажатии на кнопку "Apply"
	websocket.send(getWiFiSettings(0x24));
	websocket.send(getDeviceNameSettings(0x18));
}
function onCancelButton() {
	// Метод обработки данных при нажатии на кнопку "Cancel"
	websocket.send(JSON.stringify({type: 0x04}));
	websocket.send(JSON.stringify({type: 0x08}));
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	output.innerHTML += message;	
}

window.addEventListener("load", init, false);