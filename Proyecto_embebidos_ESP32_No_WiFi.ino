#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc; // Declaramos un RTC DS3231
volatile int numPulsos = 0; //Cuenta la cantidad de pulsos por el caudalimetro
int sensor = 4;
int relay = 5;
#define led 2

//Funcion que imprime en pantalla la fecha y hora actual
void imprimeFecha(DateTime date)
{
  //numero de dia de la semana
  float numdia = date.dayOfTheWeek();
  if (numdia==0) Serial.print("Domingo");
  if (numdia==1) Serial.print("Lunes");
  if (numdia==2) Serial.print("Martes");
  if (numdia==3) Serial.print("Miercoles");
  if (numdia==4) Serial.print("Jueves");
  if (numdia==5) Serial.print("Viernes");
  if (numdia==6) Serial.print("Sabado");

  Serial.print(" ");
  Serial.print(date.day());
  Serial.print('/');
  Serial.print(date.month());
  Serial.print('/');
  Serial.print(date.year());
  Serial.print(" ");
  Serial.print(date.hour());
  Serial.print(':');
  Serial.print(date.minute());
  Serial.print(':');
  Serial.print(date.second());
  //Serial.println();
}

//Funcion de contar pulsos
void IRAM_ATTR cuentaPulsos()
{
  numPulsos++;
  if (numPulsos == 100)
  {
    digitalWrite(led, HIGH);
    digitalWrite(relay, HIGH);
    Serial.println("Limite superado");
  }
}

//Funcion para validar la hora
void validaHorario(DateTime date)
{
  if (date.second() == 0)
  {
    Serial.println("Sistema encerado");
    digitalWrite(led, LOW);
    digitalWrite(relay, LOW);
    numPulsos = 0;
  }
}


//INT MAIN-----------------------------------------
void setup () {
  Serial.begin(9600);
 
  // Comprobamos si tenemos el RTC conectado
  if (! rtc.begin())
  {
    Serial.println("No hay un módulo RTC");
    while (1);
  }
 
  // Ponemos en hora, solo la primera vez, luego comentar y volver a cargar.
  // Ponemos en hora con los valores de la fecha y la hora en que el sketch ha sido compilado.
  rtc.adjust(DateTime(__DATE__, __TIME__));

  attachInterrupt(digitalPinToInterrupt(sensor), cuentaPulsos, RISING); //Interrupcion para contar los pulsos
  pinMode(led,OUTPUT);
  pinMode(relay,OUTPUT);

}

//PROGRAMA PRINCIPAL------------------------------
void loop () {
  DateTime fecha = rtc.now(); //Guarda la fecha en una variable llamada fecha
  imprimeFecha(fecha); //Muestra en monitor la fecha y hora actual
  Serial.println(numPulsos);
  validaHorario(fecha); //Valida que sea la hora indicada para reiniciar el conteo de pulsos
  delay(1000);
 }
