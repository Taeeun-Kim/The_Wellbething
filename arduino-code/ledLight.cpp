#include "ledLight.h"

/**
 * Constructor
 */
LedLightClass::LedLightClass()
{
}

/**
 * initialise ked strip
 * @return {bool} success
 */
bool LedLightClass::begin()
{
    strip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(ledNum, ledPin);

    strip->Begin();
    strip->Show();
    return true;
}

/**
 * lights up whole strip with one given color
 * @return void
 */
void LedLightClass::showAll(int32_t red, int32_t green, int32_t blue)
{
    for (int8_t i = 0; i < ledNum; i++)
    {
        strip->SetPixelColor(i, RgbColor(red, green, blue));
    }
    strip->Show();
}

/**
 * takes current color toggles between black and color
 */
void LedLightClass::alarm() {
    Neo3Elements::ColorObject color = strip->GetPixelColor(0);
    for (size_t i = 0; i < 5; i++)
    {
        showAll(0,0,0);
        delay(300); 
        showAll(color.R, color.G, color.B);
        delay(300);
    }
    
}

/**
 * calculates red part of hex-color (25 Bit rgb)
 * @return {uint32_t} redColor
 */
uint32_t LedLightClass::hexToRed(uint32_t hex)
{
    return (hex & 0xFF0000) >> 16;
}

/**
 * calculates green part of hex-color (25 Bit rgb)
 * @return {uint32_t} greenColor
 */
uint32_t LedLightClass::hexToGreen(uint32_t hex)
{
    return (hex & 0x00FF00) >> 8;
}

/**
 * calculates blue part of hex-color (25 Bit rgb)
 * @return {uint32_t} blueColor
 */
uint32_t LedLightClass::hexToBlue(uint32_t hex)
{
    return hex & 0x0000FF;
}

LedLightClass LEDS = LedLightClass();