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
// trigPin, echoPin, flexInputPin, potPin, currentpos, flexSensorVal, isWaiting, isOpening
// upDirection, downDirection, minHeight, maxHeight, goalVal
Wing rightWing = {12, 13, A2, A0, NULL, NULL, false, true, 
                  FORWARD, BACKWARD, 43, 60, 60 };
Wing leftWing = {10, 11, A3, A1, NULL, NULL, false, true,
                 BACKWARD, FORWARD, 34, 49, 49 };

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

  pinMode(leftWing.trigPin, OUTPUT);
  pinMode(rightWing.trigPin, OUTPUT);

  pinMode(leftWing.echoPin, INPUT);
  pinMode(rightWing.echoPin, INPUT);
  
}

 long ultrasonic_dist(Wing wing) {
  
  digitalWrite(wing.trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(wing.trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(wing.trigPin, LOW);
  long distance = pulseIn(wing.echoPin, HIGH, 200000);
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

//rightWing = flap_1_wing(rightMotor, rightWing);
//rightWing = flex_1_wing(rightMotor, rightWing);
//leftWing = flap_1_wing(leftMotor, leftWing);
//rightWing = go_to_angle(rightMotor, rightWing);
//leftWing = go_to_angle(leftMotor, leftWing);

wings = flap_2_wings(wings);

// Serial.println(leftWing.goalVal);

}
