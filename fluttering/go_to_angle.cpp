#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "Arduino.h"
#include "fluttering.h"
#include "utility/Adafruit_MS_PWMServoDriver.h"

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
