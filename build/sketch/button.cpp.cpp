#include <Arduino.h>
#line 36 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
void gyroRuntime();
#line 55 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
void displayWater();
#line 71 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
void displayTemp();
#line 87 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
void displayHum();
#line 102 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
void displayGas();
#line 116 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
void manageModes();
#line 147 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
void manageBtns();
#line 205 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
void setup();
#line 225 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
void loop();
#line 0 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
#line 1 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\button.cpp"
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
#line 1 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
#include <Arduino.h>
#include "tftDisplay.h"
#include "dhtSensor.h"
#include "gyroSensor.h"
#include "gasSensor.h"
#include "button.h"
#include "ledLight.h"
#include "waterCons.h"

// ---------------- VARIABLES ---------------- //
// timing / delay
unsigned long prevMillsGYRO = 0;
const long intervalGYRO = 700;
unsigned long prevMillsMODE = 0;
const long intervalMODE = 700;
unsigned long prevMillsBTN = 0;
const long intervalBTN = 100;

unsigned long prevMillsCLEAR = 0;
const long intervalCLEAR = 1000;
bool screenDirty = true;

/* 0 - drinking 1 - temperature 2 - humidity 3 - gas */
int8_t rotatePosition = 1;
const int8_t modeTemp = 0;
const int8_t modeWater = 1;
const int8_t modeHum = 2;
const int8_t modeGas = 3;
bool posChanged = true;

// ------------------- runtime CHECKS -------------------
/**
 * checks rotation changes
 * triggers and sets new position after change
 */
void gyroRuntime()
{
  GYROSENSOR.gyroRead();
  
  if (GYROSENSOR.GyX > 1.4)
  {
    rotatePosition = ((rotatePosition + 3) % 4);
    posChanged = true;
  }
  else if (GYROSENSOR.GyX < -1.4)
  {
    rotatePosition = ((rotatePosition + 1) % 4);
    posChanged = true;
  }
}

/**
 * interface of consumed water
 */
void displayWater()
{
  uint32_t color = WATERCONS.getWaterRelColor();
  uint32_t colorS = TFTDISPLAY.rgbHexToShort(color);

  LEDS.showAll(LEDS.hexToRed(color), LEDS.hexToGreen(color), LEDS.hexToBlue(color));
  TFTDISPLAY.showText("Wasserkonsum", 23, 45);
  TFTDISPLAY.showText("+", 63, 0);
  TFTDISPLAY.showText("-", 63, 113);
  TFTDISPLAY.showText(String(WATERCONS.getConsumed()) + " ml", 20, 63, 4, colorS);
  TFTDISPLAY.showCircles(WATERCONS.getWaterRelValue(), colorS);
}

/**
 * interface of temperature
 */
void displayTemp()
{
  uint32_t color = DHTSENSOR.getTempRelColor();
  uint32_t colorS = TFTDISPLAY.rgbHexToShort(color);

  LEDS.showAll(LEDS.hexToRed(color), LEDS.hexToGreen(color), LEDS.hexToBlue(color));
  TFTDISPLAY.showText("Temperatur", 29, 45);
  float temp = DHTSENSOR.getTemperature();
  TFTDISPLAY.showText(String(temp) + " 'C", 25, 63, 4, colorS);
  //TFTDISPLAY.showText(String(GYROSENSOR.Tmp) + " 'C", 40, 85);
  TFTDISPLAY.showCircles(DHTSENSOR.getTempRelValue(), colorS);
}

/**
 * interface of humidity
 */
void displayHum()
{
  uint32_t color = DHTSENSOR.getHumRelColor();
  uint32_t colorS = TFTDISPLAY.rgbHexToShort(color);

  LEDS.showAll(LEDS.hexToRed(color), LEDS.hexToGreen(color), LEDS.hexToBlue(color));
  TFTDISPLAY.showText("Luftfeuchtigkeit", 20, 45);
  float hum = DHTSENSOR.getHumidity();
  TFTDISPLAY.showText(String(hum) + " %", 25, 63, 4, colorS);
  TFTDISPLAY.showCircles(DHTSENSOR.getHumRelValue(), colorS);
}

/**
 * interface of gas (CO2)
 */
void displayGas()
{
  uint32_t color = GASSENSOR.getGasRelColor();
  uint32_t colorS = TFTDISPLAY.rgbHexToShort(color);

  LEDS.showAll(LEDS.hexToRed(color), LEDS.hexToGreen(color), LEDS.hexToBlue(color));
  TFTDISPLAY.showText("Luftqualitaet", 25, 45);
  TFTDISPLAY.showText(String(int(round(GASSENSOR.read()))) + " ppm", 17, 63, 4, colorS);
  TFTDISPLAY.showCircles(GASSENSOR.getGasRelValue(), colorS);
}

/**
 * checks which interface to display
 */
void manageModes()
{
  if (posChanged)
  {
    TFTDISPLAY.rotateScreen(rotatePosition);
    TFTDISPLAY.clearScreen();
    posChanged = false;
  }

  switch (rotatePosition)
  {
  case modeTemp:
    displayTemp();
    break;
  case modeWater:
    displayWater();
    break;
  case modeHum:
    displayHum();
    break;
  case modeGas:
    displayGas();
    break;
  default:
    LEDS.showAll(0, 100, 100);
  }
}

/**
 * checks button input and manages action
 */
void manageBtns()
{
  int16_t btnReturn = BTNS.isPressed();

  // small delay before refresh screen to leave a button response on screen
  if (btnReturn != 0) { // no button input
    prevMillsCLEAR = millis();
    screenDirty = true;
  }

  // no button interaction in different modes
  if (rotatePosition != modeWater) return;

  // buttons visualization
  switch (btnReturn)
  {
  case 0: // clear screen
    if (millis() - prevMillsCLEAR >= intervalCLEAR && screenDirty)
    {
      TFTDISPLAY.clearScreen();
      prevMillsMODE = 0;
      screenDirty = false;
    }
    break;
  case 1:
    TFTDISPLAY.showBtnPressCircle(1, rotatePosition, TFT_GREEN, 20);
    break;
  case 2:
    TFTDISPLAY.showBtnPressCircle(2, rotatePosition, TFT_RED, 20);
    break;
  case 3:
    TFTDISPLAY.showBtnPressCircle(1, rotatePosition, TFT_GREEN);
    break;
  case 4:
    TFTDISPLAY.showBtnPressCircle(2, rotatePosition, TFT_RED);
    break;
  case 5:
    TFTDISPLAY.showBtnPressCircle(1, rotatePosition, TFT_GOLD);
    break;
  case 6:
    TFTDISPLAY.showBtnPressCircle(2, rotatePosition, TFT_GOLD);
    break;

  default:
    break;
  }

  // water consumption interact
  if (rotatePosition == modeWater && btnReturn == 3) WATERCONS.addConsumed();
  if (rotatePosition == modeWater && btnReturn == 4) WATERCONS.removeConsumed();

  if (rotatePosition == modeWater && btnReturn == 5) LEDS.alarm();
}

// ------------------- SETUP / LOOP -------------------
/**
 * set up device with all classes and start values
 */
void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up...");

  TFTDISPLAY.begin();
  DHTSENSOR.begin();
  GYROSENSOR.begin();
  GASSENSOR.begin();
  BTNS.begin();
  LEDS.begin();

  delay(1000);

  LEDS.showAll(255, 50, 50);
}

/**
 * loops through all checks and manage methods
 */
void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - prevMillsGYRO >= intervalGYRO)
  {
    prevMillsGYRO = currentMillis;
    gyroRuntime();
  }

  if (currentMillis - prevMillsMODE >= intervalMODE)
  {
    prevMillsMODE = currentMillis;
    manageModes();
  }

  if (currentMillis - prevMillsBTN >= intervalBTN)
  {
    prevMillsBTN = currentMillis;
    manageBtns();
  }
}
