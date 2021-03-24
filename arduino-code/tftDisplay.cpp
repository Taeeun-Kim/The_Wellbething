#include "tftDisplay.h"

/**
 * Constructor
 */
TftClass::TftClass() {}

/**
 * initialise display
 * @return {bool} success
 */
bool TftClass::begin()
{
  tft = new TFT_eSPI();
  tft->init();

  tft->setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  showText("Start...");

  rotateScreen(1);
  clearScreen();

  return true;
}

/**
 * clear whole screen with default layout
 * @return void
 */
void TftClass::clearScreen()
{
  tft->fillScreen(TFT_BLACK);

  // -- circle line around display -- //
  //tft->fillCircle(offsetX + 64, offsetY + 64, 65, TFT_DARKGREY);
  tft->fillCircle(offsetX + 64, offsetY + 64, 64, TFT_BLACK);
}

/**
 * rotates screen to given direction
 * @param displayPos {int8_t} (0/1/2/3)
 * @return void
 */
void TftClass::rotateScreen(int8_t displayPos)
{
  tft->setRotation(displayPos);
  switch (displayPos)
  {
  case 0:
    offsetX = 0;
    offsetY = 26;
    break;
  case 1:
    offsetX = 26;
    offsetY = 0;
    break;
  case 2:
    offsetX = 0;
    offsetY = 6;
    break;
  case 3:
    offsetX = 6;
    offsetY = 0;
    break;
  default:
    offsetX = 0;
    offsetY = 0;
    break;
  }
}

/**
 * prints string
 * @param text {String} printed string
 * @param posX {int32_t} x-pos
 * @param posY {int32_t} y-pos
 * @param font {int16_t} font-nr (2 (default), 4, 6, 7, 8)
 * @param color {uint32_t} color of font (16 Bit rgb)
 * @return void
 */
void TftClass::showText(String text, int32_t posX, int32_t posY, int16_t font, uint32_t color)
{
  tft->setTextColor(color, TFT_BLACK);
  tft->drawString(text, offsetX + posX, offsetY + posY, font);
}

/**
 * displays half circle at the position of the pressed button
 * calculated with btnNr and rotation
 * @param btnNr {int8_t} button (1/2)
 * @param rotation {uint8_t} (0/1/2/3)
 * @param color {uint32_t} color of circle (16 Bit rgb)
 * @param radius {int32_t} circle radius (>15)
 * @return void
 */
void TftClass::showBtnPressCircle(int8_t btnNr, uint8_t rotation, uint32_t color, int32_t radius)
{
  if (rotation >= 2)
    btnNr = (btnNr == 1) ? 2 : 1;

  if ((rotation % 2) == 1)
  {
    if (btnNr == 1)
    {
      tft->fillCircle(offsetX + 64, offsetY - 15, radius, color);
    }
    else if (btnNr == 2)
    {
      tft->fillCircle(offsetX + 64, offsetY + 143, radius, color);
    }
  }
  else
  {
    if (btnNr == 1)
    {
      tft->fillCircle(offsetX + 143, offsetY + 64, radius, color);
    }
    else if (btnNr == 2)
    {
      tft->fillCircle(offsetX - 15, offsetY + 64, radius, color);
    }
  }
}

/**
 * shows circle scale with up to 10 circles
 * @param count {int32_t} circle number (up to 10)
 * @param color {uint32_t} color of the circles (16 Bit rgb)
 */
void TftClass::showCircles(int32_t count, uint32_t color)
{
  switch (count)
  {
  case 10:
    tft->fillCircle(offsetX + 46, offsetY + 10, 2, color);
  case 9:
    tft->fillCircle(offsetX + 18, offsetY + 30, 2, color);
  case 8:
    tft->fillCircle(offsetX + 9, offsetY + 63, 2, color);
  case 7:
    tft->fillCircle(offsetX + 18, offsetY + 96, 2, color);
  case 6:
    tft->fillCircle(offsetX + 46, offsetY + 115, 2, color);
  case 5:
    tft->fillCircle(offsetX + 84, offsetY + 115, 2, color);
  case 4:
    tft->fillCircle(offsetX + 109, offsetY + 96, 2, color);
  case 3:
    tft->fillCircle(offsetX + 119, offsetY + 63, 2, color);
  case 2:
    tft->fillCircle(offsetX + 109, offsetY + 30, 2, color);
  case 1:
    tft->fillCircle(offsetX + 84, offsetY + 10, 2, color);
  }
  
  switch (count)
  {
  case 0:
    tft->fillCircle(offsetX + 84, offsetY + 10, 2, TFT_BLACK);
  case 1:
    tft->fillCircle(offsetX + 109, offsetY + 30, 2, TFT_BLACK);
  case 2:
    tft->fillCircle(offsetX + 119, offsetY + 63, 2, TFT_BLACK);
  case 3:
    tft->fillCircle(offsetX + 109, offsetY + 96, 2, TFT_BLACK);
  case 4:
    tft->fillCircle(offsetX + 84, offsetY + 115, 2, TFT_BLACK);
  case 5:
    tft->fillCircle(offsetX + 46, offsetY + 115, 2, TFT_BLACK);
  case 6:
    tft->fillCircle(offsetX + 18, offsetY + 96, 2, TFT_BLACK);
  case 7:
    tft->fillCircle(offsetX + 9, offsetY + 63, 2, TFT_BLACK);
  case 8:
    tft->fillCircle(offsetX + 18, offsetY + 30, 2, TFT_BLACK);
  case 9:
    tft->fillCircle(offsetX + 46, offsetY + 10, 2, TFT_BLACK);
  }
}

/**
 * transform 24 Bit rgb Hex to 16 Bit rgb Hex
 * @param hex {uint32_t} color with 24 Bit hex format RGB
 * @return {uint32_t} hexColor (16 Bit rgb)
 */
uint32_t TftClass::rgbHexToShort(uint32_t hex)
{
  unsigned r = (hex & 0xFF0000) >> 16;
  unsigned g = (hex & 0x00FF00) >> 8;
  unsigned b = hex & 0x0000FF;

  r = (r * 31 / 255);
  g = (g * 63 / 255);
  b = (b * 31 / 255);

  r = r << 11;
  g = g << 5;

  return (r | g | b);
}

TftClass TFTDISPLAY = TftClass();