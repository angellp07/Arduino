// INCLUSIÓN DE LA LIBRERÍA DHT
#include "DHT.h"
// - #include: Directiva del preprocesador para incluir archivos
// - "DHT.h": Archivo de cabecera de la librería DHT
// - Esta librería maneja la comunicación con sensores DHT11, DHT22, etc.

// DEFINICIÓN DE CONSTANTES
#define DHTPIN 8      // Define DHTPIN como 8 (pin donde está el sensor)
// - #define: Crea una constante del preprocesador (no usa memoria de variables)
// - DHTPIN: Nombre de la constante (puede ser cualquier nombre)
// - 8: Número del pin digital donde está conectado el DHT11

#define DHTTYPE DHT11 // Especifica el tipo de sensor (DHT11 o DHT22)
// - DHTTYPE: Nombre de la constante para el tipo de sensor
// - DHT11: Valor que indica que usamos un sensor DHT11 (no DHT22)

// CREACIÓN DEL OBJETO DHT
DHT dht(DHTPIN, DHTTYPE); 
// - DHT: Clase/objeto de la librería
// - dht: Nombre de nuestra instancia del objeto (puede ser cualquier nombre)
// - (DHTPIN, DHTTYPE): Parámetros del constructor:
//   * Primer parámetro: Pin donde está conectado (8)
//   * Segundo parámetro: Tipo de sensor (DHT11)

// FUNCIÓN setup(): SE EJECUTA UNA VEZ AL INICIAR
void setup() {
  // INICIALIZACIÓN DE COMUNICACIÓN SERIAL
  Serial.begin(9600);
  // - Serial: Objeto para comunicación serie
  // - .begin(): Método para iniciar comunicación
  // - 9600: Velocidad en baudios (bits por segundo)
  // - Permite enviar datos a la computadora por USB
  
  Serial.println("=== SENSOR DHT11 ACTIVO ===");
  // - .println(): Método para imprimir texto y salto de línea
  // - "=== SENSOR DHT11 ACTIVO ===": Mensaje de inicio
  
  // INICIALIZACIÓN DEL SENSOR DHT
  dht.begin();
  // - dht: Nuestro objeto DHT creado anteriormente
  // - .begin(): Método que inicializa el sensor
  // - Configura el pin y prepara el sensor para lecturas
  
  delay(1000);
  // - delay(): Pausa el programa en milisegundos
  // - 1000: 1 segundo de espera para que el sensor se estabilice
}

// FUNCIÓN loop(): SE EJECUTA REPETIDAMENTE DESPUÉS DE setup()
void loop() {
  // PAUSA ENTRE LECTURAS (IMPORTANTE)
  delay(2000);
  // - El DHT11 necesita al menos 2 segundos entre lecturas
  // - Si lees más rápido, dará errores
  
  // LECTURA DE HUMEDAD
  float humedad = dht.readHumidity();
  // - dht.readHumidity(): Método que lee la humedad del sensor
  // - float: Tipo de dato para números decimales
  // - humedad: Variable donde se almacena el valor leído
  // - Devuelve humedad relativa en porcentaje (%)
  
  // LECTURA DE TEMPERATURA
  float temperatura = dht.readTemperature();
  // - dht.readTemperature(): Método que lee la temperatura
  // - Devuelve temperatura en grados Celsius (°C)
  // - Existe también readTemperature(true) para Fahrenheit
  
  // VERIFICACIÓN DE ERROR EN LECTURA
  if (isnan(humedad) || isnan(temperatura)) {
    // - isnan(): Función que verifica si NO es un número (Not a Number)
    // - ||: Operador OR (o) - Si humedad NO es número O temperatura NO es número
    // - Si alguna lectura falló, entra aquí
    
    Serial.println("ERROR: No se pudo leer el sensor!");
    // Mensaje de error
    
    Serial.println("Verifica conexiones y resistencia pull-up");
    // Sugerencia para solucionar el problema
    
    Serial.println("----------------------------------------");
    // Línea separadora
    
    return;
    // - return: Sale de la función loop() actual
    // - No ejecuta el resto del código en este ciclo
    // - El loop() comenzará de nuevo desde el delay(2000)
  }
  
  // IMPRESIÓN DE RESULTADOS EN MONITOR SERIAL
  Serial.print("Humedad: ");
  // - .print(): Similar a println() pero SIN salto de línea
  
  Serial.print(humedad);
  // Imprime el valor de humedad
  
  Serial.print(" %\t");
  // - %: Símbolo de porcentaje
  // - \t: Carácter de tabulación (tabulador)
  // - Separa humedad de temperatura visualmente
  
  Serial.print("Temperatura: ");
  
  Serial.print(temperatura);
  // Imprime el valor de temperatura
  
  Serial.println(" °C");
  // - °C: Símbolo de grados Celsius
  // - println(): Con salto de línea al final
  
  // CÁLCULO DE SENSACIÓN TÉRMICA (OPCIONAL)
  float indiceCalor = dht.computeHeatIndex(temperatura, humedad, false);
  // - dht.computeHeatIndex(): Calcula el índice de calor
  // - Parámetros: (temperatura, humedad, isFahrenheit)
  // - false: Indica que la temperatura está en Celsius (no Fahrenheit)
  // - El índice de calor es cómo "se siente" la temperatura combinada con humedad
  
  Serial.print("Sensación térmica: ");
  
  Serial.print(indiceCalor);
  // Imprime el índice de calor calculado
  
  Serial.println(" °C");
  // Salto de línea
  
  Serial.println("-----------------------------");
  // Línea separadora entre ciclos de lectura
}
