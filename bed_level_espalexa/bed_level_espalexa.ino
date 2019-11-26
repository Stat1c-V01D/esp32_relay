/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */


 /*
  Name:		bed_level_wifi.ino
  Created:	12.12.2018 14:15:48
  Author:	KS
 */
#define ESPALEXA_ASYNC
#include <EspalexaDevice.h>
#include <Espalexa.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <WiFi.h>
//#include <fauxmoESP.h>
#include "digitalIO.h"
#include "decodeHTTP.h"
#include "defines.h"

#define WIFI_SSID ""
#define WIFI_PASS ""

AsyncWebServer server(80);
AsyncWebServerRequest *request;

Espalexa espalexa;
EspalexaDevice* d;

void espalexa_cb(EspalexaDevice* dev);

bool c_request = false;
String capture = "", c_url = "";

bool alexa_event = false;
struct alexa_data
{
	int device_name;
	unsigned char value;
};
alexa_data current_device = { -1,0 };

struct device_data
{
	unsigned char current_val;
};
device_data dev0 = { 0 };
device_data dev1 = { 0 };

// the setup function runs once when you press reset or power the board
void setup() {
#ifdef DEBUG
	Serial.begin(115200);
#endif // DEBUG
	relay_init();
	connect_wifi();
	server_setup();
	espalexa_setup();
}

void espalexa_cb(EspalexaDevice* dev)
{
	alexa_event = true;
	current_device.device_name = d->getId();
	current_device.value = d->getValue();
}

// the loop function runs over and over again until power down or reset
void loop() {
	espalexa.loop();
	if (alexa_event)
	{
		alexa_handler(current_device.device_name, current_device.value);
		alexa_event = false;
		current_device.device_name = -1;
		current_device.value = 0;
	}
	if (handshake_complete)
	{
		process_event(event1, event2);
		handshake_complete = false;
	}
	if (c_request)
	{
#ifdef DEBUG
		Serial.print("Captured URL: ");
		Serial.println(c_url);
		Serial.print("Capture Flag: ");
		Serial.println(capture);
#endif // DEBUG
		if (capture == "booth")
		{
#ifdef DEBUG
			Serial.println("Captured BOOTH");
#endif // DEBUG
			choose_booth(c_url);
			purge_data();
			return;
		}
		if (capture == "right")
		{
#ifdef DEBUG
			Serial.println("Captured RIGHT");
#endif // DEBUG
			choose_right(c_url);
			purge_data();
			return;
		}
		if (capture == "left")
		{
#ifdef DEBUG
			Serial.println("Captured LEFT");
#endif // DEBUG
			choose_left(c_url);
			purge_data();
			return;
		}
		if (capture == "calibrate")
		{
#ifdef DEBUG
			Serial.println("Captured CALIBRATE");
#endif // DEBUG
			calibrate();
			purge_data();
			return;
		}
		if (capture == "resetall")
		{
#ifdef DEBUG
			Serial.println("Captured RESETALL");
#endif // DEBUG
			reset_all();
			purge_data();
			return;
		}

	}
#ifdef DEBUG
	static unsigned long last = millis();
	if (millis() - last > 5000) {
		last = millis();
		Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
	}
#endif // DEBUG
}

void purge_data() {
	c_request = false;
	capture = "";
	c_url = "";
}

void connect_wifi() {
	int tries = 0;
	WiFi.mode(WIFI_STA);
	//WiFi.setHostname("ESP_BED");
#ifdef DEBUG
	Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
#endif // DEBUG
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED) {
		/*
		if (tries > 200)
		{
			enable_ap();
			return;
		}
		*/
#ifdef DEBUG
		Serial.print(".");
#endif // DEBUG
		delay(100);
		//tries++;
	}
	// Connected!
#ifdef DEBUG
	Serial.println("");
	Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
	Serial.println("");
#endif // DEBUG
}
/*
void enable_ap() {
#ifdef DEBUG
	Serial.println("");
	Serial.println("[WIFI] Launching WiFi as Access Point");
#endif // DEBUG
	WiFi.mode(WIFI_AP);
#ifdef DEBUG
	Serial.println("[WIFI] Setting AP (Access Point)");
#endif // DEBUG
	// Remove the password parameter, if you want the AP (Access Point) to be open
	WiFi.softAP("ESP32-AP", "123456789");
#ifdef DEBUG
	Serial.printf("[WIFI] ACCESS POINT Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.softAPIP().toString().c_str());
	Serial.println("");
#endif // DEBUG
	//TODO create Webpage to configure ESP
}
*/
void server_setup() {
	server.on("/booth", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on BOOTH!");
#endif // HTTP_DEBUG
		capture = "booth";
		c_url = request->url();
		c_request = true;
		//choose_booth(request->url());
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on BOOTH!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
		});
	server.on("/right", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on RIGHT!");
#endif // HTTP_DEBUG
		capture = "right";
		c_url = request->url();
		c_request = true;
		//choose_right(request->url());
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on RIGHT!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
		});
	server.on("/left", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on LEFT!");
#endif // HTTP_DEBUG
		capture = "left";
		c_url = request->url();
		c_request = true;
		//choose_left(request->url());
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on LEFT!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
		});
	server.on("/calibrate", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on CALIBRATE!");
#endif // HTTP_DEBUG
		capture = "calibrate";
		c_url = request->url();
		c_request = true;
		//calibrate();
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on CALIBRATE!");
#endif // HTTP_DEBUG
		request->send(200, "text/plain", "ok");
		});
	server.on("/resetall", HTTP_ANY, [](AsyncWebServerRequest *request) {
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] GET request on RESETALL!");
#endif // HTTP_DEBUG
		capture = "resetall";
		c_url = request->url();
		c_request = true;
		//reset_all();
#ifdef HTTP_DEBUG
		Serial.println("[HTTP] SEND response on RESETALL!");
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
		esp_reboot();
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

void espalexa_setup() {
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

espalexa.addDevice(HEAD, espalexa_cb, EspalexaDeviceType::dimmable, 0); //getID = 0
espalexa.addDevice(FEET, espalexa_cb, EspalexaDeviceType::dimmable, 0); //getID = 1
espalexa.addDevice(CALIBRATE, espalexa_cb, EspalexaDeviceType::dimmable, 0); //getID = 2
espalexa.begin(&server);
}

void alexa_handler(int device_name, unsigned char value) {
	if (device_name == 0) {
#ifdef ESPALEXA_DEBUG
		Serial.println("[ESPALEXA] HEAD switched by Alexa");
#endif // ESPALEXA_DEBUG
		if (value > dev0.current_val)
		{
			head_up("booth");
			delay((value - dev0.current_val) * 1000);
			stop();
			dev0.current_val = value;
		}
		else
		{
			head_down("booth");
			delay(value * 1000);
			stop();
			dev0.current_val = value;
		}
	}
	if (device_name == 1) {
#ifdef ESPALEXA_DEBUG
		Serial.println("[ESPALEXA] FEET switched by Alexa");
#endif // ESPALEXA_DEBUG
		if (value > dev1.current_val)
		{
			feet_up("booth");
			delay((value - dev1.current_val) * 1000);
			stop();
			dev1.current_val = value;
		}
		else
		{
			feet_down("booth");
			delay(value * 1000);
			stop();
			dev1.current_val = value;
		}
	}
	if (device_name == 2) {
#ifdef ESPALEXA_DEBUG
		Serial.println("[ESPALEXA] CALIBRATE switched by Alexa");
#endif // ESPALEXA_DEBUG
		head_reset("booth", "head");
		feet_reset("booth", "feet");
	}

	/*
	if ((strcmp(device_name, LEFT_HEAD) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] LEFT HEAD switched by Alexa");
		//TODO action to be done when executed in all following Lamdas
	}
	if ((strcmp(device_name, RIGHT_HEAD) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] RIGHT HEAD switched by Alexa");

	}
	if ((strcmp(device_name, LEFT_FEET) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] LEFT FEET switched by Alexa");

	}
	if ((strcmp(device_name, RIGHT_FEET) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] RIGHT FEET switched by Alexa");

	}
	if ((strcmp(device_name, CALIBRATE) == 0)) {
		// this just sets a variable that the main loop() does something about
		Serial.println("[FAUXMO] CALIBRATE switched by Alexa");

	}
	*/
}

void esp_reboot() {
#ifdef DEBUG
	Serial.println("ESP going to restart in 1 second!");
#endif // DEBUG
	delay(1000);
	ESP.restart();
}


