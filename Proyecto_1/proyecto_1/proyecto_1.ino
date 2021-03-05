//*******IMPORTANDO LIBRERÍAS*********
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 7
#define DATA_PIN 6
#define CLK_PIN 5
MD_Parola Matriz = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
//*******OBJETO DE TIPO LiquidCrystal(rs, enable, d4, d5,d6, d7)
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
//****************ARREGLOS DE LOS DISPLAY************************
byte DisplayC[10] = {B00000001, B01001111, B00010010, B00000110, B01001100, B00100100, B00100000, B00001111, B00000000, B00000100};
byte DisplayD[7] = {B00000001, B01001111, B00010010, B00000110, B01001100, B00100100, B00100000};
byte DisplayU[10] = {B00000010, B10011110, B00100100, B00001100, B10011000, B01001000, B01000000, B00011110, B00000000, B00001000};
//*****************VARIABLES**************************
int val; // variable de temperatura
String contra;
long ejecucion;


//*********VARIABLES KEYPAD*********
const byte FILAS = 4;
const byte COLUMNAS = 3;
char keys[FILAS][COLUMNAS] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
byte pinesFilas[FILAS] = {A4, A5, A6, A7};
byte pinesColumnas[COLUMNAS] = {A8, A9, A10};
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

void setup() {
  //****PINES PARA EL SENSOR DE TEMPERATURA
  pinMode(A0, INPUT); //Declaramos el pin A0 como Entrada
  pinMode(A1, OUTPUT); //Led verde
  pinMode(A2, OUTPUT); //Led amarillo
  pinMode(A3, OUTPUT); //Led rojo
  //*****PINES DEL KEYPAD*********
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
  pinMode(A10, OUTPUT);

  //*****SALIDAS DDR PARA LOS DISPLAY DE 7 SEGMENTOS
  DDRA = B11111111;
  DDRC = B11111111;
  DDRL = B11111111;
  //******VARIABLES KEYPAD********
  contra = "";
  ejecucion = 0;
  //Iniciando pantalla conectada 16x2
  lcd.begin(16, 2);
  //**** INICIALIZANDO OBJETO MATRIZ
  Matriz.begin();
  Matriz.setIntensity(15);
  Matriz.displayClear();
  Matriz.displayText("ACYE 1 G10", PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Temperatura();
  //Reloj();
  //Password();
  MatrizLed();
}

//****MÉTODO DEL RELOJ*****
void Reloj() {
  for (int i = 0; i < 10; i++) {
    PORTA = DisplayC[i];
    for (int k = 0; k < 6; k++) {
      PORTC = DisplayD[k];
      for (int j = 0; j < 10; j++) {
        PORTL = DisplayU[j];
        delay(1000);
      }
    }
  }
}
//*****MÉTODO DEL SENSOR DE TEMPERATURA*************
void Temperatura() {
  val = analogRead(A0);   //Lectura  del pin A0, Sensor de temperatura
  //analogRead nos devuelve un valor entre 0 a 1023
  //El sensor nos devuelve de 0 a 1.5V
  float mv = (val / 1024.0) * 5000; //miliVoltios
  float temperatura = mv / 10; //temperatura en grados celsius
  //1C = 10 mV
  if (temperatura < 37.0) {
    digitalWrite(A1, HIGH); //Se Enciende el led verde
    digitalWrite(A2, LOW); //Se apaga el led amarillo
    digitalWrite(A3, LOW); //Se apaga el led rojo
  } else if (temperatura > 36.0 && temperatura < 46.0) {
    digitalWrite(A1, LOW); //Se apaga el led verde
    digitalWrite(A2, HIGH); //Se Enciende el led amarillo
    digitalWrite(A3, LOW); //Se apaga el led rojo
  } else {
    digitalWrite(A1, LOW); //Se apaga el led verde
    digitalWrite(A2, LOW); //Se apaga el led amarillo
    digitalWrite(A3, HIGH); //Se Enciende el led rojo
  }
}
//********MÉTODO DEL DISPLAY Y CONTRASEÑA******
void Password() {
  lcd.home();
  lcd.print("INGRESE");
  lcd.setCursor(0, 1);
  lcd.print("SU CONTRASENA");
  ejecucion = millis();
  while (millis() - ejecucion <= 60000) {
    char presion = teclado.getKey();
    if (presion) {
      lcd.clear();
      lcd.setCursor(0, 1);
      if (presion == '#') {
        if (contra == "123456") {
          lcd.clear();
          lcd.home();
          lcd.print("BIENVENIDO");
          lcd.setCursor (0, 1);
          lcd.print("A CASA ^_^");
          contra = "";
        } else {
          lcd.clear();
          lcd.setCursor (0, 1);
          lcd.print("ERROR");
          contra = "";
          delay(1000);
        }
      } else {
        contra += presion;
      }
      lcd.print(contra);
    }
  }
}
//********************MÉTODO MATRIZ DE LED*********
void MatrizLed(){
  Matriz.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  int velocidad = 500;
  int slide_scroll_speed = map(velocidad, 1023,0,400,15);
  Matriz.setSpeed(slide_scroll_speed);
  if(Matriz.displayAnimate()){
    Matriz.displayReset();
  }
}
