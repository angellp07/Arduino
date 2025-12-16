/*PRESIONA EL BOTON Y SE ENCIENDE EL LED, PARA SELECCIONAR OTRO LED presionar reicinio (azul)*/


// DECLARACIÓN DE VARIABLES PARA LOS LEDS
int Redled=8;     // Asigna el pin 8 para controlar el LED Rojo
int Yellowled=7;  // Asigna el pin 7 para controlar el LED Amarillo
int Greenled=6;   // Asigna el pin 6 para controlar el LED Verde

// DECLARACIÓN DE VARIABLES PARA LOS BOTONES
int Key1=5;       // Asigna el pin 5 para leer el botón que activa el LED Rojo
int Key2=4;       // Asigna el pin 4 para leer el botón que activa el LED Amarillo
int Key3=3;       // Asigna el pin 3 para leer el botón que activa el LED Verde
int KeyRest=2;    // Asigna el pin 2 para leer el botón de RESET (apaga todos los LEDs)

// VARIABLES PARA ALMACENAR EL ESTADO DE LOS BOTONES
int Red;          // Variable que guardará el estado del botón Rojo (HIGH o LOW)
int Yellow;       // Variable que guardará el estado del botón Amarillo (HIGH o LOW)
int Green;        // Variable que guardará el estado del botón Verde (HIGH o LOW)

// FUNCIÓN setup(): SE EJECUTA UNA VEZ AL INICIAR
void setup()
{
  // CONFIGURACIÓN DE PINES COMO SALIDAS (LEDs)
  pinMode(Redled,OUTPUT);    // Pin 8 como salida para el LED Rojo
  pinMode(Yellowled,OUTPUT); // Pin 7 como salida para el LED Amarillo
  pinMode(Greenled,OUTPUT);  // Pin 6 como salida para el LED Verde
  
  // CONFIGURACIÓN DE PINES COMO ENTRADAS (BOTONES)
  pinMode(Key1,INPUT);       // Pin 5 como entrada para el botón Rojo
  pinMode(Key2,INPUT);       // Pin 4 como entrada para el botón Amarillo
  pinMode(Key3,INPUT);       // Pin 3 como entrada para el botón Verde
  pinMode(KeyRest,INPUT);    // Pin 2 como entrada para el botón de RESET
}

// FUNCIÓN loop(): SE REPITE INDEFINIDAMENTE DESPUÉS DE setup()
void loop()         // Lee constantemente el estado de los botones
{
  // LECTURA DEL ESTADO ACTUAL DE CADA BOTÓN
  Red=digitalRead(Key1);     // Lee si el botón Rojo está presionado (HIGH) o no (LOW)
  Yellow=digitalRead(Key2);  // Lee si el botón Amarillo está presionado (HIGH) o no (LOW)
  Green=digitalRead(Key3);   // Lee si el botón Verde está presionado (HIGH) o no (LOW)
  
  // VERIFICACIÓN Y LLAMADA A FUNCIONES SEGÚN BOTÓN PRESIONADO
  if(Red==HIGH)Red_YES();      // Si botón Rojo está presionado, ejecuta función Red_YES()
  if(Yellow==HIGH)Yellow_YES();// Si botón Amarillo está presionado, ejecuta función Yellow_YES()
  if(Green==HIGH)Green_YES();  // Si botón Verde está presionado, ejecuta función Green_YES()
}

// FUNCIÓN Red_YES(): ACTIVA EL LED ROJO HASTA QUE SE PRESIONE RESET
void Red_YES()  // Ejecuta este código mientras el LED Rojo está encendido
{
  // BUCLE WHILE: Se repite MIENTRAS el botón de reset NO esté presionado
  // digitalRead(KeyRest)==0 significa "botón de reset NO presionado"
  while(digitalRead(KeyRest)==0)
  {
    // ENCIENDE solo el LED Rojo, apaga los otros
    digitalWrite(Redled,HIGH);    // Enciende LED Rojo (5V en pin 8)
    digitalWrite(Greenled,LOW);   // Apaga LED Verde (0V en pin 6)
    digitalWrite(Yellowled,LOW);  // Apaga LED Amarillo (0V en pin 7)
  }
  // Cuando se sale del while (botón reset presionado), apaga todos los LEDs
  clear_led();
}

// FUNCIÓN Yellow_YES(): ACTIVA EL LED AMARILLO HASTA QUE SE PRESIONE RESET
void Yellow_YES()  // Ejecuta este código mientras el LED Amarillo está encendido
{
  // Mismo patrón que Red_YES() pero para LED Amarillo
  while(digitalRead(KeyRest)==0)
  {
    // ENCIENDE solo el LED Amarillo, apaga los otros
    digitalWrite(Redled,LOW);     // Apaga LED Rojo
    digitalWrite(Greenled,LOW);   // Apaga LED Verde
    digitalWrite(Yellowled,HIGH); // Enciende LED Amarillo
  }
  clear_led();
}

// FUNCIÓN Green_YES(): ACTIVA EL LED VERDE HASTA QUE SE PRESIONE RESET
void Green_YES()   // Ejecuta este código mientras el LED Verde está encendido
{
  // Mismo patrón que las anteriores pero para LED Verde
  while(digitalRead(KeyRest)==0)
  {
    // ENCIENDE solo el LED Verde, apaga los otros
    digitalWrite(Redled,LOW);     // Apaga LED Rojo
    digitalWrite(Greenled,HIGH);  // Enciende LED Verde
    digitalWrite(Yellowled,LOW);  // Apaga LED Amarillo
  }
  clear_led();
}

// FUNCIÓN clear_led(): APAGA TODOS LOS LEDs
void clear_led()   // Establece todos los pines de LEDs en LOW (0V)
{
  digitalWrite(Redled,LOW);    // Apaga LED Rojo
  digitalWrite(Greenled,LOW);  // Apaga LED Verde
  digitalWrite(Yellowled,LOW); // Apaga LED Amarillo
}