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
	// websocket.send("0x00");
	doLoopRequest();
}
function onMessage(evt) {
	// Действие при получении информации в текстовом виде
	processJSON(evt.data);
}
function onError(evt) {
	// Действие при возникновении ошибки
	writeToScreen('<span style="color: red;">ERROR:</span> ' + evt.data);
}
function onClose(evt) {
	// Действие при разъединении socket-соединения
}

function doSend(message) {
	// Метод отправки информации в текстовом виде
	// writeToScreen("SENT: " + message); 
	websocket.send(message);
}
function doLoopRequest() {
	(function worker() {
		websocket.send("0x00");
		setTimeout(worker, 500);
	})();
}
function doDisconnect() {
	// Метод разрыва socket-соединения
	var disconnect = document.getElementById("disconnect");
	disconnect.disabled = true;
	websocket.close();
}

function processJSON(msg) {
	// Метод обработки информации, полученной из JSON	
	var messages = "";
	var ap_mode = false;
	// writeToScreen("JSON: " + msg);
	var data = JSON.parse(msg);
	$.each(data, function(key, val) {
		// writeToScreen("Root key: " + key);
		$.each(val, function(subkey, subval) {
			// writeToScreen("Subkey: " + subkey + ", value: " + subval);
			var target = $("#" + subkey);
			switch (subkey) {
				case "cur_temp":
					document.getElementById(subkey).innerHTML = subval.toFixed(2) + "&deg;C";
					break;
				case "err_sensor":
					if (subval)
						messages += '<div class="alert alert-danger" role="alert">Temperature sensor error!</div>';
					break;
				case "power":
					if (subval) {
						target.removeClass("label-danger").addClass("label-success");
						document.getElementById(subkey).innerHTML = "On";
					}
					else {
						target.removeClass("label-success").addClass("label-danger");
						document.getElementById(subkey).innerHTML = "Off";
					}
					break;
				case "mode":
					if (subval) {
						target.removeClass("label-info").addClass("label-warning");
						document.getElementById(subkey).innerHTML = "Heating";
					}
					else {
						target.removeClass("label-warning").addClass("label-info");
						document.getElementById(subkey).innerHTML = "Cooling";
					}
					break;
				case "speed":
					switch (subval) {
						case 0x01:
							document.getElementById(subkey).innerHTML = "Low";
							break;
						case 0x02:
							document.getElementById(subkey).innerHTML = "Medium";
							break;
						case 0x03:
							document.getElementById(subkey).innerHTML = "High";
							break;
						default:
							document.getElementById(subkey).innerHTML = "Stopped";
							break;
					}
					break;
				case "set_temp":
					document.getElementById(subkey).innerHTML = subval.toString() + "&deg;C";
					break;
				default:
					// document.getElementById(subkey).innerHTML = subval;
					break;
			}
		});
	});
	document.getElementById("warnings").innerHTML = messages;
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	var pre = document.createElement("p");
	pre.style.wordWrap = "break-word";
	pre.innerHTML = message;
	output.appendChild(pre);
}

window.addEventListener("load", init, false);
