# RAK WisMesh Tap Build Status

## Repro

**Commit:** f1d2f06bb1ede1ffd87f9893f51875534ee17ee3

**Command:**
```bash
python3 -m platformio run -e RAK_WisMesh_Tap_companion_radio_ble
```

**Environment:**
- PlatformIO 6.1.18
- Platform: Nordic nRF52 (10.10.0)
- Board: WisCore RAK4631 Board
- Toolchain: toolchain-gccarmnoneeabi @ 1.70201.0 (7.2.1)

**First failing error block:**
```
examples/companion_radio/main.cpp:90:12: fatal error: UITask.h: No such file or directory
   #include "UITask.h"
            ^~~~~~~~~~
compilation terminated.

examples/companion_radio/MyMesh.cpp:831:22: error: 'MAX_GROUP_CHANNELS' was not declared in this scope
     out_frame[i++] = MAX_GROUP_CHANNELS; // v3+
                      ^~~~~~~~~~~~~~~~~~
```

After fixing the above, linker errors appeared:
```
undefined reference to `EnvironmentSensorManager::querySensors(unsigned char, CayenneLPP&)'
undefined reference to `EnvironmentSensorManager::begin()'
undefined reference to `vtable for EnvironmentSensorManager'
```

## Root Cause

1. **Missing include path:** The `UITask.h` header is located in `examples/companion_radio/ui-new/` but the variant's `platformio.ini` did not add this directory to the include path.

2. **Missing macro definition:** `MAX_GROUP_CHANNELS` is required by `MyMesh.cpp` for companion_radio builds but was not defined in the variant's build flags.

3. **Missing source files:** The `helpers/sensors` directory containing `EnvironmentSensorManager.cpp` was not included in the build_src_filter, causing linker errors for sensor-related symbols.

4. **TFT variant issues (RAK_WisMesh_Tap_companion_radio_ble_tft):**
   - `WisMeshTapTFTDisplay.h` used incorrect class name `RAK14014_FT6336U` instead of `FT6336U`
   - `tft_setup.h` referenced `SPI1` which doesn't exist on nRF52; should be `SPI`

## Fix Summary

### platformio.ini changes:
- Added `-I examples/companion_radio/ui-new` to include path for both environments
- Added `-D MAX_GROUP_CHANNELS=40` and related companion_radio flags
- Added `+<helpers/sensors>` to build_src_filter in base configuration

### WisMeshTapTFTDisplay.h:
- Changed `RAK14014_FT6336U touch;` to `FT6336U touch;`

### tft_setup.h:
- Changed `#define TFT_SPI_PORT SPI1` to `#define TFT_SPI_PORT SPI`

## Verification

```bash
# Clean build of both variants
python3 -m platformio run -e RAK_WisMesh_Tap_companion_radio_ble
python3 -m platformio run -e RAK_WisMesh_Tap_companion_radio_ble_tft

# Regression check on related variants
python3 -m platformio run -e RAK_4631_companion_radio_ble
python3 -m platformio run -e RAK_WisMesh_Tag_companion_radio_ble
```

All builds succeeded:
- RAK_WisMesh_Tap_companion_radio_ble: SUCCESS (RAM: 55.0%, Flash: 51.0%)
- RAK_WisMesh_Tap_companion_radio_ble_tft: SUCCESS (RAM: 55.2%, Flash: 54.4%)
- RAK_4631_companion_radio_ble: SUCCESS (no regression)
- RAK_WisMesh_Tag_companion_radio_ble: SUCCESS (no regression)
