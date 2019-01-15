#pragma once
#include <WiFiUdp.h>
#include <WiFiType.h>
#include <WiFiSTA.h>
#include <WiFiServer.h>
#include <WiFiScan.h>
#include <WiFiMulti.h>
#include <WiFiGeneric.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ETH.h>
#include <WiFi.h>

void enable_ap();

void connect_wifi() {
	int tries = 0;
	WiFi.mode(WIFI_STA);
#ifdef DEBUG
	Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
#endif // DEBUG
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED) {
		if (tries > 1000)
		{
			enable_ap();
			return;
		}
		Serial.print(".");
		delay(100);
		tries++;
	}
	Serial.println("");
	// Connected!
#ifdef DEBUG
	Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
	Serial.println("");
#endif // DEBUG
}

void enable_ap() {
	Serial.println("");
	Serial.println("[WIFI] Launching WiFi as Access Point");
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