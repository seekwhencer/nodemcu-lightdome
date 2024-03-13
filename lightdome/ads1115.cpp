#include "ads1115.h"

void ADS_read(Adafruit_ADS1115 &ads, DynamicJsonDocument &values) {
  values[0] = ADS_readSingle(0, ads);
  values[1] = ADS_readSingle(1, ads);
  values[2] = ADS_readSingle(2, ads);
  values[3] = ADS_readSingle(3, ads);
}

float ADS_readSingle(int port, Adafruit_ADS1115 &ads) {
  float Vin = 3;
  int16_t readValue = ads.readADC_SingleEnded(port);
  float voltage = ads.computeVolts(readValue);
  double resistance = (10.0 * Vin) / voltage - 10.0;
  double illuminance = 255.84 * pow(resistance, -10/9);
  return illuminance;
}


