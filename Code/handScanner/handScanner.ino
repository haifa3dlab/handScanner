// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include "AFMotor.h"
#include "ScannerCtrl.h"



// Connect a stepper motor with 48 steps per revolution (7.5 degree)
// to motor port #2 (M3 and M4)

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  // calculate steps per revolution for base motor

   // calculate steps per revolution for vertical motor
  

  Scanner scanner();
  
  //motor.setSpeed(10);  // rpm   
}

int steps2do = 1000;
int stopDelay = 200; // mS

void loop() {
/*  Serial.println("Single coil steps");
  motor.step(steps2do, FORWARD, SINGLE); 
  delay(stopDelay);
  motor.step(steps2do, BACKWARD, SINGLE); 
  delay(stopDelay);
*/
  Serial.println("Double coil steps");
  //motor.step(steps2do, FORWARD, DOUBLE);
  delay(stopDelay);
  //motor.step(steps2do, BACKWARD, DOUBLE);
  delay(stopDelay);  

/*
  Serial.println("Interleave coil steps");
  motor.step(steps2do, FORWARD, INTERLEAVE); 
  delay(stopDelay);
  motor.step(steps2do, BACKWARD, INTERLEAVE); 
  delay(stopDelay);
  
  Serial.println("Micrsostep steps");
  motor.step(steps2do, FORWARD, MICROSTEP); 
  delay(stopDelay);  
  motor.step(steps2do, BACKWARD, MICROSTEP); 
  delay(stopDelay);
  */
}
