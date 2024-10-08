// Get references to UI elements
let container = document.querySelector(".container");

let connectButton = document.getElementById('connect');
let disconnectButton = document.getElementById('disconnect');

let startLeftTofNotifyButton = document.getElementById('startLeftTofNotifyButton');
let stopLeftTofNotifyButton = document.getElementById('stopLeftTofNotifyButton');
let startRightTofNotifyButton = document.getElementById('startRightTofNotifyButton');
let stopRightTofNotifyButton = document.getElementById('stopRightTofNotifyButton');
let startFrontUltrasonicNotifyButton = document.getElementById('startFrontUltrasonicNotifyButton');
let stopFrontUltrasonicNotifyButton = document.getElementById('stopFrontUltrasonicNotifyButton');

let terminalContainer = document.getElementById('terminal');
let leftTofContainer = document.getElementById('leftTof');
let rightTofContainer = document.getElementById('rightTof');
let frontUltrasonicContainer = document.getElementById('frontUltrasonic');

let stopCommandButton = document.getElementById('stopCommandButton');
let forwardCommandButton = document.getElementById('forwardCommandButton');
let backwardCommandButton = document.getElementById('backwardCommandButton');
let leftCommandButton = document.getElementById('leftCommandButton');
let rightCommandButton = document.getElementById('rightCommandButton');
// let enableCommandButton = document.getElementById('enableCommandButton');


let manualControlButton = document.getElementById('manualControlButton');
let autoControlButton = document.getElementById('autoControlButton');

// Connect to the device on Connect button click
connectButton.addEventListener('click', function(){connect();});

// Disconnect from the device on Disconnect button click
disconnectButton.addEventListener('click', function(){disconnect();});

startLeftTofNotifyButton.addEventListener('click', function(){startLeftTofNotifications();});
stopLeftTofNotifyButton.addEventListener('click', function(){stopLeftTofNotifications();});
startRightTofNotifyButton.addEventListener('click', function(){startRightTofNotifications();});
stopRightTofNotifyButton.addEventListener('click', function(){stopRightTofNotifications();});
startFrontUltrasonicNotifyButton.addEventListener('click', function(){startFrontUltrasonicNotifications();});
stopFrontUltrasonicNotifyButton.addEventListener('click', function(){stopFrontUltrasonicNotifications();});


manualControlButton.addEventListener('click', function(){changeControl(1)});
autoControlButton.addEventListener('click', function(){changeControl(0)});
let controlEnabled = false;

// Selected device object cache
let deviceCache = null;
let serviceCache = null;
let leftTofCharCache = null;
let rightTofCharCache = null;
let frontUltrasonicCharCache = null;
let commandCharCache = null;


function changeControl(command){
	if(command == 0){
		//autoDriving
		manualControlButton.disabled = false;
		autoControlButton.disabled = true;
	}
	else{
		//manual driving
		manualControlButton.disabled = true;
		autoControlButton.disabled = false;
	}
	sendCommand(command);
}
function requestBluetoothDevice() {
	log('Requesting Bluetooth device...');
	return navigator.bluetooth.requestDevice({
		filters: [{name: "Trackbot"}],
		optionalServices: ["114041d2-46a9-43c8-bc6e-6bd5389675c6"]}).
      then(device => {
			log('"' + device.name + '" Bluetooth device selected');
			deviceCache = device;
			deviceCache.addEventListener('gattserverdisconnected', handleDisconnection);
			connectButton.disabled = true;
			disconnectButton.disabled = false;
			return deviceCache;
      });
}

function connect() {
	return (deviceCache ? Promise.resolve(deviceCache) : requestBluetoothDevice()).
      then(device => connectDeviceAndCacheService(device)).
		then(service => {
			let leftTofChar = service.getCharacteristic("114041d2-46a9-43c8-bc6e-6bd5389675c8");
			let rightTofChar = service.getCharacteristic("114041d2-46a9-43c8-bc6e-6bd5389675c9");
			let frontUltrasonicChar = service.getCharacteristic("114041d2-46a9-43c8-bc6e-6bd5389675ca");
			let commandChar = service.getCharacteristic("114041d2-46a9-43c8-bc6e-6bd5389675c7");
			return Promise.all([leftTofChar, rightTofChar, frontUltrasonicChar, commandChar])
		}).
		then(([leftTofChar, rightTofChar, frontUltrasonicChar, commandChar]) => {
			if(leftTofChar.properties.notify)
				startLeftTofNotifyButton.disabled = false;
			if(rightTofChar.properties.notify)
				startRightTofNotifyButton.disabled = false;
			if(frontUltrasonicChar.properties.notify)
				startFrontUltrasonicNotifyButton.disabled = false;

			manualControlButton.disabled = false;
			
			
			leftTofCharCache = leftTofChar;
			rightTofCharCache = rightTofChar;
			frontUltrasonicCharCache = frontUltrasonicChar;
			commandCharCache = commandChar;
		}).
      catch(error => log(error));
}

// Connect to the device specified and get service
function connectDeviceAndCacheService(device) {
	if (device.gatt.connected && serviceCache) {
		return Promise.resolve(serviceCache);
	}
	log('Connecting to GATT server...');
	return device.gatt.connect().
      then(server => {
        log('GATT server connected, getting service...');
        return server.getPrimaryService("114041d2-46a9-43c8-bc6e-6bd5389675c6");
      }).
		then(service => {
        log('Service found');
        serviceCache = service;
        return serviceCache;
      });
}

// Disconnect from the connected device
function disconnect() {
	if (deviceCache) {
		log('Disconnecting from "' + deviceCache.name + '" Bluetooth device...');
		deviceCache.removeEventListener('gattserverdisconnected', handleDisconnection);

		if (deviceCache.gatt.connected) {
			deviceCache.gatt.disconnect();
			log('"' + deviceCache.name + '" Bluetooth device disconnected');
		} else {
			log('"' + deviceCache.name + '" Bluetooth device is already disconnected');
		}
	}	

	if (leftTofCharCache && leftTofCharCache.properties.notify) {
		leftTofCharCache.removeEventListener('characteristicvaluechanged', handleLeftTofValueChanged);
		leftTofCharCache = null;
	}	
	if (rightTofCharCache && rightTofCharCache.properties.notify) {
		rightTofCharCache.removeEventListener('characteristicvaluechanged', handleRightTofValueChanged);
		rightTofCharCache = null;
	}	
	if (frontUltrasonicCharCache && frontUltrasonicCharCache.properties.notify) {
		frontUltrasonicCharCache.removeEventListener('characteristicvaluechanged', handleFrontUltrasonicValueChanged);
		frontUltrasonicCharCache = null;
	}	
	
	deviceCache = null;
	connectButton.disabled = false;
	disconnectButton.disabled = true;
	startLeftTofNotifyButton.disabled = true;
	stopLeftTofNotifyButton.disabled = true;
	startRightTofNotifyButton.disabled = true;
	stopRightTofNotifyButton.disabled = true;
	startFrontUltrasonicNotifyButton.disabled = true;
	stopFrontUltrasonicNotifyButton.disabled = true;
	leftTofContainer.innerHTML = "?";
	rightTofContainer.innerHTML = "?";
	frontUltrasonicContainer.innerHTML = "?";
	
	// enableCommandButton.disabled = true;
	forwardCommandButton.disabled = true;
	stopCommandButton.disabled = true;
	leftCommandButton.disabled = true;
	rightCommandButton.disabled = true;
	// enableCommandButton.innerText = "Enable Control";	
}

function handleDisconnection(event) {
	let device = event.target;
	log('"' + device.name + '" Bluetooth device disconnected, trying to reconnect...');
	connectDeviceAndCacheService(device).
	then(service => {
		let leftTofChar = service.getCharacteristic("114041d2-46a9-43c8-bc6e-6bd5389675c8");
		let rightTofChar = service.getCharacteristic("114041d2-46a9-43c8-bc6e-6bd5389675c9");
		let frontUltrasonicChar = service.getCharacteristic("114041d2-46a9-43c8-bc6e-6bd5389675ca");
		let commandChar = service.getCharacteristic("114041d2-46a9-43c8-bc6e-6bd5389675c7");
		return Promise.all([leftTofChar, rightTofChar, frontUltrasonicChar, commandChar])
	}).
	then(([leftTofChar, rightTofChar, frontUltrasonicChar, commandChar]) => {
			if(leftTofChar.properties.notify)
				startLeftTofNotifyButton.disabled = false;
			if(rightTofChar.properties.notify)
				startRightTofNotifyButton.disabled = false;
			if(frontUltrasonicChar.properties.notify)
				startFrontUltrasonicNotifyButton.disabled = false;
			if(commandChar.properties.notify)
				sendCommandButton.disabled = false;

			leftTofCharCache = leftTofChar;
			rightTofCharCache = rightTofChar;
			frontUltrasonicCharCache = frontUltrasonicChar;
			commandCharCache = commandChar;
	}).
	catch(error => log(error));
}


// Output to terminal
function log(data) {
	terminalContainer.innerHTML = '<div>' + data + '</div>';
}

function handleLeftTofValueChanged(event) {
	let value = event.target.value.getInt16(0, true);
	leftTofContainer.innerHTML = value;
}

function handleRightTofValueChanged(event) {
	let value = event.target.value.getInt16(0, true);
	rightTofContainer.innerHTML = value;
}

function handleFrontUltrasonicValueChanged(event) {
	let value = event.target.value.getInt16(0, true);
	frontUltrasonicContainer.innerHTML = value;
}

function startLeftTofNotifications() {
	log('Starting left ToF sensor notifications...');
	return leftTofCharCache.startNotifications().
      then(() => {
        log('Left ToF sensor notifications started');
		  startLeftTofNotifyButton.disabled = true;
		  stopLeftTofNotifyButton.disabled = false;
		  leftTofCharCache.addEventListener('characteristicvaluechanged', handleLeftTofValueChanged);
      });
}

function stopLeftTofNotifications() {
	log('Stopping left ToF sensor notifications...');
	return leftTofCharCache.stopNotifications().
      then(() => {
        log('Left ToF sensor notifications stopped');
		  startLeftTofNotifyButton.disabled = false;
		  stopLeftTofNotifyButton.disabled = true;
		  leftTofCharCache.removeEventListener('characteristicvaluechanged', handleLeftTofValueChanged);
      });
}

function startRightTofNotifications() {
	log('Starting right ToF sensor notifications...');
	return rightTofCharCache.startNotifications().
      then(() => {
        log('Right ToF sensor notifications started');
		  startRightTofNotifyButton.disabled = true;
		  stopRightTofNotifyButton.disabled = false;
		  rightTofCharCache.addEventListener('characteristicvaluechanged', handleRightTofValueChanged);
      });
}

function stopRightTofNotifications() {
	log('Stopping right ToF sensor notifications...');
	return rightTofCharCache.stopNotifications().
      then(() => {
        log('Right ToF sensor notifications stopped');
		  startRightTofNotifyButton.disabled = false;
		  stopRightTofNotifyButton.disabled = true;
		  rightTofCharCache.removeEventListener('characteristicvaluechanged', handleRightTofValueChanged);
      });
}

function startFrontUltrasonicNotifications() {
	log('Starting front ultrasonic sensor notifications...');
	return frontUltrasonicCharCache.startNotifications().
      then(() => {
        log('Front ultrasonic sensor notifications started');
		  startFrontUltrasonicNotifyButton.disabled = true;
		  stopFrontUltrasonicNotifyButton.disabled = false;
		  frontUltrasonicCharCache.addEventListener('characteristicvaluechanged', handleFrontUltrasonicValueChanged);
      });
}

function stopFrontUltrasonicNotifications() {
	log('Stopping front ultrasonic sensor notifications...');
	return frontUltrasonicCharCache.stopNotifications().
      then(() => {
        log('Front ultrasonic sensor notifications stopped');
		  startFrontUltrasonicNotifyButton.disabled = false;
		  stopFrontUltrasonicNotifyButton.disabled = true;
		  frontUltrasonicCharCache.removeEventListener('characteristicvaluechanged', handleFrontUltrasonicValueChanged);
      });
}

function sendCommand(command) {
	if (command == 0){
		container.style.transform = "rotate(0deg)";
		leftCommandButton.disabled = true;
		rightCommandButton.disabled = true;
		forwardCommandButton.disabled = true;
		backwardCommandButton.disabled = true;
		stopCommandButton.disabled = true;
		// while(automode){
		// 	autoCommand = commandCharCache.readValue().then(value => value.getInt16(0,true));
		// 	if (autoCommand == 1 || autoCommand == 2 || autoCommand == 3)
		// 	{
		// 		container.style.transform = "rotate(0deg)";
		// 	}
		// 	else if (autoCommand = 4){
		// 		container.style.transform = "rotate(-30deg)";
		// 	}
		// 	else if (autoCommand = 5){
		// 		container.style.transform = "rotate(30deg)";
		// 	}
		// }
	}
	else if (command == 1){
		container.style.transform = "rotate(0deg)";
		leftCommandButton.disabled = true;
		rightCommandButton.disabled = true;
		forwardCommandButton.disabled = false;
		backwardCommandButton.disabled = false;
		stopCommandButton.disabled = false;
	}
	else if (command == 2){ //forward
		container.style.transform = "rotate(0deg)";
		leftCommandButton.disabled = false;
		rightCommandButton.disabled = false;
		forwardCommandButton.disabled = true;
		backwardCommandButton.disabled = false;
		stopCommandButton.disabled = false;
	}
	else if (command == 3){ //backward
		container.style.transform = "rotate(0deg)";
		leftCommandButton.disabled = true;
		rightCommandButton.disabled = true;
		forwardCommandButton.disabled = false;
		backwardCommandButton.disabled = true;
		stopCommandButton.disabled = false;
	}
	else if (command == 4)	{		// turn left
		container.style.transform = "rotate(-30deg)";
		leftCommandButton.disabled = true;	
		rightCommandButton.disabled = true;
		forwardCommandButton.disabled = false;
		backwardCommandButton.disabled = false;
		stopCommandButton.disabled = false;
	}
	else if (command == 5) {		// turn right
		container.style.transform = "rotate(30deg)"
		leftCommandButton.disabled = true;
		rightCommandButton.disabled = true;
		forwardCommandButton.disabled = false;
		backwardCommandButton.disabled = false;
		stopCommandButton.disabled = false;
	}
   const commandArray = Uint8Array.of(command);
   return commandCharCache.writeValueWithResponse(commandArray) .
		then(() => log("Command " + command + " sent successfully"));
}
