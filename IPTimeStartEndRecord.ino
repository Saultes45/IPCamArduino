/******************************************************************************
Written by: Nathanael ESNAULT
Email: nesn277@aucklanduni.ac.nz
Date: 30/03/2017


Resources:
TinyGPS++ Library  - https://github.com/mikalhart/TinyGPSPlus/releases
SoftwareSerial Library

To be used with the GPS Logger Shield from SPARKFUN
https://learn.sparkfun.com/tutorials/gps-logger-shield-hookup-guide
(link may become obsolete)

THE SWITCH ON THE SHIELD MUST BE IN "SW" POSITION

Pin12 is used as an output for the relay module
******************************************************************************/



// ------------------ External RTC DS1307N ---------------
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

// ------------------ External GPS Shield ---------------
#include <TinyGPS++.h> // Include the TinyGPS++ library
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object

#define GPS_BAUD 9600 // GPS module baud rate. GP3906 defaults to 9600.

// If you're using an Arduino Uno, Mega, RedBoard, or any board that uses the
// 0/1 UART for programming/Serial monitor-ing, use SoftwareSerial:
#include <SoftwareSerial.h>
#define ARDUINO_GPS_RX 9 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 8 // GPS RX, Arduino TX pin
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial

// Set gpsPort to either ssGPS if using SoftwareSerial or Serial1 if using an
// Arduino with a dedicated hardware serial port
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo

// Define the serial monitor port. On the Uno, Mega, and Leonardo this is 'Serial'
//  on other boards this may be 'SerialUSB'
#define SerialMonitor Serial

// ------------------ Internal Timekeeping/Handling ---------------
#include <TimeLib.h>
#include <TimeAlarms.h>

AlarmId id;

int ledPin = 12;
int dayPrevious = 0;

long int recordTime = 1296000;

// ------------------ Timetables ---------------

unsigned long int timeStartArray[2][7] = {{105900,25300,234100,105900,105900,105900,105900} ,   /*  initializers for row indexed by 0 -> time1Start */
	{210000,24900,234600,210000,210000,210000,210000}};    /*  initializers for row indexed by 1 -> time2Start */  

// ------------------ Set Up ---------------
void setup() {
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);   // turn the LED on (LOW is the voltage level)
	//Serial.begin(9600);
	//while (!Serial) ; // wait for Arduino Serial Monitor

	SerialMonitor.begin(9600);
	gpsPort.begin(GPS_BAUD);

	Wire.begin();
	RTC.begin();
	DateTime now = RTC.now();

	smartDelay(1000);

	adjustAllClocks();
	dayPrevious = day();
	// create the alarms, to trigger at specific times :sizeof

	// Days order: Sun, Mon, Tues, Wed, Thurs, Fri, Sat
	
	//timeStartArray[0][2] = (unsigned long) (hour())*10000 + (unsigned long) (minute())*100 + (unsigned long) (second())+5;
	//timeStartArray[1][2] = (unsigned long) (hour())*10000 + (unsigned long) (minute())*100 + (unsigned long) (second())+10;

        Serial.println(timeStartArray[0][2]);
        Serial.println(timeStartArray[1][2]);
        Serial.println(getHours(timeStartArray[0][2]));
        
        

	Alarm.alarmOnce(dowSunday, getHours(timeStartArray[0][0]),  getMinutes(timeStartArray[0][0]), getSeconds(timeStartArray[0][0]),  MorningAlarmStart);  // Alarm Start recording 1
	Alarm.alarmOnce(dowSunday, getHours(timeStartArray[1][0]),  getMinutes(timeStartArray[1][0]), getSeconds(timeStartArray[1][0]),  EveningAlarmStart);  // Alarm Start recording 2

	Alarm.alarmOnce(dowMonday, getHours(timeStartArray[0][1]),  getMinutes(timeStartArray[0][1]), getSeconds(timeStartArray[0][1]),  MorningAlarmStart);  // Alarm Start recording 1
	Alarm.alarmOnce(dowMonday, getHours(timeStartArray[1][1]),  getMinutes(timeStartArray[1][1]), getSeconds(timeStartArray[1][1]),  EveningAlarmStart);  // Alarm Start recording 2
	
	Alarm.alarmOnce(dowTuesday, getHours(timeStartArray[0][2]),  getMinutes(timeStartArray[0][2]), getSeconds(timeStartArray[0][3]),  MorningAlarmStart);  // Alarm Start recording 1
	Alarm.alarmOnce(dowTuesday, getHours(timeStartArray[1][2]),  getMinutes(timeStartArray[1][2]), getSeconds(timeStartArray[1][3]),  EveningAlarmStart);  // Alarm Start recording 2
	
	Alarm.alarmOnce(dowWednesday, getHours(timeStartArray[0][3]),  getMinutes(timeStartArray[0][3]), getSeconds(timeStartArray[0][3]),  MorningAlarmStart);  // Alarm Start recording 1
	Alarm.alarmOnce(dowWednesday, getHours(timeStartArray[1][3]),  getMinutes(timeStartArray[1][3]), getSeconds(timeStartArray[1][3]),  EveningAlarmStart);  // Alarm Start recording 2
	
	Alarm.alarmOnce(dowThursday, getHours(timeStartArray[0][4]),  getMinutes(timeStartArray[0][4]), getSeconds(timeStartArray[0][4]),  MorningAlarmStart);  // Alarm Start recording 1
	Alarm.alarmOnce(dowThursday, getHours(timeStartArray[1][4]),  getMinutes(timeStartArray[1][4]), getSeconds(timeStartArray[1][4]),  EveningAlarmStart);  // Alarm Start recording 2
	
	Alarm.alarmOnce(dowFriday, getHours(timeStartArray[0][5]),  getMinutes(timeStartArray[0][5]), getSeconds(timeStartArray[0][5]),  MorningAlarmStart);  // Alarm Start recording 1
	Alarm.alarmOnce(dowFriday, getHours(timeStartArray[1][5]),  getMinutes(timeStartArray[1][5]), getSeconds(timeStartArray[1][5]),  EveningAlarmStart);  // Alarm Start recording 2
	
	Alarm.alarmOnce(dowSaturday, getHours(timeStartArray[0][6]),  getMinutes(timeStartArray[0][6]), getSeconds(timeStartArray[0][1]),  MorningAlarmStart);  // Alarm Start recording 1
	Alarm.alarmOnce(dowSaturday, getHours(timeStartArray[1][6]),  getMinutes(timeStartArray[1][6]), getSeconds(timeStartArray[1][1]),  EveningAlarmStart);  // Alarm Start recording 2
	
}


// ------------------ Loop ---------------
void loop() {
	ArduinoPrintTime();
	//RTCprintTime();
	//GPSprintTime();

	if (dayPrevious != day())
	{ //another day has passed, readjust clocks
                Serial.println("Day has changed, adjusting clocks");
		dayPrevious = day();
		adjustAllClocks();

	}

	Alarm.delay(1000); // wait 500ms second between each clock display
	//smartDelay(500);  // GPS delay
	
}







// ------------------ functions to be called when an alarm triggers ---------------
void MorningAlarmStart() {
	Serial.println("Alarm Morning: - turn camera on");
	Alarm.timerOnce(recordTime, MorningAlarmStop);
	digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
	
}

void EveningAlarmStart() {
	Serial.println("Alarm Evening: - turn camera on");
	Alarm.timerOnce(recordTime, EveningAlarmStop);
	digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void MorningAlarmStop() {
	Serial.println("Alarm Morning: - turn camera off");
	digitalWrite(ledPin, LOW);   // turn the LED on (HIGH is the voltage level)
}

void EveningAlarmStop() {
	Serial.println("Alarm Evening: - turn camera off");
	digitalWrite(ledPin, LOW);   // turn the LED on (HIGH is the voltage level)
}


// ------------------ All other alarms ---------------

// This custom version of delay() ensures that the tinyGPS object
// is being "fed". From the TinyGPS++ examples.
static void smartDelay(unsigned long ms)
{
	unsigned long start = millis();
	do
	{
		// If data has come in from the GPS module
		while (gpsPort.available())
		tinyGPS.encode(gpsPort.read()); // Send it to the encode function
		// tinyGPS.encode(char) continues to "load" the tinGPS object with new
		// data coming in from the GPS module. As full NMEA strings begin to come in
		// the tinyGPS library will be able to start parsing them for pertinent info
	} while (millis() - start < ms);
}



void ArduinoPrintTime() {
	// digital clock display of the time
	Serial.print("Arduino:");
	printDigitsHours(hour());
	printDigits(minute());
	printDigits(second());
	Serial.println();
}

void printDigitsHours(int digits) {
	if (digits < 10)
	Serial.print('0');
	Serial.print(digits);
}

void printDigits(int digits) {
	Serial.print(":");
	if (digits < 10)
	Serial.print('0');
	Serial.print(digits);
}

void RTCprintTime(void)
{

	DateTime now = RTC.now();
	Serial.print("RTC:"); 
	Serial.print(now.year());
	Serial.print('/');
	Serial.print(now.month(), DEC);
	Serial.print('/');
	Serial.print(now.day(), DEC);
	Serial.print(' ');
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.println(); 

}

int getHours(unsigned long int tableElement)
{
	return((int)(tableElement/ 100000 % 10)*10 + (tableElement/ 10000 % 10));
}

int getMinutes(unsigned long int tableElement)
{
	return((int)(tableElement/ 1000 % 10)*10 + (tableElement/ 100 % 10));
}

int getSeconds(unsigned long int tableElement)
{
	return((int)(tableElement/ 10 % 10)*10 + (tableElement/ 1 % 10));
}


// printTime() formats the time into "hh:mm:ss", and prints leading 0's
// where they're called for.
void GPSprintTime()
{
	SerialMonitor.print("GPS:");
	SerialMonitor.print(tinyGPS.time.hour());
	SerialMonitor.print(":");
	if (tinyGPS.time.minute() < 10) SerialMonitor.print('0');
	SerialMonitor.print(tinyGPS.time.minute());
	SerialMonitor.print(":");
	if (tinyGPS.time.second() < 10) SerialMonitor.print('0');
	SerialMonitor.println(tinyGPS.time.second());
}

void adjustAllClocks()
{
	if(tinyGPS.satellites.value() > 3 ) //check we have GPS fix otherwise use RTC module to set time
	{
		setTime(tinyGPS.time.hour(),tinyGPS.time.minute(),tinyGPS.time.second(),tinyGPS.date.day(),tinyGPS.date.month(),tinyGPS.date.year()-2000);

		// set the RTC with GPS Time
		RTC.adjust(DateTime(tinyGPS.date.year()-2000, tinyGPS.date.month() , tinyGPS.date.day(), tinyGPS.time.hour(), tinyGPS.time.minute(), tinyGPS.time.second()));
                
                Serial.println("Adjusting with GPS");
	}
	else
	{
		//set arduino clock with RTC module
		DateTime now = RTC.now();
		setTime(now.hour(),now.minute(),now.second(),now.day(),now.month(),now.year());
                
                Serial.println("Adjusting with RTC");
	}
}


