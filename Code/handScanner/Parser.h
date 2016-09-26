#ifndef Parser_h
#define Parser_h

#include <Arduino.h>
#include "ScannerCtrl.h"

#define NO_VALUE 800
/*
 * The Parser class recieve a seriel input parse it and call the desired 
 * Scanner command with the right parametrs
 * Example:     "baseTurn 60"  ==>   scanner.baseTurn(60)
 *              "doFullScan"   ==>   scanner.doFullScan()
 *              
 * GetString ==> find command ==> find parameters
 */
 
 /*
  *  Message Structure:
  *  "#commandName param"
  *   the '#' sign indicate a message is coming
  *   commandName is an essetnail field
  *   param is optional depend on the different command
  */

const bool DEBUG_PARSER = true;
class Parser{
  public:
  Parser(){;}
  bool Listen(int timeOut = 5);  //polls bytes for the # sign and calls readStringInput
  uint32_t callCommand(Scanner &scanner);

  private:
  void debugParserCmd(String cmdName);

  String readStringInput();
  String mCommand;
  String mParam;
};



#endif
