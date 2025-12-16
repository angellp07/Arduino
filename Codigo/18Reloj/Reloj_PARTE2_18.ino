//========================================================
// VERSIÓN MAS OPTIMIZADA
//========================================================

/*
 * DISPLAY 7 SEGMENTOS - ÁNODO COMÚN - FUNCIONA CONFIRMADO
 * 
 * Tu display ENCIENDE con:
 * - Dígitos: HIGH = activado
 * - Segmentos: LOW = encendido
 */

// ================== CONFIGURACIÓN DE PINES ==================
// Segmentos (forman los números)
const int SEG_A = 9;    // Pin 9
const int SEG_B = 13;   // Pin 13  
const int SEG_C = 4;    // Pin 4
const int SEG_D = 6;    // Pin 6
const int SEG_E = 7;    // Pin 7
const int SEG_F = 10;   // Pin 10
const int SEG_G = 3;    // Pin 3

// Dígitos (controlan posición)
const int DIG1 = 8;     // Dígito 1 (izquierda) - Pin 8
const int DIG2 = 11;    // Dígito 2 - Pin 11
const int DIG3 = 12;    // Dígito 3 - Pin 12
const int DIG4 = 2;     // Dígito 4 (derecha) - Pin 2

// ================== VARIABLES ==================
int numeroMostrar = 0;  // Número a mostrar (0-9999)
int brillo = 5000;      // Tiempo en microsegundos (5ms)

// ================== SETUP ==================
void setup() {
  Serial.begin(9600);
  Serial.println("DISPLAY ANODO COMUN - CONFIRMADO FUNCIONA");
  
  // Configurar pines de segmentos
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  
  // Configurar pines de dígitos
  pinMode(DIG1, OUTPUT);
  pinMode(DIG2, OUTPUT);
  pinMode(DIG3, OUTPUT);
  pinMode(DIG4, OUTPUT);
  
  // Estado inicial: apagar todo
  apagarTodo();
  
  Serial.println("Mostrando contador 0000 a 9999");
  Serial.println("Cada número dura 2 segundos");
}

// ================== LOOP PRINCIPAL ==================
void loop() {
  // 1. Mostrar el número actual
  mostrarNumero4Digitos(numeroMostrar);
  
  // 2. Cambiar número cada 2 segundos
  static unsigned long tiempoAnterior = 0;
  if (millis() - tiempoAnterior > 2000) {  // Cada 2000ms = 2 segundos
    numeroMostrar++;
    if (numeroMostrar > 9999) {
      numeroMostrar = 0;
    }
    tiempoAnterior = millis();
    
    Serial.print("Mostrando: ");
    Serial.println(numeroMostrar);
  }
}

// ================== FUNCIÓN PRINCIPAL: MOSTRAR NÚMERO ==================
void mostrarNumero4Digitos(int numero) {
  // Limitar número entre 0 y 9999
  if (numero < 0) numero = 0;
  if (numero > 9999) numero = 9999;
  
  // Separar en dígitos individuales
  int d1 = (numero / 1000) % 10;  // Dígito 1: Miles
  int d2 = (numero / 100) % 10;   // Dígito 2: Centenas
  int d3 = (numero / 10) % 10;    // Dígito 3: Decenas
  int d4 = numero % 10;           // Dígito 4: Unidades
  
  // Mostrar cada dígito usando multiplexación
  // (encender un dígito a la vez muy rápido)
  
  // Dígito 1 (más a la izquierda)
  mostrarUnDigito(1, d1);
  delayMicroseconds(brillo);
  
  // Dígito 2
  mostrarUnDigito(2, d2);
  delayMicroseconds(brillo);
  
  // Dígito 3
  mostrarUnDigito(3, d3);
  delayMicroseconds(brillo);
  
  // Dígito 4 (más a la derecha)
  mostrarUnDigito(4, d4);
  delayMicroseconds(brillo);
}

// ================== FUNCIÓN: MOSTRAR UN SOLO DÍGITO ==================
void mostrarUnDigito(int cualDigito, int numero) {
  // 1. Primero apagar TODO
  apagarSegmentos();
  apagarDigitos();
  
  // 2. Activar SOLO el dígito que queremos
  // Para ÁNODO COMÚN: HIGH activa el dígito
  switch(cualDigito) {
    case 1: digitalWrite(DIG1, HIGH); break;
    case 2: digitalWrite(DIG2, HIGH); break;
    case 3: digitalWrite(DIG3, HIGH); break;
    case 4: digitalWrite(DIG4, HIGH); break;
  }
  
  // 3. Encender los segmentos para formar el número
  // Para ÁNODO COMÚN: LOW enciende el segmento
  switch(numero) {
    case 0:
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, LOW);   // E
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, HIGH);  // G apagado
      break;
      
    case 1:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, HIGH);
      break;
      
    case 2:
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, LOW);   // E
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 3:
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 4:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 5:
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 6:
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, LOW);   // E
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 7:
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, HIGH);
      break;
      
    case 8:
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, LOW);   // E
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    case 9:
      digitalWrite(SEG_A, LOW);   // A
      digitalWrite(SEG_B, LOW);   // B
      digitalWrite(SEG_C, LOW);   // C
      digitalWrite(SEG_D, LOW);   // D
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, LOW);   // F
      digitalWrite(SEG_G, LOW);   // G
      break;
      
    default:  // Si no es 0-9, mostrar "-" (guión)
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, LOW);   // Solo G encendido
  }
}

// ================== FUNCIONES AUXILIARES ==================
void apagarSegmentos() {
  // Apagar todos los segmentos
  // Para ÁNODO COMÚN: HIGH = segmento apagado
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
}

void apagarDigitos() {
  // Desactivar todos los dígitos
  // Para ÁNODO COMÚN: LOW = dígito desactivado
  digitalWrite(DIG1, LOW);
  digitalWrite(DIG2, LOW);
  digitalWrite(DIG3, LOW);
  digitalWrite(DIG4, LOW);
}

void apagarTodo() {
  apagarSegmentos();
  apagarDigitos();
}

/*
 * ============================================================
 * RESUMEN DE CONFIGURACIÓN CONFIRMADA:
 * ============================================================
 * 
 * TU DISPLAY ES: ÁNODO COMÚN (comprobado)
 * 
 * LÓGICA CONFIRMADA:
 * - Dígitos: HIGH los activa, LOW los desactiva
 * - Segmentos: LOW los enciende, HIGH los apaga
 * 
 * CONEXIONES:
 * Segmento A → Pin 9 Arduino
 * Segmento B → Pin 13 Arduino
 * Segmento C → Pin 4 Arduino  
 * Segmento D → Pin 6 Arduino
 * Segmento E → Pin 7 Arduino
 * Segmento F → Pin 10 Arduino
 * Segmento G → Pin 3 Arduino
 * 
 * Dígito 1 → Pin 8 Arduino
 * Dígito 2 → Pin 11 Arduino
 * Dígito 3 → Pin 12 Arduino
 * Dígito 4 → Pin 2 Arduino
 * ============================================================
 */