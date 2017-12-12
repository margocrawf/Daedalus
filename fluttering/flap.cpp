#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "Arduino.h"
#include "fluttering.h"
#include "utility/Adafruit_MS_PWMServoDriver.h"

/*
 * Flap to top and bottom of 1 wing.
 */
Wing flap(Adafruit_DCMotor *motor, Wing wing) {
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

/*
 * Flap to flex sensor value on one wing
 */
 Wing flex_follow(Adafruit_DCMotor *motor, Wing wing) {
  long dist = ultrasonic_dist(wing);
  int potVal = analogRead(wing.potPin);
  int flexVal = analogRead(wing.flexInputPin);
  int flexPot = map(-flexVal, -840, -760, wing.minHeight, wing.maxHeight);
  flexPot = constrain(flexPot, wing.minHeight, wing.maxHeight);
  Serial.println(flexPot);
  Serial.println(potVal);
  Serial.println(dist);

  motor->setSpeed(150);
  if (potVal > 60) {
    motor->run(wing.downDirection);
  } else if (potVal < 40) {
    motor->run(wing.upDirection);
//  } else if (dist <= 1000) {
//    Serial.println("ultrasonic sensor!");
//    motor->run(RELEASE);
//    delay(200);
  } else if (abs(flexPot - potVal) < 2) {
    motor->run(RELEASE);
  } else if (flexPot > potVal) {
    motor->run(wing.upDirection);
  } else {
    motor->run(wing.downDirection);
  }
  delay(100);
  return wing;
 }

/*
 * Move one wing to a specific angle.
 */
 Wing go_to_angle(Adafruit_DCMotor *motor, Wing wing) {
  // make sure there's an angle to go to
  if (wing.goalVal == NULL) {
    Serial.println("No goal angle. Go directly to jail do not collect $200");
    return wing;
  }

  // read the pot
  int potVal = analogRead(wing.potPin);
  Serial.println(potVal);
  wing.currentpos = potVal;
  // if we want to go up, go up.
  if (wing.goalVal > potVal) {
    // make sure we're not too high
    if (potVal < wing.maxHeight) {
      motor->run(wing.upDirection);
    }
    // if we are, just stop.
    else {
      motor->run(RELEASE);
    }
  } 
  // if we want to go down, go down.
  else if (wing.goalVal < potVal) {
    // make sure we're not too low
    if (potVal > wing.minHeight) {
      motor->run(wing.downDirection);
    }
    // if we are, stop it. get some help.
    else {
      motor->run(RELEASE);
    }
  }
  // wait for a bit
  delay(50);
  return wing;
 }
 
