#include <Wire.h>

#include "SparkFunBME280.h"
BME280 myBMP280;

void setup()
{
  Serial.begin(115200);

  Wire.begin(D5, D6);

  Serial.println("Reading basic values from BME280");
  myBMP280.setI2CAddress(0x76);
  if (myBMP280.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }
}

void loop()
{
  Serial.print(" Pressure: ");
  Serial.print(myBMP280.readFloatPressure()/100, 2);

  Serial.print(" Temp: ");
  Serial.print(myBMP280.readTempC(), 2);

  Serial.println();

  delay(50);
}
