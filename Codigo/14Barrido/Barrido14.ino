/*
 * ============================================================
 * PROGRAMA BÁSICO DE SERVOMOTOR - EXPLICACIÓN COMPLETA
 * Arduino UNO
 * ============================================================
 * 
 * OBJETIVO: Controlar un servomotor que se mueve oscilando
 *           entre 30° y 150° continuamente.
 * 
 * CONEXIONES:
 * Servomotor → Arduino UNO
 *   Cable ROJO/NARANJA  → 5V (alimentación)
 *   Cable MARRÓN/NEGRO  → GND (tierra)
 *   Cable AMARILLO/BLANCO → Pin 9 (señal PWM)
 * 
 * CARACTERÍSTICAS SERVOMOTOR:
 * - Voltaje: 4.8V - 6V
 * - Rango: 0° - 180° (algunos 0°-270°)
 * - Corriente: 100-250mA en movimiento
 * - Torque: 1.2kg/cm - 2.5kg/cm
 * 
 * FUNCIONAMIENTO:
 * 1. Incluye librería Servo.h
 * 2. Crea objeto 'myservo' para controlar el servo
 * 3. Configura pin 9 para señal en setup()
 * 4. En loop(): Mueve de 30°→150°→30° continuamente
 * 5. Delay de 15ms entre cada paso para movimiento suave
 * 
 * CÁLCULOS:
 * - Rango: 30° a 150° = 120° de movimiento
 * - Pasos: 120 pasos (incrementos de 1°)
 * - Tiempo por paso: 15ms
 * - Tiempo por dirección: 120 × 15ms = 1800ms = 1.8 segundos
 * - Ciclo completo: 1.8s × 2 = 3.6 segundos
 * - Ciclos por minuto: 60s ÷ 3.6s ≈ 16.7 ciclos
 * ============================================================
 */

// ======================= PARTE 1: INCLUSIÓN DE LIBRERÍA =======================
#include <Servo.h>  
/* 
 * ¿QUÉ HACE ESTA LÍNEA?
 * Incluye la librería Servo.h que contiene todas las funciones 
 * necesarias para controlar servomotores con Arduino.
 * Sin esta librería, no podríamos usar las funciones myservo.attach() o myservo.write()
 */

// ======================= PARTE 2: CREACIÓN DE OBJETO =======================
Servo myservo;  
/*
 * ¿QUÉ HACE ESTA LÍNEA?
 * Crea un objeto llamado 'myservo' de la clase 'Servo'.
 * Un "objeto" es como una representación virtual del servomotor físico.
 * A través de este objeto controlaremos el servomotor real.
 * 
 * NOTA: Arduino puede controlar hasta 12 servomotores simultáneamente
 * (dependiendo de la placa), cada uno con su propio objeto.
 */

// ======================= PARTE 3: DECLARACIÓN DE VARIABLE =======================
int pos = 0;    
/*
 * ¿QUÉ HACE ESTA LÍNEA?
 * Declara una variable entera llamada 'pos' (de position = posición)
 * que almacenará el ángulo actual del servomotor.
 * 
 * VALORES:
 * - Rango típico: 0° a 180° (para servos estándar)
 * - Este código usa: 30° a 150° (para proteger el servo)
 * - Se inicializa en 0°, pero luego se usará desde 30°
 */

// ======================= PARTE 4: SETUP() - CONFIGURACIÓN INICIAL =======================
void setup() 
{
  /*
   * FUNCIÓN setup():
   * Se ejecuta UNA SOLA VEZ cuando el Arduino se enciende o se resetea.
   * Aquí configuramos todo lo que necesita preparación inicial.
   */
  
  myservo.attach(9);  
  /*
   * ¿QUÉ HACE ESTA LÍNEA?
   * Conecta (attach) el objeto 'myservo' al PIN 9 del Arduino.
   * 
   * IMPORTANTE:
   * - El pin 9 debe ser de salida PWM (Pulse Width Modulation)
   * - PWM permite controlar la posición mediante pulsos eléctricos
   * - El cable de señal (amarillo/blanco) del servo va al pin 9
   * 
   * ¿POR QUÉ PIN 9?
   * Los pines PWM en Arduino UNO son: 3, 5, 6, 9, 10, 11
   * Cualquiera de estos funciona para servomotores.
   */
}

// ======================= PARTE 5: LOOP() - BUCLE PRINCIPAL =======================
void loop()
{
  /*
   * FUNCIÓN loop():
   * Se ejecuta CONTINUAMENTE después de setup().
   * Todo lo que pongas aquí se repite una y otra vez.
   * 
   * Este loop contiene DOS bucles for:
   * 1. Primer for: Mueve servo de 30° a 150° (incrementando)
   * 2. Segundo for: Mueve servo de 150° a 30° (decrementando)
   */

  // ============ PRIMER BUCLE FOR: 30° → 150° (IDA) ============
  for (pos = 30; pos <= 150; pos += 1) {
    /*
     * ESTRUCTURA DEL BUCLE FOR:
     * for(inicialización; condición; incremento) { ... }
     * 
     * pos = 30     → Inicia la variable pos en 30 grados
     * pos <= 150   → Continúa mientras pos sea menor o igual a 150
     * pos += 1     → Aumenta pos en 1 grado cada ciclo
     * 
     * ¿POR QUÉ 30° Y NO 0°?
     * Por seguridad. Muchos servos se dañan si los forzamos
     * a los límites extremos (0° o 180°). 30°-150° es más seguro.
     */
    
    myservo.write(pos);  
    /*
     * ¿QUÉ HACE ESTA LÍNEA?
     * Envía la posición 'pos' al servomotor.
     * 
     * FUNCIONAMIENTO INTERNO:
     * 1. Convierte el ángulo (0-180) a un ancho de pulso (500-2400µs)
     * 2. Envía pulsos PWM al pin 9
     * 3. El servo interpreta los pulsos y se mueve a esa posición
     */
    
    delay(15);  
    /*
     * ¿QUÉ HACE ESTA LÍNEA?
     * Pausa el programa durante 15 milisegundos.
     * 
     * ¿POR QUÉ 15ms?
     * - Da tiempo al servo para alcanzar la nueva posición
     * - Controla la velocidad del movimiento
     * - Más delay = movimiento más lento
     * - Menos delay = movimiento más rápido
     * 
     * TIEMPO TOTAL PRIMER BUCLE:
     * 120 pasos × 15ms = 1800ms = 1.8 segundos
     */
  }

  // ============ SEGUNDO BUCLE FOR: 150° → 30° (VUELTA) ============
  for (pos = 150; pos >= 30; pos -= 1) {
    /*
     * pos = 150    → Inicia en 150 grados (donde terminó el primer bucle)
     * pos >= 30    → Continúa mientras pos sea mayor o igual a 30
     * pos -= 1     → Disminuye pos en 1 grado cada ciclo
     * 
     * Este bucle es el inverso del anterior.
     * Juntos crean un movimiento oscilante continuo.
     */
    
    myservo.write(pos);  // Envía nueva posición al servo
    delay(15);           // Misma pausa para velocidad consistente
  }
  
  /*
   * ¿QUÉ PASA CUANDO TERMINA EL SEGUNDO BUCLE?
   * El programa vuelve al inicio de loop()
   * y todo se repite indefinidamente:
   * 30°→150°→30°→150°→30°...
   */
}

/*
 * ============================================================
 * RESUMEN DEL PROCESO COMPLETO:
 * ============================================================
 * 1. Arduino se enciende → Ejecuta setup() una vez
 *    - Incluye librería Servo
 *    - Crea objeto myservo
 *    - Conecta servo a pin 9
 * 
 * 2. Arduino entra en loop() y se repite para siempre:
 *    - Bucle 1: pos=30,31,32... hasta 150 (15ms entre cada)
 *    - Bucle 2: pos=150,149,148... hasta 30 (15ms entre cada)
 *    - Vuelve al inicio de loop()
 * 
 * 3. Resultado físico:
 *    - Servomotor oscila suavemente entre 30° y 150°
 *    - Cada ciclo completo (ida y vuelta) toma 3.6 segundos
 *    - El movimiento continúa indefinidamente
 * ============================================================
 */

/*
 * ============================================================
 * CONSEJOS:
 * ============================================================
 * 
 * CONEXIÓN CORRECTA:
 * El servomotor tiene 3 cables:
 * 1. ROJO (5V) → Conectar a puerto 5V del Arduino o shield
 * 2. MARRÓN (GND) → Conectar a GND del Arduino o shield  
 * 3. AMARILLO (Señal) → Conectar a PIN 9 (como indica el código)
 * 
 * PRECAUCIONES:
 * 1. NO forzar el servo manualmente cuando esté encendido
 * 2. NO conectar al revés los cables (podría dañarse)
 * 3. Si usas batería, asegurar voltaje entre 4.8V y 6V
 * 4. Para servos grandes, usar fuente externa (no pin 5V Arduino)
 * 
 * PRUEBAS:
 * 1. Al subir el código, el servo debería comenzar a moverse
 * 2. Si no se mueve, verificar conexiones y voltaje
 * 3. Si vibra o se mueve errático, aumentar delay(15) a delay(20)
 * 
 * MODIFICACIONES SEGURAS QUE PUEDES HACER:
 * 1. Cambiar delay(15) para más rápido/más lento
 * 2. Cambiar rango (30,150) pero mantener dentro de 0-180
 * 3. Cambiar pin 9 por otro pin PWM (3,5,6,10,11)
 * ============================================================
 */

