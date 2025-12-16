int potPin = A0;     // Potenciómetro en A0
int ledPin = 4;     // LED interno del Arduino (pin 4)
int valor = 0;       // Variable para guardar la lectura

void setup() {
  pinMode(ledPin, OUTPUT);      // Configurar pin 4 como salida
  Serial.begin(9600);           // Iniciar comunicación serial
}

void loop() {
  // 1. Leer el potenciómetro
  valor = analogRead(potPin);
  
  // 2. Mostrar valor en Monitor Serial
  Serial.print("Potenciómetro: ");
  Serial.println(valor);
  
  // 3. Controlar LED
  digitalWrite(ledPin, HIGH);   // Encender LED
  delay(valor);                 // Espera según valor del potenciómetro
  digitalWrite(ledPin, LOW);    // Apagar LED
  delay(valor);                 // Espera otra vez
}
