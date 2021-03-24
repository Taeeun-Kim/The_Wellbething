/**
 * Verkabelung:
 *  DHT  -  +   -   out
 *   |      |   |    |
 * ESP32 - 3v3 GND  15
 */

#ifndef _INFOVIS_DHT_H_
#define _INFOVIS_DHT_H_

#include <Arduino.h>
#include <DHT.h>

class DhtClass
{
private:
    const int dhtType = 22;
    const int dhtPin = 15;
    DHT* dht;

    const int tempMin = -20;
    const int tempMax = 45;
    const int tempCalibrate = -9;

    const int humMin = 0;
    const int humMax = 100;

public:
    DhtClass();
    bool begin();
    float getTemperature();
    float getHumidity();

    int getTempRelValue();
    int32_t getTempRelColor();
    
    int getHumRelValue();
    int32_t getHumRelColor();
};

extern DhtClass DHTSENSOR;

#endif