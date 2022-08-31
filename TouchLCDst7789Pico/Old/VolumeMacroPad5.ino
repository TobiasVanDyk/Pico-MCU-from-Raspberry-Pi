
///////////////////////////////////////////////////////////////////////////////////////////
// Combination of (1) Raspberry Pi Pico Touch Macro Keyboard by Dustin Watts
//                    https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard
//                (2) AdafruitTinyUSB HID examples hid_composite.ino hid_boot_keyboard.ino
//                    https://github.com/adafruit/Adafruit_TinyUSB_Arduino/
//                (3) Bodmer-Adafruit Keypad Keypad_240x320.ino
//                    https://github.com/Bodmer/TFT_eSPI/examples/
///////////////////////////////////////////////////////////////////////////////////////////
// Adapted by T van Dyk Aug 2022 for ST7789 320x240 and Adafruit TinyUSB stack
///////////////////////////////////////////////////////////////////////////////////////////

/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!
 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
*********************************************************************/

#include "Adafruit_TinyUSB.h"
#include <SPI.h>
#include <TFT_eSPI.h>

int LCDBackLight = 13; // Will be set as OUTPU and HIGH by TFT init
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

// USB HID object. For ESP32 these values cannot be changed after this declaration
// desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

// Used to dim LCD Backlight if 60 seconds no touch
unsigned long NowMillis;
unsigned long LastMillis;
unsigned long TimePeriod = 60000;
bool BackLightOn = true;

uint16_t calData[5] = {608, 3138, 471, 3158, 5};
//uint16_t calData[5];
// Replace above line with the number on display once calibration is complete

// 65 x 4 = 260 + 12 x 5 = 320
// 60 x 3 = 180 + 15 x 4 = 240
// Key sizes and spacing
#define KEY_W 65 // Key width
#define KEY_H 60 // Key height
#define KEY_SPACING_X 12 // X gap
#define KEY_SPACING_Y 15 // Y gap
// Keypad start position
#define KEY_X (KEY_W/2) + KEY_SPACING_X   // 65/2 + 12 = 45 X-axis centre first key
#define KEY_Y (KEY_H/2) + KEY_SPACING_Y   // 60/2 + 15 = 45 Y-axis centre first key

#define KEY_TEXTSIZE 2   // Font size multiplier

// Adding a delay between keypressing to give our OS time to respond
uint8_t keydelay = 50; // 100;
uint8_t keydelay2 = 15;

// Create the screen object
TFT_eSPI tft = TFT_eSPI();

// Creating the labels for the buttons
// <name>[<number-of-lables>][<number-of-chars-per-label]
// The number of chars per label should include the termination \0.
char keyLabel[12][3] = {"1", "2", "3", "V+", "4", "5", "6", "Vo", "M1", "M2", "M3", "V-"};

// Setting the colour for each button - use the colours defined in TFT_eSPI.h
uint16_t keyColor[12] = {TFT_BLUE, TFT_RED, TFT_DARKGREEN, TFT_NAVY, 
                         TFT_MAROON, TFT_MAGENTA, TFT_ORANGE, TFT_SKYBLUE, 
                         TFT_PURPLE, TFT_LIGHTGREY, TFT_GOLD, TFT_DARKCYAN
                        };
                        
// Create 12 'keys' to use later
TFT_eSPI_Button key[12];

void setup() {

  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);   // Used for Capslock
  digitalWrite(LED_BUILTIN, LOW);
  
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB HID Composite");
  // Set up output report (on control endpoint) for Capslock indicator
  usb_hid.setReportCallback(NULL, hid_report_callback);
  usb_hid.begin();
  while( !TinyUSBDevice.mounted() ) delay(1);

  // Initialise the TFT screen TFT_eSPI/TFT_eSPI.h
  tft.init();
  tft.setRotation(3);    // void setRotation(uint8_t r); Set the display orientation to 0, 1, 2 or 3
  tft.setTextFont(1);
  tft.setTextSize(KEY_TEXTSIZE);
    
  BackLightOn = true;     // TFT init will turn it on
  
  /*
  touch_calibrate();
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  for (uint8_t i = 0; i < 5; i++)
  { tft.print(calData[i]);
    if (i < 4) tft.print(", ");
  }
  delay(8000);
  */

  tft.setTouch(calData);  // void setTouch(uint16_t *data); // TFT_eSPI/Extensions/Touch.h 
  ConfigButtons();        // Draw Buttons and Labels
  LastMillis = millis();  //initial start time
}

void loop() {

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
    
  NowMillis = millis();   // get the current "time" (number of milliseconds since started)
  if ((NowMillis - LastMillis) >= TimePeriod)  //test whether the period has elapsed
  {
    digitalWrite(LCDBackLight, LOW);   // Backlight Off
    //analogWrite(LCDBackLight, 128);  // Backlight Dimmed
    LastMillis = NowMillis;            // Start Tiemer again
    BackLightOn = false;               // Until first keypress
  }
    
  if (has_consumer_key) { usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
                          has_consumer_key = false; }
  if (has_key) { usb_hid.keyboardRelease(RID_KEYBOARD);
                 has_key = false; }
     
  // uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600); // TFT_eSPI/Extensions/Touch.h
  bool pressed = tft.getTouch(&t_x, &t_y);  // True if valid key pressed
  
  // Check if any key coordinate boxes contain the touch coordinates
  for (uint8_t b = 0; b < 12; b++) {
    if (pressed && key[b].contains(t_x, t_y)) {   // bool contains(int16_t x, int16_t y);
    // void TFT_eSPI_Button::press(bool p) { laststate = currstate; currstate = p; }
    key[b].press(true);     // tell the button it is pressed
    } else {
      key[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 12; b++) {
    // void press(bool p); bool isPressed(); bool justPressed(); bool justReleased();
    if (key[b].justReleased()) key[b].drawButton(); // draw normal again

    if (key[b].justPressed()) 
    {
      key[b].drawButton(true);  // invert button colours void drawButton(bool inverted = false, String long_name = "");
      buttonpress(b);           // Send the button number
    }
  }
}

void buttonpress(int button)
{
  uint8_t keycode[6] = { 0 };
  char keych;
  uint8_t n;
  char Str1[] = "https://www.raspberrypi.com/products/raspberry-pi-pico/"; //Stringlength is chars+1
  char Str2[] = "https://github.com/raspberrypi\n\r";
  
  if ( !usb_hid.ready() ) return;
  
  LastMillis = millis();                 // Reset Timer if any key pressed
  if (!BackLightOn)                      // Keypress only active if backlight on
     {digitalWrite(LCDBackLight, HIGH);  // Backlight On
      BackLightOn = true;
      return; }                          // Skip 1st keypress
  
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
      keych = '3';
      usb_hid.keyboardPress(RID_KEYBOARD, keych);
      has_key = true;
      delay(keydelay);
      break;
    case 3:
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT);
      has_consumer_key = true;
      delay(keydelay);
      break;
    case 4:
      usb_hid.keyboardPress(RID_KEYBOARD, '4');
      has_key = true;
      delay(keydelay);
      break;
    case 5:
      keycode[0] = HID_KEY_GUI_RIGHT; // or use HID_KEY_GUI_LEFT do not use KEYBOARD_MODIFIER_LEFTGUI
      keycode[1] = HID_KEY_R;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 6:
      keycode[0] = HID_KEY_C;
      keycode[1] = HID_KEY_M;
      keycode[2] = HID_KEY_D;
      keycode[3] = HID_KEY_ENTER;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 7:
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_MUTE);
      has_consumer_key = true;
      delay(keydelay);
      break;
    case 8:
      for (n=0;  n<strlen(Str1); n++) 
          {usb_hid.keyboardPress(RID_KEYBOARD, Str1[n]);
          delay(keydelay2);
          usb_hid.keyboardRelease(RID_KEYBOARD);
          delay(keydelay2); }
      break;
    case 9:
      for (n=0;  n<strlen(Str2); n++) 
          {usb_hid.keyboardPress(RID_KEYBOARD, Str2[n]);
          delay(keydelay2);
          usb_hid.keyboardRelease(RID_KEYBOARD);
          delay(keydelay2);}
      break;
    case 10:
      usb_hid.keyboardPress(RID_KEYBOARD, '\r');
      has_key = true;
      delay(keydelay);
      break;
    case 11:
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT);
      has_consumer_key = true;
      delay(keydelay);
      break;
  }
}

void ConfigButtons() {
// Draw the keys ( 3 times 4 loops to create 12)
  tft.fillScreen(TFT_NAVY);
  for (uint8_t row = 0; row < 3; row++) {   // 3 rows
    for (uint8_t col = 0; col < 4; col++) { // of 4 buttons
     // The button number is the column we are on added to the row we are on times  the number of buttons per row. 
     // C++ uses the order of operations you use - first the row is multiplied by 4 and then the col is added.
     // The first button is 0. Example: col = 2 (3rd column), row = 1 (2nd row) =>: 1 * 4 = 4 --> 4 + 2 = 6 (7th button).
      uint8_t b = col + row * 4;
      // TFT_eSPI/Extensions/Button.h           x-middle,  y-middle,   width,      height,     outline,          fill,   text
      // void initButton(TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
      //                                uint16_t textcolor, char *label, uint8_t textsize);
      key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X), KEY_Y + row * (KEY_H + KEY_SPACING_Y), 
                                             KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE, keyLabel[b], KEY_TEXTSIZE);
      // void drawButton(bool inverted = false, String long_name = "");
      key[b].drawButton();
    }
  }
}

// Output report callback for LED indicator such as Caplocks
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) report_id;
  (void) bufsize;

  // LED indicator is output report with only 1 byte length
  if ( report_type != HID_REPORT_TYPE_OUTPUT ) return;

  // The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
  // Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
  uint8_t ledIndicator = buffer[0];

  // turn on LED if capslock is set
  digitalWrite(LED_BUILTIN, ledIndicator & KEYBOARD_LED_CAPSLOCK);
}


// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate()
{ tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(400);
}
