var output;

var set_temp_min;
var set_temp_max;
var set_temp_def;
var delta_temp_min;
var delta_temp_max;
var delta_temp_def;

var time_from_hours;
var time_from_mins;
var time_to_hours;
var time_to_mins;

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
	websocket.send(JSON.stringify({type: 0x00}));
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
					document.getElementById(subkey).selectedIndex = (subval - 1);
					break;
				case "set_temp_min":
					set_temp_min = subval;
					break;
				case "set_temp_max":
					set_temp_max = subval;
					break;
				case "set_temp_def":
					set_temp_def = subval;
					break;
				case "delta_temp_min":
					delta_temp_min = subval;
					break;
				case "delta_temp_max":
					delta_temp_max = subval;
					break;
				case "delta_temp_def":
					delta_temp_def = subval;
					break;
				case "scheduler":
					if(subval)
						document.getElementById("scheduler-on").checked = true;
					else
						document.getElementById("scheduler-off").checked = true;
					break;
				case "dow":
					document.getElementById("sun").checked = ((subval & (1 << 0)) >> 0);
					document.getElementById("mon").checked = ((subval & (1 << 1)) >> 1);
					document.getElementById("tue").checked = ((subval & (1 << 2)) >> 2);
					document.getElementById("wed").checked = ((subval & (1 << 3)) >> 3);
					document.getElementById("thu").checked = ((subval & (1 << 4)) >> 4);
					document.getElementById("fri").checked = ((subval & (1 << 5)) >> 5);
					document.getElementById("sat").checked = ((subval & (1 << 6)) >> 6);
					break;
				default:
					if(document.getElementById(subkey) != null)
						document.getElementById(subkey).value = subval;
					break;
			}
		}
	}
}

function processInput() {
	// Метод проверки ввода
	var num_inputs = [
		document.getElementById("set_temp"),
		document.getElementById("delta_temp")
	];
	
	var time_inputs = [
		document.getElementById("time_from_hours"),
		document.getElementById("time_from_mins"),
		document.getElementById("time_to_hours"),
		document.getElementById("time_to_mins")
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
			if(event.charCode && (event.charCode < 48 || event.charCode > 57))
				return false;
		}
		time_inputs[i].onpaste = function(event) {
			return false;
		}
	}
}

function getAirSettings() {
	// Получение конфигурации кондиционера в формате JSON-строки
	var root = {};
	root["type"] = 0x10;
	var settings = {};
	settings["power"] = ((document.getElementById("power-on").checked && !document.getElementById("power-off").checked) ? true : false);
	settings["mode"]  = ((document.getElementById("mode").selectedIndex == 1) ? true : false);
	settings["speed"] = (document.getElementById("speed").selectedIndex + 1);
	
	var set_temp = parseInt(document.getElementById("set_temp").value, 10);
	if((set_temp < set_temp_min) || (set_temp > set_temp_max)) {
		set_temp = set_temp_def;
		document.getElementById("set_temp").value = set_temp;
		alert("Temperature must be in range of +" + set_temp_min + " .. +" + set_temp_max + "°C!");
	}
	settings["set_temp"] = set_temp;
	
	var delta_temp = parseInt(document.getElementById("delta_temp").value, 10);
	if((delta_temp < delta_temp_min) || (delta_temp > delta_temp_max)) {
		delta_temp = delta_temp_def;
		document.getElementById("delta_temp").value = delta_temp;
		alert("Temperature tolerance must be in range of " + delta_temp_min + " .. +" + delta_temp_max + "°C!");
	}
	settings["delta_temp"] = delta_temp;
		
	root["settings"] = settings;
	return JSON.stringify(root);	
}
function getSchedulerSettings() {
	// Получение конфигурации планировщика в формате JSON-строки
	var root = {};
	root["type"] = 0x18;
	var settings = {};
	settings["scheduler"] = ((document.getElementById("scheduler-on").checked && !document.getElementById("scheduler-off").checked) ? true : false);
	settings["dow"] = (((document.getElementById("sun").checked ? 1 : 0) << 0) | ((document.getElementById("mon").checked ? 1 : 0) << 1) | ((document.getElementById("tue").checked ? 1 : 0) << 2) | ((document.getElementById("wed").checked ? 1 : 0) << 3) | ((document.getElementById("thu").checked ? 1 : 0) << 4) | ((document.getElementById("fri").checked ? 1 : 0) << 5) | ((document.getElementById("sat").checked ? 1 : 0) << 6));
	
	time_from_hours = parseInt(document.getElementById("time_from_hours").value, 10);
	if((time_from_hours < 0) || (time_from_hours > 23)) {
		time_from_hours = 0;
		document.getElementById("time_from_hours").value = 0;
		alert("Hours value must be from 0 to 23!");
	}
	settings["time_from_hours"] = time_from_hours;
	
	time_from_mins = parseInt(document.getElementById("time_from_mins").value, 10);
	if((time_from_mins < 0) || (time_from_mins > 59)) {
		time_from_mins = 0;
		document.getElementById("time_from_mins").value = 0;
		alert("Minutes value must be from 0 to 59!");
	}
	settings["time_from_mins"] = time_from_mins;

	time_to_hours = parseInt(document.getElementById("time_to_hours").value, 10);
	if((time_to_hours < 0) || (time_to_hours > 23)) {
		time_to_hours = 0;
		document.getElementById("time_to_hours").value = 0;
		alert("Hours value must be from 0 to 23!");
	}
	settings["time_to_hours"] = time_to_hours;
	
	time_to_mins = parseInt(document.getElementById("time_to_mins").value, 10);
	if((time_to_mins < 0) || (time_to_mins > 59)) {
		time_to_mins = 0;
		document.getElementById("time_to_mins").value = 0;
		alert("Minutes value must be from 0 to 59!");
	}
	settings["time_to_mins"] = time_to_mins;
	
	if((time_from_hours == time_to_hours) && (time_from_mins >= time_to_mins)) {
		time_from_mins = 0;
		time_to_mins   = 0;
		document.getElementById("time_from_mins").value = 0;
		document.getElementById("time_to_mins").value   = 0;
		alert("\"Time to\" must be greater than \"time from\"!");		
	}
	settings["time_from_mins"] = time_from_mins;
	settings["time_to_mins"]   = time_to_mins;
	
	root["settings"] = settings;
	return JSON.stringify(root);	
}

function onApplyButton() {
	// Метод обработки данных при нажатии на кнопку "Apply"
	websocket.send(getAirSettings());
	websocket.send(getSchedulerSettings());
}
function onCancelButton() {
	// Метод обработки данных при нажатии на кнопку "Cancel"
	websocket.send(JSON.stringify({type: 0x00}));
	websocket.send(JSON.stringify({type: 0x08}));
}

function writeToScreen(message) {
	// Метод вывода информации на страницу
	output.innerHTML += message;	
}

window.addEventListener("load", init, false);