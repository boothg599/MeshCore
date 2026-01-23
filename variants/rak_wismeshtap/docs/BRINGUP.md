# RAK WisMesh Tap Bring-Up Guide

## Environment

**Target Environment:** `RAK_WisMesh_Tap_companion_radio_ble_tft`

**Build Command:**
```bash
python3 -m platformio run -e RAK_WisMesh_Tap_companion_radio_ble_tft
```

**UF2 Output Path:**
```
.pio/build/RAK_WisMesh_Tap_companion_radio_ble_tft/firmware.zip
```

## Hardware Configuration

- **TFT Module:** RAK14014 / RAK14015 (ST7789 240x320)
- **Touch Controller:** FT6336U (I2C)
- **MCU:** nRF52840 (RAK4631)

## Pin Mapping (RAK14014/RAK14015)

| Function | WisBlock Pin | nRF52 GPIO | Notes |
|----------|--------------|------------|-------|
| TFT Power Enable | WB_IO2 | 34 | **Must be HIGH before TFT init** |
| TFT Backlight | WB_IO3 | 21 | HIGH = on |
| TFT DC | WB_IO4 | 4 | Data/Command |
| TFT RST | WB_IO5 | 9 | Reset (active low) |
| Touch INT | WB_IO6 | 10 | Touch interrupt |
| SPI CS | WB_SPI_CS | 26 | Chip select |
| SPI CLK | WB_SPI_CLK | 3 | Clock |
| SPI MOSI | WB_SPI_MOSI | 30 | Data out |
| SPI MISO | WB_SPI_MISO | 29 | Data in |

## Compile-Time Macros

### TFT Control
- `DISPLAY_CLASS=WisMeshTapTFTDisplay` - Enable TFT display driver
- `WISMESH_TAP_USE_TFT=1` - Enable TFT-specific code paths
- `WISMESH_TAP_TFT_BL=21` - Backlight GPIO
- `WISMESH_TAP_TFT_RST=9` - Reset GPIO (was incorrectly -1)
- `WISMESH_TAP_TFT_PWR_EN=34` - Power enable GPIO (NEW)

### BLE Control
- `BLE_PIN_CODE=123456` - Enable BLE with PIN pairing
- `BLE_DEBUG_LOGGING=1` - Enable BLE debug output
- `BLE_NAME_PREFIX="MeshCore-"` - Device name prefix (default)

### Bring-Up Mode
- `BRINGUP_MODE=1` - Enable bring-up diagnostics (off by default)

## Expected Boot Sequence

1. `Serial.begin(115200)` - Serial init
2. `board.begin()` - Board peripherals init
3. **TFT Power Enable** - Set WB_IO2 HIGH
4. **TFT Backlight** - Set WB_IO3 HIGH
5. `display.begin()` - TFT init (SPI, ST7789 controller)
6. Display "Loading..." on screen
7. `radio_init()` - LoRa radio init
8. Filesystem init
9. Mesh init
10. **BLE init** - `serial_interface.begin(device_name, pin_code)`
11. **BLE advertising start** - `startAdv()`
12. Sensors init
13. UI task init

## BRINGUP_MODE

When `BRINGUP_MODE=1` is defined:

1. Early serial logging before each subsystem
2. TFT fills screen with solid blue and prints "BRINGUP OK"
3. BLE advertises as "MeshCoreTap_BRINGUP" unconditionally
4. Continues even if TFT init fails
5. 60-second minimum advertising window

**Pre-built BRINGUP environment:**
```bash
python3 -m platformio run -e RAK_WisMesh_Tap_BRINGUP
```

**Or enable manually in platformio.ini:**
```ini
build_flags =
  ...
  -D BRINGUP_MODE=1
```

**BRINGUP UF2 Output:**
```
.pio/build/RAK_WisMesh_Tap_BRINGUP/firmware.zip
```

## Troubleshooting

### TFT Not Lighting Up
1. Check WB_IO2 (pin 34) is set HIGH - power enable
2. Check WB_IO3 (pin 21) is set HIGH - backlight
3. Verify SPI pins match hardware
4. Check RST pin is correctly configured (pin 9)

### BLE Not Advertising
1. Check `BLE_PIN_CODE` is defined
2. Verify `serial_interface.begin()` is called
3. Check `startAdv()` is reached (not blocked by TFT failure)
4. Use `BLE_DEBUG_LOGGING=1` for debug output

### Device Not Booting
1. Check for hard fault (LED patterns)
2. Enable BRINGUP_MODE for early diagnostics
3. Connect serial at 115200 baud
