
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
  String str = Serial.readString();
  return str;
}

