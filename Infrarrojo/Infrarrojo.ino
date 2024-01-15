const int sensorPin = 9;

void setup() {
  Serial.begin(38400);        //iniciar puerto serie
  pinMode(sensorPin, INPUT);  //definir pin como entrada
}

void loop() {
  int value = 0;
  value = digitalRead(sensorPin);  //lectura digital de pin

  if (value == LOW) {
    Serial.println("Detectado obstaculo");
  } else {
    Serial.println("*");
  }
  delay(1000);
}