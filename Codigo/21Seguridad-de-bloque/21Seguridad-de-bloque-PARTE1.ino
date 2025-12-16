#include <Servo.h>

const int buttonPin = 8;     // Botón en pin digital 8
const int potPin = A0;       // Potenciómetro en pin analógico A0
const int servoPin = 2;      // Servo en pin digital 2
const int ledPin = 9;        // LED verde en pin digital 9
const int redLedPin = 10;    // LED rojo en pin digital 10

Servo myServo;  // Crear objeto Servo

int buttonState = 0;         // Estado actual del botón
int lastButtonState = HIGH;  // Estado anterior del botón (inicializado en HIGH por INPUT_PULLUP)
int potValue = 0;            // Valor leído del potenciómetro
int servoAngle = 0;          // Ángulo del servo
bool controlEnabled = false; // Estado del control (habilitado/deshabilitado)

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Configurar pin del botón como entrada con resistencia pull-up interna
  pinMode(ledPin, OUTPUT);           // Configurar pin del LED verde como salida
  pinMode(redLedPin, OUTPUT);        // Configurar pin del LED rojo como salida
  myServo.attach(servoPin);          // Asociar servo al pin especificado
  
  // Estado inicial de los LEDs
  digitalWrite(ledPin, LOW);         // LED verde apagado inicialmente
  digitalWrite(redLedPin, HIGH);     // LED rojo encendido inicialmente (control deshabilitado)
}

void loop() {
  buttonState = digitalRead(buttonPin);  // Leer estado del botón

  // Detectar cambios en el estado del botón para alternar modo de control
  if (buttonState == LOW && lastButtonState == HIGH) {
    controlEnabled = !controlEnabled;  // Alternar estado del control
    delay(200);  // Retardo para eliminar rebotes
  }
  lastButtonState = buttonState;  // Actualizar estado anterior del botón

  // Controlar el estado de los LEDs
  if (controlEnabled) {
    digitalWrite(ledPin, HIGH);    // Encender LED verde cuando el control está habilitado
    digitalWrite(redLedPin, LOW);  // Apagar LED rojo cuando el control está habilitado
    potValue = analogRead(potPin); // Leer valor del potenciómetro
    servoAngle = map(potValue, 0, 1023, 0, 180);  // Mapear valor del potenciómetro a ángulo del servo (0 a 180 grados)
    myServo.write(servoAngle);     // Establecer ángulo del servo
  } else {
    digitalWrite(ledPin, LOW);     // Apagar LED verde cuando el control está deshabilitado
    digitalWrite(redLedPin, HIGH); // Encender LED rojo cuando el control está deshabilitado
  }
}
