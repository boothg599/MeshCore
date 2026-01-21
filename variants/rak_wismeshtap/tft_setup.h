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

// WisBlock IO mapping (from variant.h WB_* constants)
#define TFT_MISO WB_SPI_MISO
#define TFT_MOSI WB_SPI_MOSI
#define TFT_SCLK WB_SPI_CLK
#define TFT_CS   WB_SPI_CS
#define TFT_DC   WB_IO4
#define TFT_RST  -1
#define TFT_BL   WB_IO3

#define LOAD_GLCD 1
#define LOAD_GFXFF 1
#define TFT_RGB_ORDER 0
#define SPI_FREQUENCY 50000000
#define TFT_SPI_PORT SPI1

// Compatibility macro some code checks for
#define ST7789_CS WB_SPI_CS

// Touch (RAK14014 uses FT6336U over I2C)
#define HAS_TOUCHSCREEN 1
#define SCREEN_TOUCH_INT WB_IO6

// Optional UI behavior knobs (safe defaults)
#define SCREEN_ROTATE
#define SCREEN_TRANSITION_FRAMERATE 5
