var output;
var messages = 0;

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
	websocket.send(JSON.stringify({type: 0x06}));
	websocket.send(JSON.stringify({type: 0x08}));
}
function onMessage(evt) {
	// Действие при получении информации в текстовом виде
	messages++;
	processJSON(evt.data);
	if(messages >= 2)
		doDisconnect();
}
function onError(evt) {
	// Действие при возникновении ошибки
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
					break;
			}
		}
	}
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	output.innerHTML += message;	
}

window.addEventListener("load", init, false);