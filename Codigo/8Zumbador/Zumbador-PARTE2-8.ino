/*ZUMBIDO EN INTERVALOS*/

int buzzer = 5;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  digitalWrite(buzzer, HIGH);  // Buzzer ENCENDIDO
  delay(1000);                  // Espera 1 segundo (sonando)
  digitalWrite(buzzer, LOW);   // Buzzer APAGADO
  delay(1000);                  // Espera 1 segundo (silencio)
  // Resultado: Pitido intermitente cada segundo
}

