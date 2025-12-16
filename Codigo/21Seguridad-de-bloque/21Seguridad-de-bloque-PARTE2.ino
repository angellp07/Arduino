/*
================================================================================
PROYECTO: CONTROL DE SERVO CON BOTÓN Y POTENCIÓMETRO - VERSIÓN MEJORADA Y CORREGIDA
================================================================================
DESCRIPCIÓN:
Controla un servo motor mediante un potenciómetro, con un sistema de
habilitación/deshabilitación mediante botón. Incluye indicadores LED
y medidas de seguridad.

COMPONENTES CONECTADADOS:
- Botón: Pin 8 (con resistencia pull-up interna)
- Potenciómetro: Pin A0 (entrada analógica)
- Servo motor: Pin 2 (señal PWM)
- LED verde: Pin 9 (indicador "SISTEMA ACTIVO")
- LED rojo: Pin 10 (indicador "SISTEMA BLOQUEADO")
================================================================================
*/

#include <Servo.h>  // Biblioteca para control de servomotores

// ==============================================================================
// DEFINICIÓN DE PINES (CONSTANTES - NO CAMBIAN DURANTE LA EJECUCIÓN)
// ==============================================================================
const int BOTON_PIN = 8;      // Pin para el botón de control
const int POT_PIN = A0;       // Pin para el potenciómetro (lectura analógica)
const int SERVO_PIN = 2;      // Pin para la señal del servo motor
const int LED_VERDE_PIN = 9;  // Pin para LED verde (sistema activo)
const int LED_ROJO_PIN = 10;  // Pin para LED rojo (sistema bloqueado)

// ==============================================================================
// PARÁMETROS AJUSTABLES DEL SISTEMA
// ==============================================================================
const unsigned long TIEMPO_DEBOUNCE = 50;   // Tiempo para filtrar rebotes (ms)
const int SUAVIZADO_POT = 5;               // Nivel de suavizado para potenciómetro
const int LIMITE_CAMBIO_ANGULO = 10;       // Cambio máximo por ciclo (evita brusquedad)

// ==============================================================================
// VARIABLES GLOBALES DEL SISTEMA
// ==============================================================================
Servo miServo;                // Objeto para controlar el servo motor

// Variables de estado del sistema
bool sistemaActivo = false;   // true = control habilitado, false = deshabilitado
int anguloServo = 90;         // Ángulo actual del servo (posición inicial 90°)
int anguloObjetivo = 90;      // Ángulo que queremos alcanzar

// Variables para control del botón (debounce)
unsigned long ultimoCambioBoton = 0;  // Último momento en que cambió el botón
int ultimoEstadoBoton = HIGH;         // Último estado estable del botón
int estadoBotonActual = HIGH;         // Estado actual leído del botón
bool botonPresionado = false;         // Indica si el botón fue presionado y procesado

// Variables para suavizado del potenciómetro
int lecturaPotSuavizada = 512;        // Valor suavizado del potenciómetro

// ==============================================================================
// FUNCIÓN setup() - CONFIGURACIÓN INICIAL
// ==============================================================================
void setup() {
  // Iniciar comunicación serial para depuración y monitoreo
  Serial.begin(115200);  // Alta velocidad para mensajes frecuentes
  Serial.println("========================================");
  Serial.println("CONTROL DE SERVO - SISTEMA INICIADO");
  Serial.println("========================================");
  Serial.println("Instrucciones:");
  Serial.println("1. Presiona el botón para activar/desactivar");
  Serial.println("2. Gira el potenciómetro para mover el servo");
  Serial.println("========================================");

  // ==========================================
  // CONFIGURACIÓN DE PINES COMO ENTRADAS/SALIDAS
  // ==========================================
  
  // Botón: entrada con resistencia pull-up interna
  // La resistencia mantiene el pin en HIGH cuando no se presiona
  // Al presionar, el pin va a LOW (conexión a GND)
  pinMode(BOTON_PIN, INPUT_PULLUP);
  
  // LEDs: configurados como salidas digitales
  pinMode(LED_VERDE_PIN, OUTPUT);  // LED para indicar "sistema activo"
  pinMode(LED_ROJO_PIN, OUTPUT);   // LED para indicar "sistema bloqueado"
  
  // Servo: asociar el objeto servo al pin de control
  miServo.attach(SERVO_PIN);
  
  // ==========================================
  // ESTADO INICIAL DEL SISTEMA
  // ==========================================
  
  // Posicionar servo en ángulo inicial (90° - posición central)
  miServo.write(anguloServo);
  
  // Configurar LEDs según estado inicial (sistema desactivado)
  digitalWrite(LED_VERDE_PIN, LOW);   // Verde apagado
  digitalWrite(LED_ROJO_PIN, HIGH);   // Rojo encendido (bloqueado)
  
  // Informar estado inicial por serial
  Serial.println("Estado inicial: SISTEMA BLOQUEADO");
  Serial.print("Servo en posición: ");
  Serial.print(anguloServo);
  Serial.println("°");
  Serial.println("----------------------------------------");
}

// ==============================================================================
// FUNCIÓN loop() - BUCLE PRINCIPAL (se ejecuta continuamente)
// ==============================================================================
void loop() {
  // ----------------------------------------------------
  // PASO 1: LEER Y PROCESAR EL BOTÓN (CON DEBOUNCE MEJORADO)
  // ----------------------------------------------------
  procesarBoton();
  
  // ----------------------------------------------------
  // PASO 2: CONTROLAR INDICADORES LED SEGÚN ESTADO
  // ----------------------------------------------------
  actualizarLEDs();
  
  // ----------------------------------------------------
  // PASO 3: SI EL SISTEMA ESTÁ ACTIVO, LEER POTENCIÓMETRO
  // ----------------------------------------------------
  if (sistemaActivo) {
    leerPotenciometro();
    moverServoSuavemente();
  }
  
  // ----------------------------------------------------
  // PASO 4: PEQUEÑA PAUSA PARA ESTABILIDAD
  // ----------------------------------------------------
  delay(20);  // 20ms = 50 actualizaciones por segundo (óptimo para control)
}

// ==============================================================================
// FUNCIÓN procesarBoton() - GESTIÓN MEJORADA DEL BOTÓN (CORREGIDA)
// ==============================================================================
void procesarBoton() {
  // Leer estado actual del botón
  estadoBotonActual = digitalRead(BOTON_PIN);
  
  // Verificar si el estado del botón cambió
  if (estadoBotonActual != ultimoEstadoBoton) {
    // Reiniciar el temporizador de debounce
    ultimoCambioBoton = millis();
  }
  
  // Actualizar el último estado (esto debe hacerse SIEMPRE, no solo dentro del if)
  ultimoEstadoBoton = estadoBotonActual;
  
  // Verificar si ha pasado suficiente tiempo desde el último cambio (debounce)
  if ((millis() - ultimoCambioBoton) > TIEMPO_DEBOUNCE) {
    // Solo procesar si el botón está presionado (LOW) y aún no se ha procesado esta pulsación
    if (estadoBotonActual == LOW && !botonPresionado) {
      // Cambiar estado del sistema
      sistemaActivo = !sistemaActivo;
      botonPresionado = true;  // Marcar que ya procesamos esta pulsación
      
      // Informar por serial el cambio de estado
      Serial.println("----------------------------------------");
      if (sistemaActivo) {
        Serial.println("✓ BOTÓN PRESIONADO: Sistema ACTIVADO");
        Serial.println("  Ahora puedes controlar el servo con el potenciómetro");
      } else {
        Serial.println("✗ BOTÓN PRESIONADO: Sistema DESACTIVADO");
        Serial.println("  El servo está bloqueado");
        Serial.print("  Servo bloqueado en posición: ");
        Serial.print(anguloServo);
        Serial.println("°");
      }
    }
    
    // Si el botón fue liberado, resetear la bandera de pulsación
    if (estadoBotonActual == HIGH) {
      botonPresionado = false;
    }
  }
}

// ==============================================================================
// FUNCIÓN actualizarLEDs() - CONTROL DE INDICADORES VISUALES
// ==============================================================================
void actualizarLEDs() {
  if (sistemaActivo) {
    // SISTEMA ACTIVO: LED verde encendido, rojo apagado
    digitalWrite(LED_VERDE_PIN, HIGH);
    digitalWrite(LED_ROJO_PIN, LOW);
  } else {
    // SISTEMA BLOQUEADO: LED verde apagado, rojo encendido
    digitalWrite(LED_VERDE_PIN, LOW);
    digitalWrite(LED_ROJO_PIN, HIGH);
  }
}

// ==============================================================================
// FUNCIÓN leerPotenciometro() - LECTURA SUAVIZADA DEL POTENCIÓMETRO
// ==============================================================================
void leerPotenciometro() {
  // Leer valor crudo del potenciómetro (0 a 1023)
  int lecturaCruda = analogRead(POT_PIN);
  
  // Aplicar filtro de suavizado para lecturas estables
  // Fórmula: nuevo_valor = (antiguo * (N-1) + nuevo) / N
  lecturaPotSuavizada = (lecturaPotSuavizada * (SUAVIZADO_POT - 1) + lecturaCruda) / SUAVIZADO_POT;
  
  // Convertir lectura suavizada a ángulo objetivo (0° a 180°)
  anguloObjetivo = map(lecturaPotSuavizada, 0, 1023, 0, 180);
  
  // Informar por serial solo si hay cambio significativo
  static int ultimoAnguloReportado = -1;
  if (abs(anguloObjetivo - ultimoAnguloReportado) > 2) {
    Serial.print("Potenciometro: ");
    Serial.print(lecturaCruda);
    Serial.print(" | Objetivo servo: ");
    Serial.print(anguloObjetivo);
    Serial.println("°");
    ultimoAnguloReportado = anguloObjetivo;
  }
}

// ==============================================================================
// FUNCIÓN moverServoSuavemente() - MOVIMIENTO CONTROLADO DEL SERVO
// ==============================================================================
void moverServoSuavemente() {
  // Calcular diferencia entre posición actual y objetivo
  int diferencia = anguloObjetivo - anguloServo;
  
  // Si hay diferencia significativa
  if (abs(diferencia) > 1) {
    // Limitar cambio máximo por ciclo (evita movimientos bruscos)
    int cambio = diferencia;
    if (abs(cambio) > LIMITE_CAMBIO_ANGULO) {
      cambio = (diferencia > 0) ? LIMITE_CAMBIO_ANGULO : -LIMITE_CAMBIO_ANGULO;
    }
    
    // Aplicar cambio gradual
    anguloServo += cambio;
    
    // Enviar nuevo ángulo al servo
    miServo.write(anguloServo);
    
    // Informar movimiento por serial
    static unsigned long ultimoReporte = 0;
    if (millis() - ultimoReporte > 500) {  // Reportar cada 500ms máximo
      Serial.print("Servo en posición: ");
      Serial.print(anguloServo);
      Serial.println("°");
      ultimoReporte = millis();
    }
  }
}