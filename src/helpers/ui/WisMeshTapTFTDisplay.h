#pragma once

#include <helpers/ui/DisplayDriver.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#ifdef WISMESH_TAP_TOUCH_ENABLE
  #include <Wire.h>
  #include <RAK14014_FT6336U.h>
#endif

#ifndef WISMESH_TAP_TFT_W
  #define WISMESH_TAP_TFT_W 320
#endif

#ifndef WISMESH_TAP_TFT_H
  #define WISMESH_TAP_TFT_H 240
#endif

#ifndef WISMESH_TAP_TFT_ROTATION
  #define WISMESH_TAP_TFT_ROTATION 1
#endif

#ifndef WISMESH_TAP_TFT_BL_ACTIVE
  #define WISMESH_TAP_TFT_BL_ACTIVE HIGH
#endif

class WisMeshTapTFTDisplay : public DisplayDriver {
  TFT_eSPI display;
  TFT_eSprite buffer;
  bool _isOn = false;
  uint16_t _color = TFT_WHITE;

#ifdef WISMESH_TAP_TOUCH_ENABLE
  FT6336U touch;
  bool _touchReady = false;
#endif

  uint16_t mapColor(Color c) const;

public:
  WisMeshTapTFTDisplay() : DisplayDriver(WISMESH_TAP_TFT_W, WISMESH_TAP_TFT_H), buffer(&display) {}

  bool begin();
  bool isOn() override { return _isOn; }
  void turnOn() override;
  void turnOff() override;
  void clear() override;
  void startFrame(Color bkg = DARK) override;
  void setTextSize(int sz) override;
  void setColor(Color c) override;
  void setCursor(int x, int y) override;
  void print(const char* str) override;
  void fillRect(int x, int y, int w, int h) override;
  void drawRect(int x, int y, int w, int h) override;
  void drawXbm(int x, int y, const uint8_t* bits, int w, int h) override;
  uint16_t getTextWidth(const char* str) override;
  void endFrame() override;

  bool getTouch(int *x, int *y);
};
