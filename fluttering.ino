#include <Servo.h>

// servo
Servo servo;
// pin values for sensors
int IRInPin = A0; // infrared sensor
const int forceInputPin = A1; // pin for force sensitive resistor
// other global variables
int currentpos;
int anchorpos;
bool isOpening = true;
bool isUpFlutter = true;

/*
 * Starting up things.
 */
void setup() {
  // start the serial
  Serial.begin(9600);
  servo.attach(9);
  
}

void flutter (){
  if(isUpFlutter){
    if (currentpos + 20 < 89) {
      for (int i = 0; i < 20; i++) {
        currentpos+=1;
        servo.write(currentpos);
        delay (50);
      }
      for (int i = 0; i < 15; i++){
        currentpos-=1;
        servo.write(currentpos);
        delay (50);
      }
    } else {
        isUpFlutter = false;
    }
  } else {
      if (currentpos - 20 > 0){
        for (int i = 0; i < 20; i++){
          currentpos-=1;
          servo.write(currentpos);
          delay (50);
        }
        for (int i = 0; i < 15; i++){
          currentpos+=1;
          servo.write(currentpos);
          delay(50);
        }
      } else {
        isUpFlutter = true;
      }
    }
 }


void loop() {
  flutter();
}
