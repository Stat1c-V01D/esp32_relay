/*
 Name:		bed_level_ble.ino
 Created:	12.12.2018 14:33:04
 Author:	Kai
*/

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
int relayPin = 23;

void processReceivedValue(char command) {

	if (command == '1') { digitalWrite(relayPin, HIGH); }
	else if (command == '0') { digitalWrite(relayPin, LOW); }

	return;
}

void setup() {
	Serial.begin(115200);

	pinMode(relayPin, OUTPUT);
	digitalWrite(relayPin, LOW);

	if (!SerialBT.begin("ESP32")) {
		Serial.println("An error occurred initializing Bluetooth");
	}
	else {
		Serial.println("Bluetooth initialized");
	}
}

void loop() {

	while (SerialBT.available()) {

		char command = SerialBT.read();
		Serial.println(command);
		processReceivedValue(command);
	}

	delay(50);
}