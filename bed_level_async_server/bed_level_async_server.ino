/*
 Name:		bed_level_async_server.ino
 Created:	13.12.2018 20:46:39
 Author:	Kai
*/

#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "FRITZ-WLAN-2";
const char* password = "01823938742039394944";

String header, side, part, action;
int eventTime;

#define DEBUG
#define DETAIL_HTTP

// Assign output variables to GPIO pins
#define relay01 16
#define relay02 17
#define relay03 18
#define relay04 19

#define relay05 26
#define relay06 27
#define relay07 32
#define relay08 33

AsyncWebServer server(80);

void disable_all() {
	digitalWrite(relay01, HIGH);
	digitalWrite(relay02, HIGH);
	digitalWrite(relay03, HIGH);
	digitalWrite(relay04, HIGH);
	digitalWrite(relay05, HIGH);
	digitalWrite(relay06, HIGH);
	digitalWrite(relay07, HIGH);
	digitalWrite(relay08, HIGH);
}

void enable_all() {
	digitalWrite(relay01, LOW);
	digitalWrite(relay02, LOW);
	digitalWrite(relay03, LOW);
	digitalWrite(relay04, LOW);
	digitalWrite(relay05, LOW);
	digitalWrite(relay06, LOW);
	digitalWrite(relay07, LOW);
	digitalWrite(relay08, LOW);
}

void relay_init() {
	// Initialize the output variables as outputs
	pinMode(relay01, OUTPUT);
	pinMode(relay02, OUTPUT);
	pinMode(relay03, OUTPUT);
	pinMode(relay04, OUTPUT);
	pinMode(relay05, OUTPUT);
	pinMode(relay06, OUTPUT);
	pinMode(relay07, OUTPUT);
	pinMode(relay08, OUTPUT);
	// Prime outputs to desired value
	disable_all();
}

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

	server.on("/booth", HTTP_PATCH, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "ok");
		//TODO
		choose_booth(request->url());
	});

	server.on("/right", HTTP_PATCH, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "ok");
		//TODO
		choose_right(request->url());
	});

	server.on("/left", HTTP_PATCH, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "ok");
		//TODO
		choose_left(request->url());
	});

	server.on("/calibrate", HTTP_PATCH, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "ok");
		//TODO
		calibrate();
	});

	server.begin();
}

void loop() {}

void choose_booth(String header) {
	side = "booth";
	if (header.indexOf("/booth/head") >= 0)
	{
		part = "head";
		if (header.indexOf("/booth/head/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("/booth/head/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("/booth/head/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("/booth/head/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("/booth/head/timer") >= 0)
		{
			//TODO
		}
#ifdef DEBUG
		Serial.println("Acting now!");
#endif // DEBUG
		gpio_act(side, part, action);
	}
	else
	{
		part = "feet";
		if (header.indexOf("/booth/feet/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("/booth/feet/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("/booth/feet/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("/booth/feet/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("/booth/feet/timer") >= 0)
		{
			//TODO
		}
#ifdef DEBUG
		Serial.println("Acting now!");
#endif // DEBUG
		gpio_act(side, part, action);
	}
}
void choose_left(String header) {
	side = "left";
	if (header.indexOf("/left/head") >= 0)
	{
		part = "head";
		if (header.indexOf("/left/head/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("/left/head/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("/left/head/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("/left/head/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("/left/head/timer") >= 0)
		{
			//TODO
		}
#ifdef DEBUG
		Serial.println("Acting now!");
#endif // DEBUG
		gpio_act(side, part, action);
	}
	else
	{
		part = "feet";
		if (header.indexOf("/left/feet/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("/left/feet/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("/left/feet/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("/left/feet/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("/left/feet/timer") >= 0)
		{
			//TODO
		}
#ifdef DEBUG
		Serial.println("Acting now!");
#endif // DEBUG

		gpio_act(side, part, action);
	}
}
void choose_right(String header) {
	side = "right";
	if (header.indexOf("/right/head") >= 0)
	{
		part = "head";
		if (header.indexOf("/right/head/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("/right/head/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("/right/head/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("/right/head/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("/right/head/timer") >= 0)
		{
			//TODO
		}
#ifdef DEBUG
		Serial.println("Acting now!");
#endif // DEBUG
		gpio_act(side, part, action);
	}
	else
	{
		part = "feet";
		if (header.indexOf("/right/feet/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("/right/feet/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("/right/feet/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("/right/feet/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("/right/feet/timer") >= 0)
		{
			//TODO
		}
#ifdef DEBUG
		Serial.println("Acting now!");
#endif // DEBUG
		gpio_act(side, part, action);
	}
}

void gpio_act(String side, String part, String action) {

#ifdef DEBUG
	Serial.println("Settings:");
	Serial.print("-->side:");
	Serial.println(side);
	Serial.print("-->part:");
	Serial.println(part);
	Serial.print("-->action:");
	Serial.println(action);
#endif // DEBUG

	if (action == "stop")
	{
		stop(side, part);
	}
	if (part == "head")
	{
		if (action == "up")
		{
			head_up(side);
		}
		else if (action == "down")
		{
			head_down(side);
		}
		else if (action == "reset")
		{
			head_reset(side, part);
		}
		else
		{
			//TODO
		}
	}
	if (part == "feet")
	{
		if (action == "up")
		{
			feet_up(side);
		}
		else if (action == "down")
		{
			feet_down(side);
		}
		else if (action == "reset")
		{
			feet_reset(side, part);
		}
		else
		{
			//TODO
		}

	}
}

void head_up(String side) {
	if (side == "booth")
	{
		digitalWrite(relay02, HIGH);
		digitalWrite(relay06, HIGH);
		digitalWrite(relay01, LOW);
		digitalWrite(relay05, LOW);
	}
	else if (side == "right")
	{
		digitalWrite(relay02, HIGH);
		digitalWrite(relay01, LOW);
	}
	else
	{
		digitalWrite(relay06, HIGH);
		digitalWrite(relay05, LOW);
	}
}
void head_down(String side) {
	if (side == "booth")
	{
		digitalWrite(relay01, HIGH);
		digitalWrite(relay05, HIGH);
		digitalWrite(relay02, LOW);
		digitalWrite(relay06, LOW);
	}
	else if (side == "right")
	{
		digitalWrite(relay01, HIGH);
		digitalWrite(relay02, LOW);
	}
	else
	{
		digitalWrite(relay05, HIGH);
		digitalWrite(relay06, LOW);
	}
}
void head_reset(String side, String part) {
	head_down(side);
	delay(5000);
	stop(side, part);
}
void feet_up(String side) {
	if (side == "booth")
	{
		digitalWrite(relay04, HIGH);
		digitalWrite(relay08, HIGH);
		digitalWrite(relay03, LOW);
		digitalWrite(relay07, LOW);
	}
	else if (side == "right")
	{
		digitalWrite(relay04, HIGH);
		digitalWrite(relay03, LOW);
	}
	else
	{
		digitalWrite(relay08, HIGH);
		digitalWrite(relay07, LOW);
	}
}
void feet_down(String side) {
	if (side == "booth")
	{
		digitalWrite(relay03, HIGH);
		digitalWrite(relay07, HIGH);
		digitalWrite(relay04, LOW);
		digitalWrite(relay08, LOW);
	}
	else if (side == "right")
	{
		digitalWrite(relay03, HIGH);
		digitalWrite(relay04, LOW);
	}
	else
	{
		digitalWrite(relay07, HIGH);
		digitalWrite(relay08, LOW);
	}
}
void feet_reset(String side, String part) {
	feet_down(side);
	delay(5000);
	stop(side, part);
}
void stop(String side, String part) {
	if (side == "booth")
	{
		if (part == "head")
		{
			digitalWrite(relay01, HIGH);
			digitalWrite(relay02, HIGH);
			digitalWrite(relay05, HIGH);
			digitalWrite(relay06, HIGH);
		}
		else
		{
			digitalWrite(relay03, HIGH);
			digitalWrite(relay04, HIGH);
			digitalWrite(relay07, HIGH);
			digitalWrite(relay08, HIGH);
		}
	}
	else if (side == "right")
	{
		if (part == "head")
		{
			digitalWrite(relay01, HIGH);
			digitalWrite(relay02, HIGH);
		}
		else
		{
			digitalWrite(relay03, HIGH);
			digitalWrite(relay04, HIGH);
		}
	}
	else if (side == "left")
	{
		if (part == "head")
		{
			digitalWrite(relay05, HIGH);
			digitalWrite(relay06, HIGH);
		}
		else
		{
			digitalWrite(relay07, HIGH);
			digitalWrite(relay08, HIGH);
		}
	}
	else
	{
		disable_all();
	}
}

void calibrate() {
	enable_all();
	delay(5000);
	disable_all();
}

void timerEvent(String side, String part, int time) {

}