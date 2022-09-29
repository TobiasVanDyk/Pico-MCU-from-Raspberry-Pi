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
Layout 1  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
------------------------------------------------------------------------------------------------
[Cut Ctrl+X] [  Copy Ctrl+C   ] [Paste Ctrl+V] [VolUp  Del-Bs]    [cX]  [cC]  [cV]  [V+][Del-Bs]
[Alt+PrtScr] [Admin Powershell] [ Run window ] [VolMute L1-L4]    [M1]  [M2]  [M3]  [Vo][L1-L4 ]
[Send Text ] [Admin CMD Prompt] [Task Manager] [VolDwn  Enter]    [M4]  [M5]  [M6]  [V-][Enter ]
          Caps                Num           Scroll                    C     N     S

Layout 2  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
--------------------------------------------------------------------------------------------------
[Home   Prev  Mute] [UpArr  BsDel] [PgeUp Nxt DelRet] [VolUp  Delete] [H] [Up ] [Pu] [V+][Del-Bs]
[ArrL PlayPse L-AB] [Config  Save] [ArrR  Stop Media] [VolMute L1-L4] [<] [Cfg] [> ] [Vo][L1-L4 ]
[End    CfgFileDel] [DwnArr KeyBr] [PgeDwn StartL1L2] [VolMute Enter] [E] [Dw ] [Pd] [V-][Enter ]
                 Caps            Num               Scroll                C     N    S

Layout 2 (Config Layout) has five additional small buttons on the far right side:
Red     Press the small Red Pad on the Right to toggle the Math KeyPad on/off.
Blue    Press the small Blue Pad on the Right to toggle the Keyboard on/off.
Green   Press the small Green Pad on the Right to toggle the Media Keypad on/off.
SkyBlue Press the small SktBlue Pad on the Right to toggle the Number Keypad on/off.
Green   Not assigned.

Layout 3+4 Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
--------------------------------------------------------------------------------------------------
[Cut Ctrl+X ] [Copy Ctrl+C] [Paste Ctrl+V] [VolUp  Del-Bs]   [ cX ]  [ cC  ]  [ cV  ] [V+][Del-Bs]
[Send Text 1] [Send Text 2] [Send Text 3 ] [VolMute L1-L4]   [S1 T1] [S2 T2]  [S3 T3] [Vo][L1-L4 ]
[Send Text 4] [Send Text 5] [Send Text 6 ] [VolMute Enter]   [S4 T4] [S5 T5]  [S6 T6] [V-][Enter ]
           Caps           Num           Scroll                      C       N        S

-------------------------------------------------------------------------------------------------
Layouts 1, 3 and 4, two Layers A/B each with A=M1-M6, S1-S6, T1-T6 and B=M7-M12, S7-S12, T7-T12
Press VolumeMute consecutively within 5 seconds else count is reset to 0 or press [L1-L4] once.
[L1-L4] has a repeat period of 0.5 seconds - after this key repeat is active

Press Config Key (new active keys change colour) then:
L14 (Home) - VolumeMute -> L1, L2, L3, L4 - repeat to restore Vo
BsD (ArrowUp) - Toggle Vol+ key to either Delete or Backspace
Vol (PageUp) - Vol+ -> Delete and Vol- -> Enter - repeat to restore V+ V-
A-B (ArrowLeft) - Layouts 1, 3, 4, change to Layer A or Layer B 
Med (ArrowRight) - Change Layout 2 to Media Controls Previous-Next-PlayPause-Stop
Del (End) - Delete all files on Flash (Strings and Config)
S12 (PageDown) - Start with Layout 1 or Layout 2 on powerup - must also press Sav(e) (Cfg)
Kbd (ArrowDwn] - A-Z 0-9 keyboard send keys. ADD to macro NXT keybrd ESC quit EXE send + quit
Sav (Cfg) Info and File List to send Serial Monitor and Text/Macro and Config files saved

Text Strings: 
Send new text strings up to 200 characters to keys S1/T1 - S12/T12 via USBserial
Start string with <1 to <6 followed by the string assigned to S1/T1-S12/T12 - end string with >
If current Layout is L3 then S1 to S6 changed if Layout is L4 then T1 to T6 changed
If current Layer is A then S1/T1-S6/T6 changed, layer B S7/T7-S12/T12 changed. 

To send the text "This is a string for key [S2]." use a serial terminal to send 
<2This is a string for key [S2].> with Layer 3 visible.

Send new text for Key [M4] - start with <0 end with > and send it with Layers 1 or 3 visible - if 
sent from Layer 2 it will be ignored. 
Example 1: Send <0This is [M4] Text> with Layers 1 or 3 visible - pressing [M4] will send the text 
string "This is [M4] Text" to the PC. 
Example 2: Send the macro 0x3C 0x30 0xE0 0xE1 0x29 0x3E (which is <0 Control Shift Escape >), 
with Layer 4 visible, then pressing [M4] will open the Task Manager - refer to Key-M4-Examples.jpg below for 
this example.

Keyboard Page 1              Page 2          Page 3          Page 4              Page 5
[abc] [def] [ghi] [ESC]  [ABC] to [XY_]  [012] to [9+-]  [1F3] to [1F2]  [ALT] [SHF] [CTR] [EXE]
[jkl] [mno] [pqr] [NXT]    Uppercase     Numbers            F1 -  F12    [GUI] [TEI] [CRF] [NXT]
[stu] [vwx] [yz ] [ADD]    Uppercase     Symbols          Symbols        [LHR] [UED] [UND] [ADD] 

Page 5      [ALT] = [ Alt-L ]  [  Alt-R ]  [ PrintScr]   [EXE]
Keyboard    [SHF] = [Shift-L]  [Shift-R ]  [ Delete  ]
            [CTR] = [Contr-L]  [Contrl-R]  [BackSpace]
            [GUI] = [ Win-L ]  [ Win-R  ]  [  NULL   ]   [NXT]   
            [TEI] = [  Tab  ]  [ Escape ]  [ Insert  ]
            [CRF] = [  C/R  ]  [   L/F  ]  [  Return ]
            [LHR] = [Arrow-L]  [  Home  ]  [ Arrow-R ]   [ADD]
            [UED] = [ArrowUp]  [  End   ]  [Arrow-Dwn]
            [UND] = [PageUp ]  [ Numlock}  [ PageDwn ]
       
The keyboard is has 5 pages each with 9 triple function keys (that is 135 different keys), and 3+1 control keys 
[ESC-EXE] [NXT] [ADD]. For example page 1 has keys [abc], [def], to [y,z,space]. To select a or b or c press the
abc key once, twice or thrice - to add it to a new macro press the ADD key else press another character-symbol
modifier key or press[NXT] for the next keyboard page. Press [ESC] to leave the keyboard or on page 5 press [EXE]
to send the macro to the PC. The maximum length of a text macro is 200 characters.

The macros on page 5 are modifiers (simultaneously pressed keys) such as Control + Alt + Delete (with a maximum of 
6 keys in a macro). To send this sequence press [CTR] [ADD] [ALT] [ADD] [SHF]x3 (3 times for delete) [ADD] [EXE]. 
To send a sequence such as [WinKey] + "r" press [GUI] [ADD] [NXT] [pqr]x3 [ADD] [NXT]x4 [EXE]. To assign Alt + 
PrintScreen to key [M2] press [Cfg] [Kbd] [NXT] 4 times then [ALT] [ADD] [ALT] 3 x then [ADD] [EXE} [Up]. Alt + 
PrtScr is then executed when pressing [M2]. Another example: [Cfg] [Kbd] [NXT]4x [SHF] [NXT] [abc] [ADD] [NXT]4x 
[EXE] will send "A" not "a" because Shift is pressed at the same time as "a". to toggle the sticky keys press the 
[SHF] [ADD] 5 times then press [EXE] and save to key [M2]. (For a complete macro add [C/R] [ADD] key at the end of this
sequence.) 

Macros sent to the PC from the built-in keyboard will be saved to file KeyBrdMacro2 and if so selected, assigned
to key [M2] if of the modifiers type, and is saved as KeyBrdMacro1 an assigned to key [M1], if of the sequence type 
(200 chars max). Press [Up] after sending the macros to the PC to assign them to [M1] or [M2]. Combining macros of 
both types are planned.

F1-F12 keys are sent as keycodes (simultaneous) and not keypress types - to send [F3] press [Cfg] [Kbd] [NXT] 3 times 
then [1F3] 3 times then [ADD] [NXT] [EXE]. Press [Up] to assign it to key [M2]. Once assigned it will survive a reboot.

Math-Greek-Algebra Keyboard This is a keyboard with 3 pages and 3 x 9 x 3 = 81 math and greek algebra symbols. It is sent 
to the PC as (hex) Unicode + [ALT] + [x] and can be directly entered into MSWord.

The [M6] key section in the code has a few examples of using Alt + Number Keypad for Maths and special symbols or 
characters. There are 2 ways to do it - the first is to type the character's Unicode (hex without the 0x or decimal) into 
MSWord and then press [ALT] + [x]. The second method is to hold the Alt key down, then type in a special code number using
the number keypad and then release the Alt key. There are examples of entering the open infinity symbol and the small pi 
synbol in the [M6] key section. 

Change the LCD blank timeout: Send the macro *tb*num with the built-in keyboard where num = 0,1-9 - *tb*0 = 120 hours, 
*tb*1 = 30 seconds

Numeric Keypad    [Bsp] [7] [8] [9]  Press the small SktBlue Pad on the Right to toggle the Number Keypad on/off.
                  [Ret] [4] [5] [6]  Also show the numeric keypad by sending macro *kb from the built-in Keyboard       
                  [ 0 ] [1] [2] [3]  
                  
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

#define HIDCons RID_CONSUMER_CONTROL 
#define HIDKbrd RID_KEYBOARD         

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

#define ArraySz(x) (sizeof(x)/sizeof((x)[0]))

const int NumButtons = 17;             // Total number of buttons = 12 keys (3 rows x 4 columns) + 5 pads on right
unsigned long NowMillis = 0;           // LCD Backlight Saver
unsigned long LastMillis = 0;          // LCD Backlight Saver
unsigned long VolMuteMillisNow = 0;    // if VolMute not pressed within 5 sec cancel Layout counter
unsigned long VolMuteMillisLast = 0;   // if VolMute not pressed within 5 sec cancel Layout counter
unsigned long VolMuteTimer = 6000;     // 6 Seconds VolMute counter timeout Timer
unsigned long TimePeriod = 120000;     // 120 seconds LCD Backlight Timer change with *tb*num=0,1-5 via keybrd 1=30 sec, 0=120 days
unsigned long RepTimePeriod = 500;     // After this key repeat is active
unsigned long RepLast = 0;             // Time when key first pressed
unsigned long RepNow = 0;              // Time when key still pressed
bool BackLightOn = true;               // LCD Backlight Saver
int VolMuteCount = 0;                  // Can be 0,1=Layout1+VolMute(On/Off),2=Layout2,3=Layout3,4=Layout4 
int Layout = 1;                        // Layout 1, 2, 3 or 4
int LayerAB = 0;                       // Switch [Layout A B] for [Layers 1 3 4] Keys [M1-M6][M7-M12] etc press [Sav](e)
bool CapsLock = false;                 // Green "C"
bool NumLock = false;                  // Green "N"
bool ScrollLock = false;               // Green "S" - Excel still use this
bool Change = false;                   // Indicators changed
int  ConfigKeyCount = 0;               // Start with Layout 2 Cfg key not pressed 
bool MuteDisable = true;               // Disable VolMute with Layout 1 2 3 4 select Default is L1-L4
bool VolDisable = false;               // Disable Vol+ Vol- with Del/BSpc Enter
bool MuteDisablePrev = true;           // Restore VolMute with Layout 1 2 3 4 select
bool VolDisablePrev = false;           // Restore Vol+ Vol- with Del/BSpc Enter
bool SaveLayout = false;               // Start with selected layout 
bool Media = false;                    // Four Media Keys in Layout 2
bool DelBS = false;                    // Delete or Backspace replace Vol+
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
uint16_t calData[5] = {404, 3468, 263, 3486, 5};    // **[1]**
//uint16_t calData[5];                              // **[2]**
// Replace above line with the number on display once calibration is complete
uint16_t t_x = 0, t_y = 0; // Touch coordinates

/////////////////////////////////////////////////////////////////////////////////////////
// Layout 1 to 4 Keys
/////////////////////////////////////////////////////////////////////////////////////////
// Key sizes and spacing
#define KEY_W 65 // Key width  65 x 4 = 260 + 12 x 5 = 320  
#define KEY_H 60 // Key height 60 x 3 = 180 + 15 x 4 = 240
#define KEY_SPACING_X 12    // 12 // X gap
#define KEY_SPACING_Y 15    // 15 // Y gap
// Keypad start position upper left
// #define KEY_X (KEY_W/2) + KEY_SPACING_X   // 65/2 + 12 = 45 X-axis centre first key
// #define KEY_Y (KEY_H/2) + KEY_SPACING_Y   // 60/2 + 15 = 45 Y-axis centre first key
#define KEY_X 32                             // 30 is off-screen on left
#define KEY_Y 36                             // 30 is the topmost with no cut-off
/////////////////////////////////////////////////////////////////////////////////////////////
// Layout 5 small Config Buttons M K M (Math Numeric Keyboard Media) between 1st and 3rd row
/////////////////////////////////////////////////////////////////////////////////////////////
#define PAD_W 14          // Key width  PAD_X is middle -> Pad X is from 317 to 303 
#define PAD_H 24          // Key height
#define PAD_SPACING_Y 38  // was 74 for 3 pads // Next below 
#define PAD_X 310         // config buttons right side
#define PAD_Y 36          // config buttons top
//////////////////////////////////////////////////////////////////////////////////
// Indicators status Caps-Num-Scroll C N S XPos[7] = {0, 0, 78, 156, 0, 78, 156}
//////////////////////////////////////////////////////////////////////////////////
#define IND_X 70  // Centred on this - Sppace in RH side about 16 pixels
#define IND_Y 139 // 
////////////////////////////////////////////////////////////////////////////////
// Message status line at bottom
////////////////////////////////////////////////////////////////////////////////
#define STATUS_X 150 // Centred on this - 160 not middle buttons are off-centre
#define STATUS_Y 222 // 224 bottom of y p etc cut-off 

#define KEY_TEXTSIZE 1   // Font size multiplier

// Delay between keypress to give USB HID time to respond
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
                             
uint16_t keyColor[12] = {};  // construct = {0}; should init all elements 0x00 required for local vars

uint16_t BackgroundColor[4] = {Black, Black, Black, Black};    // 4 Layouts 1-4

uint16_t ButtonOutlineColor[4] = {White, White, White, White}; // 4 Layouts 1-4

uint16_t CfgColour    = SkyBlue;                                // Layout 2 KeyBrd
uint16_t MediaColour  = Orange;                                 // Layout 2 Media Keys 
uint16_t MathColour0   = 0xFF51;                                // Math KeyBrd                               
uint16_t MathColour1   = 0xF692;                                // Teal colours
uint16_t MathColour3   = 0xEE0E;
uint16_t MathColour4   = 0xEDCB;
uint16_t MathColour2   = 0xE547;
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
#define F01  0x3A  
#define F02  0x3B
#define F03  0x3C
#define F04  0x3D
#define F05  0x3E
#define F06  0x3F
#define F07  0x40
#define F08  0x41
#define F09  0x42
#define F10  0x43
#define F11  0x44 
#define F12  0x45
#define F13  0x68
#define F14  0x69
#define F15  0x6A
#define F16  0x6B
#define F17  0x6C
#define F18  0x6D
#define F19  0x6E
#define F20  0x6F
#define F21  0x70
#define F22  0x71
#define F23  0x72
#define F24  0x73
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
#define NmL  0x53  //NumlockKey
////////////////////////////////////////////////////////////////////////////////////////////////
#define KpDiv             0x54  
#define KpMul             0x55
#define KpMin             0x56
#define KpAdd             0x57
#define KpRet             0x58
#define Kp1               0x59  // Keypad Number used in Alt + Number for special chars and Math Symbols
#define Kp2               0x5A  // Alternative is Number + Alt+X
#define Kp3               0x5B
#define Kp4               0x5C
#define Kp5               0x5D
#define Kp6               0x5E
#define Kp7               0x5F
#define Kp8               0x60
#define Kp9               0x61
#define Kp0               0x62
#define KpDot             0x63
#define KpEqu             0x67
///////////////////////////////////////////////////////////////////////////////////////////////
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
// GUI + . or GUI + ;         - Emoji and (some) Math Symbols (click on Omega) insert
// Alt + NumPad Number        - special symbols math characters etc - Numlock must be on Alt+3 Alt+24 Alt+8733 Alt+10102
// Number + Alt+X             - Special symbols
// GUI+R charmap = Enter      - Character Map
// GUI+Space                  - Choose Key Layout
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Two Layers A B of two by four layout sets of 12 button labels <layer><layout><name>[<number-of-lables>][<number-of-chars-per-label]
// The number of chars per label include the termination char \0 => 3 character labels here:
// ~ https://wiki.sei.cmu.edu/confluence/display/c/STR11-C.+Do+not+specify+the+bound+of+a+character+array+initialized+with+a+string+literal
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char Labels[2][16][12][4] =                                            
{ CcP,   "V+",  M13,  "Vo",  M46,   "V-",  // L1 Mute ON Vol=ON   0     Group 0 Layer A
  HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2                  1
  XCV,   "V+",  S13,  "Vo",  S46,   "V-",  // L3                  2
  XCV,   "V+",  T13,  "Vo",  T46,   "V-",  // L4                  3
  CcP,   "V+",  M13,  "L1",  M46,   "V-",  // L1 Mute OFF Vol=ON    4    Group 1 Layer A 
  HPU,   "V+",  LcR,  "L2",  EPD,   "V-",  //                       5
  XCV,   "V+",  S13,  "L3",  S46,   "V-",  //                       6
  XCV,   "V+",  T13,  "L4",  T46,   "V-",  //                       7
  CcP,   "BsD", M13,  "Vo",  M46,   "Ret", // L1 Mute ON Vol=OFF  8      Group 2 Layer A
  HPU,   "BsD", LcR,  "Vo",  EPD,   "Ret", //                     9
  XCV,   "BsD", S13,  "Vo",  S46,   "Ret", //                     10
  XCV,   "BsD", T13,  "Vo",  T46,   "Ret", //                     11
  CcP,   "BsD", M13,  "L1",  M46,   "Ret", // L1 Mute OFF Vol=OFF    12  Group 3 Layer A
  HPU,   "BsD", LcR,  "L2",  EPD,   "Ret", //                        13
  XCV,   "BsD", S13,  "L3",  S46,   "Ret", //                        14
  XCV,   "BsD", T13,  "L4",  T46,   "Ret", //                        15
  CcP,   "V+",  M79,  "Vo",  M12,   "V-",  // L1 Mute ON Vol=ON          Group 0 Layer B 
  HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2
  XCV,   "V+",  S79,  "Vo",  S12,   "V-",  // L3
  XCV,   "V+",  T79,  "Vo",  T12,   "V-",  // L4
  CcP,   "V+",  M79,  "L1",  M12,   "V-",  // L1 Mute OFF Vol=ON         Group 1 Layer B
  HPU,   "V+",  LcR,  "L2",  EPD,   "V-",
  XCV,   "V+",  S79,  "L3",  S12,   "V-",
  XCV,   "V+",  T79,  "L4",  T12,   "V-",
  CcP,   "BsD", M79,  "Vo",  M12,   "Ret", // L1 Mute ON Vol=OFF         Group 2 Layer B
  HPU,   "BsD", LcR,  "Vo",  EPD,   "Ret",
  XCV,   "BsD", S79,  "Vo",  S12,   "Ret",
  XCV,   "BsD", T79,  "Vo",  T12,   "Ret",
  CcP,   "BsD", M79,  "L1",  M12,   "Ret", // L1 Mute OFF Vol=OFF        Group 3 Layer B 
  HPU,   "BsD", LcR,  "L2",  EPD,   "Ret",
  XCV,   "BsD", S79,  "L3",  S12,   "Ret",
  XCV,   "BsD", T79,  "L4",  T12,   "Ret"};
//0 1 2      4   5 6 7   8   9 10 11   12
char keyLabel[12][4] = {};                 // = ""; all 0x00  
/////////////////////////////////////////////////////////////////////////////////////////////////
//  If Cfg Key pressed new options (new Labels and new Colours)
//  Labels[0/1][1,5,9,13][0-11][0-3]
//  "Hme", "Up",  "PgU", "Del/V+", "<",   "Cfg", ">",  "V0/L14",  "End", "Dwn", "PgD", "V-/Ret",
//  "L14", "  ",  "Vol", "      ", "AB",  "Sav", " ",  "      ",  "Del", "   ", "S12", "      "
/////////////////////////////////////////////////////////////////////////////////////////////////
const static char CfgLabel[12][4] =                                          
{"L14", "BsD",  "Vol", "   ", "A-B",  "Sav", "Med",  "   ",  "Del", "Kbd", "S12", "   "};  // All 3 chars + \0

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
//  If Mouse Keys selected via KeyBrd command *mk* new options:
//                 "Hme", "Up",  "PgU", "DelV+",  "<",   "Cfg",  ">",   "V0L14",  "End", "Dwn",  "PgD", "V-Ret"
// Mousekeys1[x][] "LUp", "Up",  "RUp", "L-R",    "<",   "1-c",  ">",   "2-c",    "Ldw", "Dwn",  "RDw", "Hld" 
// keyLabel[x][]     0             2                4               6                                            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char Mousekeys1[12][4] = 
{"LUp", "Up",  "RUp", "L-R",    "<",   "1-c",  ">",   "2-c",    "Ldw", "Dwn",  "RDw", "Hld"}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  If Numeric Keys selected via KeyBrd command *kb* new options:
//                 "Hme", "Up", "PgU", "DelV+", "<",   "Cfg",  ">",  "V0L14", "End", "Dwn", "PgD", "V-Ret"
// Numkeys1[x][]   "BsD", "7",  "8",   "9",     "Ret",  "4",    "5",  "6",     "0",   "1",   "2",    "3"
// keyLabel[x][]     0             2                4               6                                            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char Numkeys1[12][4] = 
{"BsD", "7",  "8",   "9",  "Ret",  "4",  "5",  "6",  "0",  "1",  "2", "3"}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// If sequence starts with modifiers (Shift etc) then up to 6 keys sent together, else sent in sequence
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char KbrdLabel[5][12][4] =                                                    // Xlate only needed for kcode not Keypress
{"abc",  "def",  "ghi",  "ESC", "jkl", "mno", "pqr", "NXT", "stu", "vwx", "yz ", "ADD",    // ASCII->HID Subtract  dec
 "ABC",  "DEF",  "GHI",  "esc", "JKL", "MNO", "PQR", "nxt", "STU", "VWX", "YZ_", "add",    // ASCII->HID Subtract 61 dec
 "012",  "345",  "678",  "ESC", "9+-", "=*/", "!@#", "NXT", "$%^", "&,.", "~` ", "ADD",    // 1-9 subtract 19 dec 0 subtract 9 dec
 "\\|?", "\"<>", "\'()", "ESC", ":[]", "{};", "1F3", "NXT", "4F6", "7F9", "1F2", "ADD",    // F1-F12 special code
 "ALT",  "SHF",  "CTR",  "EXE", "GUI", "TEI", "CRF", "NXT", "LHR", "UED", "UND", "ADD"};      // Mainly modifiers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char KeyBrdSpc[3][12][4] = 
{"ALT",  "SHF",  "CTR",  "   ", "GUI", "Tab", "C/R", "  ", "A-L", "A-U", "PgU", "ADD",
 "A-R",  "S-R",  "C-R",  "   ", "G-R", "Esc", "L/F", "  ", "Hme", "End", "PgD", "ADD",
 "PrS",  "Del",  "BSp",  "   ", "Nul", "Ins", "RET", "  ", "A-R", "A-D", "Bsp", "ADD"};
const static byte KeyBrdNum[3][12] = 
{AltL,   ShfL,   CtrL,    0,    WinL,  Tab,   0x0D,  0, ArrLeft,  ArrUp,  PageUp, 0,
 AltR,   ShfR,   CtrR,    0,    WinR,  Esc,   0x0A,  0, KeyHome,  KeyEnd, PageDwn,0,
 PrtScr, DelK,   BckS,    0,    0x00,  Ins,   0x28,  0, ArrRight, ArrDown,NmL,    0};
const static byte KeyBrdFx[6][5] =   // F01 to F24 must use keycode keyPress 0x3C = < not F3
{F01, 0, F04, F07, F10,              // else put a modifier before Fx such as Shift + F3      
 F02, 0, F05, F08, F11, 
 F03, 0, F06, F09, F12,
 F13, 0, F16, F19, F22,               
 F14, 0, F17, F20, F23, 
 F15, 0, F18, F21, F24};
const static char KeyBrdFxChr[6][5][4] = // F01 to F24
{"F01", "   ", "F04", "F07", "F10", 
 "F02", "   ", "F05", "F08", "F11", 
 "F03", "   ", "F06", "F09", "F12", 
 "F13", "   ", "F16", "F19", "F22", 
 "F14", "   ", "F17", "F20", "F23", 
 "F15", "   ", "F18", "F21", "F24"};

//////////////////////////////////////////////////////////////////////////////////////
// Unicodes from Mathboard at https://github.com/nup002/Mathboard (MIT license)
// Greek Letters lower-case alpha to omega
// alpha beta gamma delta epsilon zeta eta theta iota kappa lambda
// mu nu xi omikron pi rho sigma(1,2) tau upsilon phi chi psi omega  
//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Greek Letters Upper-case and Set theory and Various
// Gamma Delta Theta Lambda Xi Pi Sigma Phi Psi Omega 
// Set theory:  not_subset_or_equal disjoint_union union          intersection    element_of not_element_of 
//              empty_set           subset_of      not_subset_of  set_difference  subset_or_equal 
// Various:  dotproduct crossproduct plusminus parallel degree infinity 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Algebra and calculus: sum  n_ary_product  integral  double_integral  triple_integral  line_integral  surface_integral  
//                       volume_integral  square_root  cube_root  fourth_root  arrow  partial_derivative  nabla
// Comparison and equivalence: notequal almostequal proportional identicalto lessorequal greaterorequal 
// Modifiers:  circumflex check combining_tilde combining_bar combining_arrow combining_dot combining_2dot prime double_prime 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MathLabel[3][12][4] =                                                    
{"abg",  "dez",  "ehi",  "ESC", "klm", "nko", "prs", "NXT", "stu", "fcp", "oO ", "EXE",
 "GDH",  "LKP",  "SFP",  "ESC", "Os1", "set", "set", "NXT", "set", "var", "var", "EXE",
 "Alg",  "Cal",  "Cal",  "ESC", "Alg", "Cal", "Com", "NXT", "Com", "Mod", "Mod", "EXE" };
///////////////////////////////////////////////////////////////////////////////////////////
// Type in Alt + Numpad key then after symbol show press Alt+X to get hex codes used below
///////////////////////////////////////////////////////////////////////////////////////////
const static char MathNum[3][3][12][5] = 
{"03B1",  "03B4",   "03B7",  "0   ",  "03BA",   "03BD",   "03C0",   "0  ",  "03C3",  "03C6",  "03C9", "0  ",
 "03B2",  "03B5",   "03B8",  "0   ",  "03BB",   "03BE",   "03C1",   "0  ",  "03C4",  "03C7",  "0000", "0  ",
 "03B3",  "03B6",   "03B9",  "0   ",  "03BC",   "03BF",   "03C2",   "0  ",  "03C5",  "03C8",  "0000", "0  ",
 "0393",  "039B",   "03A3",  "0   ",  "03A9",   "222A",   "2209",   "0  ",  "2284",  "22C5",  "2225", "0  ",
 "0394",  "039E",   "03A6",  "0   ",  "2288",   "2229",   "2205",   "0  ",  "2216",  "2A2F",  "00B0", "0  ",
 "0398",  "03A0",   "03A8",  "0   ",  "2A06",   "2208",   "2282",   "0  ",  "2286",  "00B1",  "221E", "0  ",
 "2211",  "2230",   "222D",  "0   ",  "221C",   "220F",   "2260",   "0  ",  "2261",  "0303",  "0307", "0  ",
 "2192",  "222B",   "222E",  "0   ",  "221A",   "2202",   "2248",   "0  ",  "2264",  "0304",  "0308", "0  ",
 "2207",  "222C",   "222F",  "0   ",  "221B",   "2032",   "221D",   "0  ",  "2265",  "20D7",  "2033", "0  " }; 
const static char MathName[3][3][12][20] = 
{"alpha", "delta",   "eta",   "0", "kappa",              "nu",          "pi",            "0 ","sigma2",         "phi",          "omega",          "0",
 "beta",  "epsilon", "theta", "0", "lambda",             "xi",          "rho",           "0 ","tau",            "chi",          "Omega",          "0",
 "gamma", "zeta",    "iota",  "0", "mu",                 "omicron",     "sigma1",        "0 ","upsilon",        "psi",          "space",          "0",
 "Gamma", "Lambda", "Sigma",  "0", "Omega",              "union",       "not-element-of","0", "not-subsetof",   "dotproduct",   "parallel",       "0",
 "Delta", "Xi",     "Phi",    "0", "not-subset-or-equal","intersection","empty-set",     "0", "setdifference",  "crossproduct", "degree",         "0",
 "Theta", "Pi",     "Psi",    "0", "disjoint-union",     "element-of",  "subset-of",     "0", "subset-or-equal","plusminus",    "infinity",       "0",
 "sum",  "volume-integral","triple-integral", "0", "fourth-root", "n-ary-prod",    "notequal",     "0", "identicalto",   "combining-tilde", "combining-dot",  "0",
 "arrow","integral",       "line-integral",   "0", "square-root", "partial-deriv", "almostequal",  "0", "lessorequal",   "combining-bar",   "combining-2dot", "0",
 "nabla","double-integral","surface-integral","0", "cube-root",   "prime",         "proportional", "0", "greaterorequal","combining-arrow", "double-prime"    "0" };
 
// 5 Small Config Buttons between 1 st and 3rd row Red Blue Green SkyBlue Gold
char padLabel[5][4] = {"", "", "", "", ""}; 
uint16_t padColor[15]   = {Black, Black, Black, Red, Blue, Green4, SkyBlue, Gold, Black, Black, Black, Black, Black, Black};
uint16_t padOutline[15] = {Black, Black, Black, White, White, White, White, White, Black, Black, Black, Black, Black, Black};


bool Math = false;               // Layer for Math KeyBrd
int  MathX = 0;                  // values 0 1 2 3 4 - 4 adds non-printing chars as numbers to KeyBrdByte
byte MathByte[200]={};           // Hold values to be sent
byte MathCode[200];              // Parallel keycodes
int  Math123 = 0;                // Such as Key [abc] = a or b or c - values 0,1,2
int  MathByteNum = 0;            // Size Math KeyBrdByte
char MathChr[20];                // Current Math description
char MathHexNum[5];              // Current Math Hex Number as ASCII without 0x

bool Fx = false;                 // F1-F12 current
bool Kbrd = false;               // Layer for KeyBrd
int  KeyBrdX = 0;                // values 0 1 2 3 4 - 4 adds non-printing chars as numbers to KeyBrdByte
byte KeyBrdByte[200]={};         // Hold values to be sent
byte KeyCode[200];               // Parallel keycodes
int  KeyBrd123 = 0;              // Such as Key [abc] = a or b or c - values 0,1,2
int  KeyBrdByteNum = 0;          // Size KeyBrdByte

byte MacroM1[200];               // M1 key sequence macro buffer
byte MacroM2[200];               // M2 key simultaneous key macro buffer (6 bytes) x more

int  PrevButton = 0;             // Used in case-button to check for double key press
char KBDisp[200]={};             // Keybrd display macro buffer in status line
int  KBDispPos = 0;              // nect char position
bool KBType = false;             // If 1st byte>0x7F use keycode[0-5] else use keyPress
bool KeyRepeat = false;          // If button same a sprevious button pressed
bool KBrdActive = false;         // Let [ADD] know any char key has been pressed at least once before
bool KeyBrdDirect = false;       // Echo Keyboard keys to PC as they are typed
int  Macro1, Macro2 = 0;         // Ststus of KeyBrd Macros 1 = has been saved, 2 = use for M1 M2
int  Macro1Size, Macro2Size = 0; // Ststus of KeyBrd Macros 1 = has been saved, 2 = use for M1 M2
bool MouseKeys = false;          // Mouse Keyboard
bool NumKeys = false;            // Numeric Keyboard

bool PadKeys = false;            // 5 Small Config Buttons on RH side Red Blue Green
int  PadKeyRBG = 0;              // Red Blue Green SkyBlue Gold-Yellow = 0 1 2 3 4
bool PadKeyR = false;
bool PadKeyB = false;
bool PadKeyG = false;
bool PadKeyS = false;
bool PadKeyY = false;
 
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if [Cfg] then [Home][UpArrow][PageUp]  [<][>]  [End][DwnArrow][PageDown] keys pressed
//            StrButton[button]     0  1  2  3  4  5  6  7  8  9  10 11          // 3 7 8 = V+ Vo V-
//                        CfgOK     x  x  x     x  x  x     x  x  x              // Not used with
static const byte CfgButton[12] = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };        // Cfg key
//////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                StrButton[button]                4  5  6     8  9  10                4  5  6     8  9  10
//  M1-M6 M7-M12 etc            c                  0  1  2     3  4  5                 6  7  8     9  10 11
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

const int  StrSize =  200;     // Check if not byte used if mafe larger 
const int  ByteSize = 200;
const byte MaxBytes = StrSize;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Must do the different sources for M1-M12 macro's properly - too many currently
/////////////////////////////////////////////////////////////////////////////////////////////////////
char Str1[StrSize],  Str2[StrSize],  Str3[StrSize],  Str4[StrSize],   Str5[StrSize],   Str6[StrSize];
char Str7[StrSize],  Str8[StrSize],  Str9[StrSize],  Str10[StrSize],  Str11[StrSize],  Str12[StrSize];
char Ttr1[StrSize],  Ttr2[StrSize],  Ttr3[StrSize],  Ttr4[StrSize],   Ttr5[StrSize],   Ttr6[StrSize];
char Ttr7[StrSize],  Ttr8[StrSize],  Ttr9[StrSize],  Ttr10[StrSize],  Ttr11[StrSize],  Ttr12[StrSize];
byte Mtr1[ByteSize], Mtr2[ByteSize], Mtr3[ByteSize], Mtr4[ByteSize],  Mtr5[ByteSize],  Mtr6[ByteSize];
byte Mtr7[ByteSize], Mtr8[ByteSize], Mtr9[ByteSize], Mtr10[ByteSize], Mtr11[ByteSize], Mtr12[ByteSize];

char * Str1to12[] = {Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,Str11,Str12} ;   
char * Ttr1to12[] = {Ttr1,Ttr2,Ttr3,Ttr4,Ttr5,Ttr6,Ttr7,Ttr8,Ttr9,Ttr10,Ttr11,Ttr12} ;
byte * Mtr1to12[] = {Mtr1,Mtr2,Mtr3,Mtr4,Mtr5,Mtr6,Mtr7,Mtr8,Mtr9,Mtr10,Mtr11,Mtr12} ;
 
char inputString[StrSize] = "Send new text through serial port start with <x end with >\n where x = key number 1 to 6 or 0 for [M4]\n";  // String for [M4] Key
bool StrOK = false;          // String is complete - ended with 0x0A
bool ByteOK = false;         // Bytes received is complete - ended with 0x00
int  RecBytesLen = 0;        // Number of chars in RecBytes
int  RecBytesType = 1;       // Type 1, 2, 3 = sequence, simulataneous, media
byte RecBytes[MaxBytes];     // Raw bytes received must start < and end with > - can have length byte/int after < to include <> in data
byte NumBytes = 0;
bool NewData = false;
//char AltNum[] = "120529";  // Used for Math symbol example in [M6] key section (button function)
char AltNum[] = "1D6D1";

// Create 12 keys + 5 config buttons for keypad
TFT_eSPI_Button key[NumButtons];

///////////////////////////////////////
// Setup
///////////////////////////////////////
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);   // Used for Capslock
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);

  if (!LittleFS.begin()) {LittleFS.format(); LittleFS.begin(); }
  /////////////////////////////////////////////////////////////////////////////////////////
  // if (MuteDisable) status("Volume Mute OFF"); else status("Volume Mute ON");  
  // if (VolDisable)  status("Volume Keys OFF"); else status("Volume Keys ON"); 
  // if (DelBS) status("Backspace ON"); else status("Delete ON");
  // if (Kbrd) status("Esc-Exe Next Add");
  // if (SaveLayout) status("Startup Layout L2"); else status("Startup Layout L1");
  // if (Media) status("Media Keys ON"); else status("Media Keys OFF"); 
  // if (LayerAB==0) {LayerAB=1; status("Layer B");} else {LayerAB=0; status("Layer A");} 
  /////////////////////////////////////////////////////////////////////////////////////////
  if (LittleFS.exists("MuteDisable"))      MuteDisable = false;  else MuteDisable = true;   // L1-L4 enabled = default VolMute Key disabled 
  if (LittleFS.exists("VolDisable"))       VolDisable  = true;  else VolDisable  = false;   // V+ V- disabled Delete-Return enabled
  if (LittleFS.exists("Layout"))           SaveLayout  = true;  else SaveLayout  = false;   // Selected Saved Layout restored
  if (LittleFS.exists("LayerB"))           LayerAB     = 1;     else LayerAB     = 0;       // Layer A 1-6 or B 7-12 restored
  if (LittleFS.exists("MediaKeys"))        Media       = true;  else Media       = false;   // Media Keys in Layout 2 restored
  if (LittleFS.exists("DeleteBackspace"))  DelBS       = true;  else DelBS       = false;   // Toggle Delete or Backspace key
  
  InitStr();  // ByteOK=StrOK=false Copy str# to Str# and Ttr# if no saved file else read Str# Ttr# from files, if Mtr files exist copy those to M1 to M12
   
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
  delay(18000);                      
  */                                 // **[4]**

  tft.setTouch(calData);   // void setTouch(uint16_t *data); // TFT_eSPI/Extensions/Touch.h 
  
  if (SaveLayout) Layout = 2; else Layout = 1;  // Layout 1 (M1-M12 keys default) or 2 (Config)
  ConfigKeyCount = 0;                           // Start up
  ConfigButtons(0);                             // Draw Buttons and Labels 0 = All 3+5 rows
  
  LastMillis = millis();                        // Initial start time
  VolMuteMillisLast = millis();                 // Layout Change Timeout
  VolMuteCount = 0;                             // Layout change
  NewData = false;
  KBrdActive = false; 
  Kbrd = false;
  KeyBrdByteNum = KBDispPos = KeyBrdX = 0;
  KeyBrdDirect = false; 
  KeyBrd123 = 0;
  PadKeys = false;                              // 5 Small Config Buttons on RH side Red Blue Green SkyBlue Gold
  PadKeyRBG = 0;                                // Red Blue Green = 0 1 2 3 4
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
  if ((NowMillis - LastMillis) >= TimePeriod)     //test whether the period has elapsed
     {digitalWrite(LCDBackLight, LOW);            // Backlight Off
      //analogWrite(LCDBackLight, 128);           // Backlight Dimmed
      LastMillis = NowMillis;                     // Start Tiemer again
      RepLast = RepNow = NowMillis;               // Reset repeat key timer
      if (!MuteDisable) VolMuteCount = 0;         // VolMuteKey pressed Count = 0 if not L1-L4 keys active
      /*
      if (ConfigKeyCount>0)                       // If [Cfg]->[Kbd] then backlight times out but still shows kbd,
         {ConfigKeyCount = 0;                     // save etc keys even though its back to arrows home end etc
         ConfigButtons(1); }                      // [Cfg][End] or [Cfg][Hme] sequences active
      */
      if (!Kbrd) status("");                      // Clear the status line if KeyBrd not active
      BackLightOn = false;   }                    // Until keypress    
    
  ///////////////////////////////////////////////////////////////////////////////////////////////////////  
  // uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600) in TFT_eSPI/Extensions/Touch.h
  // tft.setTouch(calData);              // void setTouch(uint16_t *data); // TFT_eSPI/Extensions/Touch.h 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  bool pressed = tft.getTouch(&t_x, &t_y, 250);                              // True if valid key pressed
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Check if any key coordinate boxes contain the touch coordinates - no multitouch
  // void TFT_eSPI_Button::press(bool p) { laststate = currstate; currstate = p; }
  // Shorter: key[b].press((pressed && key[b].contains(t_x, t_y)));
  // boolean Adafruit_GFX_Button::contains(int16_t x, int16_t y) 
  //       { return ((x >= _x1) && (x < (_x1 + _w)) && (y >= _y1) && (y < (_y1 + _h))); }
  // justPressed  Query whether the button was pressed since last checked state.  Returns True if was not-pressed before, now is.
  // justReleased Query whether the button was released since last checked state. Returns True if was pressed before, now is not. 
  // isPressed    Query whether the button is currently pressed.                  Returns True if pressed 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for (uint8_t b = 0; b < NumButtons; b++) { if (pressed && key[b].contains(t_x, t_y)) key[b].press(true); else key[b].press(false); }                    
   
  // Check if any key changed state void press(bool p); bool isPressed() bool justPressed(); bool justReleased();
  RepNow = millis();   // get the current time
  for (uint8_t b = 0; b < NumButtons; b++) { if (key[b].justReleased()) {key[b].drawButton(false); RepLast = RepNow; }                      // draw normal - code at release time
                                     if (key[b].justPressed())  {key[b].drawButton(true); { buttonpress(b); RepLast = millis(); }}  // draw invert - code at press time  
                                     else
                                     if (((RepNow - RepLast) >= RepTimePeriod) && (key[b].isPressed()))                             // code while the button is held
                                          {buttonpress(b); }  }                                                                     // Send the button number

  RecSerial();
  // if (NewData) showRecData();
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
  int ASize;

  Found = NewData = StrOK = ByteOK = false;
  
  a = RecBytes[0] - 48;  // ASCII Number 0-9 subtract 48 
  Found = (a<7);
    
  if (Found){ if ((LayerAB==1)&&(a>0)) c = a + 5; else c = a - 1;    // S1-S6=>S7-S12 T1-T6=>T7=T12 M1-M6=>M7=M12

              if (Layout==3) {if (a>0) {StrOK = true;  NameStr = STRname[c]; ChrPtr = Str1to12[c]; }   // Keys S1-S12
                                                 else {NameStr = "inputStr"; ChrPtr = inputString; } } // Key [M4] normal text
              if (Layout==4) {if (a>0) {StrOK = true;  NameStr = TTRname[c]; ChrPtr = Ttr1to12[c]; }   // Keys T1-T12
                                  else {ByteOK = true; return; }   }                                   // Key [M4] non-ASCII in RecBytes[] 
              if (Layout==1) {if (a>0) {ByteOK = true; NameStr = MTRname[c]; BytePtr = Mtr1to12[c];}   // Keys M1-M12
                                                 else {NameStr = "inputStr"; ChrPtr = inputString; } } // Key [M4] normal text
              
              for (n=1; n<=NumBytes; n++) ChrPtr[n-1] = RecBytes[n];     
              for (n=1; n<=NumBytes; n++) BytePtr[n-1] = RecBytes[n]; }   // End if (Found)
              
   if ((a>6)&&(a<10)) {Found = true; n--; RecBytesType = a - 6; ByteOK = true;}              // Non ASCII start string with char 7 - 9
   
   if ((Found)&&(StrOK)) {DoFileStrings(StrOK, NameStr, ChrPtr);   // Save the text/num strings 
                                  status(ChrPtr); }                // Display the new string       
     
   if ((Found)&&(ByteOK)) {ASize = DoFileBytes(ByteOK, NameStr, BytePtr, NumBytes); } // Save the numbers 
                                                 
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
// (2) Keyboard Keys pressed together    - usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode) - keycode 0 - 5 keys pressed simultaneously
// (3) Keyboard Key 1 or more held + others typed in sequence - Alt + Numpad for Math sym,bols + special characters eg. Alt-Hold + 2, 3
// (4) Media Keys pressed in sequence    - usb_hid.sendReport16(RID_CONSUMER_CONTROL, mediakeyvar) - mediakeyvar single variable
// The challenge with macro instructions is then to know how to deal with each type (1) to (4) or any combination of (1) to (4)
// Generally if non-test macro has Shift, Alt, GUI, Ctrl as the first key then it will be of type (2)  03 (3) (at least initially)
// If any other first key then is a normal keyboard key it will be of type (1) initially
// If any other first media key then is type (4) initially 
//
// typedef struct TU_ATTR_PACKED {
//  uint8_t modifier;   /**< Keyboard modifier (KEYBOARD_MODIFIER_* masks). */
//  uint8_t reserved;   /**< Reserved for OEM use, always set to 0. */
//  uint8_t keycode[6]; /**< Key codes of the currently pressed keys. */
//  } hid_keyboard_report_t;
//  Keyboard modifier codes bitmap
//  typedef enum {
//  KEYBOARD_MODIFIER_LEFTCTRL   = TU_BIT(0), ///< Left Control
//  KEYBOARD_MODIFIER_LEFTSHIFT  = TU_BIT(1), ///< Left Shift
//  KEYBOARD_MODIFIER_LEFTALT    = TU_BIT(2), ///< Left Alt
//  KEYBOARD_MODIFIER_LEFTGUI    = TU_BIT(3), ///< Left Window
//  KEYBOARD_MODIFIER_RIGHTCTRL  = TU_BIT(4), ///< Right Control
//  KEYBOARD_MODIFIER_RIGHTSHIFT = TU_BIT(5), ///< Right Shift
//  KEYBOARD_MODIFIER_RIGHTALT   = TU_BIT(6), ///< Right Alt
//  KEYBOARD_MODIFIER_RIGHTGUI   = TU_BIT(7)  ///< Right Window
//  }hid_keyboard_modifier_bm_t;
//                                         modifier reserved=0 keycode[6]
//  usb_hid.keyboardReport(RID_KEYBOARD=1, 0,                  keycode)
//
//  usb_hid.keyboardReport(KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTALT, 0, 0); // press shift + alt
//  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void buttonpress(int button)
{
  uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here
  uint8_t i, n, a, b, c;          // 
  uint8_t CfgOK ;                 // Do Cfg actions
   
  if ( !usb_hid.ready() ) return; 

  /////////////////////////////////////////////////////////
  // 5 Config Pads Red Blue Green SkyBlue Gold button > 11
  // Handle 1st then return - as if button = 0-11
  /////////////////////////////////////////////////////////
  if (button>11) 
     {PadKeys = true; 
      if (button==12) { PadKeyR = (!PadKeyR); PadKeyB = false;                        // PadKeyRBG 0 Toggle Math Pad 
                        if (Kbrd) SendBytesEnd();                                     // This sets Kbrd = false;
                        //if (Media) Media = false;                                   // If Mediakeys on stay on
                        Math = PadKeyR; ConfigButtons(1);  status(" "); return; }
                       
      if (button==14) { PadKeyG = (!PadKeyG);                                         // PadKeyRBG 2 Toggle Media Keys
                        ConfigKeyCount =0; Media = PadKeyG; 
                        if (Kbrd) { Kbrd = false; PadKeyB = false; } 
                        if (Math) { Math = false; PadKeyR = false; SendBytesEnd(); } 
                        if (Media){ MuteDisablePrev = MuteDisable; VolDisablePrev = VolDisable;
                                    MuteDisable = false; VolDisable = false;
                                    status("Media Keys ON");} 
                              else {status("Media Keys OFF"); MuteDisable = MuteDisablePrev; VolDisable = VolDisablePrev; }
                        ConfigButtons(1);  return; }                                                                    
                                                                          
      if (button==15) { PadKeyS = (!PadKeyS); PadKeyB = false;                        // PadKeyRBG 0 Toggle Numeric Pad 
                        if (Kbrd) SendBytesEnd();                                     // This sets Kbrd = false;
                        //if (Media) Media = false;                                   // If Mediakeys on stay on
                        NumKeys = PadKeyS; ConfigButtons(1);  status(" "); return; }
      
      if (button==16) { PadKeyY = (!PadKeyY);                                         // PadKeyRBG 0 Toggle 
                        status(" "); return; }
      
      PadKeyB = (!PadKeyB);  PadKeyRBG = button-12;                                  // PadKeyRBG 1 Toggle Keyboard
      if (Math) { Math = false; PadKeyR = false; SendBytesEnd(); } 
      if (Media) Media = false;
      ConfigKeyCount = 0; Kbrd = PadKeyB; 
      if (Kbrd) status("Esc-Exe Next Add"); ConfigButtons(1); return;                                          
     }
  
  n = 0;                          // for the while loops   

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
  if ((Layout==2) && (CfgOK==0) && (ConfigKeyCount>0)) {ConfigKeyCount = 0; status(" "); } 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////// 
                                                           
  if ((Layout!=2)) {   // Layout 1 or 3 or 4   
  switch(button){
    case 0: ///////////////////////////////////////////////// Cut = control + x                             
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_X; // actually x not X unless Capslock ON
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 1: ///////////////////////////////////////////////// Copy = control + c
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_C;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 2: ////////////////////////////////////////////////// Paste = control + v
      keycode[0] = CtrL;
      keycode[1] = HID_KEY_V;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);   break; 
      
    case 3: /////////////////////////////////////////////////// Volume Increase
      if (VolDisable) {if (DelBS) keycode[0] = BckS; else keycode[0] = DelK;  //Delete or BackSpace Key
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(HIDKbrd); break; }
    
      usb_hid.sendReport16(HIDCons, VolUp);  delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);      break;
      
    case 4: // M1 S1 T1 ///////////////////////////////////////////////// Alt + PrintScreen - some mini keyboards cannot do this
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } break; }
      
      if (Layout==1) 
         {if (Macro1==2) 
             { for (n=0;  n<Macro1Size; n++) 
                  {usb_hid.keyboardPress(HIDKbrd, Mtr1[n]); delay(keydelay2);
                   usb_hid.keyboardRelease(HIDKbrd);        delay(keydelay2); } break; }                 
                     
         keycode[0] = AltL; 
         keycode[1] = PrtScr;
         keycode[2] = 0x00; // Need this why? 
         usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
         usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2);
         status("PrtScr Current Window");  break;   }                            // Layout==1
      
    case 5: // M2 S2 T2 ///////////////////////////////////////////////// Open admin powershell including UAC completion
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } break; }
                                                                                
      if (Layout==1) 
         {if (Macro2==2) 
             {for (n=0;  n<Macro2Size; n++) {keycode[n] = Mtr2[n]; }
              usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); 
              usb_hid.keyboardRelease(HIDKbrd); break; }
              
          keycode[0] = HID_KEY_GUI_LEFT; // or use HID_KEY_GUI_LEFT 
          keycode[1] = HID_KEY_X;
          usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); // GUI + X then A admin powershell i normal powershell g Computer Management
          usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); // Confirm with ArrowLeft Enter 
          usb_hid.keyboardPress(HIDKbrd, 'A');           delay(keydelay2); // Can also use Alt + "Y" too confirm
          usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay3); // Can also open via Run GUI + R then Ctrl+Shft+Enter then Alt+Y
          keycode[0] = ArrLeft;
          keycode[1] = KeyEnter;
          usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); 
          usb_hid.keyboardRelease(HIDKbrd);  break; }                      // Layout==1
      
    case 6: // M3 S3 T3 /////////////////////////////////////////////// Open Run window with last coommand visible and selected
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } break; }  
      
      if (Layout==1) { n = 0; if (Mtr1to12[c][0] != 0x00)                        // set to 0x00 in Do FileBytes() if file does not exist
                                 {while (Mtr1to12[c][n] != 0x00) 
                                        {usb_hid.keyboardPress(HIDKbrd, Mtr1to12[c][n]); delay(keydelay2);
                                         usb_hid.keyboardRelease(HIDKbrd);  n++;         delay(keydelay2); }  ;break;} }               
                          
      keycode[0] = HID_KEY_GUI_RIGHT; // or use HID_KEY_GUI_LEFT
      keycode[1] = HID_KEY_X;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 7: //////////////////////////////////////////////// Volume Mute and change Layouts
      if ((MuteDisable) && (VolMuteCount==0)) VolMuteCount++;
      VolMuteCount++;
      if (VolMuteCount==2) {Layout=2; ConfigButtons(1); delay(keydelay); break; }                      // Count = 2 Navigate
      if (VolMuteCount==3) {Layout=3; ConfigButtons(1); delay(keydelay); break; }                      // Count = 3 Text Strings
      if (VolMuteCount==4) {Layout=4; ConfigButtons(1); delay(keydelay); break; }                      // Count = 4 Text Strings
      if (VolMuteCount==5) {Layout=1; VolMuteCount = 0; ConfigButtons(1); delay(keydelay); break; }    // Count = 0 start again
      
      if (MuteDisable) break;  // No VolumeMute action [Cfg][Hme] sequences

      usb_hid.sendReport16(HIDCons, VolMute); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);       break;
      
    case 8: // M4 S4 T4 ///////////////////////////////////////////// Send a text or macro - could overide by sending serial 0text or macro
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);        delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);        delay(keydelay2); } break; }
      
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //                         <    0   CtrL 0xE0 + ShfL 0xE1 + Esc 0x29 >
      //                                     224         225        41
      // Macro 0x3C 0x30 0xE0 0xE1 0x29 0x3E <0 Control Shift Escape > with Layer 4 visible press [M4] will open the Task Manager
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (ByteOK)       { // <0777> shows 4 number HEX values 30 37 37 37 0     0777    RecBytes[1] 55 which is correct
                          // Must do this while pink layer is active
                          keycode[0] = RecBytes[1];     // 0xE0 224 RecBytes[0] = 7,8,9 ASCII
                          keycode[1] = RecBytes[2];     // 0xE1 225
                          keycode[2] = RecBytes[3];     // 0x29 41
                          keycode[3] = KeyEnter;
                          usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
                          usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); break;} 
          /*
          ByteOK = false;  //Serial.println(RecBytesLen); Serial.println(RecBytes[1]);
                                           //Serial.println(RecBytes[2]); Serial.println(RecBytes[3]); Serial.println(RecBytes[4]);
          for (n=1;  n<RecBytesLen; n++) { if (RecBytes[n]==224) {keycode[0] = CtrL;}     // 0xE0
                                      if (RecBytes[n]==225) {keycode[1] = ShfL;}     // 0xE1
                                      if (RecBytes[n]==41)  {keycode[2] = Esc; }  }  // 0x29
                                      keycode[3] = 0;
                                      usb_hid.keyboardReport(HIDKbrd, 0, keycode); 
                                      usb_hid.keyboardRelease(HIDKbrd); break; }
                                      
          */                            
      for (n=0;  n<strlen(inputString); n++) {usb_hid.keyboardPress(HIDKbrd, inputString[n]); delay(keydelay2);
                                              usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } break;                                                          
    case 9: // M5 S5 T5 //////////////////////////////////////////// Open normal command prompt via Run window
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);        delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);        delay(keydelay2); } break; }
                                                              
      keycode[0] = HID_KEY_GUI_LEFT; // Admin CMD/Powershell: GUI + R then cmd/powershell then instead of Enter use Ctrl+Shft+Enter then Alt+Y
      keycode[1] = HID_KEY_R;        // 
      usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
      
      for (n=0;  n<strlen(str9); n++)                        // str7 Normal Cmd Prompt str8 winver
          {usb_hid.keyboardPress(HIDKbrd, str9[n]);     delay(keydelay2);
           usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2); }
      delay(keydelay3);
      keycode[0] = CtrL;     // Instead of just [Enter] use [Control + Shift + Enter] => Run as admin
      keycode[1] = ShfL;     // 
      keycode[2] = KeyEnter; // 
      usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
      delay(keydelay3);
      keycode[0] = AltL;     // Alt + Y on UAC window instead of ArrowLeft+Enter
      keycode[1] = KeyYes;   // Y or y
      keycode[2] = KeyEnter; //
      usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
      break;
      
    case 10: // M6 S6 T6 //////////////////////////////////////// M6 text or Enter key LayerAB==1 b = 5; else b =0; 
      if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++)     // [button-4+b] = 10 - 4 + 5 = 11
                           {usb_hid.keyboardPress(HIDKbrd, Str1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);        delay(keydelay2); } break; }
      if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                           {usb_hid.keyboardPress(HIDKbrd, Ttr1to12[c][n]); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);        delay(keydelay2); } break; }
      
      // Send Alt (Hold) + Numpad numbers for Math symbols and special characters
      //////////////////////////////////////////////////////////////////////////////////
      // Working Alt + NumPad Special Key left arrow Symbol - NB Numlock must be ON
      //         Alt + 27 as keycode [0]=Alt-L [1]=KeyPad2 [2]=KeyPad4 [3]=0x00 works
      //         Alt + 8733 (infinity symbol) only does up to Alt + 873 (small d thingy)
      //////////////////////////////////////////////////////////////////////////////////
      // Can toggle Numlock on/off with HID_KEY_NUM_LOCK as keycode[0] (not keyPress)
      //////////////////////////////////////////////////////////////////////////////////
      //usb_hid.keyboardReport(KEYBOARD_MODIFIER_LEFTALT, 0, 0);  // Not working
      //keycode[0] = HID_KEY_ALT_LEFT;
      //keycode[1] = HID_KEY_KEYPAD_2;
      //keycode[2] = HID_KEY_KEYPAD_7;                    // Alt+Keypad 27 = <- symbol               
      //keycode[3] = 0x00;                                // Needed else adds a L/F
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                 break;
      
      /////////////////////////////////////////////////////////////////////////////////////////
      // 1D6D1 + Alt + X = small pi works in MS Word (1D6D1 = 120529)
      /////////////////////////////////////////////////////////////////////////////////////////
      for (n=0; n<6; n++) {usb_hid.keyboardPress(HIDKbrd, AltNum[n]); delay(keydelay2);
                           usb_hid.keyboardRelease(HIDKbrd);          delay(keydelay2);}
      keycode[0] = AltL;
      keycode[1] = 0x1B; // char X from hid.h
      keycode[2] = 0x00; 
      usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(HIDKbrd);                 
      break;
      ////////////////////////////////////////////////////////////////////////////////////////
      // This turns sticky codes on and off
      // If used for Alt + keypad numbers make sure the option turn off sticky when 
      // 2 keys are pressed at one is off and lock modifier keys when prssed twice in a row
      // is ON. Then turn on sticky keys, press Alt twice then in word press numlock on, then 
      // type 8733 on the keypad, press Alt twice and the open infinity symbol will show
      /////////////////////////////////////////////////////////////////////////////////////////
      //keycode[0] = HID_KEY_SHIFT_LEFT;
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay2);
      //keycode[0] = HID_KEY_SHIFT_LEFT;
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay2);
      //keycode[0] = HID_KEY_SHIFT_LEFT;
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay2);
      //keycode[0] = HID_KEY_SHIFT_LEFT;
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay2);      
      //keycode[0] = HID_KEY_SHIFT_LEFT;
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay2); 
      //usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
      //usb_hid.keyboardRelease(HIDKbrd);     break;
      
      // Open Task Manager ///////////////////////////////////////////////////////
      keycode[0] = CtrL;     // 0xE0 224
      keycode[1] = ShfL;     // 0xE1 225
      keycode[2] = Esc;      // 0x29 41
      usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); break; 
      // Enter Key ///////////////////////////////////////////////////////////////                                                        
      //usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
      //usb_hid.keyboardRelease(HIDKbrd);     break;
      
    case 11: ////////////////////////////////////////// Volume decrease
      if (VolDisable) { usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(HIDKbrd); break; }
    
      usb_hid.sendReport16(HIDCons, VolDown); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);       break;
  }   // switch
  }   // if Layout=1 or Layout=2

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // #define ArrUp    0x52  #define ArrDown  0x51  #define ArrLeft  0x50  #define ArrRight 0x4F
  // #define KeyHome  0x4A  #define KeyEnd   0x4D  #define PageUp   0x4B  #define PageDwn  0x4E
  ///////////////////////////////////////////////////////////////////////////////////////////////
  else //if (Layout==2)    
  switch(button){
    case 0: // HOME - Media Previous //////////////////////////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break; }
     
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              MuteDisable = !MuteDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (MuteDisable) status("Volume Mute OFF"); else status("Volume Mute ON");
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, PrevMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }

      if (NumKeys) { keycode[0] = BckS;     // Backspace
                     keycode[1] = 0x00;     // NULL
                     usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay2);
                     usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2); break;}

      keycode[0] = KeyHome;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 1: // ARROW UP ////////////////////////////////////////////////
      if (Macro1==1) {status("[M1] = Macro1"); for (n=0; n<Macro1Size; n++) {Mtr1[n] = MacroM1[n];} Mtr1[n] = 0; Macro1 = 2; break; }
      if (Macro2==1) {status("[M2] = Macro2"); for (n=0; n<Macro2Size; n++) {Mtr2[n] = MacroM2[n];} Mtr2[n] = 0; Macro2 = 2; break; }

      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) { MakeStr(button); break; }
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              DelBS = !DelBS; 
                              //GetSysInfo(1); // Save state on Flash
                              if (DelBS) status("Backspace ON"); else status("Delete ON");
                              ConfigButtons(1); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '7'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}
                        
      keycode[0] = ArrUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 2: // PAGE UP /////////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) { MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              VolDisable = !VolDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (VolDisable) status("Volume Keys OFF"); else status("Volume Keys ON");
                              ConfigButtons(1); break; }
                              
      if (Media) {usb_hid.sendReport16(HIDCons, NxtMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }

      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '8'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = PageUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 3: // V+ or Delete or Escape(KeyBrd+Math)=Execute(Keybrd) //////////////////////////////////////
      if (Math) {Math = false; status("Math exit");  MathX = 0; Math123 = 0; ConfigButtons(1); break;}
    
      if (Kbrd) {Kbrd = false;  status((char *)KBDisp);
                 if ((KeyBrdX==4) && (KeyBrdByteNum>0)) {SendBytes();}   // Execute macro + exit keybrd
                 if (KeyBrdX<4) {KeyBrdByteNum = 0; KBDispPos = 0;}      // Exit keybrd
                 KeyBrdX = 0; KeyBrd123 = 0; ConfigButtons(1); break;}
       
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '9'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      if (VolDisable) {if (DelBS) keycode[0] = BckS; else keycode[0] = DelK;  //Delete or BackSpace Key
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(HIDKbrd); break; }
        
      usb_hid.sendReport16(HIDCons, VolUp); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);     break;
      
    case 4: // ARROW LEFT //////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) { MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              if (LayerAB==0) {LayerAB=1; status("Layer B");} else {LayerAB=0; status("Layer A");} 
                              //GetSysInfo(1); // Save state on Flash
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, PlayMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);               break;}

      keycode[0] = ArrLeft;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 5: // CONFIG //////////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) { MakeStr(button); break;}
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '4'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      GetSysInfo(0); 
      if (ConfigKeyCount==1) {status("Config and Files Saved");           ConfigKeyCount = 0; }
                        else {status("L14 BsD Ret AB Save Med Del S12"); ConfigKeyCount++ ; } 
      ConfigButtons(1);       break;
      
    case 6: // ARROW RIGHT /////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) { MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              Media = !Media; if (Media) status("Media Keys ON"); else status("Media Keys OFF"); 
                              //GetSysInfo(1); // Save state on Flash
                              ConfigButtons(1); break; }
      
      if (Media) {usb_hid.sendReport16(HIDCons, StopMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }

      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '5'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = ArrRight;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 7: // Vol Mute ///////////////////////////////////////////////////////////////
      if (Math) {if (MathX<2) MathX++; else MathX=0; 
          Math123=0; ConfigButtons(1); break;} // Changes 3 Math KeyBrds from Greek abc -> ABC -> Calculus
    
    
      if (Kbrd) {if (KeyBrdX<4) KeyBrdX++; else KeyBrdX=0; 
          KeyBrd123=0; ConfigButtons(1); break;} // Changes 5 KeyBrds from abc - ABC - 012 - *^% - SHFT
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '6'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      if ((MuteDisable) && (VolMuteCount==0)) VolMuteCount++;
      VolMuteCount++;
      if (VolMuteCount==2) {Layout=2; ConfigButtons(1); delay(keydelay); break; }                      // Count = 2 Navigate
      if (VolMuteCount==3) {Layout=3; ConfigButtons(1); delay(keydelay); break; }                      // Count = 3 Text Strings
      if (VolMuteCount==4) {Layout=4; ConfigButtons(1); delay(keydelay); break; }                      // Count = 4 Text Strings
      if (VolMuteCount==5) {Layout=1; VolMuteCount = 0; ConfigButtons(1); delay(keydelay); break; }    // Count = 0 start again
      
      if (MuteDisable) break;  // No VolumeMute action [Cfg][Hme] sequences

      usb_hid.sendReport16(HIDCons, VolMute); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);       break;
      
    case 8: // END //////////////////////////////////////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) { MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {status("Files deleted"); ConfigKeyCount-- ; DeleteFiles(); ConfigButtons(1); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '0'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = KeyEnd;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 9: // ARROW DOWN //////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) { MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--; Kbrd = true; status("Escape Next Add End"); ConfigButtons(1); break;} 
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '1'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}
     
      keycode[0] = ArrDown;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 10: // PAGE DOWN //////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) { MakeStr(button); break;}
      
      if (ConfigKeyCount==1) {ConfigKeyCount--; SaveLayout = !SaveLayout; GetSysInfo(1); // Save the state
                              if (SaveLayout) status("Startup Layout L2"); else status("Startup Layout L1"); ConfigButtons(1); break;}

      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '2'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = PageDwn;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 11: // V- or Enter or Add /////////////////////////////////////////////////////////////////////////////////////////////////////
      if (Math) { SendMath(); status("Math symbol sent"); MathByteNum=0; break; }  
    
      if (Kbrd) {if (!KBrdActive) {status("Press char/mod first"); break;} // No char key or modifier keys has been pressed at least once
                 if (KeyRepeat) {if (KeyBrdX==4) break;
                                 if (KeyBrdX<4)  KeyBrdByte[KeyBrdByteNum] = KeyBrdByte[KeyBrdByteNum-1]; 
                                 MakeStr(PrevButton);}
                 if (KeyBrdX==4) {KBDispPos = KBDispPos + 2;  if (KeyBrdByte[0]>0x7F) KBType = true; } // can be 2nd or 3rd?
                 if (Fx) {KBDispPos = KBDispPos + 2; }
                 KBDispPos++; KeyBrdByteNum++; break; }  
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '3'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      if (VolDisable) { usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(HIDKbrd); break; }

      usb_hid.sendReport16(HIDCons, VolDown); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);       break;
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
// Compute the squared distance (d2) from the button center to the current touch coordinates.
// d2 = (bx - tx)*(bx - tx) + (by - ty)*(by - ty);
// If d2 is less than the button radius squared, then that button was selected.
// You will find this function, which does not do any multiplies, in the Adafruit graphics library:
// boolean Adafruit_GFX_Button::contains(int16_t x, int16_t y) 
//          {return ((x >= _x1) && (x < (_x1 + _w)) &&  (y >= _y1) && (y < (_y1 + _h))); }}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigButtons(uint8_t rowcount) {  // rowcount=0 all 4 rows rowcount=2 last row only  rowcount=1 no background redraw 
  int n, m, b;
  uint8_t rows = 3;    // 3 rows of labels
  uint8_t row, col;    // keys
  uint16_t PadB, PadC; // 3 Pads Red Blue Green
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
  //   "L14", "BsD",  "Vol", "   ",     "A-B",  "Sav", "Med",  "   ",     "Del", "Kbd",  "S12", "   " 
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (ConfigKeyCount>0) 
    {for (m=0; m<3; m++) {keyLabel[0][m] = CfgLabel[0][m]; keyLabel[1][m] = CfgLabel[1][m]; keyLabel[2][m]  =  CfgLabel[2][m];  
                          keyLabel[4][m] = CfgLabel[4][m]; keyLabel[5][m] = CfgLabel[5][m]; keyLabel[6][m]  =  CfgLabel[6][m]; 
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

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Math) { for (m=0; m<3; m++) 
                 {keyLabel[0][m]  =  MathLabel[MathX][0][m];   keyLabel[1][m]  =  MathLabel[MathX][1][m]; 
                  keyLabel[2][m]  =  MathLabel[MathX][2][m];   keyLabel[3][m]  =  MathLabel[MathX][3][m]; 
                  keyLabel[4][m]  =  MathLabel[MathX][4][m];   keyLabel[5][m]  =  MathLabel[MathX][5][m]; 
                  keyLabel[6][m]  =  MathLabel[MathX][6][m];   keyLabel[7][m]  =  MathLabel[MathX][7][m];
                  keyLabel[8][m]  =  MathLabel[MathX][8][m];   keyLabel[9][m]  =  MathLabel[MathX][9][m]; 
                  keyLabel[10][m] = MathLabel[MathX][10][m];   keyLabel[11][m] = MathLabel[MathX][11][m];}
                  keyColor[3]=keyColor[7]=keyColor[11]=MathColour2;
                  keyColor[0]=keyColor[1]=keyColor[2]=keyColor[4]=MathColour1;
                  keyColor[6]=keyColor[5]=keyColor[8]=keyColor[9]=keyColor[10]=MathColour1;}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Kbrd) { for (m=0; m<3; m++) 
                 {keyLabel[0][m]  =  KbrdLabel[KeyBrdX][0][m];   keyLabel[1][m]  =  KbrdLabel[KeyBrdX][1][m]; 
                  keyLabel[2][m]  =  KbrdLabel[KeyBrdX][2][m];   keyLabel[3][m]  =  KbrdLabel[KeyBrdX][3][m]; 
                  keyLabel[4][m]  =  KbrdLabel[KeyBrdX][4][m];   keyLabel[5][m]  =  KbrdLabel[KeyBrdX][5][m]; 
                  keyLabel[6][m]  =  KbrdLabel[KeyBrdX][6][m];   keyLabel[7][m]  =  KbrdLabel[KeyBrdX][7][m];
                  keyLabel[8][m]  =  KbrdLabel[KeyBrdX][8][m];   keyLabel[9][m]  =  KbrdLabel[KeyBrdX][9][m]; 
                  keyLabel[10][m] = KbrdLabel[KeyBrdX][10][m];   keyLabel[11][m] = KbrdLabel[KeyBrdX][11][m];}
                  keyColor[0]=keyColor[1]=keyColor[2]=keyColor[3]=keyColor[4]=keyColor[5]=CfgColour;
                  keyColor[6]=keyColor[7]=keyColor[8]=keyColor[9]=keyColor[10]=keyColor[11]=CfgColour;}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (MouseKeys) { for (m=0; m<3; m++) 
                 {keyLabel[0][m] = Mousekeys1[0][m];   keyLabel[1][m] = Mousekeys1[1][m]; 
                  keyLabel[2][m] = Mousekeys1[2][m];   keyLabel[3][m] = Mousekeys1[3][m]; 
                  keyLabel[4][m] = Mousekeys1[4][m];   keyLabel[5][m] = Mousekeys1[5][m]; 
                  keyLabel[6][m] = Mousekeys1[6][m];   keyLabel[7][m] = Mousekeys1[7][m];
                  keyLabel[8][m] = Mousekeys1[8][m];   keyLabel[9][m] = Mousekeys1[9][m]; 
                  keyLabel[10][m]= Mousekeys1[10][m];  keyLabel[11][m] =Mousekeys1[11][m];}
                  keyColor[0]=keyColor[1]=keyColor[2]=keyColor[3]=keyColor[4]=keyColor[5]=Pink;
                  keyColor[6]=keyColor[7]=keyColor[8]=keyColor[9]=keyColor[10]=keyColor[11]=Pink;}
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (NumKeys)   { for (m=0; m<3; m++) 
                 {keyLabel[0][m] = Numkeys1[0][m];    keyLabel[1][m] = Numkeys1[1][m]; 
                  keyLabel[2][m] = Numkeys1[2][m];    keyLabel[3][m] = Numkeys1[3][m]; 
                  keyLabel[4][m] = Numkeys1[4][m];    keyLabel[5][m] = Numkeys1[5][m]; 
                  keyLabel[6][m] = Numkeys1[6][m];    keyLabel[7][m] = Numkeys1[7][m];
                  keyLabel[8][m] = Numkeys1[8][m];    keyLabel[9][m] = Numkeys1[9][m]; 
                  keyLabel[10][m]= Numkeys1[10][m];   keyLabel[11][m] =Numkeys1[11][m];}
                  keyColor[7]=keyColor[6] =           keyColor[1]=keyColor[2] =DGrey;
                  keyColor[8]=keyColor[3] =           keyColor[5]=keyColor[11]=DGrey;
                  keyColor[0]=Blue;keyColor[4]=Red;   keyColor[9]=keyColor[10]=DGrey;}

            
  b = Layout - 1;                               // reset b to 0 - 3
              
  for (row = 0 + rowcount; row < rows; row++) {   // 3 rows 0,1,2 => b = 0+(0*4)=0 1+(0*4)=1 ... 0+(1*4)=4 ....
    for (col = 0; col < 4; col++) {               // 4 columns 0,1,2,3
        b = col + row * 4;
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X), KEY_Y + row * (KEY_H + KEY_SPACING_Y), 
                                               KEY_W, KEY_H, OutlineColor, keyColor[b], White, keyLabel[b], KEY_TEXTSIZE);
        key[b].drawButton();  }  }

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // 5 Small Config Buttons between 1 st and 3rd row Red Blue Green SkyBlue Gold
   // char padLabel[5][4] = {"", "", "", "", ""}; 
   //                      0     1      2    3     4       5        6     7      8       9     10     11     12    13     
   // padColor[15]   = {Black, Black, Black, Red, Blue, Green4, SkyBlue, Gold, Black, Black, Black, Black, Black, Black, more Black ->};
   // padOutline[15] = {Black, Black, Black, White, White, White, White, White, Black, Black, Black, Black, Black, Black, more Black ->};

   if ((Layout==2)||(Layout==3)) {  // Only need the below if Layout = 2 (do config pads) or Layout = 3 (clear pad buttons)
   b = Layout - 1;                  // reset b to 0 - 3 
   //                                               0+1*3=3 0+2*3=6
   //                                               1+1*3=4 1+2*3=7
   //                                               2     5
   //                                               3     6
   //                                               4+1*3=7 4+2*3=10
   for (row = 0; row < 5; row++) {PadB = padOutline[row+(b*3)]; PadC = padColor[row+(b*3)]; // row = 0,1,2 + (0 1 2 3 = Layout-1) * 3
        //                          x-middle    y-middle                 width height outline fill  text
        //                            308       36 + 0,1,2,3,4 * 38 = 0,74,112,150,188   10    20             
        key[row+12].initButton(&tft, PAD_X,     PAD_Y + row    * PAD_SPACING_Y,         PAD_W, PAD_H, PadB, PadC, PadB, padLabel[row], KEY_TEXTSIZE);
        key[row+12].drawButton();  } } // if ((Layout==2)||(Layout==3))

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
  byte b, *BPtr;
  int ASize, n = 0;
  uint16_t ByteLen = 0;
  
  for (n=0; n<12; n++) 
      {NameStr = STRname[n]; ChrPtr = Str1to12[n];  // n=0 STRname[0] Str1to12[0] str1to12[0]
       if (!LittleFS.exists(NameStr)) strcpy(ChrPtr, str1to12[n]); else DoFileStrings(StrOK, NameStr, ChrPtr);  }
  
  for (n=0; n<12; n++) 
      {NameStr = TTRname[n]; ChrPtr = Ttr1to12[n];
       if (!LittleFS.exists(NameStr)) strcpy(ChrPtr, str1to12[n]); else DoFileStrings(StrOK, NameStr, ChrPtr); }

  for (n=0; n<12; n++) 
      {NameStr = MTRname[n]; BPtr = Mtr1to12[n]; ASize = ArraySz(Mtr1to12[n]);            // max size
       if (LittleFS.exists(NameStr)) DoFileBytes(ByteOK, NameStr, BPtr, ASize); }

  NameStr = "KeyBrdMacro1"; BPtr = Mtr1; ASize = ArraySz(Mtr1); Macro1 = 0;                                   // max size
       if (LittleFS.exists(NameStr)) { Macro1Size = DoFileBytes(ByteOK, NameStr, BPtr, ASize); Macro1 = 2; }  // actual size
     
  NameStr = "KeyBrdMacro2"; BPtr = Mtr2; ASize = ArraySz(Mtr2); Macro2 = 0; 
      if (LittleFS.exists(NameStr)) { Macro2Size = DoFileBytes(ByteOK, NameStr, BPtr, ASize); Macro2 = 2;  } 

  if (LittleFS.exists("inputStr")) {ChrPtr = inputString; DoFileStrings(StrOK, "inputStr", ChrPtr);}

}

//////////////////////////////////////////////////////////////////////////////////
uint16_t DoFileBytes(bool DoWrite, String STRf,  byte *BytePtr, uint16_t ByteArrayLen)
{ uint16_t ByteLen = 0;
  int n;
  byte b;
      
  if (!DoWrite)                            // read data
     BytePtr[0] = 0x00;                    // For test in buttons() M1 - M3
     {File f = LittleFS.open(STRf, "r");
      n = 0;
      while (f.available())
            {b = f.read();
             BytePtr[n++] = b; }
      //if (n >= ByteArrayLen) break; }
      ByteLen = f.size();
      f.close(); }
     
  if (DoWrite)                             // write data
     {File f = LittleFS.open(STRf, "w");
      f.write(&BytePtr[0], ByteArrayLen);  // write the whole array to the file
      f.print('\0');                       // f.println();
      f.close();   }

  return ByteLen;
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
      StrLen = strlen(ChrPtr);               //Sent string <5This is S5> then STRf=StrData5 StrLen=10 which is correct
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
////////////////////////////////////////////////////////////
void DeleteFiles()  // delete text/number macros and config
////////////////////////////////////////////////////////////
{ if (LittleFS.format()); 
     {LittleFS.begin();
      LittleFS.remove("/MuteDisable");      // Must be changed via Cfg - self-restored
      LittleFS.remove("/VolDisable");       // Must be changed via Cfg - self-restored
      InitStr();
      Serial.println("Files deleted");}
}
////////////////////////////////////////////////
void GetSysInfo(int Action) // Also save config
//////////////////////////////////////////////// 
{ char *ChrPtr;
  uint32_t  Bt;
  int fHeap = rp2040.getFreeHeap();
  int uHeap = rp2040.getUsedHeap();
  int tHeap = rp2040.getTotalHeap();
  int fCPU  = rp2040.f_cpu();
  
  if (Action==1) {if (SaveLayout) { char LayoutF[2]  = "1"; ChrPtr = LayoutF; DoFileStrings(1, "Layout",  ChrPtr); }
                  else LittleFS.remove("Layout"); return;}
           
  if (VolDisable)  { char VolD[2]   = "1"; ChrPtr = VolD;  DoFileStrings(true, "VolDisable",   ChrPtr); }
          else LittleFS.remove("VolDisable");
      
  if (!MuteDisable) { char MuteD[2]  = "1"; ChrPtr = MuteD; DoFileStrings(true, "MuteDisable",  ChrPtr); }
         else LittleFS.remove("MuteDisable"); 

  if (LayerAB==1) { char LayerD[2]  = "1"; ChrPtr = LayerD; DoFileStrings(true, "LayerB",  ChrPtr); }
         else LittleFS.remove("LayerB"); 

  if (Media) { char MediaD[2]  = "1"; ChrPtr = MediaD; DoFileStrings(true, "MediaKeys",  ChrPtr); }
         else LittleFS.remove("MediaKeys");

  if (DelBS) { char DelBSD[2]  = "1"; ChrPtr = DelBSD; DoFileStrings(true, "DeleteBackspace",  ChrPtr); }
         else LittleFS.remove("DeleteBackspace");
    
  Serial.println("VolumeMacroKeys version 176.0");
  Serial.printf("CPU MHz: %d\n\r", fCPU);
  Serial.printf("FreeHeap: %d\n\r", fHeap);
  Serial.printf("UsedHeap: %d\n\r", uHeap);
  Serial.printf("TotalHeap: %d\n\r", tHeap);
  Serial.printf("Core temperature: %2.1fC\n\r", analogReadTemp());
  Serial.printf("LCD Blank (seconds): %d\n\r", TimePeriod/1000);

  ListFiles();
  Bt = 0;
  int n = 10000; // Below needs some work
  if (LittleFS.exists("LCDBlankTime")) {File f = LittleFS.open("LCDBlankTime", "r");  // Stored as 6 ASCII number bytes such as 49 53 48 48 48 48 = 150000 = 150 sec
                      while (f.available()) {byte b = f.read(); if (n>10) {Bt = Bt + (b-48)*n;} n = n/10; } Serial.println(Bt); int ByteLen = f.size(); f.close();}
  
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
void SendBytesEnd()
{ int n;
  KeyBrdByteNum = 0; KBDispPos = 0;  KeyBrdX = 0; KeyBrd123 = 0;
  for (n = 0; n < 200; n++) { KeyBrdByte[n] = 0x00; KBDisp[n] = '\0'; }
  Kbrd = false; KBType = false; Fx = false; KBrdActive = false; }
////////////////
void SendBytes()
{ uint8_t keycode[6] = { 0 }; // simultaneous keys pressed in here
  uint8_t a, b, n, i;
  unsigned long t, z;
  //                 012345678901234567890123456
  char BlankArr[] = "Blank T             seconds"; 
    
  //////////////////////////////////////////////////////////////////////////// Check for special commands Start with *
  if ((KeyBrdByte[0]==0x2A)&&(KeyBrdByte[1]==0x6B)&&(KeyBrdByte[2]==0x62))  // *kb* = num keyboard
     {NumKeys = true; ConfigButtons(1); SendBytesEnd(); status(" "); return; } 
      
  if ((KeyBrdByte[0]==0x2A)&&(KeyBrdByte[1]==0x6D)&&(KeyBrdByte[2]==0x6B))  // *mk* = Mouse keys
     {MouseKeys = true; ConfigButtons(1); SendBytesEnd(); status(" "); return; } 

  if ((KeyBrdByte[0]==0x2A)&&(KeyBrdByte[1]==0x74)&&(KeyBrdByte[2]==0x62))    // *tb*num = blank lcd timemout *tb*0,1-9 = infinite,30,60,90,120,240 sec
      {t = TimePeriod; TimePeriod=(KeyBrdByte[4]-48)*30000;                   // 1=30 sec 5=150 sec 6=3 min 9=270 sec
       if (TimePeriod>6000000) TimePeriod = t;                                // 100 minutes max
       if (TimePeriod==0) TimePeriod = 600000000; // 120 days                 // 0 = 7 days
       z = TimePeriod/1000; 
       i = 6;                                                                 // 6 digits
       while (i!=0) { BlankArr[i+12] = (z%10) + 48; z = z/10; i--; }          // modulus (%) and division (/) operations 
       File f = LittleFS.open("LCDBlankTime", "w");
       if (f) {f.print(TimePeriod); f.close(); }                              // Stored as 6 ASCII number bytes such as 49 53 48 48 48 48 = 150000 = 150 sec
       Serial.println(TimePeriod);
       SendBytesEnd(); status(BlankArr); return;}  
    
  // sequence via keyPress 
  if ((!Fx)&&(KeyBrdByte[0]<0x80))                                                  // F1-F12 must be sent with keycode not keyPresssequence via keyPress 
     {for (n = 0; n < KeyBrdByteNum; n++) if (KeyBrdByte[n]<0x80)                   // When 1st char is normal alphanumeric all must be <0x80 
          {usb_hid.keyboardPress(RID_KEYBOARD, KeyBrdByte[n]); delay(keydelay);     // Converted via end of hid.h from ascii to hid
           usb_hid.keyboardRelease(RID_KEYBOARD);              delay(keydelay); 
           MacroM1[n] = KeyBrdByte[n];}
      File f = LittleFS.open("KeyBrdMacro1", "w");
      if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }         
      status("Macro1 Press [Up] save [M1]"); Macro1 = 1; Macro1Size = KeyBrdByteNum;}

  // simultaneous via keycode        
  if ((Fx)||(KeyBrdByte[0]>0x7F)&&(KeyBrdByteNum<6))
     {for (n = 0; n < KeyBrdByteNum; n++) {keycode[n] = KeyBrdByte[n]; MacroM2[n] = KeyBrdByte[n]; }  
          usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay);
          usb_hid.keyboardRelease(RID_KEYBOARD);            delay(keydelay);  
      File f = LittleFS.open("KeyBrdMacro2", "w");
      if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }  
      status("Macro2 Press [Up] save [M2]"); Macro2 = 1; Macro2Size = KeyBrdByteNum;}
      /*
      for (n = 0; n < KeyBrdByteNum; n++) 
          {Serial.print(KeyBrdByte[n], HEX);
           Serial.print(' '); }
      Serial.println(" ");
      for (n = 0; n < 6; n++)   
          {Serial.print(keycode[n], HEX);
           Serial.print(' '); }
      */ 
SendBytesEnd(); //clean-up

}

////////////////////////
void MakeStr(int button)
////////////////////////
{   int i, n;
    byte a, b, c;
    a = 0;
    Fx = false;         // Only keep it true for one MakeStr type keypress at a time see ***Fx***
    b = KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123];
    
    KBrdActive = true; // Let ADD know any char key has been pressed at least once before
    //Serial.print("Prev "); Serial.print(PrevButton);
    //Serial.print(" Curr ");Serial.println(button);

    if (KBType) {                                                                                // Needed if Mod key pressed first
       if (KeyBrdX==0) if ((b>0x60) && (b<0x7B)) {a = b - 93;}                                   // Xlate a-x
       if (KeyBrdX==1) if ((b>0x40) && (b<0x5B)) {a = b - 61;}                                   // Xlate A-X but leave DelK = 0x4C
       if (KeyBrdX==2) if ((b>0x2F) && (b<0x3A)) {if (b==0x30) a = b - 9; else a = b - 19; } }   // Xlate 0-9 to 1-0
       
       if ((KeyBrdX==3) && ((button==6)||(button==8)||(button==9)||(button==10))) {a = KeyBrdFx[KeyBrd123][button-6]; Fx = true;}   // Xlate F1 - F12
    
    
    for (int n = 0; n <= KeyBrdByteNum; n++) 
        {Serial.print(KeyBrdByte[n], HEX);
         Serial.print(' '); }
         
    Serial.println();
           
    if (KeyBrdX==4) {KeyBrdByte[KeyBrdByteNum] = KeyBrdNum[KeyBrd123][button]; 
        for (n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } Serial.println(); }
    
       
    n = 0;
    if (KeyBrdX<4) 
       {KBDisp[KBDispPos] = b; KBDisp[KBDispPos+1] = ' '; KBDisp[KBDispPos+2] = ' '; 
        if (a>0) KeyBrdByte[KeyBrdByteNum] = a; 
        if (Fx) { for (n=0; n<3; n++) {KBDisp[KBDispPos+n] = KeyBrdFxChr[KeyBrd123][button-6][n]; } } }    // Fx true for one MakeStr() ***Fx***
        
    if (KeyBrdX==4) for (n=0; n<3; n++) KBDisp[KBDispPos+n] = KeyBrdSpc[KeyBrd123][button][n];

    
    for (i = 0; i <= KBDispPos+n; i++) 
        {Serial.print(KBDisp[i]);
         Serial.print(' '); }
    Serial.println();
    

    status((char *)KBDisp);

    if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; 
}


/////////////////////////////
void MakeMathStr(int button)
/////////////////////////////
{   int i, n;
     
    for (n=0; n<=20; n++) {MathChr[n]    = MathName[MathX][Math123][button][n]; if (MathChr[n]==0) break; }   // [KeyBrd 0,1=MathX][Symbol 1,1,2 = Math123][button 1-11]
    for (n=0; n<5;   n++) {MathHexNum[n] =  MathNum[MathX][Math123][button][n]; }                             // [KeyBrd 0,1=MathX][Symbol 1,1,2 = Math123][button 1-11] 
    
    //for (n=0; n<5; n++) {Serial.print(MathHexNum[n]); Serial.print('-'); }    
    status((char *)MathChr);

    if (Math123<2) Math123++; else Math123=0; 
}

////////////////////////
void SendMath()
////////////////////////
{   int i, n;
    uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here
    /////////////////////////////////////////////////////////////////////////////////////////
    // 1D6D1 + Alt + X = small pi works in MS Word (1D6D1 = 120529)
    /////////////////////////////////////////////////////////////////////////////////////////
    for (n=0; n<5; n++) { usb_hid.keyboardPress(HIDKbrd, MathHexNum[n]); delay(keydelay2);
                          usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2);}
      keycode[0] = AltL;
      keycode[1] = 0x1B; // char X from hid.h
      keycode[2] = 0x00; 
      usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      usb_hid.keyboardRelease(HIDKbrd);
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
