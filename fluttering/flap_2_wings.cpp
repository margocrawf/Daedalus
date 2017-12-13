/*
 * Flap em both at once
 */
#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "Arduino.h"
#include "fluttering.h"
#include "utility/Adafruit_MS_PWMServoDriver.h"


Pair flap_2_wings(Pair wings) {
  // unpack values
  Wing lWing = wings.wingLeft;
  Wing rWing = wings.wingRight;
  Adafruit_DCMotor *lMotor = wings.motorLeft;
  Adafruit_DCMotor *rMotor = wings.motorRight;

  long rfUltraVal = ultrasonic_dist(rWing, "front");
  long lfUltraVal = ultrasonic_dist(lWing, "front");

  long rsUltraVal = ultrasonic_dist(rWing, "side");
  long lsUltraVal = ultrasonic_dist(lWing, "side");

  int rPotVal = analogRead(rWing.potPin);
  int rFlexVal = analogRead(rWing.flexInputPin);

  int lPotVal = analogRead(lWing.potPin);
  int lFlexVal = analogRead(lWing.flexInputPin);

  Serial.println(rPotVal);
  Serial.println(lPotVal);

  // if youre too close go down
  if ( (rfUltraVal < 1000) or (lfUltraVal < 1000) or (rsUltraVal < 1000) or (lsUltraVal < 1000) ) {
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
