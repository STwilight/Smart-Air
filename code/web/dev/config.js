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
	websocket.send(0x00);
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
	$.each(data, function(key, val) {
		$.each(val, function(subkey, subval) {
			var target = $("#" + subkey);
			switch (subkey) {				
				case "power":
					if(subval) {
						document.getElementById("power-on").checked = true;
						document.getElementById("power-off").checked = false;
					}
					else {
						document.getElementById("power-on").checked = false;
						document.getElementById("power-off").checked = true;
					}
					break;
				case "mode":
					if(subval)
						document.getElementById(subkey).selectedIndex = 1;
					else
						document.getElementById(subkey).selectedIndex = 0;
					break;
				case "speed":
					document.getElementById(subkey).selectedIndex = subval;
					break;
				default:
					document.getElementById(subkey).value = subval;
					break;
			}
		});
	});
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	output.innerHTML += message;	
}

window.addEventListener("load", init, false);