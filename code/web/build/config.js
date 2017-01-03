var output;

var set_temp_min;
var set_temp_max;
var delta_temp_min;
var delta_temp_max;

function init() {
	// Основной метод
	output = document.getElementById("messages");
	startWebSocket();
	processInput();
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
	for (var key in data) {
		var val = data[key];
		for (var subkey in val) {
			var subval = val[subkey];
			switch (subkey) {
				case "power":
					if(subval)
						document.getElementById("power-on").checked = true;
					else
						document.getElementById("power-off").checked = true;
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
				case "set_temp_min":
					set_temp_min = subval;
					break;
				case "set_temp_max":
					set_temp_max = subval;
					break;
				case "delta_temp_min":
					delta_temp_min = subval;
					break;
				case "delta_temp_max":
					delta_temp_max = subval;
					break;
				default:
					document.getElementById(subkey).value = subval;
					break;
			}
		}
	}
}

function processInput() {
	var num_inputs = [
		document.getElementById("set_temp"),
		document.getElementById("delta_temp")
	];
	
	var time_inputs = [
		document.getElementById("time_from"),
		document.getElementById("time_to")
	];

    for(var i=0; i<num_inputs.length; i++) {
        num_inputs[i].onkeypress = function(event) {
			event = event || window.event;
			if(event.charCode && (event.charCode < 48 || event.charCode > 57))
				return false;
		}
		num_inputs[i].onpaste = function(event) {
			return false;
		}
	}
	
    for(var i=0; i<time_inputs.length; i++) {
        time_inputs[i].onkeypress = function(event) {
			event = event || window.event;
			if(event.charCode && (event.charCode < 48 || event.charCode > 58))
				return false;
		}
		time_inputs[i].onpaste = function(event) {
			return false;
		}
	}
}

function processData() {
	alert("Apply button!");
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	output.innerHTML += message;	
}

window.addEventListener("load", init, false);