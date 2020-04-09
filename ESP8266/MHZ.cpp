/* MHZ library

    By Tobias Schürg
*/

#include "MHZ.h"

const int STATUS_INCOMPLETE = -4;

unsigned long lastRequest = 0;

MHZ::MHZ(uint8_t rxpin, uint8_t txpin) {
  SoftwareSerial * ss = new SoftwareSerial(rxpin, txpin);

  ss->begin(9600);
  _serial = ss;
}

/**
 * Enables or disables the debug mode (more logging).
 */
void MHZ::setDebug(boolean enable) {
  debug = enable;
  if (debug) {
    Serial.println(F("MHZ: debug mode ENABLED"));
  } else {
    Serial.println(F("MHZ: debug mode DISABLED"));
  }
}

int MHZ::readCO2UART() {
  if (debug) Serial.println(F("-- read CO2 uart ---"));
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  byte response[9];  // for answer

  if (debug) Serial.print(F("  >> Sending CO2 request"));
  _serial->write(cmd, 9);  // request PPM CO2
  lastRequest = millis();

  // clear the buffer
  memset(response, 0, 9);

  if (_serial->available() > 0) {
    int count;
    count = _serial->readBytes(response, 9);
    while ( ! ((response[0] == 0xff) & (response[1] == 0x86)) ) {
      _serial->flush();
      memset(response, 0, 9);
      delay(100);  // wait a short moment
      _serial->write(cmd, 9);  // request PPM CO2
      delay(100);  // wait a short moment
      count = _serial->readBytes(response, 9);
    }
    delay(500);  // wait a short moment
    byte abc[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};
    _serial->write(abc, 9);  // request ABC off
  } else {
    _serial->flush();
    Serial.println(F(" NG"));
    return STATUS_INCOMPLETE;
  }

  if (debug) {
    // print out the response in hexa
    Serial.print(F("  << "));
    for (int i = 0; i < 9; i++) {
      Serial.print(response[i], HEX);
      Serial.print(F("  "));
    }
    Serial.println(F(""));
  }

  int ppm_uart = 256 * (int)response[2] + response[3];

  temperature = response[4] - 44;  // - 40;

  byte status = response[5];
  if (debug) {
    Serial.print(F(" # PPM UART: "));
    Serial.println(ppm_uart);
    Serial.print(F(" # Temperature? "));
    Serial.println(temperature);
  }

  // Is always 0 for version 14a  and 19b
  // Version 19a?: status != 0x40
  if (debug && status != 0) {
    Serial.print(F(" ! Status maybe not OK ! "));
    Serial.println(status, HEX);
  } else if (debug) {
    Serial.print(F(" Status  OK: "));
    Serial.println(status, HEX);
  }

  _serial->flush();
  return ppm_uart;
}

int MHZ::getLastTemperature() {
  return temperature;
}
