// DEFINICIÓN DE PINES PARA LOS LEDs
#define RED_LED_PIN 11      // LED rojo conectado al pin 11 (PWM)
#define YELLOW_LED_PIN 10   // LED amarillo conectado al pin 10 (PWM) 
#define BLUE_LED_PIN 9      // LED azul conectado al pin 9 (PWM)
// #define: Crea constantes (no ocupan memoria de variables)
// Se usan pines PWM (9,10,11) que permiten control de brillo si fuera necesario

// DEFINICIÓN DE PINES PARA LOS BOTONES DE COLOR
#define RED_BUTTON_PIN 7    // Botón rojo en pin 7
#define YELLOW_BUTTON_PIN 6 // Botón amarillo en pin 6
#define BLUE_BUTTON_PIN 5   // Botón azul en pin 5

// DEFINICIÓN DE PINES PARA OTROS COMPONENTES
#define BUZZER_PIN 8        // Zumbador en pin 8
#define START_BUTTON_PIN 12 // Botón de inicio en pin 12

// ENUMERACIÓN PARA LOS COLORES (tipo de dato personalizado)
enum Color {RED, YELLOW, BLUE, NONE};
// Crea un nuevo tipo de dato llamado "Color" con 4 valores posibles:
// RED = 0, YELLOW = 1, BLUE = 2, NONE = 3
// Facilita la lectura del código en lugar de usar números

// VARIABLE PARA RASTREAR EL COLOR ACTUAL
Color currentColor = NONE;
// Crea una variable llamada currentColor de tipo "Color"
// Inicializada en NONE (ningún color activo)

// DECLARACIÓN DE FUNCIONES (PROTOTIPOS)
// Esto le dice al compilador que estas funciones existen
void randomColor();
void checkButtons();
void handleButtonPress(Color buttonColor);

// FUNCIÓN setup(): CONFIGURACIÓN INICIAL
void setup() {
    // CONFIGURACIÓN DE LEDs COMO SALIDAS
    pinMode(RED_LED_PIN, OUTPUT);     // Pin 11 como salida para LED rojo
    pinMode(YELLOW_LED_PIN, OUTPUT);  // Pin 10 como salida para LED amarillo
    pinMode(BLUE_LED_PIN, OUTPUT);    // Pin 9 como salida para LED azul
    
    // CONFIGURACIÓN DE BOTONES COMO ENTRADAS CON PULL-UP INTERNO
    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);     // Pin 7 como entrada con resistencia pull-up
    pinMode(YELLOW_BUTTON_PIN, INPUT_PULLUP);  // Pin 6 como entrada con resistencia pull-up
    pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);    // Pin 5 como entrada con resistencia pull-up
    // INPUT_PULLUP: Arduino activa una resistencia interna que mantiene el pin en HIGH
    // Cuando se presiona el botón, el pin pasa a LOW (0V)
    
    // CONFIGURACIÓN DEL ZUMBADOR Y BOTÓN DE INICIO
    pinMode(BUZZER_PIN, OUTPUT);        // Pin 8 como salida para el zumbador
    pinMode(START_BUTTON_PIN, INPUT_PULLUP); // Pin 12 como entrada para botón inicio
}

// FUNCIÓN loop(): BUCLE PRINCIPAL (se ejecuta infinitamente)
void loop() {
    // VERIFICAR SI SE PRESIONÓ EL BOTÓN DE INICIO
    if (digitalRead(START_BUTTON_PIN) == LOW) {
        // LOW = botón presionado (recordar que usamos INPUT_PULLUP)
        randomColor();  // Llama a la función que enciende un LED aleatorio
        delay(1000);    // Espera 1000ms (1 segundo) para que el jugador reaccione
    }
    
    checkButtons(); // Llama a la función que verifica los botones de colores
}

// FUNCIÓN PARA ENCENDER UN LED ALEATORIO
void randomColor() {
    // PRIMERO APAGA TODOS LOS LEDs
    digitalWrite(RED_LED_PIN, LOW);     // Apaga LED rojo
    digitalWrite(YELLOW_LED_PIN, LOW);  // Apaga LED amarillo
    digitalWrite(BLUE_LED_PIN, LOW);    // Apaga LED azul

    // GENERA UN COLOR ALEATORIO
    currentColor = static_cast<Color>(random(0, 3));
    // random(0, 3): Genera un número aleatorio entre 0 y 2 (3 no incluido)
    // static_cast<Color>: Convierte el número (0,1,2) a tipo Color (RED,YELLOW,BLUE)
    
    // ENCIENDE EL LED CORRESPONDIENTE AL COLOR ALEATORIO
    switch (currentColor) {
        case RED:                     // Si currentColor es RED (0)
            digitalWrite(RED_LED_PIN, HIGH);  // Enciende LED rojo
            break;                    // Sale del switch
            
        case YELLOW:                  // Si currentColor es YELLOW (1)
            digitalWrite(YELLOW_LED_PIN, HIGH); // Enciende LED amarillo
            break;
            
        case BLUE:                    // Si currentColor es BLUE (2)
            digitalWrite(BLUE_LED_PIN, HIGH);   // Enciende LED azul
            break;
            
        default:                      // Para cualquier otro caso (no debería ocurrir)
            break;                    // No hace nada
    }
}

// FUNCIÓN PARA VERIFICAR LOS BOTONES DE COLORES
void checkButtons() {
    // VERIFICA CADA BOTÓN (en orden de prioridad)
    if (digitalRead(RED_BUTTON_PIN) == LOW) {
        // Si se presiona botón rojo
        handleButtonPress(RED);  // Llama a la función manejadora con parámetro RED
        
    } else if (digitalRead(YELLOW_BUTTON_PIN) == LOW) {
        // Si se presiona botón amarillo
        handleButtonPress(YELLOW); // Llama con parámetro YELLOW
        
    } else if (digitalRead(BLUE_BUTTON_PIN) == LOW) {
        // Si se presiona botón azul
        handleButtonPress(BLUE);   // Llama con parámetro BLUE
    }
    // Nota: Solo un botón puede ser procesado a la vez por los "else if"
}

// FUNCIÓN PARA MANEJAR LA PRESIÓN DE UN BOTÓN
void handleButtonPress(Color buttonColor) {
    // Parámetro: buttonColor = color del botón presionado
    
    if (buttonColor == currentColor) {
        // ¡CORRECTO! El jugador presionó el botón del color que está encendido
        
        // APAGA TODOS LOS LEDs
        digitalWrite(RED_LED_PIN, LOW);     // Apaga LED rojo
        digitalWrite(YELLOW_LED_PIN, LOW);  // Apaga LED amarillo
        digitalWrite(BLUE_LED_PIN, LOW);    // Apaga LED azul
        
        currentColor = NONE; // Reinicia el color actual a "ninguno"
        
    } else {
        // ¡INCORRECTO! El jugador presionó el botón equivocado
        
        // ACTIVA EL ZUMBADOR DE ERROR
        tone(BUZZER_PIN, 1000); // Genera tono de 1000Hz en el pin del zumbador
        // tone(pin, frecuencia): Genera onda cuadrada a la frecuencia especificada
        // 1000Hz = tono agudo (como un pitido)
        
        delay(2000); // Mantiene el tono por 2000ms (2 segundos)
        
        noTone(BUZZER_PIN); // Detiene el tono del zumbador
        // Importante: Detener el tono para que no suene indefinidamente
    }
}