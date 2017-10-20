#include <Servo.h>

// servo
Servo servo;
// pin values for sensors
int analogPin0 = A0; // infrared sensor
int currentpos;
bool isOpening = true;

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
 * Get the average of given number of sensor readings, to account for 
 * occasional inconsistencies in readings
 */
float average_distance_val(int reps) {
  float total = 0;
  for (int i = 0; i < reps; i++) {
    // read the analog in value:
    int sensorValue = analogRead(analogPin0);
    // map it to a distance
    float distance = sensorval_to_dist(sensorValue);
    //add it to the total for averaging purposes
    total += distance;
    // wait 50 milliseconds before the next reading, to allow the
    // sensor to regather the value
    delay(50);
  }
  return total/reps;
}

/*
 * Flap the wing (one of the states we might want)
 */
void flap(int dist) {
  if (isOpening and dist < 300) {
    if (currentpos < 89) {
      currentpos += 5;
      servo.write(currentpos);
      delay(50);
    } else {
      isOpening = false;
    }
  else {
    isOpening = false;
    if (currentpos > 0) {
      currentpos -= 5;
      servo.write(currentpos);
      delay(50);
    } else {
      isOpening = true;
    }
  }
}

/*
 * Try to hold open position, close if something is close
 */
void stay_open(int dist) {
  if (dist > 100) {
    Serial.println("Too close! Retreat!");
    if (currentpos < 89) {
        currentpos += 5;
        servo.write(currentpos);
        delay(50);
    }
  } 
  else {
    Serial.println("I believe I can fly!");
    if (currentpos > 0) {
        currentpos -= 5;
        servo.write(currentpos);
        delay(50);
    }
  }
}

/*
 * Take distance, move the wing accordingly
 */
void move_motor(int dist) {
  flap(dist);
}

void loop() {
  // read from the sensor
  int distance = average_distance_val(10);
  // move (bitch, get out the way)
  move_motor(distance);
}
