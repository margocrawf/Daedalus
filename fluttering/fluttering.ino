#include <Servo.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);


// servo
Servo servo;
// pin values for sensors
int rightIRPin = A0; // infrared sensor
int leftIRPin = A1;
const int rightFlexInputPin = A2; // pin for force sensitive resistor
const int leftFlexInputPin = A3;
int rightPotPin = A4; //pin for the potentiometers
int leftPotPin = A5;
// other global variables
int rightcurrentpos;
int leftcurrentpos;
int anchorpos;
int rightFlexSensor;
int leftFlexSensor;
bool rightisUpFlutter = true;
bool leftisUpFlutter = true;
bool rightisOpening = true;
bool leftisOpening = true;
int delayTime = 20;

/*
 * Starting up things.
 */
void setup() {
  // start the serial
  Serial.begin(9600);
  servo.attach(9);
  
}

/*
 * Move a motor to a goal angle, based on pot.
 * It's a DIY servo!
 */
void moveMotor(int goalAngle, Adafruit_DCMotor *motor, int pot){
  int potVal = analogRead(pot);
  int wingAngle = map(potVal, 0, 1023, 0, 270);
  while ( wingAngle < goalAngle) {
    motor-> setSpeed(100);
    motor->run(FORWARD);
    int potVal = analogRead(pot);
    wingAngle = map(potVal, 0, 1023, 0, 270);
  }
}

void flutter (int flexSensor,int pot,Adafruit_DCMotor *motor,int IR,
              int currentpos,bool isUpFlutter,bool isOpening){
  int degree = map(flexSensor, 740, 815, 0, 135);
  degree = constrain(degree, 0, 135);
  Serial.println(flexSensor);
  if (abs(degree - currentpos) < 5) {
    return;
  }
  if (degree > currentpos) {
    isOpening = true;
  } else {
    isOpening = false;
  }
  if(isOpening){
     if (currentpos + 20 < 89) {
        if(isUpFlutter) {
           for (int i = 0; i < 20; i++) {
           currentpos+=1;
           moveMotor(currentpos,motor,pot);
           delay (delayTime);
        } 
      } else {
           for (int i = 0; i < 15; i++){
           currentpos-=1;
           moveMotor(currentpos,motor,pot);
           delay (delayTime);
      }   
    }
     
    } 
  } else { //is not opening
      if (currentpos - 20 > 0){
        if (!isUpFlutter) {
        for (int i = 0; i < 20; i++){
          currentpos-=1;
          moveMotor(currentpos,motor,pot);
          delay (delayTime);
        } 
      } else {
        for (int i = 0; i < 15; i++){
          currentpos+=1;
          moveMotor(currentpos,motor,pot);
          delay(delayTime);
        }
      }
      } 
    }
 }


void loop() {
  rightFlexSensor = analogRead(rightFlexInputPin);
  flutter(rightFlexSensor,rightPotPin,rightMotor,rightIRPin,
          rightcurrentpos,rightisUpFlutter, rightisOpening);
  leftFlexSensor = analogRead(leftFlexInputPin);
  flutter(leftFlexSensor,leftPotPin,leftMotor,leftIRPin,
          leftcurrentpos,leftisUpFlutter, leftisOpening);
}
