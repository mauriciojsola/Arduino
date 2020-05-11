/*
https://somtips.com
https://youtube.com/somtips
*/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"
#include <RTClib.h>
#include <Wire.h>

// Realtime clock
RTC_DS3231 rtc;

// Sensor de humedad y temperatura
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321    ESTE!!!!
// DHT Sensor
uint8_t DHTPin = D4; 

    // WiFi parameters
    #define WLAN_SSID       "WLAN_SSID"
    #define WLAN_PASS       "WLAN_PASS"
     
    // Adafruit IO
    #define AIO_SERVER      "io.adafruit.com"
    #define AIO_SERVERPORT  1883
    #define IO_USERNAME  "mauriciojsola11"
    #define IO_KEY       "aio_CaOh839EzEf9iY7RTpKWDBLZ6Mtz"



#define LOG_INTERVAL  15000  // mills between entries
#define ECHO_TO_SERIAL   1  // echo data to serial port if nonzero

float humidity, temperature;
long lastTimeRead = 0; //Para guardar el tiempo de la ultima lectura

char t[32];

void setup() 
{
  Serial.begin(115200);
  delay(100);

  // Inicializar el reloj
  Serial.println("Iniciando reloj...");
  Wire.begin();
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  //rtc.adjust(DateTime(2020, 5, 10, 21, 45, 0));
  
  // Inicializar el sensor de humedad/temperatura 
  Serial.println("Iniciando Sensor de Humedad...");
  pinMode(DHTPin, INPUT);
  dht.begin();  
  
  

}

void loop()
{
  DateTime now = rtc.now();

  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  
  Serial.print(F("Date/Time: "));
  

  delay(1000);
}
