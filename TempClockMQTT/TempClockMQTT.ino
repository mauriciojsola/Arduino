/*
https://somtips.com
https://youtube.com/somtips
*/
#include <RTClib.h>
#include <Wire.h>
#include "DHT.h"

// DHT Sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
uint8_t DHTPin = D4; 
              
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);   

// Define Realtime Clock
RTC_DS3231 rtc;

char formattedTime[32];
#define LOG_INTERVAL  5000  // mills between entries
#define ECHO_TO_SERIAL   1  // echo data to serial port if nonzero
float humidity, temperature;
long lastTimeRead = 0; // Stores last time read

void setup() 
{
  Serial.begin(9600);

  Serial.println("Initializing Realtime Clock!");
  Wire.begin();
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  //rtc.adjust(DateTime(2020, 5, 11, 19, 57, 0));

  Serial.println("Initializing Sensor!");
  pinMode(DHTPin, INPUT);
  dht.begin(); 
}

void loop()
{

  //---------Lectura del Sensor--------------------------
  if (millis() - lastTimeRead > LOG_INTERVAL)
  {
    // Read temperature and humidity
    ReadTH();  

    //--------Enviamos las lecturas por el puerto serial-------------
    #if ECHO_TO_SERIAL
        Serial.print(formattedTime);
        Serial.print(",");
        Serial.print(temperature);
        Serial.print(",");
        Serial.print(humidity);
        Serial.println();
    #endif  // ECHO_TO_SERIAL
    lastTimeRead = millis(); // update last time read
  }
}

void ReadTH()
{
  DateTime now = rtc.now();
  sprintf(formattedTime, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  
  humidity = dht.readHumidity(); // Read humidity
  temperature = dht.readTemperature(); // Read temperature in Celsius
  //float f = dht.readTemperature(true); // Read temperature in Fahrenheit
}
