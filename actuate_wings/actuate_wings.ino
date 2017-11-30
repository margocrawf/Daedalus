#include <Servo.h>

// servo
Servo servo;
// pin values for sensors
int IRInPin = A0; // infrared sensor
const int forceInputPin = A1; // pin for force sensitive resistor
// other global variables
int currentpos;
bool isOpening = true;

/*
 * Starting up things.
 */
void setup() {
  // start the serial
  Serial.begin(9600);
  servo.attach(9);
  
}

/*
 * Take infrared value, use regression to
 * give us an approximate distance
 * TODO make the calibration good enough to use
 */
int sensorval_to_dist(int sensorVal) {
  float p1 = 0.0002808;
  float p2 = -0.3229;
  float p3 = 112.5;
  float dist = p1*pow(sensorVal,2) + p2*sensorVal + p3;
//  Serial.print(dist);
//  Serial.print(", ");
//  Serial.println(sensorVal);
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
    int sensorValue = analogRead(IRInPin);
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
 * MODE: Open the wing based on force sensor value.
 */
 void force_sense(int dist) {
  int sensorIn = analogRead(forceInputPin);
  Serial.print("Force sensor: ");
  Serial.println(sensorIn);
  int degree = map(sensorIn, 610, 850, 0, 180);
  degree = constrain(degree, 0, 180);
  if ((degree + 10 > currentpos) and (degree - 10 < currentpos)) {
    //don't do anything, just stay where you are
    return;
  }
  else if (degree < currentpos) {
    // move down
    for (int i = currentpos; i > degree; i--) {
      delay(25);
      currentpos -= 1;
      servo.write(currentpos);
    }
  }
  else {
    // move up
    if (dist > 300) {
      //don't do anything, there's an object in the way
      return;
    }
    for (int i = currentpos; i < degree; i++) {
      delay(25);
      currentpos += 1;
      servo.write(currentpos);
    }
  }
  }

/*
 * MODE: Flap the wing (one of the states we might want)
 */
void flap(int dist) {
  if (isOpening and dist < 300) {
    if (currentpos < 89) {
      currentpos += 1;
      servo.write(currentpos);
    } else {
      isOpening = false;
    }
  }
  else {
    isOpening = false;
    if (currentpos > 0) {
      currentpos -= 1;
      servo.write(currentpos);
    } else {
      isOpening = true;
    }
  }
}

/*
 * MODE: Try to hold open position, close if something is close
 */
void stay_open(int dist) {
  if (dist < 30 ) {
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
  force_sense(dist);
}

/*
 * Main loop function
 */
void loop() {
  // read from the sensor
  int distance = average_distance_val(1);
  Serial.println(distance);
  // move (bitch, get out the way)
  move_motor(distance);
}
