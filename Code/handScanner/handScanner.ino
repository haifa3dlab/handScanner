#include "ScannerCtrl.h"
#include "Parser.h"



Scanner scanner;
Parser parser;
int cmdDelay = 20;
int init_flag = 1;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");  

  /*
  // press button to half everything, usually it is recommanded to add power down emergency button
  //just remember that on power down the camera motor is disabled and the camera will "fall" 
  attachInterrupt(digitalPinToInterrupt(interruptPin), Scanner::emergencyStop, CHANGE);
  */
}

void loop(){
  uint32_t value = NO_VALUE; //NO_VALUE defined in Parser.h
  //Serial.println("New Loop: ");
  if ( parser.Listen())
    value = parser.callCommand(scanner);

  if ( value != NO_VALUE ) {
    Serial.print("Value Recevied: ");
    Serial.print(value);
  }
  delay(cmdDelay);
}
/*
void loop() {
  
  if (init_flag){
    Serial.println("Reseting Motor First");
    scanner.init();
    init_flag = 0;
  }

  // set motor's speed
  scanner.setHeightSpeed(10);
  scanner.setAngularSpeed(10);
  
  // turn base
  delay(cmdDelay);
  scanner.baseTurn(180);
  delay(cmdDelay);
  if ( scanner.getBaseAngle() > 200 ){
    scanner.baseTurn(-180);
  }

  // release motor to reduce heating (note that if you release the camera motor's the camera might "fall")
  scanner.releaseMotors(); // They are getting hot otherwise!!!

  // move camera motor
  delay(cmdDelay);
  if (  scanner.cameraMove(60) == err_exceeds_limits){
    Serial.println("Error - exceeds motor limits!");
  }
  delay(cmdDelay*3);

  // set different speed 
  scanner.setHeightSpeed(30);
  scanner.setAngularSpeed(30);

  // do another base turn
  delay(cmdDelay);
  scanner.baseTurn(90);
  delay(cmdDelay);
  if ( scanner.getBaseAngle() > 200 ){
    scanner.baseTurn(-90);
  }

  // release motors
  scanner.releaseMotors(); // They are getting hot otherwise!!!

  // move camera motor
  delay(cmdDelay);
  if (  scanner.cameraMove(60) == err_exceeds_limits){
    Serial.println("Error - exceeds motor limits!");
  }
  delay(cmdDelay*3);

}
*/
