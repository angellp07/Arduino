/*
===========================================
PROYECTO PARPADEO CONTROLADO POR BOTÓN
===========================================
DESCRIPCIÓN: El botón funciona como interruptor.
             Presiona una vez: LED parpadea.
             Presiona otra vez: LED se apaga.
             Cada presión alterna el estado.
===========================================
CONEXIONES:
• Botón: D2 → Botón → GND
• LED:   D13 → Resistencia 220Ω → LED(+) → LED(-) → GND
===========================================
*/

// ====================
// DECLARACIÓN DE PINES
// ====================
const int pinBoton = 2;   // Pin del BOTÓN (cable azul en D2)
const int pinLED = 13;     // Pin del LED externo

// ====================
// VARIABLES DE CONTROL
// ====================
bool parpadeoActivo = false;  // Controla si el LED debe parpadear
                               // false = NO parpadea, true = SÍ parpadea
int estadoAnteriorBoton = HIGH; // Guarda el estado PREVIO del botón
                                 // Para detectar cuándo CAMBIA

// ====================
// CONFIGURACIÓN INICIAL
// ====================
void setup() {
  // Configurar botón con resistencia PULLUP interna
  pinMode(pinBoton, INPUT_PULLUP);
  
  // Configurar LED como salida
  pinMode(pinLED, OUTPUT);
  
  // Iniciar comunicación serial
  Serial.begin(9600);
  
  // Mensaje inicial
  Serial.println("💫 PARPADEO CONTROLADO POR BOTÓN");
  Serial.println("================================");
  Serial.println("Presiona el botón para:");
  Serial.println("• 1ª vez: LED comienza a parpadear");
  Serial.println("• 2ª vez: LED deja de parpadear");
  Serial.println("• Y así alternadamente...");
}

// ====================
// PROGRAMA PRINCIPAL
// ====================
void loop() {
  // =============================
  // LEER ESTADO ACTUAL DEL BOTÓN
  // =============================
  int estadoActualBoton = digitalRead(pinBoton);
  
  // =============================
  // DETECTAR PRESIÓN DEL BOTÓN
  // =============================
  // Detectamos CUANDO el botón CAMBIA de estado
  // Es decir, cuando pasamos de NO presionado a PRESIONADO
  // estadoAnteriorBoton == HIGH  → Botón estaba LIBRE
  // estadoActualBoton == LOW     → Botón ahora está PRESIONADO
  // ¡AMBAS condiciones deben cumplirse!
  if (estadoAnteriorBoton == HIGH && estadoActualBoton == LOW) {
    // =========================
    // EL BOTÓN ACABA DE SER PRESIONADO
    // =========================
    
    // Cambiar el estado de parpadeo
    // Si estaba activo, lo desactivamos
    // Si estaba inactivo, lo activamos
    parpadeoActivo = !parpadeoActivo;
    
    // Mostrar mensaje según nuevo estado
    if (parpadeoActivo) {
      Serial.println("▶️ Parpadeo ACTIVADO");
    } else {
      Serial.println("⏸️ Parpadeo DESACTIVADO");
      digitalWrite(pinLED, LOW);  // Aseguramos que el LED se apague
    }
    
    // Pequeña pausa anti-rebote
    delay(300);
  }
  
  // =============================
  // GUARDAR ESTADO PARA PRÓXIMA VUELTA
  // =============================
  estadoAnteriorBoton = estadoActualBoton;
  
  // =============================
  // CONTROLAR EL PARPADEO DEL LED
  // =============================
  if (parpadeoActivo) {
    // Si el parpadeo está ACTIVO, hacemos parpadear el LED
    
    // ENCENDER el LED
    digitalWrite(pinLED, HIGH);
    Serial.println("💡 LED ENCENDIDO");
    delay(500);  // Esperar medio segundo encendido
    
    // APAGAR el LED
    digitalWrite(pinLED, LOW);
    Serial.println("◻️ LED APAGADO");
    delay(500);  // Esperar medio segundo apagado
    
  } else {
    // Si el parpadeo está INACTIVO
    // Solo mantener el LED apagado
    // (ya lo hicimos arriba cuando desactivamos)
    delay(100);  // Pequeña pausa para no saturar
  }
}

/*
===========================================
EXPLICACIÓN DETALLADA DEL FUNCIONAMIENTO:

1. VARIABLE parpadeoActivo:
   - Es un "interruptor" en la memoria
   - Solo puede ser true (verdadero) o false (falso)
   - Empieza en false (LED no parpadea)

2. DETECCIÓN DE PRESIÓN:
   - No nos interesa si el botón "está presionado"
   - Nos interesa si el botón "acaba de ser presionado"
   - Por eso comparamos estado ANTERIOR vs ACTUAL

3. LÓGICA DEL PARPADEO:
   - Cuando parpadeoActivo es true:
        ENCENDER → esperar → APAGAR → esperar → repetir
   - Cuando parpadeoActivo es false:
        Solo mantener LED apagado

4. ANTI-REBOTE:
   - El delay(300) después de detectar presión
   - Evita que una sola presión se detecte múltiples veces
===========================================
*/
