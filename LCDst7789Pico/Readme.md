
# LCD ST7789 240x135 display SPI driver

Use the [**Waveshare ST7789**](https://www.waveshare.com/pico-lcd-1.14.htm) Pico SPI format [**240x135 1.14 inch IPS LCD**](https://www.waveshare.com/wiki/Pico-LCD-1.14), connected as [**here**](images/connections.jpg)

Arduino support through the [**Arduino_GFX graphics library**](https://github.com/moononournation/Arduino_GFX) and the [**Earlephilhower Arduino Pico Port**](https://github.com/earlephilhower/arduino-pico/), and the Adafruit GFX library.

**The install order on Windows 10x64**
1. [**Arduino IDE**](https://www.arduino.cc/en/software) version 1.8.19.
2. [**Earlephilhower Aduino Pico port**](https://github.com/earlephilhower/arduino-pico/)
3. [**Adafruit GFX library**](https://github.com/adafruit/Adafruit-GFX-Library).
4. [**Arduino_GFX graphics library**](https://github.com/moononournation/Arduino_GFX).

<p align="left">
<img src="images/lcd1.jpg" width="390" /> 
<img src="images/lcd-clock.jpg" width="235" /> 
<img src="images/lcd-hellogfx.jpg" width="250" /> 
</p>

```
#include <Arduino_GFX_Library.h>
#include <Arduino_GFX_Library.h>

// No MISO pin used for this display
// Display internal 240 x 320 - here 135 x 240 - offset is 240-135=105=52+53 x 320-240=80=40+40 
#define TFT_BL 13
#define GFX_BL 13 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
Arduino_DataBus *bus = new Arduino_RPiPicoSPI(8 /* DC */, 9 /* CS */, 10 /* SCK */, 11 /* MOSI */, -1 /* MISO */, spi1 /* spi */);
Arduino_GFX *gfx = new Arduino_ST7789(bus, 12 /* RST */, 3 /* rotation */, true /* IPS */, 135 /* width */, 240 /* height */, 52 /* col offset 1 */, 40 /* row offset 1 */, 53 /* col offset 2 */, 40 /* row offset 2 */);


```

For the LCD and MCU configuration and examples refer to these pages [**MCU-Databus-Type**](https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class), and [**Display-Type**](https://github.com/moononournation/Arduino_GFX/wiki/Display-Class), and [**Combined-Examples**](https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration).

The [**LILYGO-T-display-RP2040**](https://github.com/Xinyuan-LilyGO/LILYGO-T-display-RP2040) use the same display and use the [**Bodmer TFT_eSPI**](https://github.com/Bodmer/TFT_eSPI) ST7789 Arduino driver.



