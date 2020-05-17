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


//------ Digital I/O -----------------------

void pinMode(int pin,int mode)
{
	if (mode == INPUT || mode == INPUT_PULLUP)
		servuinoFunc(S_PIN_MODE_INPUT, pin, mode, NULL, 0);
	if (mode == OUTPUT)
		servuinoFunc(S_PIN_MODE_OUTPUT, pin, mode, NULL, 0);
}

void digitalWrite(int pin, int value)
{
	if (value == LOW)
		servuinoFunc(S_DIGITAL_WRITE_LOW, pin, value, NULL, 0);
	else
		servuinoFunc(S_DIGITAL_WRITE_HIGH, pin, value, NULL, 0);
}

int digitalRead(int pin)
{
	return servuinoFunc(S_DIGITAL_READ, pin, 0, NULL, 0);
}

void analogWrite(int pin, int value) //PWM
{
	servuinoFunc(S_ANALOG_WRITE, pin, value, NULL, 0);
}

int analogRead(int pin)
{
	pin = g_nDigPins + pin;
	return servuinoFunc(S_ANALOG_READ, pin, 0, NULL, 0);
}

//------ Advanced I/O ----------------------
void tone(int pin, unsigned int freq)
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "tone()", 0);
}

void tone(int pin, unsigned int freq, unsigned long duration)
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "tone()", 0);
}

void noTone(int pin)
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "noTone()", 0);
}

void shiftOut(int dataPin, int clockPin, int bitOrder, int value)
{
	//bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST.
	servuinoFunc(S_UNIMPLEMENTED, 0 , 0, "shiftOut()", 0);
}

int shiftIn(int dataPin, int clockPin, int bitOrder)
{
	//bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST.
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "shiftIn()", 0);
	return 0;
}

unsigned long pulseIn(int pin, int value)
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "pulseIn()", 0);
	return 0;
}

unsigned long pulseIn(int pin, int value, unsigned long timeout)
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "pulseIn()", 0);
	return 0;
}

//------ Time ------------------------------

unsigned long millis()
{
	return g_curStep*100;
}

unsigned long micros()
{
	return g_curStep*100000;
}

void delay(int ms)
{
	servuinoFunc(S_DELAY, ms, 0, NULL, 0);
}

void delayMicroseconds(int us)
{
	servuinoFunc(S_DELAY_MS, us, 0, NULL, 0);
}

//------ Math ------------------------------
/*
void test_math()
{
	double r,x,z;
	int y;
	y = min(1,2);
	y = max(1,2);
	y = abs(1);
	r = pow(x,z);
	r = sqrt(y);
}
*/

/*
double sq(double x)
{
	return sqrt(x);
}
*/

int map(int x, int fromLow, int fromHigh, int toLow, int toHigh)
{
	int y;
	y = (float) (x-fromLow)/(fromHigh - fromLow)*(toHigh - toLow) + toLow;
	//printf("%d %d\n", x, y);
	return y;
}

/*
int constrain(int x, int min, int max)
{
	if (x > max) return max;
	if (x < min) return min;
	return x;
}
*/

//------ Trigonometry ----------------------
/*
void test_trigonometry()
{
	double x;
	x = sin(1);
	x = cos(1);
	x = tan(1);
}
*/

//------ Random Numbers --------------------
void randomSeed(int seed)
{
	srand(seed);
}

long random(long upperLimit)
{
	long x = RAND_MAX/upperLimit;
	x = (long)rand()/x;
	return x;
}

long random(long lowerLimit, long upperLimit)
{
	long interval, temp = 0;
	if (lowerLimit<upperLimit)
	{
		interval = upperLimit - lowerLimit;
		temp = lowerLimit + random(interval);
	}
	return temp;
}


//------ Bits and Bytes --------------------

unsigned char clowByte(int x)
{
	return x & 0xff;
}

unsigned char chighByte(int x)
{
	unsigned char y;
	x = x&0xff00;
	y = x >> 8;
	return y;
}

void cbitSet(unsigned int *x, int n)
{
	int mask = 1 << n;
	*x = *x | mask;
}

void cbitClear(unsigned int *x, int n)
{
	int mask = 1 << n;
	*x = *x & ~mask;
}

int cbit(int n)
{
	int res;
	//for (i=0;i<n;i++) res = res*2;
	res = 1 << n;
	return res;
}

int cbitRead(int x, int n)
{
	int bit;
	//printf("cbitRead: x=%d n=%d PORTB=%d\n", x, n, PORTB);
	bit = x >> n;
	bit = bit & 0x0001;
	//printf("cbitRead: x=%d n=%d PORTB=%d bit=%d\n", x, n, PORTB, bit);
	return bit;
}

void cbitWrite(unsigned int *x, int n, int b)
{
	//printf("cbitWrite: %d %d %d PORTB=%d\n",*x,n,b,PORTB);
	if (b == 0) cbitClear(x, n);
	else cbitSet(x, n);
}

//------ External Interrupts ---------------

void attachInterrupt(int ir,void(*func)(),int mode)
{
	int pin, ok = S_NOK;

	ok = checkRange(S_OK, "interrupt", ir);

	if (ok == S_OK)
	{
		pin = inrpt[ir];

		switch (mode)
		{
		case LOW:
			servuinoFunc(S_ATTACH_INTERRUPT_LOW, pin, mode, NULL, 0);
			break;
		case RISING:
			servuinoFunc(S_ATTACH_INTERRUPT_RISING, pin, mode, NULL, 0);
			break;
		case FALLING:
			servuinoFunc(S_ATTACH_INTERRUPT_FALLING, pin, mode, NULL, 0);
			break;
		case CHANGE:
			servuinoFunc(S_ATTACH_INTERRUPT_CHANGE, pin, mode, NULL, 0);
			break;
		}
		
		interruptMode[ir] = mode;
		attached[ir]      = S_YES;
		interrupt[ir]     = func;

		//interrupt[ir](); ????

		g_attachedPin[pin]  = S_YES;
		g_interruptType[pin]= mode;

		digitalMode[pin] = mode;
	}
	else
	{
		errorLog("attachInterruptERROR",ir);
	}
}

void attachInterruptX(int x, int ir, void(*func)(), int mode)
{
	ino(x);
	attachInterrupt(ir, func, mode);
}

void detachInterrupt(int ir)
{
	int ok = S_NOK, pin;
	ok = checkRange(S_OK, "interrupt", ir);
	if (ok == S_OK)
	{
		//interrupt[ir]    = NULL;
		attached[ir]       = S_NO;
		pin                = inrpt[ir];
		g_attachedPin[pin] = S_NO;
		digitalMode[pin]   = INPUT;
	}
	
	servuinoFunc(S_DETACH_INTERRUPT, pin, 0, NULL, 0);
}

void detachInterruptX(int x, int ir)
{
	ino(x);
	detachInterrupt(ir);
}

int digitalPinToInterrupt(int pin)
{
	// see https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
	
	if (g_boardType == MEGA) // Mega2560
	{
		switch (pin)
		{
			case 2: return 0;
			case 3: return 1;
			case 21: return 2;
			case 20: return 3;
			case 19: return 4;
			case 18: return 5;
		}
	}
	/*
	else if (g_boardType == B_32u4) // 32u4 based (e.g Leonardo, Micro)
	{
		switch (pin)
		{
			case 3: return 0;
			case 2: return 1;
			case 0: return 2;
			case 1: return 3;
			case 7: return 4;
		}
	}
	else if (g_boardType == DUE) // Uno WiFiRev.2, Due, Zero, MKR Family and 101
	{
		// interrupt number = pin number
		return pin;
	}
	*/
	else // UNO, Ethernet (default)
	{
		switch (pin)
		{
			case 2: return 0;
			case 3: return 1;
		}
	}
	
	// Invalid pin
	return -1;
}


//------ Interrupts ------------------------

void interrupts()
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "interrupts()", 0);
}

void noInterrupts()
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "noInterrupts()", 0);
}


//------ Communication ---------------------

void serial::begin(int baudRate)
{
	digitalMode[0] = RX;
	digitalMode[1] = TX;

	servuinoFunc(S_SERIAL_BEGIN, baudRate, 0, NULL, 0);
}

void serial::beginX(int z, int baudRate)
{
	ino(z);
	begin(baudRate);
}

void serial::end()
{
	digitalMode[0] = FREE;
	digitalMode[1] = FREE;

	servuinoFunc(S_SERIAL_END, 0, 0, NULL, 0);
}
void serial::endX(int z)
{
	ino(z);
	end();
}

int serial::available() // returns the number of bytes available to read
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "Serial.available()", 0);
	return 1;
}
int serial::availableX(int z)
{
	ino(z);
	return available();
}

char serial::read() // the first byte of incoming serial data available (or -1 if no data is available)
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "Serial.read()", 0);
	return -1;
}

int serial::peek()
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "Serial.peek()", 0);
	return -1;
}

int serial::peekX(int z)
{
	ino(z);
	return peek();
}

void serial::flush()
{
	servuinoFunc(S_UNIMPLEMENTED, 0, 0, "Serial.flush()", 0);
}

void serial::flushX(int z)
{
	ino(z);
	flush();
}

void serial::print(int x)
{
	servuinoFunc(S_SERIAL_PRINT_INT, x, 0, NULL, 0);
}

void serial::printX(int z, int x)
{
	ino(z);
	print(x);
}

void serial::print(float x)
{
	logprintln(x);
}

void serial::printX(int z, float x)
{
	ino(z);
	print(x);
}

void serial::print(double x)
{
	logprintln(x);
}

void serial::printX(int z, double x)
{
	ino(z);
	print(x);
}

void serial::print(int x, int base)
{
	servuinoFunc(S_SERIAL_PRINT_INT_BASE, x, base, NULL, 0);
}
void serial::printX(int z, int x, int base)
{
	ino(z);
	print(x, base);
}

void serial::print(const char *p)
{
	servuinoFunc(S_SERIAL_PRINT_CHAR, 0, 0, p, 0);
}

void serial::printX(int z, const char *p)
{
	ino(z);
	print(p);
}

void serial::print(unsigned char uc)
{
	servuinoFunc(S_SERIAL_PRINT_UCHAR, 0 , 0, NULL, uc);
}

void serial::printX(int z, unsigned char uc)
{
	ino(z);
	print(uc);
}

void serial::println(int x)
{
	servuinoFunc(S_SERIAL_PRINTLN_INT, x, 0, NULL, 0);
}

void serial::printlnX(int z, int x)
{
	ino(z);
	println(x);
}

void serial::println(float x)
{
	logprintln(x);
}

void serial::printlnX(int z, float x)
{
	ino(z);
	println(x);
}

void serial::println(double x)
{
	logprintln(x);
}

void serial::printlnX(int z, double x)
{
	ino(z);
	println(x);
}

void serial::println(const char *p)
{
	servuinoFunc(S_SERIAL_PRINTLN_CHAR, 0, 0, p, 0);
}
void serial::printlnX(int z, const char *p)
{
	ino(z);
	println(p);
}

void serial::println(string s)
{
	const char *p;
	p = s.c_str();
	servuinoFunc(S_SERIAL_PRINTLN_STRING, 0, 0, p, 0);
}

void serial::printlnX(int z, string s)
{
	ino(z);
	println(s);
}

void serial::println(String s)
{
	const char *p;
	p = s.getPointer();
	servuinoFunc(S_SERIAL_PRINTLN_SSTRING, 0, 0, p, 0);
}

void serial::printlnX(int z, String s)
{
	ino(z);
	println(s);
}

void serial::println()
{
	servuinoFunc(S_SERIAL_PRINTLN_VOID, 0, 0, NULL, 0);
}
void serial::printlnX(int z)
{
	ino(z);
	println();
}

void serial::println(unsigned char uc)
{
	servuinoFunc(S_SERIAL_PRINTLN_UCHAR, 0, 0, NULL, uc);
}

void serial::printlnX(int z, unsigned char uc)
{
	ino(z);
	println(uc);
}

void serial::write(char *p)
{
	servuinoFunc(S_SERIAL_WRITE, 0, 0, p, 0);
}

void serial::writeX(int z, char *p)
{
	ino(z);
	println(p);
}

//==========================================
// String
//==========================================

// conversion (and default) constructor converts char * to String
String::String(const char *s)
	: lngth((s != 0) ? strlen(s) : 0)
{
	//cout << "Conversion (and default) constructor: " << s << endl;
	setString(s); // call utility function
}

// copy constructor
String::String(const String &copy)
	: lngth(copy.lngth)
{
	//cout << "Copy constructor: " << copy.sPtr << endl;
	setString(copy.sPtr); // call utility function
}

// Destructor
String::~String()
{
	//cout << "Destructor: " << sPtr << endl;
	delete [] sPtr; // release pointer-based string memory
}

// overloaded = operator; avoids self assignment
const String &String::operator=(const String &right)
{
	//cout << "operator= called" << endl;

	if (&right != this) // avoid self assignment
	{
		delete [] sPtr; // prevents memory leak
		lngth = right.lngth; // new String length
		setString(right.sPtr); // call utility function
	}
	else
		cout << "Attempted assignment of a String to itself" << endl;

	return *this;
}

// concatenate right operand to this object and store in this object
const String &String::operator+=(const String &right)
{
	size_t newLength = lngth + right.lngth; // new length
	char *tempPtr = new char[newLength + 1]; // create memory

	strcpy(tempPtr, sPtr); // copy sPtr
	strcpy(tempPtr + lngth, right.sPtr); // copy right.sPtr

	delete [] sPtr; // reclaim old space
	sPtr = tempPtr; // assign new array to sPtr
	lngth = newLength; // assign new length to length
	return *this; // enables cascaded calls
}

// is this String empty?
bool String::operator!() const
{
	return lngth == 0;
}

// Is this String equal to right String?
bool String::operator==(const String &right) const
{
	return strcmp(sPtr, right.sPtr) == 0;
}

// Is this String less than right String?
bool String::operator<(const String &right) const
{
	return strcmp(sPtr, right.sPtr) < 0;
}

// return reference to character in String as a modifiable lvalue
char &String::operator[](int subscript)
{
	// test for subscript out of range
	if (subscript < 0 || subscript >= lngth)
	{
		cerr << "Error: Subscript " << subscript << " out of range" << endl;
		exit(1); // terminate program
	}

	return sPtr[subscript]; // non-const return; modifiable lvalue
}

// return reference to character in String as rvalue
char String::operator[](int subscript) const
{
	// test for subscript out of range
	if (subscript < 0 || subscript >= lngth)
	{
		cerr << "Error: Subscript " << subscript << " out of range" << endl;
		exit(1); // terminate program
	}

	return sPtr[subscript]; // returns copy of this element
}

// return a substring beginning at index and of length subLength
String String::operator()(int index, int subLength) const
{
	// if index is out of range or substring length < 0,
	// return an empty String object
	if (index < 0 || index >= lngth || subLength < 0) 
		return ""; // converted to a String object automatically

	// determine length of substring
	int len;

	if (subLength == 0 || index + subLength > lngth)
		len = lngth - index;
	else
		len = subLength;

	// allocate temporary array for substring and
	// terminating null character
	char *tempPtr = new char[len + 1];

	// copy substring into char array and terminate string
	strncpy(tempPtr, &sPtr[index], len);
	tempPtr[len] = '\0';

	// create temporary String object containing the substring
	String tempString(tempPtr);
	delete [] tempPtr;
	return tempString;
}

int String::getLength() const
{
	return lngth;
}

void String::setString(const char *string2)
{
	sPtr = new char[lngth + 1];

	if (string2 != 0)
		strcpy(sPtr, string2);
	else
		sPtr[0] = '\0';
}

ostream &operator<<(ostream &output, const String &s)
{
	output << s.sPtr;
	return output;
}

istream &operator>>(istream &input, String &s)
{
	char temp[100];
	input >> setw(100) >> temp;
	s = temp;
	return input;
}

String String::operator+(String right)
{
	String temp;
	size_t newLength = lngth + right.lngth;
	char *tempPtr = new char[newLength + 1];
	strcpy(tempPtr, sPtr);
	strcpy(tempPtr + lngth, right.sPtr);
	temp.sPtr = tempPtr;
	temp.lngth = newLength;
	return temp;
}

String String::operator+(const int number)
{
	String temp;
	stringstream ss;
	String right;
	ss << number;
	right = ss.str();
	size_t newLength = lngth + right.lngth;
	char *tempPtr = new char[newLength + 1];
	strcpy(tempPtr, sPtr);
	strcpy(tempPtr + lngth, right.sPtr);
	temp.sPtr = tempPtr;
	temp.lngth = newLength;
	return temp;
}

String String::operator+(unsigned long number)
{
	String temp;
	stringstream ss;
	String right;
	ss << number;
	right = ss.str();
	size_t newLength = lngth + right.lngth;
	char *tempPtr = new char[newLength + 1];
	strcpy(tempPtr, sPtr);
	strcpy(tempPtr + lngth, right.sPtr);
	temp.sPtr = tempPtr;
	temp.lngth = newLength;
	return temp;
}

String String::operator+(const char one)
{
	String temp;
	stringstream ss;
	String right;
	ss << one;
	right = ss.str();
	size_t newLength = lngth + right.lngth;
	char *tempPtr = new char[newLength + 1];
	strcpy(tempPtr, sPtr);
	strcpy(tempPtr + lngth, right.sPtr);
	temp.sPtr = tempPtr;
	temp.lngth = newLength;
	return temp;
}

const String &String::operator=(string right)
{
	char *p;
	lngth = right.length();
	p = (char *) right.c_str(); 
	setString(p); 
	return *this;
}

const String &String::operator=(const char *right)
{
	const char *p;
	lngth = strlen(right);
	p = right; 
	setString(p);
	return *this;
}

String::String(const int x)
{
	char p[10];
	sprintf(p, "%d", x);
	lngth = strlen(p);
	setString(p);
}

String::String(const int x, const int base)
{
	char p[10] = "";
	// FIXME: print as specified
	switch (base)
	{
	case BIN:
		sprintf(p, "BIN%d", x);
		break;
	case DEC:
		sprintf(p, "DEC%d", x);
		break;
	case HEX:
		sprintf(p, "HEX%d", x);
		break;
	}
	lngth = strlen(p);
	setString(p);
}

char String::charAt(const int n)
{
	char res;
	res = sPtr[n];
	return res;
}

int String::compareTo(const String s)
{
	int res = 0;
	if (*this > s) res = -1;
	else if (*this < s) res = 1;
	return res;
}

String String::concat(const String s)
{
	*this += s;
	return *this;
}

bool String::endsWith(const String s)
{
	bool res;
	int pos;
	string p(sPtr);
	pos = lngth - s.lngth;
	res = p.compare(pos,s.lngth,s.sPtr);
	return res;
}

bool String::equals(const String s)
{
	return strcmp(sPtr, s.sPtr) == 0;
}

bool String::equalsIgnoreCase(const String s)
{
	// FIXME: Not implemented
	return strcmp(sPtr, s.sPtr) == 0;
}

void String::getBytes(int buf[], int *len)
{
	char *p = sPtr;
	for (int i = 0; i < lngth; i++)
	{
		buf[i] = (int)*p;
		p++;
	}
	*len = lngth;
}

int String::indexOf(char val)
{
	// FIXME: Not implemented
	return -1;
}

int String::indexOf(String val)
{
	// FIXME: Not implemented
	return -1;
}

int String::indexOf(char val, int from)
{
	// FIXME: Not implemented
	return -1;
}

int String::indexOf(String val, int from)
{
	// FIXME: Not implemented
	return -1;
}

int String::lastIndexOf(char val)
{
	// FIXME: Not implemented
	return -1;
}

int String::lastIndexOf(String val)
{
	// FIXME: Not implemented
	return -1;
}

int String::lastIndexOf(char val, int from)
{
	// FIXME: Not implemented
	return -1;
}

int String::lastIndexOf(String val, int from)
{
	// FIXME: Not implemented
	return -1;
}

int String::length()
{
	return lngth;
}

String String::replace(String sub1, String sub2)
{
	// FIXME: Not implemented
	String res;
	return res;
}

void String::setCharAt(int index, char c)
{
	// FIXME: Not implemented
}

bool String::startsWith(String s)
{
	// FIXME: Not implemented
	bool res;
	return res;
}

String String::substring(int from)
{
	// FIXME: Not implemented
	String res;
	return res;
}

String String::substring(int from, int to)
{
	// FIXME: Not implemented
	String res;
	return res;
}

void String::toCharArray(char buf[], int *len)
{
	// FIXME: Not implemented
}

void String::toLowerCase()
{
	// FIXME: Not implemented
}

void String::toUpperCase()
{
	// FIXME: Not implemented
}

void String::trim()
{
	// FIXME: Not implemented
}

char* String::getPointer()
{
	return sPtr;
}

//=====================================
// Function Wrappers
//=====================================

void pinModeX(int x, int pin, int mode)
{
	ino(x);
	pinMode(pin, mode);
}

void digitalWriteX(int x, int pin, int value)
{
	ino(x);
	digitalWrite(pin, value);
}

int digitalReadX(int x, int pin)
{
	ino(x);
	return digitalRead(pin);
}

void analogWriteX(int x, int pin, int value)
{
	ino(x);
	analogWrite(pin, value);
}

int analogReadX(int x, int pin)
{
	ino(x);
	return analogRead(pin);
}

void delayX(int x, int ms)
{
	ino(x);
	delay(ms);
}

void delayMicrosecondsX(int x, int us)
{
	ino(x);
	delayMicroseconds(us);
}
