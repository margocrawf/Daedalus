#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "Arduino.h"
#include "fluttering.h"
#include "utility/Adafruit_MS_PWMServoDriver.h"

Pair flap_flex_2_wings(Pair wings) {
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

  // map the flex vals to pot vals
  int lFlexPot = constrain(map(-lFlexVal, -lWing.flexMax, -lWing.flexMin, lWing.minHeight, lWing.maxHeight), lWing.minHeight, lWing.maxHeight);
  int rFlexPot = constrain(map(-rFlexVal, -rWing.flexMax, -rWing.flexMin, rWing.minHeight, rWing.maxHeight), rWing.minHeight, rWing.maxHeight);

  // LEFT WING
  // if youre waiting and your val is different, start to downflap.
  if (lWing.isWaiting) {
    if (abs(lFlexPot - lPotVal) > 2) {
      lWing.isWaiting = false;
      lWing.isOpening = false;
    } 
  }
  // if youre not waiting and your val is different-- if youre upflapping go up til val, if youre downflapping keep downflappin
  // if youre not waiting and your val is the same, start waiting.
  else {
    // if ur opening
    if (lWing.isOpening) {
      // if youre not @ desired value
      if (abs(lFlexPot - lPotVal) > 2) {
        // if youre @ max val, STOP otherwise keep goin
        if (lPotVal >= lWing.maxHeight) {
          lMotor->run(RELEASE);
          lWing.isOpening = false;
        } else {
          lMotor->run(lWing.upDirection);
        }
       // youre @ desired value!!!!!!
      } else {
        lWing.isWaiting = true;
        lWing.isOpening = false;
        lMotor->run(RELEASE);
      }
    // if youre not opening
    } else {
      // if ur too low
      if (lPotVal <= lWing.minHeight) {
          lWing.isOpening = true;
          lMotor->run(RELEASE);
      // u can still go down
      } else {
        lMotor->run(lWing.downDirection);
      
    }
    
  }
  }
  
  // RIGHT WING
  // if youre waiting and your val is different, start to downflap.
  if (rWing.isWaiting) {
    if (abs(lFlexPot - rPotVal) > 2) {
      rWing.isWaiting = false;
      rWing.isOpening = false;
    } 
  }
  // if youre not waiting and your val is different-- if youre upflapping go up til val, if youre downflapping keep downflappin
  // if youre not waiting and your val is the same, start waiting.
  else {
    // if ur opening
    if (rWing.isOpening) {
      // if youre not @ desired value
      if (abs(rFlexPot - rPotVal) > 2) {
        // if youre @ max val, STOP otherwise keep goin
        if (rPotVal >= rWing.maxHeight) {
          rMotor->run(RELEASE);
          rWing.isOpening = false;
        } else {
          rMotor->run(rWing.upDirection);
        }
       // youre @ desired value!!!!!!
      } else {
        rWing.isWaiting = true;
        rWing.isOpening = false;
        rMotor->run(RELEASE);
      }
    // if youre not opening
    } else {
      // if ur too low
      if (rPotVal <= rWing.minHeight) {
          rWing.isOpening = true;
          rMotor->run(RELEASE);
      // u can still go down
      } else {
        rMotor->run(rWing.downDirection);
      
    }
    
  }
  }
  
}

