/*
 Name:		bed_level_2.ino
 Created:	15.01.2019 11:08:31
 Author:	KS
*/

#include <fauxmoESP.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <WiFi.h>
#include "digitalIO.h"
#include "decodeHTTP.h"
#include "defines.h"
#include "wifihandler.h"
#include "alexahandler.h"

#define WIFI_SSID "FRITZ-WLAN-2"
#define WIFI_PASS "01823938742039394944"

AsyncWebServer server(80);
AsyncWebServerRequest *request;
fauxmoESP fauxmo;

bool new_event = false;
struct event_data
{
	String side, part, action;
	bool processed;
};
event_data curr_event{};
event_data prev_event{};

bool alexa_event = false;
struct alexa_data
{
	const char * device_name;
	bool state;
	unsigned char value;
};
alexa_data current_device = { "",false,0 };

// the setup function runs once when you press reset or power the board
void setup() {
#ifdef DEBUG
	Serial.begin(115200);
#endif // DEBUG
	relay_init();
	connect_wifi();
	server_setup();
	fauxmo_setup();
	// fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
	// this way it's easier to match devices to action without having to compare strings.
	fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
		Serial.printf("[FAUXMO] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "HOCH" : "RUNTER", value);
		alexa_event = true;
		current_device.device_name = device_name;
		current_device.state = state;
		current_device.value = value;
	});
}

void server_setup() {
	server.on("/booth", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on BOOTH!");
#endif // HTTP_DEBUG
		choose_booth(request->url());
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on BOOTH!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
	});
	server.on("/right", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on RIGHT!");
#endif // HTTP_DEBUG
		choose_right(request->url());
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on RIGHT!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
	});
	server.on("/left", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on LEFT!");
#endif // HTTP_DEBUG
		choose_left(request->url());
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on LEFT!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
	});
	server.on("/calibrate", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on CALIBRATE!");
#endif // HTTP_DEBUG
		calibrate();
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on CALIBRATE!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
	});
	server.on("/preset", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on PRESET!");
#endif // HTTP_DEBUG
		collect_event(request->url());
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on PRESET!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
	});
	server.on("/stop", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on STOP!");
#endif // HTTP_DEBUG
		//TODO alter STOP function to reset all ongoing actions (especially Timer Events)
		stop();
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on STOP!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
	});
	server.on("/restart", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on RESTART!");
#endif // HTTP_DEBUG
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on RESTART!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
		esp_restart();
	});
	server.on("/alarm", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on ALARM!");
#endif // HTTP_DEBUG
		collect_event(request->url());
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on ALARM!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
		//TODO form proper Function for handling wakeup sequence (reference HANGUP TODO)
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
	/*
#define LEFT_HEAD "Bett links Kopf"
#define RIGHT_HEAD "Bett rechts Kopf"
#define LEFT_FEET "Bett links Fuß"
#define RIGHT_FEET "Bett rechts Fuß"
#define CALIBRATE "Bett Kalibrieren"

	fauxmo.addDevice(LEFT_HEAD);
	fauxmo.addDevice(RIGHT_HEAD);
	fauxmo.addDevice(LEFT_FEET);
	fauxmo.addDevice(RIGHT_FEET);
	fauxmo.addDevice(CALIBRATE);
	fauxmo.enable(true);
	*/
#define HEAD "Bett Kopf"
#define FEET "Bett Fuß"
#define CALIBRATE "Bett Kalibrieren"

	fauxmo.addDevice(HEAD);
	fauxmo.addDevice(FEET);
	fauxmo.addDevice(CALIBRATE);
	fauxmo.enable(true);
}

// the loop function runs over and over again until power down or reset
void loop() {
	fauxmo.handle();
	if (alexa_event)
	{
		alexa_handler(current_device.device_name, current_device.state, current_device.value);
		alexa_event = false;
		current_device.device_name = "";
		current_device.state = false;
		current_device.value = 0;
	}
	if (handshake_complete == true)
	{
		//TODO alter code for compatibility with new event_data struct
		process_event(event1, event2);
		handshake_complete = false;
	}
#ifdef DEBUG
	static unsigned long last = millis();
	if (millis() - last > 5000) {
		last = millis();
		Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
	}
#endif // DEBUG
}

