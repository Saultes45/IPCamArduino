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

Pin13 (amber led) is used as an output
******************************************************************************/



#include <TinyGPS++.h> // Include the TinyGPS++ library
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object

#define GPS_BAUD 9600 // GPS module baud rate. GP3906 defaults to 9600.

#include <SoftwareSerial.h>
#define ARDUINO_GPS_RX 9 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 8 // GPS RX, Arduino TX pin
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo

int dayCounter = 0;
int startDateDay = 0;

// temporary varible for calcultion
int tempHours = 0;
int tempMinutes = 0;
int tempSeconds = 0;

// Time array being defined
// 2 times a day - 7 days
// HHMMSS format

  int timeStartArray[2][7] = {{71455,71455,71455,71455,71455,71455,71455} ,   /*  initializers for row indexed by 0 -> time1Start */
                              {81455,81455,81455,81455,81455,81455,81455}};    /*  initializers for row indexed by 1 -> time2Start */  

int timeStopArray[2][7] = {{71555,71555,71555,71555,71555, 71555, 71555} ,   /*  initializers for row indexed by 0 -> time1End */
                            {81555,81555,81555,81555,81555, 81555, 81555}};    /*  initializers for row indexed by 1 -> time2End */
 

  







void setup() 
{

// Prepare the pin 13 LED for blinking
pinMode(LED_BUILTIN, OUTPUT);
// Prepare the digital pin 2 for the relay command

  // Start the GPS communication @9600bauds
  gpsPort.begin(GPS_BAUD);
  startDateDay = tinyGPS.date.day();
}

void loop() 
{
  dayCounter =  startDateDay - tinyGPS.date.day();
  // if we have a fix
  
  // check the day from the GPS
  //Temporary transfor the 
  if ((tinyGPS.time.hour() > getHours(timeStartArray[0][dayCounter]) && tinyGPS.time.hour() < getHours(timeStopArray[0][dayCounter])) || (tinyGPS.time.hour() > getHours(timeStartArray[1][dayCounter]) && tinyGPS.time.hour() < getHours(timeStopArray[1][dayCounter])))
	{
      if ((tinyGPS.time.minute() > getMinutes(timeStartArray[0][dayCounter]) && tinyGPS.time.minute() < getMinutes(timeStopArray[0][dayCounter])) || (tinyGPS.time.minute() > getMinutes(timeStartArray[1][dayCounter]) && tinyGPS.time.minute() < getMinutes(timeStopArray[1][dayCounter])))
	  {
		      if ((tinyGPS.time.hour() > getSeconds(timeStartArray[0][dayCounter]) && tinyGPS.time.hour() < getSeconds(timeStopArray[0][dayCounter])) || (tinyGPS.time.hour() > getSeconds(timeStartArray[1][dayCounter]) && tinyGPS.time.hour() < getSeconds(timeStopArray[1][dayCounter])))
		      {
			      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
		      }
     }
	 
	}
  else //get already done flags here
    {
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    }
  // "Smart delay" looks for GPS data while the Arduino's not doing anything else
  smartDelay(1000); 

}

// -------------------------- Functions Definition---------------------------

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

int getHours(int tableElement)
{
  return((tableElement/ 100000 % 10)*10 + (tableElement/ 10000 % 10));
}

int getMinutes(int tableElement)
{
  return((tableElement/ 1000 % 10)*10 + (tableElement/ 100 % 10));
}

int getSeconds(int tableElement)
{
  return((tableElement/ 10 % 10)*10 + (tableElement/ 1 % 10));
}

