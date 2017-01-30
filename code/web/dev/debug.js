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
	
	writeToScreen('<span style="color: orange;">' + getTime() + " <: " + "</span>" + "<b>" + evt.data + "</b>");
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
	
	writeToScreen('<span style="color: blue;">' + getTime() + " >: " + "</span>" + "<b>" + message + "</b>"); 
	websocket.send(JSON.stringify({type: message}));
}
function doDisconnect() {
	// Метод разрыва socket-соединения
	
	var disconnect = document.getElementById("disconnect");
	disconnect.disabled = true;
	websocket.close();
	writeToScreen("DISCONNECTED");
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	
	var pre = document.createElement("div");
	pre.style.wordWrap = "break-word";
	pre.innerHTML = message;
	output.appendChild(pre);
}
function getTime() {
	// Метод для получения текущей даты и времени
	
	var timestamp = new Date();
	return (("0" + timestamp.getDate()).slice(-2) + "." + ("0" + (timestamp.getMonth() + 1)).slice(-2) + "." + timestamp.getFullYear() + ", " + ("0" + timestamp.getHours()).slice(-2) + ":" + ("0" + timestamp.getMinutes()).slice(-2) + ":" + ("0" + timestamp.getSeconds()).slice(-2) + "." + ("00" + timestamp.getMilliseconds()).slice(-3));
}

window.addEventListener("load", init, false);
