//IMPORTANDO LIBRERÍAS
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Keypad.h>

//Creando objeto tipo LiquidCrystal (rs,enable,d4,d5,d6,d7)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
//Creando objeto tipo Servo
//Servo servoOut; //servo salida
//int pos = 0;
Servo servoEntrada;
Servo servoSalida;

//Sensor Ultrasonico HC-SR04
long duracion; //tiempo en que la onda viaja al objeto y regresa
//long duracion1; //tiempo en que la onda viaja al objeto y regresa
long distancia; //distancia del objeto
//long distancia1; //distancia de la puerta de salida
int trig = 9; // recibe un pulso para comenzar el ciclo de medición
int echo = 8; //devuelve un pulso continuo que dura el tiempo en que la onda tarda en ir y regresar del objeto
int trig1 = 12; // recibe un pulso para comenzar el ciclo de medición
int echo1 = 13; //devuelve un pulso continuo que dura el tiempo en que la onda tarda en ir y regresar del objeto

const byte FILAS = 4;
const byte COLUMNAS = 3;
char keys[FILAS][COLUMNAS] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
byte pinesFilas[FILAS] = {39, 33, 29, 23};
byte pinesColumnas[COLUMNAS] = {53, 49, 45};

int contador;

boolean estadoLed = false;

byte ledAlarma = 22;

byte Buzz = 51;

byte pinServoEntrada = 10;

byte pinServoSalida = 11;

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

long ejecucion;
int estado;
String contra;
void setup() {
  //Inicializando servo
  //servoOut.attach(10); //salida pin 10
  //Iniciando pantalla conectada 16x2
  lcd.begin(16, 2);
  //Sensor Ultrasonico
  pinMode(trig, OUTPUT); //emisor
  pinMode(echo, INPUT); //receptor

  pinMode(trig1, OUTPUT); //emisor
  pinMode(echo1, INPUT); //receptor
  //Sensor Ultrasonico de salida
  //pinMode(trig1, OUTPUT); //emisor
  //pinMode(echo1, INPUT); //receptor
  ejecucion = 0;
  estado = 0;
  contra = "";
  contador = 0;

  pinMode(ledAlarma, OUTPUT);
  pinMode(Buzz, OUTPUT);
  servoEntrada.attach(pinServoEntrada, 1000, 2000);
  servoEntrada.write(0);
  servoSalida.attach(pinServoSalida, 1000, 2000);
  servoSalida.write(0);
  Serial.begin(9600);
}

void loop() {
  //*********************Sensor entrada**********************//
  //disparo para activar el ultrasónico de entrada
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duracion = pulseIn(echo, HIGH); //tiempo en que va y regresa la onda ultrasónica
  //calculo de la distancia
  distancia = duracion / 58.4; //distancia en cm
  if (distancia >= 151) {
    estado = 0;
  } else {
    estado = 1;
  }
  switch (estado) {
    case 0:
      //Mover cursor a la primera posición de la pantalla (0,0)
      lcd.home();
      //Imprime el primer mensaje en la primera fila de la pantalla
      lcd.print("CASA INTELIGENTE");
      //Mueve el cursor a la segunda fila de la pantalla
      lcd.setCursor(6, 1);
      //Imprime el final del primer mensaje
      lcd.print("ACE1");
      //Retardo y se limpia la pantalla
      ejecucion = millis();
      while (millis() - ejecucion <= 2000) {
        //disparo para activar el ultrasónico de entrada
        digitalWrite(trig, LOW);
        delayMicroseconds(4);
        digitalWrite(trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig, LOW);
        duracion = pulseIn(echo, HIGH); //tiempo en que va y regresa la onda ultrasónica
        //calculo de la distancia
        distancia = duracion / 58.4; //distancia en cm
        if ( distancia >= 151) {
          estado = 0;
        } else {
          estado = 1;
          break;
        }
      }
      lcd.clear();
      if (estado == 1) {
        break;
      } else {
        ejecucion = millis();
        while (millis() - ejecucion <= 2000) {
          //Mueve el cursor a la segunda fila de la pantalla
          lcd.setCursor(0, 1);
          //Imprime el primer mensaje en la primera fila de la pantalla
          lcd.print("ACE1-B-G10-S1");
          //disparo para activar el ultrasónico de entrada
          digitalWrite(trig, LOW);
          delayMicroseconds(4);
          digitalWrite(trig, HIGH);
          delayMicroseconds(10);
          digitalWrite(trig, LOW);
          duracion = pulseIn(echo, HIGH); //tiempo en que va y regresa la onda ultrasónica
          //calculo de la distancia
          distancia = duracion / 58.4; //distancia en cm
          if ( distancia >= 151) {
            estado = 0;
          } else {
            estado = 1;
            break;
          }
        }
        lcd.clear();
        break;
      }
    case 1:
      //*********************Sensor Ultrasonico**************//
      //Si la distancia es menor a 150 cm, se cambia el mensaje
      //como el mensaje supera los 16 caracteres se imprime en las 2 filas
      lcd.home();
      lcd.print("INGRESE");
      lcd.setCursor (0, 1);
      lcd.print("SU CONTRASENA");
      ejecucion = millis();
      while (millis() - ejecucion <= 60000) {
        char presion = teclado.getKey();
        if (presion) {
          lcd.clear();
          lcd.setCursor(0, 1);
          if (presion == '#') {
            break;
          } else {
            contra += presion;
          }
          lcd.print(contra);
        }
      }
      if (contra == "202110") {
        lcd.clear();
        lcd.home();
        lcd.print("BIENVENIDO");
        lcd.setCursor (0, 1);
        lcd.print("A CASA ^_^");
        contra = "";
        contador = 0;
        servoEntrada.write(90);
        delay(3000);
        servoEntrada.write(0);
        Iluminacion();
      } else if (contador < 2) {
        contador++;
        lcd.clear();
        lcd.home();
        lcd.print("ERROR EN");
        lcd.setCursor (0, 1);
        lcd.print("CONTRASENA");
        contra = "";
        delay(1000);
      } else {
        lcd.clear();
        lcd.home();
        lcd.print("ACCESO");
        lcd.setCursor (0, 1);
        lcd.print("DENEGADO");
        ejecucion = millis();
        while (millis() - ejecucion <= 10000) {
          digitalWrite(Buzz, HIGH);
          alarma();
        }
        digitalWrite(Buzz, LOW);
        contador = 0;
        estado = 0;
        contra = "";
      }
      break;
  }

}


void alarma() {
  digitalWrite(ledAlarma, estadoLed);
  estadoLed = !estadoLed;
  delay(500);
}


///***************MÉTODO DE ILUMINACION Y SALIDA************///
void Iluminacion() {
  while (salio()) {//ESTA DENTRO DE UN WHILE(TRUE) PARA QUE SE EJECUTE HASTA QUE SE VALIDE LA CONDICIÓN DE SALIDA, AÑADIR CONDICIÓN DE SALIDA
    int ldr1 = analogRead(A0);
    int ldr2 = analogRead(A1);
    int ldr3 = analogRead(A2);
    int ldr4 = analogRead(A3);
    if (ldr1 >= 500 && ldr2 >= 500 && ldr3 >= 500 && ldr4 >= 500) {//todas encendidas
      for (int i = 1; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
    } else if (ldr1 < 500 && ldr2 >= 500 && ldr3 >= 500 && ldr4 >= 500) { //1 apagada, 2,3,4 encendidas
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
      for (int i = 2; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
    } else if (ldr1 < 500 && ldr2 < 500 && ldr3 >= 500 && ldr4 >= 500) { //1,2 apagadas 3,4 encendidas
      for (int i = 1; i <= 2; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
      for (int i = 3; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
    } else if (ldr1 < 500 && ldr2 < 500 && ldr3 < 500 && ldr4 >= 500) { //4 encendida, demás apagadas
      for (int i = 1; i <= 3; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
      lcd.home();
      lcd.print("HABITACION 4:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
    } else if (ldr1 < 500 && ldr2 < 500 && ldr3 >= 500 && ldr4 < 500) { //1,2,4 apagadas, 3 encendida
      for (int i = 1; i <= 2; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
      lcd.home();
      lcd.print("HABITACION 3:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("HABITACION 4:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
    } else if (ldr1 < 500 && ldr2 >= 500 && ldr3 < 500 && ldr4 < 500) { //1,3,4 apagadas, 2 encendida
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("HABITACION 2:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      for (int i = 3; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
    } else if (ldr1 < 500 && ldr2 >= 500 && ldr3 < 500 && ldr4 >= 500) { //1,3 apagadas, 2,4 encendida
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("HABITACION 2:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("HABITACION 3:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("HABITACION 4:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
    } else if (ldr1 < 500 && ldr2 >= 500 && ldr3 >= 500 && ldr4 < 500) { //1,4 apagadas, 2,3 encendida
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
      for (int i = 2; i <= 3; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
      lcd.home();
      lcd.print("HABITACION 4:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
    } else if (ldr1 >= 500 && ldr2 < 500 && ldr3 < 500 && ldr4 < 500) { //1 encendida, demás apagadas
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      for (int i = 2; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
    } else if (ldr1 >= 500 && ldr2 < 500 && ldr3 < 500 && ldr4 >= 500) { //1,4 encendida, 2,3 apagada
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      for (int i = 2; i <= 3; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
      lcd.home();
      lcd.print("HABITACION 4:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
    } else if (ldr1 >= 500 && ldr2 < 500 && ldr3 >= 500 && ldr4 < 500) { //1,3 encendida, 2,4 apagada
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("HABITACION 2:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("HABITACION 3:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("HABITACION 4:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
    } else if (ldr1 >= 500 && ldr2 < 500 && ldr3 >= 500 && ldr4 >= 500) { //1,3,4 encendidas, 2 apagada
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      lcd.home();
      lcd.print("HABITACION 2:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
      for (int i = 3; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
    } else if (ldr1 >= 500 && ldr2 >= 500 && ldr3 < 500 && ldr4 < 500) { //1,2 encendidas 3,4 apagadas
      for (int i = 1; i <= 2; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
      for (int i = 3; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
    } else if (ldr1 >= 500 && ldr2 >= 500 && ldr3 >= 500 && ldr4 < 500) { //1,2,3 encendida, 4 apagada
      for (int i = 1; i <= 3; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
      lcd.home();
      lcd.print("HABITACION 4:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
    } else {//todas apagadas
      for (int i = 1; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11, 0);
        lcd.print(i);
        lcd.setCursor(12, 0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
    }
  }
  servoSalida.write(90);
  delay(3000);
  servoSalida.write(0);
  estado = 0;
}


boolean salio() {
  //disparo para activar el ultrasónico de entrada
  digitalWrite(trig1, LOW);
  delayMicroseconds(4);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  duracion = pulseIn(echo1, HIGH); //tiempo en que va y regresa la onda ultrasónica
  //calculo de la distancia
  distancia = duracion / 58.4; //distancia en cm
  return distancia >= 151;
}
