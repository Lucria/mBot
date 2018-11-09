#include "MeMCore.h"
#include <PID_v1.h>

// Notes Definition for victory music
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
#define LED_RGBWait 100

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 100 //in milliseconds 

// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds 

// Define port for IR
#define LEFT_IR A2
#define RIGHT_IR A3

// Motor speed constant
#define SPEED_SLOW 0.5
#define SPEED_FAST 0.8

//number of times scanned
#define TIMES 5
#define TIMES_CALIBRATION 20

//time for turn left or right
#define TIME_TURN_MAX 265.0
#define SPEED_MAX 255.0
#define DISTANCE_TWO_WHEELS 12.9
#define DISTANCE_ONE_GRID 27.0
#define TIME_ONE_GRID (DISTANCE_ONE_GRID/(DISTANCE_TWO_WHEELS * 3.1416/4)*TIME_TURN_MAX)

int speedRealLeft = SPEED_MAX;
int speedRealRight = SPEED_MAX;

// Takes care of the PID
double SetpointLeft, InputLeft, OutputLeft;
double SetpointRight, InputRight, OutputRight;

// Initialize PID objects
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft,0.5,0.2,0, DIRECT);
PID rightPID(&InputRight, &OutputRight, &SetpointRight,0.5,0.2,0, DIRECT);

// Initialize Motors
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

// Initialize LineFollower
MeLineFollower linefollower_2(2);

// Initialize buzzer
MeBuzzer buzzer;

void setup() {
  Serial.begin(9600);
  setupIRCalibrate();
}

void loop() {
  move(1, 100, 100);
  InputLeft = analogRead(LEFT_IR);
  InputRight = analogRead(RIGHT_IR);
  Serial.println(InputLeft);
  Serial.println(InputRight);
  leftPID.Compute();
  rightPID.Compute();
  speedRealRight = (OutputRight / 2.2) + 150;
  speedRealLeft = (OutputLeft / 2.2) + 150;
  if (isBlackLine() == 1) {
    delay(500);
    buzzer.tone(8,NOTE_G4, 4);
  }
}

/**
 * Function for calibrating intial IR values
 */
void setupIRCalibrate() {
  int i;
  // Calibrates the initial left and right distance of the mBot
  for (i = 0; i < 10; i++) {
    InputRight = analogRead(RIGHT_IR);
    InputLeft = analogRead(LEFT_IR);
    SetpointLeft += InputLeft/10;
    SetpointRight += InputRight/10;
    delay(100);
  }
  // turn PID on
  leftPID.SetMode(AUTOMATIC);
  rightPID.SetMode(AUTOMATIC);
}

/**
 * Movement functions
 */
void move(int direction, int speedLeft, int speedRight) {
  int leftSpeed = 0;
  int rightSpeed = 0;
  // 1 is move forward
  // 2 is move backward
  // 3 is turn left
  // 4 is turn right
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

void turnLeft(int speedLeft, int speedRight) {
  move(3, speedLeft, speedRight);
  delay((TIME_TURN_MAX * SPEED_MAX / (speedLeft / 2 + speedRight / 2)));
}
void turnRight(int speedLeft, int speedRight) {
  move(4, speedLeft, speedRight);
  delay((TIME_TURN_MAX * SPEED_MAX / (speedLeft / 2 + speedRight / 2)));
}
void turn180(int speedLeft, int speedRight) {
  for (int i = 0; i < 2; i++) {
    turnLeft(speedLeft, speedRight);
  }
}
void goOneGrid(int speedLeft, int speedRight) {
  move(1, speedLeft, speedRight);
  delay(TIME_ONE_GRID * SPEED_MAX / (speedLeft / 2 + speedRight / 2));
}
void turnULeft(int speedLeft, int speedRight) {
  turnLeft(speedLeft, speedRight);
  goOneGrid(speedLeft, speedRight);
  turnLeft(speedLeft, speedRight);
}
void turnURight(int speedLeft, int speedRight) {
  turnRight(speedLeft, speedRight);
  goOneGrid(speedLeft, speedRight);
  turnRight(speedLeft, speedRight);
}
void stop() {
  motor1.run(0);
  motor2.run(0);
}

/**
 * Black Line Reading function
 */
int isBlackLine() {
  if ((linefollower_2.readSensors() == S1_IN_S2_IN)
    || linefollower_2.readSensors() == S1_IN_S2_OUT
    || linefollower_2.readSensors() == S1_OUT_S2_IN) {
    stop();
    return 1;
  }
  return 0;
}

