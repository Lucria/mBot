#include "MeMCore.h"

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeLightSensor lightsensor(PORT_6);
MeBuzzer buzzer;
MeRGBLed rgbled(PORT_7);
MeUltrasonicSensor ultraSensor(PORT_3);

int red = 0;
int green = 0;
int blue = 0;
int motorSpeed = 255;

/*void setBalance(){
  //set white balance
  Serial.println("While LED For Calibration ...");
  rgbled.setColor(0, 255, 255, 255);
  rgbled.show();
  digitalWrite(LED,LOW); //Check Indicator OFF during Calibration
//scan the white sample.
//go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  for(int i = 0;i<=2;i++){
     digitalWrite(ledArray[i],HIGH);
     delay(RGBWait);
     whiteArray[i] = getAvgReading(5);         //scan 5 times and return the average, 
     digitalWrite(ledArray[i],LOW);
     delay(RGBWait);
  }
//done scanning white, time for the black sample.
//set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);     //delay for five seconds for getting sample ready 
//go through one colour at a time, set the minimum reading for red, green and blue to the black array
  for(int i = 0;i<=2;i++){
     digitalWrite(ledArray[i],HIGH);
     delay(RGBWait);
     blackArray[i] = getAvgReading(5);
     digitalWrite(ledArray[i],LOW);
     delay(RGBWait);
//the differnce between the maximum and the minimum gives the range
     greyDiff[i] = whiteArray[i] - blackArray[i];
  }

//delay another 5 seconds for getting ready colour objects
  Serial.println("Colour Sensor Is Ready.");
  delay(5000);
  }*/

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  /*int testcolor = lightsensor.read();
  Serial.println(testcolor);
  delay(100);
  for (int a = 0; a <= 20 ; a++){
  rgbled.setColor(0, 255*a/40, 255*a/30, 255*a/20);
  rgbled.show();
  motor1.run(-motorSpeed * a / 10);
  motor2.run(+motorSpeed * a / 10);
  distancesensor();
  delay(100);
  }
  for (int a = 20; a >= 0 ; a--){
  rgbled.setColor(0, 255, 0, 0);
  rgbled.show();
  motor1.run(-motorSpeed * a / 10);
  motor2.run(+motorSpeed * a / 10);
  distancesensor();
  delay(100);
  }
  leftturn();
  delay(500);
  rightturn();
  delay(500);*/
  int Distance = ultraSensor.distanceCm();
  if (Distance < 30){
    rgbled.setColor(0, 255,0,0);
    rgbled.show();
    backward();
    delay(200);
    leftturn();
    delay(315); //90 degrees turn around 315, 180 degrees around 580 to 600
    halt();
    delay(500);
  } else {
    rgbled.setColor(0,0,0,255);
    rgbled.show();
    forward();
  }
}

/**
 * Movement based functions
 */
void leftturn() {
  motor1.run(motorSpeed);
  motor2.run(motorSpeed);
}

void rightturn() {
  motor1.run(-motorSpeed);
  motor2.run(motorSpeed);
}

void forward() {
  motor1.run(-motorSpeed + 45);
  motor2.run(motorSpeed); // Differnece between 40 and 45
}

void backward() {
  motor1.run(motorSpeed - 45);
  motor2.run(-motorSpeed);
}
void halt() {
  motor1.stop();
  motor2.stop();
}


