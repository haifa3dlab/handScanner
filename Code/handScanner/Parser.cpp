
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
  return pcomm->readStringUntil(' ');
}

/*
 * Description: looping till the # is received (to infinity if not received)
 * 
 * 
 * 
 */
bool Parser::Listen(Stream& st, int timeOut) {
  setCommChan(st);
  int incomingByte = 555;
  mCommand = "NULL";
  mParam = "NULL";
  int timeOutCnt = timeOut;
  while (incomingByte != '#'){
    if (pcomm->available() > 0) {
      incomingByte = pcomm->read();
    }
    if (timeOutCnt-- <= 0 ){
      //if ( DEBUG_PARSER ) { pcomm->println("TimeOut Occured (in cmd read)"); }
      return false;
    }
  }
  mCommand = readStringInput();
  mCommand.trim();

  timeOutCnt = timeOut;
  while (pcomm->available() < 0)
  {
    if (timeOutCnt-- <= 0 ){
      if ( DEBUG_PARSER ) { pcomm->println(F("*TimeOut Occur (in param read)*")); }
      return false;
    }
  }
  mParam = readStringInput();
  mParam.trim();

  // debug
  if ( DEBUG_PARSER ) {
    pcomm->print(F("\nCommand Received: "));
    pcomm->print(mCommand);
    pcomm->print(F("\nParameter Received: "));
    pcomm->println(mParam);
  }
  return true;
}

void Parser::debugParserCmd(String cmdName)
{
  if ( !(DEBUG_PARSER) ) return;
  
  pcomm->print(cmdName);
  pcomm->print(F(" ("));
  pcomm->print(mCommand);
  pcomm->print(F(") "));
  pcomm->print(mParam);
  pcomm->print(F(" (int: "));
  pcomm->print(mParam.toInt(), DEC);
  pcomm->println(F(")"));
}

/* Description: calls the scanner command
 * 
 * @param: uses the mCommand and mParam member variable
 * @return: return uint32_t for methods "getBaseAngle" and "getCameraPosition"
 * 
 * example commands: "#SAS 100# #SHS 50# #DFS 1#"
 */
uint32_t Parser::callCommand(Scanner &scanner)
{
    scanner.setCommChan(*pcomm);
    
    if ( mParam.length() <= 0 || mParam.equals("NULL") ) {
      pcomm->println(F("*NULL param*"));
      return err_bad_param;
    }

    if ( mCommand == "BT"/*"baseTurn"*/){
        debugParserCmd("baseTurn");  
        scanner.baseTurn(mParam.toInt());
    }
    else if ( mCommand == "BTR"/*"baseTurnRel"*/){
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
        pcomm->print(F("No Such Command:"));
        pcomm->print("\"");
        pcomm->print(mCommand);
        pcomm->println("\"");
        return err_bad_cmd;
    }
    return NO_VALUE;
     
}

