#include <Servo.h>

// servo
Servo servo;
// pin values for sensors
int analogPin0 = A0; // infrared sensor
int currentpos;

void setup() {
  // start the serial
  Serial.begin(9600);
  servo.attach(9);
  
}

/*
 * Take infrared value, use regression to
 * give us an approximate distance
 * TODO make it calibrated
 */
int sensorval_to_dist(int sensorVal) {
  Serial.println(sensorVal);
  return sensorVal;
}

/*
 * Take distance, move the wing accordingly
 */
void move_motor(int dist) {
  if (dist > 100) {
    Serial.println("Too close! Retreat!");
    if (currentpos < 89) {
      for (int pos = currentpos; pos <= 90; pos++) {
        servo.write(pos);
        currentpos = pos;
        delay(50);
      }
    }
  } else {
    Serial.println("I believe I can fly!");
    if (currentpos > 0) {
      for (int pos = currentpos; pos >= 0; pos--) {
        servo.write(pos);
        currentpos = pos;
        delay(50);
      }
    }
  }
}

void loop() {
  // read from the sensor
  int sensorValue = analogRead(analogPin0);
  // convert the value to a distance
  int distance = sensorval_to_dist(sensorValue);
  // move (bitch, get out the way)
  move_motor(distance);
}
