// Basic Bluetooth sketch HC-05_03 Using the state pin
// Connect the HC-05 module and communicate using the serial monitor
//
// The HC-05 defaults to communication mode when first powered on.
// The default baud rate for communication mode is 9600

//*** https://reference.arduino.cc/reference/en/language/functions/communication/serial/

#include <SoftwareSerial.h>
SoftwareSerial Bluetooth(53, 52);  // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX.
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
// Connect the HC-05 STATE pin to Arduino pin 4.
//

char c = ' ';

// BTconnected will = false when not connected and true when connected
boolean BTconnected = false;

// connect the STATE pin to Arduino pin D4
const byte BTpin = 4;
const byte ledPin = 13;


void setup() {
  // set the BTpin for input
  pinMode(BTpin, INPUT);
  pinMode(ledPin, OUTPUT);

  Bluetooth.begin(38400);  // Default baud rate of the Bluetooth module

  // start serial communication with the serial monitor on the host computer
  Serial.begin(38400);
  Serial.println("Arduino is ready");
  Serial.println("Connect the HC-05 to an Android device to continue");

  // wait until the HC-05 has made a connection
  while (!BTconnected) {
    if (digitalRead(BTpin) == HIGH) { BTconnected = true; };
  }

  Serial.println("HC-05 is now connected");
  Serial.println("");

  // Start serial communication with the bluetooth module
  // HC-05 default serial speed for communication mode is 9600 but can be different  
  Serial3.begin(38400);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  delay(1000);
  // Keep reading from the HC-05 and send to Arduino Serial Monitor
  //Serial.print("Serial3.available?: ");
  //Serial.println(BTserial.available());
  //Serial.println(Serial3.available());
  //if (BTserial.available())

  // if (BTserial.available()>0)
  //     {
  //       Serial.println("BTserial available!");
  //         c = BTserial.read();
  //         //c = Serial3.read();
  //         Serial.write(c);
  //     }else{
  //       Serial.println("BTserial NOT available");
  //     }

  if (Bluetooth.available() > 0) {
    Serial.println("Bluetooth available! Data:");
    c = Bluetooth.read();
    //c = Serial3.read();
    Serial.write(c);
  } else {
    //Serial.println("Bluetooth NOT available");
  }



  // if (Serial3.available() > 0) {
  //   Serial.println("Serial 3 available!");
  //   //c = BTserial.read();
  //   c = Serial3.read();
  //   Serial.write(c);
  // }

  // Keep reading from Arduino Serial Monitor input field and send to HC-05
  if (Serial.available() > 0) {
    c = Serial.read();
    Serial.print("Manual Entry: ");
    Serial.println(c);
    //BTserial.write(c);
  }
}