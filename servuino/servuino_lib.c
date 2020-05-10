/*
	Copyright (C) 2012 Benny Saxen

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

void ino(int x)
{
	if (x < 1)
	{
		errorLog("Source Ino rownumber zero", x);
	}
	else
	{
		fprintf(f_ino, "%d %d\n", g_curStep + 1, x);
	}
}

void clearRW()
{
	int i;
	for (i = 0; i < g_nTotPins; i++)
	{
		x_pinRW[i] = 0;
	}
}

void interruptPinValues()
{
	int i;

	g_doInterrupt = S_NO;
	for (i = 0; i < g_nTotPins; i++)
	{
		if (g_attachedPin[i] == S_YES)
		{
			x_pinDigValue[i] = x_pinScenario[i][g_curStep];
			x_pinMode[i] = g_interruptType[i];
			g_doInterrupt = S_YES;
		}
	}
}

void logEvent(char ev[])
{
	fprintf(f_event, "+ %d ? %s\n", g_curStep, ev);
}

void logCust(char ev[])
{
	if (strstr(ev, "no-text") == NULL)
		fprintf(f_cust, "%d %s\n", g_curStep, ev);
}

void logStatus()
{
	//fprintf(s_log, "%d %s\n", g_curStep, ev);
}

int servuinoFunc(int event, int pin, int value, const char *p, unsigned char uc)
{
	int res = 999, fail = 0;
	char eventText[120];
	char custText[120];

	strcpy(custText, "no-text");

	g_curStep++;

	updateFromRegister();
	clearRW();
	if (g_ongoingInterrupt == S_NO)
		interruptPinValues();

	if (g_serialMode == S_ON)
	{
		x_pinMode[0] = RX;
		x_pinMode[1] = TX;
	}

	sprintf(eventText, "%d Unknown event: %d pin=%d value=%d", g_curStep, event, pin, value);

	if (event == S_UNIMPLEMENTED)
	{
		sprintf(eventText, "Unimplemented: %s", p);
	}
	else if (event == S_SETUP)
	{
		sprintf(eventText, "setup");
	}
	else if (event == S_LOOP)
	{
		sprintf(eventText, "servuinoLoop %d", g_curLoop);
	}
	else if (event == S_PIN_MODE_INPUT)
	{
		fail = checkRange(FAIL, "digpin", pin);
		if (fail == 0)
		{
			x_pinMode[pin] = value;
			if (value == INPUT || value == INPUT_PULLUP)
			{
				// Set pin status according to scenario when mode is INPUT
				res = x_pinScenario[pin][g_curStep];
				x_pinDigValue[pin] = res;
				writeRegister(1, R_PIN, pin, value);

				writeRegister(1, R_DDR, pin, 0);
				sprintf(eventText, "pinMode pin=%d INPUT", pin);
				sprintf(custText, "%s %d", g_custText[event][pin], pin);
			}
		}
		res = 0;
	}
	else if (event == S_PIN_MODE_OUTPUT)
	{
		fail = checkRange(FAIL, "digpin", pin);
		if (fail == 0)
		{
			x_pinMode[pin] = value;
			if (value == OUTPUT)
			{
				// Set pin status according to PORT register when mode is OUTPUT
				x_pinDigValue[pin] = readRegister(R_PORT, pin);

				writeRegister(1, R_DDR, pin, 1);
				sprintf(eventText, "pinMode pin=%d OUTPUT", pin);
				sprintf(custText, "%s %d", g_custText[event][pin], pin);
			}
		}
		res = 0;
	}
	else if (event == S_DIGITAL_WRITE_LOW || event == S_DIGITAL_WRITE_HIGH)
	{
		x_pinRW[pin] = T_WRITE;
		fail = checkRange(FAIL, "digpin", pin);
		fail = fail + checkRange(FAIL, "digval", value);
		if (fail == 0)
		{
			x_pinDigValue[pin] = value;
			writeRegister(1, R_PORT, pin, value);
			sprintf(eventText, "digitalWrite pin=%d value=%d", pin, value);
			sprintf(custText, "%s %d", g_custText[event][pin], value);
		}
		res = 0;
		if (x_pinMode[pin] != OUTPUT)
			errorLog("DigitalWrite when pin Mode is INPUT", pin);
	}
	else if (event == S_DIGITAL_READ)
	{
		x_pinRW[pin] = T_READ;
		res = x_pinScenario[pin][g_curStep];
		x_pinDigValue[pin] = res;
		writeRegister(1, R_PIN, pin, value);
		sprintf(eventText, "digitalRead pin=%d value=%d", pin, res);
		sprintf(custText, "%s %d", g_custText[event][pin], res);
		//value = getDigitalPinValue(pin,currentStep);
		if (x_pinMode[pin] != INPUT && x_pinMode[pin] != INPUT_PULLUP)
			errorLog("DigitalRead when pin Mode is OUPUT", pin);
	}
	else if (event == S_ANALOG_WRITE) //PWM pinMode OUTPUT not necessary (see arduino.com)
	{
		x_pinRW[pin] = T_WRITE;
		fail = checkRange(FAIL, "pwmpin", pin);
		fail = fail + checkRange(FAIL, "pwmval", value);
		if (fail == 0)
		{
			x_pinAnaValue[pin] = value;
			//writeRegister(pin,value);
			sprintf(eventText, "analogWrite pin=%d value=%d", pin, value);
			sprintf(custText, "%s %d", g_custText[event][pin], value);
		}
		res = 0;
	}
	else if (event == S_ANALOG_READ)
	{
		x_pinRW[pin] = T_READ;
		res = x_pinScenario[pin][g_curStep];
		x_pinAnaValue[pin] = res;
		sprintf(eventText, "analogRead pin=%d value=%d", pin - g_nDigPins, res);
		sprintf(custText, "%s %d", g_custText[event][pin], res);
	}
	else if (event == S_ANALOG_REFERENCE)
	{
		sprintf(eventText, "analogReference type=%s", p);
	}
	else if (event == S_DELAY)
	{
		sprintf(eventText, "delay %d ms", pin);
		fprintf(f_time, "+ %d %d\n", g_curStep, pin * 1000);
	}
	else if (event == S_DELAY_MS)
	{
		sprintf(eventText, "delay %d us", pin);
		fprintf(f_time, "+ %d %d\n", g_curStep, pin);
	}
	else if (event == S_ATTACH_INTERRUPT_LOW)
	{
		sprintf(eventText, "attached LOW interrupt %d", pin);
	}
	else if (event == S_ATTACH_INTERRUPT_RISING)
	{
		sprintf(eventText, "attached RISING interrupt %d", pin);
	}
	else if (event == S_ATTACH_INTERRUPT_FALLING)
	{
		sprintf(eventText, "attached FALLING interrupt %d", pin);
	}
	else if (event == S_ATTACH_INTERRUPT_CHANGE)
	{
		sprintf(eventText, "attached CHANGE interrupt %d", pin);
	}
	else if (event == S_DETACH_INTERRUPT)
	{
		g_interruptType[pin] = 0;
		sprintf(eventText, "detached interrupt %d", pin);
	}
	else if (event == S_SERIAL_BEGIN)
	{
		sprintf(eventText, "Serial.begin baud=%d", pin);
		if (g_serialMode == S_ON)
			errorLog("Serial begin already exists", g_curStep);
		g_serialMode = S_ON;
	}
	else if (event == S_SERIAL_END)
	{
		sprintf(eventText, "Serial.end");
		if (g_serialMode != S_ON)
			errorLog("Serial end without serial.begin", g_curStep);
		g_serialMode = S_OFF;
	}
	else if (event == S_SERIAL_PRINT_INT)
	{
		sprintf(eventText, "Serial.print(int) %d", pin);
		fprintf(f_serial, "%d SL [%d]\n", g_curStep, pin);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINT_INT_BASE)
	{
		sprintf(eventText, "Serial.print(int,base) %d base=%d", pin, value);
		fprintf(f_serial, "%d SL [%d]\n", g_curStep, pin);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINT_CHAR)
	{
		sprintf(eventText, "Serial.print(char) %s", p);
		fprintf(f_serial, "%d SL [%s]\n", g_curStep, p);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINT_UCHAR)
	{
		sprintf(eventText, "Serial.print(uchar) %c", uc);
		fprintf(f_serial, "%d SL [%c]\n", g_curStep, uc);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINT_STRING)
	{
		sprintf(eventText, "Serial.print(string) %s", p);
		fprintf(f_serial, "%d SL [%s]\n", g_curStep, p);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINT_SSTRING)
	{
		sprintf(eventText, "Serial.print(String) %s", p);
		fprintf(f_serial, "%d SL [%s]\n", g_curStep, p);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINTLN_INT)
	{
		sprintf(eventText, "Serial.println(int) %d", pin);
		fprintf(f_serial, "%d NL [%d]\n", g_curStep, pin);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINTLN_INT_BASE)
	{
		sprintf(eventText, "Serial.println(int,base) %d base=%d", pin, value);
		fprintf(f_serial, "%d NL [%d]\n", g_curStep, pin);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINTLN_CHAR)
	{
		sprintf(eventText, "Serial.println(char) %s", p);
		fprintf(f_serial, "%d NL [%s]\n", g_curStep, p);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINTLN_UCHAR)
	{
		sprintf(eventText, "Serial.println(uchar) %c", uc);
		fprintf(f_serial, "%d NL [%c]\n", g_curStep, *p);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINTLN_STRING)
	{
		sprintf(eventText, "Serial.println(string) %s", p);
		fprintf(f_serial, "%d NL [%s]\n", g_curStep, p);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINTLN_SSTRING)
	{
		sprintf(eventText, "Serial.println(String) %s", p);
		fprintf(f_serial, "%d NL [%s]\n", g_curStep, p);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_PRINTLN_VOID)
	{
		sprintf(eventText, "Serial.println(void)");
		fprintf(f_serial, "%d NL []\n", g_curStep);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_SERIAL_WRITE)
	{
		sprintf(eventText, "Serial.write %s", p);
		if (g_serialMode != S_ON)
			errorLog("Serial print without serial.begin", g_curStep);
	}
	else if (event == S_EEPROM_WRITE)
	{
		sprintf(eventText, "EEPROM.write address=%d value=%d", pin, value);
		if (pin > 512)
			errorLog("EEPROM write: address > 512: ", pin);
		if (pin < 0)
			errorLog("EEPROM write: address < 0: ", pin);
		if (value > 255)
			errorLog("EEPROM write: value > 255: ", value);
		if (value < 0)
			errorLog("EEPROM write: value < 0: ", value);
		res = 0;
	}
	else if (event == S_EEPROM_READ)
	{
		sprintf(eventText, "EEPROM.read address=%d value=%d", pin, value);
		if (pin > 512)
			errorLog("EEPROM read: address > 512: ", pin);
		if (pin < 0)
			errorLog("EEPROM read: address < 0: ", pin);
		if (value > 255)
			errorLog("EEPROM read: value > 255: ", value);
		if (value < 0)
			errorLog("EEPROM read: value < 0: ", value);
		res = 0;
	}

	logEvent(eventText);
	logCust(custText);
	writeStatus();

	if (g_curStep == g_simulationLength)
		stopEncoding();

	if (g_doInterrupt == S_YES && g_ongoingInterrupt == S_NO)
		interruptNow();

	return res;
}

void writeRegister(int digital, int reg, int port, int value)
{
	if (reg == R_PORT && digital == 1) // 0=LOW 1=HIGH
	{
		if (port <= 7 && port >= 0)
			cbitWrite(&PORTD, port, value);
		if (port >= 8 && port <= 15)
			cbitWrite(&PORTB, port - 8, value);
		if (port >= 16 && port <= 23)
			cbitWrite(&PORTE, port - 16, value);
		if (port >= 24 && port <= 31)
			cbitWrite(&PORTF, port - 24, value);
		if (port >= 32 && port <= 39)
			cbitWrite(&PORTK, port - 32, value);
		if (port >= 40 && port <= 47)
			cbitWrite(&PORTL, port - 40, value);
		if (port >= 48 && port <= 55)
			cbitWrite(&PORTM, port - 48, value);
	}
	if (reg == R_PORT && digital == 0)
	{
		if (port <= 7 && port >= 0)
			cbitWrite(&PORTC, port, value);
		if (port <= 15 && port >= 8)
			cbitWrite(&PORTN, port - 8, value);
	}
	//-------------------------------------------------------
	if (reg == R_DDR && digital == 1) // 0=INPUT 1=OUTPUT
	{
		if (port <= 7 && port >= 0)
			cbitWrite(&DDRD, port, value);
		if (port >= 8 && port <= 13)
			cbitWrite(&DDRB, port - 8, value);
		if (port >= 16 && port <= 23)
			cbitWrite(&DDRE, port - 16, value);
		if (port >= 24 && port <= 31)
			cbitWrite(&DDRF, port - 24, value);
		if (port >= 32 && port <= 39)
			cbitWrite(&DDRK, port - 32, value);
		if (port >= 40 && port <= 47)
			cbitWrite(&DDRL, port - 40, value);
		if (port >= 48 && port <= 55)
			cbitWrite(&DDRM, port - 48, value);
	}
	if (reg == R_DDR && digital == 0)
	{
		if (port <= 7 && port >= 0)
			cbitWrite(&DDRC, port, value);
		if (port <= 15 && port >= 8)
			cbitWrite(&DDRN, port - 8, value);
	}
	//-------------------------------------------------------
	if (reg == R_PIN && digital == 1) // 0=INPUT 1=OUTPUT
	{
		if (port <= 7 && port >= 0)
			cbitWrite(&PIND, port, value);
		if (port >= 8 && port <= 13)
			cbitWrite(&PINB, port - 8, value);
		if (port >= 16 && port <= 23)
			cbitWrite(&PINE, port - 16, value);
		if (port >= 24 && port <= 31)
			cbitWrite(&PINF, port - 24, value);
		if (port >= 32 && port <= 39)
			cbitWrite(&PINK, port - 32, value);
		if (port >= 40 && port <= 47)
			cbitWrite(&PINL, port - 40, value);
		if (port >= 48 && port <= 55)
			cbitWrite(&PINM, port - 48, value);
	}
	if (reg == R_PIN && digital == 0)
	{
		if (port <= 7 && port >= 0)
			cbitWrite(&PINC, port, value);
		if (port <= 15 && port >= 8)
			cbitWrite(&PINN, port - 8, value);
	}
}

int readRegister(int reg, int port)
{
	int value = 99, pin;

	if (reg == R_PORT && port < g_nDigPins) // 0=LOW 1=HIGH
	{
		if (port >= 0 && port <= 7)
			value = cbitRead(PORTD, port);
		if (port >= 8 && port <= 15)
			value = cbitRead(PORTB, port - 8);
		if (port >= 16 && port <= 23)
			value = cbitRead(PORTE, port - 16);
		if (port >= 24 && port <= 31)
			value = cbitRead(PORTF, port - 24);
		if (port >= 32 && port <= 39)
			value = cbitRead(PORTK, port - 32);
		if (port >= 40 && port <= 47)
			value = cbitRead(PORTL, port - 40);
		if (port >= 48 && port <= 55)
			value = cbitRead(PORTM, port - 48);
	}
	if (reg == R_PORT && port >= g_nDigPins) // Analog Pins
	{
		pin = port - g_nDigPins;
		if (pin <= 7 && pin >= 0)
			value = cbitRead(PORTC, pin);
		if (pin <= 15 && pin >= 8)
			value = cbitRead(PORTN, pin - 8); // verify
	}
	//-------------------------------------------------------
	if (reg == R_DDR && port < g_nDigPins) // 0=INPUT 1=OUTPUT
	{
		if (port <= 7 && port >= 0)
			value = cbitRead(DDRD, port);
		if (port >= 8 && port <= 15)
			value = cbitRead(DDRB, port - 8);
		if (port >= 16 && port <= 23)
			value = cbitRead(DDRE, port - 16);
		if (port >= 24 && port <= 31)
			value = cbitRead(DDRF, port - 24);
		if (port >= 32 && port <= 39)
			value = cbitRead(DDRK, port - 32);
		if (port >= 40 && port <= 47)
			value = cbitRead(DDRL, port - 40);
		if (port >= 48 && port <= 55)
			value = cbitRead(DDRM, port - 48);
	}
	if (reg == R_DDR && port >= g_nDigPins) // Analog Pins
	{
		pin = port - g_nDigPins;
		if (pin <= 7 && pin >= 0)
			value = cbitRead(DDRC, pin);
		if (pin <= 15 && pin >= 8)
			value = cbitRead(DDRN, pin - 8); // verify
	}
	//-------------------------------------------------------
	if (reg == R_PIN && port < g_nDigPins) // 0=INPUT 1=OUTPUT
	{
		if (port <= 7 && port >= 0)
			value = cbitRead(PIND, port);
		if (port >= 8 && port <= 15)
			value = cbitRead(PINB, port - 8);
		if (port >= 16 && port <= 23)
			value = cbitRead(PINE, port - 16); // verify
		if (port >= 24 && port <= 31)
			value = cbitRead(PINF, port - 24); // verify
		if (port >= 32 && port <= 39)
			value = cbitRead(PINK, port - 32); // verify
		if (port >= 40 && port <= 47)
			value = cbitRead(PINL, port - 40); // verify
		if (port >= 48 && port <= 55)
			value = cbitRead(PINM, port - 48); // verify
	}
	if (reg == R_PIN && port >= g_nDigPins) // Analog Pins
	{
		pin = port - g_nDigPins;
		if (pin <= 7 && pin >= 0)
			value = cbitRead(PINC, pin);
		if (pin <= 15 && pin >= 8)
			value = cbitRead(PINN, pin - 8); // verify
	}

	return value;
}

void updateFromRegister()
{
	int i;

	for (i = 0; i < g_nTotPins; i++)
	{
		// Pin Mode
		x_pinMode[i] = readRegister(R_DDR, i);

		// Pin Value Output
		if (x_pinMode[i] == OUTPUT)
			x_pinDigValue[i] = readRegister(R_PORT, i);

		// Pin Value Input
		if (x_pinMode[i] != OUTPUT)
			x_pinDigValue[i] = readRegister(R_PIN, i);
	}
}

void writeStatus()
{
	int i;

	fprintf(f_pinmod, "+ %d ? ", g_curStep);
	for (i = 0; i < g_nTotPins; i++)
	{
		fprintf(f_pinmod, "%d,", x_pinMode[i]);
	}
	fprintf(f_pinmod, "\n");

	fprintf(f_digval, "+ %d ? ", g_curStep);
	for (i = 0; i < g_nTotPins; i++)
	{
		fprintf(f_digval, "%d,", x_pinDigValue[i]);
	}
	fprintf(f_digval, "\n");

	fprintf(f_anaval, "+ %d ? ", g_curStep);
	for (i = 0; i < g_nTotPins; i++)
	{
		fprintf(f_anaval, "%d,", x_pinAnaValue[i]);
	}
	fprintf(f_anaval, "\n");

	fprintf(f_pinrw, "+ %d ? ", g_curStep);
	for (i = 0; i < g_nTotPins; i++)
	{
		fprintf(f_pinrw, "%d,", x_pinRW[i]);
	}
	fprintf(f_pinrw, "\n");
}

void statusLog()
{
	int x, n, i, in_out;
	char z[200];
	int tempA[MAX_PIN_ANALOG_MEGA];
	int tempD[MAX_PIN_DIGITAL_MEGA];
	int tempM[MAX_PIN_DIGITAL_MEGA];
	int pinA[MAX_PIN_ANALOG_MEGA];
	int pinD[MAX_PIN_DIGITAL_MEGA];
	int pinM[MAX_PIN_DIGITAL_MEGA];

	tempA[0] = 0;
	tempD[0] = 0;
	for (i = 0; i <= max_anaPin; i++)
	{
		tempA[i] = 0;
		pinA[i] = 0;
	}
	for (i = 0; i <= max_digPin; i++)
	{
		tempD[i] = 0;
		pinD[i] = 0;
	}

	// step,status dig pins, dig pin value, n ana pair, n PWM dig pair, ( pairs)

	// Value status of analog pins
	n = 0;
	for (i = 0; i <= max_anaPin; i++)
	{
		x = c_analogPin[i];
		if (x != 0)
		{
			n++;
			tempA[0] = n;
			tempA[n] = x;
			pinA[n] = i;
		}
	}

	// Value status of digital pins
	n = 0;
	for (i = 0; i <= max_digPin; i++)
	{
		if (g_boardType == MEGA)
			x = c_digitalPin[i];
		if (g_boardType == UNO)
			x = readRegister(R_PORT, i);
		if (x > 0)
		{
			n++;
			tempD[0] = n;
			tempD[n] = x;
			pinD[n] = i;
		}
	}

	strcpy(z, "");
	// Mode status of digital pins
	for (i = 0; i <= max_digPin; i++)
	{
		if (g_boardType == UNO)
		{
			in_out = readRegister(R_DDR, i);
			if (in_out == INPUT || in_out == INPUT_PULLUP)
			{
				in_out = INPUT;
				z[i] = 'I';
			}
			else if (in_out == OUTPUT)
			{
				z[i] = 'o';
			}
		}
		else // MEGA
		{
			if (digitalMode[i] == INPUT || in_out == INPUT_PULLUP)
			{
				in_out = INPUT;
				z[i] = 'I';
			}
			else if (digitalMode[i] == OUTPUT)
			{
				z[i] = 'o';
			}
		}

		x = digitalMode[i];
		if (in_out == INPUT || in_out == INPUT_PULLUP) // INPUT
		{
			if (x == FREE)
				z[i] = '-';
			else if (x == CHANGE)
				z[i] = 'C';
			else if (x == RISING)
				z[i] = 'R';
			else if (x == FALLING)
				z[i] = 'F';
			else if (x == LOW)
				z[i] = 'L';
			else if (x == RX)
				z[i] = 'X';
			else if (x == TX)
				z[i] = 'Y';
			//else z[i] = 'Q';
		}
		else // OUTPUT
		{
			if (x == FREE)
				z[i] = '-';
			else if (x == CHANGE)
				z[i] = 'c';
			else if (x == RISING)
				z[i] = 'r';
			else if (x == FALLING)
				z[i] = 'f';
			else if (x == LOW)
				z[i] = 'l';
			else if (x == RX)
				z[i] = 'x';
			else if (x == TX)
				z[i] = 'y';
			//else z[i] = 'q';
		}
	}
	z[i] = '\0';

	fprintf(s_log, "%d,%s,%d,%d", g_curStep, z, tempA[0], tempD[0]);
	if (tempA[0] > 0)
	{
		for (i = 1; i <= tempA[0]; i++)
			fprintf(s_log, ",%d,%d", pinA[i], tempA[i]);
	}
	if (tempD[0] > 0)
	{
		for (i = 1; i <= tempD[0]; i++)
			fprintf(s_log, ",%d,%d", pinD[i], tempD[i]);
	}
	fprintf(s_log, "\n");

	registerLog();
}

void registerLog()
{
	if (g_boardType == UNO)
	{
		setPINRegister(g_curStep);
		fprintf(r_log, "# %d,DDRB=%s,DDRC=%s,DDRD=%s,", g_curStep, int2bin(DDRB, 8), int2bin(DDRC, 8), int2bin(DDRD, 8));
		fprintf(r_log, "PORTB=%s,PORTC=%s,PORTD=%s,", int2bin(PORTB, 8), int2bin(PORTC, 8), int2bin(PORTD, 8));
		fprintf(r_log, "PINB=%s,PINC=%s,PIND=%s\n", int2bin(PINB, 8), int2bin(PINC, 8), int2bin(PIND, 8));
		fprintf(r_log, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", g_curStep, DDRB, DDRC, DDRD, PORTB, PORTC, PORTD, PINB, PINC, PIND);
	}
}

void boardInit()
{
	int i, j;

	g_nloop = 0;

	for (i = 0; i < 40; i++)
	{
		for (j = 0; j < MAX_TOTAL_PINS; j++)
		{
			strcpy(g_custText[i][j], "no-text");
		}
	}

	for (i = 0; i < MAX_READ; i++)
	{
		stepAtReadA[i] = 0;
		stepAtReadD[i] = 0;
		valueAtReadA[i] = 0;
		valueAtReadD[i] = 0;
		pinAtReadA[i] = 0;
		pinAtReadD[i] = 0;
	}

	for (i = 0; i <= max_anaPin; i++)
	{
		anaPinPos[i] = 0;
		c_analogPin[i] = 0;
		//strcpy(textAnalogRead[i],"void");
	}

	for (i = 0; i <= max_digPin; i++)
	{
		digitalMode[i] = FREE;
		digPinPos[i] = 0;
		c_digitalPin[i] = 0;
		//strcpy(textPinModeIn[i],"void");
		//strcpy(textPinModeOut[i],"void");

		//strcpy(textDigitalWriteLow[i],"void");
		//strcpy(textDigitalWriteHigh[i],"void");

		//strcpy(textAnalogWrite[i],"void");
		//strcpy(textDigitalRead[i],"void");
	}

	for (i = 0; i < max_irPin; i++)
	{
		interruptMode[i] = 0;
	}

	strcpy(interruptType[LOW], "interruptLOW");
	strcpy(interruptType[FALLING], "interruptFALLING");
	strcpy(interruptType[RISING], "interruptRISING");
	strcpy(interruptType[CHANGE], "interruptCHANGE");

	/* // Values 0 to 255    */
	/* // PWM: only pin 3,5,6,9,10,11  UNO */
	/* // PWM: only pin 2 - 13 MEGA */
}

void openFiles()
{
	// Error logging
	e_log = fopen("serv.error", "w");

	/*   // Custom Logging */
	/*   u_log = fopen("data.custom","w"); */
	/*   if(u_log == NULL) */
	/*     { */
	/*       errorLog("Unable to open data.custom",0); */
	/*     } */
	/*   fprintf(u_log,"# Servuino Custom Simulation Data Version: %s\n",g_version); */

	// Debug logging INO
	f_ino = fopen("ino.debug", "w");
	if (f_ino == NULL)
	{
		errorLog("Unable to open ino.debug", 0);
	}
	fprintf(f_ino, "# Servuino Debug Information Version: %s\n", g_version);

	// Status of pins
	f_pinmod = fopen("serv.pinmod", "w");
	if (f_pinmod == NULL)
	{
		errorLog("Unable to open data.status", 0);
	}
	fprintf(f_pinmod, "# Servuino Status Pin Mode Version: %s\n", g_version);

	// Status of pins
	f_digval = fopen("serv.digval", "w");
	if (f_digval == NULL)
	{
		errorLog("Unable to open data.status", 0);
	}
	fprintf(f_digval, "# Servuino Status Pin Digital Values Version: %s\n", g_version);

	// Status of pins
	f_anaval = fopen("serv.anaval", "w");
	if (f_anaval == NULL)
	{
		errorLog("Unable to open data.status", 0);
	}
	fprintf(f_anaval, "# Servuino Status Pin Analog Values Version: %s\n", g_version);

	// Status of pins
	f_pinrw = fopen("serv.pinrw", "w");
	if (f_pinrw == NULL)
	{
		errorLog("Unable to open data.status", 0);
	}
	fprintf(f_pinrw, "# Servuino Status Pin RW Version: %s\n", g_version);

	// Serial logging
	f_serial = fopen("serv.serial", "w");
	if (f_serial == NULL)
	{
		errorLog("Unable to open serv.serial", 0);
	}
	fprintf(f_serial, "# Servuino Serial Simulation Data Version: %s\n", g_version);

	// Serial logging
	f_time = fopen("serv.time", "w");
	if (f_time == NULL)
	{
		errorLog("Unable to open serv.time", 0);
	}
	fprintf(f_time, "# Servuino Time Delay (microseconds) Version: %s\n", g_version);

	// Register logging
	r_log = fopen("data.register", "w");
	if (r_log == NULL)
	{
		errorLog("Unable to open data.register", 0);
	}
	fprintf(r_log, "# Servuino Register Data Version: %s\n", g_version);
	// serv.event
	f_event = fopen("serv.event", "w");
	if (f_event == NULL)
	{
		errorLog("Unable to open data.register", 0);
	}
	fprintf(f_event, "# Servuino Event Log Version: %s\n", g_version);

	// serv.cust
	f_cust = fopen("serv.cust", "w");
	if (f_cust == NULL)
	{
		errorLog("Unable to open data.register", 0);
	}
	fprintf(f_cust, "# Servuino Cust Log Version: %s\n", g_version);
}

void closeFiles()
{
	fclose(f_pinmod);
	fclose(f_digval);
	fclose(f_anaval);
	fclose(f_ino);
	fclose(f_pinrw);
	fclose(e_log);
	fclose(r_log);

	fclose(f_event);
	fclose(f_cust);
	fclose(f_time);
	fclose(f_serial);
}

void errorLog(const char msg[], int x)
{
	//printf("ServuinoERROR: %s %d\n",msg,x);
	fprintf(e_log, "ServuinoERROR[step %d]: %s %d\n", g_curStep, msg, x);
}

int getAnalogPinValue(int pin, int step)
{
	int i, res = 0, limit;

	if (g_interpolation == S_YES)
	{
		limit = s_analogStep[0][pin];
		for (i = 1; i <= limit; i++)
		{
			if (step >= s_analogStep[i][pin] && step < s_analogStep[i + 1][pin])
				res = s_analogPin[i][pin];
		}
		if (step >= s_analogStep[limit][pin])
			res = s_analogPin[limit][pin];
	}
	else
	{
		limit = s_analogStep[0][pin];
		for (i = 1; i <= limit; i++)
		{
			if (step >= s_analogStep[i][pin] && step < s_analogStep[i + 1][pin])
				res = s_analogPin[i][pin];
		}
		if (step >= s_analogStep[limit][pin])
			res = s_analogPin[limit][pin];
	}

	return res;
}

int getDigitalPinValue(int pin, int step)
{
	int i, res = 0, limit;

	limit = s_digitalStep[0][pin];

	for (i = 1; i <= limit; i++)
	{
		if (step >= s_digitalStep[i][pin] && step < s_digitalStep[i + 1][pin])
			res = s_digitalPin[i][pin];
	}

	if (step >= s_digitalStep[limit][pin])
		res = s_digitalPin[limit][pin];

	return res;
}

int insDigitalPinValue(int pin, int step, int value)
{
	int i, limit, hit = 0, ok = 0;

	ok = ok + checkRange(S_OK, "digpin", pin);
	ok = ok + checkRange(S_OK, "step", step);
	ok = ok + checkRange(S_OK, "digval", value);

	if (ok == S_OK)
	{

		limit = s_digitalStep[0][pin];

		// Insert breakpoint at the end
		if (step > s_digitalStep[limit][pin])
		{
			hit = 1;
			s_digitalPin[limit + 1][pin] = value;
			s_digitalStep[limit + 1][pin] = step;
			limit++;
		}

		// Insert breakpoint at the begining
		else if (step < s_digitalStep[1][pin])
		{
			hit = limit;
			for (i = limit; i > 0; i--)
			{
				s_digitalPin[i + 1][pin] = s_digitalPin[i][pin];
				s_digitalStep[i + 1][pin] = s_digitalStep[i][pin];
			}
			s_digitalPin[1][pin] = value;
			s_digitalStep[1][pin] = step;
			limit++;
		}
		// Insert breakpoint in between
		else
		{
			for (i = 1; i <= limit; i++)
			{
				// If step in between
				if (step > s_digitalStep[i][pin] && step < s_digitalStep[i + 1][pin])
				{
					hit = i + 1; // New breakpoint index
				}
				// If breakpoint already exists - replace value
				if (step == s_digitalStep[i][pin])
				{
					hit = -1;
					s_digitalPin[i][pin] = value;
				}
			}
			// Insert breakpoint
			if (hit > 0)
			{
				for (i = limit; i >= hit; i--)
				{
					s_digitalPin[i + 1][pin] = s_digitalPin[i][pin];
					s_digitalStep[i + 1][pin] = s_digitalStep[i][pin];
				}
				s_digitalPin[hit][pin] = value;
				s_digitalStep[hit][pin] = step;
				limit++;
			}
		}
		s_digitalStep[0][pin] = limit;
	}

	if (hit == 0)
	{
		sprintf(g_temp, "Digital: Unable to insert breakpoint pin=%d step=%d value=%d", pin, step, value);
		errorLog(g_temp, 0);
	}

	return hit;
}

int insAnalogPinValue(int pin, int step, int value)
{
	int i, limit, hit = 0, ok = 0;

	ok = ok + checkRange(S_OK, "anapin", pin);
	ok = ok + checkRange(S_OK, "step", step);
	ok = ok + checkRange(S_OK, "anaval", value);

	if (ok == S_OK)
	{

		limit = s_analogStep[0][pin];

		// Insert breakpoint at the end
		if (step > s_analogStep[limit][pin])
		{
			hit = 1;
			s_analogPin[limit + 1][pin] = value;
			s_analogStep[limit + 1][pin] = step;
			limit++;
		}
		// Insert breakpoint at the begining
		else if (step < s_analogStep[1][pin])
		{
			hit = limit;
			for (i = limit; i > 0; i--)
			{
				s_analogPin[i + 1][pin] = s_analogPin[i][pin];
				s_analogStep[i + 1][pin] = s_analogStep[i][pin];
			}
			s_analogPin[1][pin] = value;
			s_analogStep[1][pin] = step;
			limit++;
		}
		// Insert breakpoint in between
		else
		{
			for (i = 1; i <= limit; i++)
			{
				// If step in between
				if (step > s_analogStep[i][pin] && step < s_analogStep[i + 1][pin])
				{
					hit = i + 1; // New breakpoint index
				}
				// If breakpoint already exists - replace value
				if (step == s_analogStep[i][pin])
				{
					hit = -1;
					s_analogPin[i][pin] = value;
				}
			}
			// Insert breakpoint
			if (hit > 0)
			{
				for (i = limit; i >= hit; i--)
				{
					s_analogPin[i + 1][pin] = s_analogPin[i][pin];
					s_analogStep[i + 1][pin] = s_analogStep[i][pin];
				}
				s_analogPin[hit][pin] = value;
				s_analogStep[hit][pin] = step;
				limit++;
			}
		}
		s_analogStep[0][pin] = limit;
	}

	if (hit == 0)
	{
		sprintf(g_temp, "Analog: Unable to insert breakpoint pin=%d step=%d value=%d", pin, step, value);
		errorLog(g_temp, 0);
	}

	return hit;
}

int delDigitalPinValue(int pin, int step)
{
	int i, limit, hit = 0, ok = 0;

	ok = ok + checkRange(S_OK, "digpin", pin);
	ok = ok + checkRange(S_OK, "step", step);

	if (ok == S_OK)
	{
		limit = s_digitalStep[0][pin];
		for (i = 1; i <= limit; i++)
		{
			if (step == s_digitalStep[i][pin])
			{
				hit = i;
			}
		}

		if (hit > 0)
		{
			s_digitalStep[0][pin]--;
			//Remove the only/last breakpoint
			if (limit == hit)
			{
				s_digitalPin[hit][pin] = 0;
				s_digitalStep[hit][pin] = 0;
			}
			else if (hit < limit)
			{
				for (i = hit; i < limit; i++)
				{
					s_digitalPin[i][pin] = s_digitalPin[i + 1][pin];
					s_digitalStep[i][pin] = s_digitalStep[i + 1][pin];
				}
			}
		}
	}

	if (hit == 0)
	{
		sprintf(g_temp, "Digital: Unable to delete breakpoint pin=%d step=%d", pin, step);
		errorLog(g_temp, 0);
	}

	return hit;
}

int delAnalogPinValue(int pin, int step)
{
	int i, limit, hit = 0, ok = 0;

	ok = ok + checkRange(S_OK, "anapin", pin);
	ok = ok + checkRange(S_OK, "step", step);

	if (ok == S_OK)
	{
		limit = s_analogStep[0][pin];
		for (i = 1; i <= limit; i++)
		{
			if (step == s_analogStep[i][pin])
			{
				hit = i;
			}
		}

		if (hit > 0)
		{
			s_analogStep[0][pin]--;
			//Remove the only/last breakpoint
			if (limit == hit)
			{
				s_analogPin[hit][pin] = 0;
				s_analogStep[hit][pin] = 0;
			}
			else if (hit < limit)
			{
				for (i = hit; i < limit; i++)
				{
					s_analogPin[i][pin] = s_analogPin[i + 1][pin];
					s_analogStep[i][pin] = s_analogStep[i + 1][pin];
				}
			}
		}
	}

	if (hit == 0)
	{
		sprintf(g_temp, "Analog: Unable to delete breakpoint pin=%d step=%d", pin, step);
		errorLog(g_temp, 0);
	}
	
	return hit;
}

int checkScenario(int now, int anadig, int pin, int step)
{
	int hit = 0, send, k;

	if (anadig == DIG)
	{
		send = s_digitalStep[0][pin];
		for (k = now + 1; k <= send; k++)
		{
			if (step == s_digitalStep[k][pin])
				hit++;
		}
	}
	else if (anadig == ANA)
	{
		send = s_analogStep[0][pin];
		for (k = now + 1; k <= send; k++)
		{
			if (step == s_digitalStep[k][pin])
				hit++;
		}
	}

	return hit;
}

void saveScenario() // Servuino input/output
{
	FILE *out = fopen("data.scen", "w");
	if (out == NULL)
	{
		errorLog("Unable to open data.scen", 0);
		return;
	}

	int i, j, k, step, ok = 0;

	for (i = 0; i <= max_digPin; i++)
	{
		for (k = 1; k <= s_digitalStep[0][i]; k++)
		{
			step = s_digitalStep[k][i];
			ok = checkScenario(k, DIG, i, step);
			if (ok == 0)
				fprintf(out, "// SCENDIGPIN %d %d %d\n", i, s_digitalStep[k][i], s_digitalPin[k][i]);
		}
	}

	for (i = 0; i <= max_anaPin; i++)
	{
		for (k = 1; k <= s_analogStep[0][i]; k++)
		{
			step = s_analogStep[k][i];
			ok = checkScenario(k, ANA, i, step);
			if (ok == 0)
				fprintf(out, "// SCENANAPIN %d %d %d\n", i, s_analogStep[k][i], s_analogPin[k][i]);
		}
	}

	fclose(out);
}

void saveScenarioExpanded() // Servuino input/output
{
	FILE *out = fopen("data.scenario", "w");
	if (out == NULL)
	{
		errorLog("Unable to open data.scenario", 0);
		return;
	}

	int i, j, k, temp;

	fprintf(out, "# Digital: ");
	for (i = 0; i <= max_digPin; i++)
	{
		if (s_digitalStep[0][i] > 0)
			fprintf(out, "%4d ", i);
	}
	fprintf(out, "\n");
	fprintf(out, "# Analog: ");
	for (i = 0; i <= max_anaPin; i++)
	{
		if (s_analogStep[0][i] > 0)
			fprintf(out, "%4d ", i);
	}
	fprintf(out, "\n");

	for (k = 1; k <= g_simulationLength; k++)
	{
		fprintf(out, "%4d          ", k);
		for (i = 0; i <= max_digPin; i++)
		{
			if (s_digitalStep[0][i] > 0)
			{
				temp = getDigitalPinValue(i, k);
				fprintf(out, "%4d ", temp);
				x_pinScenario[i][k] = temp;
			}
		}
		fprintf(out, "       ");
		for (i = 0; i <= max_anaPin; i++)
		{
			if (s_analogStep[0][i] > 0)
			{
				temp = getAnalogPinValue(i, k);
				fprintf(out, "%4d ", temp);
				x_pinScenario[i + g_nDigPins][k] = temp;
			}
		}
		fprintf(out, "\n");
	}

	fclose(out);
}

void setPINRegister(int step)
{
	int i, temp;

	for (i = 0; i <= max_digPin; i++)
	{
		temp = getDigitalPinValue(i, step);
		writeRegister(1, R_PIN, i, temp);
	}
	/*   for(i=0;i<=max_anaPin;i++) */
	/*     { */
	/*       temp = getAnalogPinValue(i,step); */
	/*       writeRegister(0,R_PIN,i,temp); */
	/*     } */
}

/* //==================================== */
/* void iLog1(const char *p, int value1) */
/* //==================================== */
/* { */
/*   fprintf(a_log,"* %d %s ? %d\n",currentStep,p,value1); */
/*   fprintf(u_log,"* %d %s ? %d\n",currentStep,p,value1); */
/*   return; */
/* } */

/* //==================================== */
/* void mLine() */
/* //==================================== */
/* { */
/*   char line[120]; */

/*   strcpy(line,"--------------------"); */
/*   fprintf(a_log,"= %d ? %s\n",currentStep,line); */
/*   fprintf(u_log,"= %d ? %s\n",currentStep,line); */
/*   return; */
/* } */
/* //==================================== */
/* void mLineText(const char *t) */
/* //==================================== */
/* { */
/*   char line[120]; */

/*   sprintf(line,"------ %s ------",t); */
/*   fprintf(a_log,"= %d ? %s\n",currentStep,line); */
/*   fprintf(u_log,"= %d ? %s\n",currentStep,line); */
/*   return; */
/* } */
/* //==================================== */
/* void mLog0(const char *p) */
/* //==================================== */
/* { */
/*   fprintf(a_log,"= %d ? %s\n",currentStep,p); */
/*   fprintf(u_log,"= %d ? %s\n",currentStep,p); */
/*   return; */
/* } */
/* //==================================== */
/* void mLog1(const char *p, int value1) */
/* //==================================== */
/* { */
/*   fprintf(a_log,"= %d ? %s %d\n",currentStep,p,value1); */
/*   fprintf(u_log,"= %d ? %s %d\n",currentStep,p,value1); */
/*   return; */
/* } */

int wCustomLog(char *in, char *out)
{
	char *q, *p;
	int pin;

	p = strstr(in, ":");
	p++;
	sscanf(p, "%d", &pin);
	p = strstr(p, "\"");
	p++;
	q = strstr(p, "\"");
	strcpy(q, "\0");
	strcpy(out, p);

	return pin;
}

void readSketchInfo()
{
	FILE *in;
	char row[120], res[40], *p, *q, value[5];
	int pin, rows = 0;

	in = fopen("sketch.ino", "r");
	if (in == NULL)
	{
		errorLog("Error: Unable to open sketch", g_curStep);
		return;
	}
	
	while (fgets(row, 120, in) != NULL)
	{
		rows++;
		//if(row[0] == '/')
		//  {
		if (p = strstr(row, "setup("))
			g_row_setup = rows;
		else if (p = strstr(row, "loop("))
			g_row_loop = rows;
		else if (p = strstr(row, "SKETCH_NAME:"))
		{
			fprintf(f_event, "#%s", row);
			q = strstr(p, ":");
			q++;
			sscanf(q, "%s", appName);
		}
		else if (p = strstr(row, "BOARD_TYPE"))
		{
			fprintf(f_event, "#%s", row);
			if (strstr(row, "UNO") != NULL)
				g_boardType = UNO;
			else if (strstr(row, "MEGA") != NULL)
				g_boardType = MEGA;
		}
		//  }
	}
	fclose(in);
}

void stopEncoding()
{
	fprintf(f_event, "# ENDOFSIM\n");
	saveScenario();
	//saveScenarioExpanded();
	closeFiles();
	exit(0);
}

void savePinStatus()
{
	int i;
	for (i = 0; i <= max_digPin; i++)
		s_digitalPin[g_curStep][i] = c_digitalPin[i];
	for (i = 0; i <= max_anaPin; i++)
		s_analogPin[g_curStep][i] = c_analogPin[i];
}

void doInterrupt(int pin, int ir, int irType, int value)
{
	if (g_ongoingInterrupt == S_YES || g_allowInterrupt == S_NO)
	{
		errorLog("Try to interrupt during ongoing interrupt", g_curStep);
		return;
	}

	g_ongoingInterrupt = S_YES;
	interrupt[ir]();
	g_ongoingInterrupt = S_NO;
}

void interruptNow()
{
	int ir, ir_1, ir_2, pin, extTrigged = S_NO;

	for (ir = 0; ir <= max_irPin; ir++)
	{

		if (attached[ir] == S_YES)
		{
			pin = inrpt[ir];

			ir_1 = getDigitalPinValue(pin, g_curStep);
			ir_2 = getDigitalPinValue(pin, g_curStep - 1);

			if (interruptMode[ir] == RISING && ir_1 == 1 && ir_2 == 0)
			{
				doInterrupt(pin, ir, RISING, 1);
			}
			else if (interruptMode[ir] == FALLING && ir_1 == 0 && ir_2 == 1)
			{
				doInterrupt(pin, ir, FALLING, 0);
			}
			else if (interruptMode[ir] == CHANGE && ir_1 != ir_2)
			{
				doInterrupt(pin, ir, CHANGE, ir_1);
			}
			else if (interruptMode[ir] == LOW && ir_1 != ir_2)
			{
				doInterrupt(pin, ir, LOW, ir_1);
			}
		}
	}
}

void readScenario()
{
	FILE *in;
	char row[120], *p, junk[20];
	int pin, step, value, i, j;
	int tmp = 0, dCount[MAX_PIN_DIGITAL_MEGA], aCount[MAX_PIN_ANALOG_MEGA];

	if (g_scenSource == 0)
		in = fopen("sketch.ino", "r");
	else if (g_scenSource == 1)
		in = fopen("data.scen", "r");

	for (i = 0; i <= max_anaPin; i++)
	{
		for (j = 0; j < SCEN_MAX; j++)
		{
			s_analogPin[j][i] = 0;
			s_analogStep[j][i] = 0;
		}
	}

	for (i = 0; i <= max_digPin; i++)
	{
		for (j = 0; j < SCEN_MAX; j++)
		{
			s_digitalPin[j][i] = 0;
			s_digitalStep[j][i] = 0;
		}
	}

	for (i = 0; i <= max_digPin; i++)
		dCount[i] = 0;
	for (i = 0; i <= max_anaPin; i++)
		aCount[i] = 0;

	if (in == NULL)
	{
		errorLog("Unable to open sketch for scenario reading", 0);
		return;
	}

	while (fgets(row, 120, in) != NULL)
	{
		if (p = strstr(row, "SCENDIGPIN"))
		{
			sscanf(p, "%s%d%d%d", junk, &pin, &step, &value);
			pin = checkRange(HEAL, "digpin", pin);
			value = checkRange(HEAL, "digval", value);
			step = checkRange(HEAL, "step", step);
			dCount[pin]++;
			tmp = dCount[pin];
			if (step < s_digitalStep[tmp - 1][pin])
				fprintf(e_log, "Error:Scenario data not given in increasing order: Digital Step=%d Pin=%d\n", step, pin);
			s_digitalStep[tmp][pin] = step;
			s_digitalPin[tmp][pin] = value;
			s_digitalStep[0][pin] = tmp;
		}
		else if (p = strstr(row, "SCENANAPIN"))
		{
			sscanf(p, "%s%d%d%d", junk, &pin, &step, &value);
			pin = checkRange(HEAL, "anapin", pin);
			value = checkRange(HEAL, "anaval", value);
			step = checkRange(HEAL, "step", step);
			aCount[pin]++;
			tmp = aCount[pin];
			if (step < s_analogStep[tmp - 1][pin])
				fprintf(e_log, "Error:Scenario data not given in increasing order: Analog Step=%d Pin=%d\n", step, pin);
			s_analogStep[tmp][pin] = step;
			s_analogPin[tmp][pin] = value;
			s_analogStep[0][pin] = tmp;
		}
	}
}

void readCustom()
{
	char row[80], res[40], *p, *q, value[5];
	int pin;

	FILE *in = fopen("sketch.ino", "r");
	if (in == NULL)
	{
		errorLog("No sketch.ino", 0);
	}

	while (fgets(row, 80, in) != NULL)
	{
		if (p = strstr(row, "PINMODE_IN:"))
		{
			pin = wCustomLog(p, res);
			//strcpy(textPinModeIn[pin],res);
			strcpy(g_custText[S_PIN_MODE_INPUT][pin], res);
		}
		else if (p = strstr(row, "PINMODE_OUT:"))
		{
			pin = wCustomLog(p, res);
			//strcpy(textPinModeOut[pin],res);
			strcpy(g_custText[S_PIN_MODE_OUTPUT][pin], res);
		}
		else if (p = strstr(row, "DIGITALWRITE_LOW:"))
		{
			pin = wCustomLog(p, res);
			//strcpy(textDigitalWriteLow[pin],res);
			strcpy(g_custText[S_DIGITAL_WRITE_LOW][pin], res);
		}
		else if (p = strstr(row, "DIGITALWRITE_HIGH:"))
		{
			pin = wCustomLog(p, res);
			//strcpy(textDigitalWriteHigh[pin],res);
			strcpy(g_custText[S_DIGITAL_WRITE_HIGH][pin], res);
		}
		else if (p = strstr(row, "ANALOGREAD:"))
		{
			pin = wCustomLog(p, res);
			//strcpy(textAnalogRead[pin],res);
			strcpy(g_custText[S_ANALOG_READ][pin], res);
		}
		else if (p = strstr(row, "DIGITALREAD:"))
		{
			pin = wCustomLog(p, res);
			//strcpy(textDigitalRead[pin],res);
			strcpy(g_custText[S_DIGITAL_READ][pin], res);
		}
		else if (p = strstr(row, "ANALOGWRITE:"))
		{
			pin = wCustomLog(p, res);
			//strcpy(textAnalogWrite[pin],res);
			strcpy(g_custText[S_ANALOG_WRITE][pin], res);
		}
	}
	fclose(in);
}
