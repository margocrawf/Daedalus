#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "fluttering.h"

// define motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// define motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);

// interrupt pin
int interruptPin = 2; 

// delay time for flutters
int delayTime = 20;

// defining wing structs, in the following order:
// IRPin, flexInputPin, potPin, currentpos, flexSensorVal, isUpFlutter, isOpening
// upDirection, downDirection
Wing rightWing = {A0, A2, A4, NULL, NULL, true, true, BACKWARD, FORWARD};
Wing leftWing = {A1, A3, A5, NULL, NULL, true, true, FORWARD, BACKWARD};


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

  //attachInterrupt(digitalPinToInterrupt(interruptPin), isr_fold_down, CHANGE);
  
}

/*
 * Move a motor to a goal angle, based on pot.
 * It's a DIY servo!
 */
void moveMotor(int goalAngle, Adafruit_DCMotor *motor, Wing wing){
  
  int potVal = analogRead(wing.potPin);
  int wingAngle = map(potVal, 0, 1023, 0, 270);
  if (wingAngle < goalAngle) {
    // moving up
    while ( wingAngle < goalAngle) {
      Serial.println(wingAngle);
      motor->setSpeed(100);
      motor->run(wing.upDirection);
      delay(1000);
      Serial.println("moving up");
      int potVal = analogRead(wing.potPin);
      wingAngle = map(potVal, 0, 1023, 0, 270);
    }
  }
  else {
    while ( wingAngle > goalAngle) {
      Serial.println(wingAngle);
      motor-> setSpeed(100);
      motor->run(wing.downDirection);
      delay(100);
      Serial.println("moving down");
      int potVal = analogRead(wing.potPin);
      wingAngle = map(potVal, 0, 1023, 0, 270);
    }
  }
}

/*
 * flutter all the way
 */
void flutter (Adafruit_DCMotor *motor, Wing wing) {
  // define the degree from the sensor
  int degree = map(wing.flexSensorVal, 740, 815, 0, 135);
  degree = constrain(degree, 0, 135);
  //flutter dat shit
  flutter_to_degree(motor, degree, wing);
  }

/*
 * flutter to a specific degree
 */
void flutter_to_degree (Adafruit_DCMotor *motor, int degree, Wing wing) {

  Serial.println(wing.flexSensorVal);
  if (abs(degree - wing.currentpos) < 5) {
    // stay where u r
    return;
  }
  // determine if you're going up or down
  if (degree > wing.currentpos) {
    wing.isOpening = true;
  } else {
    wing.isOpening = false;
  }
  if ((wing.isOpening) and (wing.currentpos + 20 < 89)) {
        if(wing.isUpFlutter) {
           for (int i = 0; i < 20; i++) {
           wing.currentpos+=1;
           moveMotor(wing.currentpos,motor,wing);
           delay (delayTime);
        } 
      } else {
           for (int i = 0; i < 15; i++){
           wing.currentpos-=1;
           moveMotor(wing.currentpos,motor,wing);
           delay (delayTime);
      }   
    }
     
  } else { //is not opening
      if (wing.currentpos - 20 > 0){
        if (!wing.isUpFlutter) {
        for (int i = 0; i < 20; i++){
          wing.currentpos-=1;
          moveMotor(wing.currentpos,motor,wing);
          delay (delayTime);
        } 
      } else {
        for (int i = 0; i < 15; i++){
          wing.currentpos+=1;
          moveMotor(wing.currentpos,motor,wing);
          delay(delayTime);
        }
      }
     } 
    }
 }

/*
 * interrupt service routine for putting the wings back down immediately.
 */
void isr_fold_down() {
  moveMotor(0, rightMotor, rightWing);
  moveMotor(0, leftMotor, leftWing);
}


void loop() {
  // the real code but were commenting it out for now
  rightWing.flexSensorVal = analogRead(rightWing.flexInputPin);
//  flutter(rightMotor,rightWing);
  leftWing.flexSensorVal = analogRead(leftWing.flexInputPin);
//  flutter(leftMotor, leftWing);


    moveMotor(180, rightMotor, rightWing);
    moveMotor(270, rightMotor, rightWing);
  

}
