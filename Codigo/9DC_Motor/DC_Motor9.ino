
int motorPin = 8;
unsigned long tiempoEncendido = 0;
const unsigned long MAX_TIEMPO = 5000; // 5 segundos máximo

void setup() {
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Control motor con TIP120 - LISTO");
}

void loop() {
  // ENCIENDE con tiempo limitado
  Serial.println("Motor ON (5 segundos max)");
  digitalWrite(motorPin, HIGH);
  tiempoEncendido = millis();
  
  // Control para no sobrecalentar
  while(millis() - tiempoEncendido < MAX_TIEMPO) {
    delay(100);
    // Podemos añadir sensor de temperatura aquí
  }
  
  // APAGA para enfriar
  Serial.println("Motor OFF - Enfriando");
  digitalWrite(motorPin, LOW);
  delay(3000); // Tiempo de enfriamiento
}
