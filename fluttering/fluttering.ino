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
// trigPin, echoPin, flexInputPin, potPin, currentpos, flexSensorVal, isUpFlutter, isOpening
// upDirection, downDirection, minHeight, maxHeight, goalVal
Wing rightWing = {12, 13, A2, A0, NULL, NULL, true, true, 
                  FORWARD, BACKWARD, 48, 60, 60 };
Wing leftWing = {10, 11, A3, A1, NULL, NULL, true, true,
                 BACKWARD, FORWARD, 38, 50, 50 };

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
  long distance = pulseIn(wing.echoPin, HIGH);
  return distance;
 }
 

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

// Wing flap(Adafruit_DCMotor *motor, Wing wing) {
////  long dist = ultrasonic_dist(wing);
//  int potVal = analogRead(wing.potPin);
//  int flexVal = analogRead(wing.flexInputPin);
//  Serial.println(potVal);
//  // new right potVal: 0 to 84
//  // int angle = map(potVal, 38, , 45, 90);
//  motor->setSpeed(150);
////  if (dist <= 1000) {
////    if (potVal >= wing.minHeight) {
////      wing.isOpening = false;
////      motor->run(RELEASE);
////      delay(100);
////    } else {
////      motor->run(RELEASE);
////      delay(100);
////      return wing;
////    }
////  }
//  if (wing.isOpening) {
//    // going up if we can
//    if (potVal >= wing.maxHeight) {
//      wing.isOpening = false;
//       motor->run(RELEASE);
//    } else {
//      motor->run(wing.upDirection);
//  } 
//  } else {
//    // going down if we can
//    if (potVal <= wing.minHeight) {
//      wing.isOpening = true;
//       motor->run(RELEASE);
//  } else {
//      motor->run(wing.downDirection);
//    }
//  }
//  delay(50);
//  return wing;
// }

Wing set_goalval(Wing wing, int goal) {
  wing.goalVal = goal;
  return wing;
}

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
leftWing = go_to_angle(leftMotor, leftWing);
Serial.println(leftWing.goalVal);

}
