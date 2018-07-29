// http://tronixstuff.com/2014/12/01/tutorial-using-ds1307-and-ds3231-real-time-clock-modules-with-arduino/
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
	return((val / 10 * 16) + (val % 10));
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
	return((val / 16 * 10) + (val % 16));
}
void setup()
{
	Wire.begin();
	Serial.begin(9600);
	//// **** Para ajustar la hora inicial:
	// DS3231 seconds, minutes, hours, day, date, month, year
	setDS3231time(00,11,19,4,30,8,17);
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
	dayOfMonth, byte month, byte year)
{
	// sets time and date data to DS3231
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0); // set next input to start at the seconds register
	Wire.write(decToBcd(second)); // set seconds
	Wire.write(decToBcd(minute)); // set minutes
	Wire.write(decToBcd(hour)); // set hours
	Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
	Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
	Wire.write(decToBcd(month)); // set month
	Wire.write(decToBcd(year)); // set year (0 to 99)
	Wire.endTransmission();
}
void readDS3231time(byte *second,
	byte *minute,
	byte *hour,
	byte *dayOfWeek,
	byte *dayOfMonth,
	byte *month,
	byte *year)
{
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0); // set DS3231 register pointer to 00h
	Wire.endTransmission();
	Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
	// request seven bytes of data from DS3231 starting from register 00h
	*second = bcdToDec(Wire.read() & 0x7f);
	*minute = bcdToDec(Wire.read());
	*hour = bcdToDec(Wire.read() & 0x3f);
	*dayOfWeek = bcdToDec(Wire.read());
	*dayOfMonth = bcdToDec(Wire.read());
	*month = bcdToDec(Wire.read());
	*year = bcdToDec(Wire.read());
}
void displayTime()
{
	byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
	// retrieve data from DS3231
	readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
		&year);
	// send it to the serial monitor
	Serial.print(hour, DEC);
	// convert the byte variable to a decimal number when displayed
	Serial.print(":");
	if (minute<10)
	{
		Serial.print("0");
	}
	Serial.print(minute, DEC);
	Serial.print(":");
	if (second<10)
	{
		Serial.print("0");
	}
	Serial.print(second, DEC);
	Serial.print(" ");
	Serial.print(dayOfMonth, DEC);
	Serial.print("/");
	Serial.print(month, DEC);
	Serial.print("/");
	Serial.print(year, DEC);
	Serial.print(" Day of week: ");
	switch (dayOfWeek) {
	case 1:
		Serial.println("Sunday");
		break;
	case 2:
		Serial.println("Monday");
		break;
	case 3:
		Serial.println("Tuesday");
		break;
	case 4:
		Serial.println("Wednesday");
		break;
	case 5:
		Serial.println("Thursday");
		break;
	case 6:
		Serial.println("Friday");
		break;
	case 7:
		Serial.println("Saturday");
		break;
	}
}
void loop()
{
	displayTime(); // display the real-time clock data on the Serial Monitor,
	delay(1000); // every second
}

/////////////////////////////////////////////////////////////////////////////////////////
/*
* Arduino DS3231 Real Time Clock Module Tutorial
*
* Crated by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
* DS3231 Library made by Henning Karlsen which can be found and downloaded from his website, www.rinkydinkelectronics.com.
*
*/
//#include <DS3231.h>
//#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
//
//DS3231  rtc(SDA, SCL);
//LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 
//
//void setup() {
//	rtc.begin(); // Initialize the rtc object
//	lcd.begin(16, 2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
//}
//
//void loop() {
//	lcd.setCursor(0, 0);
//	lcd.print("Time:  ");
//	lcd.print(rtc.getTimeStr());
//
//	lcd.setCursor(0, 1);
//	lcd.print("Date: ");
//	lcd.print(rtc.getDateStr());
//
//	delay(1000);
//}

