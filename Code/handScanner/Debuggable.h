#ifndef Debuggable_h
#define Debuggable_h

#include <Arduino.h>

class Debuggable
{
protected:
  Stream *pcomm;
public:
  Debuggable(): pcomm(&Serial) {/*nothing*/}
  setCommChan(Stream& comm1) { pcomm = &comm1; }
};

#endif
