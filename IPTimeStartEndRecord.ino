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



// ------------------ External RTC DS1307N ---------------
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

// ------------------ External RGPS Shield ---------------
#include <TinyGPS++.h> // Include the TinyGPS++ library
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object

#define GPS_BAUD 9600 // GPS module baud rate. GP3906 defaults to 9600.

#include <SoftwareSerial.h>
#define ARDUINO_GPS_RX 9 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 8 // GPS RX, Arduino TX pin
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo

// ------------------ Internal Timekeeping/Handling ---------------
#include <TimeLib.h>
#include <TimeAlarms.h>

AlarmId id;

int ledPin = 12; 

// ------------------ Timetables ---------------

long int timeStartArray[2][7] = {{3000,3000,3000,3000,3000,3000,3000} ,   /*  initializers for row indexed by 0 -> time1Start */
                              {3020,3020,3020,3020,3020,3020,3020}};    /*  initializers for row indexed by 1 -> time2Start */  

long int timeStopArray[2][7] = {{3010,3010,3010,3010,3010, 3010, 3010} ,   /*  initializers for row indexed by 0 -> time1End */
                            {3040,3040,3040,3040,3040, 3040, 3040}};    /*  initializers for row indexed by 1 -> time2End */
 



// ------------------ Set Up ---------------
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);   // turn the LED on (LOW is the voltage level)
  Serial.begin(230400);
  while (!Serial) ; // wait for Arduino Serial Monitor

// Create 4 alarms per day begining by  

  //setTime(8,29,57,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011    Wire.begin();
  //synch only at start with RTC
  Wire.begin();
  RTC.begin();
  DateTime now = RTC.now(); 
  setTime(now.hour(),now.minute(),now.second(),now.day(),now.month(),now.year()-2000); // set time to Saturday 8:29:00am Jan 1 2011

  // create the alarms, to trigger at specific times :sizeof

//    timeDayOfWeek_t eachDayVector[8];

 //   for(int i = dowInvalid; i!= dowSaturday;i++){
//        eachDayVector[i] = static_cast<timeDayOfWeek_t>(i);
//    }
  
  // Days order: Sun, Mon, Tues, Wed, Thurs, Fri, Sat
//  for(int numberOfDays = 1; numberOfDays < 8-1; numberOfDays++) // -> max of 256 alarms = 64 days max
 // {
  // Alarm.alarmRepeat(dowSunday, getHours(timeStartArray  [0][0]),  getMinutes(timeStartArray [0][0]),  getSeconds(timeStartArray [0][0]),  MorningAlarmStart);  // Alarm Start recording 1
//       Alarm.alarmRepeat(dowSunday, getHours(timeStopArray [0][0]),  getMinutes(timeStopArray  [0][0]),  getSeconds(timeStopArray  [0][0]),  MorningAlarmStop);  // Alarm Stop recording 1
//       Alarm.alarmRepeat(dowSunday, getHours(timeStartArray  [1][0]),  getMinutes(timeStartArray [1][0]),  getSeconds(timeStartArray [1][0]),  EveningAlarmStart);  // Alarm Start recording 2
//       Alarm.alarmRepeat(dowSunday, getHours(timeStopArray [1][0]),  getMinutes(timeStopArray  [1][0]),  getSeconds(timeStopArray  [1][0]),  EveningAlarmStop);  // Alarm Stop recording 2
// 
//                                                                                                                                                  
//     Alarm.alarmRepeat(dowMonday, getHours(timeStartArray  [0][1]),  getMinutes(timeStartArray [0][1]),  getSeconds(timeStartArray [0][1]),  MorningAlarmStart);  // Alarm Start recording 1
//       Alarm.alarmRepeat(dowMonday, getHours(timeStopArray [0][1]),  getMinutes(timeStopArray  [0][1]),  getSeconds(timeStopArray  [0][1]),  MorningAlarmStop);  // Alarm Stop recording 1
//       Alarm.alarmRepeat(dowMonday, getHours(timeStartArray  [1][1]),  getMinutes(timeStartArray [1][1]),  getSeconds(timeStartArray [1][1]),  EveningAlarmStart);  // Alarm Start recording 2
//       Alarm.alarmRepeat(dowMonday, getHours(timeStopArray [1][1]),  getMinutes(timeStopArray  [1][1]),  getSeconds(timeStopArray  [1][1]),  EveningAlarmStop);  // Alarm Stop recording 2
//                                                                                                                                                   
//     Alarm.alarmRepeat(dowTuesday, getHours(timeStartArray [0][2]),  getMinutes(timeStartArray [0][2]),  getSeconds(timeStartArray [0][2]),  MorningAlarmStart);  // Alarm Start recording 1
//       Alarm.alarmRepeat(dowTuesday, getHours(timeStopArray  [0][2]),  getMinutes(timeStopArray  [0][2]),  getSeconds(timeStopArray  [0][2]),  MorningAlarmStop);  // Alarm Stop recording 1
//       Alarm.alarmRepeat(dowTuesday, getHours(timeStartArray [1][2]),  getMinutes(timeStartArray [1][2]),  getSeconds(timeStartArray [1][2]),  EveningAlarmStart);  // Alarm Start recording 2
//       Alarm.alarmRepeat(dowTuesday, getHours(timeStopArray  [1][2]),  getMinutes(timeStopArray  [1][2]),  getSeconds(timeStopArray  [1][2]),  EveningAlarmStop);  // Alarm Stop recording 2
//                                                                                                                                                   
//     Alarm.alarmRepeat(dowWednesday, getHours(timeStartArray [0][3]),  getMinutes(timeStartArray [0][3]),  getSeconds(timeStartArray [0][3]),  MorningAlarmStart);  // Alarm Start recording 1
//       Alarm.alarmRepeat(dowWednesday, getHours(timeStopArray  [0][3]),  getMinutes(timeStopArray  [0][3]),  getSeconds(timeStopArray  [0][3]),  MorningAlarmStop);  // Alarm Stop recording 1
//       Alarm.alarmRepeat(dowWednesday, getHours(timeStartArray [1][3]),  getMinutes(timeStartArray [1][3]),  getSeconds(timeStartArray [1][3]),  EveningAlarmStart);  // Alarm Start recording 2
//       Alarm.alarmRepeat(dowWednesday, getHours(timeStopArray  [1][3]),  getMinutes(timeStopArray  [1][3]),  getSeconds(timeStopArray  [1][3]),  EveningAlarmStop);  // Alarm Stop recording 2
//       Serial.print("Alarm set for:"); Serial.print(dowWednesday);Serial.print(",");Serial.print(getHours(timeStartArray [0][3]));Serial.print("/");Serial.print(getMinutes(timeStartArray [0][3]));
//       Serial.print("/");Serial.print(getSeconds(timeStartArray [0][3]));Serial.print("And the year is:");Serial.println(now.year()-2000);
//                                                                                                                                                   
//     Alarm.alarmRepeat(dowThursday, getHours(timeStartArray  [0][4]),  getMinutes(timeStartArray [0][4]),  getSeconds(timeStartArray [0][4]),  MorningAlarmStart);  // Alarm Start recording 1
//       Alarm.alarmRepeat(dowThursday, getHours(timeStopArray [0][4]),  getMinutes(timeStopArray  [0][4]),  getSeconds(timeStopArray  [0][4]),  MorningAlarmStop);  // Alarm Stop recording 1
//       Alarm.alarmRepeat(dowThursday, getHours(timeStartArray  [1][4]),  getMinutes(timeStartArray [1][4]),  getSeconds(timeStartArray [1][4]),  EveningAlarmStart);  // Alarm Start recording 2
//       Alarm.alarmRepeat(dowThursday, getHours(timeStopArray [1][4]),  getMinutes(timeStopArray  [1][4]),  getSeconds(timeStopArray  [1][4]),  EveningAlarmStop);  // Alarm Stop recording 2
//                                                                                                                                                   
//     Alarm.alarmRepeat(dowFriday, getHours(timeStartArray  [0][5]),  getMinutes(timeStartArray [0][5]),  getSeconds(timeStartArray [0][5]),  MorningAlarmStart);  // Alarm Start recording 1
//       Alarm.alarmRepeat(dowFriday, getHours(timeStopArray [0][5]),  getMinutes(timeStopArray  [0][5]),  getSeconds(timeStopArray  [0][5]),  MorningAlarmStop);  // Alarm Stop recording 1
//       Alarm.alarmRepeat(dowFriday, getHours(timeStartArray  [1][5]),  getMinutes(timeStartArray [1][5]),  getSeconds(timeStartArray [1][5]),  EveningAlarmStart);  // Alarm Start recording 2
//       Alarm.alarmRepeat(dowFriday, getHours(timeStopArray [1][5]),  getMinutes(timeStopArray  [1][5]),  getSeconds(timeStopArray  [1][5]),  EveningAlarmStop);  // Alarm Stop recording 2
//                                                                                                                                                   
//     Alarm.alarmRepeat(dowSaturday, getHours(timeStartArray  [0][6]),  getMinutes(timeStartArray [0][6]),  getSeconds(timeStartArray [0][6]),  MorningAlarmStart);  // Alarm Start recording 1
//       Alarm.alarmRepeat(dowSaturday, getHours(timeStopArray [0][6]),  getMinutes(timeStopArray  [0][6]),  getSeconds(timeStopArray  [0][6]),  MorningAlarmStop);  // Alarm Stop recording 1
//       Alarm.alarmRepeat(dowSaturday, getHours(timeStartArray  [1][6]),  getMinutes(timeStartArray [1][6]),  getSeconds(timeStartArray [1][6]),  EveningAlarmStart);  // Alarm Start recording 2
//       Alarm.alarmRepeat(dowSaturday, getHours(timeStopArray [1][6]),  getMinutes(timeStopArray  [1][6]),  getSeconds(timeStopArray  [1][6]),  EveningAlarmStop);  // Alarm Stop recording 2


//Alarm.alarmRepeat(dowThursday, getHours(timeStartArray  [0][4]), getMinutes(timeStartArray  [0][4]), getSeconds(timeStartArray  [0][4]),  EveningAlarmStart);  // Alarm Stop recording 2
     Alarm.alarmRepeat(dowSunday, getHours(timeStartArray  [0][4]),  getMinutes(timeStartArray [0][4]),  getSeconds(timeStartArray [0][4]),  MorningAlarmStart);  // Alarm Start recording 1
       Alarm.alarmRepeat(dowSunday, getHours(timeStopArray [0][4]),  getMinutes(timeStopArray  [0][4]),  getSeconds(timeStopArray  [0][4]),  MorningAlarmStop);  // Alarm Stop recording 1
       Alarm.alarmRepeat(dowSunday, getHours(timeStartArray  [1][4]),  getMinutes(timeStartArray [1][4]),  getSeconds(timeStartArray [1][4]),  EveningAlarmStart);  // Alarm Start recording 2
       Alarm.alarmRepeat(dowSunday, getHours(timeStopArray [1][4]),  getMinutes(timeStopArray  [1][4]),  getSeconds(timeStopArray  [1][4]),  EveningAlarmStop);  // Alarm Stop recording 2 

            Alarm.alarmRepeat(dowMonday, getHours(timeStartArray  [0][4]),  getMinutes(timeStartArray [0][4]),  getSeconds(timeStartArray [0][4]),  MorningAlarmStart);  // Alarm Start recording 1
       Alarm.alarmRepeat(dowMonday, getHours(timeStopArray [0][4]),  getMinutes(timeStopArray  [0][4]),  getSeconds(timeStopArray  [0][4]),  MorningAlarmStop);  // Alarm Stop recording 1
       Alarm.alarmRepeat(dowMonday, getHours(timeStartArray  [1][4]),  getMinutes(timeStartArray [1][4]),  getSeconds(timeStartArray [1][4]),  EveningAlarmStart);  // Alarm Start recording 2
       Alarm.alarmRepeat(dowMonday, getHours(timeStopArray [1][4]),  getMinutes(timeStopArray  [1][4]),  getSeconds(timeStopArray  [1][4]),  EveningAlarmStop);  // Alarm Stop recording 2 

            Alarm.alarmRepeat(dowTuesday, getHours(timeStartArray  [0][4]),  getMinutes(timeStartArray [0][4]),  getSeconds(timeStartArray [0][4]),  MorningAlarmStart);  // Alarm Start recording 1
       Alarm.alarmRepeat(dowTuesday, getHours(timeStopArray [0][4]),  getMinutes(timeStopArray  [0][4]),  getSeconds(timeStopArray  [0][4]),  MorningAlarmStop);  // Alarm Stop recording 1
       Alarm.alarmRepeat(dowTuesday, getHours(timeStartArray  [1][4]),  getMinutes(timeStartArray [1][4]),  getSeconds(timeStartArray [1][4]),  EveningAlarmStart);  // Alarm Start recording 2
       Alarm.alarmRepeat(dowTuesday, getHours(timeStopArray [1][4]),  getMinutes(timeStopArray  [1][4]),  getSeconds(timeStopArray  [1][4]),  EveningAlarmStop);  // Alarm Stop recording 2 

            Alarm.alarmRepeat(dowWednesday, getHours(timeStartArray  [0][4]),  getMinutes(timeStartArray [0][4]),  getSeconds(timeStartArray [0][4]),  MorningAlarmStart);  // Alarm Start recording 1
       Alarm.alarmRepeat(dowWednesday, getHours(timeStopArray [0][4]),  getMinutes(timeStopArray  [0][4]),  getSeconds(timeStopArray  [0][4]),  MorningAlarmStop);  // Alarm Stop recording 1
       Alarm.alarmRepeat(dowWednesday, getHours(timeStartArray  [1][4]),  getMinutes(timeStartArray [1][4]),  getSeconds(timeStartArray [1][4]),  EveningAlarmStart);  // Alarm Start recording 2
       Alarm.alarmRepeat(dowWednesday, getHours(timeStopArray [1][4]),  getMinutes(timeStopArray  [1][4]),  getSeconds(timeStopArray  [1][4]),  EveningAlarmStop);  // Alarm Stop recording 2 
       
     Alarm.alarmRepeat(dowThursday, getHours(timeStartArray  [0][4]),  getMinutes(timeStartArray [0][4]),  getSeconds(timeStartArray [0][4]),  MorningAlarmStart);  // Alarm Start recording 1
       Alarm.alarmRepeat(dowThursday, getHours(timeStopArray [0][4]),  getMinutes(timeStopArray  [0][4]),  getSeconds(timeStopArray  [0][4]),  MorningAlarmStop);  // Alarm Stop recording 1
       Alarm.alarmRepeat(dowThursday, getHours(timeStartArray  [1][4]),  getMinutes(timeStartArray [1][4]),  getSeconds(timeStartArray [1][4]),  EveningAlarmStart);  // Alarm Start recording 2
       Alarm.alarmRepeat(dowThursday, getHours(timeStopArray [1][4]),  getMinutes(timeStopArray  [1][4]),  getSeconds(timeStopArray  [1][4]),  EveningAlarmStop);  // Alarm Stop recording 2

             Alarm.alarmRepeat(dowFriday, getHours(timeStartArray  [0][4]),  getMinutes(timeStartArray [0][4]),  getSeconds(timeStartArray [0][4]),  MorningAlarmStart);  // Alarm Start recording 1
       Alarm.alarmRepeat(dowFriday, getHours(timeStopArray [0][4]),  getMinutes(timeStopArray  [0][4]),  getSeconds(timeStopArray  [0][4]),  MorningAlarmStop);  // Alarm Stop recording 1
       Alarm.alarmRepeat(dowFriday, getHours(timeStartArray  [1][4]),  getMinutes(timeStartArray [1][4]),  getSeconds(timeStartArray [1][4]),  EveningAlarmStart);  // Alarm Start recording 2
       Alarm.alarmRepeat(dowFriday, getHours(timeStopArray [1][4]),  getMinutes(timeStopArray  [1][4]),  getSeconds(timeStopArray  [1][4]),  EveningAlarmStop);  // Alarm Stop recording 2 

            Alarm.alarmRepeat(dowSaturday, getHours(timeStartArray  [0][4]),  getMinutes(timeStartArray [0][4]),  getSeconds(timeStartArray [0][4]),  MorningAlarmStart);  // Alarm Start recording 1
       Alarm.alarmRepeat(dowSaturday, getHours(timeStopArray [0][4]),  getMinutes(timeStopArray  [0][4]),  getSeconds(timeStopArray  [0][4]),  MorningAlarmStop);  // Alarm Stop recording 1
       Alarm.alarmRepeat(dowSaturday, getHours(timeStartArray  [1][4]),  getMinutes(timeStartArray [1][4]),  getSeconds(timeStartArray [1][4]),  EveningAlarmStart);  // Alarm Start recording 2
       Alarm.alarmRepeat(dowSaturday, getHours(timeStopArray [1][4]),  getMinutes(timeStopArray  [1][4]),  getSeconds(timeStopArray  [1][4]),  EveningAlarmStop);  // Alarm Stop recording 2 
//   }
//Serial.println(getHours(timeStartArray  [0][2]));
//Serial.println(getMinutes(timeStartArray  [0][2]));
//Serial.println(getSeconds(timeStartArray  [0][2]));

}


// ------------------ Loop ---------------
void loop() {
  digitalClockDisplay();
  //RTCDisplay();
  Alarm.delay(1000); // wait one second between each clock display
}

// ------------------ functions to be called when an alarm triggers ---------------
void MorningAlarmStart() {
  Serial.println("Alarm: - turn lights on");
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void EveningAlarmStart() {
  Serial.println("Alarm: - turn lights on");
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void MorningAlarmStop() {
  Serial.println("Alarm: - turn lights off");
  digitalWrite(ledPin, LOW);   // turn the LED on (HIGH is the voltage level)
}

void EveningAlarmStop() {
  Serial.println("Alarm: - turn lights off");
  digitalWrite(ledPin, LOW);   // turn the LED on (HIGH is the voltage level)
}


// ------------------ All other alarms ---------------
void digitalClockDisplay() {
  // digital clock display of the time
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

void RTCDisplay(void)
{

    DateTime now = RTC.now();
    Serial.print("RTC:"); 
    Serial.print(now.year(), DEC);
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

int getHours(long int tableElement)
{
  //Serial.println((tableElement/ 100000 % 10)*10 + (tableElement/ 10000 % 10));
  return((tableElement/ 100000 % 10)*10 + (tableElement/ 10000 % 10));
}

int getMinutes(long int tableElement)
{
   
  return((tableElement/ 1000 % 10)*10 + (tableElement/ 100 % 10));
}

int getSeconds(long int tableElement)
{

  return((tableElement/ 10 % 10)*10 + (tableElement/ 1 % 10));
}



