#ifndef StopSensor_h
#define StopSensor_h

class StopSensor
{
public:
  StopSensor(byte pin, bool activeLow = true);
  void init();
  bool getState();

protected:
  bool activeLow;
  byte pin;
};

#endif


