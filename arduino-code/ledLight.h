/**
 * Verkabelung:
 *  5V  DIN  GND
 *  |    |    |
 *  5V   32  GND
 */

#ifndef _LED_LIGHT_H_
#define _LED_LIGHT_H_

#include <Arduino.h>
#include <NeoPixelBus.h>

class LedLightClass
{
private:
    const int8_t ledPin = 32;
    const int8_t ledNum = 8;
    
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip;

public:
    LedLightClass();
    bool begin();
    void alarm();
    void showAll(int32_t red, int32_t green, int32_t blue);

    uint32_t hexToRed(uint32_t hex);
    uint32_t hexToGreen(uint32_t hex);
    uint32_t hexToBlue(uint32_t hex);
};

extern LedLightClass LEDS;

#endif