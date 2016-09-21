#include "ScannerCtrl.h"
//#include "AFMotor.h"

// Connect a stepper motor with 48 steps per revolution (7.5 degree)
// to motor port #2 (M3 and M4)
Scanner scanner;
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");  
  //motor.setSpeed(10);  // rpm   
}

 
//AF_Stepper motor(200,2);
int steps2do = 1000;
int stopDelay = 200; // mS
int cmdDelay = 3000;
int init_flag = 1;


void loop() {

  if (init_flag){
    Serial.println("Reseting Motor First");
    scanner.init();
    init_flag = 0;
  }

/*  Serial.println("Single coil steps");
  motor.step(steps2do, FORWARD, SINGLE); 
  delay(stopDelay);
  motor.step(steps2do, BACKWARD, SINGLE); 
  delay(stopDelay);
*/
  
  Serial.println("Waiting for Command");
   scanner.setHeightSpeed(10);
  scanner.setAngularSpeed(10);
  delay(cmdDelay);
  scanner.baseTurn(180);
  delay(cmdDelay);
  if ( scanner.getBaseAngle() > 200 ){
    scanner.baseTurn(-180);
  }
  scanner.releaseMotors(); // They are getting hot otherwise!!!
  
  delay(cmdDelay);
  if (  scanner.cameraMove(60) == err_exceeds_limits){
    Serial.println("Error - exceeds motor limits!");
  }
  delay(cmdDelay*3);
 
  scanner.setHeightSpeed(30);
  scanner.setAngularSpeed(30);
  
  delay(cmdDelay);
  scanner.baseTurn(90);
  delay(cmdDelay);
  if ( scanner.getBaseAngle() > 200 ){
    scanner.baseTurn(-90);
  }
  scanner.releaseMotors(); // They are getting hot otherwise!!!
  
  delay(cmdDelay);
  if (  scanner.cameraMove(60) == err_exceeds_limits){
    Serial.println("Error - exceeds motor limits!");
  }
  delay(cmdDelay*3);
 


}
