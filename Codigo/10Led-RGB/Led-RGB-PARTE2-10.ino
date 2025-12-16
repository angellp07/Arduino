/*PRUEBA DE COLORES (MONITOR SERIE)*/


int redPin = 11;
int greenPin = 10;
int bluePin = 9;

// NO necesitas #define COMMON_ANODE porque es CÁTODO COMÚN

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("LED RGB configurado como CÁTODO COMÚN");
  Serial.println("(Común conectado a GND)");
  Serial.println("\nIniciando secuencia de colores...");
  
  // Prueba rápida de los 3 colores
  Serial.println("\nPRUEBA RÁPIDA:");
  testAllColors();
}

void loop() {
  // Secuencia automática de colores
  Serial.println("\n--- SECUENCIA PRINCIPAL ---");
  
  setColor(255, 0, 0);   // ROJO
  Serial.println("Color: ROJO");
  delay(1500);
  
  setColor(0, 255, 0);   // VERDE
  Serial.println("Color: VERDE");  
  delay(1500);
  
  setColor(0, 0, 255);   // AZUL
  Serial.println("Color: AZUL");
  delay(1500);
  
  setColor(255, 255, 0); // AMARILLO
  Serial.println("Color: AMARILLO");
  delay(1500);
  
  setColor(255, 0, 255); // MAGENTA (Púrpura)
  Serial.println("Color: MAGENTA");
  delay(1500);
  
  setColor(0, 255, 255); // CIAN (Aqua)
  Serial.println("Color: CIAN");
  delay(1500);
  
  setColor(255, 255, 255); // BLANCO
  Serial.println("Color: BLANCO");
  delay(1500);
  
  setColor(0, 0, 0);     // APAGADO
  Serial.println("Color: APAGADO");
  delay(1500);
}

void setColor(int red, int green, int blue) {
  // SIN inversión porque es CÁTODO COMÚN
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void testAllColors() {
  // Prueba individual de cada color
  Serial.println("Rojo:");
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  delay(2000);
  
  Serial.println("Verde:");
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
  delay(2000);
  
  Serial.println("Azul:");
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
  delay(2000);
  
  // Apagar todo
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  delay(1000);
}
