/*********************
 *  TONOS Y MELODIAS CON PIEZO SPEAKER
 * *******************
 * Este programa permite realizar la simulacion de melodias con el
 * Piezo Speaker o altavoz.
 * 
 * La melodia reproducida se llama TAPION de la serie japonesa
 * DragonBall Z
 **********************/
int speakerPin = 5;     //Declaramos el PIN digital D9 para el Piezo Speaker
int longitud = 33;      //Numero de notas que contiene la melodia mas uno
char notas[] = "ccfagfg deffgfe ccfagfg deffedcd "; //Notas musicales. Los espacios son notas vacias
int golpes[] = { 1, 1, 4, 1, 1, 1, 4, 2, 1, 1, 4, 2, 1, 1, 4, 2, 1, 1, 4, 1, 1, 1, 4, 2, 1, 1, 4, 2, 1, 1, 1, 4, 2}; //Duracion de cada una de las notas musicales
int tiempo = 250;       //Tiempo de duracion para el toque de notas
void setup() {
  pinMode(speakerPin, OUTPUT);  //Configuramos el puerto digital 9 como salida
}

void loop() {
  for(int i=0;i<longitud;i++){
    if(notas[i]==' '){
      delay(golpes[i]*tiempo);  //Retardo de acuerdo a los golpes 
      }
      else{
        Tocar_Nota(notas[i],golpes[i]*tiempo); //Reproduce notas con su duracion
        }
        delay(tiempo/2);      //Pausa entre notas
    }
}

void Tocar_Tono(int tone, int duracion){ //Una funcion para la reproduccion de tonos
  for(long i =0;i<duracion*1000L;i += tone*2){
    digitalWrite(speakerPin,HIGH);      //Presenta el tono en alto
    delayMicroseconds(tone);            //Duracion del tono
    digitalWrite(speakerPin,LOW);       //Presenta el tono en bajo
    delayMicroseconds(tone);            //Duracion del tono
   }
  }

void Tocar_Nota(char nota, int duracion){ //Una funcion para la reproduccion de notas
  char nombres[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'c'};
  //Variable local para el nombre de notas musicales existentes
  int tonos[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};
  //Variable local para el ancho de pulso (us) de las notas

  for(int i=0;i<8;i++){//Reproduciomos los tonos correspondientes al nombre de las notas
    if(nombres[i] == nota){
      Tocar_Tono(tonos[i], duracion); //Llamamos a la funcion para la reproduccion de tonos, segun el tono y duracion 
      }
    }
  }
