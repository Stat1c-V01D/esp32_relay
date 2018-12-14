/*
 Name:		bed_level_async_server.ino
 Created:	13.12.2018 20:46:39
 Author:	Kai
*/

#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "digitalIO.h"
#include "decodeHTML.h"

const char* ssid = "FRITZ-WLAN-2";
const char* password = "01823938742039394944";

#define DEBUG
#define DETAIL_HTTP

AsyncWebServer server(80);
AsyncWebServerRequest *request;
AsyncWebServerResponse *response;

void setup() {
	relay_init();
	Serial.begin(115200);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}
	Serial.println("");
	Serial.println("WiFi connected.");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	server.on("/booth", HTTP_ANY, [](AsyncWebServerRequest *request) {
		//TODO
		Serial.println("Got Request!");
		choose_booth(request->url());
		request->send(200, "text/plain", "ok");
	});

	server.on("/right", HTTP_ANY, [](AsyncWebServerRequest *request) {
		//TODO
		Serial.println("Got Request!");
		choose_right(request->url());
		request->send(200, "text/plain", "ok");
	});

	server.on("/left", HTTP_ANY, [](AsyncWebServerRequest *request) {
		//TODO
		Serial.println("Got Request!");
		choose_left(request->url());
		request->send(200, "text/plain", "ok");
	});

	server.on("/calibrate", HTTP_ANY, [](AsyncWebServerRequest *request) {
		//TODO
		Serial.println("Got Request!");
		calibrate();
		request->send(200, "text/plain", "ok");
	});

	server.begin();
}

void loop() {}

