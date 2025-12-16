/*
 * ============================================================
 * CONTROL DE DISPLAY 7 SEGMENTOS 4 DÍGITOS
 * ============================================================
 * 
 * DESCRIPCIÓN:
 * Este programa permite mostrar cualquier número de 4 dígitos
 * (0000 a 9999) en un display de 7 segmentos, controlando
 * el número desde el teclado del Monitor Serial.
 * 
 * FUNCIONALIDADES:
 * 1. Ingresar números dígito por dígito
 * 2. Borrar el número actual (volver a 0000)
 * 3. Ver el número actual en el Monitor Serial
 * 4. Modo edición avanzada para ajustar dígitos individuales
 * 
 * CONEXIONES DISPLAY → ARDUINO:
 *   Segmento A → Pin 9
 *   Segmento B → Pin 13
 *   Segmento C → Pin 4
 *   Segmento D → Pin 6
 *   Segmento E → Pin 7
 *   Segmento F → Pin 10
 *   Segmento G → Pin 3
 *   Dígito 1   → Pin 8   (izquierda)
 *   Dígito 2   → Pin 11
 *   Dígito 3   → Pin 12
 *   Dígito 4   → Pin 2   (derecha)
 * 
 * TIPO DE DISPLAY: ÁNODO COMÚN
 *   - Dígitos se activan con HIGH
 *   - Segmentos se encienden con LOW
 * 
 * COMANDOS DISPONIBLES (Monitor Serial 9600 baudios):
 *   0-9 : Ingresar dígito
 *   c/C : Borrar todo (mostrar 0000)
 *   s/S : Mostrar número actual en Serial
 *   e/E : Modo edición avanzada
 *   t/T : Prueba del segmento G
 * ============================================================
 */

// ==================== SECCIÓN 1: CONFIGURACIÓN DE PINES ====================
/*
 * DEFINICIÓN DE PINES PARA LOS SEGMENTOS:
 * Cada segmento (A-G) es un LED que forma parte de los números.
 * 
 * Distribución de segmentos:
 *        A
 *       ---
 *    F |   | B
 *       -G-   <-- Segmento central horizontal
 *    E |   | C
 *       ---
 *        D
 */
const int SEG_A = 9;    // Segmento A - Pin 9 del Arduino
const int SEG_B = 13;   // Segmento B - Pin 13 del Arduino  
const int SEG_C = 4;    // Segmento C - Pin 4 del Arduino
const int SEG_D = 6;    // Segmento D - Pin 6 del Arduino
const int SEG_E = 7;    // Segmento E - Pin 7 del Arduino
const int SEG_F = 10;   // Segmento F - Pin 10 del Arduino
const int SEG_G = 3;    // Segmento G (línea central) - Pin 3 del Arduino

/*
 * DEFINICIÓN DE PINES PARA LOS DÍGITOS:
 * Cada dígito controla una posición del display (de 4 dígitos).
 * Se activan uno a la vez usando multiplexación.
 */
const int DIG1 = 8;     // Dígito 1 - Más a la izquierda - Pin 8
const int DIG2 = 11;    // Dígito 2 - Pin 11
const int DIG3 = 12;    // Dígito 3 - Pin 12
const int DIG4 = 2;     // Dígito 4 - Más a la derecha - Pin 2

// ==================== SECCIÓN 2: VARIABLES DEL PROGRAMA ====================
/*
 * ARRAY 'numero': Almacena los 4 dígitos que se muestran
 * Formato: numero[0] = dígito 1 (miles)
 *          numero[1] = dígito 2 (centenas)
 *          numero[2] = dígito 3 (decenas)
 *          numero[3] = dígito 4 (unidades)
 */
int numero[4] = {0, 0, 0, 0};  // Inicia mostrando "0000"

/*
 * VARIABLES DE CONTROL:
 * - brillo: Tiempo que cada dígito permanece encendido (en microsegundos)
 * - posicionEntrada: Controla en qué posición se ingresará el próximo dígito
 */
int brillo = 4000;             // 4000 microsegundos = 4 milisegundos
int posicionEntrada = 0;       // Comienza en la posición 0 (dígito 1)

// ==================== SECCIÓN 3: CONFIGURACIÓN INICIAL (setup) ====================
void setup() {
  /*
   * FUNCIÓN setup(): Se ejecuta UNA SOLA VEZ al iniciar
   * Propósito: Configurar todo lo necesario antes de empezar
   */
  
  // Iniciar comunicación serial para recibir comandos del teclado
  Serial.begin(9600);  // 9600 baudios (velocidad de comunicación)
  
  // Configurar todos los pines como salidas
  configurarPines();
  
  // Asegurar que el display empiece apagado
  apagarTodo();
  
  // Mostrar instrucciones al usuario
  mostrarMensajeInicial();
}

void configurarPines() {
  /*
   * Configura todos los pines del Arduino como SALIDAS
   * porque enviaremos señales al display, no recibiremos de él.
   */
  
  // Configurar pines de segmentos (A-G)
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  
  // Configurar pines de dígitos (1-4)
  pinMode(DIG1, OUTPUT);
  pinMode(DIG2, OUTPUT);
  pinMode(DIG3, OUTPUT);
  pinMode(DIG4, OUTPUT);
}

void mostrarMensajeInicial() {
  /*
   * Muestra las instrucciones en el Monitor Serial
   * para que el usuario sepa cómo usar el programa.
   */
  
  Serial.println("\n========================================");
  Serial.println("   CONTROL DE DISPLAY 7 SEGMENTOS");
  Serial.println("========================================");
  Serial.println("INSTRUCCIONES:");
  Serial.println("  Ingresa dígitos 0-9 para formar un número");
  Serial.println("  Ejemplo: Para '1234' escribe: 1 2 3 4");
  Serial.println("");
  Serial.println("COMANDOS ESPECIALES:");
  Serial.println("  c/C : Borrar todo (mostrar 0000)");
  Serial.println("  s/S : Ver número actual");
  Serial.println("  e/E : Modo edición avanzada");
  Serial.println("  t/T : Probar segmento G");
  Serial.println("========================================");
  Serial.println("Número inicial: 0000");
  Serial.print("Ingresa tu primer dígito (0-9): ");
}

// ==================== SECCIÓN 4: BUCLE PRINCIPAL (loop) ====================
void loop() {
  /*
   * FUNCIÓN loop(): Se ejecuta CONTINUAMENTE después del setup
   * Contiene las dos tareas principales:
   * 1. Mostrar el número actual en el display
   * 2. Verificar si el usuario envió algún comando
   */
  
  // TAREA 1: Mostrar continuamente el número en el display
  mostrarNumeroEnDisplay();
  
  // TAREA 2: Verificar si hay comandos del teclado
  if (Serial.available() > 0) {
    procesarEntrada();
  }
}

// ==================== SECCIÓN 5: PROCESAMIENTO DE COMANDOS ====================
void procesarEntrada() {
  /*
   * Lee un carácter del Monitor Serial y decide qué hacer con él.
   * Caracteres posibles:
   * - '0' a '9': Dígitos para mostrar
   * - 'c' o 'C': Borrar todo
   * - 's' o 'S': Mostrar número actual
   * - 'e' o 'E': Modo edición avanzada
   * - 't' o 'T': Probar segmento G
   */
  
  char tecla = Serial.read();  // Leer un carácter
  
  // Verificar qué tecla se presionó
  if (tecla >= '0' && tecla <= '9') {
    // Es un número: Convertir carácter a número y guardarlo
    int digito = tecla - '0';  // '0' → 0, '1' → 1, etc.
    ingresarDigito(digito);
  }
  else if (tecla == 'c' || tecla == 'C') {
    // Comando para borrar
    borrarTodo();
  }
  else if (tecla == 's' || tecla == 'S') {
    // Comando para mostrar el número actual
    mostrarNumeroActual();
  }
  else if (tecla == 'e' || tecla == 'E') {
    // Comando para modo edición avanzada
    modoEdicion();
  }
  else if (tecla == 't' || tecla == 'T') {
    // Comando para probar segmento G
    pruebaSegmentoG();
  }
  else {
    // Tecla no reconocida
    Serial.println("\n⚠️  Tecla no reconocida");
    Serial.println("Usa: 0-9 para dígitos, c para borrar, s para ver, e para editar");
  }
}

// ==================== SECCIÓN 6: FUNCIONES PARA MANEJAR DÍGITOS ====================
void ingresarDigito(int digito) {
  /*
   * Guarda un dígito en la posición actual y avanza a la siguiente.
   * Ejemplo: Si numero = [0,0,0,0] y se ingresa '5'
   * Resultado: numero = [5,0,0,0] y posición avanza a 1
   */
  
  if (posicionEntrada < 4) {
    // Guardar dígito en la posición actual
    numero[posicionEntrada] = digito;
    
    // Avanzar a la siguiente posición para el próximo dígito
    posicionEntrada++;
    
    // Informar al usuario
    Serial.print("\n✅ Dígito guardado: ");
    Serial.print(digito);
    Serial.print(" en posición ");
    Serial.print(posicionEntrada);
    Serial.println("/4");
    
    // Mostrar cómo queda el número completo
    mostrarNumeroActual();
    
    // Si ya ingresamos 4 dígitos, volver al inicio
    if (posicionEntrada >= 4) {
      Serial.println("\n💡 ¡Número completo! (4 dígitos)");
      Serial.println("Presiona 'c' para borrar y empezar otro");
      posicionEntrada = 0;  // Reiniciar para el próximo número
    }
  }
}

void borrarTodo() {
  /*
   * Reinicia el número a "0000" y la posición de entrada a 0.
   * Útil cuando quieres empezar un nuevo número.
   */
  
  for (int i = 0; i < 4; i++) {
    numero[i] = 0;  // Poner todos los dígitos en 0
  }
  posicionEntrada = 0;  // Volver a la primera posición
  
  Serial.println("\n🔄 Número borrado. Mostrando: 0000");
  mostrarNumeroActual();
}

void mostrarNumeroActual() {
  /*
   * Muestra en el Monitor Serial el número actual almacenado.
   * Ejemplo: Si numero = [1,2,3,4] muestra "1234"
   */
  
  Serial.print("\n📟 Número en display: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(numero[i]);  // Mostrar cada dígito
  }
  Serial.println();
}

// ==================== SECCIÓN 7: MODO EDICIÓN AVANZADA ====================
void modoEdicion() {
  /*
   * Modo especial que permite:
   * - Moverse entre dígitos individualmente
   * - Aumentar/disminuir el valor de cada dígito
   * - Ver cambios en tiempo real
   */
  
  Serial.println("\n🎮 MODO EDICIÓN AVANZADA ACTIVADO");
  Serial.println("Ahora puedes editar cada dígito individualmente");
  Serial.println("Comandos disponibles:");
  Serial.println("  a : Mover al dígito de la izquierda");
  Serial.println("  d : Mover al dígito de la derecha");
  Serial.println("  w : Aumentar dígito actual (+1)");
  Serial.println("  x : Disminuir dígito actual (-1)");
  Serial.println("  q : Salir del modo edición");
  Serial.println("----------------------");
  
  int cursor = 0;        // Posición actual del cursor (0-3)
  bool enEdicion = true; // Controla si seguimos en modo edición
  
  while (enEdicion) {
    // Mostrar el número con el cursor marcando la posición actual
    Serial.print("\nEdición → [");
    for (int i = 0; i < 4; i++) {
      if (i == cursor) {
        Serial.print(">");  // Marcar posición del cursor
      }
      Serial.print(numero[i]);  // Mostrar dígito
      if (i == cursor) {
        Serial.print("<");  // Marcar posición del cursor
      }
      if (i < 3) Serial.print(" ");
    }
    Serial.println("]");
    
    // Esperar un comando del usuario
    while (!Serial.available()) {
      // Mientras esperamos, seguimos mostrando el número en el display
      mostrarNumeroEnDisplay();
    }
    
    // Leer el comando ingresado
    char comando = Serial.read();
    
    // Ejecutar acción según el comando
    switch(comando) {
      case 'a':  // Mover cursor a la izquierda
        cursor = (cursor - 1 + 4) % 4;  // Circular: si era 0 va a 3
        Serial.print("← Cursor movido a dígito ");
        Serial.println(cursor + 1);
        break;
        
      case 'd':  // Mover cursor a la derecha
        cursor = (cursor + 1) % 4;  // Circular: si era 3 va a 0
        Serial.print("→ Cursor movido a dígito ");
        Serial.println(cursor + 1);
        break;
        
      case 'w':  // Aumentar dígito actual
        numero[cursor] = (numero[cursor] + 1) % 10;  // 9+1=0
        Serial.print("↑ Dígito ");
        Serial.print(cursor + 1);
        Serial.print(" aumentado a: ");
        Serial.println(numero[cursor]);
        break;
        
      case 'x':  // Disminuir dígito actual
        numero[cursor] = (numero[cursor] - 1 + 10) % 10;  // 0-1=9
        Serial.print("↓ Dígito ");
        Serial.print(cursor + 1);
        Serial.print(" disminuido a: ");
        Serial.println(numero[cursor]);
        break;
        
      case 'q':  // Salir del modo edición
        enEdicion = false;
        Serial.println("👋 Saliendo del modo edición");
        break;
        
      default:
        Serial.println("❌ Comando no reconocido. Usa: a, d, w, x, q");
        break;
    }
  }
}

// ==================== SECCIÓN 8: PRUEBA DEL SEGMENTO G ====================
void pruebaSegmentoG() {
  /*
   * Prueba especial para verificar que el segmento G funciona
   * correctamente en todos los números donde debe aparecer.
   */
  
  Serial.println("\n🔧 INICIANDO PRUEBA DEL SEGMENTO G");
  Serial.println("El segmento G debe aparecer en números: 2, 3, 4, 5, 6, 8, 9");
  Serial.println("Mostrando cada número por 2 segundos...");
  
  // Probar cada número del 0 al 9
  for (int n = 0; n <= 9; n++) {
    Serial.print("\nProbando número: ");
    Serial.println(n);
    
    // Poner el mismo número en los 4 dígitos
    for (int d = 0; d < 4; d++) {
      numero[d] = n;
    }
    
    // Mostrar este número por 2 segundos
    unsigned long tiempoInicio = millis();
    while (millis() - tiempoInicio < 2000) {
      mostrarNumeroEnDisplay();
    }
    
    // Indicar si el segmento G debería estar encendido
    if (deberiaEncenderG(n)) {
      Serial.println("✓ Segmento G DEBERÍA estar encendido");
    } else {
      Serial.println("✗ Segmento G DEBERÍA estar apagado");
    }
    
    delay(500);  // Pequeña pausa entre números
  }
  
  Serial.println("\n✅ Prueba completada");
  Serial.println("Verifica que el segmento G se encienda cuando corresponde");
}

bool deberiaEncenderG(int num) {
  /*
   * Determina si para un número dado el segmento G debe estar encendido.
   * Retorna true si debe encenderse, false si debe apagarse.
   */
  
  switch(num) {
    case 2: case 3: case 4: case 5: case 6: case 8: case 9:
      return true;  // Estos números usan el segmento G
    default:
      return false; // 0, 1 y 7 no usan el segmento G
  }
}

// ==================== SECCIÓN 9: CONTROL DEL DISPLAY (MULTIPLEXACIÓN) ====================
void mostrarNumeroEnDisplay() {
  /*
   * Muestra el número actual en el display usando MULTIPLEXACIÓN.
   * La multiplexación consiste en:
   * 1. Encender solo UN dígito a la vez
   * 2. Mostrar en ese dígito el número correspondiente
   * 3. Cambiar rápidamente al siguiente dígito
   * 
   * El ojo humano no nota el cambio rápido y ve los 4 dígitos encendidos.
   */
  
  // Mostrar dígito 1 (posición 0 en el array)
  mostrarDigito(1, numero[0]);
  delayMicroseconds(brillo);  // Mantener encendido
  
  // Mostrar dígito 2 (posición 1 en el array)
  mostrarDigito(2, numero[1]);
  delayMicroseconds(brillo);
  
  // Mostrar dígito 3 (posición 2 en el array)
  mostrarDigito(3, numero[2]);
  delayMicroseconds(brillo);
  
  // Mostrar dígito 4 (posición 3 en el array)
  mostrarDigito(4, numero[3]);
  delayMicroseconds(brillo);
}

void mostrarDigito(int cualDigito, int valor) {
  /*
   * Muestra un número específico en un dígito específico.
   * Proceso:
   * 1. Apagar todos los segmentos y dígitos
   * 2. Activar solo el dígito deseado
   * 3. Encender los segmentos para formar el número
   */
  
  // Paso 1: Apagar todo (evita "fantasmas" en el display)
  apagarSegmentos();
  apagarDigitos();
  
  // Paso 2: Activar solo el dígito que queremos mostrar
  // Para display ÁNODO COMÚN: HIGH activa el dígito
  switch(cualDigito) {
    case 1: digitalWrite(DIG1, HIGH); break;
    case 2: digitalWrite(DIG2, HIGH); break;
    case 3: digitalWrite(DIG3, HIGH); break;
    case 4: digitalWrite(DIG4, HIGH); break;
  }
  
  // Paso 3: Encender los segmentos para formar el número
  encenderSegmentos(valor);
}

void encenderSegmentos(int num) {
  /*
   * Enciende los segmentos específicos para formar un número del 0 al 9.
   * Para display ÁNODO COMÚN:
   *   LOW  = Segmento ENCENDIDO
   *   HIGH = Segmento APAGADO
   */
  
  switch(num) {
    case 0:  // Número 0: Segmentos A,B,C,D,E,F (todos menos G)
      digitalWrite(SEG_A, LOW);   // A encendido
      digitalWrite(SEG_B, LOW);   // B encendido
      digitalWrite(SEG_C, LOW);   // C encendido
      digitalWrite(SEG_D, LOW);   // D encendido
      digitalWrite(SEG_E, LOW);   // E encendido
      digitalWrite(SEG_F, LOW);   // F encendido
      digitalWrite(SEG_G, HIGH);  // G apagado
      break;
      
    case 1:  // Número 1: Segmentos B,C
      digitalWrite(SEG_A, HIGH);  // A apagado
      digitalWrite(SEG_B, LOW);   // B encendido
      digitalWrite(SEG_C, LOW);   // C encendido
      digitalWrite(SEG_D, HIGH);  // D apagado
      digitalWrite(SEG_E, HIGH);  // E apagado
      digitalWrite(SEG_F, HIGH);  // F apagado
      digitalWrite(SEG_G, HIGH);  // G apagado
      break;
      
    case 2:  // Número 2: Segmentos A,B,G,E,D
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, HIGH);  // C apagado
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, LOW);   // E
      digitalWrite(SEG_F, HIGH);  // F apagado
      digitalWrite(SEG_G, LOW);   // G ENCENDIDO
      break;
      
    case 3:  // Número 3: Segmentos A,B,C,D,G
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, HIGH);  // E apagado
      digitalWrite(SEG_F, HIGH);  // F apagado
      digitalWrite(SEG_G, LOW);   // G ENCENDIDO
      break;
      
    case 4:  // Número 4: Segmentos F,G,B,C
      digitalWrite(SEG_A, HIGH);  // A apagado
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, HIGH);  // D apagado
      digitalWrite(SEG_E, HIGH);  // E apagado
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 5:  // Número 5: Segmentos A,F,G,C,D
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, HIGH);  // B apagado
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, HIGH);  // E apagado
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 6:  // Número 6: Segmentos A,F,E,D,C,G
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, HIGH);  // B apagado
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, LOW);   // E
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 7:  // Número 7: Segmentos A,B,C
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, HIGH);  // D apagado
      digitalWrite(SEG_E, HIGH);  // E apagado
      digitalWrite(SEG_F, HIGH);  // F apagado
      digitalWrite(SEG_G, HIGH);  // G apagado
      break;
      
    case 8:  // Número 8: TODOS los segmentos
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, LOW);   // E
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 9:  // Número 9: Segmentos A,B,C,D,F,G
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, HIGH);  // E apagado
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    default:  // Si no es 0-9, mostrar guión (-) en segmento G
      digitalWrite(SEG_A, HIGH);  // A apagado
      digitalWrite(SEG_B, HIGH);  // B apagado
      digitalWrite(SEG_C, HIGH);  // C apagado
      digitalWrite(SEG_D, HIGH);  // D apagado
      digitalWrite(SEG_E, HIGH);  // E apagado
      digitalWrite(SEG_F, HIGH);  // F apagado
      digitalWrite(SEG_G, LOW);   // Solo G encendido
  }
}

// ==================== SECCIÓN 10: FUNCIONES AUXILIARES ====================
void apagarSegmentos() {
  /*
   * Apaga TODOS los segmentos del display.
   * Para ánodo común: HIGH = segmento apagado
   */
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void apagarDigitos() {
  /*
   * Desactiva TODOS los dígitos del display.
   * Para ánodo común: LOW = dígito desactivado
   */
  digitalWrite(DIG1, LOW);
  digitalWrite(DIG2, LOW);
  digitalWrite(DIG3, LOW);
  digitalWrite(DIG4, LOW);
}

void apagarTodo() {
  /*
   * Apaga completamente el display.
   * Útil para inicialización y evitar consumir energía innecesaria.
   */
  apagarSegmentos();
  apagarDigitos();
}

/*
 * ============================================================
 * RESUMEN DE USO PASO A PASO:
 * ============================================================
 * 
 * 1. CONECTA EL DISPLAY al Arduino según los pines indicados.
 * 
 * 2. SUBE ESTE CÓDIGO al Arduino.
 * 
 * 3. ABRE EL MONITOR SERIAL (Herramientas → Monitor Serial).
 *    Asegúrate de configurar 9600 baudios.
 * 
 * 4. PARA MOSTRAR UN NÚMERO, por ejemplo "4567":
 *    - Escribe: 4 (presiona Enter)
 *    - Escribe: 5 (presiona Enter)
 *    - Escribe: 6 (presiona Enter)
 *    - Escribe: 7 (presiona Enter)
 *    - El display mostrará "4567"
 * 
 * 5. COMANDOS ESPECIALES:
 *    - 'c' : Borra todo (vuelve a "0000")
 *    - 's' : Muestra el número actual en el Monitor Serial
 *    - 'e' : Entra al modo edición avanzada
 *    - 't' : Ejecuta prueba del segmento G
 * 
 * 6. EN MODO EDICIÓN ('e'):
 *    - Usa 'a' y 'd' para moverte entre dígitos
 *    - Usa 'w' y 'x' para aumentar/disminuir el dígito actual
 *    - Usa 'q' para salir del modo edición
 * 
 * 7. AJUSTES:
 *    - Para cambiar el brillo, modifica la variable 'brillo'
 *    - Valores típicos: 2000 (tenue) a 10000 (brillante)
 * ============================================================
 */