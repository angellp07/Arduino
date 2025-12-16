/*
 * ============================================================
 * CONTROL COMPLETO 74HC595 - ÁNODO COMÚN (LEDS)
 * ============================================================
 * 
 * DESCRIPCIÓN COMPLETA:
 * Este programa controla 8 LEDs conectados a un registro de 
 * desplazamiento 74HC595 desde Arduino UNO. Los LEDs usan
 * configuración de ÁNODO COMÚN, lo que invierte la lógica:
 *   0 = LED ENCENDIDO, 1 = LED APAGADO
 * 
 * CONEXIONES OBLIGATORIAS:
 * Arduino → 74HC595:
 *   Pin 5 (D5) → Pin 12 (ST_CP/LATCH)   - Control transferencia
 *   Pin 6 (D6) → Pin 11 (SH_CP/CLOCK)   - Sincroniza desplazamiento  
 *   Pin 4 (D4) → Pin 14 (DS/DATA)       - Datos seriales entrada
 *   5V        → Pin 16 (VCC) y Pin 10 (MR) - Alimentación y Reset
 *   GND       → Pin 8 (GND) y Pin 13 (OE)  - Tierra y Habilitación
 * 
 * CONFIGURACIÓN KIT S19 (VERIFICAR):
 *   - OE (Pin 13) DEBE ir a GND (activa salidas)
 *   - MR (Pin 10) DEBE ir a 5V (desactiva reset)
 *   - LEDs en ÁNODO COMÚN (5V común a todos)
 * 
 * COMANDOS DISPONIBLES (Monitor Serial 9600 baudios):
 *   A: Encendido progresivo (ola)
 *   B: Knight Rider (coche fantástico)
 *   C: LEDs alternos (pares/impares)
 *   D: Contador binario 0-15
 *   E: Todos encendidos/apagados (parpadeo)
 *   F: Carrera simple
 *   X: APAGAR todos los LEDs
 *   Y: ENCENDER todos los LEDs
 *   0-7: Alternar LED individual (0=primer LED, 7=último LED)
 * 
 * LÓGICA ÁNODO COMÚN (IMPORTANTE):
 *   Estado inicial: 11111111 = Todos APAGADOS
 *   bitClear(ledsState, n) → Pone bit en 0 → ENCIENDE LED n
 *   bitSet(ledsState, n)   → Pone bit en 1 → APAGA LED n
 * ============================================================
 */

// ================= CONFIGURACIÓN =================
const int LATCH_PIN = 5;     // Pin LATCH (ST_CP) - Controla transferencia datos
const int CLOCK_PIN = 6;     // Pin CLOCK (SH_CP) - Sincroniza desplazamiento bits
const int DATA_PIN = 4;      // Pin DATA (DS)     - Entrada serial de datos

const bool ANODO_COMUN = true;  // CONFIRMADO: Kit S19 usa ánodo común (5V común)
const int NUM_LEDS = 8;         // 8 LEDs controlados por el 74HC595
const int DELAY_BASE = 300;     // Velocidad base animaciones (ms)

byte ledsState = 0b11111111;    // Estado inicial: TODOS APAGADOS (11111111 binario)

// ================= SETUP (INICIALIZACIÓN) =================
void setup() {
  Serial.begin(9600);  // Inicia comunicación serial a 9600 baudios
  
  Serial.println("\n========================================");
  Serial.println("CONTROL 74HC595");
  Serial.println("========================================");
  Serial.println("CONFIGURACIÓN: ÁNODO COMÚN CONFIRMADO");
  Serial.println("  0 = LED ENCENDIDO | 1 = LED APAGADO");
  Serial.println("  Estado inicial: 11111111 (todos apagados)");
  Serial.println("========================================\n");
  
  // Configurar pines Arduino como SALIDAS
  pinMode(LATCH_PIN, OUTPUT);   // Pin LATCH como salida
  pinMode(CLOCK_PIN, OUTPUT);   // Pin CLOCK como salida  
  pinMode(DATA_PIN, OUTPUT);    // Pin DATA como salida
  
  // Estado inicial: enviar 11111111 al 74HC595 (todos LEDs APAGADOS)
  actualizarRegistro(ledsState);
  
  delay(1000);  // Pausa inicial
  
  // Ejecutar demostración automática de prueba
  demostracionInicial();
}

// ================= LOOP (BUCLE PRINCIPAL) =================
void loop() {
  mostrarMenu();     // Mostrar opciones en Monitor Serial
  esperarComando();  // Esperar y procesar comando del usuario
}

// ================= FUNCIÓN PRINCIPAL: ACTUALIZAR REGISTRO =================
/*
 * FUNCIÓN: actualizarRegistro(byte data)
 * PROPÓSITO: Enviar datos al registro 74HC595
 * PROCESO en 3 pasos:
 *   1. LATCH = LOW: Prepara registro para recibir datos
 *   2. shiftOut(): Envía 8 bits serialmente (MSB primero)
 *   3. LATCH = HIGH: Transfiere datos a salidas Q0-Q7
 * 
 * PARÁMETRO: data - Byte (8 bits) con estado de LEDs
 *            0 = LED encendido, 1 = LED apagado (ánodo común)
 */
void actualizarRegistro(byte data) {
  digitalWrite(LATCH_PIN, LOW);           // Paso 1: Permitir entrada de datos
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);  // Paso 2: Enviar 8 bits
  digitalWrite(LATCH_PIN, HIGH);          // Paso 3: Mostrar en salidas
  
  // Mostrar estado actual en Monitor Serial (opcional, para debugging)
  Serial.print("Estado LEDs: ");
  for (int i = 7; i >= 0; i--) {
    Serial.print(bitRead(data, i) ? "1" : "0");  // Mostrar bits 7→0
  }
  Serial.print(" (binario: ");
  Serial.print(data, BIN);
  Serial.println(")");
}

// ================= FUNCIONES DE CONTROL BÁSICO =================

/*
 * FUNCIÓN: apagarTodos()
 * PROPÓSITO: Apagar los 8 LEDs simultáneamente
 * LÓGICA ÁNODO COMÚN: 11111111 = todos bits en 1 = LEDs APAGADOS
 */
void apagarTodos() {
  ledsState = 0b11111111;      // Todos bits en 1
  actualizarRegistro(ledsState);
  Serial.println("✅ TODOS LOS LEDs APAGADOS");
}

/*
 * FUNCIÓN: encenderTodos()
 * PROPÓSITO: Encender los 8 LEDs simultáneamente  
 * LÓGICA ÁNODO COMÚN: 00000000 = todos bits en 0 = LEDs ENCENDIDOS
 */
void encenderTodos() {
  ledsState = 0b00000000;      // Todos bits en 0
  actualizarRegistro(ledsState);
  Serial.println("💡 TODOS LOS LEDs ENCENDIDOS");
}

/*
 * FUNCIÓN: encenderLED(int ledNum)
 * PROPÓSITO: Encender un LED específico (0-7)
 * LÓGICA ÁNODO COMÚN: bitClear() pone bit en 0 → LED ENCENDIDO
 * PARÁMETRO: ledNum - Número del LED (0=primero, 7=último)
 */
void encenderLED(int ledNum) {
  if (ledNum >= 0 && ledNum < NUM_LEDS) {  // Validar rango
    bitClear(ledsState, ledNum);           // Poner bit en 0 (encender)
    actualizarRegistro(ledsState);
    Serial.print("LED ");
    Serial.print(ledNum);
    Serial.println(" ENCENDIDO");
  }
}

/*
 * FUNCIÓN: apagarLED(int ledNum)
 * PROPÓSITO: Apagar un LED específico (0-7)
 * LÓGICA ÁNODO COMÚN: bitSet() pone bit en 1 → LED APAGADO
 * PARÁMETRO: ledNum - Número del LED (0=primero, 7=último)
 */
void apagarLED(int ledNum) {
  if (ledNum >= 0 && ledNum < NUM_LEDS) {  // Validar rango
    bitSet(ledsState, ledNum);             // Poner bit en 1 (apagar)
    actualizarRegistro(ledsState);
    Serial.print("LED ");
    Serial.print(ledNum);
    Serial.println(" APAGADO");
  }
}

/*
 * FUNCIÓN: toggleLED(int ledNum)
 * PROPÓSITO: Alternar estado de un LED (si está encendido→apagar, si apagado→encender)
 * LÓGICA: Usa bitRead() para leer estado actual, luego invierte
 * PARÁMETRO: ledNum - Número del LED (0-7)
 */
void toggleLED(int ledNum) {
  if (ledNum >= 0 && ledNum < NUM_LEDS) {
    if (bitRead(ledsState, ledNum) == 1) {  // Si bit es 1 (LED APAGADO)
      encenderLED(ledNum);                   // Enciéndelo (pon a 0)
    } else {                                 // Si bit es 0 (LED ENCENDIDO)
      apagarLED(ledNum);                     // Apágalo (pon a 1)
    }
  }
}

// ================= PATRONES DE ANIMACIÓN =================

/*
 * PATRÓN 1: Encendido progresivo (ola)
 * DESCRIPCIÓN: Enciende LEDs 0→7 uno por uno, luego apaga 7→0
 * EFECTO VISUAL: Ola que avanza y retrocede
 */
void patronProgresivo() {
  Serial.println("\n🔸 PATRÓN: ENCENDIDO PROGRESIVO");
  
  apagarTodos();      // Comenzar con todos apagados
  delay(500);         // Pausa inicial
  
  // FASE 1: Encender progresivamente 0→7
  Serial.println("Fase 1: Encendiendo LEDs 0→7...");
  for (int i = 0; i < NUM_LEDS; i++) {
    encenderLED(i);    // Encender LED i
    delay(DELAY_BASE); // Esperar antes de siguiente
  }
  
  // FASE 2: Apagar progresivamente 7→0
  Serial.println("Fase 2: Apagando LEDs 7→0...");
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    apagarLED(i);      // Apagar LED i
    delay(DELAY_BASE); // Esperar antes de siguiente
  }
  
  Serial.println("✅ Patrón completado");
}

/*
 * PATRÓN 2: Knight Rider (Coche Fantástico)
 * DESCRIPCIÓN: LED central que se mueve con efecto de cola
 * EFECTO VISUAL: Similar a las luces de KITT en la serie
 */
void patronKnightRider() {
  Serial.println("\n🚗 PATRÓN: KNIGHT RIDER");
  
  apagarTodos();      // Comenzar limpio
  int pos = 0;        // Posición LED principal (0-7)
  int direccion = 1;  // Dirección: 1=derecha, -1=izquierda
  
  // Repetir ciclo 20 veces
  for (int ciclo = 0; ciclo < 20; ciclo++) {
    ledsState = 0b11111111;  // Resetear a todos apagados
    
    // LED principal en posición actual
    bitClear(ledsState, pos);  // Encender LED principal (bit a 0)
    
    // Efecto de cola: LEDs adyacentes también encendidos
    if (pos > 0) bitClear(ledsState, pos - 1);  // Cola izquierda 1
    if (pos > 1) bitClear(ledsState, pos - 2);  // Cola izquierda 2
    if (pos < NUM_LEDS - 1) bitClear(ledsState, pos + 1);  // Cola derecha 1
    if (pos < NUM_LEDS - 2) bitClear(ledsState, pos + 2);  // Cola derecha 2
    
    actualizarRegistro(ledsState);  // Mostrar patrón
    
    // Mover posición
    pos += direccion;
    
    // Cambiar dirección al llegar a extremos
    if (pos >= NUM_LEDS - 1 || pos <= 0) {
      direccion = -direccion;  // Invertir dirección
    }
    
    delay(DELAY_BASE / 2);  // Velocidad rápida para efecto fluido
  }
  
  apagarTodos();  // Finalizar con todos apagados
  Serial.println("✅ Patrón completado");
}

/*
 * PATRÓN 3: LEDs alternos
 * DESCRIPCIÓN: Alterna entre LEDs pares e impares
 * EFECTO VISUAL: Parpadeo alternado
 */
void patronAlternos() {
  Serial.println("\n⚡ PATRÓN: LEDs ALTERNOS");
  
  // Alternar 10 veces entre patrón par e impar
  for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      ledsState = 0b10101010;  // LEDs pares encendidos (bits 0,2,4,6 en 0)
    } else {
      ledsState = 0b01010101;  // LEDs impares encendidos (bits 1,3,5,7 en 0)
    }
    actualizarRegistro(ledsState);
    delay(DELAY_BASE);  // Esperar entre cambios
  }
  
  apagarTodos();  // Finalizar con todos apagados
  Serial.println("✅ Patrón completado");
}

/*
 * PATRÓN 4: Contador binario
 * DESCRIPCIÓN: Cuenta de 0 a 15 mostrando representación binaria
 * EFECTO VISUAL: LEDs muestran números en binario (invertido por ánodo común)
 * EDUCACIONAL: Ayuda a entender sistema binario
 */
void patronContadorBinario() {
  Serial.println("\n🔢 PATRÓN: CONTADOR BINARIO (0-15)");
  
  for (int i = 0; i < 16; i++) {
    // Para ánodo común, invertimos los bits (~ operador)
    ledsState = ~byte(i);  // Invertir bits porque 0=encendido
    
    actualizarRegistro(ledsState);
    
    // Mostrar información educativa
    Serial.print("Decimal: ");
    Serial.print(i);
    Serial.print(" → Binario normal: ");
    for (int j = 7; j >= 0; j--) {
      Serial.print((i >> j) & 1);  // Mostrar bits 7→0
    }
    Serial.print(" → Binario invertido (ánodo común): ");
    Serial.println(ledsState, BIN);
    
    delay(DELAY_BASE * 2);  // Más lento para leer
  }
  
  apagarTodos();  // Finalizar con todos apagados
  Serial.println("✅ Patrón completado");
}

/*
 * PATRÓN 5: Todos encendidos/apagados
 * DESCRIPCIÓN: Parpadeo simultáneo de todos los LEDs
 * EFECTO VISUAL: Luz intermitente
 */
void patronTodos() {
  Serial.println("\n💡 PATRÓN: TODOS ENCENDIDOS/APAGADOS");
  
  // Parpadear 5 veces
  for (int i = 0; i < 5; i++) {
    encenderTodos();     // Todos encendidos
    delay(DELAY_BASE);   // Mantener encendidos
    apagarTodos();       // Todos apagados
    delay(DELAY_BASE);   // Mantener apagados
  }
  
  Serial.println("✅ Patrón completado");
}

/*
 * PATRÓN 6: Carrera simple
 * DESCRIPCIÓN: Un solo LED que recorre de izquierda a derecha y vuelve
 * EFECTO VISUAL: Punto de luz que se mueve
 */
void patronCarrera() {
  Serial.println("\n🏃 PATRÓN: CARRERA SIMPLE");
  
  // Repetir 3 ciclos completos (ida y vuelta)
  for (int j = 0; j < 3; j++) {
    // Ida: posición 0→7
    for (int i = 0; i < NUM_LEDS; i++) {
      apagarTodos();    // Limpiar todos
      encenderLED(i);   // Encender solo LED i
      delay(DELAY_BASE / 2);  // Velocidad rápida
    }
    
    // Vuelta: posición 7→0
    for (int i = NUM_LEDS - 1; i >= 0; i--) {
      apagarTodos();    // Limpiar todos
      encenderLED(i);   // Encender solo LED i
      delay(DELAY_BASE / 2);  // Velocidad rápida
    }
  }
  
  apagarTodos();  // Finalizar con todos apagados
  Serial.println("✅ Patrón completado");
}

// ================= FUNCIONES DE UTILIDAD =================

/*
 * FUNCIÓN: demostracionInicial()
 * PROPÓSITO: Ejecutar secuencia de prueba automática al iniciar
 * VERIFICA: Que todos los LEDs funcionan correctamente
 */
void demostracionInicial() {
  Serial.println("\n🌟 DEMOSTRACIÓN INICIAL DE PRUEBA");
  
  // 1. Mostrar todos encendidos
  Serial.println("1. Todos LEDs ENCENDIDOS...");
  encenderTodos();
  delay(1000);
  
  // 2. Mostrar todos apagados
  Serial.println("2. Todos LEDs APAGADOS...");
  apagarTodos();
  delay(1000);
  
  // 3. Prueba individual rápida de cada LED
  Serial.println("3. Prueba individual LEDs 0-7...");
  for (int i = 0; i < NUM_LEDS; i++) {
    encenderLED(i);    // Encender LED i
    delay(150);        // Breve pausa
  }
  
  delay(1000);         // Mantener último LED encendido
  
  // 4. Finalizar con todos apagados
  apagarTodos();
  Serial.println("✅ Demostración completada - SISTEMA LISTO\n");
}

/*
 * FUNCIÓN: mostrarMenu()
 * PROPÓSITO: Mostrar menú de opciones en Monitor Serial
 * INTERFAZ: Usuario selecciona opción con teclado
 */
void mostrarMenu() {
  Serial.println("\n════════════════════════════════════════");
  Serial.println("           MENÚ DE CONTROL 74HC595");
  Serial.println("════════════════════════════════════════");
  Serial.println("PATRONES DE ANIMACIÓN:");
  Serial.println("  [A] - Encendido progresivo (ola)");
  Serial.println("  [B] - Knight Rider (coche fantástico)");
  Serial.println("  [C] - LEDs alternos (pares/impares)");
  Serial.println("  [D] - Contador binario 0-15");
  Serial.println("  [E] - Todos encendidos/apagados");
  Serial.println("  [F] - Carrera simple");
  Serial.println("");
  Serial.println("CONTROL MANUAL:");
  Serial.println("  [X] - APAGAR todos los LEDs");
  Serial.println("  [Y] - ENCENDER todos los LEDs");
  Serial.println("  [0-7] - Alternar LED individual");
  Serial.println("          (0=primer LED, 7=último LED)");
  Serial.println("════════════════════════════════════════");
}

/*
 * FUNCIÓN: esperarComando()
 * PROPÓSITO: Esperar y procesar entrada del usuario
 * FUNCIONAMIENTO:
 *   1. Espera caracter por Serial
 *   2. Convierte a mayúscula
 *   3. Ejecuta acción correspondiente con switch-case
 *   4. Limpia buffer serial
 */
void esperarComando() {
  Serial.println("⌨️  Esperando comando... (ingresa letra/número)");
  
  // Esperar hasta que haya datos disponibles
  while (Serial.available() == 0) {
    delay(100);  // Pequeña pausa para no saturar CPU
  }
  
  // Leer comando ingresado
  char comando = Serial.read();
  Serial.print("📥 Comando recibido: '");
  Serial.print(comando);
  Serial.println("'");
  
  // Convertir a mayúscula para manejar minúsculas también
  comando = toupper(comando);
  
  // Declarar variable fuera del switch (requerido por C++)
  int ledNum;
  
  // Ejecutar acción según comando
  switch(comando) {
    case 'A':
      patronProgresivo();
      break;
      
    case 'B':
      patronKnightRider();
      break;
      
    case 'C':
      patronAlternos();
      break;
      
    case 'D':
      patronContadorBinario();
      break;
      
    case 'E':
      patronTodos();
      break;
      
    case 'F':
      patronCarrera();
      break;
      
    case 'X':
      apagarTodos();
      break;
      
    case 'Y':
      encenderTodos();
      break;
      
    // Comandos para LEDs individuales (caracteres '0' a '7')
    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
      ledNum = comando - '0';  // Convertir char a int: '0'→0, '1'→1, etc.
      toggleLED(ledNum);       // Alternar estado del LED
      break;
      
    default:
      Serial.println("⚠️  Comando no reconocido");
      Serial.println("Usa: A-F para patrones, X/Y para control total, 0-7 para LEDs individuales");
      break;
  }
  
  // Limpiar buffer serial por si hay caracteres extras
  while (Serial.available() > 0) {
    Serial.read();
  }
}

/*
 * ============================================================
 * INFORMACIÓN ADICIONAL
 * ============================================================
 * 
 * VERIFICACIÓN DE CONEXIONES (si algo no funciona):
 * 1. OE (pin 13 74HC595) → DEBE estar en GND
 * 2. MR (pin 10 74HC595) → DEBE estar en 5V  
 * 3. VCC (pin 16) → 5V estable
 * 4. GND (pin 8) → Tierra común
 * 
 * DIAGNÓSTICO RÁPIDO:
 * - Comando 'X' → Debe apagar todos LEDs (11111111)
 * - Comando 'Y' → Debe encender todos LEDs (00000000)
 * - Comando '0' → Debe encender primer LED (11111110)
 * 
 * MODIFICACIONES POSIBLES:
 * - Cambiar DELAY_BASE: Aumentar para más lento, disminuir para más rápido
 * - Añadir más patrones: Crear nuevas funciones y añadir al switch-case
 * - Control externo: Añadir botones físicos o sensores
 * 
 * EXPANSIÓN PARA MÁS LEDs:
 * - Conectar Q7' (pin 9) al DS del siguiente 74HC595
 * - Usar array de bytes en lugar de un solo byte
 * - Modificar actualizarRegistro() para enviar múltiples bytes
 * ============================================================
 */