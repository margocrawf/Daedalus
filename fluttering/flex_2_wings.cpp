#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "Arduino.h"
#include "fluttering.h"
#include "utility/Adafruit_MS_PWMServoDriver.h"

/*
 * Move both to the flex sensor value
 */
 Pair flex_2_wings(Pair wings) {
  // unpack values
  Wing lWing = wings.wingLeft;
  Wing rWing = wings.wingRight;
  Adafruit_DCMotor *lMotor = wings.motorLeft;
  Adafruit_DCMotor *rMotor = wings.motorRight;

  // get the ultrasonic sensor values
  long rfUltraVal = ultrasonic_dist(rWing, "front");
  long lfUltraVal = ultrasonic_dist(lWing, "front");
  long rsUltraVal = ultrasonic_dist(rWing, "side");
  long lsUltraVal = ultrasonic_dist(lWing, "side");

  // get the right potentiometer and flex values
  int rPotVal = analogRead(rWing.potPin);
  int rFlexVal = analogRead(rWing.flexInputPin);

  // get the left potentiometer and flex values
  int lPotVal = analogRead(lWing.potPin);
  int lFlexVal = analogRead(lWing.flexInputPin);

  Serial.println(rPotVal);
  Serial.println(lPotVal);


  // if youre too close go down
  if ( (rsUltraVal < 1000) or (lsUltraVal < 1000) or (rfUltraVal < 1000) or (lfUltraVal < 1000) ) {
    Serial.println("retreat!");
    lMotor->run(RELEASE);
    rMotor->run(RELEASE);
    delay(50);
    return wings;
    
  }

  int lFlexPot = constrain(map(-lFlexVal, -lWing.flexMax, -lWing.flexMin, lWing.minHeight, lWing.maxHeight), lWing.minHeight, lWing.maxHeight);
  int rFlexPot = constrain(map(-rFlexVal, -rWing.flexMax, -rWing.flexMin, rWing.minHeight, rWing.maxHeight), rWing.minHeight, rWing.maxHeight);
  
  // move left wing
  if (lPotVal >= lWing.maxHeight) {
    lMotor->run(lWing.downDirection);
  } else if (lPotVal <= lWing.minHeight) {
    lMotor->run(lWing.downDirection);
  } else if (abs(lFlexPot - lPotVal) < 2) {
    lMotor->run(RELEASE);
  } else if (lFlexPot > lPotVal) {
    lMotor->run(lWing.upDirection);
  } else {
    lMotor->run(lWing.downDirection);
  }
  
  // move right wing
  if (rPotVal >= rWing.maxHeight) {
    rMotor->run(lWing.downDirection);
  } else if (rPotVal <= rWing.minHeight) {
    rMotor->run(rWing.downDirection);
  } else if (abs(rFlexPot - rPotVal) < 2) {
    rMotor->run(RELEASE);
  } else if (rFlexPot > rPotVal) {
    rMotor->run(rWing.upDirection);
  } else {
    rMotor->run(rWing.downDirection);
  }

  Pair newWings = {lWing, rWing, lMotor, rMotor};
  delay(100);
  return newWings;
}
