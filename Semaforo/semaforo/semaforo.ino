const int rojo = 11;
const int verde = 13;
const int amarillo = 11;
const int boton = 5;
int lecturaBoton;

void setup() {
  // put your setup code here, to run once:
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(boton, INPUT);
}

void loop() {

  lecturaBoton = digitalRead(boton);

  if (lecturaBoton == HIGH) {
    digitalWrite(amarillo, HIGH);
    digitalWrite(verde, LOW);

  } else {
    digitalWrite(verde, HIGH);
    digitalWrite(amarillo, LOW);
  }


  // digitalWrite(verde, HIGH);  //encendemos el led rojo
  // delay(500);                //esperamos 2 segundos
  // digitalWrite(verde, LOW);   //apagamos el led rojo
  // delay(500);
  // digitalWrite(verde, HIGH);  //encendemos el led rojo
  // delay(500);                //esperamos 2 segundos
  // digitalWrite(verde, LOW);   //apagamos el led rojo
  // delay(500);
  // digitalWrite(verde, HIGH);  //encendemos el led rojo
  // delay(500);                //esperamos 2 segundos
  // digitalWrite(verde, LOW);   //apagamos el led rojo
  // delay(500);


  // digitalWrite(amarillo, HIGH);  //encendemos el led amarillo
  // delay(1000);                   //esperamos 2 segundos
  // digitalWrite(amarillo, LOW);   //apagamos el led amarillo
  // delay(500);                    //esperamos medio segundo

  // digitalWrite(rojo, HIGH);  //encendemos el led verde
  // delay(1000);               //esperamos 2 segundos
  // digitalWrite(rojo, LOW);   //apagamos el led verde
  // delay(500);                //esperamos medio segundo
}
