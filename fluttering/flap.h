#ifndef flap_h
#define flap_h

#include "fluttering.h"
#include "Arduino.h"

Wing flap(Adafruit_DCMotor *motor, Wing wing);

Wing flex_follow(Adafruit_DCMotor *motor, Wing wing);

Wing go_to_angle(Adafruit_DCMotor *motor, Wing wing);

#endif
