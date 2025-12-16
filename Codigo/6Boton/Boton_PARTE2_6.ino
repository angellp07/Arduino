/*
===========================================
PROYECTO BÁSICO: BOTÓN + LED
===========================================
DESCRIPCIÓN: El LED se enciende solo cuando
             mantienes presionado el botón.
             Al soltar, el LED se apaga.
===========================================
CONEXIONES:
• Botón: D2 → Botón → GND
• LED:   D13 → Resistencia 220Ω → LED(+) → LED(-) → GND
===========================================
*/

// ====================
// DECLARACIÓN DE PINES
// ====================
const int pinBoton = 2;   // Pin donde está conectado el BOTÓN (cable azul)
const int pinLED = 13;     // Pin donde está conectado el LED externo

// ====================
// CONFIGURACIÓN INICIAL
// ====================
void setup() {
  // Configurar el pin del BOTÓN como ENTRADA con resistencia PULLUP interna
  pinMode(pinBoton, INPUT_PULLUP);
  
  // Configurar los pines de los LEDs como SALIDAS
  pinMode(pinLED, OUTPUT);
  
  // Iniciar comunicación serial para ver mensajes en la computadora
  Serial.begin(9600);
  
  // Mensaje inicial
  Serial.println("🟢 PROYECTO BÁSICO BOTÓN + LED");
  Serial.println("==============================");
  Serial.println("Presiona el botón para encender el LED");
  Serial.println("Suelta para apagarlos");
}

// ====================
// PROGRAMA PRINCIPAL
// ====================
void loop() {
  // LEER ESTADO DEL BOTÓN
  int estadoBoton = digitalRead(pinBoton);
  
  // CONTROLAR EL LED
  if (estadoBoton == LOW) {  // Si el botón está PRESIONADO
    // ENCENDER LED
    digitalWrite(pinLED, HIGH);
    
    // Mostrar mensaje en Monitor Serial
    Serial.println("✅ Botón PRESIONADO - LED ENCENDIDO");
    
    // Pequeña pausa para evitar "rebote" del botón
    delay(250);
    
  } else {  // Si el botón está LIBRE
    // APAGAR LED
    digitalWrite(pinLED, LOW);
    
    // Mostrar mensaje en Monitor Serial
    Serial.println("🔘 Botón LIBRE - LED APAGADO");
  }
  
  // PAUSA PARA ESTABILIDAD
  delay(100);
}

/*
===========================================
RESUMEN DE FUNCIONAMIENTO:
1. Arduino lee constantemente el pin D2
2. Si detecta LOW (botón presionado):
   - Enciende el LED
   - Muestra mensaje "PRESIONADO"
3. Si detecta HIGH (botón libre):
   - Apaga el LED
   - Muestra mensaje "LIBRE"
4. Se repite infinitamente
===========================================
*/
