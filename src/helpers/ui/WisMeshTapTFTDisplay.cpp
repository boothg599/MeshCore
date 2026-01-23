#include "WisMeshTapTFTDisplay.h"

#ifndef WISMESH_TAP_TFT_BL
  #define WISMESH_TAP_TFT_BL (-1)
#endif

#ifndef WISMESH_TAP_TFT_PWR_EN
  #define WISMESH_TAP_TFT_PWR_EN (-1)
#endif

#ifndef WISMESH_TAP_TFT_RST
  #define WISMESH_TAP_TFT_RST (-1)
#endif

uint16_t WisMeshTapTFTDisplay::mapColor(Color c) const {
  switch (c) {
    case DARK:
      return TFT_BLACK;
    case LIGHT:
      return TFT_WHITE;
    case RED:
      return TFT_RED;
    case GREEN:
      return TFT_GREEN;
    case BLUE:
      return TFT_BLUE;
    case YELLOW:
      return TFT_YELLOW;
    case ORANGE:
      return TFT_ORANGE;
    default:
      return TFT_WHITE;
  }
}

bool WisMeshTapTFTDisplay::begin() {
  if (_isOn) {
    return true;
  }

  // Step 1: Enable TFT power (WB_IO2 on RAK14014/RAK14015)
  if (WISMESH_TAP_TFT_PWR_EN >= 0) {
    pinMode(WISMESH_TAP_TFT_PWR_EN, OUTPUT);
    digitalWrite(WISMESH_TAP_TFT_PWR_EN, HIGH);
    delay(10); // Allow power to stabilize
  }

  // Step 2: Hardware reset if RST pin is configured
  if (WISMESH_TAP_TFT_RST >= 0) {
    pinMode(WISMESH_TAP_TFT_RST, OUTPUT);
    digitalWrite(WISMESH_TAP_TFT_RST, LOW);
    delay(10);
    digitalWrite(WISMESH_TAP_TFT_RST, HIGH);
    delay(120); // ST7789 needs 120ms after reset
  }

  // Step 3: Enable backlight
  if (WISMESH_TAP_TFT_BL >= 0) {
    pinMode(WISMESH_TAP_TFT_BL, OUTPUT);
    digitalWrite(WISMESH_TAP_TFT_BL, WISMESH_TAP_TFT_BL_ACTIVE);
  }

  // Step 4: Initialize TFT controller
  SPI.begin();
  display.init();
  display.setRotation(WISMESH_TAP_TFT_ROTATION);
  display.fillScreen(TFT_BLACK);
  display.setTextColor(TFT_WHITE);

  buffer.setColorDepth(16);
  buffer.createSprite(width(), height());
  buffer.setTextColor(TFT_WHITE);
  buffer.fillScreen(TFT_BLACK);

#ifdef WISMESH_TAP_TOUCH_ENABLE
  Wire.begin();
  if (WISMESH_TAP_TOUCH_INT >= 0) {
    pinMode(WISMESH_TAP_TOUCH_INT, INPUT_PULLUP);
  }
  touch.begin();
  _touchReady = true;
#endif

  _isOn = true;
  return true;
}

void WisMeshTapTFTDisplay::turnOn() {
  WisMeshTapTFTDisplay::begin();
}

void WisMeshTapTFTDisplay::turnOff() {
  if (_isOn) {
    if (WISMESH_TAP_TFT_BL >= 0) {
      digitalWrite(WISMESH_TAP_TFT_BL, !WISMESH_TAP_TFT_BL_ACTIVE);
    }
    _isOn = false;
  }
}

void WisMeshTapTFTDisplay::clear() {
  buffer.fillScreen(TFT_BLACK);
}

void WisMeshTapTFTDisplay::startFrame(Color bkg) {
  buffer.fillScreen(mapColor(bkg));
  buffer.setTextColor(TFT_WHITE);
}

void WisMeshTapTFTDisplay::setTextSize(int sz) {
  buffer.setTextSize(sz);
}

void WisMeshTapTFTDisplay::setColor(Color c) {
  _color = mapColor(c);
  buffer.setTextColor(_color);
}

void WisMeshTapTFTDisplay::setCursor(int x, int y) {
  buffer.setCursor(x, y);
}

void WisMeshTapTFTDisplay::print(const char* str) {
  buffer.print(str);
}

void WisMeshTapTFTDisplay::fillRect(int x, int y, int w, int h) {
  buffer.fillRect(x, y, w, h, _color);
}

void WisMeshTapTFTDisplay::drawRect(int x, int y, int w, int h) {
  buffer.drawRect(x, y, w, h, _color);
}

void WisMeshTapTFTDisplay::drawXbm(int x, int y, const uint8_t* bits, int w, int h) {
  buffer.drawXBitmap(x, y, bits, w, h, _color);
}

uint16_t WisMeshTapTFTDisplay::getTextWidth(const char* str) {
  return buffer.textWidth(str);
}

void WisMeshTapTFTDisplay::endFrame() {
  buffer.pushSprite(0, 0);
}

bool WisMeshTapTFTDisplay::getTouch(int *x, int *y) {
#ifdef WISMESH_TAP_TOUCH_ENABLE
  if (!_touchReady || x == nullptr || y == nullptr) {
    return false;
  }
  (void)x;
  (void)y;
#endif
  return false;
}
