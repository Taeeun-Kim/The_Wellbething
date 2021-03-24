/**
 * Verkabelung:
 * Display - LED  SCK  SDA  A0  RESET  CS  GND  VCC
 *    |        |    |    |   |    |     |    |    |
 * ESP32   - 5V   18   23   2    0     4   GND  3v3
 * 
 * 160 x 128 Pixel
 */

#ifndef _INFOVIS_DISPLAY_H_
#define _INFOVIS_DISPLAY_H_

#include <Arduino.h>
#include "TFT_eSPI.h"
#include "SPI.h"

class TftClass
{
private:
    TFT_eSPI *tft;
    // offsets to center all elements at round (square)
    int32_t offsetX = 0;
    int32_t offsetY = 0;

public:
    TftClass();
    bool begin();
    void clearScreen();
    void rotateScreen(int8_t displayPos); // 0-3
    
    void showText(String text, int32_t posX = 0, int32_t posY = 0, int16_t font = 2, uint32_t color = TFT_WHITE);
    void showBtnPressCircle(int8_t btnNr, uint8_t rotation = 1, uint32_t color = TFT_WHITE, int32_t radius = 28);
    void showCircles(int32_t count, uint32_t color = TFT_BLUE);

    uint32_t rgbHexToShort(uint32_t hex);
};

extern TftClass TFTDISPLAY;

#endif