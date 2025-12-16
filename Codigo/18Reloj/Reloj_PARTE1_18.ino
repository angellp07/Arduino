/*
 * ============================================================
 * DISPLAY 7 SEGMENTOS 4 DÍGITOS - EXPLICACIÓN COMPLETA
 * Arduino UNO
 * ============================================================
 * 
 * OBJETIVO: Mostrar números de 4 dígitos en un display de 7 segmentos
 *           usando multiplexación (multiplexing).
 * 
 * PRINCIPIO: Multiplexación por tiempo - Enciende un dígito a la vez
 *            muy rápido (54µs cada uno) para que el ojo humano
 *            vea los 4 dígitos encendidos simultáneamente.
 * 
 * CONEXIONES REQUERIDAS:
 * 
 * 1. SEGMENTOS (a-g): Controlan qué segmentos se encienden
 *    A (ASeg) → Pin 9
 *    B (BSeg) → Pin 13
 *    C (CSeg) → Pin 4
 *    D (DSeg) → Pin 6
 *    E (ESeg) → Pin 7
 *    F (FSeg) → Pin 10
 *    G (GSeg) → Pin 3
 * 
 * 2. DÍGITOS (1-4): Controlan qué posición se enciende
 *    Dígito 1 (a1) → Pin 8
 *    Dígito 2 (a2) → Pin 11
 *    Dígito 3 (a3) → Pin 12
 *    Dígito 4 (a4) → Pin 2
 * 
 * 3. TIPO DE DISPLAY: CÁTODO COMÚN (los segmentos se encienden con HIGH)
 * ============================================================
 */

// ==================== PARTE 1: DECLARACIÓN DE PINES DE SEGMENTOS ====================
int ASeg = 9;   // Segmento A
int BSeg = 13;  // Segmento B
int CSeg = 4;   // Segmento C
int DSeg = 6;   // Segmento D
int ESeg = 7;   // Segmento E
int FSeg = 10;  // Segmento F
int GSeg = 3;   // Segmento G

/*
 * DISPOSICIÓN DE LOS SEGMENTOS:
 *        A
 *       ---
 *    F |   | B
 *       ---  <-- G
 *    E |   | C
 *       ---
 *        D
 * 
 * Cada segmento es un LED individual.
 * Para formar números, se encienden combinaciones específicas.
 */

// ==================== PARTE 2: DECLARACIÓN DE PINES DE DÍGITOS ====================
int a1 = 8;  // Control del dígito 1 (más a la izquierda)
int a2 = 11; // Control del dígito 2
int a3 = 12; // Control del dígito 3
int a4 = 2;  // Control del dígito 4 (más a la derecha)

/*
 * ORGANIZACIÓN DE DÍGITOS:
 *   [DÍGITO 1] [DÍGITO 2] [DÍGITO 3] [DÍGITO 4]
 *   (Miles)    (Centenas)  (Decenas)  (Unidades)
 */

// ==================== PARTE 3: VARIABLES DE CONTROL ====================
long n = 0;      // Contador que se muestra (incrementa continuamente)
int x = 100;     // Divisor para ajustar velocidad del contador
int del = 54;    // Tiempo de multiplexación en microsegundos

/*
 * EXPLICACIÓN DE VARIABLES:
 * 
 * 1. long n = 0:
 *    - Contador principal que va de 0 a 9999
 *    - Se incrementa en 1 cada ciclo completo de 4 dígitos
 *    - Tipo 'long' para números grandes (hasta 2,147,483,647)
 * 
 * 2. int x = 100:
 *    - Divisor que controla qué tan rápido incrementa 'n'
 *    - n/x muestra los números
 *    - Con x=100: n incrementa, pero n/x cambia más lento
 *    - Ejemplo: si n=500, n/x = 5 (se muestra "0005")
 * 
 * 3. int del = 54:
 *    - Tiempo que cada dígito permanece encendido (en µs)
 *    - 54µs × 4 dígitos = 216µs por ciclo completo
 *    - 1 segundo / 216µs ≈ 4,630 ciclos por segundo
 *    - Suficiente para que el ojo vea todos encendidos
 */

// ==================== PARTE 4: SETUP() - CONFIGURACIÓN ====================
void setup()
{
  // Configurar todos los pines como salidas
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(a3, OUTPUT);
  pinMode(a4, OUTPUT);
  pinMode(ASeg, OUTPUT);
  pinMode(BSeg, OUTPUT);
  pinMode(CSeg, OUTPUT);
  pinMode(DSeg, OUTPUT);
  pinMode(ESeg, OUTPUT);
  pinMode(FSeg, OUTPUT);
  pinMode(GSeg, OUTPUT);
  
  /*
   * IMPORTANTE: Este display es de CÁTODO COMÚN
   * - Los segmentos se encienden con HIGH (1)
   * - Los dígitos se activan con HIGH (1)
   * 
   * OJO: ¡Es al revés de lo que vimos con los LEDs!
   */
}

// ==================== PARTE 5: LOOP() - BUCLE PRINCIPAL ====================
void loop()
{
   // DÍGITO 1 (MILES): Muestra el dígito de las unidades de mil
   clearLEDS();              // Apagar todos los segmentos
   pickDigit(1);             // Activar solo el primer dígito
   pickNumber((n/x/1000)%10); // Calcular y mostrar el dígito
   delayMicroseconds(del);   // Mantener encendido 54µs
   
   // DÍGITO 2 (CENTENAS): Muestra el dígito de las centenas
   clearLEDS();
   pickDigit(2);
   pickNumber((n/x/100)%10);
   delayMicroseconds(del);
     
   // DÍGITO 3 (DECENAS): Muestra el dígito de las decenas
   clearLEDS();
   pickDigit(3);
   pickNumber((n/x/10)%10);
   delayMicroseconds(del);
     
   // DÍGITO 4 (UNIDADES): Muestra el dígito de las unidades
   clearLEDS();
   pickDigit(4);
   pickNumber(n/x%10);
   delayMicroseconds(del);
   
   // INCREMENTAR EL CONTADOR
   n++;  // Aumenta n en 1 cada ciclo completo
   
   /*
    * CÁLCULO DE LOS DÍGITOS:
    * 
    * Para n = 1234 y x = 100:
    * n/x = 12.34 → se muestra como 12 (enteros)
    * 
    * Dígito 1: (1234/100/1000)%10 = (12/1000)%10 = 0%10 = 0
    * Dígito 2: (1234/100/100)%10 = (12/100)%10 = 0%10 = 0  
    * Dígito 3: (1234/100/10)%10 = (12/10)%10 = 1%10 = 1
    * Dígito 4: (1234/100)%10 = 12%10 = 2
    * 
    * Se muestra: "0012"
    */
}

// ==================== PARTE 6: FUNCIÓN pickDigit() - SELECCIONAR DÍGITO ====================
void pickDigit(int x) 
{
  // Apagar todos los dígitos primero
  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
  digitalWrite(a3, LOW);
  digitalWrite(a4, LOW);

  // Encender solo el dígito seleccionado
  switch(x)
  {
  case 1:
    digitalWrite(a1, HIGH);  // Activar dígito 1
    break;
  case 2:
    digitalWrite(a2, HIGH);  // Activar dígito 2
    break;
  case 3:
    digitalWrite(a3, HIGH);  // Activar dígito 3
    break;
  case 4:
    digitalWrite(a4, HIGH);  // Activar dígito 4
    break;  
  }
  
  /*
   * IMPORTANTE: Este display usa lógica POSITIVA
   * - HIGH en el pin del dígito = dígito ENCENDIDO
   * - LOW en el pin del dígito = dígito APAGADO
   */
}

// ==================== PARTE 7: FUNCIÓN pickNumber() - MOSTRAR NÚMERO ====================
void pickNumber(int x) 
 {
   // Seleccionar qué número mostrar (0-9)
   switch(x) {
   case 1: one(); break;
   case 2: two(); break;
   case 3: three(); break;
   case 4: four(); break;
   case 5: five(); break;
   case 6: six(); break;
   case 7: seven(); break;
   case 8: eight(); break;
   case 9: nine(); break;
   default: zero(); break;  // 0 o cualquier otro valor
 }
}

// ==================== PARTE 8: FUNCIÓN clearLEDS() - LIMPIAR PANTALLA ====================
void clearLEDS()
{
   // Poner todos los segmentos en HIGH (apagado para display cátodo común)
   digitalWrite(ASeg, HIGH);
   digitalWrite(BSeg, HIGH);
   digitalWrite(CSeg, HIGH);
   digitalWrite(DSeg, HIGH);
   digitalWrite(ESeg, HIGH);
   digitalWrite(FSeg, HIGH);
   digitalWrite(GSeg, HIGH);
   
   /*
    * ¿POR QUÉ HIGH APAGA LOS SEGMENTOS?
    * En display CÁTODO COMÚN:
    * - Los cátodos de todos los LEDs están conectados juntos
    * - Para encender un LED: ÁNODO=HIGH, CÁTODO=LOW
    * - Los pines de segmentos son los ánodos
    * - HIGH en segmento + LOW en dígito = LED ENCENDIDO
    * - HIGH en ambos = LED APAGADO (sin diferencia de voltaje)
    */
}

// ==================== PARTE 9: FUNCIONES PARA CADA NÚMERO (0-9) ====================

void zero() {
  // Patrón para mostrar "0": Todos los segmentos excepto G
  digitalWrite(ASeg, LOW);  // A encendido
  digitalWrite(BSeg, LOW);  // B encendido
  digitalWrite(CSeg, LOW);  // C encendido
  digitalWrite(DSeg, LOW);  // D encendido
  digitalWrite(ESeg, LOW);  // E encendido
  digitalWrite(FSeg, LOW);  // F encendido
  digitalWrite(GSeg, HIGH); // G apagado
}

void one() { 
  // Patrón para "1": Solo segmentos B y C
  digitalWrite(ASeg, HIGH);
  digitalWrite(BSeg, LOW);   // B encendido
  digitalWrite(CSeg, LOW);   // C encendido
  digitalWrite(DSeg, HIGH);
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, HIGH);
  digitalWrite(GSeg, HIGH);
}

void two() {
  // Patrón para "2": A, B, G, E, D
  digitalWrite(ASeg, LOW);   // A
  digitalWrite(BSeg, LOW);   // B
  digitalWrite(CSeg, HIGH);
  digitalWrite(DSeg, LOW);   // D
  digitalWrite(ESeg, LOW);   // E
  digitalWrite(FSeg, HIGH);
  digitalWrite(GSeg, LOW);   // G
}

void three() {
  // Patrón para "3": A, B, C, D, G
  digitalWrite(ASeg, LOW);   // A
  digitalWrite(BSeg, LOW);   // B
  digitalWrite(CSeg, LOW);   // C
  digitalWrite(DSeg, LOW);   // D
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, HIGH);
  digitalWrite(GSeg, LOW);   // G
}

void four() {
  // Patrón para "4": F, G, B, C
  digitalWrite(ASeg, HIGH);
  digitalWrite(BSeg, LOW);   // B
  digitalWrite(CSeg, LOW);   // C
  digitalWrite(DSeg, HIGH);
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, LOW);   // F
  digitalWrite(GSeg, LOW);   // G
}

void five() {
  // Patrón para "5": A, F, G, C, D
  digitalWrite(ASeg, LOW);   // A
  digitalWrite(BSeg, HIGH);
  digitalWrite(CSeg, LOW);   // C
  digitalWrite(DSeg, LOW);   // D
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, LOW);   // F
  digitalWrite(GSeg, LOW);   // G
}

void six() {
  // Patrón para "6": A, F, E, D, C, G
  digitalWrite(ASeg, LOW);   // A
  digitalWrite(BSeg, HIGH);
  digitalWrite(CSeg, LOW);   // C
  digitalWrite(DSeg, LOW);   // D
  digitalWrite(ESeg, LOW);   // E
  digitalWrite(FSeg, LOW);   // F
  digitalWrite(GSeg, LOW);   // G
}

void seven() {
  // Patrón para "7": A, B, C
  digitalWrite(ASeg, LOW);   // A
  digitalWrite(BSeg, LOW);   // B
  digitalWrite(CSeg, LOW);   // C
  digitalWrite(DSeg, HIGH);
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, HIGH);
  digitalWrite(GSeg, HIGH);
}

void eight() {
  // Patrón para "8": Todos los segmentos
  digitalWrite(ASeg, LOW);   // A
  digitalWrite(BSeg, LOW);   // B
  digitalWrite(CSeg, LOW);   // C
  digitalWrite(DSeg, LOW);   // D
  digitalWrite(ESeg, LOW);   // E
  digitalWrite(FSeg, LOW);   // F
  digitalWrite(GSeg, LOW);   // G
}

void nine() {
  // Patrón para "9": A, B, C, D, F, G
  digitalWrite(ASeg, LOW);   // A
  digitalWrite(BSeg, LOW);   // B
  digitalWrite(CSeg, LOW);   // C
  digitalWrite(DSeg, LOW);   // D
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, LOW);   // F
  digitalWrite(GSeg, LOW);   // G
}


/*
 * ============================================================
 * VERSIÓN SIMPLE PARA PROBAR:
 * ============================================================
 */

/*
// Configuración de pines
int segmentos[] = {9, 13, 4, 6, 7, 10, 3};  // A, B, C, D, E, F, G
int digitos[] = {8, 11, 12, 2};              // D1, D2, D3, D4

// Patrones para números 0-9 (1=encendido, 0=apagado)
byte patrones[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void setup() {
  // Configurar pines
  for(int i=0; i<7; i++) pinMode(segmentos[i], OUTPUT);
  for(int i=0; i<4; i++) pinMode(digitos[i], OUTPUT);
}

void loop() {
  static int numero = 0;
  
  // Mostrar número
  for(int d=0; d<4; d++) {
    // Apagar todos los dígitos
    for(int i=0; i<4; i++) digitalWrite(digitos[i], LOW);
    
    // Encender solo este dígito
    digitalWrite(digitos[d], HIGH);
    
    // Mostrar el dígito correspondiente
    int digito = obtenerDigito(numero, d);
    mostrarDigito(digito);
    
    delay(5);  // Multiplexación
  }
  
  // Incrementar número cada segundo aprox.
  static unsigned long ultimo = 0;
  if(millis() - ultimo > 1000) {
    numero++;
    if(numero > 9999) numero = 0;
    ultimo = millis();
  }
}

int obtenerDigito(int num, int pos) {
  // pos: 0=miles, 1=centenas, 2=decenas, 3=unidades
  switch(pos) {
    case 0: return (num/1000)%10;
    case 1: return (num/100)%10;
    case 2: return (num/10)%10;
    case 3: return num%10;
  }
  return 0;
}

void mostrarDigito(int num) {
  // Apagar todos los segmentos primero
  for(int i=0; i<7; i++) digitalWrite(segmentos[i], LOW);
  
  // Encender segmentos según patrón
  for(int i=0; i<7; i++) {
    digitalWrite(segmentos[i], patrones[num][i]);
  }
}
*/

/*
 * ============================================================
 * CONSEJOS PARA TU DISPLAY 7 SEGMENTOS:
 * ============================================================
 * 
 * 1. IDENTIFICAR TIPO DE DISPLAY:
 *    - Cátodo común: Segmentos en HIGH para encender
 *    - Ánodo común: Segmentos en LOW para encender
 * 
 * 2. RESISTORES LIMITADORES:
 *    - Necesitas resistores para cada segmento (220-330Ω)
 *    - O resistores para cada dígito
 * 
 * 3. BRIGHTNESS (brillo):
 *    - Aumentar tiempo de multiplexación = más brillo
 *    - Disminuir tiempo = menos brillo pero más estable
 * 
 * 4. PROBLEMAS COMUNES:
 *    - Fantasmas: Apagar segmentos ANTES de cambiar dígito
 *    - Brillo desigual: Ajustar tiempos por dígito
 *    - Números ilegibles: Verificar patrones de segmentos
 * 
 * 5. EXPERIMENTOS:
 *    - Mostrar letras (A, b, C, d, E, F)
 *    - Contador con botón para pausar/reiniciar
 *    - Termómetro digital con sensor
 *    - Reloj con RTC (Real Time Clock)
 * ============================================================
 */