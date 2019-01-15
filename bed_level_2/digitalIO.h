#pragma once
#include "defines.h"

// Assign output variables to GPIO pins
#define relay01 16
#define relay02 17
#define relay03 18
#define relay04 19

#define relay05 26
#define relay06 27
#define relay07 32
#define relay08 33

String side, part, action, event1 = "", event2 = "";
void timeEvent(String side, String part, int time);
void stop(String side, String part);
void head_up(String side);
void head_down(String side);
void feet_up(String side);
void feet_down(String side);
void head_reset(String side, String part);
void feet_reset(String side, String part);

void esp_restart() {
#ifdef DEBUG
	Serial.println("ESP going to restart in 1 second!");
#endif // DEBUG
	delay(1000);
	ESP.restart();
}

void disable_all() {
#ifdef DEBUG
	Serial.println("[COMMON] Relay disable all");
#endif // DEBUG
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
#ifdef DEBUG
	Serial.println("[COMMON] Relay enable all");
#endif // DEBUG
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
#ifdef DEBUG
	Serial.println("[INIT] Relay Initialization");
#endif // DEBUG
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

void gpio_act(String side, String part, String action) {

#ifdef DEBUG
	Serial.println("[GPIO] Settings:");
	Serial.println("------------------");
	Serial.print("[GPIO] SIDE: ");
	Serial.println(side);
	Serial.print("[GPIO] PART: ");
	Serial.println(part);
	Serial.print("[GPIO] ACTION: ");
	Serial.println(action);
	Serial.println("------------------");
#endif // DEBUG

	if (action == "stop")
	{
		stop(side, part);
		side = "";
		part = "";
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
#ifdef DEBUG
			Serial.println("[GPIO] Malformed Statement! Something is wrong:");
			Serial.print("SIDE = ");
			Serial.println(side);
			Serial.print("PART = ");
			Serial.println(part);
#endif // DEBUG
			return;
		}
		side = "";
		part = "";
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
#ifdef DEBUG
			Serial.println("[GPIO] Malformed Statement! Something is wrong:");
			Serial.print("SIDE = ");
			Serial.println(side);
			Serial.print("PART = ");
			Serial.println(part);
#endif // DEBUG
			return;
		}
		side = "";
		part = "";
	}
}
void head_up(String side) {
#ifdef DEBUG
	Serial.print("[GPIO] HEAD UP on SIDE: ");
	Serial.println(side);
#endif // DEBUG
	if (side == "booth")
	{
		digitalWrite(relay02, HIGH);
		digitalWrite(relay06, HIGH);
		digitalWrite(relay01, LOW);
		digitalWrite(relay05, LOW);
		return;
	}
	else if (side == "right")
	{
		digitalWrite(relay02, HIGH);
		digitalWrite(relay01, LOW);
		return;
	}
	else
	{
		digitalWrite(relay06, HIGH);
		digitalWrite(relay05, LOW);
		return;
	}
}
void head_down(String side) {
#ifdef DEBUG
	Serial.print("[GPIO] HEAD DOWN on SIDE: ");
	Serial.println(side);
#endif // DEBUG
	if (side == "booth")
	{
		digitalWrite(relay01, HIGH);
		digitalWrite(relay05, HIGH);
		digitalWrite(relay02, LOW);
		digitalWrite(relay06, LOW);
		return;
	}
	else if (side == "right")
	{
		digitalWrite(relay01, HIGH);
		digitalWrite(relay02, LOW);
		return;
	}
	else
	{
		digitalWrite(relay05, HIGH);
		digitalWrite(relay06, LOW);
		return;
	}
}
void head_reset(String side, String part) {
	//TODO compare with feet_reset() might be deprecated
#ifdef DEBUG
	Serial.print("[GPIO] RESET HEAD on SIDE: ");
	Serial.print(side);
	Serial.print(" with PART: ");
	Serial.println(part);
#endif // DEBUG
	head_down(side);
	delay(5000);
	stop(side, part);
}
void feet_up(String side) {
#ifdef DEBUG
	Serial.print("[GPIO] FEET UP on SIDE: ");
	Serial.println(side);
#endif // DEBUG
	if (side == "booth")
	{
		digitalWrite(relay04, HIGH);
		digitalWrite(relay08, HIGH);
		digitalWrite(relay03, LOW);
		digitalWrite(relay07, LOW);
		return;
	}
	else if (side == "right")
	{
		digitalWrite(relay04, HIGH);
		digitalWrite(relay03, LOW);
		return;
	}
	else
	{
		digitalWrite(relay08, HIGH);
		digitalWrite(relay07, LOW);
		return;
	}
}
void feet_down(String side) {
#ifdef DEBUG
	Serial.print("[GPIO] FEET DOWN on SIDE: ");
	Serial.println(side);
#endif // DEBUG
	if (side == "booth")
	{
		digitalWrite(relay03, HIGH);
		digitalWrite(relay07, HIGH);
		digitalWrite(relay04, LOW);
		digitalWrite(relay08, LOW);
		return;
	}
	else if (side == "right")
	{
		digitalWrite(relay03, HIGH);
		digitalWrite(relay04, LOW);
		return;
	}
	else
	{
		digitalWrite(relay07, HIGH);
		digitalWrite(relay08, LOW);
		return;
	}
}
void feet_reset(String side, String part) {
#ifdef DEBUG
	Serial.print("[GPIO] RESET FEET on SIDE: ");
	Serial.print(side);
	Serial.print(" with PART: ");
	Serial.println(part);
#endif // DEBUG
	feet_down(side);
	delay(5000);
	stop(side, part);
}

void stop(String side, String part) {
#ifdef DEBUG
	Serial.print("[GPIO] Stopping on SIDE: ");
	Serial.print(side);
	Serial.print(" with PART: ");
	Serial.println(part);
#endif // DEBUG
	if (side == "booth")
	{
		if (part == "head")
		{
			digitalWrite(relay01, HIGH);
			digitalWrite(relay02, HIGH);
			digitalWrite(relay05, HIGH);
			digitalWrite(relay06, HIGH);
			return;
		}
		else
		{
			digitalWrite(relay03, HIGH);
			digitalWrite(relay04, HIGH);
			digitalWrite(relay07, HIGH);
			digitalWrite(relay08, HIGH);
			return;
		}
	}
	else if (side == "right")
	{
		if (part == "head")
		{
			digitalWrite(relay01, HIGH);
			digitalWrite(relay02, HIGH);
			return;
		}
		else
		{
			digitalWrite(relay03, HIGH);
			digitalWrite(relay04, HIGH);
			return;
		}
	}
	else if (side == "left")
	{
		if (part == "head")
		{
			digitalWrite(relay05, HIGH);
			digitalWrite(relay06, HIGH);
			return;
		}
		else
		{
			digitalWrite(relay07, HIGH);
			digitalWrite(relay08, HIGH);
			return;
		}
	}
	else
	{
		disable_all();
	}
}


void calibrate() {
#ifdef DEBUG
	Serial.println("[GPIO] Calibrating...");
#endif // DEBUG
	head_reset("booth", "head");
	feet_reset("booth", "feet");
}

void timeEvent(String side, String part, int time) {
	if (part=="head")
	{
		head_reset(side, part);
	}
	else if (part=="feet")
	{
		feet_reset(side, part);
	}
#ifdef DEBUG
	Serial.println("[PRESET] TimeEvent");
	Serial.print("[PRESET] TIME: ");
#endif // DEBUG
	Serial.println(time);
	gpio_act(side, part, "up");
	delay(time);
	stop(side, part);
}