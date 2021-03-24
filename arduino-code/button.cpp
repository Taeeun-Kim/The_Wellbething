#include "button.h"

/**
 * Constructor
 */
ButtonClass::ButtonClass()
{
}

/**
 * initialize Buttons
 * @return {bool} success
 */
bool ButtonClass::begin()
{
    pinMode(btnPin1, INPUT);
    pinMode(btnPin2, INPUT);
    return true;
}

/**
 * Serial print which button is pressed
 */
void ButtonClass::read()
{
    int btnnState1 = digitalRead(btnPin1);
    if (btnnState1 == HIGH)
    {
        Serial.println("Button1 gedrückt");
    }
    int btnnState2 = digitalRead(btnPin2);
    if (btnnState2 == HIGH)
    {
        Serial.println("Button2 gedrückt");
    }
}

/**
 * 0 - no btn pressed
 * 1/2 - btn 1/2 currently pressed
 * 3/4 - btn 1/2 pressed once
 * 5/6 - btn 1/2 pressed long
 * 
 * @return int8_t 
 */
int16_t ButtonClass::isPressed()
{
    // BUTTON 1
    int btnState1 = digitalRead(btnPin1);
    if(btnState1 == HIGH && !btn1Pressed) {
        btn1Pressed = true;
        longPressMillis1 = millis();
    } else if(btnState1 == LOW && btn1Pressed) {
        btn1Pressed = false;
        if(millis() - longPressMillis1 >= longPressTime) {
            return 5;
        } else {
            return 3;
        }
    }

    // BUTTON 2
    int btnState2 = digitalRead(btnPin2);
    if(btnState2 == HIGH && !btn2Pressed) {
        btn2Pressed = true;
        longPressMillis2 = millis();
    } else if(btnState2 == LOW && btn2Pressed) {
        btn2Pressed = false;
        if(millis() - longPressMillis2 >= longPressTime) {
            return 6;
        } else {
            return 4;
        }
    }

    if(btn1Pressed == HIGH) return 1;
    if(btn2Pressed == HIGH) return 2;

    return 0;
}

ButtonClass BTNS = ButtonClass();