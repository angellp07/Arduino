// ============================================
// DEFINICIÓN DE PINES - ASIGNACIÓN DE COMPONENTES
// ============================================

// PINES PARA LOS LEDs (SALIDAS)
#define RED_LED_PIN 11      // Pin del LED rojo - encendido cuando toca presionar botón rojo
#define YELLOW_LED_PIN 10   // Pin del LED amarillo - encendido cuando toca presionar botón amarillo
#define BLUE_LED_PIN 9      // Pin del LED azul - encendido cuando toca presionar botón azul
// Nota: Los pines 9, 10, 11 son PWM, aunque aquí solo usamos encendido/apagado

// PINES PARA LOS BOTONES DE COLOR (ENTRADAS)
#define RED_BUTTON_PIN 7    // Pin del botón rojo - jugador lo presiona cuando el LED rojo está encendido
#define YELLOW_BUTTON_PIN 6 // Pin del botón amarillo - jugador lo presiona cuando el LED amarillo está encendido
#define BLUE_BUTTON_PIN 5   // Pin del botón azul - jugador lo presiona cuando el LED azul está encendido
// Nota: Usaremos INPUT_PULLUP, así que LOW significa "presionado"

// PINES PARA OTROS COMPONENTES
#define BUZZER_PIN 8        // Pin del zumbador - emite sonido cuando el jugador comete error
#define START_BUTTON_PIN 12 // Pin del botón de inicio - comienza una nueva ronda del juego

// ============================================
// DEFINICIÓN ADELANTADA DEL TIPO Color
// ============================================

// ENUMERACIÓN DE COLORES DISPONIBLES
enum Color {RED, YELLOW, BLUE, NONE};
// Este enum crea un nuevo tipo de dato llamado "Color" con 4 valores posibles:
// RED = 0, YELLOW = 1, BLUE = 2, NONE = 3
// Se usa para hacer el código más legible que usar números (0,1,2,3)

// ============================================
// PROTOTIPOS DE FUNCIONES
// ============================================

// FUNCIÓN: startGame()
// Propósito: Inicia una nueva ronda del juego
// Acciones: Activa el juego y enciende un LED aleatorio
void startGame();

// FUNCIÓN: randomColor()
// Propósito: Enciende un LED de color aleatorio
// Acciones: Apaga todos los LEDs, elige un color al azar, enciende el LED correspondiente
void randomColor();

// FUNCIÓN: checkButtons()
// Propósito: Verifica constantemente si se presiona algún botón de color
// Acciones: Revisa los 3 botones con control de rebote (debounce)
void checkButtons();

// FUNCIÓN: handleButtonPress()
// Propósito: Procesa lo que pasa cuando el jugador presiona un botón
// Parámetro: buttonColor - el color del botón que fue presionado
// Acciones: Compara si el botón presionado coincide con el LED encendido
//           Si es correcto: apaga LEDs, felicita al jugador
//           Si es incorrecto: suena el buzzer, apaga LEDs, informa error
void handleButtonPress(Color buttonColor);

// ============================================
// VARIABLES GLOBALES
// ============================================

// VARIABLE: currentColor
// Tipo: Color (definido arriba)
// Valor inicial: NONE (ningún color)
// Propósito: Guarda qué color está activo actualmente (qué LED está encendido)
// Uso: Cuando el jugador presiona un botón, comparamos con esta variable
Color currentColor = NONE;

// VARIABLE: gameActive
// Tipo: bool (booleano - verdadero o falso)
// Valor inicial: false (juego inactivo)
// Propósito: Controla si el juego está en progreso o no
// Uso: Solo verificamos botones de color cuando gameActive es true
bool gameActive = false;

// ============================================
// CONFIGURACIÓN INICIAL - setup()
// ============================================

// FUNCIÓN: setup()
// Propósito: Se ejecuta UNA SOLA VEZ al encender o resetear el Arduino
// Tiempo de ejecución: Al inicio del programa
void setup() {
    // ===== CONFIGURAR PINES DE LEDs COMO SALIDAS =====
    // OUTPUT significa que el pin enviará voltaje (5V o 0V)
    pinMode(RED_LED_PIN, OUTPUT);     // Pin 11 controlará el LED rojo
    pinMode(YELLOW_LED_PIN, OUTPUT);  // Pin 10 controlará el LED amarillo
    pinMode(BLUE_LED_PIN, OUTPUT);    // Pin 9 controlará el LED azul
    
    // ===== CONFIGURAR PINES DE BOTONES COMO ENTRADAS =====
    // INPUT_PULLUP significa:
    // 1. El pin es entrada (lee voltaje)
    // 2. Arduino activa una resistencia interna que mantiene el pin en HIGH (5V)
    // 3. Cuando el botón se presiona, conecta el pin a GND (0V), cambiando a LOW
    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);     // Botón rojo en pin 7
    pinMode(YELLOW_BUTTON_PIN, INPUT_PULLUP);  // Botón amarillo en pin 6
    pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);    // Botón azul en pin 5
    
    // ===== CONFIGURAR OTROS COMPONENTES =====
    pinMode(BUZZER_PIN, OUTPUT);        // Zumbador en pin 8 (salida de sonido)
    pinMode(START_BUTTON_PIN, INPUT_PULLUP); // Botón de inicio en pin 12
    
    // ===== INICIAR COMUNICACIÓN SERIAL =====
    // Propósito: Permitir comunicación con la computadora para depuración
    // 9600 es la velocidad (baud rate) - bits por segundo
    Serial.begin(9600);
    
    // Mensaje inicial para confirmar que el Arduino está listo
    Serial.println("Juego de Reaccion - Listo");
    // Este mensaje aparece en "Herramientas" → "Monitor Serial" del IDE Arduino
}

// ============================================
// BUCLE PRINCIPAL - loop()
// ============================================

// FUNCIÓN: loop()
// Propósito: Se ejecuta INFINITAMENTE después de setup()
// Tiempo de ejecución: Continuamente, una y otra vez
void loop() {
    // ===== VERIFICAR BOTÓN DE INICIO =====
    // INPUT_PULLUP hace que normalmente sea HIGH, LOW cuando se presiona
    if (digitalRead(START_BUTTON_PIN) == LOW) {
        // DEBOUNCE: Espera para eliminar falsos contactos por vibración del botón
        delay(50);  // 50 milisegundos de espera
        
        // Verificación doble para confirmar que realmente está presionado
        if (digitalRead(START_BUTTON_PIN) == LOW) {
            // Iniciar nueva ronda del juego
            startGame();
            
            // Esperar a que el jugador suelte el botón
            // Esto evita múltiples inicios por una sola pulsación
            while(digitalRead(START_BUTTON_PIN) == LOW) {
                delay(10);  // Pequeña pausa para no sobrecargar el procesador
            }
        }
    }
    
    // ===== VERIFICAR BOTONES DE COLOR (SOLO SI EL JUEGO ESTÁ ACTIVO) =====
    // gameActive es true solo después de presionar START
    if (gameActive) {
        checkButtons();  // Revisa si el jugador presionó rojo, amarillo o azul
    }
    
    // El bloop vuelve al inicio y repite todo continuamente
    // Normalmente hay 16 millones de ciclos por segundo, pero los delays lo hacen más lento
}

// ============================================
// FUNCIÓN: startGame()
// ============================================

// Propósito: Preparar todo para una nueva ronda del juego
void startGame() {
    // Mensaje para el Monitor Serial
    Serial.println("\n🎮 INICIANDO JUEGO...");
    
    // Activar el estado del juego
    // Ahora checkButtons() responderá a las pulsaciones
    gameActive = true;
    
    // Encender un LED aleatorio (rojo, amarillo o azul)
    randomColor();
    
    // Después de esto, el jugador tiene que presionar el botón del color correcto
}

// ============================================
// FUNCIÓN: randomColor()
// ============================================

// Propósito: Elegir y encender un color aleatorio
void randomColor() {
    // ===== PASO 1: APAGAR TODOS LOS LEDs =====
    // LOW = 0V = LED apagado
    // Aseguramos que solo un LED esté encendido a la vez
    digitalWrite(RED_LED_PIN, LOW);     // Apagar LED rojo
    digitalWrite(YELLOW_LED_PIN, LOW);  // Apagar LED amarillo
    digitalWrite(BLUE_LED_PIN, LOW);    // Apagar LED azul

    // ===== PASO 2: GENERAR UN COLOR ALEATORIO =====
    // random(0, 3) genera: 0, 1 o 2 (el 3 no se incluye)
    // (Color) convierte el número (0,1,2) al tipo Color (RED,YELLOW,BLUE)
    currentColor = (Color)random(0, 3);
    
    // ===== PASO 3: ENCENDER EL LED CORRESPONDIENTE =====
    // HIGH = 5V = LED encendido
    // El switch elige qué LED encender según el valor de currentColor
    switch (currentColor) {
        case RED:  // Si currentColor es RED (0)
            digitalWrite(RED_LED_PIN, HIGH);  // Encender pin 11 (LED rojo)
            Serial.println("🔴 LED ROJO encendido");  // Mensaje en computadora
            break;  // Salir del switch
            
        case YELLOW:  // Si currentColor es YELLOW (1)
            digitalWrite(YELLOW_LED_PIN, HIGH);  // Encender pin 10 (LED amarillo)
            Serial.println("🟡 LED AMARILLO encendido");
            break;
            
        case BLUE:  // Si currentColor es BLUE (2)
            digitalWrite(BLUE_LED_PIN, HIGH);  // Encender pin 9 (LED azul)
            Serial.println("🔵 LED AZUL encendido");
            break;
            
        default:  // Esto no debería ocurrir, pero es buena práctica incluirlo
            break;  // No hacer nada
    }
    // Ahora el jugador sabe qué botón debe presionar
}

// ============================================
// FUNCIÓN: checkButtons()
// ============================================

// Propósito: Revisar constantemente si el jugador presiona algún botón
void checkButtons() {
    // ===== VARIABLES PARA CONTROL DE REBOTE (DEBOUNCE) =====
    // lastPress: guarda la última vez que se procesó una pulsación
    // static: Mantiene su valor entre llamadas a la función (no se reinicia)
    static unsigned long lastPress = 0;
    
    // debounceTime: tiempo mínimo entre pulsaciones (200ms)
    // Evita que una sola pulsación se cuente múltiples veces
    const unsigned long debounceTime = 200;
    
    // ===== VERIFICAR SI HA PASADO SUFICIENTE TIEMPO =====
    // millis(): tiempo en milisegundos desde que empezó el programa
    // Solo procesamos una nueva pulsación si han pasado más de 200ms
    if (millis() - lastPress > debounceTime) {
        
        // ===== VERIFICAR BOTÓN ROJO =====
        if (digitalRead(RED_BUTTON_PIN) == LOW) {
            // Registrar el momento de esta pulsación
            lastPress = millis();
            // Llamar a la función que procesa la pulsación
            // RED es el parámetro que indica qué botón se presionó
            handleButtonPress(RED);
        } 
        // ===== VERIFICAR BOTÓN AMARILLO =====
        // else if: Solo se verifica si el rojo NO fue presionado
        else if (digitalRead(YELLOW_BUTTON_PIN) == LOW) {
            lastPress = millis();
            handleButtonPress(YELLOW);
        } 
        // ===== VERIFICAR BOTÓN AZUL =====
        // else if: Solo se verifica si rojo y amarillo NO fueron presionados
        else if (digitalRead(BLUE_BUTTON_PIN) == LOW) {
            lastPress = millis();
            handleButtonPress(BLUE);
        }
        // Si ningún botón está presionado, no hace nada
    }
    // Si no ha pasado suficiente tiempo desde la última pulsación, no hace nada
}

// ============================================
// FUNCIÓN: handleButtonPress()
// ============================================

// Propósito: Determinar si la pulsación fue correcta o incorrecta
// Parámetro: buttonColor - el color del botón que el jugador presionó
void handleButtonPress(Color buttonColor) {
    // ===== MOSTRAR INFORMACIÓN EN MONITOR SERIAL =====
    // Esto ayuda a depurar y entender qué está pasando
    Serial.print("Botón presionado: ");
    
    // Identificar qué botón se presionó (para mostrar en pantalla)
    switch(buttonColor) {
        case RED: 
            Serial.print("ROJO"); 
            break;
        case YELLOW: 
            Serial.print("AMARILLO"); 
            break;
        case BLUE: 
            Serial.print("AZUL"); 
            break;
        default: 
            Serial.print("DESCONOCIDO"); 
            break;
    }
    
    // Mostrar qué LED estaba encendido
    Serial.print(" - LED actual: ");
    switch(currentColor) {
        case RED: 
            Serial.print("ROJO"); 
            break;
        case YELLOW: 
            Serial.print("AMARILLO"); 
            break;
        case BLUE: 
            Serial.print("AZUL"); 
            break;
        case NONE: 
            Serial.print("NINGUNO"); 
            break;
    }
    
    // ===== VERIFICAR SI LA RESPUESTA ES CORRECTA =====
    // Comparar: ¿El botón presionado coincide con el LED encendido?
    if (buttonColor == currentColor) {
        // ========== RESPUESTA CORRECTA ==========
        Serial.println(" ✅ CORRECTO!");
        
        // ----- APAGAR TODOS LOS LEDs -----
        // El juego terminó, apagamos las luces
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(BLUE_LED_PIN, LOW);
        
        // ----- REINICIAR VARIABLES DEL JUEGO -----
        currentColor = NONE;   // Ya no hay color activo
        gameActive = false;    // El juego terminó (ganó)
        // Nota: Ahora checkButtons() no responderá hasta que se presione START
        
        // ----- MENSAJE DE FELICITACIÓN -----
        Serial.println("🎉 ¡GANASTE! Presiona START para jugar otra vez");
        
    } else {
        // ========== RESPUESTA INCORRECTA ==========
        Serial.println(" ❌ ERROR!");
        
        // ----- ACTIVAR ZUMBADOR DE ERROR -----
        // tone(pin, frecuencia): Genera una onda cuadrada en el pin especificado
        // 1000Hz = tono agudo (como un pitido de error)
        tone(BUZZER_PIN, 1000);
        
        // Mantener el tono por 2 segundos
        delay(2000);
        
        // Detener el tono (importante, sino sigue sonando)
        noTone(BUZZER_PIN);
        
        // ----- APAGAR TODOS LOS LEDs -----
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(BLUE_LED_PIN, LOW);
        
        // ----- REINICIAR VARIABLES DEL JUEGO -----
        currentColor = NONE;   // Ya no hay color activo
        gameActive = false;    // El juego terminó (perdió)
        
        // ----- MENSAJE DE INTENTAR NUEVAMENTE -----
        Serial.println("😞 Perdiste. Presiona START para intentar otra vez");
    }
    // La función termina aquí y vuelve a checkButtons(), luego a loop()
}

// ============================================
// RESUMEN DEL FLUJO DEL PROGRAMA:
// ============================================
// 1. Arduino se enciende → ejecuta setup() una vez
// 2. Entra en loop() que se repite infinitamente:
//    a. Verifica si se presiona START
//    b. Si se presiona START: llama a startGame()
//       - startGame() activa el juego y llama a randomColor()
//       - randomColor() enciende un LED aleatorio
//    c. Si gameActive es true: llama a checkButtons()
//       - checkButtons() revisa los 3 botones
//       - Si detecta una pulsación: llama a handleButtonPress()
//    d. handleButtonPress() decide si fue correcto o incorrecto
//       - Correcto: apaga LEDs, mensaje de ganador
//       - Incorrecto: suena buzzer, apaga LEDs, mensaje de perder
// 3. Vuelve al inicio de loop() y repite

// ============================================
// CONSEJOS PARA MODIFICAR EL JUEGO:
// ============================================
// 1. Para cambiar los tiempos:
//    - Debounce: cambiar debounceTime en checkButtons() (línea ~160)
//    - Tono error: cambiar delay(2000) en handleButtonPress() (línea ~258)
//    - Espera START: cambiar delay(50) en loop() (línea ~103)
//
// 2. Para cambiar los pines:
//    - Modificar los números en los #define al inicio
//    - Asegurarse de usar pines digitales (2-13) excepto 0 y 1
//
// 3. Para agregar más colores:
//    - Agregar al enum Color: ej: GREEN,
//    - Agregar #define para nuevo LED y botón
//    - Agregar en setup(), randomColor(), checkButtons()
//    - Actualizar handleButtonPress()
//
// 4. Para hacer el juego más difícil:
//    - Agregar temporizador que apague el LED después de X segundos
//    - Hacer que los LEDs parpadeen en lugar de estar fijos
//    - Agregar secuencias de múltiples colores