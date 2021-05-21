#include <Servo.h>

//servos
Servo door;
Servo panel;

//pines
const int ledPin = 5;
const int panelPin = 6;
const int doorPin = 7;
const int closed = 7;
const int buttonPin = 8;
const int presPin = 9;
const int lPin = A0;
const int cPin = A1;
const int rPin = A2;

//angulos limite del servo para paneles
const int maxAngle = 150;
const int minAngle = 0;

//tiempo de apertura de la puerta en milisegundos
const unsigned int openTime = 5000;

//datos
int lVal,cVal,rVal;
char* btnStatus;
char* presStatus;
int push;
unsigned long toim;
unsigned long pushtmr;
double avg;
double avgR;
double avgL;
int servoPos = 150/2;


void setup(){
  door.attach(doorPin);
  door.write(closed);
  panel.attach(panelPin);
  panel.write(servoPos);
  
  pinMode(ledPin, OUTPUT);
  
  pinMode(presPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(lPin, INPUT);
  pinMode(cPin, INPUT);
  pinMode(rPin, INPUT);
  
  Serial.begin(9600);
  Serial.println("| LDR1 (lm)  | LDR2 (lm) | LDR3 (lm) | PRESENCE  | DOOR      |");
}

void loop(){
  
  //lectura de pines de entrada
  toim = millis();
  push = digitalRead(buttonPin);
  lVal = analogRead(lPin)-10;
  cVal = analogRead(cPin);
  rVal = analogRead(rPin);
  
  //promedios de luminosidad
  avg = (lVal + cVal + rVal) / 3.00;
  avgR = (cVal + rVal) / 2.00;
  avgL = (cVal + lVal) / 2.00;
  
  //control del servo de paneles
  if(abs(avgR-avgL)>2){
    if(avgR > avgL){
      servoPos -= 5;
      if(servoPos <= minAngle){
        servoPos=minAngle;
      }
    }
    if(avgL > avgR){
      servoPos +=5;
      if(servoPos >= maxAngle){
        servoPos=maxAngle;
      }
    }
  }
  panel.write(servoPos);
  
  //control servo puerta
  if(push == HIGH){
    btnStatus = "open ";
    door.write(closed+97);
    pushtmr = toim;
  }else{
    btnStatus = "close";
  }
  if(pushtmr + openTime <= toim){
    door.write(closed);
    pushtmr = 0;
  }
  
  //control iluminación
  if(digitalRead(presPin) == 1){
    presStatus = "detected";
    digitalWrite(ledPin, HIGH);
  }else{
    presStatus = "not     ";
    digitalWrite(ledPin, LOW);
  }

  //log
  Serial.print("| ");
  Serial.print(avgL);
  Serial.print("         ");
  Serial.print("| ");
  Serial.print(avg);
  Serial.print("        ");
  Serial.print("| ");
  Serial.print(avgR);
  Serial.print("        ");
  Serial.print("| ");
  Serial.print(presStatus);
  Serial.print("  ");
  Serial.print("| ");
  Serial.print(btnStatus);
  Serial.println("      |");
  delay(20);
}
