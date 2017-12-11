#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "fluttering.h"

// define motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// define motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);

// interrupt pin
int interruptPin = 2; 

// delay time for flutters
int delayTime = 20;

// defining wing structs, in the following order:
// trigPin, echoPin, flexInputPin, potPin, currentpos, flexSensorVal, isUpFlutter, isOpening
// upDirection, downDirection
Wing rightWing = {12, 13, A2, A0, NULL, NULL, false, false, FORWARD, BACKWARD};
Wing leftWing = {10, 11, A3, A1, NULL, NULL, true, true, FORWARD, BACKWARD};

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
  Serial.println(distance);
  return distance;
 }

 Wing flex_follow(Adafruit_DCMotor *motor, Wing wing) {
  // long dist = ultrasonic_dist(wing);
  int potVal = analogRead(wing.potPin);
  int flexVal = analogRead(wing.flexInputPin);
  int flexPot = map(-flexVal, -840, -740, 42, 60);
  flexPot = constrain(flexPot, 42, 60);
  Serial.println(flexPot);
  Serial.println(potVal);

  motor->setSpeed(150);
  if (potVal > 60) {
    Serial.println("too high");
    motor->run(wing.downDirection);
  }
  else if (potVal < 42) {
    Serial.println("shawty got low low low low low low low low");
    motor->run(wing.upDirection);
  }
  else if (abs(flexPot - potVal) < 5) {
    Serial.println("hold it right there!");
    motor->run(RELEASE);
  } else if (flexPot > potVal) {
    Serial.println("going up!");
    motor->run(wing.upDirection);
  } else {
    Serial.println("sugar we're goin down swingin");
    motor->run(wing.downDirection);
  }
  delay(100);
  return wing;
 }

 Wing flap(Adafruit_DCMotor *motor, Wing wing) {
  long dist = ultrasonic_dist(wing);
  int potVal = analogRead(wing.potPin);
  int flexVal = analogRead(wing.flexInputPin);
  Serial.println(potVal);
  Serial.println(flexVal);
  // new right potVal: 0 to 84
  // int angle = map(potVal, 38, , 45, 90);
  motor->setSpeed(150);
  if (dist <= 1000) {
    if (potVal >= 45) {
      wing.isOpening = false;
      delay(100);
    } else {
      delay(100);
      return wing;
    }
  }
  if (wing.isOpening) {
    // going up if we can
    if (potVal >= 60) {
      wing.isOpening = false;
      // motor->run(wing.downDirection);
    } else {
      motor->run(wing.upDirection);
  } 
  } else {
    // going down if we can
    if (potVal <= 45) {
      wing.isOpening = true;
      // motor->run(wing.upDirection);
  } else {
      motor->run(wing.downDirection);
    }
  }
  delay(100);
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

//rightWing = flap(rightMotor, rightWing);
rightWing = flex_follow(rightMotor, rightWing);

}
