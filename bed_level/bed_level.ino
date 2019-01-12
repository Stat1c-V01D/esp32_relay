/*
 Name:		bed_level_wifi.ino
 Created:	12.12.2018 14:15:48
 Author:	KS
*/
#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "BluetoothSerial.h"
#include <Arduino.h>
#include <WiFi.h>
#include <fauxmoESP.h>
#include "digitalIO.h"
#include "decodeHTTP.h"
#include "defines.h"

#define WIFI_SSID "FRITZ-WLAN-2"
#define WIFI_PASS "01823938742039394944"

AsyncWebServer server(80);
AsyncWebServerRequest *request;
fauxmoESP fauxmo;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	relay_init();
	connect_wifi();
	server_setup();
	fauxmo_setup();
	// fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
	// this way it's easier to match devices to action without having to compare strings.
	fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
		Serial.printf("[FAUXMO] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF",value);
		fauxmo_act(device_id, device_name, state, value);
	});
}

// the loop function runs over and over again until power down or reset
void loop() {
	fauxmo.handle();
#ifdef DEBUG
	static unsigned long last = millis();
	if (millis() - last > 5000) {
		last = millis();
		Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
	}
#endif // DEBUG
	if (handshake_complete == true)
	{
		process_event(event1, event2);
		handshake_complete = false;
	}
}

void connect_wifi() {
	int tries = 0;
	WiFi.mode(WIFI_STA);
	Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED) {
		if (tries > 100)
		{
			enable_ap();
			break;
		}
		Serial.print(".");
		delay(100);
		tries++;
	}
	Serial.println();
	// Connected!
	Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void enable_ap() {
	Serial.println("[WIFI] Launching WiFi as Access Point");
	WiFi.mode(WIFI_AP);
#ifdef DEBUG
	Serial.print("[WIFI] Setting AP (Access Point)…");
#endif // DEBUG
	// Remove the password parameter, if you want the AP (Access Point) to be open
	WiFi.softAP("ESP32-AP", "123456789");
#ifdef DEBUG
	Serial.printf("[WIFI] ACCESS POINT Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.softAPIP().toString().c_str());
#endif // DEBUG
	//TODO create Webpage to configure ESP
}

void server_setup() {
	server.on("/booth", HTTP_ANY, [](AsyncWebServerRequest *request) {
		Serial.println("[HTTP] GET request on BOOTH!");
		choose_booth(request->url());
		Serial.println("[HTTP] SEND response on BOOTH!");
		request->send(200, "text/plain", "ok");
	});
	server.on("/right", HTTP_ANY, [](AsyncWebServerRequest *request) {
		Serial.println("[HTTP] GET request on RIGHT!");
		choose_right(request->url());
		Serial.println("[HTTP] SEND response on RIGHT!");
		request->send(200, "text/plain", "ok");
	});
	server.on("/left", HTTP_ANY, [](AsyncWebServerRequest *request) {
		Serial.println("[HTTP] GET request on LEFT!");
		choose_left(request->url());
		Serial.println("[HTTP] SEND response on LEFT!");
		request->send(200, "text/plain", "ok");
	});
	server.on("/calibrate", HTTP_ANY, [](AsyncWebServerRequest *request) {
		Serial.println("[HTTP] GET request on CALIBRATE!");
		calibrate();
		Serial.println("[HTTP] SEND response on CALIBRATE!");
		request->send(200, "text/plain", "ok");
	});
	server.on("/preset", HTTP_ANY, [](AsyncWebServerRequest *request) {
		Serial.println("[HTTP] GET request on PRESET!");
		collect_event(request->url());
		Serial.println("[HTTP] SEND response on PRESET!");
		request->send(200, "text/plain", "ok");
	});
	server.on("/stop", HTTP_ANY, [](AsyncWebServerRequest *request) {
		//TODO alter STOP function to reset all ongoing actions (especially Timer Events)
		Serial.println("[HTTP] GET request on STOP!");
		stop();
		Serial.println("[HTTP] SEND response on STOP!");
		request->send(200, "text/plain", "ok");
	});
	server.on("/alarm", HTTP_ANY, [](AsyncWebServerRequest *request) {
		Serial.println("[HTTP] GET request on ALARM!");
		collect_event(request->url());
		Serial.println("[HTTP] SEND response on ALARM!");
		request->send(200, "text/plain", "ok");
		//TODO form proper Funtion for handling wakeup sequence (reference HANGUP TODO)
		/*
		if (handshake_complete==true)
		{
			wake_routine(event1, event2);
			handshake_complete = false;
		}
		*/
	});
	server.begin();
}

void fauxmo_setup() {
	//TODO create setup for Fauxmo Device(s)
#define HEAD_UP "Bett Kopf hoch"
#define HEAD_DOWN "Bett Kopf runter"
#define FEET_UP "Bett Fuss hoch"
#define FEET_DOWN "Bett Fuss runter"
#define CALIBRATE "Bett Kalibrieren"

	fauxmo.addDevice(HEAD_UP);
	fauxmo.addDevice(HEAD_DOWN);
	fauxmo.addDevice(FEET_UP);
	fauxmo.addDevice(FEET_DOWN);
	fauxmo.addDevice(CALIBRATE);
	fauxmo.enable(true);
}

void fauxmo_act(unsigned char device_id, const char * device_name, bool state, unsigned char value) {
	if ((strcmp(device_name, HEAD_UP) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] HEAD UP switched by Alexa");
		//TODO action to be done when executed in all following Lamdas
	}
	if ((strcmp(device_name, HEAD_DOWN) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] HEAD DOWN switched by Alexa");

	}
	if ((strcmp(device_name, FEET_UP) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] FEET UP switched by Alexa");

	}
	if ((strcmp(device_name, FEET_DOWN) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] FEET DOWN switched by Alexa");

	}
	if ((strcmp(device_name, CALIBRATE) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] CALIBRATE switched by Alexa");

	}
}