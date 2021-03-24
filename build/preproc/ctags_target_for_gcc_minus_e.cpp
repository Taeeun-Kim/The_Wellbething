# 1 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\button.cpp"
# 2 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\button.cpp" 2

/**

 * Constructor

 */
# 6 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\button.cpp"
ButtonClass::ButtonClass()
{
}

/**

 * initialize Buttons

 * @return {bool} success

 */
# 14 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\button.cpp"
bool ButtonClass::begin()
{
    pinMode(btnPin1, 0x01);
    pinMode(btnPin2, 0x01);
    return true;
}

/**

 * Serial print which button is pressed

 */
# 24 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\button.cpp"
void ButtonClass::read()
{
    int btnnState1 = digitalRead(btnPin1);
    if (btnnState1 == 0x1)
    {
        Serial.println("Button1 gedrückt");
    }
    int btnnState2 = digitalRead(btnPin2);
    if (btnnState2 == 0x1)
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
# 46 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\button.cpp"
int16_t ButtonClass::isPressed()
{
    // BUTTON 1
    int btnState1 = digitalRead(btnPin1);
    if(btnState1 == 0x1 && !btn1Pressed) {
        btn1Pressed = true;
        longPressMillis1 = millis();
    } else if(btnState1 == 0x0 && btn1Pressed) {
        btn1Pressed = false;
        if(millis() - longPressMillis1 >= longPressTime) {
            return 5;
        } else {
            return 3;
        }
    }

    // BUTTON 2
    int btnState2 = digitalRead(btnPin2);
    if(btnState2 == 0x1 && !btn2Pressed) {
        btn2Pressed = true;
        longPressMillis2 = millis();
    } else if(btnState2 == 0x0 && btn2Pressed) {
        btn2Pressed = false;
        if(millis() - longPressMillis2 >= longPressTime) {
            return 6;
        } else {
            return 4;
        }
    }

    if(btn1Pressed == 0x1) return 1;
    if(btn2Pressed == 0x1) return 2;

    return 0;
}

ButtonClass BTNS = ButtonClass();
# 1 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
# 2 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino" 2
# 3 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino" 2
# 4 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino" 2
# 5 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino" 2
# 6 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino" 2
# 7 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino" 2
# 8 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino" 2
# 9 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino" 2

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
# 36 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
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
# 55 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
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
# 71 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
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
# 87 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
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
# 102 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
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
# 116 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
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
# 147 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
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
    TFTDISPLAY.showBtnPressCircle(1, rotatePosition, 0x07E0 /*   0, 255,   0 */, 20);
    break;
  case 2:
    TFTDISPLAY.showBtnPressCircle(2, rotatePosition, 0xF800 /* 255,   0,   0 */, 20);
    break;
  case 3:
    TFTDISPLAY.showBtnPressCircle(1, rotatePosition, 0x07E0 /*   0, 255,   0 */);
    break;
  case 4:
    TFTDISPLAY.showBtnPressCircle(2, rotatePosition, 0xF800 /* 255,   0,   0 */);
    break;
  case 5:
    TFTDISPLAY.showBtnPressCircle(1, rotatePosition, 0xFEA0 /* 255, 215,   0 */);
    break;
  case 6:
    TFTDISPLAY.showBtnPressCircle(2, rotatePosition, 0xFEA0 /* 255, 215,   0 */);
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
# 205 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
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
# 225 "c:\\Users\\rumpe\\CloudStation\\Drive\\UNI_5\\InfoVis\\infovis-2021-gruppe-1\\arduino-code\\main.ino"
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
