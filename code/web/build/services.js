var ntp_request_interval;
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
	websocket.send(JSON.stringify({type: 0x02}));
	websocket.send(JSON.stringify({type: 0x03}));
	websocket.send(JSON.stringify({type: 0xE0}));
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
				case "ntp_timezone":
					var num = parseFloat(subval);
					var hours = Math.floor(num);
					// var mins = num - hours;
					document.getElementById(subkey).selectedIndex = (-1 * hours + 12);
					break;
				case "ftp_server_enabled":
					if(subval)
						document.getElementById("ftp-on").checked = true;
					else
						document.getElementById("ftp-off").checked = true;
					break;
				case "ntp_request_interval":
					ntp_request_interval = subval;
					break;
				case "cur_datetime":
					document.getElementById(subkey).innerHTML = subval;
					break;
				default:
					if(document.getElementById(subkey) != null)
						document.getElementById(subkey).value = subval;
					break;
			}
		}
	}
}

function getNTPSettings(type) {
	// Получение конфигурации NTP-сервера в формате JSON-строки
	var root = {};
	root["type"] = type;
	
	var ntp_client = {};
	ntp_client["ntp_server_address"] = document.getElementById("ntp_server_address").value;
	ntp_client["ntp_request_interval"] = ntp_request_interval;
	ntp_client["ntp_timezone"] = (((document.getElementById("ntp_timezone").selectedIndex - 12) * -1) + ".00");
	
	root["ntp_client"] = ntp_client;
	return JSON.stringify(root);	
}
function getFTPSettings(type) {
	// Получение конфигурации FTP-сервера в формате JSON-строки
	var root = {};
	root["type"] = type;
	
	var ftp_server = {};

	ftp_server["ftp_server_enabled"] = ((document.getElementById("ftp-on").checked && !document.getElementById("ftp-off").checked) ? true : false);
	ftp_server["ftp_server_name"] = document.getElementById("ftp_server_name").value;
	ftp_server["ftp_server_pass"] = document.getElementById("ftp_server_pass").value;
	
	root["ftp_server"] = ftp_server;
	return JSON.stringify(root);	
}

function onUpdateButton() {
	// Метод обработки данных при нажатии на кнопку "Update"
	websocket.send(JSON.stringify({type: 0xE0}));
}
function onSaveButton() {
	// Метод обработки данных при нажатии на кнопку "Save"
	websocket.send(getNTPSettings(0x12));
	websocket.send(getFTPSettings(0x13));
}
function onApplyButton() {
	// Метод обработки данных при нажатии на кнопку "Apply"
	websocket.send(getNTPSettings(0x22));
	websocket.send(getFTPSettings(0x23));
}
function onCancelButton() {
	// Метод обработки данных при нажатии на кнопку "Cancel"
	websocket.send(JSON.stringify({type: 0x02}));
	websocket.send(JSON.stringify({type: 0x03}));
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	output.innerHTML += message;	
}

window.addEventListener("load", init, false);