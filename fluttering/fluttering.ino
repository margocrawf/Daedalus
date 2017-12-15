#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "utility/Adafruit_MS_PWMServoDriver.h"

#include "fluttering.h"
#include "flap_1_wing.h"
#include "flex_1_wing.h"
#include "go_to_angle.h"
#include "flap_2_wings.h"

// define motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// define motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);

// delay time for flutters
int delayTime = 20;

// MIN RIGHT: 38, MAX RIGHT: 60
// MIN LEFT: 28 MAX LEFT: 50
// defining wing structs, in the following order:
// sTrigPin, sEchoPin, fTrigPin, fEchoPin, flexInputPin, potPin, flexSensorVal, isWaiting, isOpening
// upDirection, downDirection, minHeight, maxHeight, goalVal
Wing rightWing = {6, 7, 8, 9, A2, A3,  NULL, false, true, 
                  FORWARD, BACKWARD, 41, 60, 50 };
Wing leftWing = {12, 13, 10, 11, A1, A0, NULL, false, true,
                 BACKWARD, FORWARD, 31, 50, 39 };

Pair wings = {leftWing, rightWing, leftMotor, rightMotor};

/*
 * Starting up things.
 */
void setup() {
  // start the serial
  Serial.begin(9600);

  AFMS.begin();  // create with the default frequency 1.6KHz

  rightMotor->setSpeed(100);
  rightMotor->run(FORWARD);
  rightMotor->run(RELEASE);

  leftMotor->setSpeed(100);
  leftMotor->run(FORWARD);
  leftMotor->run(RELEASE);

//  pinMode(leftWing.sTrigPin, OUTPUT);
//  pinMode(rightWing.sTrigPin, OUTPUT);
//  pinMode(leftWing.sEchoPin, INPUT);
//  pinMode(rightWing.sEchoPin, INPUT);
//
//  pinMode(leftWing.fTrigPin, OUTPUT);
//  pinMode(rightWing.fTrigPin, OUTPUT);
//  pinMode(leftWing.fEchoPin, INPUT);
//  pinMode(rightWing.fEchoPin, INPUT);
  
}

/*
 * Takes a wing and which sensor to look at, returns ultrasonic distance (time)
 */
 long ultrasonic_dist(Wing wing, String sensor="front") {
    int trigPin;
    int echoPin;
    
    if (sensor == "side") {
      trigPin = wing.sTrigPin;
      echoPin = wing.sEchoPin;
    } else {
      trigPin = wing.fTrigPin;
      echoPin = wing.fEchoPin;
    }
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    long distance = pulseIn(echoPin, HIGH, 200000);
  //  Serial.println(distance);
    if (distance > 0) {
      return distance;
    } 
    return 10000;
 }

Wing set_goalval(Wing wing, int goal) {
  wing.goalVal = goal;
  return wing;
}


void loop() {

leftWing = go_to_angle(leftMotor, leftWing);

}
