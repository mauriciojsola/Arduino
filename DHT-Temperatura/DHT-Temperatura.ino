#include "DHT.h"


// Uncomment one of the lines below for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321    ESTE!!!!

#define LOG_INTERVAL  15000  // mills between entries
#define ECHO_TO_SERIAL   1  // echo data to serial port if nonzero

// DHT Sensor
uint8_t DHTPin = D4; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);   
             
float humidity, temperature;
long lastTimeRead = 0; //Para guardar el tiempo de la ultima lectura

void setup() {
  Serial.begin(115200);
  delay(100);
  
  pinMode(DHTPin, INPUT);

  ConsoleLn("Iniciando Sensor de Humedad...");
  dht.begin();              
}

void loop() {
  //---------Lectura del Sensor--------------------------
  if (millis() - lastTimeRead > LOG_INTERVAL)
  {
    // Reat temperature and humidity
    ReadTH();  

    //--------Enviamos las lecturas por el puerto serial-------------
    #if ECHO_TO_SERIAL
        Serial.print(temperature);
        Serial.print(",");
        Serial.print(humidity);
        Serial.println();
    #endif  // ECHO_TO_SERIAL
    lastTimeRead = millis(); //actualizamos el tiempo de la ultima lectura
}
}

void ReadTH()
{
  humidity = dht.readHumidity(); //Leemos la Humedad
  temperature = dht.readTemperature(); //Leemos la temperatura en grados Celsius
  //float f = dht.readTemperature(true); //Leemos la temperatura en grados Fahrenheit
}


void ConsoleLn(String message) {
  Console(message, true);
}

void ConsoleLn(int value) {
  Console(String(value), true);
}

void Console(int value, bool newLine) {
  Console(String(value), true);
}

void Console(String message, bool newLine) {
#if ECHO_TO_SERIAL
  if (newLine)
  {
    Serial.println(message);
  }
  else
  {
    Serial.print(message);
  }
#endif  // ECHO_TO_SERIAL
}
