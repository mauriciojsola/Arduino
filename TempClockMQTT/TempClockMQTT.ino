/*
https://somtips.com
https://youtube.com/somtips
*/
#include <ESP8266WiFi.h>
#include <RTClib.h>
#include <Wire.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include "arduino_secrets.h"

// DHT Sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
uint8_t DHTPin = D4; 
              
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);   

// Define Realtime Clock
RTC_DS3231 rtc;

#define LOG_INTERVAL  60000  // mills between entries
#define ECHO_TO_SERIAL   1  // echo data to serial port if nonzero

// Credentials comes from the arduino_secrets.h file!

char ssid[] = WLAN_SSID;                        // your network SSID (name)
char pass[] = WLAN_PASS;                        // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;
char server[] = AIO_SERVER;                     // name address for Adafruit IOT Cloud
char user[] = AIO_USERNAME;
char key[] = AIO_KEY;
unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 7000;     // delay between updates, in milliseconds

char formattedTime[32];
float humidity, temperature;
long lastTimeRead = 0; // Stores last time read

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

int state = 2;

void setup() 
{
  Serial.begin(9600);

  Serial.println("Initializing Realtime Clock!");
  Wire.begin();
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  //rtc.adjust(DateTime(2020, 5, 12, 14, 57, 0));

  Serial.println("Initializing Sensor!");
  pinMode(DHTPin, INPUT);
  dht.begin(); 

  ConectToWIFI();  
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

    POSTDataReadings();
    
    lastTimeRead = millis(); // update last time read
  }
}

void ReadTH()
{
  DateTime now = rtc.now();
  sprintf(formattedTime, "%02d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());  
  
  humidity = dht.readHumidity(); // Read humidity
  temperature = dht.readTemperature(); // Read temperature in Celsius
  //float f = dht.readTemperature(true); // Read temperature in Fahrenheit
}

void ConectToWIFI()
{
   // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  printWifiStatus();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
//  long rssi = WiFi.RSSI();
//  Serial.print("signal strength (RSSI):");
//  Serial.print(rssi);
//  Serial.println(" dBm");
}


void POSTDataReadings() 
{
// {
//  "value": "{\"timestamp\":\"2020-05-11 15:44\", \"humidity\": \"60.40\", \"temperature\":\"21.5\"}",
//  "lat": 0,
//  "lon": 0,
//  "ele": 0
//}

const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + 200;
StaticJsonDocument<capacity> doc;
char json[capacity];

//doc["value"] = "{\"timestamp\":\"2020-05-12 16:53:13\",\"humidity\":44.1,\"temperature\":23.9}";
doc["value"] = (String)"{\"timestamp\":\"" + formattedTime + "\",\"humidity\":" + humidity + ",\"temperature\":" + temperature + "}";
doc["lat"] = 0;
doc["lon"] = 0;
doc["ele"] = 0;


serializeJson(doc, json);
//Serial.print("SERIALIZED JSON:");
//Serial.println(json);

// close any connection before send a new request.
  // This will free the socket on the Nina module
  client.stop();
  Serial.println("\nStarting connection to server...");
  if (client.connect(server, 80)) 
  {
    Serial.println("connected to server");
    // Make a HTTP request:
    Serial.println((String)"POST /api/v2/" + user + "/feeds/weather-feed.wheater-checkpoint/data.json HTTP/1.1");
    Serial.println((String)"Host: " + server);
    Serial.println((String)"X-AIO-Key: " + key);
//    Serial.print("Content-Length: ");  
//    Serial.println(measureJson(doc));
    //serializeJson(doc, Serial); 
    
    client.println((String)"POST /api/v2/" + user + "/feeds/weather-feed.wheater-checkpoint/data.json HTTP/1.1"); 
    client.println("Content-Type: application/json");  
    client.println((String)"X-AIO-Key: " + key); 
    client.print("Content-Length: ");    
    client.println(strlen(json));

    Serial.print("Content-Length: ");
    Serial.println(strlen(json));
    
    //client.println(measureJson(doc));
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");    
    
    client.println();
        
    // Send JSON document in body
    
    //serializeJson(doc, client);
    //serializeJson(doc, (WiFiClient&)client);
    client.println(json);
    //client.println();
    
    // note the time that the connection was made:
    lastConnectionTime = millis();


//    Serial.println("[Response:]");
//    while (client.connected() || client.available())
//    {
//      if (client.available())
//      {
//        String line = client.readStringUntil('\n');
//        Serial.println(line);
//      }
//    }
//    
//    Serial.println("\nData sent to Cloud!");
    
  } else {
    Serial.println("connection to ADAFRUIT FEEDS FAILED!");
  }  

  
}
