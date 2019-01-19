/*
 Name:		bed_level_wifi.ino
 Created:	12.12.2018 14:15:48
 Author:	KS
*/
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <WiFi.h>
#include <fauxmoESP.h>
#include "digitalIO.h"
#include "decodeHTTP.h"
#include "defines.h"

#define WIFI_SSID "Linksyshome"
#define WIFI_PASS "Erika280531"

AsyncWebServer server(80);
AsyncWebServerRequest *request;
fauxmoESP fauxmo;

bool c_request = false;
String capture ="",c_url="";

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
		if (capture=="booth")
		{
#ifdef DEBUG
			Serial.println("Captured BOOTH");
#endif // DEBUG
			choose_booth(c_url);
			purge_data();
			return;
		}
		if (capture=="right")
		{
#ifdef DEBUG
			Serial.println("Captured RIGHT");
#endif // DEBUG
			choose_right(c_url);
			purge_data();
			return;
		}
		if (capture=="left")
		{
#ifdef DEBUG
			Serial.println("Captured LEFT");
#endif // DEBUG
			choose_left(c_url);
			purge_data();
			return;
		}
		if (capture=="calibrate")
		{
#ifdef DEBUG
			Serial.println("Captured CALIBRATE");
#endif // DEBUG
			calibrate();
			purge_data();
			return;
		}
		if (capture=="resetall")
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
		if (tries > 200)
		{
			enable_ap();
			return;
		}
#ifdef DEBUG
		Serial.print(".");
#endif // DEBUG
		delay(100);
		tries++;
	}
	// Connected!
#ifdef DEBUG
	Serial.println("");
	Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
	Serial.println("");
#endif // DEBUG
}

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

void fauxmo_setup() {
	//TODO create setup for Fauxmo Device(s)
	/*
#define LEFT_HEAD "Bett links Kopf"
#define RIGHT_HEAD "Bett rechts Kopf"
#define LEFT_FEET "Bett links Fuﬂ"
#define RIGHT_FEET "Bett rechts Fuﬂ"
#define CALIBRATE "Bett Kalibrieren"

	fauxmo.addDevice(LEFT_HEAD);
	fauxmo.addDevice(RIGHT_HEAD);
	fauxmo.addDevice(LEFT_FEET);
	fauxmo.addDevice(RIGHT_FEET);
	fauxmo.addDevice(CALIBRATE);
	fauxmo.enable(true);
	*/
#define HEAD "Bett Kopf"
#define FEET "Bett Fuﬂ"
#define CALIBRATE "Bett Kalibrieren"

	fauxmo.addDevice(HEAD);
	fauxmo.addDevice(FEET);
	fauxmo.addDevice(CALIBRATE);
	fauxmo.enable(true);
}

void alexa_handler(const char * device_name, bool state, unsigned char value) {
	if ((strcmp(device_name, HEAD) == 0)) {
#ifdef FAUXMO_DEBUG
		Serial.println("[FAUXMO] HEAD switched by Alexa");
#endif // FAUXMO_DEBUG
		if (state)
		{
			head_up("booth");
			delay(value * 1000);
			stop();
		}
		else
		{
			head_down("booth");
			delay(value * 1000);
			stop();
		}
	}
	if ((strcmp(device_name, FEET) == 0)) {
#ifdef FAUXMO_DEBUG
		Serial.println("[FAUXMO] FEET switched by Alexa");
#endif // FAUXMO_DEBUG
		if (state)
		{
			feet_up("booth");
			delay(value * 1000);
			stop();
		}
		else
		{
			feet_down("booth");
			delay(value * 1000);
			stop();
		}
	}
	if ((strcmp(device_name, CALIBRATE) == 0)) {
#ifdef FAUXMO_DEBUG
		Serial.println("[FAUXMO] CALIBRATE switched by Alexa");
#endif // FAUXMO_DEBUG
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