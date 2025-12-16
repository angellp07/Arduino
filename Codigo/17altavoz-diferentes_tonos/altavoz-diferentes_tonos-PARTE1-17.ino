/*
 * ============================================================
 * FOTORESISTOR CONTROLA TONO - EXPLICACIÓN COMPLETA
 * Arduino UNO
 * ============================================================
 * 
 * OBJETIVO: Crear un Theremin óptico simple.
 *           La luz que incide en el fotoresistor controla
 *           el tono (frecuencia) de un sonido en el altavoz.
 * 
 * PRINCIPIO FÍSICO: 
 *   Más luz → Menor resistencia → Mayor voltaje → Tono más agudo
 *   Menos luz → Mayor resistencia → Menor voltaje → Tono más grave
 * 
 * CONEXIONES REQUERIDAS:
 * 
 * 1. FOTORESISTOR (LDR - Light Dependent Resistor):
 *    Un terminal → 5V
 *    Otro terminal → A0 y Resistor 10KΩ a GND
 *    (Divisor de voltaje: A0 mide voltaje entre fotoresistor y resistor)
 * 
 * 2. ALTAVOZ (8Ω):
 *    Terminal positivo (+) → Pin 11 (con resistor 100Ω en serie)
 *    Terminal negativo (-) → GND
 * 
 * MATERIALES DEL KIT S19:
 *    - Fotoresistor (LDR)
 *    - Altavoz piezo o pequeño altavoz
 *    - Resistor 10KΩ para divisor de voltaje
 *    - Resistor 100Ω para proteger el altavoz (opcional pero recomendado)
 * ============================================================
 */

// ==================== PARTE 1: DECLARACIÓN DE VARIABLES ====================
int speakerPin = 11;    // Pin donde se conecta el altavoz (PWM)
int photocellPin = A0;  // Pin analógico donde se conecta el fotoresistor

/*
 * EXPLICACIÓN DE LAS VARIABLES:
 * 
 * 1. int speakerPin = 11:
 *    - Pin 11 es PWM (puede generar tonos con tone())
 *    - Otros pines PWM útiles: 3, 5, 6, 9, 10
 *    - El altavoz necesita una señal PWM para producir sonido
 * 
 * 2. int photocellPin = A0:
 *    - Pin analógico A0 (14 en numeración digital)
 *    - Lee valores de 0 a 1023 (10 bits)
 *    - 0 = 0V (oscuridad total)
 *    - 1023 = 5V (luz brillante directa)
 */

// ==================== PARTE 2: SETUP() - CONFIGURACIÓN INICIAL ====================
void setup()
{
  /*
   * FUNCIÓN setup():
   * ¡ESTÁ VACÍA! Esto es inusual pero válido.
   * 
   * Normalmente aquí configuraríamos:
   *   - pinMode() para pines
   *   - Serial.begin() para monitoreo
   *   - Inicialización de variables
   * 
   * ¿POR QUÉ ESTÁ VACÍA?
   * 1. Los pines analógicos (A0) no necesitan pinMode() por defecto
   * 2. El tone() funciona sin configuración inicial
   * 3. Es un programa mínimo para demostración
   * 
   * PROBLEMA: Sin Serial.begin(), no podemos depurar
   */
}

// ==================== PARTE 3: LOOP() - BUCLE PRINCIPAL ====================
void loop()
{
  // PASO 1: Leer el valor del fotoresistor
  int reading = analogRead(photocellPin);
  /*
   * analogRead(photocellPin):
   *   - Lee el pin A0 (fotoresistor)
   *   - Valores típicos:
   *       * Oscuridad completa: 0-50
   *       * Luz ambiente: 200-500
   *       * Luz directa: 800-1023
   *   - Depende de la configuración del divisor de voltaje
   */
  
  // PASO 2: Calcular la frecuencia del tono
  int pitch = 200 + reading / 4;
  /*
   * CÁLCULO: pitch = 200 + reading / 4
   * 
   * EJEMPLOS:
   *   reading = 0 (oscuridad) → 200 + 0/4 = 200 Hz (grave)
   *   reading = 400 (luz media) → 200 + 400/4 = 300 Hz
   *   reading = 800 (luz brillante) → 200 + 800/4 = 400 Hz
   *   reading = 1023 (máxima luz) → 200 + 1023/4 ≈ 456 Hz
   * 
   * RANGO DE FRECUENCIAS:
   *   Mínimo: 200 Hz (grave, como Do bajo)
   *   Máximo: ~456 Hz (agudo, como La sostenido)
   * 
   * ESCALA MUSICAL APROXIMADA:
   *   200 Hz ≈ G3 (Sol)
   *   262 Hz ≈ C4 (Do central)
   *   330 Hz ≈ E4 (Mi)
   *   392 Hz ≈ G4 (Sol)
   *   456 Hz ≈ A#4 (La sostenido)
   */
  
  // PASO 3: Generar el tono en el altavoz
  tone(speakerPin, pitch);
  /*
   * tone(pin, frecuencia):
   *   - Genera una onda cuadrada de la frecuencia especificada
   *   - El altavoz vibra a esa frecuencia produciendo el sonido
   *   - Sigue sonando hasta que se llame noTone() o otro tone()
   * 
   * IMPORTANTE: tone() usa temporizadores internos del Arduino
   *             No bloquea el programa (funciona en segundo plano)
   * 
   * PROBLEMA EN ESTE CÓDIGO:
   *   Siempre está generando tono, nunca se detiene
   *   Esto puede saturar el sistema
   */
}
