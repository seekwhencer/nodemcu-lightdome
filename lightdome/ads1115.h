#include <Adafruit_ADS1X15.h>
#include "config.h"

void ADS_read(Adafruit_ADS1115 &ads, DynamicJsonDocument &values);
float ADS_readSingle(int port, Adafruit_ADS1115 &ads);