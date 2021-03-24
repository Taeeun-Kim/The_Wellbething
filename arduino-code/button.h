/**
 * Verkabelung:
 *    5V   10kOhm -- GND
 *   -|------|-
 *    |Button|
 *   -|------|-
 *          PIN (12, 14)
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>

class ButtonClass
{
private:
    const int8_t btnPin1 = 12;
    const int8_t btnPin2 = 14;

    bool btn1Pressed = false;
    unsigned long longPressMillis1 = 0;
    bool btn2Pressed = false;
    unsigned long longPressMillis2 = 0;
    const long longPressTime = 1000;

public:
    ButtonClass();
    bool begin();
    void read();

    int16_t isPressed();
};

extern ButtonClass BTNS;

#endif