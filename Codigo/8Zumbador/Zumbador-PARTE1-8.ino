/*ZUMBIDO MODO DIFERENTES TONOS COMO ALARMA*/

int buzzer = 5;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  tone(buzzer, 1000);  // Tono de 1000 Hz (más agudo)
  delay(500);
  tone(buzzer, 500);   // Tono de 500 Hz (más grave)
  delay(500);
  noTone(buzzer);      // Silencio
  delay(1000);
}