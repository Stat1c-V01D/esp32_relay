#pragma once
void alexa_handler(const char * device_name, bool state, unsigned char value) {
	if ((strcmp(device_name, "HEAD") == 0)) {
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
	if ((strcmp(device_name, "FEET") == 0)) {
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
	if ((strcmp(device_name, "CALIBRATE") == 0)) {
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