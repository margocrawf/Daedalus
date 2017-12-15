Different wing motions for the wings.
Fluttering.ino is the main function, within it, the Wing structs and other variables are initialized and any other functions can be called. The following movement functions are available:

* Wing flap_1_wing(Adafruit_DCMotor *motor, Wing wing);
  Flaps a single wing from its bottom to its top position repeatedly, except if there is a nearby object detected.

* Pair flap_2_wings(Pair wings);
  Flaps both wings in sync, retreats if an object is in the way.

* Pair flap_flex_2_wings(Pair wings);
  Flaps both wings with changes in flex sensor value. Flaps them down and then up again.

* Wing flex_1_wing(Adafruit_DCMotor *motor, Wing wing);
  Moves to the position of the arm for 1 wing.

* Pair flex_2_wings(Pair wings);
  Moves both wings to the position of the arm.

* Wing go_to_angle(Adafruit_DCMotor *motor, Wing wing);u
  Move a wing to an angle specified by goal angle.
