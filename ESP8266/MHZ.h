/* MHZ library

    By Tobias Schürg
*/
#ifndef MHZ_H
#define MHZ_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SoftwareSerial.h>

// status codes
extern const int STATUS_INCOMPLETE;

class MHZ {
 public:
  MHZ(uint8_t rxpin, uint8_t txpin);

  void setDebug(boolean enable);

  int readCO2UART();
  int getLastTemperature();

 private:
  uint8_t temperature;
  boolean debug = false;

  Stream * _serial;
};

#endif
