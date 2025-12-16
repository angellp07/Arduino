// DECLARACIÓN DE CONSTANTES PARA LOS PINES
const int buttonPin = 2;     // Define una constante llamada buttonPin con valor 2
                             // Esto significa que el pulsador está conectado al pin digital 2
const int ledPin =  13;      // Define una constante llamada ledPin con valor 13
                             // Esto significa que el LED está conectado al pin digital 13

// DECLARACIÓN DE VARIABLES
int buttonState = 0;         // Crea una variable entera llamada buttonState inicializada en 0
                             // Esta variable almacenará el estado del pulsador:
                             // 0 = LOW (no presionado), 1 = HIGH (presionado)

// FUNCIÓN SETUP - SE EJECUTA UNA SOLA VEZ AL INICIAR
void setup() {
  // Configura el pin del LED como SALIDA:
  pinMode(ledPin, OUTPUT);
  // Explicación: pinMode() configura cómo se comportará un pin
  // OUTPUT significa que el pin enviará señales eléctricas (para controlar el LED)
  
  // Configura el pin del pulsador como ENTRADA:
  pinMode(buttonPin, INPUT);
  // Explicación: INPUT significa que el pin leerá señales eléctricas
  // Esto permite detectar si el pulsador está presionado o no
}

// FUNCIÓN LOOP - SE EJECUTA REPETIDAMENTE DESPUÉS DE SETUP
void loop() {
  // Lee el estado del pulsador y lo guarda en buttonState:
  buttonState = digitalRead(buttonPin);
  // Explicación: digitalRead() lee el valor digital en un pin
  // Devuelve HIGH (1) si hay voltaje (usualmente 5V)
  // Devuelve LOW (0) si no hay voltaje (0V)
  // El valor leído se almacena en la variable buttonState
  
  // Verifica si el pulsador está presionado:
  // Si buttonState es HIGH (1), significa que el pulsador está presionado
  if (buttonState == HIGH) {
    // Enciende el LED:
    digitalWrite(ledPin, HIGH);
    // Explicación: digitalWrite() envía un valor digital a un pin
    // HIGH envía 5V al pin 13, lo que enciende el LED
    // El LED del pin 13 es el integrado en la placa Arduino
    
  } else {
    // Apaga el LED:
    digitalWrite(ledPin, LOW);
    // Explicación: LOW envía 0V al pin 13, lo que apaga el LED
  }
  // El bucle loop() se repite continuamente, verificando el pulsador miles de veces por segundo
}