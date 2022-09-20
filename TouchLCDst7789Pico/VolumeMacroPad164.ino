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
/************************************************************************************
 Adafruit invests time and resources providing this open source code, please support 
 Adafruit and open-source hardware by purchasing products from Adafruit
 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
*************************************************************************************/
/****************************************************************************************************
Layout 1  Cycle through Layout 1 to 4 press VolumeMute [L1-L4] once or [Vo] 3 or more times
------------------------------------------------------------------------------------------------
[Cut Ctrl+X] [  Copy Ctrl+C   ] [Paste Ctrl+V] [VolUp  Del-Bs]    [cX]  [cC]  [cV]  [V+][Del-Bs]
[Alt+PrtScr] [Admin Powershell] [ Run window ] [VolMute L1-L4]    [M1]  [M2]  [M3]  [Vo][L1-L4 ]
[Send Text ] [Admin CMD Prompt] [Task Manager] [VolDwn  Enter]    [M4]  [M5]  [M6]  [V-][Enter ]
          Caps                Num           Scroll                    C     N     S
Layout 2  Cycle through Layout 1 to 4 press VolumeMute [L1-L4] once or [Vo] 3 or more times
--------------------------------------------------------------------------------------------------
[Home   Prev  Mute] [UpArr  BsDel] [PgeUp Nxt DelRet] [VolUp  Delete] [H] [Up ] [Pu] [V+][Del-Bs]
[ArrL PlayPse L-AB] [Config  Save] [ArrR  Stop Media] [VolMute L1-L4] [<] [Cfg] [> ] [Vo][L1-L4 ]
[End    CfgFileDel] [DwnArr KeyBr] [PgeDwn     L1-L2] [VolMute Enter] [E] [Dw ] [Pd] [V-][Enter ]
                 Caps            Num               Scroll                C     N    S
Layout 3+4 Cycle through Layout 1 to 4 press VolumeMute [L1-L4] once or [Vo] 3 or more times
--------------------------------------------------------------------------------------------------
[Cut Ctrl+X ] [Copy Ctrl+C] [Paste Ctrl+V] [VolUp  Del-Bs]   [ cX ]  [ cC  ]  [ cV  ] [V+][Del-Bs]
[Send Text 1] [Send Text 2] [Send Text 3 ] [VolMute L1-L4]   [S1 T1] [S2 T2]  [S3 T3] [Vo][L1-L4 ]
[Send Text 4] [Send Text 5] [Send Text 6 ] [VolMute Enter]   [S4 T4] [S5 T5]  [S6 T6] [V-][Enter ]
           Caps           Num           Scroll                      C       N        S
Cycle through Layout 1 to 4 press VolumeMute [L1-L4] once or [Vo] 3 or more times
-------------------------------------------------------------------------------------------------
Layouts 1, 3 and 4, two Layers A/B each with A=M1-M6, S1-S6, T1-T6 and B=M7-M12, S7-S12, T7-T12
Press VolumeMute consecutively within 5 seconds else count is reset to 0 or press [L1-L4] once.
[L1-L4] has a repeat period of 0.5 seconds - after this key repeat is active

Press Config Key (new active keys change colour) then:
L14 (Home) - VolumeMute -> L1, L2, L3, L4 - repeat to restore Vo
BsD (ArrowUp) - Toggle Vol+ key to either Delete or Backspace
D-R (PageUp) - Vol+ -> Delete and Vol- -> Enter - repeat to restore V+ V-
A-B (ArrowLeft) - Layouts 1, 3, 4, change to Layer A or Layer B 
Med (ArrowRight) - Change Layout 2 to Media Controls Previous-Next-PlayPause-Stop
Del (End) - Delete all files on Flash (Strings and Config)
L12 (PageDown) - Layout 1 or Layout 2 on powerup - must also press Sav(e) (Cfg)
Kbd (ArrowDwn] - A-Z 0-9 keyboard send keys. ADD to list NXT keybrd ESC quit EXE send + quit
Sav (Cfg) Info and File List to sent Serial Monitor and Text/Macro and Config files saved

Text Strings: 
Send new text strings up to 200 characters to keys S1/T1 - S12/T12 via USBserial
Start string with <1 to <6 followed by the string to be assigned to S1/T1-S12/T12 - end string with >
If current Layout is L3 then S1 to S6 changed if Layout is L4 then T1 to T6 changed
If current Layer is A then S1/T1-S6/T6 changed, layer B S7/T7-S12/T12 changed
Send new Text for Key [M4] - start with <0 end with >

Keyboard:
[abc] [def] [ghi] [ESC]                     [ALT] [SHF] [CTR] [EXE]
[jkl] [mno] [pqr] [NXT] -> 3 other pages -> [GUI] [TEI] [CRF] [NXT]                       
[stu] [vwx] [yz ] [ADD]                     [LHR] [UED] [PUD] [ADD] 

for eample SHF = Shift-L Shift-R Delete - the name of the sub-key is shown on the status line

The keyboard is has 5 circular pages each with 9 triple function keys and 3 control keys [ESC-EXE] [NXT] [ADD].
As an example the first page has keys [abc], [def], to [y,z,space]. To select a or b or c press the abc key once,
twice or thrice - to add it to the macro press the ADD key else press another macro key or press NXT for the next
keyboard page. Press ESC(ape) to leave the keyboard or on page 5 press EXE to send the macro to the PC. The macro's
on page 5 are simultaneous pressed keys such as Control + Alt + Delete (max 6 keys in macro) To send this sequence
press [CTR] [ADD] [ALT] [ADD] [SHF]x3 (3 times for del function) [ADD] [EXE]. To send a sequence such as WinKey + r
press [GUI] [ADD] [NXT] [pqr]x3 [ADD] [NXT]x4 [EXE]. Other macros from pages 1 to 4 can be up to 200 characters.
Macros sent to the PC from the builtin keyboard will be saved to a file KeyBrdMacro2 if of simultaneous type 
6 keys max pressed together) Macros sent to the PC from the built-in keyboard will be saved as KeyBrdMacro1 if of
sequence type (200 chars max) Combining macros of both types will be added later. Assigning these macros to 
M1 - M12 will be added later
***********************************************************************************************************************************/
#include "Adafruit_TinyUSB.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include <FS.h>

// Custom Strings file
#define STR1f  "StrData1"
#define STR2f  "StrData2"
#define STR3f  "StrData3"
#define STR4f  "StrData4"
#define STR5f  "StrData5"
#define STR6f  "StrData6"
#define STR7f  "StrData7"
#define STR8f  "StrData8"
#define STR9f  "StrData9"
#define STR10f "StrData10"
#define STR11f "StrData11"
#define STR12f "StrData12"

#define TTR1f  "TtrData1"
#define TTR2f  "TtrData2"
#define TTR3f  "TtrData3"
#define TTR4f  "TtrData4"
#define TTR5f  "TtrData5"
#define TTR6f  "TtrData6"
#define TTR7f  "TtrData7"
#define TTR8f  "TtrData8"
#define TTR9f  "TtrData9"
#define TTR10f "TtrData10"
#define TTR11f "TtrData11"
#define TTR12f "TtrData12"

#define MTR1f  "MtrData1"
#define MTR2f  "MtrData2"
#define MTR3f  "MtrData3"
#define MTR4f  "MtrData4"
#define MTR5f  "MtrData5"
#define MTR6f  "MtrData6"
#define MTR7f  "MtrData7"
#define MTR8f  "MtrData8"
#define MTR9f  "MtrData9"
#define MTR10f "MtrData10"
#define MTR11f "MtrData11"
#define MTR12f "MtrData12"

char STRname [12][11] = { STR1f, STR2f, STR3f, STR4f, STR5f, STR6f, STR7f, STR8f, STR9f, STR10f, STR11f, STR12f };
char TTRname [12][11] = { TTR1f, TTR2f, TTR3f, TTR4f, TTR5f, TTR6f, TTR7f, TTR8f, TTR9f, TTR10f, TTR11f, TTR12f };
char MTRname [12][11] = { MTR1f, MTR2f, MTR3f, MTR4f, MTR5f, MTR6f, MTR7f, MTR8f, MTR9f, MTR10f, MTR11f, MTR12f };

int LCDBackLight = 13; // Will be set as OUTPUT and HIGH by TFT init

// Report ID
enum
{
  RID_KEYBOARD = 1,
  RID_MOUSE,
  RID_CONSUMER_CONTROL, // Media, volume etc ..
};

// HID report descriptor using TinyUSB's template
uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(RID_KEYBOARD) ),
  TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(RID_MOUSE) ),
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
int LayerAB = 0;                       // Switch Layout  A or B for Layers 1, 3, 4 M1-M6/M7-M12 etc - press [Sav] to save
bool CapsLock = false;                 // Green "C"
bool NumLock = false;                  // Green "N"
bool ScrollLock = false;               // Green "S" - Excel still use this
bool Change = false;                   // Indicators changed
int  ConfigKeyCount = 0;               // Start with Layout 2 Cfg key not pressed 
bool MuteDisable = false;              // Disable VolMute if pressed Layout switch
bool VolDisable = false;               // Disable Vol+ Vol- keys now DEL ENTER
bool SaveLayout = false;               // Start with selected layout 
bool Media = false;                    // Four Media Keys in Layout 2
bool DelBS = false;                    // Delete or Backspace Vol+
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Layout 1 to 4 Keys
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KEY_TEXTSIZE 1   // Font size multiplier

// Messages status line at bottom
#define STATUS_X 150 // Centred on this - 160 not middle buttons are off-centre
#define STATUS_Y 222 // 224 bottom of y p etc cut-off 

// Indicators status Caps-Num-Scroll C N S XPos[7] = {0, 0, 78, 156, 0, 78, 156};
#define IND_X 70  // Centred on this - Sppace in RH side about 16 pixels
#define IND_Y 139 // 

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
#define Black    0x0000      //   0,   0,   0 
#define Navy     0x000F      //   0,   0, 128 
#define DGreen   0x03E0      //   0, 128,   0 
#define DCyan    0x03EF      //   0, 128, 128 
#define Maroon   0x7800      // 128,   0,   0 
#define Purple   0x780F      // 128,   0, 128 
#define Olive    0x7BE0      // 128, 128,   0 
#define LGrey    0xD69A      // 211, 211, 211 
#define DGrey    0x7BEF      // 128, 128, 128 
#define Blue     0x001F      //   0,   0, 255 
#define Green    0x07E0      //   0, 255,   0 
#define Cyan     0x07FF      //   0, 255, 255 
#define Red      0xF800      // 255,   0,   0 
#define Magenta  0xF81F      // 255,   0, 255 
#define Yellow   0xFFE0      // 255, 255,   0 
#define White    0xFFFF      // 255, 255, 255 
#define Orange   0xFDA0      // 255, 180,   0 
#define Green1   0xB7E0      // 180, 255,   0 
#define Green2   0x26C8      
#define Green3   0x1750
#define Green4   0x65C8
#define Green5   0x5527
#define Pink     0xFE19      // 255, 192, 203  
#define Brown    0x9A60      // 150,  75,   0 
#define Gold     0xFEA0      // 255, 215,   0 
#define Silver   0xC618      // 192, 192, 192 
#define SkyBlue  0x867D      // 135, 206, 235 
#define Violet   0x915C      // 180,  46, 226 

///////////////////////////////////////////////////////////////////////////////////
// Four layout sets of 12 button colours - use the colours defined in TFT_eSPI.h
// Modified by Caps-Scroll-Num Lock Keys cannot use const - 16x12 = 192 16bit int
// 16 groups of 4 columns by 3 rows i.e. Layout 1 - 4 each has 4 Groups 0 - 3
///////////////////////////////////////////////////////////////////////////////////
#define BrG    Blue,    Red,     Green5
#define LG3    LGrey,   LGrey,   LGrey
#define BLR    Blue,    LGrey,   Red
#define LbL    LGrey,   Black,   LGrey
#define SB3    SkyBlue, SkyBlue, SkyBlue
#define Pk3    Pink,    Pink,    Pink

uint16_t Colours[16][12] = {
BrG, Orange,   // Row 1 Normal L1 ///////////////////// 12 Keys group 0 12x4=48
LG3, Orange,   // Row 2 12 keys [0][0] to [0][11]
LG3, Orange,   // Row 3 [0][8] Layer1 Key8
BLR, Orange,   // Navigation L2
LbL, Orange, 
BLR, Orange,   // [1][8] 
BrG, Orange,   // Text Macros S1-S6 L3
SB3, Orange, 
SB3, Orange,   // [2][8]
BrG, Orange,   // Text Macros T1-T6 l4
Pk3, Orange, 
Pk3, Orange,   // [3][8]
BrG, Orange,   // Normal + VolMute OFF L1 ///////////// 12 Keys group 1 12x8=96
LG3, Violet, 
LG3, Orange,   // [4][8]
BLR, Orange,   // Navigation + VolMute OFF L2
LbL, Violet, 
BLR, Orange,   // [5][8]
BrG, Orange,   // Text Macros S1-S6 + VolMute OFF L3
SB3, Violet, 
SB3, Orange,   // [6][8]
BrG, Orange,   // Text Macros T1-T6 + VolMute OFF L4
Pk3, Violet, 
Pk3, Orange,   // [7][8]
BrG, Blue,     // Normal + Vol OFF L1 ///////////////// 12 Keys group 2 12x12=144
LG3, Orange, 
LG3, Red,      // [8][8]
BLR, Blue,     // Navigation
LbL, Orange, 
BLR, Red,      // [9][8]
BrG, Blue,     // Text Macros S1-S6
SB3, Orange, 
SB3, Red,      // [10][8]
BrG, Blue,     // Text Macros T1-T6
Pk3, Orange, 
Pk3, Red,      // [11][8]
BrG, Blue,     // Normal + Vol OFF VolMute OFF //////// 12 Keys group 3 12x16=192
LG3, Violet, 
LG3, Red,      // [12][8]
BLR, Blue,     // Navigation + Vol OFF VolMute OFF 
LbL, Violet, 
BLR, Red,      // [13][8]
BrG, Blue,     // Text Macros S1-S6 + Vol OFF VolMute OFF 
SB3, Violet, 
SB3, Red,      // [14][8]
BrG, Blue,     // Text Macros T1-T6 + Vol OFF VolMute OFF 
Pk3, Violet, 
Pk3, Red };    // [15][8]
                             
uint16_t keyColor[12] = {};

uint16_t BackgroundColor[4] = {Black, Black, Black, Black};   

uint16_t ButtonOutlineColor[4] = {White, White, White, White};

uint16_t CfgColour   = SkyBlue;  
uint16_t MediaColour = Orange;

//////////////////////////////////////////////////////////////////////////////////////////
// Consumer keys are 16 bits - names are long to use will use hex values or short alias
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
////////////////////////////////////////////////////////////////////////////////////////
#define KeyCut   0x7B   // Ctrl + X
#define KeyCopy  0x7C   // Ctrl + C
#define KeyPaste 0x7D   // Ctrl + V
#define PrtScr   0x46
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
#define KeyYes   0x1C // "Y" or "y" note ACII Y = 0x59 y = 0x79 (difference of 61 decimal)
///////////////////////////////////////////////////////////////////////////////////////////
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
// Ctrl + Shft + Escape       - open Task Manager
// Ctrl + Shft + N            - create new Folder
// GUI + I                    - open Settings
// AltL + ShftL + Numlock     - Mouse Keys on/off
// Shft 5 times               - Sticky Keys on/off
// Numlock 5 seconds          - Toggle Keys on/off
// ShftR 8 seconds            - Filter Keys on/off
// GUI + Ctrl + O             - On-Screen Keyboard
// Shift + F10                - Menu (Mouse Right-click)
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
#define CcP    "Cut", "Cpy", "Pst"
#define XCV    "cX",  "cC",  "cV"
#define HPU    "Hme", "Up",  "PgU"
#define LcR    "<",   "Cfg", ">"
#define EPD    "End", "Dwn", "PgD"
#define S13    "S1",  "S2",  "S3"
#define S46    "S4",  "S5",  "S6"
#define T13    "T1",  "T2",  "T3"
#define T46    "T4",  "T5",  "T6"
#define M13    "M1",  "M2",  "M3"
#define M46    "M4",  "M5",  "M6"
#define S79    "S7",  "S8",  "S9"
#define S12    "S10", "S11", "S12"
#define T79    "T7",  "T8",  "T9"
#define T12    "T10", "T11", "T12"
#define M79    "M7",  "M8",  "M9"
#define M12    "M10", "M11", "M12"

const static char Labels[2][16][12][4] =                                            
{CcP,   "V+",  M13,  "Vo",  M46,   "V-",  // L1 Mute ON Vol=ON   0     Group 0 Layer A
 HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2                  1
 XCV,   "V+",  S13,  "Vo",  S46,   "V-",  // L3                  2
 XCV,   "V+",  T13,  "Vo",  T46,   "V-",  // L4                  3
 CcP,   "V+",  M13,  "L1",  M46,   "V-",  // L1 Mute OFF Vol=ON    4    Group 1 Layer A 
 HPU,   "V+",  LcR,  "L2",  EPD,   "V-",  //                       5
 XCV,   "V+",  S13,  "L3",  S46,   "V-",  //                       6
 XCV,   "V+",  T13,  "L4",  T46,   "V-",  //                       7
 CcP,   "BsD", M13,  "Vo",  M46,   "Ent", // L1 Mute ON Vol=OFF  8      Group 2 Layer A
 HPU,   "BsD", LcR,  "Vo",  EPD,   "Ent", //                     9
 XCV,   "BsD", S13,  "Vo",  S46,   "Ent", //                     10
 XCV,   "BsD", T13,  "Vo",  T46,   "Ent", //                     11
 CcP,   "BsD", M13,  "L1",  M46,   "Ent", // L1 Mute OFF Vol=OFF    12  Group 3 Layer A
 HPU,   "BsD", LcR,  "L2",  EPD,   "Ent", //                        13
 XCV,   "BsD", S13,  "L3",  S46,   "Ent", //                        14
 XCV,   "BsD", T13,  "L4",  T46,   "Ent", //                        15
 CcP,   "V+",  M79,  "Vo",  M12,   "V-",  // L1 Mute ON Vol=ON          Group 0 Layer B 
 HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2
 XCV,   "V+",  S79,  "Vo",  S12,   "V-",  // L3
 XCV,   "V+",  T79,  "Vo",  T12,   "V-",  // L4
 CcP,   "V+",  M79,  "L1",  M12,   "V-",  // L1 Mute OFF Vol=ON         Group 1 Layer B
 HPU,   "V+",  LcR,  "L2",  EPD,   "V-",
 XCV,   "V+",  S79,  "L3",  S12,   "V-",
 XCV,   "V+",  T79,  "L4",  T12,   "V-",
 CcP,   "BsD", M79,  "Vo",  M12,   "Ent", // L1 Mute ON Vol=OFF         Group 2 Layer B
 HPU,   "BsD", LcR,  "Vo",  EPD,   "Ent",
 XCV,   "BsD", S79,  "Vo",  S12,   "Ent",
 XCV,   "BsD", T79,  "Vo",  T12,   "Ent",
 CcP,   "BsD", M79,  "L1",  M12,   "Ent", // L1 Mute OFF Vol=OFF        Group 3 Layer B 
 HPU,   "BsD", LcR,  "L2",  EPD,   "Ent",
 XCV,   "BsD", S79,  "L3",  S12,   "Ent",
 XCV,   "BsD", T79,  "L4",  T12,   "Ent"};
//012     4    567     8    91011    12
char keyLabel[12][4] = {};     
/////////////////////////////////////////////////////////////////////////////////////////////////
//  If Cfg Key pressed new options (new Labels and new Colours)
//  Labels[0/1][1,5,9,13][0-11][0-3]
//  "Hme", "Up",  "PgU", "Del/V+", "<",   "Cfg", ">",  "V0/L14",  "End", "Dwn", "PgD", "V-/Ret",
//  "L14", "  ",  "D-R", "      ", "AB",  "Sav", " ",  "      ",  "Del", "   ", "L12", "      "
/////////////////////////////////////////////////////////////////////////////////////////////////
const static char CfgLabel[12][4] =                                          
{"L14", "BsD",  "D-R", "   ", "A-B",  "Sav", "Med",  "   ",  "Del", "Kbd", "L12", "   "};  // All 3 chars + \0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  If Media Controls Arrow Right Key pressed new options in different colour:
//                 "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn",  "PgD", "V-/Ret"
// MediaLabel[x][] "Prv", "   ", "Nxt", "   ",    "P-P",  "   ",  "Stp", "   ",     "   ", "   ",  "  ",  "   " 
// MediaLabel[x][] "|< ", "   ", " >|", "   ",    ">||",  "   ",  "|||", "   ",     "   ", "   ",  "  ",  "   " 
// keyLabel[x][]     0             2                4               6                                            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// const static char MediaLabel[12][4] = 
// {"Prv", "   ", "Nxt", "   ",    "P-P",  "   ",  "Stp", "   ",     "   ", "   ",  "  ", "   "}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MediaLabel[12][4] = 
{"|< ", "   ", " >|", "   ",    ">||",  "   ",  "|||", "   ",     "   ", "   ",  "  ", "   "}; 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Must add code to send byte array KeyBrdByte with size KeyBrdByteNum = 1-200 if ESC pressed whilst in KeyBrd 3 (0,1,2,3,)
// Must add code to add values for F1 to F12, (1F3=F1,F2,F3) (1F2=F10,F11,F12) SHF = Shift, ShiftLeft, ShiftRight etc
// If sequence starts with modifiers (Shift etc) then up to 5 keys sent together, else sent in sequence
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char KbrdLabel[5][12][4] =                                                    // Xlate only needed for kcode not Keypress
{"abc",  "def",  "ghi",  "ESC", "jkl", "mno", "pqr", "NXT", "stu", "vwx", "yz ", "ADD",    // ASCII->HID Subtract  dec
 "ABC",  "DEF",  "GHI",  "esc", "JKL", "MNO", "PQR", "nxt", "STU", "VWX", "YZ_", "add",    // ASCII->HID Subtract 61 dec
 "012",  "345",  "678",  "ESC", "9+-", "=*/", "!@#", "NXT", "$%^", "&,.", "~` ", "ADD",    // 1-9 subtract 19 dec 0 subtract 9 dec
 "\\|?", "\"<>", "\'()", "ESC", ":[]", "{};", "1F3", "NXT", "4F6", "7F9", "1F2", "ADD",    // F1-F12 special code
 "ALT", "SHF", "CTR", "EXE", "GUI", "TEI", "CRF", "NXT", "LHR", "UED", "PUD", "ADD"};      // All special code
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char KeyBrdSpc[3][12][4] = 
{"ALT",  "SHF",  "CTR",  "   ", "GUI", "Tab", "C/R", "  ", "A-L", "A-U", "PgU", "ADD",
 "A-R",  "S-R",  "C-R",  "   ", "G-R", "Esc", "L/F", "  ", "Hme", "End", "PgD", "ADD",
 "PrS",  "Del",  "BSp",  "   ", "Nul", "Ins", "RET", "  ", "A-R", "A-D", "Bsp", "ADD"};
const static byte KeyBrdNum[3][12] = 
{AltL,   ShfL,   CtrL,    0,    WinL,  Tab,   0x0D,  0, ArrLeft,  ArrUp,  PageUp, 0,
 AltR,   ShfR,   CtrR,    0,    WinR,  Esc,   0x0A,  0, KeyHome,  KeyEnd, PageDwn,0,
 PrtScr, DelK,   BckS,    0,    0x00,  Ins,   0x28,  0, ArrRight, ArrDown,BckS,   0};
const static byte KeyBrdFx[3][5] = // F1 to F12
{0x3A, 0, 0x3D, 0x40, 0x43, 
 0x3B, 0, 0x3E, 0x41, 0x44, 
 0x3C, 0, 0x3F, 0x42, 0x45};
const static char KeyBrdFxChr[3][5][4] = // F1 to F12
{"F01", "   ", "F04", "F07", "F10", 
 "F02", "   ", "F05", "F08", "F11", 
 "F03", "   ", "F06", "F09", "F12"};
 
bool Fx = false;               // F1-F12 current
bool Kbrd = false;             // Layer for KeyBrd
int  KeyBrdX = 0;              // values 0 1 2 3 4 - 4 adds non-printing chars as numbers to KeyBrdByte
byte KeyBrdByte[200]={};       // Hold values to be sent
int  KeyBrd123 = 0;            // a or b or c - values 0,1,2
int  KeyBrdByteNum = 0;        // Size
int  PrevButton = 0;           // 
char KBDisp[200]={};
int  KBDispPos = 0;
bool KBType = false;           // If 1st byte>0x7F use keycode[0-5] else use keyPress
bool KeyRepeat = false;        // If button same a sprevious button pressed
bool KBrdActive = true;        // Let [ADD] know any char key has been pressed at least once before

////////////////////////////////////////////////////////////////////////////////////////////////////
// reset if not [Cfg] and [Home] [UpArrow] [PageUp] [<] [>] [End] [DwnArrow] [PageDown] key pressed
//             StrButton[button]    0  1  2  3  4  5  6  7  8  9  10 11
//                       CfgOK      x  x  x     x  x  x     x  x  x 
static const byte CfgButton[12] = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
////////////////////////////////////////////////////////////////////////////////////////////////////

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
const int ByteSize = 200;

char Str1[StrSize], Str2[StrSize], Str3[StrSize], Str4[StrSize],  Str5[StrSize],  Str6[StrSize];
char Str7[StrSize], Str8[StrSize], Str9[StrSize], Str10[StrSize], Str11[StrSize], Str12[StrSize];
char Ttr1[StrSize], Ttr2[StrSize], Ttr3[StrSize], Ttr4[StrSize],  Ttr5[StrSize],  Ttr6[StrSize];
char Ttr7[StrSize], Ttr8[StrSize], Ttr9[StrSize], Ttr10[StrSize], Ttr11[StrSize], Ttr12[StrSize];
byte Mtr1[ByteSize], Mtr2[ByteSize], Mtr3[ByteSize], Mtr4[ByteSize],  Mtr5[ByteSize],  Mtr6[ByteSize];
byte Mtr7[ByteSize], Mtr8[ByteSize], Mtr9[ByteSize], Mtr10[ByteSize], Mtr11[ByteSize], Mtr12[ByteSize];
char LayerStr[2][4][4] = {"L1A","L2 ","L3A","L4A","L1B","L2 ","L3B","L4B"};   //Start with 7 followed by the startup layer 1 to 4 L1-L4

char * Str1to12[] = {Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,Str11,Str12} ;
char * Ttr1to12[] = {Ttr1,Ttr2,Ttr3,Ttr4,Ttr5,Ttr6,Ttr7,Ttr8,Ttr9,Ttr10,Ttr11,Ttr12} ;
byte * Mtr1to12[] = {Mtr1,Mtr2,Mtr3,Mtr4,Mtr5,Mtr6,Mtr7,Mtr8,Mtr9,Mtr10,Mtr11,Mtr12} ;
 
char inputString[StrSize] = "Send new text through serial port start with 0 end with newline\n";  // String to hold incoming data for [M4] Key
bool StrOK = false;          // String is complete - ended with 0x0A
bool ByteOK = false;         // Bytes received is complete - ended with 0x00
int RecBytesLen = 0;          // Number of chars in RecBytes
int RecBytesType = 1;         // Type 1, 2, 3 = sequence, simulataneous, media

const byte MaxBytes = StrSize;
byte RecBytes[MaxBytes];     // Raw bytes received must start < and end with > - can have length byte/int after < to include <> in data
byte NumBytes = 0;
bool NewData = false;

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
      
  if (LittleFS.exists("MuteDisable"))      MuteDisable = true; else MuteDisable = false;   // VolMute Key disabled L1-L4 enabled
  if (LittleFS.exists("VolDisable"))       VolDisable  = true;  else VolDisable = false;   // V+ V- disabled Delete-Return enabled
  if (LittleFS.exists("Layout"))           SaveLayout  = true;  else SaveLayout = false;   // Selected Saved Layout restored
  if (LittleFS.exists("LayerB"))           LayerAB     = 1;     else LayerAB     = 0;      // Layer A 1-6 or B 7-12 restored
  if (LittleFS.exists("MediaKeys"))        Media       = true;  else Media       = false;  // Media Keys in Layout 2 restored
  if (LittleFS.exists("DeleteBackspace"))  DelBS       = true;  else DelBS       = false;  // Toggle Delete or Backspace key

  InitStr();  // ByteOK = StrOK = false - Copy str# to Str# and Ttr# if no saved file else read Str# Ttr# from files, if Mtr files exist copy those to M1 to M12
   
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB HID Composite");
  usb_hid.setReportCallback(NULL, hid_report_callback); // Activate report CapsScrollNum Lock indicators
  usb_hid.begin();
  while( !TinyUSBDevice.mounted() ) delay(1);

  // Initialise the TFT screen TFT_eSPI/TFT_eSPI.h
  tft.init();
  tft.setRotation(3);                     // Set the display orientation to 0, 1, 2 or 3
  tft.fillScreen(Black); 
  tft.setFreeFont(&FreeSans12pt7b);       // use KEY_TEXTSIZE 1 
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
  NewData = false;
  KeyBrdByteNum = KBDispPos = KeyBrdX = KeyBrd123 = 0;
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
      if (!Kbrd) status("");                  // Clear the status line
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

  RecSerial();
  if (NewData) showRecData();
  if (NewData) DoNewData();   // First char 0,1,2,3,4,5,6 then store file after that char 7 to 9 non-ASCII data
  
} // main Loop

/////////////////////////////////////////////
// Check for new character strings or macros
/////////////////////////////////////////////
void DoNewData()
{ int n = 0;
  char *ChrPtr;
  byte *BytePtr;
  bool Found = false;
  byte a, c = 0;
  String NameStr;

  Found = NewData = StrOK = ByteOK = false;
  
  a = RecBytes[0] - 48;  // ASCII Number 0-9 subtract 48 
  Found = (a<7);
    
  if (Found){ if ((LayerAB==1)&&(a>0)) c = a + 5; else c = a - 1;    // S1-S6=>S7-S12 T1-T6=>T7=T12 M1-M6=>M7=M12

              if (Layout==3) {if (a>0) {StrOK = true;  NameStr = STRname[c]; ChrPtr = Str1to12[c]; }   // Keys S1-S12
                                                 else {NameStr = "inputStr"; ChrPtr = inputString; } } // Key [M4] normal text
              if (Layout==4) {if (a>0) {StrOK = true;  NameStr = TTRname[c]; ChrPtr = Ttr1to12[c]; }   // Keys T1-T12
                                  else {ByteOK = true; return; }   }                                   // Key [M4] non-ASCII in RecBytes[] 
              if (Layout==1) {if (a>0) {Serial.println("Layout1");
                                        ByteOK = true; NameStr = MTRname[c]; BytePtr = Mtr1to12[c]; }  // Keys M1-M12
                                  else {ByteOK = true; return; }   }                                   // Key [M4] non-ASCII in RecBytes[] 
              
              for (n=1; n<=NumBytes; n++) ChrPtr[n-1] = RecBytes[n];     
              for (n=1; n<=NumBytes; n++) BytePtr[n-1] = RecBytes[n]; }   // End if (Found)
              
   if ((a>6)&&(a<10)) {Found = true; n--; RecBytesType = a - 6; ByteOK = true;}              // Non ASCII start string with char 7 - 9
   
   if ((Found)&&(StrOK)) {DoFileStrings(StrOK, NameStr, ChrPtr);   // Save the text/num strings 
                                  status(ChrPtr); }                // Display the new string       
     
   if ((Found)&&(ByteOK)) {Serial.println(NameStr); Serial.println(BytePtr[0]); DoFileBytes(ByteOK, NameStr, BytePtr); } // Save the numbers 
                                                 
}


////////////////////////////////////////////////////////////////////////////////////
void RecSerial() // https://forum.arduino.cc/t/serial-input-basics-updated/382007/3
////////////////////////////////////////////////////////////////////////////////////
{   static boolean InProgress = false;
    static byte n = 0;
    byte StartMarker = 0x3C; 
    byte EndMarker = 0x3E;
    byte b;

    while ((Serial.available()>0) && (!NewData)) 
          { b = Serial.read();
            if (InProgress) { if (b!=EndMarker) { RecBytes[n] = b;
                                                   n++;
                                                   if (n >= MaxBytes) { n = MaxBytes - 1; } }
                              else { RecBytes[n] = '\0'; // terminate the string
                                     InProgress = false;
                                     NumBytes = n;   
                                     n = 0;
                                     NewData = true;
                                   }
                             }
            else if (b == StartMarker) { InProgress = true; }
          }
}
////////////////////////
void showRecData() 
////////////////////////
{   char RecStr[MaxBytes];
    Serial.print(NumBytes);
    Serial.print(" number HEX values ");
    for (byte n = 0; n <= NumBytes; n++) 
        {RecStr[n] = RecBytes[n];
         Serial.print(RecBytes[n], HEX);
         Serial.print(' '); }
    Serial.println();
    Serial.println(RecStr);
    //NewData = false;
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
  uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here
  uint8_t i, n, a, b, c;          // 
  uint8_t CfgOK ;                 // Do Cfg actions
  
  if ( !usb_hid.ready() ) return; 

  KeyRepeat = false;
  if (PrevButton!=button) KeyBrd123 = 0; else KeyRepeat = true; // Start at a not b or c if other button previously used
  PrevButton = button;
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
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // reset if not [Cfg] and [Home] [UpArrow] [PageUp] [<] [Cfg] [>] [End] [DwnArrow] [PageDown] key pressed
  // StrButton[button]      0  1  2  3  4  5  6  7  8  9  10 11
  //             CfgOK      x  x  x     x  x  x     x  x  x 
  // byte CfgButton[12] = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
  CfgOK = CfgButton[button];
  if ((Layout==2) && (CfgOK==0) && (ConfigKeyCount>0)) {ConfigKeyCount = 0; status(""); } 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////      
                                                        
  if ((Layout!=2)) {   // Layout 1 or 3 or 4   
  switch(button){
    case 0: ///////////////////////////////////////////////// Cut = control + x                             
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_X; // actually x not X unless Capslock ON
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 1: ///////////////////////////////////////////////// Copy = control + c
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_C;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 2: ////////////////////////////////////////////////// Paste = control + v
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_V;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);   break; 
      
    case 3: /////////////////////////////////////////////////// Volume Increase
      if (VolDisable) {if (DelBS) keycode[0] = BckS; else keycode[0] = DelK;  //Delete or BackSpace Key
                       usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(RID_KEYBOARD); break; }
    
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolUp);  delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);      break;
      
    case 4: // M1 S1 T1 ///////////////////////////////////////////////// Alt + PrintScreen - some mini keyboards cannot do this
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);               delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);               delay(keydelay2); } break; }
      /*
      if (Layout==1) { const byte arraySize = sizeof(Mtr1to12[c]) / sizeof(Mtr1to12[c][0]);
                       for (n=0;  n<arraySize; n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Mtr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);               delay(keydelay2); } break; }  
      */                      
      keycode[0] = AltL; 
      keycode[1] = PrtScr;
      keycode[2] = 0x00; // Need this why? 
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay);
      usb_hid.keyboardRelease(RID_KEYBOARD);            delay(keydelay2);
      status("PrtScr Current Window");  break;
      
    case 5: // M2 S2 T2 ///////////////////////////////////////////////// Open admin powershell including UAC completion
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);               delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);               delay(keydelay2); } break; }
                                                                                
      keycode[0] = HID_KEY_GUI_LEFT; // or use HID_KEY_GUI_LEFT 
      keycode[1] = HID_KEY_X;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);   delay(keydelay2); // GUI + X then A admin powershell i normal powershell g Computer Management
      usb_hid.keyboardRelease(RID_KEYBOARD);              delay(keydelay2); // Confirm with ArrowLeft Enter 
      usb_hid.keyboardPress(RID_KEYBOARD, 'A');           delay(keydelay2); // Can also use Alt + "Y" too confirm
      usb_hid.keyboardRelease(RID_KEYBOARD);              delay(keydelay3); // Can also open via Run GUI + R then Ctrl+Shft+Enter then Alt+Y
      keycode[0] = ArrLeft;
      keycode[1] = KeyEnter;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);   delay(keydelay2); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 6: // M3 S3 T3 /////////////////////////////////////////////// Open Run window with last coommand visible and selected
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);               delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);               delay(keydelay2); } break; }  
      if (Layout==1) { const byte arraySize = sizeof(Mtr1to12[c]) / sizeof(Mtr1to12[c][0]);
                       for (n=0;  n<arraySize; n++) 
                           {usb_hid.keyboardPress(RID_KEYBOARD, Mtr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(RID_KEYBOARD);               delay(keydelay2); } break; }                  
                          
      keycode[0] = HID_KEY_GUI_RIGHT; // or use HID_KEY_GUI_LEFT do not use KEYBOARD_MODIFIER_LEFTGUI
      keycode[1] = HID_KEY_X;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay);
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 7: //////////////////////////////////////////////// Volume Mute and change Layouts
      if ((MuteDisable) && (VolMuteCount==0)) VolMuteCount++;
      VolMuteCount++;
      if (VolMuteCount==2) {Layout=2; ConfigButtons(1); delay(keydelay); break; }                      // Count = 2 Navigate
      if (VolMuteCount==3) {Layout=3; ConfigButtons(1); delay(keydelay); break; }                      // Count = 3 Text Strings
      if (VolMuteCount==4) {Layout=4; ConfigButtons(1); delay(keydelay); break; }                      // Count = 4 Text Strings
      if (VolMuteCount==5) {Layout=1; VolMuteCount = 0; ConfigButtons(1); delay(keydelay); break; }    // Count = 0 start again
      
      if (MuteDisable) break;  // No VolumeMute action [Cfg][Hme] sequences

      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolMute); delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);       break;
      
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
      if (ByteOK)       { Serial.print("RecBytes[1] "); Serial.println(RecBytes[1]); 
                          // <0777> shows 4 number HEX values 30 37 37 37 0     0777    RecBytes[1] 55 which is correct
                          // Must do this while pink layer is active
                          keycode[0] = RecBytes[1];     // 0xE0 224 RecBytes[0] = 7,8,9 ASCII
                          keycode[1] = RecBytes[2];     // 0xE1 225
                          keycode[2] = RecBytes[3];     // 0x29 41
                          keycode[3] = KeyEnter;
                          usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);      delay(keydelay2);
                          usb_hid.keyboardRelease(RID_KEYBOARD);                 delay(keydelay2); break;} 
          /*
          ByteOK = false;  //Serial.println(RecBytesLen); Serial.println(RecBytes[1]);
                                           //Serial.println(RecBytes[2]); Serial.println(RecBytes[3]); Serial.println(RecBytes[4]);
          for (n=1;  n<RecBytesLen; n++) { if (RecBytes[n]==224) {keycode[0] = CtrL;}     // 0xE0
                                      if (RecBytes[n]==225) {keycode[1] = ShfL;}     // 0xE1
                                      if (RecBytes[n]==41)  {keycode[2] = Esc; }  }  // 0x29
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
      //usb_hid.keyboardRelease(RID_KEYBOARD);     break;
      
    case 11: ////////////////////////////////////////// Volume decrease
      if (VolDisable) { usb_hid.keyboardPress(RID_KEYBOARD, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(RID_KEYBOARD); break; }
    
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolDown); delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);       break;
  }   // switch
  }   // if Layout=1 or Layout=2

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // #define ArrUp    0x52  #define ArrDown  0x51  #define ArrLeft  0x50  #define ArrRight 0x4F
  // #define KeyHome  0x4A  #define KeyEnd   0x4D  #define PageUp   0x4B  #define PageDwn  0x4E
  ///////////////////////////////////////////////////////////////////////////////////////////////
  else //if (Layout==2)    
  switch(button){
    case 0: // HOME - Media Previous //////////////////////////////////////////////////////////////////
      if (Kbrd) {KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123]; 
                 MakeStr(button); if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; break;}
     
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              MuteDisable = !MuteDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (MuteDisable) status("Volume Mute OFF"); else status("Volume Mute ON");
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(RID_CONSUMER_CONTROL, PrevMed); delay(keydelay);
                  usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0); break; }

      keycode[0] = KeyHome;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 1: // ARROW UP ////////////////////////////////////////////////
      if (Kbrd) {KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123]; 
                 MakeStr(button); if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              DelBS = !DelBS; 
                              //GetSysInfo(1); // Save state on Flash
                              if (DelBS) status("Backspace ON"); else status("Delete ON");
                              ConfigButtons(1); break; }
                              
      keycode[0] = ArrUp;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 2: // PAGE UP /////////////////////////////////////////////////
      if (Kbrd) {KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123]; 
                 MakeStr(button); if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              VolDisable = !VolDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (VolDisable) status("Volume Keys OFF"); else status("Volume Keys ON");
                              ConfigButtons(1); break; }
                              
      if (Media) {usb_hid.sendReport16(RID_CONSUMER_CONTROL, NxtMed); delay(keydelay);
                  usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0); break; }

      keycode[0] = PageUp;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 3: // V+ or Delete or Escape(Keybrd) //////////////////////////////////////
      if (Kbrd) {Kbrd = false;  status((char *)KBDisp);
                 if ((KeyBrdX==4) && (KeyBrdByteNum>0)) {SendBytes();} 
                 if (KeyBrdX<4) {KeyBrdByteNum = 0; KBDispPos = 0;} 
                 KeyBrdX = 0; KeyBrd123 = 0; ConfigButtons(1); break;}
       
      if (VolDisable) {if (DelBS) keycode[0] = BckS; else keycode[0] = DelK;  //Delete or BackSpace Key
                       usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(RID_KEYBOARD); break; }
        
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolUp); delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);     break;
      
    case 4: // ARROW LEFT //////////////////////////////////////////////
      if (Kbrd) {KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123]; 
                 MakeStr(button); if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              if (LayerAB==0) {LayerAB=1; status("Layer B");} else {LayerAB=0; status("Layer A");} 
                              //GetSysInfo(1); // Save state on Flash
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(RID_CONSUMER_CONTROL, PlayMed); delay(keydelay);
                  usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0); break; }
    
      keycode[0] = ArrLeft;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 5: // CONFIG //////////////////////////////////////////////////
      if (Kbrd) {KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123]; 
                 MakeStr(button); if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; break;}
    
      GetSysInfo(0); 
      if (ConfigKeyCount==1) {status("Config and Files Saved");           ConfigKeyCount = 0; }
                        else {status("L14 BsD Ret AB Save Med Del L12"); ConfigKeyCount++ ; } 
      ConfigButtons(1);       break;
      
    case 6: // ARROW RIGHT /////////////////////////////////////////////
      if (Kbrd) {KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123]; 
                 MakeStr(button); if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              Media = !Media; if (Media) status("Media Keys ON"); else status("Media Keys OFF"); 
                              //GetSysInfo(1); // Save state on Flash
                              ConfigButtons(1); break; }
      
      if (Media) {usb_hid.sendReport16(RID_CONSUMER_CONTROL, StopMed); delay(keydelay);
                  usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0); break; }

      keycode[0] = ArrRight;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 7: // Vol Mute ///////////////////////////////////////////////////////////////
      if (Kbrd) {if (KeyBrdX<4) KeyBrdX++; else KeyBrdX=0; KeyBrd123=0; ConfigButtons(1); break;} // Changes 5 KeyBrds from abc - ABC - 012 - *^% - SHFT
    
      if ((MuteDisable) && (VolMuteCount==0)) VolMuteCount++;
      VolMuteCount++;
      if (VolMuteCount==2) {Layout=2; ConfigButtons(1); delay(keydelay); break; }                      // Count = 2 Navigate
      if (VolMuteCount==3) {Layout=3; ConfigButtons(1); delay(keydelay); break; }                      // Count = 3 Text Strings
      if (VolMuteCount==4) {Layout=4; ConfigButtons(1); delay(keydelay); break; }                      // Count = 4 Text Strings
      if (VolMuteCount==5) {Layout=1; VolMuteCount = 0; ConfigButtons(1); delay(keydelay); break; }    // Count = 0 start again
      
      if (MuteDisable) break;  // No VolumeMute action [Cfg][Hme] sequences

      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolMute); delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);       break;
      
    case 8: // END //////////////////////////////////////////////////////////////////////////////
      if (Kbrd) {KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123]; 
                 MakeStr(button); if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; break;}
    
      if (ConfigKeyCount==1) {status("Files deleted"); ConfigKeyCount-- ; DeleteFiles(); ConfigButtons(1); break; }
      
      keycode[0] = KeyEnd;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 9: // ARROW DOWN //////////////////////////////////////////////
      if (Kbrd) {KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123]; 
                 MakeStr(button); if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--; Kbrd = true; status("Escape Next Add End"); ConfigButtons(1); break;} 
           
      keycode[0] = ArrDown;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 10: // PAGE DOWN //////////////////////////////////////////////
      if (Kbrd) {KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123]; 
                 MakeStr(button); if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; break;}
      
      if (ConfigKeyCount==1) {ConfigKeyCount--; SaveLayout = !SaveLayout; GetSysInfo(1); // Save the state
                              if (SaveLayout) status("Startup Layout L2"); else status("Startup Layout L1"); ConfigButtons(1); break;}

      keycode[0] = PageDwn;
      usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(RID_KEYBOARD);  break;
      
    case 11: // V- or Enter or Add /////////////////////////////////////////////////////////////////////////////////////////////////////
      if (Kbrd) {if (!KBrdActive) {status("Add char/modifier first"); Serial.println("Add char/modifier first"); break;} // No char key or modifier keys has been pressed at least once
                 if (KeyRepeat) {if (KeyBrdX==4) break;
                                 if (KeyBrdX<4)  KeyBrdByte[KeyBrdByteNum] = KeyBrdByte[KeyBrdByteNum-1]; 
                                 MakeStr(PrevButton);}
                 if (KeyBrdX==4) {KBDispPos = KBDispPos + 2;  if (KeyBrdByte[0]>0x7F) KBType = true; } 
                 if (Fx) {KBDispPos = KBDispPos + 2; Fx = false; }
                 KBDispPos++; KeyBrdByteNum++; break; }  
    
      if (VolDisable) { usb_hid.keyboardPress(RID_KEYBOARD, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(RID_KEYBOARD); break; }

      usb_hid.sendReport16(RID_CONSUMER_CONTROL, VolDown); delay(keydelay);
      usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);       break;
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
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  If Layout 2 [Cfg] Key pressed new options in different colour:
  //   "Hme", "Up",   "PgU", "Del/V+",  "<",    "Cfg", ">",    "V0/L14",  "End", "Dwn", "PgD", "V-/Ret"
  //   "L14", "BsD",  "D-R", "   ",     "A-B",  "Sav", "Med",  "   ",     "Del", "Kbd",  "L12", "   " 
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (ConfigKeyCount>0) 
    {for (m=0; m<3; m++) {keyLabel[0][m] = CfgLabel[0][m]; keyLabel[1][m] = CfgLabel[1][m]; keyLabel[2][m] =  CfgLabel[2][m];  
                           keyLabel[4][m] = CfgLabel[4][m]; keyLabel[5][m] = CfgLabel[5][m]; keyLabel[6][m] =  CfgLabel[6][m]; 
                           keyLabel[8][m] = CfgLabel[8][m]; keyLabel[9][m] = CfgLabel[9][m]; keyLabel[10][m] = CfgLabel[10][m]; };
      keyColor[0]=keyColor[1]=keyColor[2]=keyColor[4]=keyColor[5]=keyColor[6]=keyColor[8]=keyColor[9]=keyColor[10]=CfgColour; }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  If Media Controls Arrow Right Key pressed new options in different colour:
  //                 "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn", "PgD", "V-/Ret"
  // MediaLabel[x][] "Prv", "  ", "Nxt", "   ",    "P-P",  "   ",  "Stp", "   ",     "   ", "   ",  "  ", "   " 
  // keyLabel[x][]    0             2                4               6                                            
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((Media)&&(b==1)&&(ConfigKeyCount==0)) 
     {for (m=0; m<3; m++) {keyLabel[0][m] = MediaLabel[0][m]; keyLabel[2][m] = MediaLabel[2][m]; 
                           keyLabel[4][m] = MediaLabel[4][m]; keyLabel[6][m] = MediaLabel[6][m]; };
      keyColor[0] = keyColor[2] = keyColor[4] = keyColor[6] = MediaColour; }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Kbrd) { for (m=0; m<3; m++) 
                 {keyLabel[0][m] =  KbrdLabel[KeyBrdX][0][m];   keyLabel[1][m] =  KbrdLabel[KeyBrdX][1][m]; 
                  keyLabel[2][m] =  KbrdLabel[KeyBrdX][2][m];   keyLabel[3][m] =  KbrdLabel[KeyBrdX][3][m]; 
                  keyLabel[4][m] =  KbrdLabel[KeyBrdX][4][m];   keyLabel[5][m] =  KbrdLabel[KeyBrdX][5][m]; 
                  keyLabel[6][m] =  KbrdLabel[KeyBrdX][6][m];   keyLabel[7][m] =  KbrdLabel[KeyBrdX][7][m];
                  keyLabel[8][m] =  KbrdLabel[KeyBrdX][8][m];   keyLabel[9][m] =  KbrdLabel[KeyBrdX][9][m]; 
                  keyLabel[10][m] = KbrdLabel[KeyBrdX][10][m];  keyLabel[11][m] = KbrdLabel[KeyBrdX][11][m];}
                  keyColor[0]=keyColor[1]=keyColor[2]=keyColor[3]=keyColor[4]=keyColor[5]=CfgColour;
                  keyColor[6]=keyColor[7]=keyColor[8]=keyColor[9]=keyColor[10]=keyColor[11]=CfgColour;}
            
  b = Layout - 1;                               // reset b to 0 - 3
              
  for (uint8_t row = 0 + rowcount; row < rows; row++) {   // 3 rows 0,1,2 => b = 0+(0*4)=0 1+(0*4)=1 ... 0+(1*4)=4 ....
    for (uint8_t col = 0; col < 4; col++) {               // 4 columns 0,1,2,3
        b = col + row * 4;
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X), KEY_Y + row * (KEY_H + KEY_SPACING_Y), 
                                               KEY_W, KEY_H, OutlineColor, keyColor[b], White, keyLabel[b], KEY_TEXTSIZE);
        key[b].drawButton();  }  }

  if (Change) indicators();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Output report callback for LED indicator such as Caplocks
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) report_id;
  (void) bufsize;
  bool CapsLockPrev = CapsLock;            
  bool NumLockPrev = NumLock;             
  bool ScrollLockPrev = ScrollLock; 
  
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

  if (Change) indicators();
   
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
//  Only used in setup ByteOk = StrOK = false; => Read file
//////////////////////////////////////////////////////////////////
void InitStr()
{ char *ChrPtr;
  String NameStr;
  byte *BPtr;
  int n = 0;
  
  for (n=0; n<12; n++) 
      {NameStr = STRname[n]; ChrPtr = Str1to12[n];  // n=0 STRname[0] Str1to12[0] str1to12[0]
       if (!LittleFS.exists(NameStr)) strcpy(ChrPtr, str1to12[n]); else DoFileStrings(StrOK, NameStr, ChrPtr);  }
  
  for (n=0; n<12; n++) 
      {NameStr = TTRname[n]; ChrPtr = Ttr1to12[n];
       if (!LittleFS.exists(NameStr)) strcpy(ChrPtr, str1to12[n]); else DoFileStrings(StrOK, NameStr, ChrPtr); }

  for (n=0; n<12; n++) 
      {NameStr = MTRname[n]; BPtr = Mtr1to12[n];
       if (LittleFS.exists(NameStr)) DoFileBytes(ByteOK, NameStr, BPtr); }

  if (LittleFS.exists("inputStr")) {ChrPtr = inputString; DoFileStrings(StrOK, "inputStr", ChrPtr);}
}


//////////////////////////////////////////////////////////////////
void DoFileBytes(bool DoWrite, String STRf,  byte *BytePtr)
{ uint16_t ByteLen;
  const uint16_t ByteArrayLen = sizeof(BytePtr) / sizeof(BytePtr[0]);
  int n;
  byte b;
      
  if (!DoWrite)                            // read data
     {File f = LittleFS.open(STRf, "r");
      n = 0;
      while (f.available())
            {b = f.read();
             BytePtr[n++] = b;
      if (n >= ByteArrayLen) break; }
      ByteLen = f.size();
      f.close(); }
     
  if (DoWrite)                             // write data
     {File f = LittleFS.open(STRf, "w");
      f.write(&BytePtr[0], ByteArrayLen);  // write the whole array to the file
      f.print('\0');                       // f.println();
      f.close();  }
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
       
  if (DoWrite)  // write data
     {File f = LittleFS.open(STRf, "w");
      StrLen = strlen(ChrPtr);
      f.print(ChrPtr);                       // if byte[] not char[]?
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
      Serial.print(file.name());
      Serial.print(" ");
      Serial.println(file.size());
      n++;
      file = root.openNextFile();
  }
}

void DeleteFiles()  // delete text/number macros and config
{ if (LittleFS.format()); 
     {LittleFS.begin();
      LittleFS.remove("/MuteDisable");
      LittleFS.remove("/VolDisable");
      Serial.println("All files deleted");}
}
////////////////////////////////////////////////
void GetSysInfo(int Action) // Also save config
//////////////////////////////////////////////// 
{  char *ChrPtr;
  
  int fHeap = rp2040.getFreeHeap();
  int uHeap = rp2040.getUsedHeap();
  int tHeap = rp2040.getTotalHeap();
  int fCPU  = rp2040.f_cpu();
  
  if (Action==1) {if (SaveLayout) { char LayoutF[2]  = "1"; ChrPtr = LayoutF; DoFileStrings(1, "Layout",  ChrPtr); }
                  else LittleFS.remove("Layout"); return;}
           
  if (VolDisable)  { char VolD[2]   = "1"; ChrPtr = VolD;  DoFileStrings(true, "VolDisable",   ChrPtr); }
          else LittleFS.remove("VolDisable");
      
  if (MuteDisable) { char MuteD[2]  = "1"; ChrPtr = MuteD; DoFileStrings(true, "MuteDisable",  ChrPtr); }
         else LittleFS.remove("MuteDisable"); 

  if (LayerAB==1) { char LayerD[2]  = "1"; ChrPtr = LayerD; DoFileStrings(true, "LayerB",  ChrPtr); }
         else LittleFS.remove("LayerB"); 

  if (Media) { char MediaD[2]  = "1"; ChrPtr = MediaD; DoFileStrings(true, "MediaKeys",  ChrPtr); }
         else LittleFS.remove("MediaKeys");

  if (DelBS) { char DelBSD[2]  = "1"; ChrPtr = DelBSD; DoFileStrings(true, "DeleteBackspace",  ChrPtr); }
         else LittleFS.remove("DeleteBackspace");
    
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
  tft.setTextColor(SkyBlue, Black);
  if (Kbrd) tft.setTextColor(Yellow, Black);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(1);
  //tft.setTextSize(1);
  tft.drawString(msg, STATUS_X, STATUS_Y);
  tft.setFreeFont(&FreeSans12pt7b);         // KEY_TEXTSIZE 1 
}

////////////////////////
// SendBytes()
///////////////////////
void SendBytes()
{ uint8_t keycode[6] = { 0 }; // simultaneous keys pressed in here
  uint8_t a, b, n;  
    
  if (KeyBrdByte[0]<0x80)                                                           // All sent in sequence via keyPress 
     {for (n = 0; n < KeyBrdByteNum; n++) if (KeyBrdByte[n]<0x80)                   // When 1st char is normal alphanumeric all must be <0x80 
          {usb_hid.keyboardPress(RID_KEYBOARD, KeyBrdByte[n]); delay(keydelay);     // Converted via end of hid.h from ascii to hid
           usb_hid.keyboardRelease(RID_KEYBOARD);              delay(keydelay); }
      File f = LittleFS.open("KeyBrdMacro1", "w");
      if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }
      status("Sequence Macro sent"); }
          
  if ((KeyBrdByte[0]>0x7F) && (KeyBrdByteNum<6))
     {for (n = 0; n < KeyBrdByteNum; n++) {keycode[n] = KeyBrdByte[n]; }  
          usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay);
          usb_hid.keyboardRelease(RID_KEYBOARD);            delay(keydelay);  
      File f = LittleFS.open("KeyBrdMacro2", "w");
      if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }  
      status("Parallel Macro sent"); aaDDD}
 
      for (n = 0; n < KeyBrdByteNum; n++) 
          {Serial.print(KeyBrdByte[n], HEX);
           Serial.print(' '); }
      Serial.println(" ");
      for (n = 0; n < 6; n++)   
          {Serial.print(keycode[n], HEX);
           Serial.print(' '); }
       
  KeyBrdByteNum = 0; KBDispPos = 0;  KeyBrdX = 0; KeyBrd123 = 0;
  for (n = 0; n < 200; n++) { KeyBrdByte[n] = 0x00; KBDisp[n] = '\0'; }
  Kbrd = false; KBType = false; KBrdActive = false;
}

////////////////////////
void MakeStr(int button)
////////////////////////
{   int i, n;
    byte a, b, c;
    a = 0;
    b = KeyBrdByte[KeyBrdByteNum];

    KBrdActive = true; // Let ADD know any char key has been pressed at least once before
    Serial.print("Prev "); Serial.print(PrevButton);
    Serial.print(" Curr ");Serial.println(button);

    
    if (KBType) {
       if (KeyBrdX==0) if ((b>0x60) && (b<0x7B)) {a = b - 93;}  // Xlate a-x
       if (KeyBrdX==1) if ((b>0x40) && (b<0x5B)) {a = b - 61;}  // Xlate A-X but leave DelK = 0x4C
       if (KeyBrdX==2) if ((b>0x2F) && (b<0x3A)) {if (b==0x30) a = b - 9; else a = b - 19; } }       // Xlate 0-9 to 1-0
       
       if ((KeyBrdX==3) && ((button==6)||(button==8)||(button==9)||(button==10))) {a = KeyBrdFx[KeyBrd123][button-6]; Fx = true;}   // Xlate F1 - F12

    for (int n = 0; n <= KeyBrdByteNum; n++) 
        {Serial.print(KeyBrdByte[n], HEX);
         Serial.print(' '); }
         
    Serial.println();
           
    if (KeyBrdX==4) {KeyBrdByte[KeyBrdByteNum] = KeyBrdNum[KeyBrd123][button]; //}
        for (n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } Serial.println(); }
       
    n = 0;
    if (KeyBrdX<4) 
       {KBDisp[KBDispPos] = b; KBDisp[KBDispPos+1] = ' '; KBDisp[KBDispPos+2] = ' '; 
        if (a>0) KeyBrdByte[KeyBrdByteNum] = a; 
        if (Fx) { for (n=0; n<3; n++) {KBDisp[KBDispPos+n] = KeyBrdFxChr[KeyBrd123][button-6][n]; } } }
        
    if (KeyBrdX==4) for (n=0; n<3; n++) KBDisp[KBDispPos+n] = KeyBrdSpc[KeyBrd123][button][n];
     
    for (i = 0; i <= KBDispPos+n; i++) 
        {Serial.print(KBDisp[i]);
         Serial.print(' '); }
    Serial.println();
     

    status((char *)KBDisp);
}

////////////////////////////////////////////////////////////////////////////
// #define IND_X 70  // Centred on this - Sppace in RH side about 16 pixels
// #define IND_Y 139 // 
// IndXPos[7] = {0, 0, 78, 156, 0, 78, 156};
////////////////////////////////////////////////////////////////////////////
void indicators() {
  tft.setTextPadding(14);
  tft.setTextColor(Green, Black);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(1);
  if (CapsLock)   tft.drawString("C", IND_X, IND_Y);     else tft.drawString(" ", IND_X, IND_Y);
  if (NumLock)    tft.drawString("N", IND_X+78, IND_Y);  else tft.drawString(" ", IND_X+78, IND_Y);
  if (ScrollLock) tft.drawString("S", IND_X+156, IND_Y); else tft.drawString(" ", IND_X+156, IND_Y);
  tft.setFreeFont(&FreeSans12pt7b);     
}  
