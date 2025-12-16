/*PRUEBA COLORES*/

// DECLARACIÓN DE PINES PARA EL LED RGB
int redPin = 11;    // Pin PWM para el canal ROJO del LED
int greenPin = 10;  // Pin PWM para el canal VERDE del LED  
int bluePin = 9;    // Pin PWM para el canal AZUL del LED

// COMENTARIO SOBRE TIPO DE LED
// Traducción: "descomenta esta línea si usas un LED de Ánodo Común"
 
// FUNCIÓN setup(): CONFIGURACIÓN INICIAL
void setup()
{
  // CONFIGURA LOS 3 PINES COMO SALIDAS
  pinMode(redPin, OUTPUT);    // Pin 11 como salida para Rojo
  pinMode(greenPin, OUTPUT);  // Pin 10 como salida para Verde
  pinMode(bluePin, OUTPUT);   // Pin 9 como salida para Azul
}
 
void loop()
{
  setColor(255, 0, 0);  // rojo - Rojo al máximo, Verde 0, Azul 0
  delay(1000);           // espera 1 segundo
  
  setColor(0, 255, 0);  // verde - Rojo 0, Verde máximo, Azul 0
  delay(1000);
  
  setColor(0, 0, 255);  // azul - Rojo 0, Verde 0, Azul máximo
  delay(1000);
  
  setColor(255, 255, 0);  // amarillo - Rojo+Verde máximo, Azul 0
  delay(1000);  
  
  setColor(80, 0, 80);  // púrpura - Rojo medio, Verde 0, Azul medio
  delay(1000);
  
  setColor(0, 255, 255);  // aqua/cian - Rojo 0, Verde+Azul máximo
  delay(1000);
}
 
void setColor(int red, int green, int blue)
{
  // COMPILACIÓN CONDICIONAL: Solo si COMMON_ANODE está definido
  #ifdef COMMON_ANODE
    // INVIERTE LOS VALORES para LED de Ánodo Común
    red = 255 - red;    // 255 se convierte en 0, 0 en 255
    green = 255 - green; // Lógica invertida
    blue = 255 - blue;
  #endif
  
  // ESCRIBE LAS INTENSIDADES EN LOS PINES PWM
  analogWrite(redPin, red);      // Controla intensidad Rojo (0-255)
  analogWrite(greenPin, green);  // Controla intensidad Verde (0-255)
  analogWrite(bluePin, blue);    // Controla intensidad Azul (0-255)
}

