
# Adafruit VS1053B and Raspberry Pi Pico Music Player

<p align="left">
<img src="pico-vs1053.jpg" width="600" /> 
<br>

#define CLK 18       //pin24 Pico-GPIO18 to SPI Clock, shared with SD card<br>
#define MISO 16      //pin21 Pico-GPIO16 to Input data, from VS1053/SD card<br>
#define MOSI 19      //pin25 Pico-GPIO19 to Output data, to VS1053/SD card<br>
<br>
// These are the pins used for the breakout example<br>
#define BREAKOUT_RESET  10   //pin14 Pico-GPIO10 to VS1053 reset pin (output)<br>
#define BREAKOUT_CS     17   //pin22 Pico-GPIO17 to VS1053 chip select pin (output)<br>
#define BREAKOUT_DCS    11   //pin15 Pico-GPIO11 to VS1053 Data/command select pin (output)<br>
#define CARDCS 9             //pin12 Pico-GPIO9 to Card chip select pin<br>
#define DREQ 8               //pin11 Pico-GPIO8 to VS1053 Data request ideally an Interrupt pin<br>
<br>
Wire.setSDA(4); //pin6<br>
Wire.setSCL(5); //pin7<br>
SPI.setRX(16);  //pin21<br>
SPI.setTX(19);  //pin25<br>
SPI.setSCK(18); //pin24<br>
SPI.setCS(17);  //pin22<br>
