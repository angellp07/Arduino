/*
 * PROGRAMA PARA CONTROLAR 8 LEDs CON REGISTRO DE DESPLAZAMIENTO 74HC595
 * Arduino UNO 
 * 
 * DESCRIPCIÓN:
 * Este código controla 8 LEDs conectados a un registro de desplazamiento 74HC595
 * usando sólo 3 pines del Arduino, permitiendo ahorrar pines de E/S.
 * 
 * FUNCIONAMIENTO:
 * 1. Apaga todos los LEDs durante 0.5 segundos
 * 2. Enciende los LEDs progresivamente uno a uno cada 0.5 segundos
 * 3. Al final: Todos los LEDs encendidos (11111111 binario)
 * 4. Se repite desde el inicio
 * 
 * CONEXIONES DEL REGISTRO 74HC595:
 * - latchPin (5)  -> ST_CP (pin 12) del 74HC595 (Latch/Store Clock Pulse)
 * - clockPin (6)  -> SH_CP (pin 11) del 74HC595 (Shift Clock Pulse)
 * - dataPin (4)   -> DS    (pin 14) del 74HC595 (Data Serial)
 * - VCC (5V)      -> pin 16 del 74HC595
 * - GND           -> pin 8 del 74HC595
 * - Q0-Q7 (pins 1-7, 15) -> LEDs con resistencias de 220-330Ω
 * 
 * VENTAJAS DEL 74HC595:
 * - Ahorra pines: Controla 8 LEDs con sólo 3 pines del Arduino
 * - Cascadable: Puedes conectar múltiples 74HC595 para más LEDs
 * - Consistente: El estado se mantiene hasta la próxima actualización
 */

// ==============================================================================
// DECLARACIÓN DE VARIABLES Y CONSTANTES (QUITAR "/*" QUE ESTAN CON "---------")|
// ==============================================================================


/*---------------------------------------------------------------------------------------------------------------------------------------------*/
int latchPin = 5;    // Pin ST_CP (Latch) del 74HC595 - Controla cuando se muestran los datos
int clockPin = 6;    // Pin SH_CP (Clock) del 74HC595 - Sincroniza el desplazamiento de bits
int dataPin = 4;     // Pin DS (Data) del 74HC595 - Por donde entran los datos serialmente
byte leds = 0;       // Variable de 8 bits (0-255) que almacena el estado de los 8 LEDs
                     // Cada bit representa un LED: bit 0 = LED1, bit 7 = LED8

// ============================================
// CONFIGURACIÓN INICIAL
// ============================================

void setup() 
{
  // Configura los 3 pines de control como salidas
  pinMode(latchPin, OUTPUT);  // Pin LATCH - Para cargar datos en las salidas
  pinMode(dataPin, OUTPUT);   // Pin DATA  - Datos seriales de entrada
  pinMode(clockPin, OUTPUT);  // Pin CLOCK - Reloj para desplazamiento
  
  // Todos los LEDs comienzan apagados
  digitalWrite(latchPin, LOW);
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
}

// ============================================
// BUCLE PRINCIPAL - SECUENCIA DE LEDs
// ============================================

void loop() 
{
  // PASO 1: Apagar todos los LEDs durante 0.5 segundos
  leds = 0;                    // 00000000 binario - Todos los bits en 0 (LEDs apagados)
  updateShiftRegister();       // Envía el estado al registro 74HC595
  delay(500);                  // Espera 500 ms (0.5 segundos)
  
  // PASO 2: Encendido progresivo de LEDs
  // Itera por cada uno de los 8 LEDs (bits 0 a 7)
  for (int i = 0; i < 8; i++)
  {
    // bitSet(leds, i): Establece el bit 'i' en 1 (enciende el LED correspondiente)
    // El parámetro LSBFIRST en updateShiftRegister() hace que el bit 0 sea el LED más a la derecha
    bitSet(leds, i);          // Enciende el LED en posición i (0-7)
    updateShiftRegister();    // Actualiza el registro con el nuevo estado
    delay(500);               // Espera 500 ms entre cada LED
  }
  
  // Al finalizar el for: leds = 11111111 binario (255 decimal) - Todos los LEDs encendidos
  
  // El ciclo se repite automáticamente:
  // 1. Vuelve a apagar todos los LEDs
  // 2. Vuelve a encender progresivamente
}/*-------------------------------------------------------------------------------------------------------------*/

// ============================================
// FUNCIÓN PARA COMUNICARSE CON EL 74HC595
// ============================================

/*
 * PROCESO DE COMUNICACIÓN CON EL REGISTRO:
 * 1. latchPin = LOW: Prepara el registro para recibir datos (modo "carga")
 * 2. shiftOut(): Envía los 8 bits serialmente
 * 3. latchPin = HIGH: Muestra los datos en las salidas (modo "almacenamiento")
 * 
 * FUNCIONAMIENTO INTERNO DEL 74HC595:
 * - shiftOut() envía cada bit por dataPin con un pulso en clockPin
 * - Los bits se desplazan internamente en el registro
 * - Al poner latchPin HIGH, los bits se copian a las salidas paralelas Q0-Q7
 */

 /*--------------------------------------------------------------------------------------------------------------*/
void updateShiftRegister()
{
   // PASO 1: Preparar el registro para recibir datos nuevos
   digitalWrite(latchPin, LOW);    // Permite que los datos entren al registro de desplazamiento
   
   // PASO 2: Enviar los 8 bits serialmente
   // shiftOut(pinDatos, pinReloj, Orden, valor)
   // - LSBFIRST: Envía primero el LSB (Least Significant Bit - bit menos significativo)
   //   Esto significa que el bit 0 de 'leds' va al primer LED (Q0)
   // - Alternativa: MSBFIRST para orden inverso
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   
   // PASO 3: Mostrar los datos en las salidas del registro
   digitalWrite(latchPin, HIGH);   // Transfiere los datos desplazados a las salidas paralelas
}/*-----------------------------------------------------------------------------------------------------------------*/

// ============================================
// INFORMACIÓN ADICIONAL
// ============================================

/*
 * CONSEJOS
 * 
 * 1. VERIFICAR CONEXIONES:
 *    - El módulo de registro de desplazamiento debe estar correctamente conectado
 *    - Verificar que los jumpers estén en la posición correcta
 *    - Confirmar polaridad de LEDs si no encienden
 * 
 * 2. POSIBLES MODIFICACIONES:
 *    - Cambiar delay(500) para más rápido/más lento
 *    - Usar MSBFIRST en lugar de LSBFIRST para orden inverso
 *    - Crear patrones diferentes modificando la variable 'leds'
 * 
 * 3. EJEMPLO DE PATRÓN ALTERNATIVO (para añadir al loop):
 *    // Patrón de "carrera" o "Knight Rider"
 *    for (int i = 0; i < 8; i++) {
 *      leds = (1 << i);  // Solo un LED encendido que se desplaza
 *      updateShiftRegister();
 *      delay(100);
 *    }
 *    for (int i = 6; i > 0; i--) {
 *      leds = (1 << i);  // Regreso
 *      updateShiftRegister();
 *      delay(100);
 *    }
 * 
 * 4. CONTROLAR MÁS DE 8 LEDs:
 *    - Conectar múltiples 74HC595 en cadena
 *    - Usar un array de bytes en lugar de un solo byte
 *    - Enviar múltiples bytes en la función shiftOut()
 */

/*
 * MAPA DE BITS A LEDs:
 * leds = 0b00000001 (1)  -> Solo LED 1 encendido (Q0)
 * leds = 0b00000011 (3)  -> LEDs 1 y 2 encendidos
 * leds = 0b00000111 (7)  -> LEDs 1, 2 y 3 encendidos
 * leds = 0b11111111 (255)-> Todos los LEDs encendidos
 * 
 * PARA ENCENDER LEDs ESPECÍFICOS:
 * leds = 0b10101010;  // LEDs pares encendidos
 * leds = 0b01010101;  // LEDs impares encendidos
 * leds = 0b11000011;  // LEDs extremos encendidos
 */
