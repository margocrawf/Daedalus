#ifndef fluttering_h
#define fluttering_h

#include "Arduino.h"

typedef struct { int trigPin;
                 int echoPin;
                 int flexInputPin;
                 int potPin;
                 int currentpos;
                 int flexSensorVal;
                 bool isUpFlutter;
                 bool isOpening; 
                 int upDirection;
                 int downDirection;
                 } Wing;

#endif

                 



