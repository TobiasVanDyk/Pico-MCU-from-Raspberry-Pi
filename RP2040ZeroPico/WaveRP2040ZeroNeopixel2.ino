// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        16 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 1 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 1500 // Time (in milliseconds) to pause between pixels
byte ArrayRGB[] = {0, 0, 0};
byte ValRGB = 80;

void setup() {

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  pixels.clear();           // Set all pixel colors to 'off'
  for(int i=0; i<3; i++)    // The first NeoPixel in a strand is #0, second is 1
     {ArrayRGB[i] = ValRGB;
      pixels.setPixelColor(0, pixels.Color(ArrayRGB[0], ArrayRGB[1], ArrayRGB[2]));
      pixels.show();        // Send the updated pixel colors to the hardware.
      ArrayRGB[i] = 0;
      delay(DELAYVAL); }      
}
