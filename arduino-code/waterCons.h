/**
 * 
 */

#ifndef _WATERC_H_
#define _WATERC_H_

#include <Arduino.h>

class WaterConsClass
{
private:
    int consumed = 0.0;

    const int consAdd = 50;

    const int consMin = 0;
    const int consMax = 2000;

public:
    WaterConsClass();

    int addConsumed(int value = 50);
    int removeConsumed(int value = 50);
    int getConsumed();

    int getWaterRelValue();
    int32_t getWaterRelColor();
};

extern WaterConsClass WATERCONS;

#endif