#ifndef fluttering_h
#define fluttering_h

#include "Arduino.h"

typedef struct { int sTrigPin; // ultrasonic echo pins
                 int sEchoPin;
                 int fTrigPin;
                 int fEchoPin;
                 int flexInputPin; // flex sensor pin
                 int potPin; // potentiometer pin
                 int flexSensorVal;  
                 bool isWaiting; // is waiting for other one to meet it
                 bool isOpening; // is opening (in flap mode)
                 int upDirection; // motor direction for up
                 int downDirection; // motor direction for down
                 int minHeight; // minimum potentiometer value
                 int maxHeight; // maximum potentiometer value
                 int goalVal; // for going to a specific angle
                 int flexMin; // minimum flex sensor value (should be reached at top)
                 int flexMax; // maximum flex sensor value (should be reached at bottom)
                 } Wing;

typedef struct { Wing wingLeft;
                 Wing wingRight;
                 Adafruit_DCMotor *motorLeft;
                 Adafruit_DCMotor *motorRight;
} Pair;

long ultrasonic_dist(Wing wing, String sensor="front");

#endif

                 



