
#define CLK 18       //pin24 Pico-GPIO18 to SPI Clock, shared with SD card
#define MISO 16      //pin21 Pico-GPIO16 to Input data, from VS1053/SD card
#define MOSI 19      //pin25 Pico-GPIO19 to Output data, to VS1053/SD card

// These are the pins used for the breakout example
#define BREAKOUT_RESET  10   //pin14 Pico-GPIO10 to VS1053 reset pin (output)
#define BREAKOUT_CS     17   //pin22 Pico-GPIO17 to VS1053 chip select pin (output)
#define BREAKOUT_DCS    11   //pin15 Pico-GPIO11 to VS1053 Data/command select pin (output)
#define CARDCS 9             //pin12 Pico-GPIO9 to Card chip select pin
#define DREQ 8               //pin11 Pico-GPIO8 to VS1053 Data request ideally an Interrupt pin

  Wire.setSDA(4); //pin6
  Wire.setSCL(5); //pin7
  SPI.setRX(16);  //pin21
  SPI.setTX(19);  //pin25
  SPI.setSCK(18); //pin24
  SPI.setCS(17);  //pin22