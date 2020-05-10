#ifndef Arduino_h
#define Arduino_h

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;

#include <iomanip>
using std::setw;

#include <cstring>
using std::strcmp;
using std::strcpy;
using std::strcat;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include <sstream>
using std::stringstream;

#include "binary.h"
#include "pins.h"

// see https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE  11
#define FALLING 12
#define RISING  13

#ifdef abs
#undef abs
#endif

// undefine stdlib's abs if encountered
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define _BV(bit) (1 << (bit))

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))


#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER1C 5
#define TIMER2  6
#define TIMER2A 7
#define TIMER2B 8

#define TIMER3A 9
#define TIMER3B 10
#define TIMER3C 11
#define TIMER4A 12
#define TIMER4B 13
#define TIMER4C 14
#define TIMER4D 15
#define TIMER5A 16
#define TIMER5B 17
#define TIMER5C 18

typedef unsigned int word;
typedef bool boolean;
typedef uint8_t byte;

//typedef unsigned int uint8_t;
//typedef unsigned int uint16_t;
typedef unsigned int uint32_t;
//typedef unsigned int uint64_t;

// Wrappers
void pinModeX(int x, int pin, int mode);
void digitalWriteX(int x, int pin, int value);
int  digitalReadX(int x, int pin);
void analogWriteX(int x, int pin, int value);
int  analogReadX(int x, int pin);
void delayX(int x, int ms);
void delayMicrosecondsX(int x, int us);
void attachInterruptX(int x, int ir, void(*func)(), int mode);
void detachInterruptX(int x, int ir);

void cbitWrite(unsigned int *x, int n, int b);
int  cbitRead(int x, int n);

class String
{
	friend ostream &operator<<(ostream &, const String &);
	friend istream &operator>>(istream &, String &);

public:
	String(const char * = ""); // conversion/default constructor
	String(const String &); // copy constructor
	~String(); // destructor

	const String &operator=(const String &); // assignment operator
	const String &operator+=(const String &); // concatenation operator

	bool operator!() const; // is String empty?
	bool operator==(const String &) const; // test s1 == s2
	bool operator<(const String &) const; // test s1 < s2

	bool operator!=(const String &right) const
	{
		return !(*this == right);
	}

	bool operator>(const String &right) const
	{
		return right < *this;
	}

	bool operator<=(const String &right) const
	{
		return !(right < *this);
	}

	bool operator>=(const String &right) const
	{
		return !(*this < right);
	}

	char &operator[](int); // subscript operator (modifiable lvalue)
	char operator[](int) const; // subscript operator (rvalue)
	String operator()(int, int = 0) const; // return a substring
	int getLength() const; // return string length

	String(const int x);
	String(const int x, const int base);

	const String &operator=(string right);
	const String &operator=(const char *right);

	String operator+(const int number);
	String operator+(const char one);
	String operator+(unsigned long number);
	String operator+(String right);

	char charAt(const int n);
	int compareTo(const String s);
	String concat(const String s); 	
	bool endsWith(const String s);
	bool equals(const String s);
	bool equalsIgnoreCase(const String s);	
	void getBytes(int buf[], int *len);	
	int indexOf(char val);
	int indexOf(String val);
	int indexOf(char val, int from);
	int indexOf(String val, int from);
	int lastIndexOf(char val);
	int lastIndexOf(String val);
	int lastIndexOf(char val, int from);
	int lastIndexOf(String val, int from);
	int length();
	String replace(String sub1, String sub2);
	void setCharAt(int index, char c);
	bool startsWith(String s);
	String substring(int from);
	String substring(int from, int to);
	void toCharArray(char buf[], int *len);
	void toLowerCase();
	void toUpperCase();
	void trim();
	char *getPointer();

private:
	int lngth; // string length (not counting null terminator)
	char *sPtr; // pointer to start of pointer-based string

	void setString(const char *); // utility function
};

class serial {
public:
	void begin(int baudRate);
	void end();
	int  available();
	char read();
	int  peek();
	void flush();
	void print(int x);
	void print(int x, int base);
	void print(const char *p);
	void print(unsigned char c);
	void println(int x);
	void println(const char *p);
	void println(string p);
	void println(String p);
	void println();
	void println(unsigned char c);
	void write(char *p);

	// Wrappers
	void beginX(int z, int baudRate);
	void endX(int z);
	int  availableX(int z);
	char readX(int z);
	int  peekX(int z);
	void flushX(int z);
	void printX(int z, int x);
	void printX(int z, int x, int base);
	void printX(int z, const char *p);
	void printX(int z, unsigned char c);
	void printlnX(int z, int x);
	void printlnX(int z, const char *p);
	void printlnX(int z, string p);
	void printlnX(int z, String p);
	void printlnX(int z);
	void printlnX(int z, unsigned char c);
	void writeX(int z, char *p);
};
serial Serial, Serial1, Serial2, Serial3;

#endif
