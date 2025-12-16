// El pin 13 tiene un LED conectado en la mayoría de placas Arduino.
// Se le asigna un nombre:
int led = 13;  // Declara una variable llamada 'led' con valor 13
 
// La rutina setup (configuración) se ejecuta una vez al presionar reset:
void setup() {                
  // Inicializa el pin digital como salida.
  pinMode(led, OUTPUT);     // Configura el pin 13 como PIN DE SALIDA
}
 
// La rutina loop (bucle) se ejecuta una y otra vez para siempre:
void loop() {
  digitalWrite(led, HIGH);      // Enciende el LED (HIGH es el nivel de voltaje alto = 5V)
  delay(1000);                  // Espera un segundo (1000 milisegundos)
  digitalWrite(led, LOW);       // Apaga el LED poniendo el voltaje en BAJO (0V)
  delay(1000);                  // Espera un segundo (1000 milisegundos)
}

/*
EXPLICACIÓN DETALLADA:

1. int led = 13;
   - Crea una variable entera llamada 'led' que vale 13
   - El número 13 representa el PIN digital 13 de Arduino

2. void setup()
   - Se ejecuta UNA SOLA VEZ al empezar o reiniciar
   - Aquí se configuran los ajustes iniciales

3. pinMode(led, OUTPUT);
   - Configura el pin 13 como SALIDA
   - OUTPUT = puede enviar señales (como encender/apagar LED)
   - INPUT = puede recibir señales (como leer un botón)

4. void loop()
   - Se repite INFINITAMENTE después del setup()
   - Aquí va el programa principal

5. digitalWrite(led, HIGH);
   - Envía 5 voltios al pin 13
   - El LED se enciende

6. delay(1000);
   - Pausa el programa por 1000 milisegundos = 1 segundo
   - El LED permanece encendido

7. digitalWrite(led, LOW);
   - Envía 0 voltios al pin 13
   - El LED se apaga

8. delay(1000);
   - Otra pausa de 1 segundo
   - El LED permanece apagado

9. El ciclo se repite:
   ENCIENDE → Espera 1s → APAGA → Espera 1s → ENCIENDE...
   Resultado: LED parpadea cada segundo
*/

// Para modificar la velocidad de parpadeo:
// Cambia los números dentro de delay():
// delay(500);  = medio segundo
// delay(2000); = dos segundos
// delay(100);  = décima de segundo (parpadeo rápido)
