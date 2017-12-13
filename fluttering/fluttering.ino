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
Wing rightWing = {12, 13, NULL, NULL, A2, A0,  NULL, false, true, 
                  FORWARD, BACKWARD, 43, 60, 60 };
Wing leftWing = {10, 11, NULL, NULL, A3, A1, NULL, false, true,
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

  pinMode(leftWing.sTrigPin, OUTPUT);
  pinMode(rightWing.sTrigPin, OUTPUT);

  pinMode(leftWing.sEchoPin, INPUT);
  pinMode(rightWing.sEchoPin, INPUT);
  
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

//rightWing = flap_1_wing(rightMotor, rightWing);
//rightWing = flex_1_wing(rightMotor, rightWing);
//leftWing = flap_1_wing(leftMotor, leftWing);
//rightWing = go_to_angle(rightMotor, rightWing);
//leftWing = go_to_angle(leftMotor, leftWing);

wings = flap_2_wings(wings);

// Serial.println(leftWing.goalVal);

}
