#include "MeMCore.h"

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeLightSensor lightsensor(PORT_6);
MeBuzzer buzzer;

int red = 0;
int green = 0;
int blue = 0;
int motorSpeed = 255;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  long testcolor = lightsensor.read();
  Serial.println(testcolor);
  delay(100);
  /*for (int a = 0; a <= 10 ; a++){
  motor1.run(-motorSpeed * a / 10);
  motor2.run(+motorSpeed * a / 10);
  delay(100);
  }
  
  for (int a = 10; a >= 0 ; a--){
  motor1.run(-motorSpeed * a / 10);
  motor2.run(+motorSpeed * a / 10);
  delay(100);
  }
  for (int a = 0; a <= 10 ; a++){
  motor1.run(+motorSpeed * a / 10);
  motor2.run(-motorSpeed * a / 10);
  delay(100);
  }
  
  for (int a = 10; a >= 0 ; a--){
  motor1.run(+motorSpeed * a / 10);
  motor2.run(-motorSpeed * a / 10);
  delay(100);
  }*/
  
}