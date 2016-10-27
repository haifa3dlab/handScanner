
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
  int timeOutCnt = timeOut;
  while (incomingByte != '#'){
    if (Serial.available() > 0) {
      incomingByte = Serial.read();
    }
    if (timeOutCnt-- <= 0 ){
      //if ( DEBUG_PARSER ) { Serial.println("TimeOut Occured (in cmd read)"); }
      return false;
    }
  }
  mCommand = readStringInput();
  mCommand.trim();

  timeOutCnt = timeOut;
  while (Serial.available() < 0)
  {
    if (timeOutCnt-- <= 0 ){
      if ( DEBUG_PARSER ) { Serial.println("*TimeOut Occur (in param read)*"); }
      return false;
    }
  }
  mParam = readStringInput();
  mParam.trim();

  // debug
  if ( DEBUG_PARSER ) {
    Serial.print("\nCommand Received: ");
    Serial.print(mCommand);
    Serial.print("\nParameter Received: ");
    Serial.print(mParam);
    Serial.print("\n");
  }
  return true;
}

void Parser::debugParserCmd(String cmdName)
{
  if ( !(DEBUG_PARSER) ) return;
  
  Serial.print(cmdName);
  Serial.print(" (");
  Serial.print(mCommand);
  Serial.print(") ");
  Serial.print(mParam);
  Serial.print(" (int: ");
  Serial.print(mParam.toInt(), DEC);
  Serial.print(")");
  Serial.print("\n");
}

/* Description: calls the scanner command
 * 
 * @param: uses the mCommand and mParam member variable
 * @return: return uint32_t for methods "getBaseAngle" and "getCameraPosition"
 * 
 * example commands: "#SAS 100# #SHS 50# #DFS 1#"
 */
uint32_t Parser::callCommand(Scanner &scanner){
    if ( mParam.length() <= 0 || mParam.equals("NULL") ) {
      Serial.println("*NULL param*");
      return err_bad_param;
    }

    if ( mCommand == "BT"/*"baseTurn"*/){
        debugParserCmd("baseTurn");  
        scanner.baseTurn(mParam.toInt());
    }
    if ( mCommand == "BTR"/*"baseTurnRel"*/){
        debugParserCmd("baseTurnRel");  
        scanner.baseTurnRel(mParam.toInt());
    }
    else if ( mCommand == "SAS"/*"setAngularSpeed"*/){
        debugParserCmd("setAngularSpeed");
        scanner.setAngularSpeed(mParam.toInt());
    }
    else if ( mCommand == "CM"/*"cameraMove"*/){
       debugParserCmd("cameraMove ");
       scanner.cameraMove(mParam.toInt());
    }
    else if ( mCommand == "CMR"/*"cameraMoveRel"*/){
       debugParserCmd("cameraMoveRel");
       scanner.cameraMoveRel(mParam.toInt());
    }
    else if ( mCommand == "SHS"/*"setHeightSpeed"*/){
        debugParserCmd("setHeightSpeed");
        scanner.setHeightSpeed(mParam.toInt());
    }
    else if ( mCommand == "DFS"/*"doFullScan"*/){
       debugParserCmd("doFullScan");
       scanner.doFullScan();
    }
    else if ( mCommand == "RM"/*"releaseMotors"*/){
      debugParserCmd("releaseMotors");
      scanner.releaseMotors();
    }
    else if ( mCommand == "GBA"/*"getBaseAngle"*/){
        debugParserCmd("getBaseAngle");
        return scanner.getBaseAngle();
    }
    else if ( mCommand == "GCP"/*"getCameraPosition"*/){
        debugParserCmd("getCameraPosition");
        return scanner.getCameraPosition();
    }
    else if ( mCommand == "INI"/*"init"*/){
         debugParserCmd("init");
        scanner.init();     
    }
    else {
        Serial.print("No Such Command:");
        Serial.print("\"");
        Serial.print(mCommand);
        Serial.println("\"");
        return err_bad_cmd;
    }
    return NO_VALUE;
     
}

