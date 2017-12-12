#ifndef fluttering_h
#define fluttering_h

#include "Arduino.h"

typedef struct { int trigPin;
                 int echoPin;
                 int flexInputPin;
                 int potPin;
                 int currentpos;
                 int flexSensorVal;
                 bool isWaiting;
                 bool isOpening; 
                 int upDirection;
                 int downDirection;
                 int minHeight;
                 int maxHeight;
                 int goalVal;
                 } Wing;

typedef struct { Wing wingLeft;
                 Wing wingRight;
                 Adafruit_DCMotor *motorLeft;
                 Adafruit_DCMotor *motorRight;
} Pair;

long ultrasonic_dist(Wing wing);

#endif

                 



