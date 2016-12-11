var output;

function init() {
	// Основной метод
	
	output = document.getElementById("output");
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
	
	writeToScreen("CONNECTED");
}
function onMessage(evt) {
	// Действие при получении информации в текстовом виде
	
	writeToScreen('<span style="color: blue;">RESPONSE: ' + evt.data+'</span>');
}
function onError(evt) {
	// Действие при возникновении ошибки
	
	writeToScreen('<span style="color: red;">ERROR:</span> ' + evt.data);
}
function onClose(evt) {
	// Действие при разъединении socket-соединения
	
	writeToScreen("DISCONNECTED");
}

function doSend(message) {
	// Метод отправки информации в текстовом виде
	
	// writeToScreen("SENT: " + message); 
	websocket.send(message);
}
function doDisconnect() {
	// Метод разрыва socket-соединения
	
	var disconnect = document.getElementById("disconnect");
	disconnect.disabled = true;
	websocket.close();
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	
	var pre = document.createElement("p");
	pre.style.wordWrap = "break-word";
	pre.innerHTML = message;
	output.appendChild(pre);
}

window.addEventListener("load", init, false);
