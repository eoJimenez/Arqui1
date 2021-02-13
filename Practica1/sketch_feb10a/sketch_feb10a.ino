//Agregando librerias necesarias
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

//Definiendo pines a utilizar
//DIN (pin 11)
//CS (pin 10)
//CLK (pin 13)
//LEFT-RIGHT (pin 6)
//HIGH-LOW (pin 5)

//***********Parametros para el controlador************
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 2 //Número de matrices a utilizar
#define CS_PIN 10
#define DATA_PIN 11
#define CLK_PIN 13
//Creando objeto de tipo MD_Parola
MD_Parola matriz = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//Creando objeto max7219 (esto para el juego)
MD_MAX72XX matrizJuego1 = MD_MAX72XX(MD_MAX72XX::GENERIC_HW, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//Pines que seran utilizados como entradas para los botones
const int buttonPin5 = 5;//pin entrada velocidad del mensaje
const int buttonPin6 = 6;//pin entrada direccion mensaje






/////////////////JUEGO!!!
//1. Definición del struct que contenerá las coordenadas de cada puntito que conforma al snake.
typedef struct
{
  int fila;
  int columna;
}  coordenada_snake;
//2.El snake de fijo que no puede tener una longitud más grande que 128 leds.
coordenada_snake snake[128];
//3.Se declara una variable que será la encargada de manejar la longitud activa del snake.
int largo_snake = 1;//En un inicio solo existe la cabeza.
//4. Se declara una variable para la fila donde se encontrará la comida.
int fila_comida = 0;
//5. Se declara una variable para la columna donde se encontrará la comida.
int columna_comida = 0;
//6. Se declara una variable que en el void setup tomará un valor aleatorio de 0 o 1, esta servirá para determinar la coordenada inicial de la cabeza.
long coordenada_inicial;
//7. Se declara una variable que en el void setup tomará un valor aleatorio de 0 y 7 o de 8 y 15.
long fila_inicial;
//8. Se declara una variable estado, que es lo que dirá al loop que hacer.
int estado;
//9.Se declara contador de tiempo de ejecución
long ejec;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin6, INPUT);

  matrizJuego1.begin();

  matriz.begin(); //inicializando objeto tipo MD_Parola
  matriz.setIntensity(15); //intensidad de brillo valores entre 0 - 15
  matriz.displayClear(); //limpiando la matriz
  matriz.displayText("*TP1 - GRUPO 10 - SECCION B*", PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  randomSeed(analogRead(0));
  estado = 0;
}

void loop() {

  switch (estado) {
    case 0:
      //Se procede a iniciar las coordenadas de la cabeza en la posición randomica.
      coordenada_inicial = random(0, 2);
      if (coordenada_inicial == 0) {
        snake[0].columna = 7;
        fila_inicial = random(8, 16);
        snake[0].fila = fila_inicial;
      } else {
        snake[0].columna = 0;
        fila_inicial = random(0, 8);
        snake[0].fila = fila_inicial;
      }
      estado = 1;
      break;
    case 1:
      if (coordenada_inicial == 0) {
        estado = 2;
      } else {
        estado = 3;
      }
      break;
    case 2:
      ejec = millis();
      while (millis() - ejec > 500) {
        matrizJuego1.setPoint(snake[0].columna, snake[0].fila, true);
      }
      matrizJuego1.setPoint(snake[0].columna, snake[0].fila, false);
      //snake[0].columna ++;
      break;
    case 3:
      ejec = millis();
      while (millis() - ejec > 500) {
        matrizJuego1.setPoint(snake[0].columna, snake[0].fila, true);
      }
      matrizJuego1.setPoint(snake[0].columna, snake[0].fila, false);
      //snake[0].fila ++;
      break;
  }
  /*
    matrizJuego1.setPoint(snake[0].columna, snake[0].fila, true);
    delay(2000);
    matrizJuego1.setPoint(snake[0].columna, snake[0].fila, false);
    delay(2000);
  */

}
