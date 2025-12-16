/*
 * SERVOMOTOR BÁSICO PERO COMPLETO
 * Versión ultra-optimizada para Arduino UNO
 */

#include <Servo.h>

Servo myservo;
const byte PIN_SERVO = 9;
byte pos = 90;
byte velocidad = 2;  // 1=lento, 2=medio, 3=rápido
boolean automatico = false;

void setup() {
  Serial.begin(9600);
  myservo.attach(PIN_SERVO);
  myservo.write(pos);
  Serial.println(F("Servo listo. Comandos:"));
  Serial.println(F("A: Auto  B: Manual  C: Parar"));
  Serial.println(F("+: +10°  -: -10°    0: 90°"));
  Serial.println(F("S: Velocidad  M: Menu"));
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    procesarComando(c);
  }
  
  if (automatico) {
    modoAuto();
  }
}

void procesarComando(char c) {
  c = toupper(c);
  
  switch(c) {
    case 'A':
      automatico = true;
      Serial.println(F("Modo AUTO"));
      break;
      
    case 'B':
      automatico = false;
      Serial.println(F("Modo MANUAL"));
      Serial.println(F("Usa + - 0 para mover"));
      break;
      
    case 'C':
      automatico = false;
      Serial.println(F("PARADO"));
      break;
      
    case '+':
      moverServo(pos + 10);
      break;
      
    case '-':
      moverServo(pos - 10);
      break;
      
    case '0':
      moverServo(90);
      break;
      
    case 'S':
      cambiarVelocidad();
      break;
      
    case 'M':
      mostrarMenu();
      break;
  }
}

void moverServo(byte nuevaPos) {
  // Limitar entre 30 y 150 grados
  if (nuevaPos < 30) nuevaPos = 30;
  if (nuevaPos > 150) nuevaPos = 150;
  
  // Calcular delay según velocidad
  byte delayMs;
  switch(velocidad) {
    case 1: delayMs = 30; break;
    case 2: delayMs = 15; break;
    case 3: delayMs = 5; break;
    default: delayMs = 15;
  }
  
  // Mover gradualmente
  if (nuevaPos > pos) {
    for (byte i = pos; i <= nuevaPos; i++) {
      myservo.write(i);
      delay(delayMs);
    }
  } else {
    for (byte i = pos; i >= nuevaPos; i--) {
      myservo.write(i);
      delay(delayMs);
    }
  }
  
  pos = nuevaPos;
  Serial.print(F("Posicion: "));
  Serial.print(pos);
  Serial.println(F("°"));
}

void modoAuto() {
  static boolean subiendo = true;
  
  if (subiendo) {
    moverServo(150);
    if (pos >= 149) subiendo = false;
  } else {
    moverServo(30);
    if (pos <= 31) subiendo = true;
  }
}

void cambiarVelocidad() {
  velocidad++;
  if (velocidad > 3) velocidad = 1;
  
  Serial.print(F("Velocidad: "));
  switch(velocidad) {
    case 1: Serial.println(F("LENTA")); break;
    case 2: Serial.println(F("MEDIA")); break;
    case 3: Serial.println(F("RAPIDA")); break;
  }
}

void mostrarMenu() {
  Serial.println(F("\n=== MENU SERVO ==="));
  Serial.println(F("A: Modo automatico"));
  Serial.println(F("B: Modo manual"));
  Serial.println(F("C: Parar"));
  Serial.println(F("+: +10 grados"));
  Serial.println(F("-: -10 grados"));
  Serial.println(F("0: Centro (90°)"));
  Serial.println(F("S: Cambiar velocidad"));
  Serial.println(F("M: Mostrar menu"));
  Serial.println(F("=================="));
}