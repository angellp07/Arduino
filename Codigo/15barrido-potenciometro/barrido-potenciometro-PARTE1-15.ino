/*
 * ============================================================
 * SERVOMOTOR CONTROLADO POR POTENCIÓMETRO - EXPLICACIÓN COMPLETA
 * Arduino UNO
 * ============================================================
 * 
 * OBJETIVO: Controlar un servomotor usando un potenciómetro.
 *           Girar el potenciómetro mueve el servo proporcionalmente.
 * 
 * CONEXIONES REQUERIDAS:
 * 
 * 1. SERVOMOTOR (3 cables):
 *    Cable ROJO/NARANJA  → 5V del Arduino
 *    Cable MARRÓN/NEGRO  → GND del Arduino
 *    Cable AMARILLO/BLANCO → Pin 9 del Arduino (señal PWM)
 * 
 * 2. POTENCIÓMETRO (3 terminales):
 *    Terminal izquierdo  → GND del Arduino
 *    Terminal central    → Pin A0 del Arduino (señal analógica)
 *    Terminal derecho    → 5V del Arduino
 * 
 * MATERIALES:
 *    - Servomotor SG90 o similar
 *    - Potenciómetro de 10K ohmios
 *    - Cables dupont para conexiones
 * ============================================================
 */

// ==================== PARTE 1: INCLUSIÓN DE LIBRERÍA ====================
#include <Servo.h>  
/*
 * ¿QUÉ HACE ESTA LÍNEA?
 * Incluye la librería Servo.h necesaria para controlar servomotores.
 * Esta librería proporciona funciones como:
 *   - myservo.attach(pin)  → Conectar servo a un pin
 *   - myservo.write(ángulo) → Mover servo a posición específica
 */

// ==================== PARTE 2: DECLARACIÓN DE VARIABLES ====================
Servo myservo;       // Crea un objeto 'myservo' para controlar el servo
int potpin = 0;      // Pin analógico donde se conecta el potenciómetro (A0)
int val;             // Variable para almacenar el valor leído del potenciómetro

/*
 * EXPLICACIÓN DE LAS VARIABLES:
 * 
 * 1. Servo myservo:
 *    - Objeto que representa al servomotor físico
 *    - Permite usar funciones como write(), attach(), etc.
 * 
 * 2. int potpin = 0:
 *    - Pin A0 (analógico 0) donde se conecta el potenciómetro
 *    - Los pines analógicos A0-A5 pueden leer valores de 0 a 1023
 *    - El valor 0 representa el pin A0
 * 
 * 3. int val:
 *    - Variable que almacenará el valor leído del potenciómetro
 *    - Valores posibles: 0 a 1023 (10 bits de resolución)
 */

// ==================== PARTE 3: SETUP() - CONFIGURACIÓN INICIAL ====================
void setup() {
  myservo.attach(9);  // Conecta el servomotor al pin digital 9
}

/*
 * FUNCIÓN setup():
 * Se ejecuta UNA SOLA VEZ al iniciar el Arduino.
 * 
 * myservo.attach(9):
 *   - Asocia el objeto 'myservo' con el PIN 9 del Arduino
 *   - El pin 9 debe ser PWM (Pulse Width Modulation)
 *   - Pines PWM en Arduino UNO: 3, 5, 6, 9, 10, 11
 *   - PWM permite controlar la posición mediante pulsos
 */

// ==================== PARTE 4: LOOP() - BUCLE PRINCIPAL ====================
void loop() {
  // PASO 1: Leer el valor del potenciómetro
  val = analogRead(potpin);
  /*
   * analogRead(potpin):
   *   - Lee el valor del pin analógico A0 (potpin = 0)
   *   - Retorna un valor entre 0 y 1023
   *   - 0 = 0 voltios (GND)
   *   - 1023 = 5 voltios (VCC)
   *   - Valor proporcional a la posición del potenciómetro
   */
  
  // PASO 2: Convertir el rango 0-1023 a 0-180 grados
  val = map(val, 0, 1023, 0, 180);
  /*
   * map(valor, desdeBajo, desdeAlto, haciaBajo, haciaAlto):
   *   - Convierte un valor de un rango a otro
   *   - En este caso: 0-1023 → 0-180
   *   - Ejemplos:
   *       * 0 → 0 grados
   *       * 511 → 90 grados
   *       * 1023 → 180 grados
   *   - Función lineal: cada 5.68 unidades = 1 grado
   */
  
  // PASO 3: Mover el servomotor a la posición calculada
  myservo.write(val);
  /*
   * myservo.write(valor):
   *   - Mueve el servo al ángulo especificado (0-180°)
   *   - Internamente convierte el ángulo a pulsos PWM
   *   - El servo mantiene la posición hasta nueva instrucción
   */
  
  // PASO 4: Pequeña pausa para estabilidad
  delay(15);
  /*
   * delay(15):
   *   - Pausa de 15 milisegundos
   *   - Da tiempo al servo para alcanzar la posición
   *   - Evita lectura/escritura demasiado rápida
   *   - Sin delay, el código se ejecutaría miles de veces por segundo
   */
}

/*
 * ============================================================
 * PROCESO COMPLETO EN CADA CICLO:
 * ============================================================
 * 1. Leer posición potenciómetro (0-1023)
 * 2. Convertir a ángulo servo (0-180°)
 * 3. Mover servo a ese ángulo
 * 4. Esperar 15ms
 * 5. Repetir desde paso 1
 * 
 * VELOCIDAD DE ACTUALIZACIÓN:
 *   1000ms / 15ms = ~66.7 ciclos por segundo
 *   Suficiente para control manual en tiempo real
 * ============================================================
 */

/*
 * ============================================================
 * DIAGRAMA DE CONEXIONES DETALLADO:
 * ============================================================
 * 
 * ARDUINO UNO ←→ POTENCIÓMETRO (10KΩ):
 * ┌─────────────────────────────────────────────┐
 * │       POTENCIÓMETRO        │    ARDUINO     │
 * ├─────────────────────────────┼────────────────┤
 * │ Terminal IZQUIERDO (GND)   │   GND          │
 * │ Terminal CENTRAL (señal)   │   A0 (Pin 14)  │
 * │ Terminal DERECHO (VCC)     │   5V           │
 * └─────────────────────────────┴────────────────┘
 * 
 * ARDUINO UNO ←→ SERVOMOTOR:
 * ┌─────────────────────────────┬────────────────┐
 * │       SERVOMOTOR           │    ARDUINO     │
 * ├─────────────────────────────┼────────────────┤
 * │ Cable ROJO (VCC)           │   5V           │
 * │ Cable MARRÓN (GND)         │   GND          │
 * │ Cable AMARILLO (señal)     │   Pin 9 (PWM)  │
 * └─────────────────────────────┴────────────────┘
 * 
 * NOTA: Para servos grandes (>200mA), usar fuente externa
 * ============================================================
 */

/*
 * ============================================================
 * EJEMPLOS DE VALORES:
 * ============================================================
 * 
 * POSICIÓN POTENCIÓMETRO → VALOR ANALOGREAD → ÁNGULO SERVO
 * ──────────────────────────────────────────────────────────
 * Totalmente a la izquierda  → 0      → 0°
 * 25% girado                 → ~256   → 45°
 * 50% girado (centro)        → ~512   → 90°
 * 75% girado                 → ~768   → 135°
 * Totalmente a la derecha    → 1023   → 180°
 * 
 * FÓRMULA DE CONVERSIÓN:
 *   ángulo = (valor_analógico × 180) / 1023
 * 
 * RESOLUCIÓN:
 *   180° / 1024 pasos = ~0.176° por paso
 *   Suficiente para control preciso
 * ============================================================
 */

/*
 * ============================================================
 * POSIBLES PROBLEMAS Y SOLUCIONES:
 * ============================================================
 * 
 * PROBLEMA 1: Servo no se mueve suavemente
 *   SOLUCIÓN: Aumentar delay(15) a delay(20) o más
 * 
 * PROBLEMA 2: Movimiento brusco o errático
 *   SOLUCIÓN: Verificar conexiones y voltaje (5V estable)
 * 
 * PROBLEMA 3: Servo no llega a 0° o 180°
 *   SOLUCIÓN: Ajustar map(): map(val, 0, 1023, 10, 170)
 * 
 * PROBLEMA 4: Potenciómetro no responde
 *   SOLUCIÓN: Verificar conexiones GND-5V-signal
 * 
 * PROBLEMA 5: Servo vibra en lugar de moverse
 *   SOLUCIÓN: Comprobar alimentación (posible falta de corriente)
 * ============================================================
 */