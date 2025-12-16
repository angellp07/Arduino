// Código para controlar 6 LEDs en secuencia

int BASE = 2;               // Primer pin donde comienzan los LEDs (D2)
int NUM = 6;                // Número total de LEDs (6 LEDs)

void setup()
{
  // Configura todos los pines de los LEDs como SALIDAS (for = bucle)
  /*int en el bucle pasa hacer "i", mientras "i" sea menor a base+num = 8, suma uno a "i" = i++*/
  for (int i = BASE; i < BASE + NUM; i++)
  {
    pinMode(i, OUTPUT);      // Configura el pin 'i' como puerto de salida
  }
}

void loop()
{
  // SECUENCIA 1: Apagar todos los LEDs uno por uno
  for (int i = BASE; i < BASE + NUM; i++)
  {
    digitalWrite(i, LOW);      // Apaga el LED en el pin 'i'
    delay(200);                // Espera 200 milisegundos (0.2 segundos)
  }
  
  // SECUENCIA 2: Encender todos los LEDs uno por uno
  for (int i = BASE; i < BASE + NUM; i++)
  {
    digitalWrite(i, HIGH);     // Enciende el LED en el pin 'i'
    delay(200);                // Espera 200 milisegundos (0.2 segundos)
  }
}