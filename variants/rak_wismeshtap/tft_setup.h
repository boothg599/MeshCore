#pragma once

// Keep all TFT/touch configuration out of variant.h.
// This file is included by target.h when building WisMesh Tap firmware.

#define RAK14014
#define USER_SETUP_LOADED 1
#define DISABLE_ALL_LIBRARY_WARNINGS 1

// Display driver: ST7789 240x320 (RAK14014 TFT module)
#define ST7789_DRIVER 1
#define TFT_WIDTH 240
#define TFT_HEIGHT 320

// WisBlock IO mapping (numeric GPIO values for TFT_eSPI preprocessor)
// WB_SPI_MISO=29, WB_SPI_MOSI=30, WB_SPI_CLK=3, WB_SPI_CS=26
// WB_IO2=34 (power), WB_IO3=21 (backlight), WB_IO4=4 (DC), WB_IO5=9 (RST)
#define TFT_MISO 29
#define TFT_MOSI 30
#define TFT_SCLK 3
#define TFT_CS   26
#define TFT_DC   4
#define TFT_RST  9
#define TFT_BL   21

#define LOAD_GLCD 1
#define LOAD_GFXFF 1
#define TFT_RGB_ORDER 0
#define SPI_FREQUENCY 50000000
#define TFT_SPI_PORT SPI

// Compatibility macro some code checks for
#define ST7789_CS 26

// Touch (RAK14014 uses FT6336U over I2C)
#define HAS_TOUCHSCREEN 1
#define SCREEN_TOUCH_INT 10

// Optional UI behavior knobs (safe defaults)
#define SCREEN_ROTATE
#define SCREEN_TRANSITION_FRAMERATE 5
