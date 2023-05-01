const int LedRojo = 8;
const int LedAmarillo = 11;
const int LedVerde = 13;
const int LedChiquito = 9;
const int LedBlanco = 2;
const int Boton = 5;

int buttonState = 0;

int presionoBotonEncendido = 0;


void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  pinMode (LedRojo, OUTPUT);
  pinMode (LedAmarillo, OUTPUT);
  pinMode (LedVerde, OUTPUT);
  pinMode (LedChiquito, OUTPUT);
  pinMode (LedBlanco, OUTPUT);
  pinMode (Boton, INPUT);

Serial.println("INICIANDO  SEMAFORO");


}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead (Boton);
  if (buttonState == LOW) {

  } else {
    presionoBotonEncendido = HIGH;
    Serial.println ("Boton APRETADO");
  }

  if (presionoBotonEncendido) {

    Serial.println ("ROJO");
    digitalWrite (LedRojo, HIGH);
    digitalWrite (LedAmarillo, LOW);
    digitalWrite (LedVerde, LOW);
    digitalWrite (LedChiquito, LOW);
    digitalWrite (LedBlanco, HIGH);
    delay (3000);
    Serial.println ("AMARILLO");
    digitalWrite (LedRojo, LOW);
    digitalWrite (LedAmarillo, HIGH);
    delay (1500);
    Serial.println ("VERDE");
    digitalWrite (LedAmarillo, LOW);
    digitalWrite (LedVerde, HIGH);
    digitalWrite (LedChiquito, HIGH);
    digitalWrite (LedBlanco, LOW);
    delay (3000);  
    Serial.println ("AMARILLO");  
    digitalWrite (LedRojo, LOW);
    digitalWrite (LedAmarillo, HIGH);
    digitalWrite (LedVerde, LOW);
    delay (1500);

    //--------------------------------------------------


    Serial.println ("ROJO");
    digitalWrite (LedRojo, HIGH);
    digitalWrite (LedAmarillo, LOW);
    digitalWrite (LedVerde, LOW);
    digitalWrite (LedChiquito, LOW);
    digitalWrite (LedBlanco, HIGH);
    delay (3000);
    Serial.println ("AMARILLO");
    digitalWrite (LedRojo, LOW);
    digitalWrite (LedAmarillo, HIGH);
    delay (1500);
    Serial.println ("VERDE");
    digitalWrite (LedAmarillo, LOW);
    digitalWrite (LedVerde, HIGH);
    digitalWrite (LedChiquito, HIGH);
    digitalWrite (LedBlanco, LOW);
    delay (3000);
    Serial.println ("AMARILLO");
    digitalWrite (LedRojo, LOW);
    digitalWrite (LedAmarillo, HIGH);
    digitalWrite (LedVerde, LOW);
    delay (1500);
    Serial.println ("APAGANDO...");
    digitalWrite (LedRojo, LOW);
    digitalWrite (LedAmarillo, LOW);
    digitalWrite (LedVerde, LOW);
    digitalWrite (LedChiquito, LOW);
    digitalWrite (LedBlanco, LOW);


    presionoBotonEncendido = 0;
  }

}
