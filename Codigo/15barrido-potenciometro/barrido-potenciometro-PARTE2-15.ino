/*
 * SERVOMOTOR CON POTENCIÓMETRO - VERSIÓN MEJORADA
 * Con suavizado, límites y monitor serial
 */

#include <Servo.h>

Servo myservo;            // Objeto para controlar el servo
const int POT_PIN = A0;   // Pin del potenciómetro (más claro que '0')
int valorPoten;           // Valor leído del potenciómetro
int anguloServo;          // Ángulo calculado para el servo
int anguloAnterior = 90;  // Para suavizar el movimiento

void setup() {
  Serial.begin(9600);     // Inicia comunicación serial para monitoreo
  myservo.attach(9);      // Servo en pin 9 (PWM)
  
  Serial.println("====================================");
  Serial.println("SERVOMOTOR + POTENCIOMETRO");
  Serial.println("Gira el potenciometro para mover el servo");
  Serial.println("====================================");
}

void loop() {
  // 1. LEER POTENCIÓMETRO (0-1023)
  valorPoten = analogRead(POT_PIN);
  
  // 2. CONVERTIR A ÁNGULO (30°-150° por seguridad)
  anguloServo = map(valorPoten, 0, 1023, 30, 150);
  
  // 3. SUAVIZAR MOVIMIENTO (evita saltos bruscos)
  // Fórmula: 60% valor anterior + 40% valor nuevo
  int anguloSuave = (anguloAnterior * 0.6) + (anguloServo * 0.4);
  
  // 4. MOVER SERVOMOTOR
  myservo.write(anguloSuave);
  anguloAnterior = anguloSuave;  // Guardar para siguiente ciclo
  
  // 5. MOSTRAR INFO POR SERIAL (opcional)
  Serial.print("Poten: ");
  Serial.print(valorPoten);
  Serial.print(" | Angulo: ");
  Serial.print(anguloSuave);
  Serial.println("°");
  
  // 6. PEQUEÑA PAUSA
  delay(20);  // Controla la velocidad de actualización
}

/*
 * EXPLICACIÓN DE LAS MEJORAS:
 * 
 * 1. NOMBRES DESCRIPTIVOS:
 *    - POT_PIN en lugar de potpin
 *    - valorPoten en lugar de val
 *    - Más fácil de entender y mantener
 * 
 * 2. LÍMITES DE SEGURIDAD (30-150°):
 *    - Evita forzar el servo en extremos (0° y 180°)
 *    - Prolonga la vida útil del servo
 * 
 * 3. SUAVIZADO DE MOVIMIENTO:
 *    - Evita saltos bruscos al girar el potenciómetro
 *    - Movimiento más natural y profesional
 * 
 * 4. MONITOR SERIAL:
 *    - Permite ver los valores en tiempo real
 *    - Útil para debugging y calibración
 * 
 * 5. DELAY OPTIMIZADO (20ms):
 *    - Más suave que 15ms
 *    - 50 actualizaciones por segundo (suficiente para control manual)
 */