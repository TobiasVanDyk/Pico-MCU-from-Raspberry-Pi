///////////////////////////////////////////////////////////////////////////////////////////
// Combination of (1) Raspberry Pi Pico Touch Macro Keyboard by Dustin Watts
//                    https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard
//                (2) AdafruitTinyUSB HID examples hid_composite.ino hid_boot_keyboard.ino
//                    https://github.com/adafruit/Adafruit_TinyUSB_Arduino/
//                (3) Bodmer-Adafruit Keypad Keypad_480x320.ino
//                    https://github.com/Bodmer/TFT_eSPI/examples/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adapted by T van Dyk Oct 2022 for ILI9486/ILI9488 480x320 and Adafruit TinyUSB stack
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// just as there is never the wrong time to do the right thing there is never the right time to do the wrong thing
/************************************************************************************
 Adafruit invests time and resources providing this open source code, please support 
 Adafruit and open-source hardware by purchasing products from Adafruit
 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
*************************************************************************************/

/************************************************************************************************************
Layout 1  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
------------------------------------------------------------------------------------------------
[Cut Ctrl+X] [  Copy Ctrl+C   ] [Paste Ctrl+V] [VolUp  Del-Bs]    [cX]  [cC]  [cV]  [V+][Del-Bs]
[Alt+PrtScr] [Admin Powershell] [ Run window ] [VolMute L1-L4]    [M1]  [M2]  [M3]  [Vo][L1-L4 ]
[Send Text ] [Admin CMD Prompt] [Task Manager] [VolDwn  Enter]    [M4]  [M5]  [M6]  [V-][Enter ]
          Caps                Num           Scroll                    C     N     S

Layout 2  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
--------------------------------------------------------------------------------------------------
[Home Stop    Mute] [UpArr BsDel] [PgeUp     DelRet] [VolUp  Delete] [Hme] [Up ] [Pgu] [V+][DelBs]
[ArrL Prev     A-B] [Config Save] [ArrR  Nxt  Media] [VolMute L1-L4] [  <] [Cfg] [>  ] [Vo][L1-L4]
[End  PlayP CfgDel] [DwnArr POff] [PgeDwn StartL1L2] [VolMute Enter] [End] [Dwn] [PgD] [V-][Enter]
                 Caps           Num               Scroll                  C     N     S

Layout 2  (Config Layout) has five additional small pad-buttons on the far right side:
Red     Press the small Red Pad on the Right to toggle the Math KeyPad on/off.
Blue    Press the small Blue Pad on the Right to toggle the Keyboard on/off.
Green   Press the small Green Pad on the Right to toggle the Media Keypad on/off.
SkyBlue Press the small SkyBlue Pad on the Right to toggle the Number Keypad on/off.
Yellow  Options Pad. KeyBrd Mode Select [M] key current macro save  - indicator blue "1" to "12".
                                 Direct Mode On/Off (Blue "D" indicator).
                     NumPad mode switch between 3 NumPad pages. 

Layout 3+4  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
--------------------------------------------------------------------------------------------------
[Cut Ctrl+X ] [Copy Ctrl+C] [Paste Ctrl+V] [VolUp  Del-Bs]   [ cX ]  [ cC  ]  [ cV  ] [V+][Del-Bs]
[Send Text 1] [Send Text 2] [Send Text 3 ] [VolMute L1-L4]   [S1 T1] [S2 T2]  [S3 T3] [Vo][L1-L4 ]
[Send Text 4] [Send Text 5] [Send Text 6 ] [VolMute Enter]   [S4 T4] [S5 T5]  [S6 T6] [V-][Enter ]
           Caps           Num           Scroll                      C       N        S

-------------------------------------------------------------------------------------------------
Layouts 1, 3 and 4, two Layers A/B each with A=M1-M6, S1-S6, T1-T6 and B=M7-M12, S7-S12, T7-T12
Press VolumeMute consecutively within 5 seconds else count is reset to 0 or press [L1-L4] once.
[L1-L4] has a repeat period of 0.5 seconds - after this key repeat is active

Press Config Key (new set of config keys change colour):
L14 (Home) - VolumeMute -> L1, L2, L3, L4 - repeat to restore Vo
BsD (ArrowUp) - Toggle Vol+ key to either Delete or Backspace
Vol (PageUp) - Vol+ -> Delete and Vol- -> Enter - repeat to restore V+ V-
A-B (ArrowLeft) - Layouts 1, 3, 4, change to Layer A or Layer B 
Med (ArrowRight) - Change Layout 2 to Media Controls Previous-Next-PlayPause-Stop
Del (End) - Delete all files on Flash (Strings and Config) - will re-calibrate on restart
S12 (PageDown) - Start with Layout 1 or Layout 2 on powerup - must also press Sav(e) (Cfg)
Sav (Cfg) Info and File List to send Serial Monitor and Text/Macro and Config files saved
ROf (ArrowDwn] - Restart-PowerOff-Logoff - Bottom row [Rst][Log][Off] - cancel by pressing 
    [Cfg][ROf] - There are long and short Timer options as well

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

Macro Composition Keyboard:

Keyboard Page 1              Page 2          Page 3          Page 4              Page 5
[abc] [def] [ghi] [EXE]  [ABC] to [XY_]  [012] to [9+-]  [1F3] to [1F2]  [ALT] [SHF] [CTR] [EXE]
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
       
The keyboard is has 5 pages each with 9 triple function keys (that is 135 different keys), and 3 control keys 
[EXE] [NXT] [ADD]. For example page 1 has keys [abc], [def], to [y,z,space]. To select a or b or c press the
abc key once, twice or thrice - to add it to a new macro press the ADD key else press another character-symbol
modifier key or press [NXT] for the next keyboard page. Press the Pad 2 again to leave the keyboard or press
[EXE] to send the macro to the PC and save to the current [M]key as indicated by the blue number 1 to 12.
Change the target [M]key by pressing the last (yellow) Pad. The macro is saved when the Up-Arrow key is pressed
after the [EXE] key and after the next power-on will stay associated with that [M]key.

The macros on page 5 are modifiers (simultaneously pressed keys) such as Control + Alt + Delete + GUI (maximum 
of 6 keys in macro). To send this sequence press [CTR][ADD][ALT][ADD][SHF]x3 (3 times for delete)[ADD][EXE]. 

To open the windows snipping tool press [GUI][ADD][SHF][ADD][NXT][s][ADD][EXE] - the snipping tool will open. 
This can then be saved to [M2] by pressing the [Up] button. 

To send a sequence such as [WinKey] + "r" press [GUI][ADD][NXT][pqr]x3[ADD][EXE]. To assign Alt + PrintScr to
key [M2] press [NXT]4x[ALT][ADD][ALT]3x[ADD][EXE} and [Up] to save. Alt + PrtScr is then executed when pressing
[M2]. 

Another example: [GUI][ADD][NXT]1x[r][EXE][Up] then [n][ADD][o][ADD][t][ADD][e][ADD][p][ADD][a][ADD][d][ADD]
[NXT]3x[CRF][ADD][NXT][h][ADD][e][ADD][l][ADD][l][ADD][e][ADD][EXE][Up]. After this press just two keys 
[M2][M1] to open notepad and insert the text "hello". To make the text larger [CTR][ADD][NXT]3x[9+-]2x to make
the text larger.

Toggle the sticky keys: Press [SHF][ADD] 5 times then press [EXE] and save to key [M2]. (For a complete 
macro add [C/R] [ADD] key at the end of this sequence.) 

The option to use the combined modifier bit instead of a modifier byte, is used in the top row Cut-Copy-Paste 
keys, and also for some of the pre-programmed examples for the M1-M12 keys. 

Macros sent to the PC from the built-in keyboard will be saved to file KeyBrdMacroX X=1-12, and if selected, 
assigned to key [MX]. Press [Up] after sending the macros to the PC to assign them to [MX]key. Chaining 
macros are planned.

F1-F12 keys are sent as keycodes (simultaneous) and not keypress types - to send [F3] open the macro keyboard then
press [NXT]3x[1F3]3x[ADD][EXE]. Press [Up] to assign it to key [M2]. Once assigned it will survive a reboot.

The Keyboard has a Direct (to PC) Mode - use the Yellow Options Pad on the bottom right, to switch Direct Mode On/Off.
A Blue "D" indicator will show if its is on. Any character selected (shows in status bar), will be sent to the PC by 
pressing [EXE] - [ADD] is not necessary. If a character or more than one characters have been [ADD]ed they will only 
be sent after Direct Mode is switched off.

The Keyboard can be used to change various options by sending *option*value command such as:
* 1 LCD blank timeout - Send the macro *tb*num with the built-in keyboard where num = 0,1-9 - *tb*0 = 120 hours, 
*tb*1 = 30 seconds.  
* 2 LCD blank dim value - Send the macro *db*num with the built-in keyboard where num = 0,1-9 - *db*0 = no
dimmming - just on/off after blank timeout, and 1-9 = 3 to 30 % PWM instead of blank. 
* 3 LCD screen brightness - Send the macro *bb*num with the built-in keyboard where num = 0,1-9 - *bb*0 = full
brightness and 1-9 = 3 to 50 % PWM brightness.
* 4 Power Keys Menu/Command Option - Send the  macro *po* with the built-in keyboard to toggle the Power Keys Option
to use the Menu (GUI+X + U + i,u,r or the run command GUI+R + shutdown + options command.
* 5 Power Keys time values - Default Short Time = 30 seconds and Default Long Time 600 seconds (10 minutes). To change 
Time values send PowerOff *ot*num or *oT*num or Restart *rt*num or *rT*num where num 0 = 6000 second (100 minutes)
1 = 30 second (x100 for T) to 9 = 300 second (x100 for T).
* 6 Calibration Enable/Disable - Send the macro *ca* with the built-in keyboard to set/clear re-calibration option after a 
restart to ON or OFF. 
* 7 LCD Orientation 180 degree change - Send the macro *ro* with the built-in keyboard - the USB cable will then 
be on the opposite side. This forces a re-calibration on restart. See the panic mode reset at the end of this description.
* 8 Button Font Bold/Normal change - Send the macro *fo* with the built-in keyboard to turn the labels on the buttons from
normal to bold. 

Math-Greek-Algebra Keyboard:
This is a triple-key macro keyboard with 4 pages and 4 x 9 x 3 = 108 math and Greek algebra
symbols. It is sent to the PC as (hex) Unicode + [ALT] + [x] and can be directly entered into MSWord and in LibreOffice
(also on Linux). The three control keys are [EXE] - send the symbol to the PC, [NXT] - next page of symbols, and [ESC]
go back to the main Config layout - or press the small red pad again to leave the Math keyboard. Watch out for MSWord 
insists on capitilizing the first letter of a sentence - which will change the first math symbol in a sentence/line 
after a second symbol or letter or enter is typed. 

The [M6] key section in the code has a few examples of using Alt + Number Keypad for Maths and special symbols or 
characters. There are 2 ways to do it - the first is to type the character's Unicode (hex without the 0x or decimal) into 
MSWord and then press [ALT] + [x]. The second method is to hold the Alt key down, then type in a special code number using
the number keypad and then release the Alt key. There are examples of entering the open infinity symbol and the small pi 
synbol in the [M6] key section. 

Numeric Keypad    [ BackSpc] [ 7 Spc aA ] [ 8 % bB ] [ 9 xX cC ]  Press 4th Pad - toggle the Number Keypad on/off.
                  [ Return ] [ 4  ,  dD ] [ 5 . eE ] [ 6 =  fF ]  Press 5th Pad - switch Num Pages 1-3 - CapsLock a-A       
                  [ 0  + ( ] [ 1  -   ) ] [ 2 *  [ ] [ 3 /   ] ]  Macro *kb from the built-in Keyboard show keybrd

Power Restart KeyPad    [Restart  Long Timer] [ Stop ] [PowerOff  Long Timer]        [R-T] [Stp] [O-T] [ ]   
                        [Restart Short Timer] [  Cfg ] [PowerOff Short Timer]        [R-t] [Cfg] [O-t] [ ]       
                        [Restart   Immediate] [Logoff] [PowerOff    Imediate]        [Rst] [Log] [Off] [ ]
 
To cancel a timed shutdown press the [Stop] key. To exit the Power Restart Keypad press any of the [black] non-
functional keys or press [Cfg][ROf] again.

Panic mode reset. If for any reason your keypad becomes unresponsive or behaves strangely (*) reset it as follows:

(1) If the Config button [Cfg] still works press it once and then press the [Del]ete key at the left-hand bottom. Then 
    unplug or reset the keypad. This will delete all files. macros and settings and you should have a re-calibration at
    start-up.
(2) Press either the white button at the bottom and hold it in, then press the black reset button at the back and release
    it (or unplug and re-plug the USB cable instead of the rest button), then only release the white button at the bottom.
    The file manager should show a new storage device named RPI-RP2. Drag and drop any of the code.UF2 files to this 
    device. It will restart after a second or two. If this still does not reset the keypad then instead of the code.UF2 
    file drag and drop the file flash_nuke.uf2, wait a few seconds and then drag the code.UF2 file to the device.

(*) Writing Greek letters is not behaving strangely.             
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

#define KBM1f  "KeyBrdMacro1"
#define KBM2f  "KeyBrdMacro2"
#define KBM3f  "KeyBrdMacro3"
#define KBM4f  "KeyBrdMacro4"
#define KBM5f  "KeyBrdMacro5"
#define KBM6f  "KeyBrdMacro6"
#define KBM7f  "KeyBrdMacro7"
#define KBM8f  "KeyBrdMacro8"
#define KBM9f  "KeyBrdMacro9"
#define KBM10f "KeyBrdMacro10"
#define KBM11f "KeyBrdMacro11"
#define KBM12f "KeyBrdMacro12"

char STRname [12][11] = { STR1f, STR2f, STR3f, STR4f, STR5f, STR6f, STR7f, STR8f, STR9f, STR10f, STR11f, STR12f };
char TTRname [12][11] = { TTR1f, TTR2f, TTR3f, TTR4f, TTR5f, TTR6f, TTR7f, TTR8f, TTR9f, TTR10f, TTR11f, TTR12f };
char MTRname [12][11] = { MTR1f, MTR2f, MTR3f, MTR4f, MTR5f, MTR6f, MTR7f, MTR8f, MTR9f, MTR10f, MTR11f, MTR12f };
char KBM1M12 [12][14] = { KBM1f, KBM2f, KBM3f, KBM4f, KBM5f, KBM6f, KBM7f, KBM8f, KBM9f, KBM10f, KBM11f, KBM12f };

int LCDBackLight = 13; // Will be set as OUTPUT and HIGH by TFT init or Brightness level via analog PWM

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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// See the Arduino language specification discussion here: https://github.com/arduino/language/discussions/2
// on multitasking, timing logic and limitations of state machines overcome by threads.
////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
unsigned long NowMillis = 0;           // LCD Backlight Saver
unsigned long LastMillis = 0;          // LCD Backlight Saver
unsigned long VolMuteMillisNow = 0;    // if VolMute not pressed within 5 sec cancel Layout counter
unsigned long VolMuteMillisLast = 0;   // if VolMute not pressed within 5 sec cancel Layout counter
unsigned long VolMuteTimer = 3000;     // 3 Seconds VolMute counter timeout Timer - if too short V0 does not switch Layouts
unsigned long TimePeriod = 120000;     // 120 seconds LCD Backlight Timer change with *tb*num=0,1-5 via keybrd 1=30 sec, 0=120 days
unsigned long RepTimePeriod = 500;     // After this key repeat is active
unsigned long RepLast = 0;             // Time when key first pressed
unsigned long RepNow = 0;              // Time when key still pressed
bool BackLightOn = true;               // LCD Backlight Saver
int VolMuteCount = 0;                  // Can be 0,1=Layout1+VolMute(On/Off),2=Layout2,3=Layout3,4=Layout4 
int Layout = 1;                        // Layout 1, 2, 3 or 4
int LayerAB = 0;                       // Switch [Layout A B] for [Layers 1 3 4] Keys [M1-M6][M7-M12] etc press [Sav](e)

int DimVal = 0;                        // 0 -> Backlight ON/OFF 1-9 dimmed value
int NormVal = 0;                       // Max normal value LCD brightness
const static uint8_t dbArr[10] = {0,        4,    8,    16,   25,   38,   51,   64,  102,  128};
const static uint8_t bbArr[10] = {0,        8,   16,    25,   38,   51,   64,  102,  128,  166};
const static char DBArr[10][3] = { " 0", " 1", " 3",  " 6", "10", "15", "20", "25", "40", "50"};
const static char BBArr[10][4] = {"100", " 3", " 6",  "10", "15", "20", "25", "40", "50", "65"};
const static long unsigned int tbArray[10] = {600000000, 30000, 60000, 90000, 120000, 180000, 300000, 600000, 1800000, 3600000 };

unsigned long timePowerOff = 30;                          
unsigned long TimePowerOff = 3000;
unsigned long timeRestart = 30;                          
unsigned long TimeRestart = 3000;

//                    012345678901234567890123456
char BlankArr[]    = "Blank Time          seconds"; 
char DimArr[]      = "LCD Blank Dim       (x/255)";
char NormArr[]     = "LCD Brightness      (x/255)";
char RestartArr[]  = "Restart T           seconds"; 
char PowerOffArr[] = "PowerOff T          seconds";

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
//////////////////////////////////////////////////////////////////////////////////////////////
// To get new calibration data: 
// 1. Send the macro *ca* with the built-in keyboard to toggle re-calibration option to ON
// 2. Restart or Reset the macro pad
// 3. Do the calibration - press at the TIP of the arrow
// 4. Send the macro *ca* with the built-in keyboard to toggle re-calibration option to OFF
// Press [Del]ete in [Cfg] options - next power-up will do a re-calibration
// Typical Calibration Data: 426, 3464, 279, 3488, 5
///////////////////////////////////////////////////////////////////////////////////////////////
//uint16_t calData[5] = {404, 3468, 263, 3486, 5};     
uint16_t calData[5];                              
uint8_t  calDataOK = 0;
#define CalFile "TouchCalData"  // Name of the Touch calibration file
bool DoCal = false;             // Set true if you want the calibration to run on each boot 

uint16_t t_x = 0, t_y = 0; // Touch coordinates

/////////////////////////////////////////////////////////////////////////////////////////
// Layout 1 to 4 Keys
/////////////////////////////////////////////////////////////////////////////////////////
// Key sizes and spacing
#define KEY_W 95 // Key width  95 x 4 = 380 + 15 x 4 = 440  
#define KEY_H 80 // Key height 80 x 3 = 240 + 12 x 4 = 288
#define KEY_SPACING_X 15    // 12 // X gap
#define KEY_SPACING_Y 12    // 15 // Y gap
// Keypad start position upper left
// #define KEY_X (KEY_W/2) + KEY_SPACING_X   // 65/2 + 12 = 45 X-axis centre first key
// #define KEY_Y (KEY_H/2) + KEY_SPACING_Y   // 60/2 + 15 = 45 Y-axis centre first key
#define KEY_X 50                             // 30 is off-screen on left
#define KEY_Y 50                             // 30 is the topmost with no cut-off

/////////////////////////////////////////////////////////////////////////////////////////////
// Layout 5 small Config Buttons M K M (Math Numeric Keyboard Media) between 1st and 3rd row
/////////////////////////////////////////////////////////////////////////////////////////////
#define PAD_W 30          // 20          // Key width  PAD_X is middle -> Pad X is from 317 to 303 
#define PAD_H 40          // 24          // Key height
#define PAD_SPACING_Y 54  // 30  // was 74 for 3 pads // Next below 
#define PAD_X 455         // 300         // config buttons right side
#define PAD_Y 34           // config buttons top
//////////////////////////////////////////////////////////////////////////////////
// Indicators status Caps-Num-Scroll C N S XPos[7] = {0, 0, 105, 220, 0, 105, 220}
//////////////////////////////////////////////////////////////////////////////////
#define IND_X 105 // 70  // Centred on this - Space in RH side about 16 pixels
#define IND_Y 180 // 139 // 
//////////////////////////////////////////////////////////////////////////////////
// Options Indicators status D 1 2 XPos[7] = {0, 0, 105, 220, 0, 105, 220}
//////////////////////////////////////////////////////////////////////////////////
#define OPT_X 105  // Centred on this - Sppace in RH side about 16 pixels
#define OPT_Y 88   // 
////////////////////////////////////////////////////////////////////////////////
// Message status line at bottom
////////////////////////////////////////////////////////////////////////////////
#define STATUS_X 220 // Centred on this - not middle buttons are off-centre
#define STATUS_Y 296 // 305 bottom of y p etc cut-off - 222 very close to buttons last row

#define KEY_TEXTSIZE 1   // Font size multiplier

// Delay between keypress to give USB HID time to respond
uint8_t keydelay = 50;        // keycode[1,..,6] delay milliseconds
uint8_t keydelay2 = 25;       // keyPress delay
uint8_t keydelay1 = 200;      // keyPress delay for Logoff Poweroff Restart
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
#define LGrey1   0xD69A      // 211, 211, 211
#define LGrey    0xA534 
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
#define Pink1    0xFE19      // 255, 192, 203 
#define Pink     0xFBEE 
#define Brown    0x9A60      // 150,  75,   0 
#define Gold     0xFEA0      // 255, 215,   0 
#define Silver   0xC618      // 192, 192, 192 
#define SkyBlue1 0x867D      // 135, 206, 235 
#define SkyBlue2 0x35B9
#define SkyBlue3 0x2D57
#define SkyBlue  0x2CD5
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
                             
uint16_t keyColor[12] = {0x0000}; // = {};  // construct = {0}; should init all elements 0x00 required for local vars

uint16_t BackgroundColor[4] = {Black, Black, Black, Black};    // 4 Layouts 1-4

uint16_t ButtonOutlineColor[4] = {White, White, White, White}; // 4 Layouts 1-4

uint16_t CfgColour     = SkyBlue;                               // Layout 2 KeyBrd
uint16_t MediaColour   = Orange;                                // Layout 2 Media Keys 
uint16_t MathColour0   = 0xFF51;                                // Math KeyBrd                               
uint16_t MathColour10  = 0xF692;                                // Teal colours
uint16_t MathColour3   = 0xEE0E;
uint16_t MathColour4   = 0xEDCB;
uint16_t MathColour20  = 0xE547;
uint16_t MathColour1   = 0xDD42;
uint16_t MathColour5   = 0xAC41;
uint16_t MathColour2   = 0x8B81;
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
#define Spc  0x20
#define Esc  0x29
#define CtrL 0xE0  // Modifier values defined in button()
#define CtrR 0xE4
#define ShfL 0xE1
#define ShfR 0xE5
#define AltL 0xE2
#define AltR 0xE6
#define WinL 0xE3 
#define WinR 0xE7
#define GuiL 0xE3 // Super Key Left
#define GuiR 0xE7 // Super Key Right
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ctl + Alt + Backspace Logs you out of X back to the login screen - works with all desktop environments and window managers.
// Ctrl + Alt + Delete   Start the reboot process - works in all desktop environments and all window managers
// Ctrl + Alt + T is the shortcut to open terminal in Ubuntu.
// Alt + F2: Run console quick command, instead of opening a terminal and running the command there, you can use Alt+F2 to run the console.
// Alt + F1 (= Super key) Switch between the Activities overview and desktop
// Super + Tab Switch between windows. Hold down Shift for reverse order.
// Super +`~ Switch between windows from the same application
// Alt + Esc Switch between windows in the current workspace. Hold down Shift for reverse order.
// Ctrl + Alt + Tab Give keyboard focus to the top bar. 
// Super + A Show the list of applications.
// Super + PageUp/PageDown Switch between workspaces.
// Shift + Super + PageUp/PageDown Move the current window to a different workspace.
// Shift + Super + ? Move the current window one monitor to the left.
// Super + L Lock the screen.
// Super + V Show the notification list - Esc to close.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ctrl + Escape              - Open Start Menu
// GUI + Ctrl + Shft + B      - Wake PC from black or blank screen:
// GUI + X                    - Windows shutdown options
// Ctrl + Shft + Escape       - open Task Manager
// Ctrl + Shft + N            - create new Folder
// GUI + Shift + S            - Snipping Tool
// GUI + V                    - Clipboard window
// GUI + I                    - open Settings
// AltL + ShftL + Numlock     - Mouse Keys on/off
// Shft 5 times               - Sticky Keys on/off
// Numlock 5 times            - Toggle Keys on/off
// ShftR 8 times              - Filter Keys on/off
// GUI + Ctrl + O             - On-Screen Keyboard
// Shift + F10                - Menu (Mouse Right-click)
// GUI + . or GUI + ;         - Emoji and (some) Math Symbols (click on Omega) insert
// Alt + NumPad Number        - special symbols math characters etc - Numlock must be on Alt+3 Alt+24 Alt+8733 Alt+10102
// Number + Alt+X             - Special symbols
// GUI + R                    - Character Map
// GUI + Space                - Choose Key Layout
// A combination: https://www.tindie.com/products/dekuNukem/duckypad-do-it-all-mechanical-macropad/
// GUI + R + notepad + C/R + Hello World + Ctrl + (repeat 10x) - Open Notepad insert Hello World make it bigger 10 x
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
char keyLabel[12][4] = {""}; //  = {};                 // = ""; all 0x00  
/////////////////////////////////////////////////////////////////////////////////////////////////
//  If Cfg Key pressed new options (new Labels and new Colours)
//  Labels[0/1][1,5,9,13][0-11][0-3]
//  "Hme", "Up",  "PgU", "Del/V+", "<",   "Cfg", ">",  "V0/L14",  "End", "Dwn", "PgD", "V-/Ret",
//  "L14", "  ",  "Vol", "      ", "AB",  "Sav", " ",  "      ",  "Del", "   ", "S12", "      "
/////////////////////////////////////////////////////////////////////////////////////////////////
const static char CfgLabel[12][4] =                                          
{"L14", "BsD",  "Vol", "   ", "A-B",  "Sav", "Med",  "   ",  "Del", "ROf", "S12", "   "};  // All 3 chars + \0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  If Media Controls Arrow Right Key pressed new options in different colour:                                        
//                 "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn",  "PgD", "V-/Ret"
// MediaLabel[x][] "Stp", "   ", "   ", "   ",    "Prv",  "   ",  "Nxt", "   ",     "P-P", "   ",  "  ",  "   " 
// MediaLabel[x][] "|||", "   ", "   ", "   ",    "|<",   "   ",  ">|",  "   ",     ">||", "   ",  "  ",  "   " 
// keyLabel[x][]     0                              4               6                8                            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// const static char MediaLabel[12][4] = 
// {"Stp", "   ", "   ", "   ",    "Prv",  "   ",  "Nxt", "   ",  "P-P", "   ",  "   ", "   "}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MediaLabel[12][4] = 
{   "|||", "   ", "   ", "   ",    "|<",   "   ",  ">|",  "   ",  ">||", "   ",  "   ", "   "}; 

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
char NumkeysX[12][4] =  { 0 };
const static char Numkeys1[12][4] = 
{"BsD", "7",  "8",   "9",  "Ret",  "4",  "5",  "6",  "0",  "1",  "2", "3"}; 
const static char Numkeys2[12][4] = 
{"BsD", " ",  "%",   "xX",  "Ret",  ",",  ".",  "=",  "+",  "-",  "*", "/"}; 
const static char Numkeys3[12][4] = 
{"BsD", "aA", "bB",  "cC",  "Ret", "dD", "eE", "fF",  "(",  ")",  "[", "]"}; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// If sequence starts with modifiers (Shift etc) then up to 6 keys sent together, else sent in sequence
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char KbrdLabel[5][12][4] =                                                    // Xlate only needed for kcode not Keypress
{"abc",  "def",  "ghi",  "EXE", "jkl", "mno", "pqr", "NXT", "stu", "vwx", "yz ", "ADD",    // ASCII->HID Subtract  dec
 "ABC",  "DEF",  "GHI",  "exe", "JKL", "MNO", "PQR", "nxt", "STU", "VWX", "YZ_", "add",    // ASCII->HID Subtract 61 dec
 "012",  "345",  "678",  "EXE", "9+-", "=*/", "!@#", "NXT", "$%^", "&,.", "~` ", "ADD",    // 1-9 subtract 19 dec 0 subtract 9 dec
 "\\|?", "\"<>", "\'()", "EXE", ":[]", "{};", "1F3", "NXT", "4F6", "7F9", "1F2", "ADD",    // F1-F12 special code
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicodes from Mathboard at https://github.com/nup002/Mathboard (MIT license)
// Also see: https://en.wikipedia.org/wiki/Mathematical_operators_and_symbols_in_Unicode
// Greek Letters lower-case alpha to omega
// alpha beta gamma delta epsilon zeta eta theta iota kappa lambda
// mu nu xi omikron pi rho sigma(1,2) tau upsilon phi chi psi omega  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Greek Letters Upper-case and Set theory and Various
// Gamma Delta Theta Lambda Xi Pi Sigma Phi Psi Omega 
// Set theory: not-subset-or-equal disjoint-union union intersection element-of not-element-of empty-set subset-of 
//             not-subset-of set-difference subset-or-equal plus-sign-above-equals-sign double-square-intersection
//             double-square-union intersection-with-dot amalgamation join
// Various:  dotproduct crossproduct plusminus parallel degree infinity 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Algebra and calculus: sum  n-ary-product  integral  double-integral  triple-integral  line-integral  surface-integral  
//                       volume-integral  square-root  cube-root  fourth-root  arrow  partial-derivative  nabla
// Comparison and equivalence: notequal almostequal proportional identicalto lessorequal greaterorequal 
// Modifiers: check combining-tilde combining-bar combining-arrow combining-dot combining-2dot prime double-prime 
//            sup-0 sup-1 sup-2 sup-n sup-i sup-j sub-0 sub-1 sub-2 sub-n sub-i sub-j circumflex 
// Logic: arrow-implies arrow-if-and-only-if there-exists there-does-not-exists uncoditionally-true 
//        unconditionally-false and or xor not proves models  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MathLabel[4][12][4] =                                                    
{"abg",  "dez",  "ehi",  "ESC", "klm", "nko", "prs", "NXT", "stu", "fcp", "oO ", "EXE",
 "GDH",  "LKP",  "SFP",  "ESC", "Os1", "Set", "Set", "NXT", "Set", "Var", "Var", "EXE",
 "Alg",  "Cal",  "Cal",  "ESC", "Alg", "Cal", "Com", "NXT", "Com", "Mod", "Mod", "EXE", 
 "Log",  "Log",  "Log",  "ESC", "Log", "Mod", "Mod", "NXT", "Mod", "Set", "Set", "EXE" };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type in Alt + Numpad key then after symbol show press Alt+X to get hex codes used below
// MathNum[4][3][12][5] -> 4 pages x 3 options per key x 12 keys x 5 chars per uni(hex)code
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MathNum[4][3][12][5] = 
{"03B1",  "03B4",   "03B7",  "0   ",  "03BA",   "03BD",   "03C0",   "0  ",  "03C3",  "03C6",  "03C9", "0  ",
 "03B2",  "03B5",   "03B8",  "0   ",  "03BB",   "03BE",   "03C1",   "0  ",  "03C4",  "03C7",  "03A9", "0  ",
 "03B3",  "03B6",   "03B9",  "0   ",  "03BC",   "03BF",   "03C2",   "0  ",  "03C5",  "03C8",  "0020", "0  ",
 "0393",  "039B",   "03A3",  "0   ",  "03A9",   "222A",   "2209",   "0  ",  "2284",  "22C5",  "2225", "0  ",
 "0394",  "039E",   "03A6",  "0   ",  "2288",   "2229",   "2205",   "0  ",  "2216",  "2A2F",  "00B0", "0  ",
 "0398",  "03A0",   "03A8",  "0   ",  "2A06",   "2208",   "2282",   "0  ",  "2286",  "00B1",  "221E", "0  ",
 "2211",  "2230",   "222D",  "0   ",  "221C",   "220F",   "2260",   "0  ",  "2261",  "0303",  "0307", "0  ",
 "2192",  "222B",   "222E",  "0   ",  "221A",   "2202",   "2248",   "0  ",  "2264",  "0304",  "0308", "0  ",
 "2207",  "222C",   "222F",  "0   ",  "221B",   "2032",   "221D",   "0  ",  "2265",  "20D7",  "2033", "0  ", 
 "21D2",  "2204",   "22BB",  "0   ",  "00AC",   "2070",   "2080",   "0  ",  "2099",  "2A1D",  "2A4F", "0  ",
 "21D4",  "22A4",   "2227",  "0   ",  "22A2",   "2071",   "2081",   "0  ",  "1D62",  "2A3F",  "2A4E", "0  ",
 "2203",  "22A5",   "2228",  "0   ",  "22A8",   "02B2",   "2082",   "0  ",  "2C7C",  "2A40",  "2A72", "0  " }; 

//MathName[4][3][12][32] -> 4 pages x 3 options per key x 12 keys x 32 chars per description
const static char MathName[4][3][12][32] = 
{"alpha", "delta",   "eta",   "0", "kappa",              "nu",          "pi",            "0 ","sigma2",         "phi",          "omega",          "0",
 "beta",  "epsilon", "theta", "0", "lambda",             "xi",          "rho",           "0 ","tau",            "chi",          "Omega",          "0",
 "gamma", "zeta",    "iota",  "0", "mu",                 "omicron",     "sigma1",        "0 ","upsilon",        "psi",          "space",          "0",
 "Gamma", "Lambda", "Sigma",  "0", "Omega",              "union",       "not-element-of","0", "not-subsetof",   "dotproduct",   "parallel",       "0",
 "Delta", "Xi",     "Phi",    "0", "not-subset-or-equal","intersection","empty-set",     "0", "setdifference",  "crossproduct", "degree",         "0",
 "Theta", "Pi",     "Psi",    "0", "disjoint-union",     "element-of",  "subset-of",     "0", "subset-or-equal","plusminus",    "infinity",       "0",
 "sum",  "volume-integral","triple-integral", "0", "fourth-root", "n-ary-prod",    "notequal",     "0", "identicalto",   "combining-tilde", "combining-dot",  "0",
 "arrow","integral",       "line-integral",   "0", "square-root", "partial-deriv", "almostequal",  "0", "lessorequal",   "combining-bar",   "combining-2dot", "0",
 "nabla","double-integral","surface-integral","0", "cube-root",   "prime",         "proportional", "0", "greaterorequal","combining-arrow", "double-prime",   "0",
 "arrow-implies",          "there-does-not-exists", "xor",  "0", "not",    "sup-0", "sub-0",  "0", "sup-n", "join",                 "double-square-union",  "0", 
 "arrow-if-and-only-if",   "unconditionally-true",  "and",  "0", "proves", "sup-i", "sub-1",  "0", "sub-i", "amalgamation",         "double-square-intersection",  "0",   
 "there-exists",           "unconditionally-false", "or",   "0", "models", "sup-j", "sub-2",  "0", "sub-j", "intersection-with-dot","plus-sign-above-equals-sign", "0" }; 
 
///////////////////////////////////////////////////////////////////////////////////////////////////////
// 5 Small Config Buttons between 1 st and 3rd row Red Blue Green SkyBlue Gold
char padLabel[5][4] = {"", "", "", "", ""}; 
uint16_t padColor[20]   = {Black, Black, Black, Black, Black, Red,   Blue,  Green4, SkyBlue, Gold,  
                           Black, Black, Black, Black, Black, Black, Black, Black,  Black,   Black };
uint16_t padOutline[20] = {Black, Black, Black, Black, Black, White, White, White,  White,   White, 
                           Black, Black, Black, Black, Black, Black, Black, Black,  Black,   Black};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Power Restart KeyPad    [Restart  Long Time] [Abort ] [PowerOff  Long Time]        [R-T] [Stp] [O-T] [ ]   
//                         [Restart Short Time] [  Cfg ] [PowerOff Short Time]        [R-t] [Cfg] [O-t] [ ]       
//                         [Restart  Immediate] [Logoff] [PowerOff   Imediate]        [Rst] [Log] [Off] [ ]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char PowerKeysLabel[12][4] = {"R-T",  "Stp",  "O-T", "   ",
                                           "R-t",  "Cfg",  "O-t", "   ",
                                           "Rst",  "Log",  "Off", "   " };
                                           
uint16_t PowerKeysColor[12]             = {Green4, Orange, Pink,  Black, 
                                           Green4, Black,  Pink,  Black,  
                                           Green2, Blue,   Red,   Black };
                         
char ShutDwn[12][45] = { "C:\\Windows\\System32\\shutdown.exe /r /t 6000",
                         "C:\\Windows\\System32\\shutdown.exe /a",
                         "C:\\Windows\\System32\\shutdown.exe /s /t 6000", "   ",
                         "C:\\Windows\\System32\\shutdown.exe /r /t 30",   "Cfg",
                         "C:\\Windows\\System32\\shutdown.exe /s /t 30", "   ",
                         "C:\\Windows\\System32\\shutdown.exe /r /t 0",
                         "C:\\Windows\\System32\\shutdown.exe /l",
                         "C:\\Windows\\System32\\shutdown.exe /s /t 0",  "   " };  // = shutdown.exe /p
                         
const static char hex16[16][2] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"}; 
const static char num16[17][3] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"};
                        
bool Rotate180 = false;          // Rotate screen 180 degrees so that USB cable is on other side

bool Math = false;               // Layer for Math KeyBrd
int  MathX = 0;                  // values 0 1 2 3 4 - 4 adds non-printing chars as numbers to KeyBrdByte
byte MathByte[200]= ""; // ={};  // Hold values to be sent
byte MathCode[200];              // Parallel keycodes
int  Math123 = 0;                // Such as Key [abc] = a or b or c - values 0,1,2
int  MathByteNum = 0;            // Size Math KeyBrdByte
char MathChr[38];                // Current Math description = mathhexnumber + description
char MathHexNum[5];              // Current Math Hex Number as ASCII without 0x

bool Fx = false;                 // F1-F12 current
bool Kbrd = false;               // Layer for KeyBrd
int  KeyBrdX = 0;                // values 0 1 2 3 4 - 4 adds non-printing chars as numbers to KeyBrdByte
byte KeyBrdByte[200]= ""; // ={};// Hold values to be sent
byte KeyCode[200];               // Parallel keycodes
int  KeyBrd123 = 0;              // Such as Key [abc] = a or b or c - values 0,1,2
int  KeyBrdByteNum = 0;          // Size KeyBrdByte

int  Numkeys123 = 0;             // Numkeys1 to Numkeys3

int  PrevButton = 0;             // Used in case-button to check for double key press
char KBDisp[200]= "";   //  ={}; // Keybrd display macro buffer in status line
int  KBDispPos = 0;              // nect char position
bool KBType = false;             // If 1st byte>0x7F use keycode[0-5] else use keyPress
bool KeyRepeat = false;          // If button same a sprevious button pressed
bool KBrdActive = false;         // Let [ADD] know any char key has been pressed at least once before
bool KeyBrdDirect = false;       // Echo Keyboard keys to PC as they are typed

int  MacroM1M12[12] = { 0 };

int  MacroSizeM1M12[12] = { 0 }; 

bool MouseKeys = false;          // Mouse Keyboard
bool NumKeys = false;            // Numeric Keyboard
bool PowerKeys = false;          // Bottom row on Cfg Layout 3 Logoff Off Restart
bool PowerKeysMenu = false;      // If true use GUI+X+u,i,r instead of GUI+R+shutdown - change with *po*
bool PadKeys = false;            // 5 Small Config Buttons on RH side Red Blue Green
bool KeyFontBold = false;        // Button Font Bold/Normal labels
int Option1 = 0;                 // Shows [M1]-[M12] use for macro in keybrd
int Option2 = 0;                 // Not assigned as yet - see optionsindicators()
 
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
char Str1[StrSize],  Str2[StrSize],  Str3[StrSize],  Str4[StrSize],   Str5[StrSize],   Str6[StrSize];
char Str7[StrSize],  Str8[StrSize],  Str9[StrSize],  Str10[StrSize],  Str11[StrSize],  Str12[StrSize];
char Ttr1[StrSize],  Ttr2[StrSize],  Ttr3[StrSize],  Ttr4[StrSize],   Ttr5[StrSize],   Ttr6[StrSize];
char Ttr7[StrSize],  Ttr8[StrSize],  Ttr9[StrSize],  Ttr10[StrSize],  Ttr11[StrSize],  Ttr12[StrSize];
byte Mtr1[ByteSize], Mtr2[ByteSize], Mtr3[ByteSize], Mtr4[ByteSize],  Mtr5[ByteSize],  Mtr6[ByteSize];
byte Mtr7[ByteSize], Mtr8[ByteSize], Mtr9[ByteSize], Mtr10[ByteSize], Mtr11[ByteSize], Mtr12[ByteSize]; 

byte MacroM1[ByteSize], MacroM3[ByteSize], MacroM5[ByteSize], MacroM7[ByteSize], MacroM9[ByteSize],  MacroM11[ByteSize]; // M1-M11 
byte MacroM2[ByteSize], MacroM4[ByteSize], MacroM6[ByteSize], MacroM8[ByteSize], MacroM10[ByteSize], MacroM12[ByteSize]; // M2-M12 

char * Str1to12[] = {Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,Str11,Str12} ;   
char * Ttr1to12[] = {Ttr1,Ttr2,Ttr3,Ttr4,Ttr5,Ttr6,Ttr7,Ttr8,Ttr9,Ttr10,Ttr11,Ttr12} ;
byte * Mtr1to12[] = {Mtr1,Mtr2,Mtr3,Mtr4,Mtr5,Mtr6,Mtr7,Mtr8,Mtr9,Mtr10,Mtr11,Mtr12} ;

byte * MacroM1to12[] = {MacroM1, MacroM2, MacroM3, MacroM4, MacroM5, MacroM6, MacroM7, MacroM8, MacroM9, MacroM10, MacroM11, MacroM12} ;
 
char inputString[StrSize] = "Send new text through serial port start with <x end with >\n where x = key number 1 to 6 or 0 for [M4]\n";  // String for [M4] Key
bool StrOK = false;          // String is complete - ended with 0x0A
bool ByteOK = false;         // Bytes received is complete - ended with 0x00
int  RecBytesLen = 0;        // Number of chars in RecBytes
int  RecBytesType = 1;       // Type 1, 2, 3 = sequence, simulataneous, media
byte RecBytes[MaxBytes];     // Raw bytes received must start < and end with > - can have length byte/int after < to include <> in data
byte NumBytes = 0;
bool NewData = false;
char AltNum[] = "1D6D1";

const int NumButtons = 17;             // Total number of buttons = 12 keys (3 rows x 4 columns) + 5 pads on right
TFT_eSPI_Button key[NumButtons];       // Create 12 keys + 5 config buttons for keypad

///////////////////////////////////////
// Setup
///////////////////////////////////////
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);   // Used for Capslock
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);

  if (!LittleFS.begin()) {LittleFS.format(); LittleFS.begin(); } 
  
  InitCfg(1);                     // Setup config values and strings/macros
   
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB HID Composite");
  usb_hid.setReportCallback(NULL, hid_report_callback); // Activate report CapsScrollNum Lock indicators
  usb_hid.begin();                                      // Assume it does -> tusb_init();
  while( !TinyUSBDevice.mounted() ) delay(1);

  // Initialise the TFT screen TFT_eSPI/TFT_eSPI.h
  tft.init();
  if (Rotate180) tft.setRotation(1);
            else tft.setRotation(3);    // Set the display orientation to 1 or 3 (180 degrees different)
  touch_calibrate();                    // New Calibration Data obtained if DoCal = true
  
  tft.fillScreen(Black); 
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);       
  tft.setTextSize(KEY_TEXTSIZE);
 
  BackLightOn = true;      // TFT init will turn it on

  if (NormVal==0) digitalWrite(LCDBackLight, HIGH);    // Backlight Full ON
            else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON
   
  tft.setTouch(calData);   // void setTouch(uint16_t *data); in  TFT_eSPI/Extensions/Touch.h 
  
  if (SaveLayout) Layout = 2; else Layout = 1;  // Layout 1 (M1-M12 keys default) or 2 (Config)
  ConfigKeyCount = 0;                           // Start up
  ConfigButtons(0);                             // Draw Buttons and Labels 0 = All 3+5 rows

  NumKeysChange();                              // NumkeysX = Layout 0

  VolMuteMillisLast = LastMillis = millis();    // Layout Change Timeout + Initial start time
  VolMuteCount = 0;                             // Layout change
  NewData = KBrdActive = Kbrd = false;
  KeyBrdByteNum = KBDispPos = KeyBrdX = 0;      // Toggle On/Off with yellow pad in Kbrd mode
  KeyBrdDirect = PadKeys = false;               // 5 Small Pads RH side Red Blue Green SkyBlue Gold 
  KeyBrd123 = Numkeys123 = 0; 
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
  if ((NowMillis - LastMillis) >= TimePeriod)            //test whether the period has elapsed
     {if (DimVal==0) digitalWrite(LCDBackLight, LOW);    // Backlight Off
               else  analogWrite(LCDBackLight, DimVal);  // Backlight Dimmed
      LastMillis = NowMillis;                            // Start Timer again
      RepLast = RepNow = NowMillis;                      // Reset repeat key timer
      if (!MuteDisable) VolMuteCount = 0;                // VolMuteKey pressed Count = 0 if not L1-L4 keys active
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
  bool pressed = tft.getTouch(&t_x, &t_y, 450);                              // True if valid key pressed
  
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
  for (uint8_t b = 0; b < NumButtons; b++) { if (key[b].justReleased()) {key[b].drawButton(false); RepLast = RepNow; }              // draw normal - code at release time
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
              
   if ((a>6)&&(a<10)) {Found = true; n--; RecBytesType = a - 6; ByteOK = true;}        // (6<a<10) Non ASCII start string with char 7 - 9
   
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
///////////////////////////////////////////////
void DoPowerKeys (char Cmd, bool Menu, int s) 
///////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here                  
  int n = 0;                      // s = button pressed

  //if ((s==3)||(s==7)||(s==11)) {PowerKeys = false; status(" "); ConfigButtons(1); return;}
  
  if ((s>7)||(s==1)) { PowerKeys = false; // If immediate action Must do this before the actions below
                       status(" ");
                       ConfigButtons(1); }
   
  if ((Menu)&&(s>0)) { keycode[0] = GuiL;             // or use HID_KEY_GUI_RIGHT
                       keycode[1] = HID_KEY_X;        // 'X' or 'x' not the same here
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); // GUI + X then U for shutdown options
                       usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); // Confirm with ArrowLeft Enter 
                       usb_hid.keyboardPress(HIDKbrd, 'u');           delay(keydelay2); // After U it is i u r
                       usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay1); 
                       usb_hid.keyboardPress(HIDKbrd, Cmd);           delay(keydelay2); // logoff switch off restart
                       usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay1); return; }
              
          
  keycode[0] = GuiL;             // or use HID_KEY_GUI_RIGHT
  keycode[1] = HID_KEY_R;        // 'R' or 'r' not the same here
  usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); // GUI + X then U for shutdown options
  usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2);  
  
  for (n=0;  n<strlen(ShutDwn[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn[s][n]); delay(keydelay2);
                                         usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); } 
                            
  usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);           // Confirm with Enter
  usb_hid.keyboardRelease(HIDKbrd);

  if (PowerKeys) { PowerKeys = false; // Clear Power Keys for Timed action
                   status("Timer running");
                   ConfigButtons(1); }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do Button Actions Layouts 1 to 4
// Three HID Keys action types: 
// (1) Keyboard Keys pressed in sequence - usb_hid.keyboardPress(RID_KEYBOARD, keyvar) - keyvar single variable - only ASCII type keys
//     see: https://github.com/adafruit/Adafruit_TinyUSB_Arduino/issues/50
// (2) Keyboard Keys pressed together    - usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode) - keycode 0 - 5 i.e. 6 keys pressed together
//     If modifier used i.e (0xEn keys become 0x0n) use tud_hid_keyboard_report(HIDKbrd, modifier, keycode); to press and send keycode[0]=0x00
//     tud_task(); tud_hid_keyboard_report(HIDKbrd, 0x00, keycode); to release - see Cut Copy Paste in M1 and Control Shift Escape in M6 below - 
//     also see https://forums.raspberrypi.com/viewtopic.php?t=311221
// (3) Keyboard Key 1 or more held + others typed in sequence - Alt + Numpad for Math sym,bols + special characters eg. Alt-Hold + 2, 3 - use
//     tud_hid_keyboard_report(HIDKbrd, modifier, keycode); do not usb_hid.keyboardReport
// (4) Media Keys pressed in sequence    - usb_hid.sendReport16(RID_CONSUMER_CONTROL, mediakeyvar) - mediakeyvar single variable
// The challenge with macro instructions is then to know how to deal with each type (1) to (4) or any combination of (1) to (4)
// Generally if non-test macro has Shift, Alt, GUI, Ctrl as the first key then it will be of type (2) or (3) (at least initially)
// If any other first key then is a normal keyboard key it will be of type (1) initially
// If any other first media key then is type (4) initially 
//
// typedef struct TU_ATTR_PACKED {
//  uint8_t modifier;   /**< Keyboard modifier (KEYBOARD_MODIFIER_* masks). */
//  uint8_t reserved;   /**< Reserved for OEM use, always set to 0. */
//  uint8_t keycode[6]; /**< Key codes of the currently pressed keys. */
//  } hid_keyboard_report_t;
//
#define ModCtrL 0x01 // 0xEn -> 0x0n for modifiers
#define ModCtrR 0x10 // Can just add (instead of or)
#define ModShfL 0x02
#define ModShfR 0x20
#define ModAltL 0x04
#define ModAltR 0x40
#define ModWinL 0x08 
#define ModWinR 0x80
#define ModGuiL 0x08 // Super Key Left
#define ModGuiR 0x80 // Super Key Right
//
//   Key modifiers as defined by the IEEE HID Spec:
//    LEFT  CONTROL          1    # 00000001 as binary
//    LEFT  SHIFT            2    # 00000010
//    LEFT  ALT              4    # 00000100
//    LEFT  CMD|WIN          8    # 00001000
//    RIGHT CONTROL          16   # 00010000
//    RIGHT SHIFT            32   # 00100000
//    RIGHT ALT              64   # 01000000
//    RIGHT CMD|WIN          128  # 10000000
//
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
//  USB keyboards HID Report Descriptor:
// Bit:        7   6   5   4   3   2   1   0
//           +---+---+---+---+---+---+---+---+
// Byte 0    | RG| RA| RS| RC| LG| LA| LS| LC|  Modifier bits (LC=Left Control, LS= Left Shift, etc)
//           +---+---+---+---+---+---+---+---+
// Byte 1    |        Reserved byte          |
//           +---+---+---+---+---+---+---+---+
// Byte 2    |        Key 1                  |
//           +---+---+---+---+---+---+---+---+
// Byte 3    |        Key 2                  |
//           +---+---+---+---+---+---+---+---+
// Byte 4    |        Key 3                  |
//           +---+---+---+---+---+---+---+---+
// Byte 5    |        Key 4                  |
//           +---+---+---+---+---+---+---+---+
// Byte 6    |        Key 5                  |
//           +---+---+---+---+---+---+---+---+
// Byte 7    |        Key 6                  |
//           +---+---+---+---+---+---+---+---+
// https://stackoverflow.com/questions/66671427/multiple-modifiers-2-in-keyboard-input-report-for-custom-hid-keyboard
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MacroKeys(byte c)
{uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here
 uint8_t n;

 if (Layout==3) { for (n=0;  n<strlen(Str1to12[c]); n++) 
                { usb_hid.keyboardPress(HIDKbrd, Str1to12[c][n]); delay(keydelay2);
                  usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } return true; }

 if (Layout==4) { for (n=0;  n<strlen(Ttr1to12[c]); n++) 
                { usb_hid.keyboardPress(HIDKbrd, Ttr1to12[c][n]); delay(keydelay2);
                  usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } return true; }

 if (Layout==1) 
    { //Serial.print(" c "); Serial.print(c); 
      //Serial.print(" ");Serial.print(MacroM1M12[c]);
      //Serial.print(" ");Serial.print(Mtr1to12[c][0]);
      //Serial.print(" ");Serial.println(Mtr1to12[c][1]);                    
      if (MacroM1M12[c]==2) {      // i = LayerAB*6; (MacroM1M12[0+i]==2) = (MacroM1M12[c]==2) M4 = 0,6 = c
          if (Mtr1to12[c][0]<0x80) { for (n=0;  n<MacroSizeM1M12[c]; n++) 
                                     { usb_hid.keyboardPress(HIDKbrd, Mtr1to12[c][n]); delay(keydelay2);
                                       usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); } return true; }                 
          if (Mtr1to12[c][0]>0x7F) { for (n=0;  n<MacroSizeM1M12[c]; n++) { keycode[n] = Mtr1to12[c][n]; }
                                       usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2); 
                                       usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);   return true; } } } // Macro1,7==2  

 return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PadKeysState(int Pbutton, bool Restore)  // Pads are now Pbuttons 1 to 5
{ if (Restore) { if (Pbutton==3) { VolDisable = VolDisablePrev; MuteDisable = MuteDisablePrev;} }                              
          else { if (Pbutton==3) { VolDisablePrev = VolDisable; MuteDisablePrev = MuteDisable;}
                 if (Pbutton==1) { Kbrd = Media = NumKeys = MouseKeys = false;}
                 if (Pbutton==2) { Math = Media = NumKeys = MouseKeys = false;}
                 if (Pbutton==3) { Math = Kbrd  = NumKeys = MouseKeys = false;}                   
                 if (Pbutton==4) { Math = Media = Kbrd    = MouseKeys = false;}
                 if (Pbutton==5) { ;}  } // used as an option key
  ConfigButtons(1);
  if (Kbrd) status("Escape Next Add End"); else status(" ");
}
/////////////////////////////
void buttonpress(int button)
/////////////////////////////
{
  uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here
  uint8_t i, n, a, b, c;          // 
  uint8_t CfgOK ;                 // Do Cfg actions
  uint8_t modifiers = 0;          // Alt Shift Control Gui modifiers b7 RG| RA| RS| RC| LG| LA| LS| LC| b0
  byte *BPtr;
  
  if ( !usb_hid.ready() ) return; // assume it includes -> if (tud_hid_ready()) 

  ////////////////////////////////////////////////////////////////// Skip 1st any-key keypress which re-activates backlight ONLY if completely blank
  LastMillis = millis();                                          // Reset Backlight Timer if any key pressed                                        
  if (!BackLightOn)                                               // Keypress only active if backlight on
     {BackLightOn = true;
      if (NormVal==0) {digitalWrite(LCDBackLight, HIGH); return;} // Backlight Full ON skip 1st keypress
                  else analogWrite(LCDBackLight, NormVal);      } // Backlight Brightness value - continue
   
  // Power Keys must either be executed (buttons 1 or 0,2,4,6,8,9,10) or cancelled (button 5 or 3,7,11)
  if (PowerKeys) {if ((button==3)||(button==7)||(button==11)||(button>11)) {status("Power Keys cancelled"); PowerKeys=false; ConfigButtons(1); return;} }
  
  ///////////////////////////////////////////////////////////////////////////////
  // 5 Config Pads Red Blue Green SkyBlue Gold button > 11
  // Handle 1st then return - as if button = 0-11
  // Pad Keys switch off all other KeyBrds except restore MediaKeys is on before
  ///////////////////////////////////////////////////////////////////////////////
  if ((button>11)&&(Layout==2)) 
     {PadKeys = true; 
      if (button==12) { Math = !Math;           PadKeysState(button-11, !Math);      return; } // Pbutton = 1 to 5                       
      if (button==14) { Media = !Media;         PadKeysState(button-11, !Media);     return; }
      if (button==15) { NumKeys = !NumKeys;     PadKeysState(button-11, !NumKeys);   return; }       
      if (button==16) { if (Kbrd) {if (Option1<11) Option1++; else Option1=0; optionsindicators(); }  // Option 1 = 0-11 12 [M] keys 1-12
                        if (NumKeys) { if (Numkeys123<2) Numkeys123++ ; else Numkeys123 = 0; NumKeysChange(); ConfigButtons(1); }
                        return; }       
      Kbrd = !Kbrd; SendBytesEnd(0); PadKeysState(button-11, !Kbrd); return;                                       
     }
  
  n = 0;                          // for the while loops   

  KeyRepeat = false;
  if (PrevButton!=button) KeyBrd123 = 0; else KeyRepeat = true; // Start at a not b or c if other button previously used
  PrevButton = button;
    
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // StrButton[button]                 4  5  6     8  9  10                4  5  6     8  9   10
  //                c                  0  1  2     3  4  5                 6  7  8     9  10  11
  //byte StrButton[24] = { 0, 0, 0, 0, 0, 1, 2, 0, 3, 4, 5, 0, 0, 0, 0, 0, 6, 7, 8, 0, 9, 10, 11, 0 };
  c = StrButton[button + (LayerAB*12)];   // map button 0-11 to macrokeys 0-11->1-12
  ///////////////////////////////////////////////////////////////////////////////////////////////////

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
      keycode[0] = HID_KEY_X; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(keydelay); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = HID_KEY_X;  
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      //usb_hid.keyboardRelease(HIDKbrd);  break;
    case 1: ///////////////////////////////////////////////// Copy = control + c
      keycode[0] = HID_KEY_C; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(keydelay); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = HID_KEY_C; 
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      //usb_hid.keyboardRelease(HIDKbrd);  break;
    case 2: ////////////////////////////////////////////////// Paste = control + v
      keycode[0] = HID_KEY_V; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(keydelay); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = HID_KEY_V;  
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      //usb_hid.keyboardRelease(HIDKbrd);  break;
    case 3: /////////////////////////////////////////////////// Volume Increase
      if (VolDisable) {if (DelBS) keycode[0] = BckS; else keycode[0] = DelK;  //Delete or BackSpace Key
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(HIDKbrd); break; }
    
      usb_hid.sendReport16(HIDCons, VolUp);  delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);      break;
      
    case 4: // M1 S1 T1 ///////////////////////////////////////////////// Alt + PrintScreen - some mini keyboards cannot do this
         if (MacroKeys(c)) break;

         if (Layout==1) 
            {keycode[0] = AltL; 
             keycode[1] = PrtScr;
             keycode[2] = 0x00; // Need this why? 
             usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
             usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2);
             status("PrtScr Current Window");   }  break; // Layout==1
      
    case 5: // M2 S2 T2 ///////////////////////////////////////////////// Open admin powershell including UAC completion
         if (MacroKeys(c)) break;

         if (Layout==1) 
            {keycode[0] = GuiL;                                               // or use HID_KEY_GUI_RIGHT
             keycode[1] = HID_KEY_X;
             usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); // GUI + X then A admin powershell i normal powershell g Computer Management
             usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); // Confirm with ArrowLeft Enter 
             usb_hid.keyboardPress(HIDKbrd, 'A');           delay(keydelay2); // Can also use Alt + "Y" too confirm
             usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay3); // Can also open via Run GUI + R then Ctrl+Shft+Enter then Alt+Y
             keycode[0] = ArrLeft;
             keycode[1] = KeyEnter;
             usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); 
             usb_hid.keyboardRelease(HIDKbrd);   } break; // Layout==1
      
    case 6: // M3 S3 T3 /////////////////////////////////////////////// Open Run window with last coommand visible and selected
         if (MacroKeys(c)) break;

         if (Layout==1) 
            {keycode[0] = GuiL;                 // or use HID_KEY_GUI_RIGHT
             keycode[1] = HID_KEY_X;
             usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
             usb_hid.keyboardRelease(HIDKbrd);     }  break; // Layout==1
      
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
         if (MacroKeys(c)) break;

         if (Layout==1) {     

      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //                         <    0   CtrL 0xE0 + ShfL 0xE1 + Esc 0x29 >
      //                                     224         225        41
      // Macro 0x3C 0x30 0xE0 0xE1 0x29 0x3E <0 Control Shift Escape > with Layer 4 visible press [M4] will open the Task Manager
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      /*
      if (ByteOK)       { // <0777> shows 4 number HEX values 30 37 37 37 0     0777    RecBytes[1] 55 which is correct
                          // Must do this while pink layer is active
                          keycode[0] = RecBytes[1];     // 0xE0 224 RecBytes[0] = 7,8,9 ASCII
                          keycode[1] = RecBytes[2];     // 0xE1 225
                          keycode[2] = RecBytes[3];     // 0x29 41
                          keycode[3] = KeyEnter;
                          usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
                          usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); break;} 
      */
                          
    for (n=0;  n<strlen(inputString); n++) {usb_hid.keyboardPress(HIDKbrd, inputString[n]); delay(keydelay2);
                                            usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); }   }  break; // Layout==1                                                        
    case 9: // M5 S5 T5 //////////////////////////////////////////// Open normal command prompt via Run window
         if (MacroKeys(c)) break;

         if (Layout==1) 
            {keycode[0] = GuiL;             // Admin CMD/Powershell: GUI + R then cmd/powershell then instead of Enter use Ctrl+Shft+Enter then Alt+Y
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
                 usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); }                      // Layout==1
          break; 
    case 10: // M6 S6 T6 //////////////////////////////////////// M6 text or Enter key LayerAB==1 b = 5; else b =0; 
         if (MacroKeys(c)) break;

         if (Layout==1) {

      // Send Alt (Hold) + Numpad numbers for Math symbols and special characters
      //////////////////////////////////////////////////////////////////////////////////
      // Working Alt + NumPad Special Key left arrow Symbol - NB Numlock must be ON
      //         Alt + 27 as keycode [0]=Alt-L [1]=KeyPad2 [2]=KeyPad4 [3]=0x00 works
      //         Alt + 8733 (infinity symbol) only does up to Alt + 873 (small d thingy)
      //////////////////////////////////////////////////////////////////////////////////
      // Can toggle Numlock on/off with HID_KEY_NUM_LOCK as keycode[0] (not keyPress)
      //////////////////////////////////////////////////////////////////////////////////
      //keycode[0] = HID_KEY_ALT_LEFT;                    // Working
      //keycode[1] = HID_KEY_KEYPAD_2;
      //keycode[2] = HID_KEY_KEYPAD_7;                    // Alt+Keypad 27 = <- symbol               
      //keycode[3] = 0x00;                                // Needed else adds a L/F
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                 break;
      /////////////////////////////////////////////////////////////////////////////////////////
      // 1D6D1 + Alt + X = small pi works in MS Word (1D6D1 = 120529)
      /////////////////////////////////////////////////////////////////////////////////////////
      //for (n=0; n<6; n++) {usb_hid.keyboardPress(HIDKbrd, AltNum[n]); delay(keydelay2);
      //                     usb_hid.keyboardRelease(HIDKbrd);          delay(keydelay2);}
      //keycode[0] = AltL;
      //keycode[1] = 0x1B; // char X from hid.h
      //keycode[2] = 0x00; 
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                 
      //break;          
      ////////////////////////////////////////////////////////////////////////////////////////
      // This turns sticky codes on and off
      // If used for Alt + keypad numbers make sure the option turn off sticky when 
      // 2 keys are pressed at one is off and lock modifier keys when prssed twice in a row
      // is ON. Then turn on sticky keys, press Alt twice then in word press numlock on, then 
      // type 8733 on the keypad, press Alt twice and the open infinity symbol will show
      /////////////////////////////////////////////////////////////////////////////////////////
      /*
      for (n=0; n<5; n++) { keycode[0] = HID_KEY_SHIFT_LEFT;             delay(keydelay2);
                            usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2); }
      usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
      usb_hid.keyboardRelease(HIDKbrd);     break;
      */
      // Open Task Manager ///////////////////////////////////////////////////////
      keycode[0] = Esc; // Thsi works and is shorter than the other way below
      tud_hid_keyboard_report(HIDKbrd, ModCtrL + ModShfL + 0x00, keycode); delay(keydelay); 
      tud_task();        // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode); }   break; // Layout==1
  
      // Enter Key ///////////////////////////////////////////////////////////////                                                        
      //usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
      //usb_hid.keyboardRelease(HIDKbrd);     break; 
      
    case 11: ////////////////////////////////////////// Volume decrease or Return
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
      if (PowerKeys) { DoPowerKeys ('T', PowerKeysMenu, button); break; }
    
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break; }
     
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              MuteDisable = !MuteDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (MuteDisable) status("Volume Mute Key Disable"); else status("Volume Mute Key Enable");
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, StopMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }      

      if (NumKeys) { keycode[0] = BckS;     // Backspace
                     keycode[1] = 0x00;     // NULL
                     usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay2);
                     usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2); break;}

      keycode[0] = KeyHome;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 1: // ARROW UP ////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('a', PowerKeysMenu, button); break; }

      if (MacroM1M12[Option1]==1)     // M1 to M12
         {status("Macro OK"); 
          for (n=0; n<MacroSizeM1M12[Option1]; n++) { Mtr1to12[Option1][n] = MacroM1to12[Option1][n]; } 
          Mtr1to12[Option1][n] = 0x00; MacroM1M12[Option1] = 2; 
          BPtr = MacroM1to12[Option1];
          DoFileBytes(1, MTRname[Option1], BPtr, MacroSizeM1M12[Option1]); break; }
    
      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) {MakeStr(button); break; }
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              DelBS = !DelBS; 
                              //GetSysInfo(1); // Save state on Flash
                              if (DelBS) status("Backspace ON"); else status("Delete ON");
                              ConfigButtons(1); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}
                        
      keycode[0] = ArrUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 2: // PAGE UP /////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('T', PowerKeysMenu, button); break; }
    
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              VolDisable = !VolDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (VolDisable) status("Volume Change Keys Disable"); else status("Volume Change Keys Enable");
                              ConfigButtons(1); break; }                              
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = PageUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 3: // V+ or Delete or Escape(Math) or Execute(Keybrd) //////////////////////////////////////
      if (Math) {Math = false; status("Math exit");  MathX = 0; Math123 = 0; ConfigButtons(1); break;}
    
      if (Kbrd) {if (KeyBrdDirect)  {usb_hid.keyboardPress(HIDKbrd, KeyBrdByte[KeyBrdByteNum]); delay(keydelay);     
                                     usb_hid.keyboardRelease(HIDKbrd);                          delay(keydelay); break; }
                 Kbrd = false; status((char *)KBDisp);                    
                 if (KeyBrdByteNum>0) {SendBytes();}          // Execute macro + exit keybrd
                 SendBytesEnd(1); ConfigButtons(1); break;}   // Clear incase wrong symbols added or no char added
       
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      if (VolDisable) {if (DelBS) keycode[0] = BckS; else keycode[0] = DelK;  //Delete or BackSpace Key
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(HIDKbrd); break; }
        
      usb_hid.sendReport16(HIDCons, VolUp); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);     break;
      
    case 4: // ARROW LEFT //////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('t', PowerKeysMenu, button); break; }
    
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              if (LayerAB==0) {LayerAB=1; status("Layer B");} else {LayerAB=0; status("Layer A");} 
                              //GetSysInfo(1); // Save state on Flash
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, PrevMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }      
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);               break;}

      keycode[0] = ArrLeft;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 5: // CONFIG //////////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) {MakeStr(button); break;}
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      GetSysInfo(0);             //  "Logoff PowerOff Restart         "
      if (ConfigKeyCount==1) {status("Config and Files Saved");           ConfigKeyCount = 0; }
                        else {status("L14 Bs Vol AB Sav Med Del Po S12"); ConfigKeyCount++ ; } 
      ConfigButtons(1);       break;
      
    case 6: // ARROW RIGHT /////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('t', PowerKeysMenu, button); break; }
      
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;               
                              Media = !Media; if (Media) status("Media Keys ON"); else status("Media Keys OFF"); 
                              //GetSysInfo(1); // Save state on Flash
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, NxtMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = ArrRight;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 7: // Vol Mute ///////////////////////////////////////////////////////////////
      if (Math) {if (MathX<3) MathX++; else MathX=0; 
          Math123=0; ConfigButtons(1); break;} // Changes 4 Math KeyBrds from Greek abc -> ABC -> Calculus -> Logic
        
      if (Kbrd) {if (KeyBrdX<4) KeyBrdX++; else KeyBrdX=0; 
          KeyBrd123=0; ConfigButtons(1); break;} // Changes 5 KeyBrds from abc - ABC - 012 - *^% - SHFT
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
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
      if (PowerKeys) { DoPowerKeys ('r', PowerKeysMenu, button); break;} 

      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) {MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {status("Files deleted"); ConfigKeyCount-- ; DeleteFiles(); ConfigButtons(1); break; }
      
      if (Media) {usb_hid.sendReport16(HIDCons, PlayMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }

      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = KeyEnd;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 9: // ARROW DOWN //////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break;}

      // Bottom row on Cfg Layout 3 Logoff Off Restart
      if (ConfigKeyCount==1) {ConfigKeyCount--; PowerKeys = !PowerKeys;  // = true; // works differently but ok
          if (PowerKeys) status("[Rst]art  [Log]off  Power[Off]"); else status(" ");
          ConfigButtons(1); break;}   
             
      if (PowerKeys) { DoPowerKeys ('i', PowerKeysMenu, button); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}
     
      keycode[0] = ArrDown;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 10: // PAGE DOWN //////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('u', PowerKeysMenu, button); break; }

      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) {MakeStr(button); break;}
      
      if (ConfigKeyCount==1) {ConfigKeyCount--; SaveLayout = !SaveLayout; GetSysInfo(1); // Save the state
                              if (SaveLayout) status("Startup Layout L2"); else status("Startup Layout L1"); ConfigButtons(1); break;}

      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
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
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
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
  // also see for loop comma operator use here: https://forum.arduino.cc/t/arduino-does-not-understand-a-ranged-for-loop/164312/10
  int cR[9] = { 0, 1, 2, 4, 5, 6, 8, 9, 10 };    
  if (ConfigKeyCount>0) {for (m=0; m<9; m++) { n = cR[m]; strcpy(keyLabel[n], CfgLabel[n]); keyColor[n]=CfgColour;} } 
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                 "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn",  "PgD", "V-/Ret"
  // MediaLabel[x][] "Stp", "   ", "   ", "   ",    "Prv",  "   ",  "Nxt", "   ",     "P-P", "   ",  "  ",  "   " 
  // MediaLabel[x][] "|||", "   ", "   ", "   ",    "|<",   "   ",  ">|",  "   ",     ">||", "   ",  "  ",  "   " 
  // keyLabel[x][]     0                              4               6                8                                           
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int mR[4] = { 0, 4, 6, 8 };
  if ((Media)&&(b==1)&&(ConfigKeyCount==0)) {for (m=0; m<4; m++) {n = mR[m]; strcpy(keyLabel[n], MediaLabel[n]); }
       keyColor[0] = keyColor[4] = keyColor[6] = keyColor[8] = MediaColour; }
                 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // PowerKeys
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((PowerKeys)&&(b==1)&&(ConfigKeyCount==0)) {for (m=0; m<12; m++) {strcpy(keyLabel[m], PowerKeysLabel[m]); keyColor[m] = PowerKeysColor[m]; } }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Math) { for (m=0; m<12; m++) {strcpy(keyLabel[m], MathLabel[MathX][m]);}
              for (m=0; m<11; m++) {keyColor[m] = MathColour1;} keyColor[3]=keyColor[7]=keyColor[11]=MathColour2; goto KeyBrdsDone; } // I can do it once

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Kbrd) {for (m=0; m<12; m++) {strcpy(keyLabel[m], KbrdLabel[KeyBrdX][m]);}
             for (m=0; m<12; m++) {keyColor[m] = CfgColour;}; goto KeyBrdsDone; }                                                     // twice

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (MouseKeys) { for (m=0; m<12; m++) {strcpy(keyLabel[m], Mousekeys1[m]);}
                   for (m=0; m<12; m++) {keyColor[m] = Pink;}; goto KeyBrdsDone; }                                                    // thrice
                  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (NumKeys)   { for (m=0; m<12; m++) {strcpy(keyLabel[m], NumkeysX[m]);}
                   for (m=1; m<12; m++) {keyColor[m] = DGrey;} keyColor[0]=Blue;keyColor[4]=Red; }
                   
  KeyBrdsDone:

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
   //                     0      1      2     3       4     5       6     7      8       9       10     11     12    13       14    15     16     17      18      19
   // padColor[20]   = {Black, Black, Black, Black, Black, Red,   Blue,  Green4, SkyBlue, Gold,  Black, Black, Black, Black, Black, Black, Black, Black, Black, Black };
   // padOutline[20] = {Black, Black, Black, Black, Black, White, White, White,  White,   White, Black, Black, Black, Black, Black, Black, Black, Black, Black,  Black};

   if ((Layout==2)||(Layout==3)) {  // Only need the below if Layout = 2 (do config pads) or Layout = 3 (clear pad buttons)
   b = Layout - 1;                  // reset b to 0 - 3 

   for (row = 0; row < 5; row++) {PadB = padOutline[row+(b*5)]; PadC = padColor[row+(b*5)]; // row = 0,1,2 + (0 1 2 3 = Layout-1) * 3
        //                           x-middle                    y-middle               width height outline fill  text
        //                            310       36 + 0,1,2,3,4 * 48 = 48,144,182,230,286   10    20             
        key[row+12].initButton(&tft, PAD_X,     PAD_Y + row    * PAD_SPACING_Y,         PAD_W, PAD_H, PadB,  PadC, PadB, padLabel[row], KEY_TEXTSIZE);
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
  KEYBOARD_LED_KANA       = TU_BIT(4)  ///< Kana mode
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
   
  if ((Change)&&(!BackLightOn))  {if (NormVal==0) digitalWrite(LCDBackLight, HIGH);     // Backlight Full ON
                                             else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON
                                  BackLightOn = true; }
}
///////////////////////////
// Number Pad Page Select
///////////////////////////
void NumKeysChange()
{ for (int m=0; m<12; m++) {if (Numkeys123==0) strcpy(NumkeysX[m], Numkeys1[m]);
                            if (Numkeys123==1) strcpy(NumkeysX[m], Numkeys2[m]);
                            if (Numkeys123==2) strcpy(NumkeysX[m], Numkeys3[m]); }
}
//////////////////////////////////////////////////////////////////
//  Only used in setup ByteOk = StrOK = false; => Read file
//////////////////////////////////////////////////////////////////
void InitCfg(bool Option)
{ char *ChrPtr;
  String NameStr;
  byte b, *BPtr;
  int ASize, n = 0;
  uint16_t ByteLen = 0;
  
  if (Option) {
  if (LittleFS.exists("MuteDisable"))      MuteDisable = false; else MuteDisable = true;    // L1-L4 enabled = default VolMute Key disabled 
  if (LittleFS.exists("VolDisable"))       VolDisable  = true;  else VolDisable  = false;   // V+ V- disabled Delete-Return enabled
  if (LittleFS.exists("Layout"))           SaveLayout  = true;  else SaveLayout  = false;   // Selected Saved Layout restored
  if (LittleFS.exists("LayerB"))           LayerAB     = 1;     else LayerAB     = 0;       // Layer A 1-6 or B 7-12 restored
  if (LittleFS.exists("MediaKeys"))        Media       = true;  else Media       = false;   // Media Keys in Layout 2 restored
  if (LittleFS.exists("DeleteBackspace"))  DelBS       = true;  else DelBS       = false;   // Toggle Delete or Backspace key
  if (LittleFS.exists("DoCal"))            DoCal       = true;  else DoCal       = false;   // 1 -> Run calibration on start
  if (LittleFS.exists("Rotate180"))        Rotate180   = true;  else Rotate180   = false;   // Set the display orientation to 1 or 3 (180 degrees different)
  if (LittleFS.exists("KeyFontBold"))      KeyFontBold = true;  else KeyFontBold = false; } // Button Font Bold/Normal labels
  
  for (n=0; n<12; n++) 
      {NameStr = STRname[n]; ChrPtr = Str1to12[n];  // n=0 STRname[0] Str1to12[0] str1to12[0]
       if (!LittleFS.exists(NameStr)) strcpy(ChrPtr, str1to12[n]); else DoFileStrings(StrOK, NameStr, ChrPtr);  }
  
  for (n=0; n<12; n++) 
      {NameStr = TTRname[n]; ChrPtr = Ttr1to12[n];
       if (!LittleFS.exists(NameStr)) strcpy(ChrPtr, str1to12[n]); else DoFileStrings(StrOK, NameStr, ChrPtr); }

  for (n=0; n<12; n++) 
      {NameStr = MTRname[n]; BPtr = Mtr1to12[n]; ASize = ArraySz(Mtr1to12[n]);            // max size
       if (LittleFS.exists(NameStr)) DoFileBytes(ByteOK, NameStr, BPtr, ASize); }
   
  for (n=0; n<12; n++) 
      {NameStr = KBM1M12[n]; BPtr = MacroM1to12[n]; ASize = ArraySz(MacroM1to12[n]); MacroM1M12[n] = 0;                       // 0=M1  11=M12
       if (LittleFS.exists(NameStr)) { MacroSizeM1M12[n] = DoFileBytes(ByteOK, NameStr, BPtr, ASize); MacroM1M12[n] = 2; } }  // actual size

  if (LittleFS.exists("inputStr")) {ChrPtr = inputString; DoFileStrings(StrOK, "inputStr", ChrPtr);}

  if (LittleFS.exists("LCDBlankTime")) {ReadLCDBlank();}

  if (LittleFS.exists("LCDDimmer")) {ReadLCDDim();}

  if (LittleFS.exists("LCDBrightness")) {ReadLCDBright();}

  if (LittleFS.exists("tRestart"))  { timeRestart  = ReadPowerKeys(0);}
  if (LittleFS.exists("TRestart"))  { TimeRestart  = ReadPowerKeys(1);}
  if (LittleFS.exists("tPowerOff")) { timePowerOff = ReadPowerKeys(2);}
  if (LittleFS.exists("TPowerOff")) { TimePowerOff = ReadPowerKeys(3);}
}

///////////////////////////////////////////////////////////////////////////////////////
uint16_t DoFileBytes(bool DoWrite, String STRf,  byte *BytePtr, uint16_t ByteArrayLen)
///////////////////////////////////////////////////////////////////////////////////////
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
// LittleFS supports directories. Path + filename can only have a maximum of 31 characters 
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
{ VolDisable  = true;
  MuteDisable = true;
  if (LittleFS.format()); 
     {LittleFS.begin();
      LittleFS.remove("MuteDisable");      // Must be changed via Cfg - self-restored
      LittleFS.remove("VolDisable");       // Must be changed via Cfg - self-restored
      InitCfg(0);
      Serial.println("Files + Config deleted");}
}
////////////////////////////////////////////////
void GetSysInfo(int Action) // Also save config
//////////////////////////////////////////////// 
{ char *ChrPtr;
  uint32_t  Bt;
  int b = 0;
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
  
  Serial.println("VolumeMacro 11");
  Serial.printf("CPU MHz: %d\n\r", fCPU);
  Serial.printf("FreeHeap: %d\n\r", fHeap);
  Serial.printf("UsedHeap: %d\n\r", uHeap);
  Serial.printf("TotalHeap: %d\n\r", tHeap);
  Serial.printf("Core temperature: %2.1fC\n\r", analogReadTemp());
  
  Serial.println("Files (Number Name Size):");
  ListFiles();
  
  ReadLCDBlank();
  Serial.printf("LCD Blank Timeout: %d seconds\n\r", TimePeriod/1000);     // make sure this is never 0 

  ReadLCDDim(); b = (DimVal*100)/255;
  Serial.printf("LCD Dimmed: %d percent (%d/255)\n", b, DimVal);  

  ReadLCDBright(); b = (NormVal*100)/255; if (b==0) b = 100;
  Serial.printf("LCD Brightness: %d percent (%d/255)\n", b, NormVal);
      
  Serial.printf("Restart Time  (seconds): %d %d\n", timeRestart,  TimeRestart);
  Serial.printf("PowerOff Time (seconds): %d %d\n", timePowerOff, TimePowerOff);

  Serial.print("Calibration Data: "); for (int i = 0; i < 5; i++) { Serial.print(calData[i]); if (i < 4) Serial.print(", "); }
   
}
//////////////////////
void ReadLCDBlank() 
//////////////////////
{ unsigned long int current = 0;             // https://stackoverflow.com/questions/62816850/arduino-read-integer-from-txt
  File f = LittleFS.open("LCDBlankTime", "r");  
  while (f.available()) { int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  if (current>10000) TimePeriod = current; else TimePeriod = 30000;             // 10 seconds minimum Blank time - never 0!
}
//////////////////////
void ReadLCDBright() 
//////////////////////
{ int current = 0;             
  File f = LittleFS.open("LCDBrightness", "r");  
  while (f.available()) { int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  if (current<255) NormVal = current; else NormVal = 0;  // 50% else maximum on                   
}
//////////////////////
void ReadLCDDim() 
//////////////////////
{ int current = 0;             
  File f = LittleFS.open("LCDDimmer", "r");  
  while (f.available()) { int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  if (current<129) DimVal = current; else DimVal = 0;                         // 50% max eles OFF
}
/////////////////////////////////////////
unsigned long ReadPowerKeys(int Option)
/////////////////////////////////////////
{ int current = 0; 
  char PArray[4][10] = {"tRestart", "TRestart", "tPowerOff", "TPowerOff"};
  File f = LittleFS.open(PArray[Option], "r"); 
  while (f.available()) { int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  return current; 
}
///////////////////////////////////////////
void WritePowerKeys(unsigned PVal, int Option) 
///////////////////////////////////////////
{ char PArray[4][10] = {"tRestart", "TRestart", "tPowerOff", "TPowerOff"};
   //                     012345678901234567890123456    012345678901234567890123456
  char RPArr[4][28]  = { "Restart t           seconds", "Restart T           seconds",
                         "PowerOff t          seconds", "PowerOff T          seconds" }; 
    
  File f = LittleFS.open(PArray[Option], "w"); 
  if (f) {f.print(PVal); f.close(); }

  int i = 6;                                                                   // 3 digits
  while (i!=0) { RPArr[Option][i+12] = (PVal%10) + 48; PVal = PVal/10; i--; };  // modulus (%) and division (/) operations 
  status(RPArr[Option]); SendBytesEnd(1); 
 
}
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                012345678901234567890123456789012345                   01234567890123456789012345678901
  // if (ConfigKeyCount==1) status("Config and Files Saved");               else   status("L14 Bs Vol AB Sav Med Del Po S12"); 
  // if (Rotate180)         status("Restart 180 degrees rotate calibrate");
  // if (PowerKeys)         status("[Rst]art  [Log]off  Power[Off]");       else   status(" ");
  // if (PowerKeys)         status("Timer running");
  // if (MuteDisable)       status("Volume Mute Key Disable");              else   status("Volume Mute Key Enable");  
  // if (VolDisable)        status("Volume Change Keys Disable");           else   status("Volume Change Keys Enable"); 
  // if (DelBS)             status("Backspace ON");                         else   status("Delete ON");
  // if (Kbrd)              status("Escape Next Add End");                  else   status(" ");
  // if (!KBrdActive)       status("Press char/mod first");  
  // if (SaveLayout)        status("Startup Layout L2");                    else   status("Startup Layout L1");
  // if (Media)             status("Media Keys ON");                        else   status("Media Keys OFF"); 
  // if (LayerAB==0)        status("Layer B");                              else   status("Layer A"); 
  // if (DoCal)             status("Calibrate ON - send *ca* OFF");         else   status("Calibrate OFF - send *ca* ON");
  // if (KeyFontBold)       status("Key Font Bold is ON");                  else status("Key Font Bold is OFF");
  // if (KeyBrdDirect)      status("KeyBoard Direct ON");                   else status("KeyBoard Direct OFF");  
  // if (Macro1==1)         status("[M1] = Macro 1"); 
  //                        status("Macro 1 Press [Up] save [M1]");
  // if (Macro2==1)         status("[M2] = Macro 2"); 
  //                        status("Macro 2 Press [Up] save [M2]");
  // if (Math)              status("Math exit");  
  // if (Math)              status("Math symbol sent");
  // 
  //                        status((char *)MathChr); status((char *)KBDisp); 
  //                        status(BlankArr); status(DimArr); status(RPArr[Option]);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////
// Display mini status bar
// STATUS_X 150 // Centred on this 320/2=160
// STATUS_Y 222 // bottom
/////////////////////////////////////////////////
void status(const char *msg) {
  tft.setTextPadding(320);
  //tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(SkyBlue1, Black);
  if (Kbrd) tft.setTextColor(Yellow, Black);
  if (Math) tft.setTextColor(White, Black);
  if (PowerKeys) tft.setTextColor(Orange, Black);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(1);
  //tft.setTextSize(1);
  tft.drawString(msg, STATUS_X, STATUS_Y);
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);
}

/////////////////////////
bool SendBytesStarCodes()
///////////////////////// 
{ unsigned long t, z;
  uint8_t a, b, n, i, d;

  if ((KeyBrdByte[1]==0x66)&&(KeyBrdByte[2]==0x6F))     // *fo* = Key Label Bold Normal Font
     {KeyFontBold = !KeyFontBold; 
      if (KeyFontBold) { status("Key Font Bold is ON"); 
                         File f = LittleFS.open("KeyFontBold", "w"); 
                         if (f) {f.print("1"); f.close(); } 
                         tft.setFreeFont(&FreeSansBold12pt7b); }
                  else { status("Key Font Bold is OFF"); 
                         LittleFS.remove("KeyFontBold"); 
                         tft.setFreeFont(&FreeSans12pt7b); } 
      ConfigButtons(1); SendBytesEnd(1); return true; } 

  if ((KeyBrdByte[1]==0x6B)&&(KeyBrdByte[2]==0x62))     // *kb* = num keyboard turn on
     {NumKeys = true; ConfigButtons(1); SendBytesEnd(1); status(" "); return true; }

  if ((KeyBrdByte[1]==0x72)&&(KeyBrdByte[2]==0x6F))     // *ro* = rotate 180 degrees turn on
     {Rotate180 = !Rotate180;                           // Toggle On/Off
     if (Rotate180) 
        {File f = LittleFS.open("Rotate180", "w"); 
         if (f) {f.print(Rotate180); f.close(); } }
     if (!Rotate180) LittleFS.remove("Rotate180");      // no else here must be crystal-clear
     status("Restart 180 degrees rotate calibrate"); 
     LittleFS.remove("TouchCalData"); return true; }    // this forces re-calibration as well     

  if ((KeyBrdByte[1]==0x63)&&(KeyBrdByte[2]==0x61))     // *ca* = Calibration On/Off 
     {DoCal = !DoCal; if (DoCal) { status("Calibrate ON - send *ca* OFF"); 
                                   File f = LittleFS.open("DoCal", "w"); if (f) {f.print(DoCal); f.close(); } return true; } 
                            else { status("Calibrate OFF - send *ca* ON"); LittleFS.remove("DoCal"); return true; }  }      
      
  if ((KeyBrdByte[1]==0x6D)&&(KeyBrdByte[2]==0x6B))     // *mk* = Mouse keys turn on
     {MouseKeys = true; ConfigButtons(1); SendBytesEnd(1); status(" "); return true; }

  if ((KeyBrdByte[1]==0x70)&&(KeyBrdByte[2]==0x6F))     // *po* = Power Keys Menu Option 
     {PowerKeysMenu = !PowerKeysMenu; if (PowerKeysMenu) status("PowerKeys Menu ON"); else status("PowerKeys Menu OFF"); return true; }

  b = (KeyBrdByte[4]-48);    // b = 0 - 9
  if (b >9) return false;    // do nothing if not
                      
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // z = TimePeriod/1000;  i = 6; while (i!=0) { BlankArr[i+12] = (z%10) + 48; z = z/10; i--; }   // modulus (%) and division (/) operations 
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  if ((KeyBrdByte[1]==0x74)&&(KeyBrdByte[2]==0x62))                    // *tb*num = blank lcd timemout *tb*0,1-9 
      {t = TimePeriod; TimePeriod=tbArray[b];                          // 1 = 30 sec 9 = 60 min
       z = TimePeriod/1000;  i = 6;                                    // 6 digits
       while (i!=0) { BlankArr[i+12] = (z%10) + 48; z = z/10; i--; }   // modulus (%) and division (/) operations 
       File f = LittleFS.open("LCDBlankTime", "w");
       if (f) {f.print(TimePeriod); f.close(); }                       // Stored as 6 ASCII number bytes such as 49 53 48 48 48 48 = 150000 = 150 sec
       Serial.println(TimePeriod);
       SendBytesEnd(1); status(BlankArr); return true; }  
                                                                                                               
  if ((KeyBrdByte[1]==0x64)&&(KeyBrdByte[2]==0x62))                    // *db*num = dim backlight pwm  *db*0,1-9
      {DimVal=dbArr[b];                                                // 
       for (i=0; i<2; i++) DimArr[i+16] = BBArr[b][i];                 //
       File f = LittleFS.open("LCDDimmer", "w");
       if (f) {f.print(DimVal); f.close(); }                           // Stored as 1-3 ASCII number bytes 48 48 56 = 008 = 8/255 %
       Serial.println(DimVal);
       SendBytesEnd(1); status(DimArr); return true; }      

  if ((KeyBrdByte[1]==0x62)&&(KeyBrdByte[2]==0x62))                    // *bb*num = normal backlight pwm  *bb*0,1-9 0 8/255 - 128/255 % PWM
      {NormVal=bbArr[b];                                               // 0, 8/255 - 128/255 % PWM
      if (NormVal==0) digitalWrite(LCDBackLight, HIGH);                // Backlight Full ON
                else  analogWrite(LCDBackLight, NormVal);              // Backlight Brightness ON
       for (i=0; i<3; i++) NormArr[i+15] = BBArr[b][i];                // 
       File f = LittleFS.open("LCDBrightness", "w");
       if (f) {f.print(NormVal); f.close(); }                          // Stored as 1-3 ASCII number bytes 48 48 56 = 008 = 8/255 %
       Serial.println(NormVal);
       SendBytesEnd(1); status(NormArr); return true; }      
       
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
   if (KeyBrdByte[1]==0x6F)
      { if (KeyBrdByte[2]==0x74)                                       // *ot*num 
           {t = timePowerOff; timePowerOff=b*30;                       // 1 = 30 seconds to 9 = 270 seconds
           if (timePowerOff>600) timePowerOff = t;                     // 10 minutes max
           if (timePowerOff==0) timePowerOff = 600;                    // 10 minutes
           WritePowerKeys(timePowerOff, 2);}                           // "tRestart", "TRestart", "tPowerOff", "TPowerOff"
        if (KeyBrdByte[2]==0x54)                                       // *oT*num 
           {t = TimePowerOff; TimePowerOff=b*1000;                     // 1 = 1000 seconds to 9 = 9000 seconds (150 minutes)
           if (TimePowerOff>60000) TimePowerOff = t;                   // restore to current time
           if (TimePowerOff==0) TimePowerOff = 60000;                  // 1000 minutes max (16.7 hours)
           WritePowerKeys(TimePowerOff, 3) ;} return true; }           // "tRestart", "TRestart", "tPowerOff", "TPowerOff"
      
   if (KeyBrdByte[1]==0x72)
      { if (KeyBrdByte[2]==0x74)                                       // *rt*num 
           {t = timeRestart; timeRestart=b*30;                         // 1 = 30 seconds to 9 = 270 seconds
           if (timeRestart>600) timeRestart = t;                       // 10 minutes max
           if (timeRestart==0) timeRestart = 600;                      // 10 minutes
           WritePowerKeys(timeRestart, 0); return true; }              // "tRestart", "TRestart", "tPowerOff", "TPowerOff"
       if ((KeyBrdByte[1]==0x72)&&(KeyBrdByte[2]==0x54))               // *rT*num 
          {t = TimeRestart; TimeRestart=b*1000;                        // 1 = 1000 seconds to 9 = 9000 seconds (150 minutes)
          if (TimeRestart>60000) TimeRestart = t;                      // restore to current time
          if (TimeRestart==0) TimeRestart = 60000;                     // 1000 minutes max (16.7 hours)
          WritePowerKeys(TimeRestart, 1); return true;} }              // "tRestart", "TRestart", "tPowerOff", "TPowerOff"    

   return false;
}

////////////////////////////
void SendBytesEnd(bool All)
////////////////////////////
{ int n;
  KeyBrdByteNum = 0; KBDispPos = 0;  KeyBrdX = 0; KeyBrd123 = 0;
  for (n = 0; n < 200; n++) { KeyBrdByte[n] = 0x00; KBDisp[n] = '\0'; }
  if (All) {Kbrd = false;} KBType = false; Fx = false; KBrdActive = false; optionsindicators(); }
//////////////////////////////////////////////////
void SendBytes()          //  KeyBrd Macro Editor
//////////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 }; // simultaneous keys pressed in here
  uint8_t a, b, n, i;
  char status1[] = {"Macro Press [Up] save [M00]"} ;
  String NameStr;
 
  // Check for special commands Start with * eg *ab*n n = 0-9
  if (KeyBrdByte[0]==0x2A) { if (SendBytesStarCodes()) return; }  

  NameStr = KBM1M12[Option1]; 
  if (Option1<10) {status1[25] = num16[Option1][0];} else {status1[24] = '1'; status1[25] = num16[Option1][1];}
  Serial.print(status1);  Serial.print(' '); Serial.println(Option1);
  
  // sequential via keyPress 
  if ((!Fx)&&(KeyBrdByte[0]<0x80))                                                   // F1-F12 must be sent with keycode not keyPress
     { for (n = 0; n < KeyBrdByteNum; n++) if (KeyBrdByte[n]<0x80)                   // When 1st char is normal alphanumeric all must be <0x80    
           {usb_hid.keyboardPress(RID_KEYBOARD, KeyBrdByte[n]); delay(keydelay);     // Converted via end of hid.h from ascii to hid
            usb_hid.keyboardRelease(RID_KEYBOARD);              delay(keydelay); 
            MacroM1to12[Option1][n] = KeyBrdByte[n];}
       File f = LittleFS.open(NameStr, "w");
       if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }         
       status(status1); MacroM1M12[Option1] = 1; MacroSizeM1M12[Option1] = KeyBrdByteNum;}

  // simultaneous via keycode 
  for (n=0; n<6; n++) keycode[6] = 0x00;       
  if ((Fx)||(KeyBrdByte[0]>0x7F)&&(KeyBrdByteNum<6))
     { for (n = 0; n < KeyBrdByteNum; n++) {keycode[n] = KeyBrdByte[n]; MacroM1to12[Option1][n] = KeyBrdByte[n]; }  
           usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode); delay(keydelay);
           usb_hid.keyboardRelease(RID_KEYBOARD);            delay(keydelay);  
       File f = LittleFS.open(NameStr, "w");
       if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }  
       status(status1); MacroM1M12[Option1] = 1; MacroSizeM1M12[Option1] = KeyBrdByteNum;}
       
      /*
      for (n = 0; n < KeyBrdByteNum; n++) 
          {Serial.print(KeyBrdByte[n], HEX);
           Serial.print(' '); }
      Serial.println(" ");
      for (n = 0; n < 6; n++)   
          {Serial.print(keycode[n], HEX);
           Serial.print(' '); }
      */ 
      
SendBytesEnd(1); //clean-up

}

////////////////////////
void MakeStr(int button)
////////////////////////
{   int i, n;
    byte a, b, c;
    a = 0;
    Fx = false;     // Only true for one MakeStr type keypress at a time see ***Fx*** below

    b = KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123];  // b is current key label character or modifier
    
    KBrdActive = true; // Let [ADD] know any char key has been pressed at least once before

    if (KBType)                                                                          // Needed if Mod key pressed first
       {if (KeyBrdX==0) if (0x60<b<0x7B) {a = b - 93;}                                   // ((b>0x60) && (b<0x7B)) Xlate a-x
        if (KeyBrdX==1) if (0x40<b<0x5B) {a = b - 61;}                                   // ((b>0x40) && (b<0x5B)) Xlate A-X but leave DelK = 0x4C
        if (KeyBrdX==2) if (0x2F<b<0x3A) {if (b==0x30) a = b - 9; else a = b - 19; } }   // ((b>0x2F) && (b<0x3A)) Xlate 0-9 to 1-0
       
    if ((KeyBrdX==3) && ((button==6)||(button==8)||(button==9)||(button==10))) {a = KeyBrdFx[KeyBrd123][button-6]; Fx = true;}   // Xlate F1 - F12   
    
    for (int n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } Serial.println();
           
    if (KeyBrdX==4) {KeyBrdByte[KeyBrdByteNum] = KeyBrdNum[KeyBrd123][button]; 
        for (n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } Serial.println(); }
           
    n = 0;
    if (KeyBrdX<4) 
       {KBDisp[KBDispPos] = b; KBDisp[KBDispPos+1] = ' '; KBDisp[KBDispPos+2] = ' '; 
        if (a>0) KeyBrdByte[KeyBrdByteNum] = a; 
        if (Fx) { for (n=0; n<3; n++) {KBDisp[KBDispPos+n] = KeyBrdFxChr[KeyBrd123][button-6][n]; } } }    // Fx true for one MakeStr() ***Fx***
        
    if (KeyBrdX==4) for (n=0; n<3; n++) KBDisp[KBDispPos+n] = KeyBrdSpc[KeyBrd123][button][n];
    
    for (i = 0; i <= KBDispPos+n; i++) {Serial.print(KBDisp[i]);  Serial.print(' '); } Serial.println();    

    status((char *)KBDisp);

    if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; 
}


/////////////////////////////
void MakeMathStr(int button)
/////////////////////////////
{   int i, n;
    for (i=0; i<4;  i++) {MathHexNum[i] = MathNum[MathX][Math123][button][i];     // [KeyBrd 0-3=MathX][Symbol 0,1,2 = Math123][button 1-11]
                          MathChr[i] = MathHexNum[i]; }                           // MathNum = 4 hex chars + 0x00 -> use i<4 not i<5                          
    MathChr[i] = Spc;                         
    for (n=0; n<32; n++) {MathChr[n+i+1] = MathName[MathX][Math123][button][n]; if (MathChr[n+i+1]==0) break; }          
                          
    for (n=0; n<5; n++) {Serial.print(MathHexNum[n]); Serial.print(' '); } Serial.println(MathChr);   
    status((char *)MathChr);

    if (Math123<2) Math123++; else Math123=0; 
}

////////////////////////
void SendMath()
////////////////////////
{   int n;
    uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here    
    /////////////////////////////////////////////////////////////////////////////////////////
    // 1D6D1 + Alt + X = small pi works in MS Word and LibreOffice (1D6D1 = 120529)
    // Watch out for auto-correction if the symbol is the first in a sentence/line.
    /////////////////////////////////////////////////////////////////////////////////////////
    for (n=0; n<4; n++) { usb_hid.keyboardPress(HIDKbrd, MathHexNum[n]); delay(keydelay2);   // MathNum = 4 hex chars + 0x00 -> use i<4 not i<5
                          usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2);}
                          
      for (n=0; n<6; n++) keycode[6] = 0x00;                      
      keycode[0] = AltL;
      keycode[1] = 0x1B; // char X from hid.h       
      usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(100);
      usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
}
//////////////////////////////////////////////////////////////////////////////////
// Options Indicators status D XPos[7] = {0, 0, 78, 156, 0, 78, 156}
//////////////////////////////////////////////////////////////////////////////////
// #define OPT_X 105   // Centred on this - Sppace in RH side about 16 pixels
// #define OPT_Y 88    // 
//////////////////////////////////////////////////////////////////////////////////
void optionsindicators() {
  
  tft.setTextPadding(28);
  tft.setTextColor(Cyan, Black);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(1);
  if ((KeyBrdDirect)&&(Kbrd)) tft.drawString("D", OPT_X, OPT_Y);     else { tft.drawString(" ", OPT_X, OPT_Y); }
  if ((Option1<12)&&(Kbrd))   tft.drawString(num16[Option1], OPT_X+110, OPT_Y); else { tft.drawString("  ", OPT_X+110, OPT_Y); }
  if ((Option2>0)&&(Kbrd))    tft.drawString(num16[Option2], OPT_X+220, OPT_Y); else { tft.drawString("  ", OPT_X+220, OPT_Y); }
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);   
}
////////////////////////////////////////////////////////////////////////////
// #define IND_X 105  // Centred on this - Space in RH side about 16 pixels
// #define IND_Y 180  // 
// IndXPos[7] = {0, 0, 78, 156, 0, 78, 156};
////////////////////////////////////////////////////////////////////////////

void indicators() {
  tft.setTextPadding(14);
  tft.setTextColor(Green, Black);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(1);
  if (CapsLock)   tft.drawString("C", IND_X, IND_Y);      else tft.drawString(" ", IND_X, IND_Y);
  if (NumLock)    tft.drawString("N", IND_X+110, IND_Y);  else tft.drawString(" ", IND_X+110, IND_Y);
  if (ScrollLock) tft.drawString("S", IND_X+220, IND_Y);  else tft.drawString(" ", IND_X+220, IND_Y);
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);   
}  

////////////////////////////
void touch_calibrate()
///////////////////////////
{ LittleFS.begin();
    
  if (LittleFS.exists(CalFile))        
     {if ( DoCal)                
         { LittleFS.remove(CalFile);}             // Delete if we want to re-calibrate
      else {File f = LittleFS.open(CalFile, "r"); // check if calibration file exists and size is correct
            if (f) {if (f.readBytes((char *)calData, 14) == 14) calDataOK = 1;  f.close(); }  }  }

  if (calDataOK && !DoCal)         // calibration data valid
     {tft.setTouch(calData);}    
     else { tft.fillScreen(Black); // data not valid so recalibrate
            tft.setCursor(20, 0);
            tft.setTextFont(2);
            tft.setTextSize(1);
            tft.setTextColor(White, Black);
            tft.println("Touch corners as indicated");
            tft.setTextFont(1);
            tft.println();
            if (DoCal) 
               { tft.setTextColor(Red, Black);
                 tft.println("Set DoCal to false to stop this running again!"); }
            tft.calibrateTouch(calData, SkyBlue, Black, 15);
            tft.setTextColor(Green, Black);
            tft.println("Calibration complete!");
            File f = LittleFS.open(CalFile, "w");  // store data
            if (f) { f.write((const unsigned char *)calData, 14); f.close();  }  }
}
