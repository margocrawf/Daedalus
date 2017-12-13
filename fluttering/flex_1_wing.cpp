#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "Arduino.h"
#include "fluttering.h"
#include "utility/Adafruit_MS_PWMServoDriver.h"

/*
 * Flap to flex sensor value on one wing
 */
 Wing flex_1_wing(Adafruit_DCMotor *motor, Wing wing) {
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
