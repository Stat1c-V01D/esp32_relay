#pragma once
#include <string>
#include "digitalIO.h"
#include "defines.h"

int eventTime;
bool handshake_complete = false;

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
		else if (header.indexOf("/booth/head/time") >= 0)
		{
			//TODO
			header.replace("/booth/head/time/", "");
			eventTime = header.toInt();
			timerEvent(side, part, eventTime);
			return;
		}
#ifdef DEBUG
		Serial.println("[DECODE] Acting now!");
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
		else if (header.indexOf("/booth/feet/time") >= 0)
		{
			//TODO			
			header.replace("/booth/feet/time/", "");
			eventTime = header.toInt();
			timerEvent(side, part, eventTime);
			return;
		}
#ifdef DEBUG
		Serial.println("[DECODE] Acting now!");
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
		else if (header.indexOf("/left/head/time") >= 0)
		{
			//TODO
			header.replace("/left/head/time/", "");
			eventTime = header.toInt();
			timerEvent(side, part, eventTime);
			return;
		}
#ifdef DEBUG
		Serial.println("[DECODE] Acting now!");
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
		else if (header.indexOf("/left/feet/time") >= 0)
		{
			//TODO
			header.replace("/left/feet/time/", "");
			eventTime = header.toInt();
			timerEvent(side, part, eventTime);
			return;
		}
#ifdef DEBUG
		Serial.println("[DECODE] Acting now!");
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
		else if (header.indexOf("/right/head/time") >= 0)
		{
			//TODO
			header.replace("/right/head/time/", "");
			eventTime = header.toInt();
			timerEvent(side, part, eventTime);
			return;
		}
#ifdef DEBUG
		Serial.println("[DECODE] Acting now!");
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
		else if (header.indexOf("/right/feet/time") >= 0)
		{
			//TODO
			header.replace("/right/feet/time/", "");
			eventTime = header.toInt();
			timerEvent(side, part, eventTime);
			return;
		}
#ifdef DEBUG
		Serial.println("[DECODE] Acting now!");
#endif // DEBUG
		gpio_act(side, part, action);
	}
}

void collect_event(String header) {
	header.replace("/preset", "");
	if (event1=="")
	{
		Serial.print("[EVENT] Setting EVENT1 to: ");
		Serial.println(header);
		event1 = header;
		return;
	}
	else
	{
		Serial.print("[EVENT] Setting EVENT2 to: ");
		Serial.println(header);
		event2 = header;
	}
	handshake_complete = true;
	return;
}

void process_event(String event1, String event2) {
	if (event1.indexOf("/booth") >= 0)
	{
		Serial.println("[EVENT] Processing EVENT1 to BOOTH");
		choose_booth(event1);
	}
	else if (event1.indexOf("/left") >= 0)
	{
		Serial.println("[EVENT] Processing EVENT1 to LEFT");
		choose_left(event1);
	}
	else if (event1.indexOf("/right") >= 0)
	{
		Serial.println("[EVENT] Processing EVENT1 to RIGHT");
		choose_right(event1);
	}
	if (event2.indexOf("/booth") >= 0)
	{
		Serial.println("[EVENT] Processing EVENT1 to BOOTH");
		choose_booth(event2);
	}
	else if (event2.indexOf("/left") >= 0)
	{
		Serial.println("[EVENT] Processing EVENT2 to LEFT");
		choose_left(event2);
	}
	else if (event2.indexOf("/right") >= 0)
	{
		Serial.println("[EVENT] Processing EVENT2 to RIGHT");
		choose_right(event2);
	}
	event1 = "";
	event2 = "";
}