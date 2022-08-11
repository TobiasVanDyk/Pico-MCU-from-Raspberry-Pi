//////////////////////////////////////////////////////////////////////////////////
// Combination of (1) Raspberry Pi Pico Touch Macro Keyboard by Dustin Watts
//                    https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard
//                (2) AdafruitTinyUSB HID example hid_composite.ino 
//                    https://github.com/adafruit/Adafruit_TinyUSB_Arduino/
//////////////////////////////////////////////////////////////////////////////////
// Adapted by T van Dyk Aug 2022 for ST7789 320x240 and Adafruit TinyUSB stack
//////////////////////////////////////////////////////////////////////////////////

/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!
 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
*********************************************************************/

#include <FS.h>
#include <LittleFS.h>
#include "Adafruit_TinyUSB.h"
#include <SPI.h>
#include <TFT_eSPI.h>

const int pin = 26;
bool activeState = false;
bool has_consumer_key = false;
bool has_key = false;

// Report ID
enum
{
  RID_KEYBOARD = 1,
  //RID_MOUSE,
  RID_CONSUMER_CONTROL, // Media, volume etc ..
};

// HID report descriptor using TinyUSB's template
uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(RID_KEYBOARD) ),
  //TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(RID_MOUSE) ),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(RID_CONSUMER_CONTROL) )
};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);


// Name of the Touch calibration file
#define CALIBRATION_FILE "/TouchCalData2"

// Define our filesystem
#define FILESYSTEM LittleFS

// Set to tru if you want the calibration to run on each boot
#define REPEAT_CAL true

// Was 95 x 4 = 380 + 20 x 5 = 100 + 380 = 480 new 60 x 4 = 240 + 80 = 320 new 65 x 4 = 260 + 12 x 5 = 320
// Was 95 x 3 = 285 + 10 x 4 = 40 + 285 = 325  new 60 x 3 = 180 + 60 = 240
// Keypad start position, key sizes and spacing
#define KEY_W 65 // Key width
#define KEY_H 60 // Key height
#define KEY_SPACING_X 12 // X gap
#define KEY_SPACING_Y 15 // Y gap

#define KEY_X (KEY_W/2) + KEY_SPACING_X // X-axis centre of the first key
#define KEY_Y (KEY_H/2) + KEY_SPACING_Y // Y-axis centre of the first key

#define KEY_TEXTSIZE 2   // Font size multiplier

// Choose the font you are using
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font

// Adding a delay between keypressing to give our OS time to respond
uint8_t keydelay = 100;

// Create the screen object
TFT_eSPI tft = TFT_eSPI();

// Creating the labels for the buttons
// <name>[<number-of-lables>][<number-of-chars-per-label]
// The number of chars per label should include the termination \0.
char keyLabel[12][3] = {"1", "2", "3", "V+", "4", "5", "6", "Vo", "M1", "M2", "M3", "V-"};

// Setting the colour for each button
// You can use the colours defined in TFT_eSPI.h
uint16_t keyColor[12] = {TFT_BLUE, TFT_RED, TFT_GREEN, TFT_NAVY, 
                         TFT_MAROON, TFT_MAGENTA, TFT_ORANGE, TFT_SKYBLUE, 
                         TFT_PURPLE, TFT_CYAN, TFT_PINK, TFT_DARKCYAN
                        };
                        
// Create 12 'keys' to use later
TFT_eSPI_Button key[12];

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB HID Composite");
  usb_hid.begin();
  while( !TinyUSBDevice.mounted() ) delay(1);

  // Initialise the TFT screen
  tft.init();
  tft.setRotation(3);
  touch_calibrate();
  tft.fillScreen(TFT_NAVY);

  // Draw the keys ( 3 times 4 loops to create 12)
  for (uint8_t row = 0; row < 3; row++) {   // 3 rows
    for (uint8_t col = 0; col < 4; col++) { // of 4 buttons
     // The button number is the column we are on added to the row we are on times  the number of buttons per row. 
     // C++ uses the order of operations you use - first the row is multiplied by 4 and then the col is added.
     // The first button is 0. Example: col = 2 (3rd column), row = 1 (2nd row) =>: 1 * 4 = 4 --> 4 + 2 = 6 (7th button).
      uint8_t b = col + row * 4;
      key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel[b], KEY_TEXTSIZE);
      key[b].drawButton();
    }
  }

}

void loop() {

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  
  //delay(keydelay);
  
  if (has_consumer_key) { usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
                          has_consumer_key = false; }
  if (has_key) { usb_hid.keyboardRelease(RID_KEYBOARD);
                 has_key = false; }
                            
  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&t_x, &t_y);
  
  // Check if any key coordinate boxes contain the touch coordinates
  for (uint8_t b = 0; b < 12; b++) {
    if (pressed && key[b].contains(t_x, t_y)) {
      key[b].press(true);  // tell the button it is pressed
    } else {
      key[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 12; b++) {

    if (key[b].justReleased()) key[b].drawButton(); // draw normal again

    if (key[b].justPressed()) 
    {
      key[b].drawButton(true);  // draw invert (background becomes text colour and text becomes background colour
      buttonpress(b);           // Send the button number
    }
  }

}

void buttonpress(int button)
{
  uint8_t keycode[6] = { 0 };
  switch(button){
    case 0:
      keycode[0] = HID_KEY_1;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 1:
      keycode[0] = HID_KEY_2;
      keycode[1] = HID_KEY_ENTER;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 2:
      keycode[0] = HID_KEY_3;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 3:
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT);
      has_consumer_key = true;
      delay(keydelay);
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_MUTE);
      has_consumer_key = true;
      delay(keydelay);
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT);
      has_consumer_key = true;
      delay(keydelay);
      break;
  }
}

void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!FILESYSTEM.begin()) {
    //Serial.println("Formating file system");
    FILESYSTEM.format();
    FILESYSTEM.begin();
  }

  // check if calibration file exists and size is correct
  if (FILESYSTEM.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      FILESYSTEM.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = FILESYSTEM.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = FILESYSTEM.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}
  
