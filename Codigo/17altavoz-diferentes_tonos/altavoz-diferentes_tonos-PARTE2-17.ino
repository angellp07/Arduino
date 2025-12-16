const int SPEAKER_PIN = 11;    // Pin del altavoz (PWM)
const int PHOTOCELL_PIN = A0;  // Pin del fotoresistor

// Variables para suavizado y control
int lastReading = 0;      // Para detectar cambios
int lastPitch = 0;        // Última frecuencia generada
const int MIN_PITCH = 100;   // Frecuencia mínima (Hz)
const int MAX_PITCH = 1000;  // Frecuencia máxima (Hz)
const int CHANGE_THRESHOLD = 5; // Cambio mínimo para actualizar tono

void setup() {
  Serial.begin(9600);  // Inicia comunicación serial para monitoreo
  Serial.println("====================================");
  Serial.println("THEREMIN OPTICO - Fotoresistor + Altavoz");
  Serial.println("Acerca/aleja la mano para cambiar el tono");
  Serial.println("====================================");
  
  // Configurar pines (opcional pero buena práctica)
  pinMode(SPEAKER_PIN, OUTPUT);
  // Los pines analógicos no necesitan pinMode para lectura
}

void loop() {
  // 1. LEER FOTORESISTOR
  int reading = analogRead(PHOTOCELL_PIN);
  
  // 2. APLICAR FILTRO PARA SUAVIZAR (promedio móvil simple)
  static int smoothReading = 512;  // Valor inicial en medio del rango
  smoothReading = (smoothReading * 7 + reading * 3) / 10;  // 70% anterior, 30% nuevo
  
  // 3. CONVERTIR A FRECUENCIA (con rango más amplio)
  int pitch = map(smoothReading, 0, 1023, MIN_PITCH, MAX_PITCH);
  
  // 4. SOLO CAMBIAR TONO SI HAY CAMBIO SIGNIFICATIVO
  if (abs(pitch - lastPitch) > CHANGE_THRESHOLD) {
    // Detener tono anterior (evita superposición)
    noTone(SPEAKER_PIN);
    
    // Generar nuevo tono
    tone(SPEAKER_PIN, pitch);
    
    // Actualizar último tono
    lastPitch = pitch;
    
    // 5. MOSTRAR INFORMACIÓN (opcional, para depuración)
    Serial.print("Luz: ");
    Serial.print(reading);
    Serial.print(" | Suave: ");
    Serial.print(smoothReading);
    Serial.print(" | Frecuencia: ");
    Serial.print(pitch);
    Serial.println(" Hz");
  }
  
  // 6. PEQUEÑA PAUSA PARA NO SATURAR
  delay(50);  // 20 actualizaciones por segundo (suficiente para control)
}

/*
 * ============================================================
 * MEJORAS IMPLEMENTADAS:
 * ============================================================
 * 
 * 1. SUAVIZADO DE LECTURA:
 *    - Promedio móvil para evitar fluctuaciones bruscas
 *    - Lecturas más estables y musicales
 * 
 * 2. DETECCIÓN DE CAMBIOS:
 *    - Solo cambia el tono si hay diferencia significativa
 *    - Evita parpadeo sonoro con lecturas similares
 * 
 * 3. RANGO MÁS AMPLIO (100-1000 Hz):
 *    - Más expresividad musical
 *    - Casi 3 octavas de rango
 * 
 * 4. MONITOR SERIAL:
 *    - Permite ver valores en tiempo real
 *    - Útil para calibración y aprendizaje
 * 
 * 5. CONTROL DE VELOCIDAD:
 *    - delay(50) para 20 actualizaciones/segundo
 *    - Balance entre respuesta y estabilidad
 * 
 * 6. BUENAS PRÁCTICAS:
 *    - Constantes en mayúsculas
 *    - Variables descriptivas
 *    - Comentarios explicativos
 * ============================================================
 */