///////////////////////////////////////////////////////////////////////////////////////////
// Combination of (1) Raspberry Pi Pico Touch Macro Keyboard by Dustin Watts
//                    https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard
//                (2) AdafruitTinyUSB HID examples hid_composite.ino hid_boot_keyboard.ino
//                    https://github.com/adafruit/Adafruit_TinyUSB_Arduino/
//                (3) Bodmer-Adafruit Keypad Keypad_240x320.ino
//                    https://github.com/Bodmer/TFT_eSPI/examples/
///////////////////////////////////////////////////////////////////////////////////////////
// Adapted by T van Dyk Sep 2022 for ST7789 320x240 and Adafruit TinyUSB stack
///////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!
 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
*********************************************************************/
/****************************************************************************************************
Layout 1  Cycle through Layout 1 to 4 press VolumeMute [Vo] or [L1-L4] 3 or more times
------------------------------------------------------------------------------------------------
[Cut Ctrl+X] [  Copy Ctrl+C   ] [Paste Ctrl+V] [VolUp  Delete]    [cX]  [cC]  [cV]  [V+][Delete]
[Alt+PrtScr] [Admin Powershell] [ Run window ] [VolMute L1-L4]    [M1]  [M2]  [M3]  [Vo][L1-L4 ]
[Send Text ] [Admin CMD Prompt] [Task Manager] [VolDwn  Enter]    [M4]  [M5]  [M6]  [V-][Enter ]
                                                                  Caps  Num  Scroll
Layout 2  Cycle through Layout 1 to 4 press VolumeMute [Vo][L1-L4] 3 or more times consecutively
--------------------------------------------------------------------------------------------------
[Home Prv Mute   ] [ Up Arrow ] [PageUp Nxt DelRet] [VolUp  Delete] [H]  [Up ]  [Pu]  [V+][Delete]
[ArrL P-P LayerAB] [  Config  ] [ArrR  Stop  Media] [VolMute L1-L4] [<]  [Cfg]  [> ]  [Vo][L1-L4 ]
[End CfgDelete   ] [Down Arrow] [PageDwn     L1-L2] [VolMute Enter] [E]  [Dw ]  [Pd]  [V-][Enter ]
                                                                    Caps  Num  Scroll
Layout 3+4 Cycle through Layout 1 to 4 press VolumeMute [Vo][L1-L4] 3 or more times
--------------------------------------------------------------------------------------------------
[Cut Ctrl+X ] [Copy Ctrl+C] [Paste Ctrl+V] [VolUp  Delete]   [ cX ]  [ cC  ]  [ cV  ] [V+][Delete]
[Send Text 1] [Send Text 2] [Send Text 3 ] [VolMute L1-L4]   [S1 T1] [S2 T2]  [S3 T3] [Vo][L1-L4 ]
[Send Text 4] [Send Text 5] [Send Text 6 ] [VolMute Enter]   [S4 T4] [S5 T5]  [S6 T6] [V-][Enter ]
                                                              Caps    Num     Scroll
Layout change: Cycle through Layout 1 - 4 press [Vo] 3 or more times or press [L1][L2][L3][L4]
-------------------------------------------------------------------------------------------------
Layouts 1, 3 and 4, two Layers A/B each with A=M1-M6, S1-S6, T1-T6 and B=M7-M12, S7-S12, T7-T12
Must press VolumeMute or [L1-L4] consecutively and within 5 seconds else the count is reset to 0
RepTimePeriod 0.5 seconds - after this a key repeat is active

Press Config Key (new active keys change colour) then:
L14 (Home) - VolumeMute -> L1, L2, L3, L4 - repeat to restore Vo
D-R (PageUp) - Vol+ -> Delete and Vol- -> Enter - repeat to restore V+ V-
A-B (ArrowLeft) - Layouts 1, 3, 4, change to Layer A or Layer B 
Med (ArrowRight) - Change Layout 2 to Media Controls Previous-Next-PlayPause-Stop
Del (End) - Delete all files on Flash (Strings and Config)
L12 (PageDown) - Layout 1 or Layout 2 on powerup - must also press Sav(e) (Cfg)
Save (Cfg) Info and File List to sent Serial Monitor and Text/Macro and Config files saved

Text Strings: Send new text strings up to 200 characters to keys S1/T1 - S12/T12 via USBserial
Start string with 1 to 6 followed by the string to be assigned to S1/T1-S12/T12
If current Layout is L3 then S1 to S6 changed if Layout is L4 then T1 to T6 changed
If current Layer is A then S1/T1-S6/T6 changed, layer B S7/T7-S12/T12 changed
Send new Text for Key [M4] - start with 0 end with LF
End text string with LF. To send numbers use RealTerm - see the picture below
***********************************************************************************************************************************/
#include "Adafruit_TinyUSB.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include <FS.h>

// Custom Strings file
#define STR1f  "/StrData1"
#define STR2f  "/StrData2"
#define STR3f  "/StrData3"
#define STR4f  "/StrData4"
#define STR5f  "/StrData5"
#define STR6f  "/StrData6"
#define STR7f  "/StrData7"
#define STR8f  "/StrData8"
#define STR9f  "/StrData9"
#define STR10f "/StrData10"
#define STR11f "/StrData11"
#define STR12f "/StrData12"

#define TTR1f  "/TtrData1"
#define TTR2f  "/TtrData2"
#define TTR3f  "/TtrData3"
#define TTR4f  "/TtrData4"
#define TTR5f  "/TtrData5"
#define TTR6f  "/TtrData6"
#define TTR7f  "/TtrData7"
#define TTR8f  "/TtrData8"
#define TTR9f  "/TtrData9"
#define TTR10f "/TtrData10"
#define TTR11f "/TtrData11"
#define TTR12f "/TtrData12"

char STRname [12][11] = { STR1f, STR2f, STR3f, STR4f, STR5f, STR6f, STR7f, STR8f, STR9f, STR10f, STR11f, STR12f };
char TTRname [12][11] = { TTR1f, TTR2f, TTR3f, TTR4f, TTR5f, TTR6f, TTR7f, TTR8f, TTR9f, TTR10f, TTR11f, TTR12f };

int LCDBackLight = 13; // Will be set as OUTPUT and HIGH by TFT init

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
unsigned long NowMillis = 0;           // LCD Backlight Saver
unsigned long LastMillis = 0;          // LCD Backlight Saver
unsigned long VolMuteMillisNow = 0;    // if VolMute not pressed within 5 sec cancel Layout counter
unsigned long VolMuteMillisLast = 0;   // if VolMute not pressed within 5 sec cancel Layout counter
unsigned long VolMuteTimer = 6000;     // 6 Seconds VolMute counter timeout Timer
unsigned long TimePeriod = 60000;      // 60 seconds LCD Backlight Timer
unsigned long RepTimePeriod = 500;     // After this key repeat is active
unsigned long RepLast = 0;             // Time when key first pressed
unsigned long RepNow = 0;              // Time when key still pressed
bool BackLightOn = true;               // LCD Backlight Saver
int VolMuteCount = 0;                  // Can be 0,1=Layout1+VolMute(On/Off),2=Layout2,3=Layout3,4=Layout4 
int Layout = 1;                        // Layout 1, 2, 3 or 4
int LayerAB = 0;                       // Switch between 2 Layouts for Layers 1, 3, 4 M1-M6 or M7-M12 etc
                                       // To save it to for next powerup Flash must press [Sav]
// If true change color of bottom three keys Colours = Layer[0,1,2,3]Key[8,9,10]
bool CapsLock = false;             //
bool NumLock = false;              // 
bool ScrollLock = false;           // Excel still use this
int ConfigKeyCount = 0;            // Start with Layout 2 Cfg key not pressed 
bool MuteDisable = false;          // Disable VolMute if pressed Layout switch
bool VolDisable = false;           // Disable Vol+ Vol- keys now DEL ENTER
bool SaveLayout = false;           // Start with selected layout 
bool Media = false;                // Four Media Keys in Layout 2
///////////////////////////////////////////////////////////////////////////////
// To get new calibration data:
// 1. Comment out **[1]**
// 2. Uncomment   **[2]**
// 3. In setup() uncomment section between **[3]** and **[4]**
// 4. Load Pico with new uf2 and press the four calibration arrows
// 5. Read the first 4 of 5 calibration points
// 6. Use the new calibration points for calData[5] below
// 7. Reverse points 1 to 3
///////////////////////////////////////////////////////////////////////////////
uint16_t calData[5] = {608, 3138, 471, 3158, 5};    // **[1]**
//uint16_t calData[5];                              // **[2]**
// Replace above line with the number on display once calibration is complete
uint16_t t_x = 0, t_y = 0; // Touch coordinates

// 65 x 4 = 260 + 12 x 5 = 320  // Very close to RHSide some Backlight bleed into Volume Buttons 70 x 4 = 280 + 8 x 5 = 320
// 60 x 3 = 180 + 15 x 4 = 240
// Key sizes and spacing
#define KEY_W 65 // Key width
#define KEY_H 60 // Key height
#define KEY_SPACING_X 12    // 12 // X gap
#define KEY_SPACING_Y 15    // 15 // Y gap
// Keypad start position
// #define KEY_X (KEY_W/2) + KEY_SPACING_X   // 65/2 + 12 = 45 X-axis centre first key
// #define KEY_Y (KEY_H/2) + KEY_SPACING_Y   // 60/2 + 15 = 45 Y-axis centre first key
#define KEY_X 32                             // 30 is off-screen on left
#define KEY_Y 36                             // 30 is the topmost with no cut-off

#define KEY_TEXTSIZE 1   // Font size multiplier

// We have a status line for messages
#define STATUS_X 150 // Centred on this - 160 not middle buttons are off-centre
#define STATUS_Y 222 // 224 bottom of y p etc cut-off 

// Adding a delay between keypressing to give our OS time to respond
uint8_t keydelay = 50;        // keycode[1,..,6] delay milliseconds
uint8_t keydelay2 = 25;       // keyPress delay
uint16_t keydelay3 = 500;     // used in UAC confirm

// Create the screen object
TFT_eSPI tft = TFT_eSPI();

//////////////////////////////////////////////////////
// RGB565 Color Picker
// http://www.barth-dev.de/online/rgb565-color-picker/
//////////////////////////////////////////////////////
#define Black    0x0000      /*   0,   0,   0 */
#define Navy     0x000F      /*   0,   0, 128 */
#define DGreen   0x03E0      /*   0, 128,   0 */
#define DCyan    0x03EF      /*   0, 128, 128 */
#define Maroon   0x7800      /* 128,   0,   0 */
#define Purple   0x780F      /* 128,   0, 128 */
#define Olive    0x7BE0      /* 128, 128,   0 */
#define LGrey    0xD69A      /* 211, 211, 211 */
#define DGrey    0x7BEF      /* 128, 128, 128 */
#define Blue     0x001F      /*   0,   0, 255 */
#define Green    0x07E0      /*   0, 255,   0 */
#define Cyan     0x07FF      /*   0, 255, 255 */
#define Red      0xF800      /* 255,   0,   0 */
#define Magenta  0xF81F      /* 255,   0, 255 */
#define Yellow   0xFFE0      /* 255, 255,   0 */
#define White    0xFFFF      /* 255, 255, 255 */
#define Orange   0xFDA0      /* 255, 180,   0 */
#define Green1   0xB7E0      /* 180, 255,   0 */
#define Green2   0x26C8      
#define Green3   0x1750
#define Green4   0x65C8
#define Green5   0x5527
#define Pink     0xFE19      /* 255, 192, 203 */ 
#define Brown    0x9A60      /* 150,  75,   0 */
#define Gold     0xFEA0      /* 255, 215,   0 */
#define Silver   0xC618      /* 192, 192, 192 */
#define SkyBlue  0x867D      /* 135, 206, 235 */
#define Violet   0x915C      /* 180,  46, 226 */

///////////////////////////////////////////////////////////////////////////////////
// Four layout sets of 12 button colours - use the colours defined in TFT_eSPI.h
// Modified by Caps-Scroll-Num Lock Keys cannot use const - 16x12 = 192 16bit int
// 16 groups of 4 columns by 3 rows i.e. Layout 1 - 4 each has 4 Groups 0 - 3
///////////////////////////////////////////////////////////////////////////////////
uint16_t Colours[16][12] = {Blue,    Red,     Green5,  Orange,    // Row 1 Normal L1 ///////////////////// 12 Keys group 0 12x4=48
                            LGrey,   LGrey,   LGrey,   Orange,    // Row 2 12 keys [0][0] to [0][11]
                            LGrey,   LGrey,   LGrey,   Orange,    // Row 3 [0][8] Layer1 Key8
                            Blue,    LGrey,   Red,     Orange,    // Navigation L2
                            LGrey,   Black,   LGrey,   Orange, 
                            Blue,    LGrey,   Red,     Orange,    // [1][8] 
                            Blue,    Red,     Green5,  Orange,    // Text Macros S1-S6 L3
                            SkyBlue, SkyBlue, SkyBlue, Orange, 
                            SkyBlue, SkyBlue, SkyBlue, Orange,    // [2][8]
                            Blue,    Red,     Green5,  Orange,    // Text Macros T1-T6 l4
                            Pink,    Pink,    Pink,    Orange, 
                            Pink,    Pink,    Pink,    Orange,    // [3][8]
                            Blue,    Red,     Green5,  Orange,    // Normal + VolMute OFF L1 ///////////// 12 Keys group 1 12x8=96
                            LGrey,   LGrey,   LGrey,   Violet, 
                            LGrey,   LGrey,   LGrey,   Orange,    // [4][8]
                            Blue,    LGrey,   Red,     Orange,    // Navigation + VolMute OFF L2
                            LGrey,   Black,   LGrey,   Violet, 
                            Blue,    LGrey,   Red,     Orange,    // [5][8]
                            Blue,    Red,     Green5,  Orange,    // Text Macros S1-S6 + VolMute OFF L3
                            SkyBlue, SkyBlue, SkyBlue, Violet, 
                            SkyBlue, SkyBlue, SkyBlue, Orange,    // [6][8]
                            Blue,    Red,     Green5,  Orange,    // Text Macros T1-T6 + VolMute OFF L4
                            Pink,    Pink,    Pink,    Violet, 
                            Pink,    Pink,    Pink,    Orange,    // [7][8]
                            Blue,    Red,     Green5,  Blue,     // Normal + Vol OFF L1 ///////////////// 12 Keys group 2 12x12=144
                            LGrey,   LGrey,   LGrey,   Orange, 
                            LGrey,   LGrey,   LGrey,   Red,      // [8][8]
                            Blue,    LGrey,   Red,     Blue,     // Navigation
                            LGrey,   Black,   LGrey,   Orange, 
                            Blue,    LGrey,   Red,     Red,      // [9][8]
                            Blue,    Red,     Green5,  Blue,     // Text Macros S1-S6
                            SkyBlue, SkyBlue, SkyBlue, Orange, 
                            SkyBlue, SkyBlue, SkyBlue, Red,      // [10][8]
                            Blue,    Red,     Green5,  Blue,     // Text Macros T1-T6
                            Pink,    Pink,    Pink,    Orange, 
                            Pink,    Pink,    Pink,    Red,      // [11][8]
                            Blue,    Red,     Green5,  Blue,     // Normal + Vol OFF VolMute OFF //////// 12 Keys group 3 12x16=192
                            LGrey,   LGrey,   LGrey,   Violet, 
                            LGrey,   LGrey,   LGrey,   Red,      // [12][8]
                            Blue,    LGrey,   Red,     Blue,     // Navigation + Vol OFF VolMute OFF 
                            LGrey,   Black,   LGrey,   Violet, 
                            Blue,    LGrey,   Red,     Red,      // [13][8]
                            Blue,    Red,     Green5,  Blue,     // Text Macros S1-S6 + Vol OFF VolMute OFF 
                            SkyBlue, SkyBlue, SkyBlue, Violet, 
                            SkyBlue, SkyBlue, SkyBlue, Red,      // [14][8]
                            Blue,    Red,     Green5,  Blue,     // Text Macros T1-T6 + Vol OFF VolMute OFF 
                            Pink,    Pink,    Pink,    Violet, 
                            Pink,    Pink,    Pink,    Red };    // [15][8]
                             
uint16_t keyColor[12] = {};

uint16_t BackgroundColor[4] = {Black, Black, Black, Black};   

uint16_t ButtonOutlineColor[4] = {White, White, White, White};

uint16_t CfgColour   = SkyBlue;  
uint16_t MediaColour = Orange;

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
#define BassUp  0x0152
#define BassDwn 0x0153
#define TrebUp  0x0154
#define TrebDwn 0x0155
#define PlayMed 0x00CD  // PlayPause
#define NxtMed  0x00B5  // Next
#define PrevMed 0x00B6  // Previous
#define StopMed 0x00B7  // Stop
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
#define KeyYes   0x1C // "Y" or "y"
////////////////////////////////////////////////////////////////////////////////////////
#define KF1  0x3A
#define KF12 0x45
#define BShl 0x31   // Backslash
#define FShl 0x38   // Slash
#define Ins  0x49
#define Tab  0x2B
#define Spc  0x2C
#define Esc  0x29
#define CtrL 0xE0
#define CtrR 0xE4
#define ShfL 0xE1
#define ShfR 0xE5
#define AltL 0xE2
#define AltR 0xE6
#define WinL 0xE3
#define WinR 0xE7
#define BckS 0x2A
#define DelK 0x4C
////////////////////////////////////////////////////////////////////////////////////////////////
// GUI + Ctrl + Shft + B      - Wake PC from black or blank screen:
// GUI + X                    - Windows shutdown options
// Ctrl + Shft + Escape       - open Task Manage
// Ctrl + Shft + N            - create new Folder
// GUI + I                    - open Settings
// AltL + ShftL + Numlock     - Mouse Keys on/off
// Shft 5 times               - Sticky Keys on/off
// Numlock 5 seconds          - Toggle Keys on/off
// ShftR 8 seconds            - Filter Keys on/off
// GUI + Ctrl + O             - On-Screen Keyboard
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Two Layers A B of two by four layout sets of 12 button labels <layer><layout><name>[<number-of-lables>][<number-of-chars-per-label]
// The number of chars per label include the termination char \0 => 3 character labels here
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  b = Layout - 1                               Layout 1 - 4  and b = 0 - 3 
//  b = 4,5,6,7      VolDisable)                 b = b + 8 
//  b = 8,9,10,11    MuteDisable                 b = b + 4 VolDisable AND MuteDisable  b = b + 12 
//  Labels[LayerAB][b][n][m]; 
//  Colours[b][n];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Same 16 groups of 4 columns by 3 rows i.e. Layout 1 - 4 has 4 Groups 0 - 3 used in Colours[16][12]
// < 2 Layers A B > < 4 Groups of 4 Layouts 1 - 4 > < 12 Keys > < 4 chars > chars include \0
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char Labels[2][16][12][4] =                                            
{"Cut", "Cpy", "Pst", "V+",  "M1",  "M2", "M3",  "Vo",  "M4",  "M5",  "M6",  "V-",  // L1 Mute ON Vol=ON   0     Group 0 Layer A
 "Hme", "Up",  "PgU", "V+",  "<",   "Cfg",">",   "Vo",  "End", "Dwn", "PgD", "V-",  // L2                  1
 "cX",  "cC",  "cV",  "V+",  "S1",  "S2", "S3",  "Vo",  "S4",  "S5",  "S6",  "V-",  // L3                  2
 "cX",  "cC",  "cV",  "V+",  "T1",  "T2", "T3",  "Vo",  "T4",  "T5",  "T6",  "V-",  // L4                  3
 "Cut", "Cpy", "Pst", "V+",  "M1",  "M2", "M3",  "L1",  "M4",  "M5",  "M6",  "V-",  // L1 Mute OFF Vol=ON    4    Group 1 Layer A 
 "Hme", "Up",  "PgU", "V+",  "<",   "Cfg",">",   "L2",  "End", "Dwn", "PgD", "V-",  //                       5
 "cX",  "cC",  "cV",  "V+",  "S1",  "S2", "S3",  "L3",  "S4",  "S5",  "S6",  "V-",  //                       6
 "cX",  "cC",  "cV",  "V+",  "T1",  "T2", "T3",  "L4",  "T4",  "T5",  "T6",  "V-",  //                       7
 "Cut", "Cpy", "Pst", "Del", "M1",  "M2", "M3",  "Vo",  "M4",  "M5",  "M6",  "Ret", // L1 Mute ON Vol=OFF  8      Group 2 Layer A
 "Hme", "Up",  "PgU", "Del", "<",   "Cfg",">",   "Vo",  "End", "Dwn", "PgD", "Ret", //                     9
 "cX",  "cC",  "cV",  "Del", "S1",  "S2", "S3",  "Vo",  "S4",  "S5",  "S6",  "Ret", //                     10
 "cX",  "cC",  "cV",  "Del", "T1",  "T2", "T3",  "Vo",  "T4",  "T5",  "T6",  "Ret", //                     11
 "Cut", "Cpy", "Pst", "Del", "M1",  "M2", "M3",  "L1",  "M4",  "M5",  "M6",  "Ret", // L1 Mute OFF Vol=OFF    12  Group 3 Layer A
 "Hme", "Up",  "PgU", "Del", "<",   "Cfg",">",   "L2",  "End", "Dwn", "PgD", "Ret", //                        13
 "cX",  "cC",  "cV",  "Del", "S1",  "S2", "S3",  "L3",  "S4",  "S5",  "S6",  "Ret", //                        14
 "cX",  "cC",  "cV",  "Del", "T1",  "T2", "T3",  "L4",  "T4",  "T5",  "T6",  "Ret", //                        15
 "Cut", "Cpy", "Pst", "V+",  "M7",  "M8", "M9",  "Vo",  "M10", "M11", "M12", "V-",  // L1 Mute ON Vol=ON          Group 0 Layer B 
 "Hme", "Up",  "PgU", "V+",  "<",   "Cfg",">",   "Vo",  "End", "Dwn", "PgD", "V-",  // L2
 "cX",  "cC",  "cV",  "V+",  "S7",  "S8", "S9",  "Vo",  "S10", "S11", "S12", "V-",  // L3
 "cX",  "cC",  "cV",  "V+",  "T7",  "T8", "T9",  "Vo",  "T10", "T11", "T12", "V-",  // L4
 "Cut", "Cpy", "Pst", "V+",  "M7",  "M8", "M9",  "L1",  "M10", "M11", "M12", "V-",  // L1 Mute OFF Vol=ON         Group 1 Layer B
 "Hme", "Up",  "PgU", "V+",  "<",   "Cfg",">",   "L2",  "End", "Dwn", "PgD", "V-",
 "cX",  "cC",  "cV",  "V+",  "S7",  "S8", "S9",  "L3",  "S10", "S11", "S12", "V-",
 "cX",  "cC",  "cV",  "V+",  "T7",  "T8", "T9",  "L4",  "T10", "T11", "T12", "V-",
 "Cut", "Cpy", "Pst", "Del", "M7",  "M2", "M9",  "Vo",  "M10", "M11", "M12", "Ret", // L1 Mute ON Vol=OFF         Group 2 Layer B
 "Hme", "Up",  "PgU", "Del", "<",   "Cfg",">",   "Vo",  "End", "Dwn", "PgD", "Ret",
 "cX",  "cC",  "cV",  "Del", "S7",  "S8", "S9",  "Vo",  "S10", "S11", "S12", "Ret",
 "cX",  "cC",  "cV",  "Del", "T7",  "T8", "T9",  "Vo",  "T10", "T11", "T12", "Ret",
 "Cut", "Cpy", "Pst", "Del", "M7",  "M8", "M9",  "L1",  "M10", "M11", "M12", "Ret", // L1 Mute OFF Vol=OFF        Group 3 Layer B 
 "Hme", "Up",  "PgU", "Del", "<",   "Cfg",">",   "L2",  "End", "Dwn", "PgD", "Ret",
 "cX",  "cC",  "cV",  "Del", "S7",  "S8", "S9",  "L3",  "S10", "S11", "S12", "Ret",
 "cX",  "cC",  "cV",  "Del", "T7",  "T8", "T9",  "L4",  "T10", "T11", "T12", "Ret"};
// 0      1      2      4      5      6     7      8      9      10     11     12
char keyLabel[12][4] = {};     
/////////////////////////////////////////////////////////////////////////////////////////////////
//  If Cfg Key pressed new options (new Labels and new Colours)
//  Labels[0/1][1,5,9,13][0-11][0-3]
//  "Hme", "Up",  "PgU", "Del/V+", "<",   "Cfg", ">",  "V0/L14",  "End", "Dwn", "PgD", "V-/Ret",
//  "L14", "  ",  "D-R", "      ", "AB",  "Sav", " ",  "      ",  "Del", "   ", "L12", "      "
/////////////////////////////////////////////////////////////////////////////////////////////////
const static char CfgLabel[12][4] =                                          
{"L14", "   ",  "D-R", "   ", "A-B",  "Sav", "Med",  "   ",  "Del", "   ", "L12", "   "};  // All 3 chars + \0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  If Media Controls Arrow Right Key pressed new options in different colour:
//                 "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn", "PgD", "V-/Ret"
// MediaLabel[x][] "Prv", "   ", "Nxt", "   ",    "P-P",  "   ",  "Stp", "   ",     "   ", "   ",  "  ", "   " 
// keyLabel[x][]    0             2                4               6                                            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// const static char MediaLabel[12][4] = 
// {"Prv", "   ", "Nxt", "   ",    "P-P",  "   ",  "Stp", "   ",     "   ", "   ",  "  ", "   "}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MediaLabel[12][4] = 
{"|< ", "   ", " >|", "   ",    ">||",  "   ",  "|||", "   ",     "   ", "   ",  "  ", "   "}; 

//////////////////////////////////////////////////////////////////////////////
// reset if not [Cfg] and [Home] [PageUp] [<] [>] [End] [PageDown] key pressed
//             StrButton[button]    0  1  2  3  4  5  6  7  8  9  10 11
//                       CfgOK      x     x     x  x  x     x     x 
static const byte CfgButton[12] = { 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0 };
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//              StrButton[button]               4  5  6     8  9  10                4  5  6     8  9  10
//                             c                0  1  2     3  4  5                 6  7  8     9  10 11
static const byte StrButton[24] = { 0, 0, 0, 0, 0, 1, 2, 0, 3, 4, 5, 0, 0, 0, 0, 0, 6, 7, 8, 0, 9, 10, 11, 0 };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Escape sequences \\ = backslash \? = ? \t = tab \b = backspace \" or \'  \x0A = hexadecimal number (byte)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char str1[] = "https://www.raspberrypi.com/products/raspberry-pi-pico/"; // Strlen is chars+1
const static char str2[] = "https://github.com/raspberrypi\n\r";
const static char str3[] = "email.1@email.com";                             
const static char str4[] = "email.2@email.com";
const static char str5[] = "sfc /scannow\n";  
const static char str6[] = "\n\r"; 
const static char str7[] = "cmd\r";                             
const static char str8[] = "winver\n";  
const static char str9[] = "cmd";
const static char str10[] = "Dism /Online /Cleanup-Image /RestoreHealth"; 
const static char str11[] = "Dism.exe /online /Cleanup-Image /StartComponentCleanup";                             
const static char str12[] = "Dism.exe /online /Cleanup-Image /StartComponentCleanup /ResetBase"; 

const char * str1to12[] = {str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12} ;

const int StrSize = 200;

char Str1[StrSize], Str2[StrSize], Str3[StrSize], Str4[StrSize],  Str5[StrSize],  Str6[StrSize];
char Str7[StrSize], Str8[StrSize], Str9[StrSize], Str10[StrSize], Str11[StrSize], Str12[StrSize];
char Ttr1[StrSize], Ttr2[StrSize], Ttr3[StrSize], Ttr4[StrSize],  Ttr5[StrSize],  Ttr6[StrSize];
char Ttr7[StrSize], Ttr8[StrSize], Ttr9[StrSize], Ttr10[StrSize], Ttr11[StrSize], Ttr12[StrSize];
char LayerStr[2][4][4] = {"L1A","L2 ","L3A","L4A","L1B","L2 ","L3B","L4B"};   //Start with 7 followed by the startup layer 1 to 4 L1-L4

char * Str1to12[] = {Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,Str11,Str12} ;
char * Ttr1to12[] = {Ttr1,Ttr2,Ttr3,Ttr4,Ttr5,Ttr6,Ttr7,Ttr8,Ttr9,Ttr10,Ttr11,Ttr12} ;
 
char inputString[200] = "Send new text through serial port start with 0 end with newline\n";  // String to hold incoming data for [M4] Key
bool StrOK = false;          // String is complete - ended with 0x0A
byte InBytes[256];           // Serial received        
bool ByteOK = false;         // Bytes received is complete - ended with 0x00
int InBytesLen = 0;          // Number of chars in InBytes
int inBytesType = 1;         // Type 1, 2, 3 = sequence, simulataneous, media

// Create 12 keys 
TFT_eSPI_Button key[12];

///////////////////////////////////////
// Setup
///////////////////////////////////////
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);   // Used for Capslock
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);
  /*
  // if (!Serial) { Serial.println("Card Mount Failed");
                    for (;;) { delay(200); digitalWrite(LED_BUILTIN, 1);
                               delay(300); digitalWrite(LED_BUILTIN, 0); } }
  
  */
  if (!LittleFS.begin()) 
     {LittleFS.format();
      LittleFS.begin(); }
      
  if (LittleFS.exists("MuteDisable")) MuteDisable = true; else MuteDisable = false;   // VolMute Key disabled L1-L4 enabled
  if (LittleFS.exists("VolDisable"))  VolDisable  = true;  else VolDisable = false;   // V+ V- disabled Delete-Return enabled
  if (LittleFS.exists("Layout"))      SaveLayout  = true;  else SaveLayout = false;   // Selected Saved Layout restored
  if (LittleFS.exists("LayerB"))      LayerAB     = 1;     else LayerAB     = 0;      // Layer A 1-6 or B 7-12 restored
  if (LittleFS.exists("MediaKeys"))   Media       = true;  else Media       = false;  // Media Keys in Layout 2 restored

  InitStr();  // Copy str# to Str# and Ttr# if no saved file else read Str# Ttr# from filesm
   
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB HID Composite");
  usb_hid.setReportCallback(NULL, hid_report_callback); // Activate report CapsScrollNum Lock indicators
  usb_hid.begin();
  while( !TinyUSBDevice.mounted() ) delay(1);

  // Initialise the TFT screen TFT_eSPI/TFT_eSPI.h
  tft.init();
  tft.setRotation(3);                     // Set the display orientation to 0, 1, 2 or 3
  tft.setFreeFont(&FreeSans12pt7b);       // use KEY_TEXTSIZE 1 
  //tft.setTextFont(1);                   // use KEY_TEXTSIZE 2
  tft.setTextSize(KEY_TEXTSIZE);
    
  BackLightOn = true;     // TFT init will turn it on
  
  //////////////////////////////////////////////
  // Uncomment for new calibration
  //////////////////////////////////////////////
  /*                                 // **[3]**
  touch_calibrate();
  tft.fillScreen(Black);
  tft.setCursor(20, 0);
  tft.setTextColor(White, Black);
  for (uint8_t i = 0; i < 5; i++)
  { tft.print(calData[i]);
    if (i < 4) tft.print(", ");
  }
  delay(8000);                      
  */                                 // **[4]**

  tft.setTouch(calData);   // void setTouch(uint16_t *data); // TFT_eSPI/Extensions/Touch.h 
  
  if (SaveLayout) Layout = 2; else Layout = 1;  // Layout 1 or 2
  ConfigKeyCount = 0;                           // Start up
  ConfigButtons(0);                             // Draw Buttons and Labels 0 = All 3 rows
  
  LastMillis = millis();                        // Initial start time
  VolMuteMillisLast = millis();                 // Layout Change Timeout
  VolMuteCount = 0;                             // Layout change
  
}

/////////////////////////////
// Main Loop
/////////////////////////////
void loop() {
    
  if (!MuteDisable)                                                // only cancel slow press if Vo not if L1-L4
     {VolMuteMillisNow = millis();                                 // to switch layouts must press [Vo] or [Lx] briskly
     if ((VolMuteMillisNow - VolMuteMillisLast) >= VolMuteTimer)   // test whether the period has elapsed
        {VolMuteCount = 0;                                         // VolMuteKey pressed Count = 0
         VolMuteMillisLast = VolMuteMillisNow; } }

  NowMillis = millis();   // get the current "time" (number of milliseconds since started)
  if ((NowMillis - LastMillis) >= TimePeriod) //test whether the period has elapsed
     {digitalWrite(LCDBackLight, LOW);        // Backlight Off
      //analogWrite(LCDBackLight, 128);       // Backlight Dimmed
      LastMillis = NowMillis;                 // Start Tiemer again
      RepLast = RepNow = NowMillis;           // Reset repeat key timer
      if (!MuteDisable) VolMuteCount = 0;     // VolMuteKey pressed Count = 0 if not L1-L4 keys active
      ConfigKeyCount = 0;                     // [Cfg][End] or [Cfg][Hme] sequences active
      status("");                             // Clear the status line
      BackLightOn = false;   }                // Until keypress    
    
  ///////////////////////////////////////////////////////////////////////////////////////////////////////  
  // uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600) in TFT_eSPI/Extensions/Touch.h
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  bool pressed = tft.getTouch(&t_x, &t_y, 300);                              // True if valid key pressed
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Check if any key coordinate boxes contain the touch coordinates - no multitouch
  // void TFT_eSPI_Button::press(bool p) { laststate = currstate; currstate = p; }
  // Shorter: key[b].press((pressed && key[b].contains(t_x, t_y)));
  // justPressed  Query whether the button was pressed since last checked state.  Returns True if was not-pressed before, now is.
  // justReleased Query whether the button was released since last checked state. Returns True if was pressed before, now is not. 
  // isPressed    Query whether the button is currently pressed.                  Returns True if pressed 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for (uint8_t b = 0; b < 12; b++) { if (pressed && key[b].contains(t_x, t_y)) key[b].press(true);   
                                                                         else  key[b].press(false); }                    
   
  // Check if any key changed state void press(bool p); bool isPressed() bool justPressed(); bool justReleased();
  RepNow = millis();   // get the current time
  for (uint8_t b = 0; b < 12; b++) { if (key[b].justReleased()) {key[b].drawButton(false); RepLast = RepNow; }                      // draw normal - code at release time
                                     if (key[b].justPressed())  {key[b].drawButton(true); { buttonpress(b); RepLast = millis(); }}  // draw invert - code at press time  
                                     else
                                     if (((RepNow - RepLast) >= RepTimePeriod) && (key[b].isPressed()))    // code while the button is held
                                          {buttonpress(b); }                                         
                                   } // Send the button number

  GetSerialChars();   // Check serial port for first char 0,1,2,3,4,5,6 then store file after that
 
} // main Loop

/////////////////////////////////////////////
// Check for new character strings or macros
/////////////////////////////////////////////
void GetSerialChars()
{ int i, n = 0;
  char *ChrPtr;
  bool Found = false;
  byte a, b, c = 0;
  char inChar;
  String NameStr;
  while ((Serial.available())&&(n<StrSize+1)&&(n<256))          // 200<256 
        {if (Found) { ChrPtr[n-1] = inChar; InBytes[n-1] = b; } // Skip the first char Found = true after next read
         b = Serial.read();
         inChar = char(b);       // inChar = (char)b;
         a = inChar - '0';       // -48 easy ASCII to number

         if ((a<7)&&(!Found)&&(n==0)) 
            {Found = true; n--;   // n can be minus
            if ((LayerAB==1)&&(a>0)) c = a + 5; else c = a - 1;                            // S1-S6=>S7-S12 T1-T6=>T7=T12
            if (Layout!=4) {if (a>0) {NameStr = STRname[c]; ChrPtr = Str1to12[c]; }        // Keys S1-S12
                                else { ChrPtr = inputString; NameStr = "inputStr";}   }    // Key [M4]
            if (Layout==4) {if (a>0) {NameStr = TTRname[c]; ChrPtr = Ttr1to12[c]; }        // Keys T1-T12
                                else { ChrPtr = inputString; NameStr = "inputStr";}   }  } // Key [M4]

            if ((a>6)&&(a<10)&&(!Found)&&(n==0))                                           // Non ASCII start string with char 7 - 9
               {Found = true; n--; inBytesType = a - 6; }                        // Type 1, 2, 3 as discussed in buttonpress() below
            n++; // max StrSize=200 even if first 0-6 char not found                              
            if ((Found)&&(n>0)&&(inChar=='\n')) { StrOK = true; ChrPtr[n-1] = '\0'; break;}  // inChar=='\n' exit inChar==0x0A
            if ((Found)&&(n>0)&&(b==0x00)) { ByteOK = true; break;}  // exit loop 
        }   // while loop
           
           if ((Found)&&(StrOK)) {DoFileStrings(StrOK, NameStr, ChrPtr);  // Save the text/num strings 
                                  status(ChrPtr); }                       // Display the new string
           if ((Found)&&(ByteOK)) {InBytesLen = n-1; status("Bytes Received"); for (i=0; i<n; i++) Serial.println(InBytes[i]); }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do Button Actions Layouts 1 to 4
// Three HID Keys action types: 
// (1) Keyboard Keys pressed in sequence - usb_hid.keyboardPress(RID_KEYBOARD, keyvar)      - keyvar single variable
// (2) Keyboard Keys pressed together    - usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode) - keycode 0 - 4 keys pressed simultaneously
// (3) Media Keys pressed in sequence    - usb_hid.sendReport16(RID_CONSUMER_CONTROL, mediakeyvar) - mediakeyvar single variable
// The challenge with macro instructions is then to know how to deal with each type (1) to (3) or any combination of (1) to (3)
// Generally if non-test macro has Shift, Alt, GUI, Ctrl as the first key then it will be of type (2) (at least initially)
// If any other first key then is a normal keyboard key it will be of type (1) initially
// If any other first media key then is type (3) initially 
// For non-text macros received via the serial port the type (1) (2) (3) will be decided by the first (discarded) character = 7, 8, 9
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void buttonpress(int button)
{
  byte keycode[6] = {0};  // simultaneous keys pressed in here
  byte i, n, a, b, c;     // 
  byte CfgOK ;            // Do Cfg actions
  
  if ( !usb_hid.ready() ) return; 
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // StrButton[button]                 4  5  6     8  9  10                4  5  6     8  9  10
  //                c                  0  1  2     3  4  5                 6  7  8     9  10 11
  //byte StrButton[24] = { 0, 0, 0, 0, 0, 1, 2, 0, 3, 4, 5, 0, 0, 0, 0, 0, 6, 7, 8, 0, 9, 10, 11, 0 };
  c = StrButton[button + (LayerAB*12)];
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////// Skip 1st any-key keypress which activates backlight
  LastMillis = millis();                 // Reset Backlight Timer if any key pressed                                        
  if (!BackLightOn)                      // Keypress only active if backlight on
     {digitalWrite(LCDBackLight, HIGH);  // Backlight On
      BackLightOn = true;                //
      return; }                          //

  if ((!MuteDisable)&&(VolMuteCount>0)&&(button!=7)) {VolMuteCount = 0; // Any other key resets Mute pressed count
                                      VolMuteMillisLast  = millis(); }  // Set Timer = now 5 seconds to select next Layout
  
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // reset if not [Cfg] and [Home] [PageUp] [<] [Cfg] [>] [End] [PageDown] key pressed
  // StrButton[button]      0  1  2  3  4  5  6  7  8  9  10 11
  //             CfgOK      x     x     x  x  x     x     x 
  // byte CfgButton[12] = { 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0 };
  CfgOK = CfgButton[button];
  if ((Layout==2) && (CfgOK==0) && (ConfigKeyCount>0)) {ConfigKeyCount = 0; status(""); } 
  ///////////////////////////////////////////////////////////////////////////////////////////////      
                                                        
  if ((Layout!=2)) {   // Layout 1 or 3 or 4   
  switch(button){
    case 0: ///////////////////////////////////////////////// Cut = control + x                             
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_X; // actually x not X unless Capslock ON
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 1: ///////////////////////////////////////////////// Copy = control + c
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_C;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 2: ////////////////////////////////////////////////// Paste = control + v
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_V;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); usb_hid.keyboardRelease(RID_KEYBOARD);
      break; 
    case 3: /////////////////////////////////////////////////// Volume Increase
      if (VolDisable) {keycode[0] = DelK;
                       usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(RID_KEYBOARD); break; }
    
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolUp); 
      delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
      break;
    case 4: // M1 S1 T1 ///////////////////////////////////////////////// Alt + PrintScreen - some mini keyboards cannot do this
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      keycode[0] = AltL; 
      keycode[1] = PrtScr;
      keycode[2] = 0x00; // Need this why? 
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay2);
      usb_hid.keyboardRelease(RID_KEYBOARD);            delay(keydelay2);
      status("PrtScr Current Window");
      break;
    case 5: // M2 S2 T2 ///////////////////////////////////////////////// Open admin powershell including UAC completion
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
                                                                                
      keycode[0] = HID_KEY_GUI_LEFT; // or use HID_KEY_GUI_LEFT 
      keycode[1] = HID_KEY_X;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);   delay(keydelay2); // GUI + X then A admin powershell i normal powershell g Computer Management
      usb_hid.keyboardRelease(RID_KEYBOARD);              delay(keydelay2); // Confirm with ArrowLeft Enter 
      usb_hid.keyboardPress(RID_KEYBOARD, 'A');           delay(keydelay2); // Can also use Alt + "Y" too confirm
      usb_hid.keyboardRelease(RID_KEYBOARD);              delay(keydelay3); // Can also open via Run GUI + R then Ctrl+Shft+Enter then Alt+Y
      keycode[0] = ArrLeft;
      keycode[1] = KeyEnter;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);   delay(keydelay2); 
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 6: // M3 S3 T3 /////////////////////////////////////////////// Open Run window with last coommand visible and selected
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }                   
                             
      keycode[0] = HID_KEY_GUI_RIGHT; // or use HID_KEY_GUI_LEFT do not use KEYBOARD_MODIFIER_LEFTGUI
      keycode[1] = HID_KEY_R;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay);
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 7: //////////////////////////////////////////////// Volume Mute and change Layouts
      if ((MuteDisable) && (VolMuteCount==0)) VolMuteCount++;
      VolMuteCount++;
      if (VolMuteCount==2) {Layout=2; ConfigButtons(1); delay(keydelay); break; }                      // Count = 2 Navigate
      if (VolMuteCount==3) {Layout=3; ConfigButtons(1); delay(keydelay); break; }                      // Count = 3 Text Strings
      if (VolMuteCount==4) {Layout=4; ConfigButtons(1); delay(keydelay); break; }                      // Count = 4 Text Strings
      if (VolMuteCount==5) {Layout=1; VolMuteCount = 0; ConfigButtons(1); delay(keydelay); break; }    // Count = 0 start again
      
      if (MuteDisable) break;  // No VolumeMute action [Cfg][Hme] sequences

      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolMute); 
      delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
      break;
    case 8: // M4 S4 T4 ///////////////////////////////////////////// Send a text or macro - could overide by sending serial 0text or macro
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //                                CtrL 0xE0 + ShfL 0xE1 + Esc 0x29
      //                                     224         225        41
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (ByteOK)       { keycode[0] = InBytes[0];     // 0xE0 224
                          keycode[1] = InBytes[1];     // 0xE1 225
                          keycode[2] = InBytes[2];      // 0x29 41
                          keycode[3] = KeyEnter;
                          usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);      delay(keydelay2);
                          usb_hid.keyboardRelease(RID_KEYBOARD);                 delay(keydelay2); ByteOK = false; break;} 
          /*
          ByteOK = false;  //Serial.println(InBytesLen); Serial.println(InBytes[0]);
                                           //Serial.println(InBytes[1]); Serial.println(InBytes[2]); Serial.println(InBytes[3]);
          for (n=0;  n<InBytesLen; n++) { if (InBytes[n]==224) {keycode[0] = CtrL;}     // 0xE0
                                      if (InBytes[n]==225) {keycode[1] = ShfL;}     // 0xE1
                                      if (InBytes[n]==41)  {keycode[2] = Esc; }  }  // 0x29
                                      keycode[3] = 0;
                                      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); 
                                      usb_hid.keyboardRelease(RID_KEYBOARD); break; }
                                      
          */                            
      for (n=0;  n<strlen(inputString); n++) {usb_hid.keyboardPress(RID_KEYBOARD, inputString[n]); delay(keydelay2);
                                              usb_hid.keyboardRelease(RID_KEYBOARD);               delay(keydelay2); } break;                                                          
    case 9: // M5 S5 T5 //////////////////////////////////////////// Open normal command prompt via Run window
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
                                                              
      keycode[0] = HID_KEY_GUI_LEFT; // Admin CMD/Powershell: GUI + R then cmd/powershell then instead of Enter use Ctrl+Shft+Enter then Alt+Y
      keycode[1] = HID_KEY_R;        // 
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(RID_KEYBOARD);                 delay(keydelay2);
      
      for (n=0;  n<strlen(str9); n++)                        // str7 Normal Cmd Prompt str8 winver
          {usb_hid.keyboardPress(RID_KEYBOARD, str9[n]);     delay(keydelay2);
           usb_hid.keyboardRelease(RID_KEYBOARD);            delay(keydelay2); }
      delay(keydelay3);
      keycode[0] = CtrL;     // Instead of just [Enter] use [Control + Shift + Enter] => Run as admin
      keycode[1] = ShfL;     // 
      keycode[2] = KeyEnter; // 
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(RID_KEYBOARD);                 delay(keydelay2);
      delay(keydelay3);
      keycode[0] = AltL;     // Alt + Y on UAC window instead of ArrowLeft+Enter
      keycode[1] = KeyYes;   // Y or y
      keycode[2] = KeyEnter; //
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(RID_KEYBOARD);                 delay(keydelay2);
      break;
    case 10: // M6 S6 T6 //////////////////////////////////////// M6 text or Enter key LayerAB==1 b = 5; else b =0; 
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++)     // [button-4+b] = 10 - 4 + 5 = 11
                           {usb_hid.keyboardPress(RID_KEYBOARD, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);        delay(keydelay2); } break; }
      
      // Open Task Manager ///////////////////////////////////////////////////////
      keycode[0] = CtrL;     // 0xE0 224
      keycode[1] = ShfL;     // 0xE1 225
      keycode[2] = Esc;      // 0x29 41
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(RID_KEYBOARD);                 delay(keydelay2); 
      // Enter Key ///////////////////////////////////////////////////////////////                                                        
      //usb_hid.keyboardPress(RID_KEYBOARD, '\r'); delay(keydelay);
      //usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 11: ////////////////////////////////////////// Volume decrease
      if (VolDisable) { usb_hid.keyboardPress(RID_KEYBOARD, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(RID_KEYBOARD); break; }
    
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolDown); delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
      break;
  }   // switch
  }   // if Layout=1 or Layout=2
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // #define ArrUp    0x52  #define ArrDown  0x51  #define ArrLeft  0x50  #define ArrRight 0x4F
  // #define KeyHome  0x4A  #define KeyEnd   0x4D  #define PageUp   0x4B  #define PageDwn  0x4E
  ///////////////////////////////////////////////////////////////////////////////////////////////
  else //if (Layout==2)    
  switch(button){
    case 0: // HOME - Media Previous ////////////////////////////////////////////////////////////////// 
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              MuteDisable = !MuteDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (MuteDisable) status("Volume Mute OFF"); else status("Volume Mute ON");
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(RID_CONSUMER_CONTROL, PrevMed); delay(keydelay);
                  usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0); break; }

      keycode[0] = KeyHome;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 1: // ARROW UP ////////////////////////////////////////////////
      keycode[0] = ArrUp;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 2: // PAGE UP /////////////////////////////////////////////////
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              VolDisable = !VolDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (VolDisable) status("Volume Keys OFF"); else status("Volume Keys ON");
                              ConfigButtons(1); break; }
                              
      if (Media) {usb_hid.sendReport16(RID_CONSUMER_CONTROL, NxtMed); delay(keydelay);
                  usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0); break; }

      keycode[0] = PageUp;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 3: // V+ or Delete //////////////////////////////////////
      if (VolDisable) {keycode[0] = DelK;
                       usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(RID_KEYBOARD); break; }
        
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolUp); 
      delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
      break;
    case 4: // ARROW LEFT //////////////////////////////////////////////
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              if (LayerAB==0) {LayerAB=1; status("Layer B");} else {LayerAB=0; status("Layer A");} 
                              //GetSysInfo(1); // Save state on Flash
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(RID_CONSUMER_CONTROL, PlayMed); delay(keydelay);
                  usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0); break; }
    
      keycode[0] = ArrLeft;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 5: // CONFIG //////////////////////////////////////////////////
      GetSysInfo(0); 
      if (ConfigKeyCount==1) {status("Config and Files Saved");           ConfigKeyCount = 0; }
                        else {status("L1-4 DelRet AB Save Med Del L1-2"); ConfigKeyCount++ ; } 
      ConfigButtons(1);
      break;
    case 6: // ARROW RIGHT /////////////////////////////////////////////
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              Media = !Media; if (Media) status("Media Keys ON"); else status("Media Keys OFF"); 
                              //GetSysInfo(1); // Save state on Flash
                              ConfigButtons(1); break; }
      
      if (Media) {usb_hid.sendReport16(RID_CONSUMER_CONTROL, StopMed); delay(keydelay);
                  usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0); break; }

      keycode[0] = ArrRight;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 7: // Vol Mute /////////////////////////////////////////////////////////
      if ((MuteDisable) && (VolMuteCount==0)) VolMuteCount++;
      VolMuteCount++;
      if (VolMuteCount==2) {Layout=2; ConfigButtons(1); delay(keydelay); break; }                      // Count = 2 Navigate
      if (VolMuteCount==3) {Layout=3; ConfigButtons(1); delay(keydelay); break; }                      // Count = 3 Text Strings
      if (VolMuteCount==4) {Layout=4; ConfigButtons(1); delay(keydelay); break; }                      // Count = 4 Text Strings
      if (VolMuteCount==5) {Layout=1; VolMuteCount = 0; ConfigButtons(1); delay(keydelay); break; }    // Count = 0 start again
      
      if (MuteDisable) break;  // No VolumeMute action [Cfg][Hme] sequences

      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolMute); 
      delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
      break;
    case 8: // END //////////////////////////////////////////////////////////////////////////////
      if (ConfigKeyCount==1) {status("Files deleted"); ConfigKeyCount-- ; DeleteFiles(); ConfigButtons(1); break; }
      
      keycode[0] = KeyEnd;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 9: // ARROW DOWN //////////////////////////////////////////////
      keycode[0] = ArrDown;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 10: // PAGE DOWN //////////////////////////////////////////////
      if (ConfigKeyCount==1) {ConfigKeyCount--; SaveLayout = !SaveLayout; GetSysInfo(1); // Save the state
                              if (SaveLayout) status("Startup Layout L2"); else status("Startup Layout L1"); ConfigButtons(1); break;}

      keycode[0] = PageDwn;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);
      break;
    case 11: // V- or Enter ///////////////////////////////////////
      if (VolDisable) { usb_hid.keyboardPress(RID_KEYBOARD, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(RID_KEYBOARD); break; }

      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolDown); 
      delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
      break;
  }   // Layout=2 button switch
}     // buttonpress

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw the keys ( 3 times 4 loops to create 12) Button Label 10 chars max
// The button number is the current column added to the current row times  the number of buttons per row. 
// First the row is multiplied by 4 and then the col is added.
// The first button is 0. Example: col = 2 (3rd column), row = 1 (2nd row) =>: 1 * 4 = 4 --> 4 + 2 = 6 (7th button).
// TFT_eSPI/Extensions/Button.h           x-middle,  y-middle,   width,      height,     outline,          fill,   text
// void initButton(TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
//                                uint16_t textcolor, char *label, uint8_t textsize);
// void drawButton(bool inverted = false, String long_name = "");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigButtons(uint8_t rowcount) {  // rowcount=0 all 4 rows rowcount=2 last row only  rowcount=1 no background redraw 
  int n, m, b;
  uint8_t rows = 3;  // 3 rows of labels
  /////////////////////////////////////////////////////////////////////////////////////////////
  // Four Layouts possible:  Layout=1 b=0   Layout=2 b=1   Layout=3 b=2   Layout=4 b=3
  /////////////////////////////////////////////////////////////////////////////////////////////
  b = Layout - 1;                              //  L=1/b=0  L=2/b=1  L=3/b=2 L=4/b=3 
    
  if (rowcount==0) tft.fillScreen(BackgroundColor[b]);      // All four layouts Black background
  if (rowcount==1) rowcount = 0;                            // No background refresh
  
  uint16_t OutlineColor = ButtonOutlineColor[b];
  
  if (MuteDisable) b = b + 4;                   // b = 4,5,6,7
  if (VolDisable)  b = b + 8;                   // b = 8,9,10,11 
  // (VolDisable) && (MuteDisable) b = b + 12   // b = 12,13,14,15
  
  for (n=0; n<12; n++) for (m=0; m<3; m++) keyLabel[n][m] = Labels[LayerAB][b][n][m]; // label always = 4 chars \0 terminate stringlen 1,2,3
  for (n=0; n<12; n++) keyColor[n] = Colours[b][n];

  b = Layout - 1;                               // reset b to 0 - 3
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  If Layout 2 [Cfg] Key pressed new options in different colour:
  //   "Hme", "Up",   "PgU", "Del/V+",  "<",    "Cfg", ">",    "V0/L14",  "End", "Dwn", "PgD", "V-/Ret"
  //   "L14", "   ",  "D-R", "   ",     "A-B",  "Sav", "Med",  "   ",     "Del", "   ",  "L12", "   " 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (ConfigKeyCount>0) 
     {for (m=0; m<3; m++) {keyLabel[0][m] = CfgLabel[0][m]; keyLabel[2][m] = CfgLabel[2][m]; keyLabel[4][m] = CfgLabel[4][m]; 
                           keyLabel[5][m] = CfgLabel[5][m]; keyLabel[6][m] = CfgLabel[6][m]; keyLabel[8][m] = CfgLabel[8][m]; 
                           keyLabel[10][m] = CfgLabel[10][m]; };
      keyColor[0] = keyColor[2] = keyColor[4] = keyColor[5] = keyColor[6] = CfgColour; 
      if (keyColor[8]!=Green2)  keyColor[8]  = CfgColour;                   // if Capslock
      if (keyColor[10]!=Green2) keyColor[10] = CfgColour; }                 // if Numlock

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  If Media Controls Arrow Right Key pressed new options in different colour:
  //                 "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn", "PgD", "V-/Ret"
  // MediaLabel[x][] "Prv", "   ", "Nxt", "   ",    "P-P",  "   ",  "Stp", "   ",     "   ", "   ",  "  ", "   " 
  // keyLabel[x][]    0             2                4               6                                            
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((Media)&&(b==1)&&(ConfigKeyCount==0)) {
     {for (m=0; m<3; m++) {keyLabel[0][m] = MediaLabel[0][m]; keyLabel[2][m] = MediaLabel[2][m]; 
                           keyLabel[4][m] = MediaLabel[4][m]; keyLabel[6][m] = MediaLabel[6][m]; };
      keyColor[0] = keyColor[2] = keyColor[4] = keyColor[6] = MediaColour; } }
            
  b = Layout - 1;                               // reset b to 0 - 3
              
  for (uint8_t row = 0 + rowcount; row < rows; row++) {   // 3 rows 0,1,2 => b = 0+(0*4)=0 1+(0*4)=1 ... 0+(1*4)=4 ....
    for (uint8_t col = 0; col < 4; col++) {               // 4 columns 0,1,2,3
        b = col + row * 4;
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X), KEY_Y + row * (KEY_H + KEY_SPACING_Y), 
                                               KEY_W, KEY_H, OutlineColor, keyColor[b], White, keyLabel[b], KEY_TEXTSIZE);
        key[b].drawButton();  }  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Output report callback for LED indicator such as Caplocks
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define key8LGrey Colours[0][8]=Colours[4][8]=Colours[8][8]=Colours[12][8]=LGrey 
#define key9LGrey Colours[0][9]=Colours[4][9]=Colours[1][9]=Colours[5][9]=Colours[8][9]=Colours[9][9]=Colours[12][9]=Colours[13][9]=LGrey 
#define key10LGrey Colours[0][10]=Colours[4][10]=Colours[8][10]=Colours[12][10]=LGrey 
#define key8SkyBlue Colours[2][8]=Colours[6][8]=Colours[10][8]=Colours[14][8]=SkyBlue 
#define key9SkyBlue Colours[2][9]=Colours[6][9]=Colours[10][9]=Colours[14][9]=SkyBlue 
#define key10SkyNlue Colours[2][10]=Colours[6][10]=Colours[10][10]=Colours[14][10]=SkyBlue 
#define key8Pink Colours[3][8]=Colours[7][8]=Colours[11][8]=Colours[15][8]=Pink 
#define key9Pink Colours[3][9]=Colours[7][9]=Colours[11][9]=Colours[15][9]=Pink 
#define key10Pink Colours[3][10]=Colours[7][10]=Colours[11][10]=Colours[15][10]=Pink 
#define key8Blue Colours[1][8]=Colours[5][8]=Colours[9][8]=Colours[13][8]=Blue 
#define key10Red Colours[1][10]=Colours[5][10]=Colours[9][10]=Colours[13][10]=Red
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) report_id;
  (void) bufsize;
  bool CapsLockPrev = CapsLock;            
  bool NumLockPrev = NumLock;             
  bool ScrollLockPrev = ScrollLock; 
  bool Change = false; 
  int i;         
  
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
 
  Change = ((CapsLock!=CapsLockPrev)||(NumLock!=NumLockPrev)||(ScrollLock!=ScrollLockPrev));

  if (Change) 
     { if (CapsLock)   {for (i=0; i<16; i++) Colours[i][8]  = Green2;} else {key8LGrey;  key8SkyBlue;  key8Pink;  key8Blue;}
       if (NumLock)    {for (i=0; i<16; i++) Colours[i][9]  = Green2;} else {key9LGrey;  key9SkyBlue;  key9Pink;}
       if (ScrollLock) {for (i=0; i<16; i++) Colours[i][10] = Green2;} else {key10LGrey; key10SkyNlue; key10Pink; key10Red;}
       ConfigButtons(2);}   // 2 => only last row redraw Buttons and Labels  

   if ((Change)&&(!BackLightOn))  { digitalWrite(LCDBackLight, HIGH); // Turn backlight on if Caps-Num-Scroll lock change
                                    BackLightOn = true; }
}

////////////////////////////////////////////////////////////////////////////////////////
// Code to run a screen calibration, not needed when calibration values set in setup()
////////////////////////////////////////////////////////////////////////////////////////
void touch_calibrate(){
  tft.fillScreen(Black);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(White, Black);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, Magenta, Black, 15);

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

  tft.fillScreen(Black);
  
  tft.setTextColor(Green, Black);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(400);
}

//////////////////////////////////////////////////////////////////
//  strcpy(Str1,str1);
//  for (n=0; n<6; n++) strcpy(Str1to6[n], str1to6[n]);
//////////////////////////////////////////////////////////////////
void InitStr()
{ char *ChrPtr;
  String NameStr;
  int n = 0;
  
  for (n=0; n<12; n++) 
      {NameStr = STRname[n]; ChrPtr = Str1to12[n];  // n=0 STRname[0] Str1to12[0] str1to12[0]
       if (!LittleFS.exists(NameStr)) strcpy(ChrPtr, str1to12[n]); else DoFileStrings(StrOK, NameStr, ChrPtr);  }
  
  for (n=0; n<12; n++) 
      {NameStr = TTRname[n]; ChrPtr = Ttr1to12[n];
       if (!LittleFS.exists(NameStr)) strcpy(ChrPtr, str1to12[n]); else DoFileStrings(StrOK, NameStr, ChrPtr); }

  if (LittleFS.exists("inputStr")) {ChrPtr = inputString; DoFileStrings(StrOK, "inputStr", ChrPtr);}
}

//////////////////////////////////////////////////////////////////
// case 1:  ChrPtr = Ttr1; NameStr = TTR1f; 
// if (Found) DoFileStrings(StrOK, NameStr,   ChrPtr); 
//            DoFileStrings(StrOK, TTR1f, ChrPtr); 
//////////////////////////////////////////////////////////////////
void DoFileStrings(bool DoWrite, String STRf,  char *ChrPtr)
{ uint16_t StrLen;
      
  if (!DoWrite)                               // read data
     {File f = LittleFS.open(STRf, "r");
     //StrLen = f.available(); 
     //StrLen = strlen(Str1); 
     StrLen = f.size();
     f.readBytes((char *)ChrPtr, StrLen);
     f.close(); }
     //Str1[StrLen+1]='\0'; }
  
  if (DoWrite)  // write data
     {File f = LittleFS.open(STRf, "w");
      StrLen = strlen(ChrPtr);
      f.print(ChrPtr);
      f.print('\0');
      f.close();  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LittleFS supports directories. Path + filename  can only have a maximum of 31 characters 
// Dir dir = LittleFS.openDir("/");
// while (dir.next()) {
//        Serial.print(dir.fileName());
//        File f = dir.openFile("r");
//        Serial.println(f.size()); }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ListFiles()
{ int n = 1;
  File root = LittleFS.open("/", "r");
  File file = root.openNextFile();
  while(file){
      Serial.print(n);
      Serial.print(" ");
      Serial.println(file.name());
      n++;
      file = root.openNextFile();
  }
}

void DeleteFiles()  // delete text/number macros and config
{ if (LittleFS.format()); {LittleFS.begin();
                           Serial.println("\nAll files deleted");}
}
 
void GetSysInfo(int Action) // Also save config 
{  char *ChrPtr;
  
  int fHeap = rp2040.getFreeHeap();
  int uHeap = rp2040.getUsedHeap();
  int tHeap = rp2040.getTotalHeap();
  int fCPU  = rp2040.f_cpu();
  
  if (Action==1) {if (SaveLayout) { char LayoutF[2]  = "1"; ChrPtr = LayoutF; DoFileStrings(1, "Layout",  ChrPtr); }
                  else LittleFS.remove("Layout"); return;}
           
  if (VolDisable)  { char VolD[2]   = "1"; ChrPtr = VolD;  DoFileStrings(1, "VolDisable",   ChrPtr); }
          else LittleFS.remove("VolDisable");
      
  if (MuteDisable) { char MuteD[2]  = "1"; ChrPtr = MuteD; DoFileStrings(1, "MuteDisable",  ChrPtr); }
         else LittleFS.remove("MuteDisable"); 

  if (LayerAB==1) { char LayerD[2]  = "1"; ChrPtr = LayerD; DoFileStrings(1, "LayerB",  ChrPtr); }
         else LittleFS.remove("LayerB"); 

  if (Media) { char LayerD[2]  = "1"; ChrPtr = LayerD; DoFileStrings(1, "MediaKeys",  ChrPtr); }
         else LittleFS.remove("MediaKeys");
    
  Serial.println("VolumeMacroKeys version 148.0");
  Serial.printf("CPU MHz: %d\n\r", fCPU);
  Serial.printf("FreeHeap: %d\n\r", fHeap);
  Serial.printf("UsedHeap: %d\n\r", uHeap);
  Serial.printf("TotalHeap: %d\n\r", tHeap);
  Serial.printf("Core temperature: %2.1fC\n\r", analogReadTemp());
  ListFiles();
  
}

/////////////////////////////////////////////////
// Display mini status bar
// STATUS_X 150 // Centred on this 320/2=160
// STATUS_Y 222 // bottom
/////////////////////////////////////////////////
void status(const char *msg) {
  tft.setTextPadding(320);
  //tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(LGrey, Black);
  tft.setFreeFont(&FreeSans9pt7b);
  tft.setTextDatum(1);
  //tft.setTextSize(1);
  tft.drawString(msg, STATUS_X, STATUS_Y);
  tft.setFreeFont(&FreeSans12pt7b);         // KEY_TEXTSIZE 1 
  //tft.setTextSize(KEY_TEXTSIZE);
}  
