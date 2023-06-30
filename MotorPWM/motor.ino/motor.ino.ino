int MOTOR = 6; //13;

void setup() {
  
  //declaramos el pin 13 como salida para controlar el transistor
  pinMode(MOTOR, OUTPUT);
}

void loop() {
  //incrementamos lentamente la señal PWM en el transistor
   for (int a = 100; a < 255; a++){
     analogWrite(MOTOR, a);
     delay(10);
   }
   //disminuyendo lentamente la señal PWM en el transistor 
   for(int a = 255; a> 100; a--){
     analogWrite(MOTOR, a);
     delay(10);
   }

//  digitalWrite(MOTOR, HIGH);
//  delay(2000);
//  digitalWrite(MOTOR, LOW);
//  delay(2000);
}
