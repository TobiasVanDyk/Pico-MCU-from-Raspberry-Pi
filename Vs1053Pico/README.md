
# Adafruit VS1053B and Raspberry Pi Pico Music Player

Used the [**Earle F. Philhower III Arduino Port**](https://github.com/earlephilhower/arduino-pico/), and three August 2022 versions of the Adafruit GFX, SSD1306, and VS1053 libraries.

<p align="left">
<img src="images/pico-vs1053.jpg" width="700" /> 
</p>

```
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels  
#define OLED_RESET     -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CLK 18       // pin24 Pico-GPIO18 to SPI Clock, shared with SD card
#define MISO 16      // pin21 Pico-GPIO16 to Input data, from VS1053/SD card
#define MOSI 19      // pin25 Pico-GPIO19 to Output data, to VS1053/SD card

// These are the pins used for the Adafruit VS1053
#define BREAKOUT_RESET  10   // pin14 Pico-GPIO10 to VS1053 reset pin (output)
#define BREAKOUT_CS     17   // pin22 Pico-GPIO17 to VS1053 chip select pin (output)
#define BREAKOUT_DCS    11   // pin15 Pico-GPIO11 to VS1053 Data/command select pin (output)
#define CARDCS          9    // pin12 Pico-GPIO9 to Card chip select pin
#define DREQ            8    // pin11 Pico-GPIO8 to VS1053 Data request ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

void setup() {
 
  Wire.setSDA(4); //pin6
  Wire.setSCL(5); //pin7
  SPI.setRX(16);  //pin21
  SPI.setTX(19);  //pin25
  SPI.setSCK(18); //pin24
  SPI.setCS(17);  //pin22
```

<p align="left">
<img src="images/adafruitlibs.jpg" width="350" /> 
<img src="images/earlephillhower.jpg" width="350" /> 
</p> 
