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
	doLoopRequest();
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
	// writeToScreen('<div class="alert alert-info" role="alert">Connection closed.</div>');	
}

function doSend(message) {
	// Метод отправки информации в текстовом виде 
	websocket.send(message);
}
function doLoopRequest() {
	//(function repeater() {
		websocket.send(0x01);
		websocket.send(0x05);
		//setTimeout(repeater, 500);
	//})();
}
function doDisconnect() {
	// Метод разрыва socket-соединения
	websocket.close();
}

function processJSON(msg) {
	// Метод обработки информации, полученной из JSON
	var ap_wifi_enabled = false;
	var st_wifi_enabled = false;
	var data = JSON.parse(msg);
	writeToScreen("<br>" + msg + "<br>");
	for (var key in data) {
		writeToScreen("Key: " + key + "<br>");
		var val = data[key];
		for (var subkey in val) {
			var subval = val[subkey];
			writeToScreen(subkey + "=" + subval + "<br>");
			switch (subkey) {
				case "cur_temp":
					document.getElementById(subkey).innerHTML = subval.toFixed(2) + "&deg;C";
					break;
				case "err_sensor":
					if (subval)
						document.getElementById("messages").innerHTML = '<div class="alert alert-danger" role="alert">Temperature sensor error!</div>';
					break;
				case "power":
					if (subval) {
						// target.removeClass("label-danger").addClass("label-success");
						document.getElementById(subkey).innerHTML = "On";
					}
					else {
						// target.removeClass("label-success").addClass("label-danger");
						document.getElementById(subkey).innerHTML = "Off";
					}
					break;
				case "mode":
					if (subval) {
						// target.removeClass("label-info").addClass("label-warning");
						document.getElementById(subkey).innerHTML = "Heating";
					}
					else {
						// target.removeClass("label-warning").addClass("label-info");
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
				case "ap_wifi_enabled":
					ap_wifi_enabled = subval;
					if (subval) {
						// target.removeClass("label-danger").addClass("label-success");
						document.getElementById(subkey).innerHTML = "On";
					}
					else {
						// target.removeClass("label-success").addClass("label-danger");
						document.getElementById(subkey).innerHTML = "Off";
					}
					document.getElementById("ap_status").innerHTML = "";
					break;
				case "ap_wifi_ssid":
					if (ap_wifi_enabled)
						document.getElementById("ap_status").innerHTML += '<p>AP SSID: <span id="' + subkey + '" class="label label-default">' + subval + '</span></p>';
					break;
				case "ap_wifi_ip":
					if (ap_wifi_enabled)
						document.getElementById("ap_status").innerHTML += '<p>AP IP: <span id="' + subkey + '" class="label label-default">' + subval + '</span></p>';
					break;
				case "st_wifi_enabled":
					st_wifi_enabled = subval;
					if (subval) {
						// target.removeClass("label-danger").addClass("label-success");
						document.getElementById(subkey).innerHTML = "On";
					}
					else {
						// target.removeClass("label-success").addClass("label-danger");
						document.getElementById(subkey).innerHTML = "Off";
					}
					document.getElementById("st_status").innerHTML = "";
					break;
				case "st_wifi_ssid":
					if (st_wifi_enabled)
						document.getElementById("st_status").innerHTML += '<p>Station SSID: <span id="' + subkey + '" class="label label-default">' + subval + '</span></p>';
					break;
				case "st_wifi_connected":
					if (st_wifi_enabled) {
						if (subval)
							document.getElementById("st_status").innerHTML += '<p>Client connected: <span id="' + subkey + '" class="label label-success">' + "Yes" + '</span></p>';
						else
							document.getElementById("st_status").innerHTML += '<p>Client connected: <span id="' + subkey + '" class="label label-danger">' + "No" + '</span></p>';
					}
					break;
				case "st_wifi_ip":
					if (st_wifi_enabled)
						document.getElementById("st_status").innerHTML += '<p>Client IP: <span id="' + subkey + '" class="label label-default">' + subval + '</span></p>';
					break;
				default:
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