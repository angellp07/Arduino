int ledPin = 5;   // LED conectado al pin digital 5 (D5)

void setup() {
 		 // Nada en setup - pin 5 ya funciona como salida PWM
}

void loop() {
  // FADE IN: Aumentar brillo gradualmente
  /*Creamos un contador "fadevalue" que vale 0, si el contador es menor o igual a 255, por vuelta se le va añadiendo 5*/

  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue); 	 // Establece brillo
    delay(30);                       	// Espera para ver efecto
  }

  // FADE OUT: Disminuir brillo gradualmente  
  /*Creamos un contador "fadevalue" que vale 0, si el contador es menor o igual a 255, por vuelta se le va restando 5*/

  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);  // Establece brillo
    delay(30);                       // Espera para ver efecto
  }
}
