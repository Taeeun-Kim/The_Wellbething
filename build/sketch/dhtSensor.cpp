#include "dhtSensor.h"

/**
 * Constructor
 */
DhtClass::DhtClass()
{
}

/**
 * initialise dht-Sensor
 * @return {bool} success
 */
bool DhtClass::begin()
{
  dht = new DHT(dhtPin, dhtType, 11);
  dht->begin();

  return true;
}

/**
 * reading temperature from sensor
 * @return {float} temperature
 */
float DhtClass::getTemperature()
{
  return (dht->readTemperature() + tempCalibrate);
}

/**
 * reading humidity from sensor
 * @return {float} humidity
 */
float DhtClass::getHumidity()
{
  return dht->readHumidity();
}

/**
 * calculates scale position 
 * of temperature between 0 and 10
 * @return {int} scalePosition
 */
int DhtClass::getTempRelValue()
{
  float relative = 10.0 / float(tempMax - tempMin);
  relative *= getTemperature() - tempMin;
  return round(relative);
}

/**
 * color of temperature on the scale 
 * @return {int32_t} hexColor
 */
int32_t DhtClass::getTempRelColor()
{
  switch (getTempRelValue())
  {
  case 0:
    return 0x18FFB7;
  case 1:
  case 2:
    return 0x9FFFB7;
  case 3:
  case 4:
    return 0xFFFFB7;
  case 5:
  case 6:
    return 0xFFB71D;
  case 7:
  case 8:
    return 0xFF600A;
  case 9:
  case 10:
    return 0xFF0A0A;
  default:
    return 0xFFFFFF; // white
  }
}

/**
 * calculates scale position 
 * of humidity between 0 and 10
 * @return {int} scalePosition
 */
int DhtClass::getHumRelValue()
{
  float relative = 10.0 / float(humMax - humMin);
  relative *= getHumidity() - humMin;
  return round(relative);
}

/**
 * color of humidity on the scale 
 * @return {int32_t} hexColor
 */
int32_t DhtClass::getHumRelColor()
{
  switch (getHumRelValue())
  {
  case 0:
    return 0x0AFAFF;
  case 1:
  case 2:
    return 0x0ACAFF;
  case 3:
  case 4:
    return 0x0A9AFF;
  case 5:
  case 6:
    return 0x0A6AFF;
  case 7:
  case 8:
    return 0x0A30FF;
  case 9:
  case 10:
    return 0x0A0AFF;
  default:
    return 0xFFFFFF; // white
  }
}

DhtClass DHTSENSOR = DhtClass();