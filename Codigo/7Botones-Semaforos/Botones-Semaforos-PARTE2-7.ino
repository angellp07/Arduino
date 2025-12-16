/*SEMAFORO, PARA APAGAR UN LED PRECIONA EL BOTON Y LOS DEMAS SEGUIRAN FUNCIONANDO*/
// COMENTARIO GENERAL: Sistema de semáforo donde cada botón controla un color específico

// Definición de pines
const int Redled = 8;     // LED Rojo conectado al pin digital 8
const int Yellowled = 7;  // LED Amarillo conectado al pin digital 7
const int Greenled = 6;   // LED Verde conectado al pin digital 6
const int Key1 = 5;       // Botón para controlar el LED Rojo (pin 5)
const int Key2 = 4;       // Botón para controlar el LED Amarillo (pin 4)
const int Key3 = 3;       // Botón para controlar el LED Verde (pin 3)
const int KeyRest = 2;    // Botón de Reset para apagar todos los LEDs (pin 2)

// Variables para debounce (anti-rebote)
unsigned long lastDebounceTime = 0;  // Variable para almacenar el último tiempo de detección
const unsigned long debounceDelay = 50;  // Tiempo de espera para evitar rebotes (50ms)
// NOTA: Esta variable lastDebounceTime se declara pero NO se usa en el código

// Estado actual del semáforo
int currentState = 0;  // Variable que guarda el estado actual:
                       // 0 = todos apagados
                       // 1 = rojo encendido
                       // 2 = amarillo encendido
                       // 3 = verde encendido

void setup() {
  // Configurar LEDs como salidas
  pinMode(Redled, OUTPUT);    // Pin 8 como salida para el LED rojo
  pinMode(Yellowled, OUTPUT); // Pin 7 como salida para el LED amarillo
  pinMode(Greenled, OUTPUT);  // Pin 6 como salida para el LED verde
  
  // Configurar botones con resistencias PULLUP internas
  pinMode(Key1, INPUT_PULLUP);   // Pin 5 como entrada con resistencia PULLUP interna
  pinMode(Key2, INPUT_PULLUP);   // Pin 4 como entrada con resistencia PULLUP interna
  pinMode(Key3, INPUT_PULLUP);   // Pin 3 como entrada con resistencia PULLUP interna
  pinMode(KeyRest, INPUT_PULLUP); // Pin 2 como entrada con resistencia PULLUP interna
  // INPUT_PULLUP: Arduino activa resistencia interna que mantiene el pin en HIGH (5V)
  // Cuando se presiona el botón, el pin se conecta a GND y lee LOW (0V)
  
  // Apagar todos los LEDs al inicio
  clear_led();  // Llama a la función que apaga todos los LEDs
}

void loop() {
  // Leer botones (con lógica invertida por PULLUP)
  // LOW = botón presionado, HIGH = no presionado
  int redBtn = digitalRead(Key1);    // Leer estado del botón rojo (pin 5)
  int yellowBtn = digitalRead(Key2); // Leer estado del botón amarillo (pin 4)
  int greenBtn = digitalRead(Key3);  // Leer estado del botón verde (pin 3)
  int resetBtn = digitalRead(KeyRest); // Leer estado del botón reset (pin 2)
  
  // Detectar si se presionó el botón de reset
  if (resetBtn == LOW) {  // Si el botón reset está presionado (LOW)
    delay(50);  // Espera 50ms para estabilizar (debounce simple)
    if (digitalRead(KeyRest) == LOW) {  // Vuelve a leer para confirmar
      currentState = 0;  // Cambia estado a 0 (apagado)
      clear_led();       // Apaga todos los LEDs
    }
  }
  
  // Manejar botón rojo
  if (redBtn == LOW) {  // Si el botón rojo está presionado
    delay(50);  // Debounce de 50ms
    if (digitalRead(Key1) == LOW) {  // Confirmar que sigue presionado
      setRed();      // Llama función para encender rojo
      currentState = 1;  // Actualiza estado a 1 (rojo)
    }
  }
  
  // Manejar botón amarillo
  if (yellowBtn == LOW) {  // Si el botón amarillo está presionado
    delay(50);  // Debounce
    if (digitalRead(Key2) == LOW) {  // Confirmar
      setYellow();   // Llama función para encender amarillo
      currentState = 2;  // Actualiza estado a 2 (amarillo)
    }
  }
  
  // Manejar botón verde
  if (greenBtn == LOW) {  // Si el botón verde está presionado
    delay(50);  // Debounce
    if (digitalRead(Key3) == LOW) {  // Confirmar
      setGreen();   // Llama función para encender verde
      currentState = 3;  // Actualiza estado a 3 (verde)
    }
  }
  
  // Pequeña pausa para estabilidad
  delay(10);  // Espera 10ms antes de repetir el loop
}

// Función para encender LED rojo y apagar los demás
void setRed() {
  digitalWrite(Redled, HIGH);    // Enciende LED rojo (5V en pin 8)
  digitalWrite(Yellowled, LOW);  // Apaga LED amarillo (0V en pin 7)
  digitalWrite(Greenled, LOW);   // Apaga LED verde (0V en pin 6)
}

// Función para encender LED amarillo y apagar los demás
void setYellow() {
  digitalWrite(Redled, LOW);     // Apaga LED rojo
  digitalWrite(Yellowled, HIGH); // Enciende LED amarillo
  digitalWrite(Greenled, LOW);   // Apaga LED verde
}

// Función para encender LED verde y apagar los demás
void setGreen() {
  digitalWrite(Redled, LOW);     // Apaga LED rojo
  digitalWrite(Yellowled, LOW);  // Apaga LED amarillo
  digitalWrite(Greenled, HIGH);  // Enciende LED verde
}

// Función para apagar todos los LEDs
void clear_led() {
  digitalWrite(Redled, LOW);     // Apaga LED rojo
  digitalWrite(Yellowled, LOW);  // Apaga LED amarillo
  digitalWrite(Greenled, LOW);   // Apaga LED verde
}
