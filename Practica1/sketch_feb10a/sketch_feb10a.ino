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
//Pines que seran utilizados como entradas para los botones
const int buttonPin5 = 5;//pin entrada velocidad del mensaje
const int buttonPin6 = 6;//pin entrada direccion mensaje

void setup() {
  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin6, INPUT);
  matriz.begin(); //inicializando objeto tipo MD_Parola
  matriz.setIntensity(15); //intensidad de brillo valores entre 0 - 15
  matriz.displayClear(); //limpiando la matriz
  matriz.displayText("*TP1 - GRUPO 10 - SECCION B*", PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  //if que controla si va de izquierda a derecha o viceversa
  if(digitalRead(buttonPin6) == HIGH){
    matriz.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    //if que controla le velocidad del mensaje
    if(digitalRead(buttonPin5) == HIGH){
      int velocidad = 500;
      int slide_scroll = map(velocidad, 1023,0,400,15);
      matriz.setSpeed(slide_scroll);
    }else{
      int velocidad = 200;
      int slide_scroll = map(velocidad, 1023,0,400,15);
      matriz.setSpeed(slide_scroll);
    }
  }else{
    matriz.setTextEffect(PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
    //if que controla le velocidad del mensaje
    if(digitalRead(buttonPin5) == HIGH){
      int velocidad = 500;
      int slide_scroll = map(velocidad, 1023,0,400,15);
      matriz.setSpeed(slide_scroll);
    }else{
      int velocidad = 200;
      int slide_scroll = map(velocidad, 1023,0,400,15);
      matriz.setSpeed(slide_scroll);
    }
  }
  if(matriz.displayAnimate()){
    matriz.displayReset();
  }
}
