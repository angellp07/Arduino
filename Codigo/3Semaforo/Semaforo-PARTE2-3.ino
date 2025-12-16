// DEFINICIÓN DE PINES
const int RED_LED = 3;
const int YELLOW_LED = 5;
const int GREEN_LED = 7;

// DEFINICIÓN DE TIEMPOS
const unsigned long GREEN_TIME = 5000;     // 5 segundos
const unsigned long RED_TIME = 5000;       // 5 segundos  
const unsigned long BLINK_TIME = 500;      // 500ms
const int BLINK_COUNT = 3;                 // 3 parpadeos

// Estados del semáforo
enum TrafficLightState {
  STATE_GREEN,
  STATE_YELLOW_BLINK,
  STATE_RED
};

TrafficLightState currentState = STATE_GREEN;
unsigned long previousTime = 0;
int blinkCounter = 0;
bool yellowLedState = false;
bool initialized = false;  // Bandera para controlar inicialización

void setup() {
  // Configuración de pines
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  // Apagar todos los LEDs inicialmente
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  
  // Iniciar comunicación serial
  Serial.begin(9600);
  Serial.println("Semáforo inicializado - Versión mejorada");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Inicializar previousTime en la primera ejecución
  if (!initialized) {
    previousTime = currentTime;
    initialized = true;
    digitalWrite(GREEN_LED, HIGH);  // Encender verde al inicio
    Serial.println("Estado inicial: VERDE");
  }
  
  switch(currentState) {
    case STATE_GREEN:
      if (currentTime - previousTime >= GREEN_TIME) {
        previousTime = currentTime;
        currentState = STATE_YELLOW_BLINK;
        blinkCounter = 0;
        yellowLedState = false;
        digitalWrite(GREEN_LED, LOW);  // Apagar verde
        Serial.println("Cambiando a: AMARILLO PARPADEANTE");
      }
      break;
      
    case STATE_YELLOW_BLINK:
      if (currentTime - previousTime >= BLINK_TIME) {
        previousTime = currentTime;
        yellowLedState = !yellowLedState;
        digitalWrite(YELLOW_LED, yellowLedState);
        
        if (!yellowLedState) {
          blinkCounter++;
          Serial.print("Parpadeo ");
          Serial.print(blinkCounter);
          Serial.println("/3");
        }
        
        if (blinkCounter >= BLINK_COUNT) {
          currentState = STATE_RED;
          digitalWrite(YELLOW_LED, LOW);
          Serial.println("Cambiando a: ROJO");
        }
      }
      break;
      
    case STATE_RED:
      digitalWrite(RED_LED, HIGH);
      
      if (currentTime - previousTime >= RED_TIME) {
        previousTime = currentTime;
        currentState = STATE_GREEN;
        digitalWrite(RED_LED, LOW);  // Apagar rojo
        digitalWrite(GREEN_LED, HIGH); // Encender verde
        Serial.println("Cambiando a: VERDE");
      }
      break;
  }
}
