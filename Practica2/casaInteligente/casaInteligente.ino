//IMPORTANDO LIBRERÍAS
#include <LiquidCrystal.h>
#include <Servo.h>

//Creando objeto tipo LiquidCrystal (rs,enable,d4,d5,d6,d7)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
//Creando objeto tipo Servo
//Servo servoOut; //servo salida
//int pos = 0;

//Sensor Ultrasonico HC-SR04
long duracion; //tiempo en que la onda viaja al objeto y regresa
//long duracion1; //tiempo en que la onda viaja al objeto y regresa
long distancia; //distancia del objeto
//long distancia1; //distancia de la puerta de salida
int trig = 9; // recibe un pulso para comenzar el ciclo de medición
int echo = 8; //devuelve un pulso continuo que dura el tiempo en que la onda tarda en ir y regresar del objeto
//int trig1 = 12; // recibe un pulso para comenzar el ciclo de medición
//int echo1 = 13; //devuelve un pulso continuo que dura el tiempo en que la onda tarda en ir y regresar del objeto

void setup() {
  //Inicializando servo
  //servoOut.attach(10); //salida pin 10
  //Iniciando pantalla conectada 16x2
  lcd.begin(16, 2);

  //Sensor Ultrasonico
  pinMode(trig, OUTPUT); //emisor
  pinMode(echo, INPUT); //receptor
  //Sensor Ultrasonico de salida
  //pinMode(trig1, OUTPUT); //emisor
  //pinMode(echo1, INPUT); //receptor
}

void loop() {
    //*********************Sensor entrada**********************//
    //disparo para activar el ultrasónico de entrada
    digitalWrite(trig, LOW);
    delayMicroseconds(4);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    //termina el disparo de activación

//  //disparo para activar el ultrasónico de salida
//  digitalWrite(trig1, LOW);
//  delayMicroseconds(4);
//  digitalWrite(trig1, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trig1, LOW);
//  //termina el disparo de activación

    duracion = pulseIn(echo, HIGH); //tiempo en que va y regresa la onda ultrasónica
    //calculo de la distancia
    distancia = duracion / 58.4; //distancia en cm
//  duracion1 = pulseIn(echo1, HIGH); //tiempo en que va y regresa la onda ultrasónica
//  //calculo de la distancia
//  distancia1 = duracion1 / 58.4; //distancia en cm

    //*********************Pantalla LCD***********************//
    if (distancia >= 151) {
      //Mover cursor a la primera posición de la pantalla (0,0)
      lcd.home();
      //Imprime el primer mensaje en la primera fila de la pantalla
      lcd.print("CASA INTELIGENTE");
      //Mueve el cursor a la segunda fila de la pantalla
      lcd.setCursor(6, 1);
      //Imprime el final del primer mensaje
      lcd.print("ACE1");
      //Retardo y se limpia la pantalla
      delay(2000);
      lcd.clear();
      //Mueve el cursor a la segunda fila de la pantalla
      lcd.setCursor(0, 1);
      //Imprime el primer mensaje en la primera fila de la pantalla
      lcd.print("ACE1-B-G10-S1");
      //Retardo y se limpia la pantalla
      delay(2000);
      lcd.clear();
    } else {
      //*********************Sensor Ultrasonico**************//
      //Si la distancia es menor a 150 cm, se cambia el mensaje
      //como el mensaje supera los 16 caracteres se imprime en las 2 filas
      lcd.home();
      lcd.print("INGRESE");
      lcd.setCursor (0, 1);
      lcd.print("SU CONTRASENA");
      delay(2000);
      lcd.clear();
  //*********************Validar Contraseña*************************//
    //********************Sistema de iluminación*********************//
    Iluminacion();
    //********************Sensor Ultrasonico de Salida******//
  
  }
}

///***************MÉTODO DE ILUMINACION Y SALIDA************///
void Iluminacion() {
  while (true) {//ESTA DENTRO DE UN WHILE(TRUE) PARA QUE SE EJECUTE HASTA QUE SE VALIDE LA CONDICIÓN DE SALIDA, AÑADIR CONDICIÓN DE SALIDA
    int ldr1 = analogRead(A0);
    int ldr2 = analogRead(A1);
    int ldr3 = analogRead(A2);
    int ldr4 = analogRead(A3);
    if (ldr1 >= 500 && ldr2 >= 500 && ldr3 >= 500 && ldr4 >= 500) {//todas encendidas
      for (int i = 1; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
    }else if (ldr1 < 500 && ldr2 >= 500 && ldr3 >= 500 && ldr4 >= 500){//1 apagada, 2,3,4 encendidas
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
      for (int i = 2; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
    }else if(ldr1 < 500 && ldr2 < 500 && ldr3 >= 500 && ldr4 >= 500){//1,2 apagadas 3,4 encendidas
      for (int i = 1; i <= 2; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
      for (int i = 3; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
    }else if(ldr1 < 500 && ldr2 < 500 && ldr3 < 500 && ldr4 >= 500){//4 encendida, demás apagadas
      for (int i = 1; i <= 3; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
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
    }else if (ldr1 < 500 && ldr2 < 500 && ldr3 >= 500 && ldr4 < 500){//1,2,4 apagadas, 3 encendida
      for (int i = 1; i <= 2; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
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
    }else if (ldr1 < 500 && ldr2 >= 500 && ldr3 < 500 && ldr4 < 500){//1,3,4 apagadas, 2 encendida
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
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
    }else if (ldr1 < 500 && ldr2 >= 500 && ldr3 < 500 && ldr4 >= 500){//1,3 apagadas, 2,4 encendida
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
    }else if (ldr1 < 500 && ldr2 >= 500 && ldr3 >= 500 && ldr4 < 500){//1,4 apagadas, 2,3 encendida
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Apagada");
      delay(1000);
      lcd.clear();
      for (int i = 2; i <= 3; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
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
    }else if (ldr1 >= 500 && ldr2 < 500 && ldr3 < 500 && ldr4 < 500){//1 encendida, demás apagadas
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      for (int i = 2; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
    }else if (ldr1 >= 500 && ldr2 < 500 && ldr3 < 500 && ldr4 >= 500){//1,4 encendida, 2,3 apagada
      lcd.home();
      lcd.print("HABITACION 1:");
      lcd.setCursor (0, 1);
      lcd.print("Luz Encendida");
      delay(1000);
      lcd.clear();
      for (int i = 2; i <= 3; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
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
    }else if (ldr1 >= 500 && ldr2 < 500 && ldr3 >= 500 && ldr4 < 500){//1,3 encendida, 2,4 apagada
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
    }else if (ldr1 >= 500 && ldr2 < 500 && ldr3 >= 500 && ldr4 >= 500){//1,3,4 encendidas, 2 apagada
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
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
    }else if(ldr1 >= 500 && ldr2 >= 500 && ldr3 < 500 && ldr4 < 500){//1,2 encendidas 3,4 apagadas
      for (int i = 1; i <= 2; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Encendida");
        delay(1000);
        lcd.clear();
      }
      for (int i = 3; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
    }else if (ldr1 >= 500 && ldr2 >= 500 && ldr3 >= 500 && ldr4 < 500){//1,2,3 encendida, 4 apagada
      for (int i = 1; i <= 3; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
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
    }else {//todas apagadas
      for (int i = 1; i <= 4; i++) {
        lcd.home();
        lcd.print("HABITACION");
        lcd.setCursor(11,0);
        lcd.print(i);
        lcd.setCursor(12,0);
        lcd.print(":");
        lcd.setCursor (0, 1);
        lcd.print("Luz Apagada");
        delay(1000);
        lcd.clear();
      }
    }
  }
}
