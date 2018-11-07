#include "MeMCore.h"

// Define time delay before the LED is ON
#define LED_RGBWait 200

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 200 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds 


MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

MeLightSensor lightsensorTOP(PORT_6);

MeUltrasonicSensor ultrasonicSensor(PORT_3);

MeRGBLed rgbled(PORT_7, 2);

MeBuzzer buzzer;

MeLineFollower linefollower_2(2);

int red = 0;
int green = 0;
int blue = 0;
int motorSpeed = 255;

////////////////////// * MAIN METHOD * ////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    setup_Color_Challenge();
}
void loop() {
	long testcolor = lightsensorTOP.read();
  if (lightsensorTOP.read() < 600) {
    turnOnOffRGBLed(); 
  }
  Serial.println(lightsensorTOP.read());
  move (1, 100);
  if(((linefollower_2.readSensors())==(0))){
    turnOnRedLed();
    turnOffLed();
  }else{
    if(((linefollower_2.readSensors())==(1))){
      turnOnGreenLed();
      turnOffLed();
    }else{
      if(((linefollower_2.readSensors())==(3))){
        turnOnBlueLed();
        turnOffLed();
      }else{
         move (1, 100); 
      }
    }
  }
}

////////////////////// * MOVE METHOD * ////////////////////////////////////////////

void move(int direction, int speed) {
  int leftSpeed = 0;
  int rightSpeed = 0;
  // 1 is move forward
  // 2 is move backward
  // 3 is rotate1
  // 4 is rotate2 
      if(direction == 1){
          leftSpeed = speed;
          rightSpeed = speed;
      }else if(direction == 2){
          leftSpeed = -speed;
          rightSpeed = -speed;
      }else if(direction == 3){
          leftSpeed = -speed;
          rightSpeed = speed;
      }else if(direction == 4){
          leftSpeed = speed;
          rightSpeed = -speed;
      }
      motor1.run(M1 == M1? -(leftSpeed) :(leftSpeed ));
      motor2.run(M2 == M1? -(rightSpeed):(rightSpeed));
}

////////////////////// * COLOR CHALLENGE * ////////////////////////////////////////////

//placeholders for colour detected
int red = 0;
int green = 0;
int blue = 0;

//floats to hold colour arrays
float colourArray[] = {0,0,0};
float whiteArray[] = {0,0,0};
float blackArray[] = {0,0,0};
float greyDiff[] = {0,0,0};

//3 rows, 5 columns
char colourStr[3][5] = {"R = ", "G = ", "B = "};

void setup_Color_Challenge(){
  //setup the outputs for the colour sensor
  turnOffLed();

  setBalance();  //calibration
  digitalWrite(LED, HIGH); //Check Indicator -- ON after Calibration
}

void loopColorChallenge(){
//turn on one colour at a time and LDR reads 5 times
  for(int c = 0;c<=2;c++){    
    Serial.print(colourStr[c]);

    //TODO: CHANGE THIS CODE TO RGD IN MOBOT
    digitalWrite(ledArray[c],HIGH); //turn ON the LED, red, green or blue, one colour at a time.
    delay(RGBWait);
    
    //get the average of 5 consecutive readings for the current colour and return an average 
    colourArray[c] = getAvgReading(5);
    
    //the average reading returned minus the lowest value divided by the maximum possible range, multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity (i.e. the colour LDR is exposed to)
    colourArray[c] = (colourArray[c] - blackArray[c])/(greyDiff[c])*255;
    digitalWrite(ledArray[c],LOW);  //turn off the current LED colour
    delay(RGBWait);
    Serial.println(int(colourArray[c])); //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code
  } 
}

void setBalance(){
//set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);           //delay for five seconds for getting sample ready
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
  }


int getAvgReading(int times){      
//find the average reading for the requested number of times of scanning LDR
  int reading;
  int total =0;
//take the reading as many times as requested and add them up
  for(int i = 0;i < times;i++){
     reading = analogRead(LDR);
     total = reading + total;
     delay(LDRWait);
  }
//calculate the average and return it
  return total/times;
}

void turnOnOffRGBLed() {
  turnOnRedLed();
  turnOffLed();
  turnOnGreenLed();
  turnOffLed();
  turnOnBlueLed();
  turnOffLed();
}

void turnOnRedLed() {
	rgbled.setColor(0, 150, 0, 0);
	rgbled.show();
	delay(LED_RGBWait);
}

void turnOnGreenLed() {
	rgbled.setColor(0, 0, 150, 0);
	rgbled.show();
	delay(LED_RGBWait);
}

void turnOnBlueLed() {
	rgbled.setColor(0, 0, 0, 150);
	rgbled.show();
	delay(LED_RGBWait);
}

void turnOffLed() {
  rgbled.setColor(0, 0, 0, 0);
  rgbled.show();
  delay(LED_RGBWait);

}
void moveForward() {
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
