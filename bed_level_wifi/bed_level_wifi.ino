/*
 Name:		bed_level.ino
 Created:	06.12.2018 11:03:12
 Author:	KS
*/

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "FRITZ-WLAN-2";
const char* password = "01823938742039394944";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
String side, part, action;
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
	Serial.begin(115200);
	relay_init();
	// Wi-Fi Setup
	// Connect to Wi-Fi network with SSID and password
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	// Print local IP address and start web server
	Serial.println("");
	Serial.println("WiFi connected.");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	server.begin();
}

void loop() {
	WiFiClient client = server.available();   // Listen for incoming clients
	if (client) {                             // If a new client connects,
		client_on_connect(client);
		delay(500);
	}
}

void client_on_connect(WiFiClient client) {
#ifdef DEBUG
	Serial.println("New Client.");          // print a message out in the serial port
#endif // DEBUG
	String currentLine = "";                // make a String to hold incoming data from the client
	while (client.connected()) {            // loop while the client's connected
		if (client.available()) {             // if there's bytes to read from the client,
			char c = client.read();				// read a byte, then
#ifdef DETAIL_HTTP
			Serial.write(c);	// print it out the serial monitor
#endif // DEBUG                   
			header += c;
			if (c == '\n') {                    // if the byte is a newline character
			  // if the current line is blank, you got two newline characters in a row.
			  // that's the end of the client HTTP request, so send a response:
				if (currentLine.length() == 0) {
					client.println();
					//toggle GPIOs
					decode(header);
					// Break out of the while loop
					break;
				}
				else { // if you got a newline, then clear currentLine
					currentLine = "";
				}
			}
			else if (c != '\r') {  // if you got anything else but a carriage return character,
				currentLine += c;      // add it to the end of the currentLine
			}
		}
	}
	header = "";
	client.stop();
}
void decode(String header) {
	if (header.indexOf("GET /booth") >= 0)
	{
		choose_booth(header);
	}
	else if (header.indexOf("GET /right") >= 0)
	{
		choose_right(header);
	}
	else if (header.indexOf("GET /left") >= 0)
	{
		choose_left(header);
	}
	else if (header.indexOf("GET /calibrate") >= 0)
	{
		calibrate();
	}
	else
	{
		Serial.println("Malformed HTTP Request");
	}
}

void choose_booth(String header) {
	side = "booth";
	if (header.indexOf("GET /booth/head") >= 0)
	{
		part = "head";
		if (header.indexOf("GET /booth/head/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("GET /booth/head/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("GET /booth/head/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("GET /booth/head/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("GET /booth/head/timer") >= 0)
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
		if (header.indexOf("GET /booth/feet/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("GET /booth/feet/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("GET /booth/feet/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("GET /booth/feet/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("GET /booth/feet/timer") >= 0)
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
	if (header.indexOf("GET /left/head") >= 0)
	{
		part = "head";
		if (header.indexOf("GET /left/head/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("GET /left/head/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("GET /left/head/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("GET /left/head/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("GET /left/head/timer") >= 0)
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
		if (header.indexOf("GET /left/feet/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("GET /left/feet/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("GET /left/feet/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("GET /left/feet/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("GET /left/feet/timer") >= 0)
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
	if (header.indexOf("GET /right/head") >= 0)
	{
		part = "head";
		if (header.indexOf("GET /right/head/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("GET /right/head/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("GET /right/head/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("GET /right/head/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("GET /right/head/timer") >= 0)
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
		if (header.indexOf("GET /right/feet/stop") >= 0)
		{
			action = "stop";
		}
		else if (header.indexOf("GET /right/feet/up") >= 0)
		{
			action = "up";
		}
		else if (header.indexOf("GET /right/feet/down") >= 0)
		{
			action = "down";
		}
		else if (header.indexOf("GET /right/feet/reset") >= 0)
		{
			action = "reset";
		}
		else if (header.indexOf("GET /right/feet/timer") >= 0)
		{
			//TODO
		}
#ifdef DEBUG
		Serial.println("Acting now!");
#endif // DEBUG
		gpio_act(side, part, action);
	}
}

void calibrate() {
	enable_all();
	delay(5000);
	disable_all();
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

	if (action=="stop")
	{
		stop(side,part);
	}
	if (part=="head")
	{
		if (action=="up")
		{
			head_up(side);
		}
		else if (action=="down")
		{
			head_down(side);
		}
		else if (action=="reset")
		{
			head_reset(side,part);
		}
		else
		{
			//TODO
		}
	}
	if (part =="feet")
	{
		if (action == "up")
		{
			feet_up(side);
		}
		else if (action == "down")
		{
			feet_down(side);
		}
		else if (action=="reset")
		{
			feet_reset(side,part);
		}
		else
		{
			//TODO
		}
		
	}
}

void head_up(String side) {
	if (side=="booth")
	{
		digitalWrite(relay02, HIGH);
		digitalWrite(relay06, HIGH);
		digitalWrite(relay01, LOW);
		digitalWrite(relay05, LOW);
	}
	else if (side=="right")
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
	stop(side,part);
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
void feet_reset(String side,String part) {
	feet_down(side);
	delay(5000);
	stop(side, part);
}
void stop(String side,String part) {
	if (side == "booth")
	{
		if (part=="head")
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
		if (part=="head")
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
	else if (side=="left")
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

void timeEvent(String side, String part, int time) {

}

void apmode() {
#ifdef DEBUG
	Serial.print("Setting AP (Access Point)…");
#endif // DEBUG
	// Remove the password parameter, if you want the AP (Access Point) to be open
	WiFi.softAP("ESP32-AP", "123456789");
	IPAddress IP = WiFi.softAPIP();
#ifdef DEBUG
	Serial.print("AP IP address: ");
	Serial.println(IP);
#endif // DEBUG
		//TODO

}
