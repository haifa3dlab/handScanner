#include "Arduino.h"

#include "StopSensor.h"

StopSensor::StopSensor(byte pin, bool activeLow)
{
  this->pin = pin;
  this->activeLow = activeLow;
}
  
void StopSensor::init()
{
  pinMode(pin, INPUT_PULLUP);
}

bool StopSensor::getState()
{
  bool res = digitalRead(pin);
  if (activeLow)
    return !res;
  else
    return res;
}

