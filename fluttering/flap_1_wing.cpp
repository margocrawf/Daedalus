#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "Arduino.h"
#include "fluttering.h"
#include "utility/Adafruit_MS_PWMServoDriver.h"

/*
 * Flap to top and bottom of 1 wing.
 */
Wing flap_1_wing(Adafruit_DCMotor *motor, Wing wing) {
//  long dist = ultrasonic_dist(wing);
  int potVal = analogRead(wing.potPin);
  int flexVal = analogRead(wing.flexInputPin);
  Serial.println(potVal);
  // new right potVal: 0 to 84
  // int angle = map(potVal, 38, , 45, 90);
  motor->setSpeed(150);
//  if (dist <= 1000) {
//    if (potVal >= wing.minHeight) {
//      wing.isOpening = false;
//      motor->run(RELEASE);
//      delay(100);
//    } else {
//      motor->run(RELEASE);
//      delay(100);
//      return wing;
//    }
//  }
  if (wing.isOpening) {
    // going up if we can
    if (potVal >= wing.maxHeight) {
      wing.isOpening = false;
       motor->run(RELEASE);
    } else {
      motor->run(wing.upDirection);
  } 
  } else {
    // going down if we can
    if (potVal <= wing.minHeight) {
      wing.isOpening = true;
       motor->run(RELEASE);
  } else {
      motor->run(wing.downDirection);
    }
  }
  delay(50);
  return wing;
 }

 
