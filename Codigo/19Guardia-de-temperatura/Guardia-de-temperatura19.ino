// INCLUSIÓN DE LA LIBRERÍA NECESARIA
#include <DHT.h>
// Incluye la biblioteca para trabajar con sensores DHT (DHT11, DHT22, etc.)
// Esta biblioteca maneja la comunicación con el sensor

// DEFINICIÓN DEL PIN DONDE ESTÁ CONECTADO EL SENSOR
#define DHTPIN 5
// Asigna el número 5 al nombre DHTPIN
// El sensor DHT11 debe estar conectado físicamente al pin digital 5 del Arduino

// DEFINICIÓN DEL TIPO DE SENSOR
#define DHTTYPE DHT11
// Especifica que estamos usando un sensor DHT11 (no DHT22 u otro)
// Si tuvieras un DHT22, cambiarías esto a: #define DHTTYPE DHT22

// CREACIÓN DEL OBJETO PARA MANEJAR EL SENSOR
DHT dht(DHTPIN, DHTTYPE);
// Crea un objeto llamado "dht" que nos permitirá interactuar con el sensor
// Parámetros: (pin donde está conectado, tipo de sensor)

// DEFINICIÓN DE PINES PARA CONTROLAR LOS DISPOSITIVOS
const int fanControlPin = 3;  // Pin para controlar el ventilador (transistor)
// Este pin controla la base del transistor 2N2222 que activa el ventilador
// HIGH (5V) = transistor conduce = ventilador ENCENDIDO
// LOW (0V) = transistor no conduce = ventilador APAGADO

const int greenLedPin = 12;   // Pin para el LED verde
const int yellowLedPin = 11;  // Pin para el LED amarillo  
const int redLedPin = 10;     // Pin para el LED rojo
// Cada LED se usa como indicador visual del estado de temperatura

// DEFINICIÓN DE UMBRALES DE TEMPERATURA PARA EL VENTILADOR
const float fanOnThreshold = 30.6;   // Temperatura para ENCENDER el ventilador
const float fanOffThreshold = 29.0;  // Temperatura para APAGAR el ventilador
// Esto crea HISTÉRESIS: evita que el ventilador se encienda y apague constantemente
// Ejemplo: Se enciende a 30.6°C, se apaga a 29°C (no a 30.5°C)

// VARIABLE PARA RASTREAR EL ESTADO DEL VENTILADOR
bool fanRunning = false;
// Variable booleana (verdadero/falso) que guarda si el ventilador está encendido
// false = ventilador APAGADO, true = ventilador ENCENDIDO

// DEFINICIÓN DE UMBRALES PARA LOS LEDs INDICADORES
const float tempNormal = 28.0;    // Menos de 28°C = estado NORMAL (LED verde)
const float tempWarning = 30.0;   // 28°C a 30°C = estado ALERTA (LED amarillo)
const float tempDanger = 31.0;    // Más de 30°C = estado PELIGRO (LED rojo)

// FUNCIÓN setup(): SE EJECUTA UNA VEZ AL INICIAR EL PROGRAMA
void setup() {
  // INICIALIZACIÓN DE LA COMUNICACIÓN SERIAL
  Serial.begin(9600);
  // Inicia la comunicación con la computadora a 9600 bits por segundo
  // Permite enviar mensajes al Monitor Serial del Arduino IDE
  
  // INICIALIZACIÓN DEL SENSOR DHT11
  dht.begin();
  // Prepara el sensor DHT11 para comenzar a tomar lecturas
  
  // CONFIGURACIÓN DE PINES COMO SALIDAS
  pinMode(fanControlPin, OUTPUT);  // Pin 3 como salida para controlar el ventilador
  pinMode(greenLedPin, OUTPUT);    // Pin 12 como salida para LED verde
  pinMode(yellowLedPin, OUTPUT);   // Pin 11 como salida para LED amarillo
  pinMode(redLedPin, OUTPUT);      // Pin 10 como salida para LED rojo
  
  // ESTADO INICIAL DE LOS DISPOSITIVOS
  digitalWrite(fanControlPin, LOW);  // Asegura que el ventilador empiece APAGADO
  allLedsOff();                      // Apaga todos los LEDs
  
  // MENSAJES INICIALES EN EL MONITOR SERIAL
  Serial.println("Sistema con transistor 2N2222 en pin D3");
  Serial.println("========================================");
  
  // LLAMADA A LA FUNCIÓN DE PRUEBA
  testFanConnection();
  // Ejecuta una prueba del ventilador al iniciar para verificar que funciona
}

// FUNCIÓN loop(): SE EJECUTA REPETIDAMENTE DESPUÉS DE setup()
void loop() {
  // PAUSA ENTRE LECTURAS
  delay(2000); // Espera 2000 milisegundos = 2 segundos
  // El sensor DHT11 necesita al menos 2 segundos entre lecturas para ser preciso
  
  // LECTURA DE LA TEMPERATURA
  float temperature = dht.readTemperature();
  // Lee la temperatura actual del sensor DHT11
  // El valor se guarda en la variable "temperature" como número decimal
  
  // VERIFICACIÓN DE SI LA LECTURA FUE EXITOSA
  if (isnan(temperature)) {
    // isnan() significa "is not a number" (no es un número)
    // Si la lectura falla, dht.readTemperature() devuelve "NaN" (Not a Number)
    
    Serial.println("Error: No se puede leer el sensor DHT11");
    // Muestra mensaje de error en el Monitor Serial
    
    blinkError(); // Llama a la función que hace parpadear los LEDs para indicar error
    return;       // Sale de esta ejecución del loop() y comienza de nuevo
  }
  
  // MUESTRA LA TEMPERATURA EN EL MONITOR SERIAL
  Serial.print("🌡️ Temperatura: ");
  Serial.print(temperature, 1); // Muestra temperatura con 1 decimal
  Serial.println(" °C");        // Añade unidad y salto de línea
  
  // CONTROL AUTOMÁTICO DEL VENTILADOR
  if (!fanRunning && temperature > fanOnThreshold) {
    // Si: ventilador está APAGADO Y temperatura > 30.6°C
    fanRunning = true;                     // Cambia estado a "encendido"
    digitalWrite(fanControlPin, HIGH);     // Envía 5V al pin 3 (activa transistor)
    Serial.println("🌀 Ventilador: ENCENDIDO (transistor activado)");
    
  } else if (fanRunning && temperature < fanOffThreshold) {
    // Si: ventilador está ENCENDIDO Y temperatura < 29°C
    fanRunning = false;                    // Cambia estado a "apagado"
    digitalWrite(fanControlPin, LOW);      // Envía 0V al pin 3 (desactiva transistor)
    Serial.println("🌀 Ventilador: APAGADO");
  }
  // Nota: Si ninguna condición se cumple, el ventilador mantiene su estado actual
  
  // CONTROL DE LOS LEDs INDICADORES
  controlLeds(temperature);
  // Llama a la función que decide qué LED encender según la temperatura
}

// FUNCIÓN PARA CONTROLAR LOS LEDs SEGÚN LA TEMPERATURA
void controlLeds(float temp) {
  // Parámetro: temp = temperatura actual (número decimal)
  
  allLedsOff(); // Primero apaga todos los LEDs
  
  if (temp < tempNormal) {
    // Si temperatura < 28°C
    digitalWrite(greenLedPin, HIGH);  // Enciende LED verde
    Serial.println("💡 Estado: NORMAL (Verde)");
    
  } else if (temp >= tempNormal && temp < tempWarning) {
    // Si 28°C ≤ temperatura < 30°C
    digitalWrite(yellowLedPin, HIGH); // Enciende LED amarillo
    Serial.println("💡 Estado: ALERTA (Amarillo)");
    
  } else {
    // Si temperatura ≥ 30°C
    digitalWrite(redLedPin, HIGH);    // Enciende LED rojo
    Serial.println("💡 Estado: PELIGRO (Rojo)");
  }
}

// FUNCIÓN PARA APAGAR TODOS LOS LEDs
void allLedsOff() {
  digitalWrite(greenLedPin, LOW);   // Apaga LED verde
  digitalWrite(yellowLedPin, LOW);  // Apaga LED amarillo
  digitalWrite(redLedPin, LOW);     // Apaga LED rojo
}

// FUNCIÓN PARA INDICAR ERROR CON PARPADEO DE LEDs
void blinkError() {
  // Esta función se ejecuta cuando hay error en el sensor
  for (int i = 0; i < 3; i++) {  // Repite 3 veces
    digitalWrite(redLedPin, HIGH); // Enciende LED rojo
    delay(200);                    // Espera 200ms
    allLedsOff();                  // Apaga todos
    delay(200);                    // Espera 200ms
  }
  // Resultado: LED rojo parpadea 3 veces
}

// FUNCIÓN PARA PROBAR LA CONEXIÓN DEL VENTILADOR AL INICIAR
void testFanConnection() {
  Serial.println("🔧 PRUEBA DE CONEXIÓN DEL VENTILADOR:");
  Serial.println("1. Encendiendo ventilador por 3 segundos...");
  digitalWrite(fanControlPin, HIGH); // Activa el ventilador
  delay(3000);                       // Espera 3 segundos
  
  Serial.println("2. Apagando ventilador...");
  digitalWrite(fanControlPin, LOW);  // Desactiva el ventilador
  delay(1000);                       // Espera 1 segundo
  
  Serial.println("✅ Prueba completada. Sistema listo.");
  Serial.println("========================================");
}