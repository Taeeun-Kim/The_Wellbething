#include "waterCons.h"

/**
 * Constructor
 */
WaterConsClass::WaterConsClass()
{
}

/**
 * add more consumed water
 * @param value {int} added water in ml (milliliters)
 * @return {int} consumed (consumed water after addition)
 */
int WaterConsClass::addConsumed(int value)
{
  consumed += value;
  if (consumed > consMax)
    consumed = consMax;
  return consumed;
}

/**
 * remove from consumed water
 * @param value {int} remove water in ml (milliliters)
 * @return {int} consumed (consumed water after removing)
 */
int WaterConsClass::removeConsumed(int value)
{
  consumed -= value;
  if (consumed < consMin)
    consumed = consMin;
  return consumed;
}

/**
 * get consumed water (in milliliters ml)
 * @return {int} consumed
 */
int WaterConsClass::getConsumed()
{
  return consumed;
}

/**
 * calculates scale position 
 * of consumed water between 0 and 10
 * @return {int} scalePosition
 */
int WaterConsClass::getWaterRelValue()
{
  float relative = 10.0 / float(consMax - consMin);
  relative *= getConsumed() - consMin;
  return round(relative);
}

/**
 * color of consumed water on the scale 
 * @return {int32_t} hexColor (24 Bit rgb)
 */
int32_t WaterConsClass::getWaterRelColor()
{
  switch (getWaterRelValue())
  {
  case 0:
    return 0xFF0A0A;
  case 1:
  case 2:
    return 0xFF9A0A;
  case 3:
  case 4:
    return 0xC1AD0A;
  case 5:
  case 6:
    return 0x87C80A;
  case 7:
  case 8:
    return 0x44E30A;
  case 9:
  case 10:
    return 0x0AFF0A; // Green
  default:
    return 0xFFFFFF; // White
  }
}

WaterConsClass WATERCONS = WaterConsClass();