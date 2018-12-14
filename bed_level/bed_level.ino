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
#include "fauxmoESP.h"
#include "digitalIO.h"
#include "decodeHTML.h"

AsyncWebServer server(80);
AsyncWebServerRequest *request;
AsyncWebServerResponse *response;
BluetoothSerial SerialBT;
fauxmoESP fauxmo;

// the setup function runs once when you press reset or power the board
void setup() {

}

// the loop function runs over and over again until power down or reset
void loop() {

}