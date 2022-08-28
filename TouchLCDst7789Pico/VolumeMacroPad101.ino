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
/*******************************************************************************************
Layout 1                        Change to Layout 2 : press or swipe [cX]  [cC]  [cV]
-------------------------------------------------------------------------------------------
[Cut Ctrl+X] [  Copy Ctrl+C   ] [Paste Ctrl+V] [Volume Increase]    [cX]  [cC]  [cV]  [V+]
[Alt+PrtScr] [Admin Powershell] [ Run window ] [  Volume Mute  ]    [M1]  [M2]  [M3]  [Vo]
[Send Text ] [ Command Prompt ] [ Enter key  ] [Volume Decrease]    [M4]  [M5]  [M6]  [V-]
                                                                    Caps  Num  Scroll
                                                                    
Layout 2                         Change to Layout 1: press or swipe [H]  [Up]  [Pu]
-------------------------------------------------------------------------------------------
[   Home   ] [ Up Arrow ] [  Page Up  ] [Copy  Ctrl+C]              [H]  [Up]  [Pu]  [cX]                
[Left Arrow] [          ] [Right Arrow] [Copy  Ctrl+C]              [<]  [  ]  [> ]  [cC]
[   End    ] [Down Arrow] [ Page Down ] [Paste Ctrl+V]              [E]  [Dw]  [Pd]  [cV]
                                                                    Caps  Num Scroll

Layout 3                          Change to Layout 1 or 3: press Volume Mute [Vo] 5 times]
-------------------------------------------------------------------------------------------
[Cut Ctrl+X ] [Copy Ctrl+C] [Paste Ctrl+V] [Volume Increase]    [cX]  [cC]  [cV]  [V+]
[Send Text 1] [Send Text 2] [Send Text 3 ] [  Volume Mute  ]    [T1]  [T2]  [T3]  [Vo]
[Send Text 4] [Send Text 5] [Send Text 6 ] [Volume Decrease]    [T4]  [T5]  [T6]  [V-]
                                                                Caps  Num Scroll
********************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Layout change: Can slide across the 3 keys as well. 
//                Click on an open deskspace first before switching layouts.
//                The 3rd key in the sequence will have no action (i.e. "Paste" or "PageUp")
//                Layout 3 has the six M1 to M6 keys changed to send text strings T1 to T6
//                Change to Layout 3 by pressing VolumeMute 4 times, press again 4 times to switch back
//                Must press VolumeMute 4x consecutively else the count is reset to 0
// Text Strings:  Send new text strings up to 200 cahracters assigned to keys T1 to T6 via serial USB
//                Start each string with 1 to 6 followed by the string itself to be assigned to T1 to T6
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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
unsigned long TimePeriod = 60000; // 60 seconds
bool BackLightOn = true;          // Screen Saver
int LayoutKey1 = 0;               // Select sequence of 3 keys pressed once
int LayoutKey2 = 0;               // Will then switch to second layout
bool AltMenu = false;             // 1st or 2nd Layout
bool M1toM6Text = false;          // M1 to M6 all now text keys send Str1 to Str6
int VolMuteCount = 0;             // If tapped 5 times switch M1 to M6 all text 
// If true change color of bottom three keys Colours = [0,1][8,9,10]
bool CapsLock = false;            //
bool NumLock = false;             // 
bool ScrollLock = false;          // Excel still use this

///////////////////////////////////////////////////////////////////////////////
// To get new calibration data:
// 1. Comment out **[1]**
// 2. Uncomment   **[2]**
// 3. In setup() uncomment section between **[3]** and **[4]**
// 4. Load Pico with new uf2 and press the four calibration arrows
// 5. Observe (display or serial) the 5 calibration points
// 6. Use the new calibration points for calData[5]
// 7. Reverse points 1 to 3
///////////////////////////////////////////////////////////////////////////////
uint16_t calData[5] = {608, 3138, 471, 3158, 5};    // **[1]**
//uint16_t calData[5];                              // **[2]**
// Replace above line with the number on display once calibration is complete

// 65 x 4 = 260 + 12 x 5 = 320
// 60 x 3 = 180 + 15 x 4 = 240
// Key sizes and spacing
#define KEY_W 65 // Key width
#define KEY_H 60 // Key height
#define KEY_SPACING_X 12    // 12 // X gap
#define KEY_SPACING_Y 15    // 15 // Y gap
// Keypad start position
#define KEY_X (KEY_W/2) + KEY_SPACING_X   // 65/2 + 12 = 45 X-axis centre first key
#define KEY_Y (KEY_H/2) + KEY_SPACING_Y   // 60/2 + 15 = 45 Y-axis centre first key

#define KEY_TEXTSIZE 2   // Font size multiplier

// Adding a delay between keypressing to give our OS time to respond
uint8_t keydelay = 50;  // 100;  // keycode[1,..,6] delay milliseconds
uint8_t keydelay2 = 15;          // keyPress delay
uint16_t keydelay3 = 500;        // used in UAC confirm

// Create the screen object
TFT_eSPI tft = TFT_eSPI();

// Three sets of 12 button colours - use the colours defined in TFT_eSPI.h 
uint16_t Colours[3][12] =       {TFT_BLUE,      TFT_RED,       TFT_ORANGE,    TFT_DARKCYAN, 
                                 TFT_LIGHTGREY, TFT_LIGHTGREY, TFT_LIGHTGREY, TFT_DARKCYAN, 
                                 TFT_LIGHTGREY, TFT_LIGHTGREY, TFT_LIGHTGREY, TFT_DARKCYAN,
                                 TFT_BLUE,      TFT_LIGHTGREY, TFT_RED,       TFT_BLUE, 
                                 TFT_LIGHTGREY, TFT_BLACK,     TFT_LIGHTGREY, TFT_RED, 
                                 TFT_BLUE,      TFT_LIGHTGREY, TFT_RED,       TFT_ORANGE,
                                 TFT_BLUE,      TFT_RED,       TFT_ORANGE,    TFT_DARKCYAN, 
                                 TFT_PINK,      TFT_PINK,      TFT_PINK,      TFT_SKYBLUE, 
                                 TFT_PINK,      TFT_PINK,      TFT_PINK,      TFT_DARKCYAN,};
                             
uint16_t keyColor[12] = {};

uint16_t BackgroundColor[3] = {TFT_BLACK, TFT_BLACK, TFT_BLACK};   // TFT_NAVY

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
#define KeyTab   0x2B
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

//////////////////////////////////////////////
// Actions assigned to keys
// Should really use a struct here:
// struct Action[12] {
//        String aStr;
//        char aChr;
//        byte HIDKey;
//        int  CSMKey;  }
/////////////////////////////////////////////

// Two sets of 12 button labels <name>[<number-of-lables>][<number-of-chars-per-label]
// The number of chars per label should include the termination \0.
const static char Labels[3][12][3] = {"cX", "cC", "cV", "V+", "M1", "M2", "M3", "Vo", "M4", "M5", "M6", "V-",
                                      "H",  "Up", "Pu", "cX", "<",  " ",  ">",  "cC", "E",  "Dw", "Pd", "cV",
                                      "cX", "cC", "cV", "V+", "T1", "T2", "T3", "Vo", "T4", "T5", "T6", "V-",};
char keyLabel[12][3] = {};

char Str1[200] = "https://www.raspberrypi.com/products/raspberry-pi-pico/"; // Strlen is chars+1
char Str2[200] = "https://github.com/raspberrypi\n\r";
char Str3[200] = "email.1@email.com";                             
char Str4[200] = "email.2@email.com";
char Str5[200] = "sfc /scannow\n";  
char Str6[200] = "\n\r"; 
const static char Str7[] = "cmd\r";                                    // Str3[5] = {'c','m','d','\r','\0'};
const static char Str8[] = "winver\n"; 

char inputString[200] = "";         // String to hold incoming data
bool stringComplete = false;        // String is complete

// Create 12 'keys' to use later
TFT_eSPI_Button key[12];

///////////////////////////////////////
//
///////////////////////////////////////
void setup() {

  Serial.begin(115200);
  //Serial.println("VolumeMacroKeys version 10.2");
  
  pinMode(LED_BUILTIN, OUTPUT);   // Used for Capslock
  digitalWrite(LED_BUILTIN, LOW);
  
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB HID Composite");
  usb_hid.setReportCallback(NULL, hid_report_callback); // Activate report CapsScrollNum Lock indicators
  usb_hid.begin();
  while( !TinyUSBDevice.mounted() ) delay(1);

  // Initialise the TFT screen TFT_eSPI/TFT_eSPI.h
  tft.init();
  tft.setRotation(3);    // Set the display orientation to 0, 1, 2 or 3
  tft.setTextFont(1);
  tft.setTextSize(KEY_TEXTSIZE);
    
  BackLightOn = true;     // TFT init will turn it on
  
  //////////////////////////////////////////////
  // Uncomment for new calibration
  //////////////////////////////////////////////
  /*                                 // **[3]**
  touch_calibrate();
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  for (uint8_t i = 0; i < 5; i++)
  { tft.print(calData[i]);
    if (i < 4) tft.print(", ");
  }
  delay(8000);                      
  */                                 // **[4]**

  tft.setTouch(calData);  // void setTouch(uint16_t *data); // TFT_eSPI/Extensions/Touch.h 
  ConfigButtons();        // Draw Buttons and Labels
  LastMillis = millis();  //initial start time
}

/////////////////////////////
//
/////////////////////////////
void loop() {

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
    
  NowMillis = millis();   // get the current "time" (number of milliseconds since started)
  if ((NowMillis - LastMillis) >= TimePeriod)  //test whether the period has elapsed
  {
    digitalWrite(LCDBackLight, LOW);   // Backlight Off
    //analogWrite(LCDBackLight, 128);  // Backlight Dimmed
    LastMillis = NowMillis;            // Start Tiemer again
    BackLightOn = false;               // Until keypress
    LayoutKey1 = LayoutKey2 = 0;     // Reset Special keys for Alt Menu
  }
     
  if (has_consumer_key) { usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
                          has_consumer_key = false; }
  if (has_key) { usb_hid.keyboardRelease(RID_KEYBOARD);
                 has_key = false; }
   
  ///////////////////////////////////////////////////////////////////////////////////////////////////////  
  // uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600) in TFT_eSPI/Extensions/Touch.h
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  bool pressed = tft.getTouch(&t_x, &t_y, 300);                             // True if valid key pressed
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  // Check if any key coordinate boxes contain the touch coordinates - no multitouch
  // void TFT_eSPI_Button::press(bool p) { laststate = currstate; currstate = p; }
  // Shorter: key[b].press((pressed && key[b].contains(t_x, t_y)));
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  for (uint8_t b = 0; b < 12; b++) { if (pressed && key[b].contains(t_x, t_y)) key[b].press(true);   
                                                                         else  key[b].press(false); }                    
   
  // Check if any key changed state void press(bool p); bool isPressed() bool justPressed(); bool justReleased();
  for (uint8_t b = 0; b < 12; b++) { if (key[b].justReleased())   key[b].drawButton();           // draw normal again
                                     if (key[b].justPressed())  { key[b].drawButton(true);       // invert button colours
                                                                  buttonpress(b);           } }  // Send the button number

  int i, n = 0;
  char *ChrPtr;
  bool Found = false;
  char inChar;
  while ((Serial.available())&&(n<200)) {
    if (Found) { ChrPtr[n-1] = inChar; } // Skip the first char
    // get the new byte:
    inChar = (char)Serial.read();
    i = inChar-'0';
    if ((i<7)&&(!Found)) { Found = true; n--;
               switch(i) { case 0:  ChrPtr = inputString; break;
                           case 1:  ChrPtr = Str1; break;
                           case 2:  ChrPtr = Str2; break;
                           case 3:  ChrPtr = Str3; break;
                           case 4:  ChrPtr = Str4; break;
                           case 5:  ChrPtr = Str5; break;
                           case 6:  ChrPtr = Str6; break; } }
    n++;
    // check for newline
    if (inChar == '\n') { stringComplete = true; ChrPtr[n-1] = '\0';}
   }                                             
}


//////////////////////////////////////
//
//////////////////////////////////////
void buttonpress(int button)
{
  byte keycode[6] = {0};  //
  char keych;
  byte n;
  
  if ( !usb_hid.ready() ) return;
  
  LastMillis = millis();                 // Reset Timer if any key pressed
    
  if (!BackLightOn)                      // Keypress only active if backlight on
     {digitalWrite(LCDBackLight, HIGH);  // Backlight On
      BackLightOn = true;
      return; }                          // Skip 1st keypress

  if ((VolMuteCount>0)&&(button!=7)) VolMuteCount = 0; // Must press VolumeMute 5x consecutively
  
  if (!AltMenu)     
  switch(button){
    case 0: ///////////////////////////////////////////////// Cut = control + x                             
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_X; // actually v not V
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); has_key = true; delay(keydelay); 
      if (LayoutKey2==0) LayoutKey1 = 1;                  // First key pressed to change layout button + 1;  
      break;
    case 1: ///////////////////////////////////////////////// Copy = control + c
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_C;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); has_key = true;  delay(keydelay);
      if (LayoutKey1==1)LayoutKey2 = 2; // button + 1;     // Second key pressed to change layout
      break;
    case 2: ////////////////////////////////////////////////// Paste = control + v
      if ((LayoutKey1==1)&&(LayoutKey2==2)){ DoKeyboard1(); // Last key to change layout - do before key 
                                             break ; }      // Done here
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_V;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); has_key = true; delay(keydelay);
      break; 
    case 3: /////////////////////////////////////////////////// Volume Increase
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolUp); has_consumer_key = true;  delay(keydelay);
      break;
    case 4: // M1 ///////////////////////////////////////////////// Alt + PrintScreen - focus window - some mini keyboards cannot do this
      if (M1toM6Text) { for (n=0;  n<strlen(Str1); n++) 
                            {usb_hid.keyboardPress(RID_KEYBOARD, Str1[n]); delay(keydelay2);
                             usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      keycode[0] = AltL; 
      keycode[1] = PrtScr;
      keycode[2] = 0x00; // Need this why? 
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay2);
      usb_hid.keyboardRelease(RID_KEYBOARD);            delay(keydelay2);
      break;
    case 5: // M2 ///////////////////////////////////////////////// Open admin powershell including UAC completion
      if (M1toM6Text) { for (n=0;  n<strlen(Str2); n++) 
                            {usb_hid.keyboardPress(RID_KEYBOARD, Str2[n]); delay(keydelay2);
                             usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
                             
      keycode[0] = HID_KEY_GUI_LEFT; // or use HID_KEY_GUI_LEFT 
      keycode[1] = HID_KEY_X;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);   delay(keydelay2);
      usb_hid.keyboardRelease(RID_KEYBOARD);              delay(keydelay2);
      usb_hid.keyboardPress(RID_KEYBOARD, 'A');           delay(keydelay2); // A admin powershell i normal powershell g Computer Management
      usb_hid.keyboardRelease(RID_KEYBOARD);              delay(keydelay3);
      keycode[0] = ArrLeft;
      keycode[1] = KeyEnter;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);   delay(keydelay2);
      has_key = true;                                     delay(keydelay2);
      break;
    case 6: // M3 /////////////////////////////////////////////// Open Run window with last coommand visible
      if (M1toM6Text) { for (n=0;  n<strlen(Str3); n++) 
                            {usb_hid.keyboardPress(RID_KEYBOARD, Str3[n]); delay(keydelay2);
                             usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
                             
      keycode[0] = HID_KEY_GUI_RIGHT; // or use HID_KEY_GUI_LEFT do not use KEYBOARD_MODIFIER_LEFTGUI
      keycode[1] = HID_KEY_R;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); has_key = true; delay(keydelay);
      break;
    case 7: //////////////////////////////////////////////// Volume Mute and change M1 to M6 all text keys
      if (VolMuteCount==3) {M1toM6Text = (!M1toM6Text);  VolMuteCount = 0; ConfigButtons(); break;} else VolMuteCount++ ; 
         
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolMute); has_consumer_key = true; delay(keydelay);
      break;
    case 8: // M4 ///////////////////////////////////////////// Send a text piece - could overide by sending serial text piece
      if (!stringComplete) for (n=0; n<=strlen(Str4); n++) inputString[n] = Str4[n]; 
      for (n=0;  n<strlen(inputString); n++) 
          {usb_hid.keyboardPress(RID_KEYBOARD, inputString[n]);   delay(keydelay2);
          usb_hid.keyboardRelease(RID_KEYBOARD);                  delay(keydelay2); }
      break;
    case 9: // M5 //////////////////////////////////////////// Open normal command prompt via Run window
      if (M1toM6Text) { for (n=0;  n<strlen(Str5); n++) 
                            {usb_hid.keyboardPress(RID_KEYBOARD, Str5[n]); delay(keydelay2);
                             usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
                             
      keycode[0] = HID_KEY_GUI_LEFT; // or use HID_KEY_GUI_LEFT do not use KEYBOARD_MODIFIER_LEFTGUI
      keycode[1] = HID_KEY_R;        // HID_KEY_X Then A admin powershell i normall powershell g Computer Managemnet
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(RID_KEYBOARD);                 delay(keydelay2);
      for (n=0;  n<strlen(Str7); n++)                        // Str7:Cmd Prompt Str8: winver
          {usb_hid.keyboardPress(RID_KEYBOARD, Str7[n]);     delay(keydelay2);
          usb_hid.keyboardRelease(RID_KEYBOARD);             delay(keydelay2); }
      break;
    case 10: // M6 //////////////////////////////////////// M6 text or Enter key
      if (M1toM6Text) { for (n=0;  n<strlen(Str6); n++) 
                            {usb_hid.keyboardPress(RID_KEYBOARD, Str6[n]); delay(keydelay2);
                             usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
                             
      usb_hid.keyboardPress(RID_KEYBOARD, '\r'); has_key = true; delay(keydelay);
      break;
    case 11: ////////////////////////////////////////// Volume decrease
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolDown); has_consumer_key = true; delay(keydelay);
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
    case 0:  
      keycode[0] = KeyHome;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay); 
      if (LayoutKey2==0) LayoutKey1 = 1; // button + 1;  
      break;
    case 1:
      keycode[0] = ArrUp;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      if (LayoutKey1==1)LayoutKey2 = 2; // button + 1;
      break;
    case 2:
      if ((LayoutKey1==1)&&(LayoutKey2==2)){ DoKeyboard1(); // Last key to change layout - do before key 
                                               break ; }      // Done here
      keycode[0] = PageUp;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
      has_key = true;
      delay(keydelay);
      if (LayoutKey1==1)LayoutKey2 = 2; // button + 1;
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
  LayoutKey1 = 0;        // Reset
  LayoutKey2 = 0;
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

  if (AltMenu) b = 1; else if (M1toM6Text) b = 2; else b =0;
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

/////////////////////////////////////////////////////////////////////////////
// Output report callback for LED indicator such as Caplocks
/////////////////////////////////////////////////////////////////////////////
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) report_id;
  (void) bufsize;
  bool CapsLockPrev = CapsLock;            
  bool NumLockPrev = NumLock;             
  bool ScrollLockPrev = ScrollLock; 
  bool Change = false;          
  
  /*********************************************************
  KEYBOARD_LED_NUMLOCK    = TU_BIT(0), ///< Num Lock LED
  KEYBOARD_LED_CAPSLOCK   = TU_BIT(1), ///< Caps Lock LED
  KEYBOARD_LED_SCROLLLOCK = TU_BIT(2), ///< Scroll Lock LED
  KEYBOARD_LED_COMPOSE    = TU_BIT(3), ///< Composition Mode
  KEYBOARD_LED_KANA       = TU_BIT(4) ///< Kana mode
  **********************************************************/

  // LED indicator is output report with only 1 byte length
  if ( report_type != HID_REPORT_TYPE_OUTPUT ) return;

  // The LED bit map:  Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
  uint8_t ledIndicator = buffer[0];

  // turn on LED if capslock is set
  digitalWrite(LED_BUILTIN, ledIndicator & KEYBOARD_LED_CAPSLOCK);
  
  CapsLock   = (ledIndicator & KEYBOARD_LED_CAPSLOCK); 
  NumLock    = (ledIndicator & KEYBOARD_LED_NUMLOCK);
  ScrollLock = (ledIndicator & KEYBOARD_LED_SCROLLLOCK);

  if ((CapsLock!=CapsLockPrev)||(NumLock!=NumLockPrev)||(ScrollLock!=ScrollLockPrev)) Change = true;
  
  if (Change) {if (CapsLock)   { Colours[0][8] =  TFT_DARKGREEN; Colours[1][8] =  TFT_DARKGREEN; Colours[2][8] =  TFT_DARKGREEN; } // TFT_OLIVE TFT_MAROON
                   else        { Colours[0][8] =  TFT_LIGHTGREY; Colours[1][8] =  TFT_BLUE;    ; Colours[2][8] =  TFT_PINK;      } 
               if (NumLock)    { Colours[0][9] =  TFT_DARKGREEN; Colours[1][9] =  TFT_DARKGREEN; Colours[2][9] =  TFT_DARKGREEN; } // TFT_DARKGREEN 
                   else        { Colours[0][9] =  TFT_LIGHTGREY; Colours[1][9] =  TFT_LIGHTGREY; Colours[2][9] =  TFT_PINK;      } 
               if (ScrollLock) { Colours[0][10] = TFT_DARKGREEN; Colours[1][10] = TFT_DARKGREEN; Colours[1][10] = TFT_DARKGREEN; } // TFT_GREENYELLOW similar
                   else        { Colours[0][10] = TFT_LIGHTGREY; Colours[1][10] = TFT_RED;       Colours[1][10] = TFT_PINK;      } // to TFT_GREEN
               ConfigButtons();}                                                                                                   // Draw Buttons and Labels  

   if ((Change)&&(!BackLightOn))  { digitalWrite(LCDBackLight, HIGH); // Turn backlight on if Caps-Num-Scroll lock change
                                    BackLightOn = true; }
}


////////////////////////////////////////////////////////////////////////////////////////
// Code to run a screen calibration, not needed when calibration values set in setup()
////////////////////////////////////////////////////////////////////////////////////////
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
