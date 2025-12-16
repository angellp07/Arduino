// SISTEMA BOTÓN + RELÉ - VERSIÓN SIMPLE Y FUNCIONAL
const int buttonPin = 2;    // Pin del botón (usar INPUT_PULLUP)
const int relayPin = 8;     // Pin del relé

// Variables para debounce
int buttonState;            // Estado actual del botón
int lastButtonState = LOW;  // Estado anterior del botón
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Estado del relé
bool relayState = false;    // false = apagado, true = encendido

void setup() {
  // Iniciar comunicación serial para monitoreo
  Serial.begin(9600);
  
  // Configurar pines
  pinMode(buttonPin, INPUT_PULLUP);  // IMPORTANTE: Usar pull-up interno
  pinMode(relayPin, OUTPUT);
  
  // Estado inicial: relé APAGADO
  digitalWrite(relayPin, LOW);
  relayState = false;
  
  // Mensaje inicial
  Serial.println("====================================");
  Serial.println("   SISTEMA BOTÓN + RELÉ");
  Serial.println("====================================");
  Serial.println("Configuración:");
  Serial.println("- Botón en pin 2 (INPUT_PULLUP)");
  Serial.println("- Relé en pin 8");
  Serial.println("- LED integrado en pin 13 como indicador");
  Serial.println("");
  Serial.println("Lógica: LOW = botón presionado");
  Serial.println("Presiona el botón para alternar el relé");
  Serial.println("====================================\n");
}

void loop() {
  // Leer el estado actual del botón
  int reading = digitalRead(buttonPin);
  
  // Si el estado cambió (debido a ruido o presión)
  if (reading != lastButtonState) {
    // Reiniciar el temporizador de debounce
    lastDebounceTime = millis();
  }
  
  // Si ha pasado suficiente tiempo desde el último cambio
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Si el estado del botón ha cambiado
    if (reading != buttonState) {
      buttonState = reading;
      
      // Solo cambiar el estado del relé si el botón fue presionado
      // (flanco descendente en INPUT_PULLUP: HIGH -> LOW)
      if (buttonState == LOW) {
        // Invertir el estado del relé
        relayState = !relayState;
        
        // Aplicar al relé
        digitalWrite(relayPin, relayState ? HIGH : LOW);
        
        // Usar LED integrado como indicador visual
        digitalWrite(13, relayState ? HIGH : LOW);
        
        // Mostrar información en Monitor Serial
        Serial.println("══════════════════════════════");
        Serial.print("🕒 Tiempo: ");
        Serial.print(millis() / 1000.0, 1);
        Serial.println(" segundos");
        Serial.println("🔘 Botón: PRESIONADO");
        Serial.print("⚡ Estado del relé: ");
        Serial.println(relayState ? "ENCENDIDO ✅" : "APAGADO ❌");
        Serial.print("   Pin ");
        Serial.print(relayPin);
        Serial.print(": ");
        Serial.println(relayState ? "HIGH (5V)" : "LOW (0V)");
        Serial.println("══════════════════════════════\n");
      }
    }
  }
  
  // Guardar el estado actual como el último estado
  lastButtonState = reading;
  
  // Pequeña pausa para estabilidad
  delay(10);
}