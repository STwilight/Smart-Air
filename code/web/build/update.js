var list = document.getElementById("file_restore");
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
	websocket.send(JSON.stringify({type: 0x09}));
	websocket.send(JSON.stringify({type: 0xB1}));
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
				default:
					if(document.getElementById(subkey) != null)
						document.getElementById(subkey).innerHTML = subval;
					else if(!isNaN(parseInt(subkey)) && isFinite(subkey)) {
						if((subval.length != 0) && (subval.endsWith(".backup"))) {
							list = document.getElementById("file_restore");
							var option = document.createElement("option");
							option.text = subval;
							list.add(option, list[subkey]);
						}
					}
					break;
			}
		}
	}
	if(list.length != 0)
		list.selectedIndex = 0;
}

function getRestoreParams(type) {
	// Получение параметров для восстановления конфигурации в формате JSON-строки
	var root = {};
	root["type"] = type;
	
	var res_file = {};
	res_file["file_name"] = document.getElementById("file_restore").value;
	
	root["res_file"] = res_file;
	return JSON.stringify(root);	
}

function onBackupButton() {
	// Метод обработки данных при нажатии на кнопку "Backup"
	list = document.getElementById("file_restore");
	while(list.length != 0)
		list.remove(0);
	websocket.send(JSON.stringify({type: 0xA0}));
	websocket.send(JSON.stringify({type: 0xB1}));
}
function onRestoreButton() {
	// Метод обработки данных при нажатии на кнопку "Restore"
	websocket.send(getRestoreParams(0xB0));
}
function onUpdateButton() {
	// Метод обработки данных при нажатии на кнопку "Update"
	
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	output.innerHTML += message;	
}

window.addEventListener("load", init, false);