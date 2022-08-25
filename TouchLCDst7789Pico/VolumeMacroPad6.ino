
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

int LCDBackLight = 13; // Will be set as OUTPUT and HIGH by TFT init
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

// USB HID object desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

// Used to dim LCD Backlight if 60 seconds no touch
unsigned long NowMillis = 0;
unsigned long LastMillis = 0;
unsigned long TimePeriod = 60000;  // 60 seconds
bool BackLightOn = true;
int SpecialKey1 = 0;
int SpecialKey2 = 0; // Select sequence of 3 keys pressed once
bool AltMenu = false;

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
uint8_t keydelay = 50; // 100;  // milliseconds
uint8_t keydelay2 = 15;

// Create the screen object
TFT_eSPI tft = TFT_eSPI();

// Two sets of 12 button colours - use the colours defined in TFT_eSPI.h
const static uint16_t Colours[2][12] = {TFT_BLUE, TFT_RED, TFT_ORANGE, TFT_DARKCYAN, 
                                 TFT_LIGHTGREY, TFT_LIGHTGREY, TFT_LIGHTGREY, TFT_SKYBLUE, 
                                 TFT_LIGHTGREY, TFT_LIGHTGREY, TFT_LIGHTGREY, TFT_DARKCYAN,
                                 TFT_BLUE, TFT_GOLD, TFT_BLUE, TFT_BLUE, 
                                 TFT_GOLD, TFT_BLACK, TFT_GOLD, TFT_RED, 
                                 TFT_RED, TFT_GOLD, TFT_RED, TFT_ORANGE  };
                             
uint16_t keyColor[12] = {};

uint16_t BackgroundColor[2] = {TFT_NAVY, TFT_PURPLE}; 

//////////////////////////////////////////////////////////////////////////////////////////
// Consumer keys are 16 bits - names are long to use will use hex values or short alias
// HID_USAGE_CONSUMER_POWER                             = 0x0030,
// HID_USAGE_CONSUMER_RESET                             = 0x0031,
// HID_USAGE_CONSUMER_SLEEP                             = 0x0032,
// HID_USAGE_CONSUMER_BRIGHTNESS_INCREMENT              = 0x006F,
// HID_USAGE_CONSUMER_BRIGHTNESS_DECREMENT              = 0x0070,
// HID_USAGE_CONSUMER_PLAY_PAUSE                        = 0x00CD,
// HID_USAGE_CONSUMER_SCAN_NEXT                         = 0x00B5,
// HID_USAGE_CONSUMER_SCAN_PREVIOUS                     = 0x00B6,
// HID_USAGE_CONSUMER_STOP                              = 0x00B7,
// HID_USAGE_CONSUMER_VOLUME                            = 0x00E0,
// HID_USAGE_CONSUMER_MUTE                              = 0x00E2,
// HID_USAGE_CONSUMER_BASS                              = 0x00E3,
// HID_USAGE_CONSUMER_TREBLE                            = 0x00E4,
// HID_USAGE_CONSUMER_BASS_BOOST                        = 0x00E5,
// HID_USAGE_CONSUMER_VOLUME_INCREMENT                  = 0x00E9,
// HID_USAGE_CONSUMER_VOLUME_DECREMENT                  = 0x00EA,
// HID_USAGE_CONSUMER_BASS_INCREMENT                    = 0x0152,
// HID_USAGE_CONSUMER_BASS_DECREMENT                    = 0x0153,
// HID_USAGE_CONSUMER_TREBLE_INCREMENT                  = 0x0154,
// HID_USAGE_CONSUMER_TREBLE_DECREMENT                  = 0x0155,
////////////////////////////////////////////////////////////////////////////////////////
#define VolUp   0x00E9
#define VolDown 0x00EA
#define VolMute 0x00E2
////////////////////////////////////////////////////////////////////////////////////////
#define KeyCut   0x7B   // Ctrl + X
#define KeyCopy  0x7C   // Ctrl + C
#define KeyPaste 0x7D   // Ctrl + V
#define PrtScr   0x46
////////////////////////////////////////////////////////////////////////////////////////
#define ArrUp    0x52
#define ArrDown  0x51
#define ArrLeft  0x50
#define ArrRight 0x4F
#define KeyHome  0x4A
#define KeyEnd   0x4D
#define PageUp   0x4B
#define PageDwn  0x4E
#define KeyEnter 0x28 // KEYPAD_ENTER = 0x58 KEY_RETURN = 0x9E KEY_EXECUTE = 0x74
////////////////////////////////////////////////////////////////////////////////////////
// #define HID_KEY_CONTROL_LEFT              0xE0
// #define HID_KEY_SHIFT_LEFT                0xE1
// #define HID_KEY_ALT_LEFT                  0xE2
// #define HID_KEY_GUI_LEFT                  0xE3
// #define HID_KEY_CONTROL_RIGHT             0xE4
// #define HID_KEY_SHIFT_RIGHT               0xE5
// #define HID_KEY_ALT_RIGHT                 0xE6
// #define HID_KEY_GUI_RIGHT                 0xE7
////////////////////////////////////////////////////////////////////////////////////////
#define CtrL 0xE0
#define ShfL 0xE1
#define AltL 0xE2
#define WinL 0xE3
////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////
// Actions assigned to keys - really need a struct:
// struct Action {
//        String aStr;
//        char aChr;
//        byte HIDKey;
//        int  CSMKey;  }
////////////////////////////////////////////////////

// Two sets of 12 button labels <name>[<number-of-lables>][<number-of-chars-per-label]
// The number of chars per label should include the termination \0.
const static char Labels[2][12][3] = {"cX", "cC", "cV", "V+", "M1", "M2", "M3", "Vo", "M4", "M5", "M6", "V-",
                                      "H",  "Up", "Pu", "cX", "<",  " ",  ">",  "cC", "E",  "Dw", "Pd", "cV" };
char keyLabel[12][3] = {};

const static char Str1[] = "https://www.raspberrypi.com/products/raspberry-pi-pico/"; //Stringlength is chars+1
const static char Str2[] = "https://github.com/raspberrypi\n\r";

// Create 12 'keys' to use later
TFT_eSPI_Button key[12];

///////////////////////////////////////
//
///////////////////////////////////////
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
  tft.setRotation(3);    // Set the display orientation to 0, 1, 2 or 3
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
    SpecialKey1 = SpecialKey2 = 0;     // Reset Special keys
  }
      
  if (has_consumer_key) { usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
                          has_consumer_key = false; }
  if (has_key) { usb_hid.keyboardRelease(RID_KEYBOARD);
                 has_key = false; }
     
  // uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600); // TFT_eSPI/Extensions/Touch.h
  bool pressed = tft.getTouch(&t_x, &t_y);  // True if ANY valid key pressed
  
  // Check if any key coordinate boxes contain the touch coordinates - no multitouch
  // void TFT_eSPI_Button::press(bool p) { laststate = currstate; currstate = p; }
  // Shorter: key[b].press((pressed && key[b].contains(t_x, t_y)));
  for (uint8_t b = 0; b < 12; b++) { if (pressed && key[b].contains(t_x, t_y))  key[b].press(true);   
                                              else  key[b].press(false);                         }                    
   
  // Check if any key changed state void press(bool p); bool isPressed(); bool justPressed(); bool justReleased();
  for (uint8_t b = 0; b < 12; b++) { if (key[b].justReleased())   key[b].drawButton();           // draw normal again
                                     if (key[b].justPressed())  { key[b].drawButton(true);       // invert button colours
                                                                  buttonpress(b);           } }  // Send the button number
}


//////////////////////////////////////////////////////////////////////////
void buttonpress(int button)
{
  byte keycode[6] = { 0 };
  char keych;
  byte n;
  
  if ( !usb_hid.ready() ) return;
  
  LastMillis = millis();                 // Reset Timer if any key pressed
    
  if (!BackLightOn)                      // Keypress only active if backlight on
     {digitalWrite(LCDBackLight, HIGH);  // Backlight On
      BackLightOn = true;
      return; }                          // Skip 1st keypress
  if (!AltMenu)     
  switch(button){
    case 0:                              // Key 0 special case check long or shortpress
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_X;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay); 
      if (SpecialKey2==0) SpecialKey1 = 1; // button + 1;  
      break;
    case 1:
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_C;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      if (SpecialKey1==1)SpecialKey2 = 2; // button + 1;
      break;
    case 2:
      if ((SpecialKey1==1)&&(SpecialKey2==2)) DoKeyboard1(); // Do before key 
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_V;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 3:
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolUp);
      has_consumer_key = true;
      delay(keydelay);
      break;
    case 4:
      for (n=0;  n<strlen(Str1); n++) 
          {usb_hid.keyboardPress(RID_KEYBOARD, Str1[n]);
          delay(keydelay2);
          usb_hid.keyboardRelease(RID_KEYBOARD);
          delay(keydelay2); }
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
      keycode[3] = KeyEnter;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 7:
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolMute);
      has_consumer_key = true;
      delay(keydelay);
      break;
    case 8:
      for (n=0;  n<strlen(Str2); n++) 
          {usb_hid.keyboardPress(RID_KEYBOARD, Str2[n]);
          delay(keydelay2);
          usb_hid.keyboardRelease(RID_KEYBOARD);
          delay(keydelay2); }
      break;
    case 9:
      keycode[0] = HID_KEY_2;
      keycode[1] = KeyEnter;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      if (SpecialKey1==1)SpecialKey2 = 2; // button + 1;
      break;
    case 10:
      usb_hid.keyboardPress(RID_KEYBOARD, '\r');
      has_key = true;
      delay(keydelay);
      break;
    case 11:
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolDown);
      has_consumer_key = true;
      delay(keydelay);
      break;
  }

  /*************************
  #define ArrUp    0x52
  #define ArrDown  0x51
  #define ArrLeft  0x50
  #define ArrRight 0x4F
  #define KeyHome  0x4A
  #define KeyEnd   0x4D
  #define PageUp   0x4B
  #define PageDwn  0x4E
  #define KeyEnter 0x28
  ***********************/
  if (AltMenu)     
  switch(button){
    case 0:                              // Key 0 special case check long or shortpress
      keycode[0] = KeyHome;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay); 
      if (SpecialKey2==0) SpecialKey1 = 1; // button + 1;  
      break;
    case 1:
      keycode[0] = ArrUp;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      if (SpecialKey1==1)SpecialKey2 = 2; // button + 1;
      break;
    case 2:
      if ((SpecialKey1==1)&&(SpecialKey2==2)) DoKeyboard1(); // Do before key 
      keycode[0] = PageUp;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 3:
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_X;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 4:
      keycode[0] = ArrLeft;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 5:
      delay(keydelay);
      break;
    case 6:
      keycode[0] = ArrRight;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 7:
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_C;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 8:
      keycode[0] = KeyEnd;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 9:
      keycode[0] = ArrDown;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 10:
      keycode[0] = PageDwn;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
    case 11:
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_V;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      break;
  }
  
}

void DoKeyboard1() {
  int n, m = 0;
  AltMenu = (!AltMenu);
  ConfigButtons();        // Draw Buttons and Labels
  SpecialKey1 = 0;        // Reset
  SpecialKey2 = 0;
}

void DoKeyboard2() {
  uint8_t keycode[6] = { 0 };
  keycode[0] = HID_KEY_0;
  usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
  has_key = true;
  delay(keydelay); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw the keys ( 3 times 4 loops to create 12)
// The button number is the column we are on added to the row we are on times  the number of buttons per row. 
// C++ uses the order of operations you use - first the row is multiplied by 4 and then the col is added.
// The first button is 0. Example: col = 2 (3rd column), row = 1 (2nd row) =>: 1 * 4 = 4 --> 4 + 2 = 6 (7th button).
// TFT_eSPI/Extensions/Button.h           x-middle,  y-middle,   width,      height,     outline,          fill,   text
// void initButton(TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
//                                uint16_t textcolor, char *label, uint8_t textsize);
// void drawButton(bool inverted = false, String long_name = "");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigButtons() {     
  int n, m, b;

  if (AltMenu) b = 1; else b = 0 ;
  tft.fillScreen(BackgroundColor[b]);
  for (n=0; n<12; n++) for (m=0; m<3; m++) keyLabel[n][m] = Labels[b][n][m]; 
  for (n=0; n<12; n++) keyColor[n] = Colours[b][n];
                
  for (uint8_t row = 0; row < 3; row++) {   // 3 rows 0,1,2 => b = 0+(0*4)=0 1+(0*4)=1 ... 0+(1*4)=4 ....
    for (uint8_t col = 0; col < 4; col++) { // 4 columns 0,1,2,3
        b = col + row * 4;
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X), KEY_Y + row * (KEY_H + KEY_SPACING_Y), 
                                               KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE, keyLabel[b], KEY_TEXTSIZE);
        key[b].drawButton();  }  }
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
