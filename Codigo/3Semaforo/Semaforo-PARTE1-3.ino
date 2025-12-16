int redled = 3;    // Declara y asigna el pin digital 3 para el LED rojo
int yellowled = 5; // Declara y asigna el pin digital 5 para el LED amarillo  
int greenled = 7;  // Declara y asigna el pin digital 7 para el LED verde

void setup() // Función que se ejecuta una vez al iniciar el programa
{
  pinMode(redled, OUTPUT);     // Configura el pin del LED rojo como SALIDA
  pinMode(yellowled, OUTPUT);  // Configura el pin del LED amarillo como SALIDA
  pinMode(greenled, OUTPUT);   // Configura el pin del LED verde como SALIDA
}

void loop() // Función que se repite indefinidamente después de setup()
{
  // FASE 1: LED VERDE ENCENDIDO
  digitalWrite(greenled, HIGH); // Enciende el LED verde
  delay(5000);                  // Mantiene encendido por 5 segundos
  digitalWrite(greenled, LOW);  // Apaga el LED verde
  
  // FASE 2: LED AMARILLO INTERMITENTE (3 PARPADEOS)
  for(int i=0; i<3; i++)        // Bucle que se repite 3 veces
  {
    delay(500);                 // Espera 0.5 segundos
    digitalWrite(yellowled, HIGH); // Enciende el LED amarillo
    delay(500);                 // Mantiene encendido 0.5 segundos
    digitalWrite(yellowled, LOW);  // Apaga el LED amarillo
  }
  
  // FASE 3: LED ROJO ENCENDIDO
  delay(500);                   // Espera 0.5 segundos adicionales
  digitalWrite(redled, HIGH);  // Enciende el LED rojo
  delay(5000);                 // Mantiene encendido por 5 segundos
  digitalWrite(redled, LOW);   // Apaga el LED rojo
  
  // El ciclo se reinicia automáticamente, volviendo a la FASE 1
}
