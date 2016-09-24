
#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #if defined(__AVR__)
    #include <avr/io.h>
  #endif
  #include "WProgram.h"
#endif

#include "Parser.h"




String Parser::readStringInput(){
  return Serial.readStringUntil(' ');
}

/*
 * Description: looping till the # is received (to infinity if not received)
 * 
 * 
 * 
 */
bool Parser::Listen(int timeOut){
  int incomingByte = 555;
  mCommand = "NULL";
  mParam = "NULL";
  while (incomingByte != '#'){
    if (Serial.available() > 0) {
                incomingByte = Serial.read();
    }
    if (timeOut -- == 0 ){
      //Serial.println("TimeOut Occur!");
      return false;
    }
  }
  mCommand = readStringInput(); mCommand.trim();
  if (Serial.available() > 0){ mParam = readStringInput(); mParam.trim();}
  // debug
  Serial.print("\nCommand Received: ") ; Serial.print(mCommand); Serial.print("\nParameter Received: ") ; Serial.print(mParam); Serial.print("\n");
  return true;
}


/* Description: calls the scanner command
 * 
 * @param: uses the mCommand and mParam member variable
 * @return: return uint32_t for methods "getBaseAngle" and "getCameraPosition"
 */
uint32_t Parser::callCommand(Scanner &scanner){
    if ( mCommand == "baseTurn"){
        if ( DEBUG_PARSER ){Serial.print("baseTurn ");Serial.print(mParam);Serial.print("\n");}
        scanner.baseTurn(mParam.toInt());
    }
    else if ( mCommand == "setAngularSpeed"){
      if ( DEBUG_PARSER ) {Serial.print("setAngularSpeed ");Serial.print(mParam);Serial.print("\n");}
        scanner.setAngularSpeed(mParam.toInt());
    }
    else if ( mCommand == "cameraMove"){
       if ( DEBUG_PARSER ) {Serial.print("cameraMove ");Serial.print(mParam);Serial.print("\n");}
       scanner.cameraMove(mParam.toInt());
    }
    else if ( mCommand == "setHeightSpeed"){
        if ( DEBUG_PARSER ) {}
        scanner.setHeightSpeed(mParam.toInt());
    }
    else if ( mCommand == "doFullScan"){
       if ( DEBUG_PARSER ) {Serial.print("doFullScan\n");}
        scanner.doFullScan();
    }
    else if ( mCommand == "releaseMotors"){
      if ( DEBUG_PARSER ) {Serial.print("releaseMotors\n");}
      scanner.releaseMotors();
    }
    else if ( mCommand == "getBaseAngle"){
        if ( DEBUG_PARSER ) {Serial.print("getBaseAngle\n");}
        return scanner.getBaseAngle();
    }
    else if ( mCommand == "getCameraPosition"){
        if ( DEBUG_PARSER ) {Serial.print("getCameraPosition\n");}
        return scanner.getCameraPosition();
    }
    else if ( mCommand == "init"){
        scanner.init();     
    }
    else {
        Serial.print("No Such Command!\n");
        return NO_VALUE;
    }
    return NO_VALUE;
     
}

