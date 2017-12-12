#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "fluttering.h"
#include "flap.h"

// define motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// define motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);

// interrupt pin
int interruptPin = 2; 

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

  //attachInterrupt(digitalPinToInterrupt(interruptPin), isr_fold_down, CHANGE);
  
}


 long ultrasonic_dist(Wing wing) {
  
  digitalWrite(wing.trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(wing.trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(wing.trigPin, LOW);
  long distance = pulseIn(wing.echoPin, HIGH, 200000);
  Serial.println(distance);
  if (distance > 0) {
    return distance;
  } 
  return 10000;
 }

Wing set_goalval(Wing wing, int goal) {
  wing.goalVal = goal;
  return wing;
}

/*
 * Flap em both at once
 */
Pair flap_2_wings(Pair wings) {
  // unpack values
  Wing lWing = wings.wingLeft;
  Wing rWing = wings.wingRight;
  Adafruit_DCMotor *lMotor = wings.motorLeft;
  Adafruit_DCMotor *rMotor = wings.motorRight;

  long rUltraVal = ultrasonic_dist(rWing);
  long lUltraVal = ultrasonic_dist(lWing);

  rUltraVal = ultrasonic_dist(rWing);
  lUltraVal = ultrasonic_dist(lWing);

  int rPotVal = analogRead(rWing.potPin);
  int rFlexVal = analogRead(rWing.flexInputPin);

  int lPotVal = analogRead(lWing.potPin);
  int lFlexVal = analogRead(lWing.flexInputPin);

  Serial.println(rPotVal);
  Serial.println(lPotVal);

  // if youre too close go down
  if ( (rUltraVal < 1000) or (lUltraVal < 1000) ) {
    Serial.println("retreat!");
    lWing.isOpening = false;
    rWing.isOpening = false;
    lMotor->run(RELEASE);
    rMotor->run(RELEASE);
    delay(50);
  }
  
  if (!lWing.isWaiting) {
        // left wing
    if (lWing.isOpening) {
      if (lPotVal >= lWing.maxHeight) {
        lMotor->run(RELEASE);
        lWing.isWaiting = true;
        lWing.isOpening = false;
      } else {
        lMotor->run(lWing.upDirection);
      }
      } else {
        if (lPotVal <= lWing.minHeight) {
          lWing.isWaiting = true;
          lWing.isOpening = true;
          lMotor->run(RELEASE);
      } else {
        lMotor->run(lWing.downDirection);
      }
    }
  } 
  if (!rWing.isWaiting) {
    // right wing
    if (rWing.isOpening) {
      if (rPotVal >= rWing.maxHeight) {
        rMotor->run(RELEASE);
        rWing.isWaiting = true;
        rWing.isOpening = false;
      } else {
        rMotor->run(rWing.upDirection);
      }
      } else {
        if (rPotVal <= rWing.minHeight) {
          rWing.isWaiting = true;
          rWing.isOpening = true;
          rMotor->run(RELEASE);
      } else {
        rMotor->run(rWing.downDirection);
      }
    }
  } 
  if (lWing.isWaiting and rWing.isWaiting) {
    // if they're both waiting, neither is waiting!
    lWing.isWaiting = false;
    rWing.isWaiting = false;
    
  }
  Pair newWings = {lWing, rWing, lMotor, rMotor};
  delay(50);
  return newWings;
}

/*
 * Move both to the flex sensor value
 */
 Pair flex_follow_2_wings(Pair wings) {
  // unpack
  Wing lWing = wings.wingLeft;
  Wing rWing = wings.wingRight;
  Adafruit_DCMotor *lMotor = wings.motorLeft;
  Adafruit_DCMotor *rMotor = wings.motorRight;

  // get the ultrasonic sensor values
  long rUltraVal = ultrasonic_dist(rWing);
  long lUltraVal = ultrasonic_dist(lWing);
  rUltraVal = ultrasonic_dist(rWing);
  lUltraVal = ultrasonic_dist(lWing);

  // get the right potentiometer and flex values
  int rPotVal = analogRead(rWing.potPin);
  int rFlexVal = analogRead(rWing.flexInputPin);

  // get the left potentiometer and flex values
  int lPotVal = analogRead(lWing.potPin);
  int lFlexVal = analogRead(lWing.flexInputPin);

  Serial.println(rPotVal);
  Serial.println(lPotVal);

  // if youre too close go down
  if ( (rUltraVal < 1000) or (lUltraVal < 1000) ) {
    Serial.println("retreat!");
    lWing.isOpening = false;
    rWing.isOpening = false;
    lMotor->run(RELEASE);
    rMotor->run(RELEASE);
    delay(50);
  }

  
  
 }

/*
 * interrupt service routine for putting the wings back down immediately.
 */
void isr_fold_down() {
//  moveMotor(0, rightMotor, rightWing);
//  moveMotor(0, leftMotor, leftWing);
}


void loop() {

// rightWing = flap(rightMotor, rightWing);
//rightWing = flex_follow(rightMotor, rightWing);
// leftWing = flap(leftMotor, leftWing);
// rightWing = go_to_angle(rightMotor, rightWing);
// leftWing = go_to_angle(leftMotor, leftWing);

wings = flap_2_wings(wings);

// Serial.println(leftWing.goalVal);

}
