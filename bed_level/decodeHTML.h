#pragma once
#include <string>
#include "digitalIO.h"

#define DEBUG

int eventTime;

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
		else if (header.indexOf("/booth/feet/time") >= 0)
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
		else if (header.indexOf("/left/head/time") >= 0)
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
		else if (header.indexOf("/left/feet/time") >= 0)
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
		else if (header.indexOf("/right/head/time") >= 0)
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
		else if (header.indexOf("/right/feet/time") >= 0)
		{
			//TODO
		}
#ifdef DEBUG
		Serial.println("Acting now!");
#endif // DEBUG
		gpio_act(side, part, action);
	}
}