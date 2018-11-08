#include "MeMCore.h"

// define notes for playing music at the end
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// Define time delay before the LED is ON
#define LED_RGBWait 200

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 200 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds 

// Define port for IR
#define LEFT_IR A2
#define RIGHT_IR A3

// Motor speed constant
#define SPEED_SLOW 0.5
#define SPEED_FAST 0.8

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

MeLightSensor lightsensorTOP(PORT_6);

MePort input(PORT_3);

MeUltrasonicSensor ultrasonicSensor(PORT_1);

MeRGBLed rgbled(PORT_7, 2);

MeBuzzer buzzer;

MeLineFollower linefollower_2(2);

//playing victory music at the end
// notes in the melody:
int melody[] = { NOTE_G4,//5  
        NOTE_G4,//5
        NOTE_A4,//6
        NOTE_G4,//5
        NOTE_C5,//1.
        NOTE_B4,//7
        0,
        NOTE_G4,//5
        NOTE_G4,//5
        NOTE_A4,//6
        NOTE_G4,//5
        NOTE_D5,//2.
        NOTE_C5,//1.
        0,
        NOTE_G4,//5
        NOTE_G4,//5
        NOTE_G5,//5.
        NOTE_E5,//3.
        NOTE_C5,//1.
        NOTE_B4,//7
        NOTE_A4,//6
        0,
        NOTE_F5,//4.
        NOTE_F5,//4.
        NOTE_E5,//3.
        NOTE_C5,//1.
        NOTE_D5,//2.
        NOTE_C5,//1.
        0,
};

int noteDurations[] = { 8,8,4,4,4,4,
            4,
            8,8,4,4,4,4,
            4,
            8,8,4,4,4,4,2,
            8,
            8,8,4,4,4,2,
            4,
};

//placeholders for colour detected
int red = 0;
int green = 0;
int blue = 0;

//floats to hold colour arrays
float colourArray[] = { 0,0,0 };
float whiteArray[] = { 0,0,0 };
float blackArray[] = { 0,0,0 };
float greyDiff[] = { 0,0,0 };

//3 rows, 5 columns
char colourStr[3][5] = { "R = ", "G = ", "B = " };

// Motor speed
int motorSpeedMax = 255;

// IR sensor
int IR1 = 780;
int IR2 = 555;

int sum_left, sum_right;


////////////////////// * MAIN METHOD * ////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  setup_Color_Challenge();
}
void loop() {
  long testcolor = lightsensorTOP.read();
  //move(1, motorSpeedMax * SPEED_FAST, motorSpeedMax * SPEED_FAST);
  //if (isBlackLine == 1) {
    if (lightsensorTOP.read() < 600) {
      Serial.println("READ COLOR CHALLENGE: ");
      loopColorChallenge();
      stop();
    }
  //}
  Serial.println("--------The light sensor is: ");
  Serial.println(lightsensorTOP.read());

  // Always read the IR sensor
  IR1 = analogRead(LEFT_IR);
  IR2 = analogRead(RIGHT_IR);
  Serial.print("=================The left IR sensor is: ");
  Serial.println(IR1);
  Serial.print("=================The right IR sensor is: ");
  Serial.println(IR2);
}


////////////////////// * COLOR CHALLENGE CALIBRATION AND READING* ////////////////////////////////////////////


void setup_Color_Challenge() {
  //setup the outputs for the colour sensor
  turnOffLed();

  setBalance();  //calibration
}

void loopColorChallenge() {
  //turn on one colour at a time and LDR reads 5 times
  for (int c = 0; c <= 2; c++) {
    Serial.print(colourStr[c]);

    // Turn on LED Red, Green, Blue at a time
    if (c == 0) {
      turnOnRedLed();
    }
    else if (c == 1) {
      turnOnGreenLed();
    }
    else {
      turnOnBlueLed();
    }

    // get the average of 5 consecutive readings for the current colour and return an average 
    colourArray[c] = getAvgReading(5);

    // the average reading returned minus the lowest value divided by the maximum possible range,
    // multiplied by 255 will give a value between 0-255, representing the value for the current reflectivity 
    // (i.e. the colour LDR is exposed to)
    colourArray[c] = (colourArray[c] - blackArray[c]) / (greyDiff[c]) * 255;

    //turn off the current LED colour
    turnOffLed();

    delay(LED_RGBWait);

    Serial.println(int(colourArray[c])); //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code
  }
}

void setBalance() {
  //set white balance
  Serial.println("Put White Sample For Calibration ...");

  delay(5000);  //delay for five seconds for getting sample ready

  turnOffLed(); //Check Indicator OFF during Calibration

  //scan the white sample.
  //go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  for (int i = 0; i <= 2; i++) {
    // Turn on LED Red, Green, Blue at a time
    if (i == 0) {
      turnOnRedLed();
    }
    else if (i == 1) {
      turnOnGreenLed();
    }
    else {
      turnOnBlueLed();
    }

    whiteArray[i] = getAvgReading(5);         //scan 5 times and return the average, 

    //turn off the current LED colour
    turnOffLed();

    delay(LED_RGBWait);
  }

  //done scanning white, time for the black sample.

  //set black balance
  Serial.println("Put Black Sample For Calibration ...");

  delay(5000); //delay for five seconds for getting sample ready 

  //go through one colour at a time, set the minimum reading for red, green and blue to the black array
  for (int i = 0; i <= 2; i++) {
    // Turn on LED Red, Green, Blue at a time
    if (i == 0) {
      turnOnRedLed();
    }
    else if (i == 1) {
      turnOnGreenLed();
    }
    else {
      turnOnBlueLed();
    }

    blackArray[i] = getAvgReading(5);

    //turn off the current LED colour
    turnOffLed();

    delay(LED_RGBWait);
    //the differnce between the maximum and the minimum gives the range
    greyDiff[i] = whiteArray[i] - blackArray[i];
  }

  //delay another 5 seconds for getting ready colour objects
  Serial.println("Colour Sensor Is Ready.");
  delay(5000);
}


int getAvgReading(int times) {
  //find the average reading for the requested number of times of scanning LDR
  int reading;
  int total = 0;
  //take the reading as many times as requested and add them up
  for (int i = 0; i < times; i++) {
    reading = lightsensorTOP.read();
    total = reading + total;
    delay(LED_RGBWait);
  }
  //calculate the average and return it
  return total / times;
}

////////////////////// * MOVE METHOD * ////////////////////////////////////////////

void move(int direction, int speedLeft, int speedRight) {
  int leftSpeed = 0;
  int rightSpeed = 0;
  // 1 is move forward
  // 2 is move backward
  // 3 is rotate1
  // 4 is rotate2 
  if (direction == 1) {
    leftSpeed = speedLeft;
    rightSpeed = speedRight;
  }
  else if (direction == 2) {
    leftSpeed = -speedLeft;
    rightSpeed = -speedRight;
  }
  else if (direction == 3) {
    leftSpeed = -speedLeft;
    rightSpeed = speedRight;
  }
  else if (direction == 4) {
    leftSpeed = speedLeft;
    rightSpeed = -speedRight;
  }
  motor1.run(M1 == M1 ? -(leftSpeed) : (leftSpeed));
  motor2.run(M2 == M1 ? -(rightSpeed) : (rightSpeed));
}

void stop() {
  motor1.run(0);
  motor2.run(0);
}


////////////////////// * READ BLACK LINE * ////////////////////////////////////////////
int isBlackLine() {
  if (linefollower_2.readSensors() == S1_IN_S2_IN
    || linefollower_2.readSensors() == S1_IN_S2_OUT
    || linefollower_2.readSensors() == S1_OUT_S2_IN) {
    return 1;
  }
  return 0;
}


////////////////////// * TURN ON OFF RGB LED * ////////////////////////////////////////////

void turnOnOffRGBLed() {
  turnOnRedLed();
  turnOffLed();
  turnOnGreenLed();
  turnOffLed();
  turnOnBlueLed();
  turnOffLed();
}

void turnOnRedLed() {
  rgbled.setColor(0, 255, 0, 0);
  rgbled.show();
  delay(LED_RGBWait);
}

void turnOnGreenLed() {
  rgbled.setColor(0, 0, 255, 0);
  rgbled.show();
  delay(LED_RGBWait);
}

void turnOnBlueLed() {
  rgbled.setColor(0, 0, 0, 255);
  rgbled.show();
  delay(LED_RGBWait);
}

void turnOffLed() {
  rgbled.setColor(0, 0, 0, 0);
  rgbled.show();
  delay(LED_RGBWait);

}


////////////////////// * PLAY VICTORY MUSIC * ////////////////////////////////////////////
void play() {
  for (int thisNote = 0; thisNote < 29; thisNote++) {
    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    buzzer.tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.00;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    buzzer.noTone(8);
  }
}

