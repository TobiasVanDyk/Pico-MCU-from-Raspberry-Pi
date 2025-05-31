///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Combination of (1) Raspberry Pi Pico Touch Macro Keyboard by Dustin Watts
//                    https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard
//                (2) AdafruitTinyUSB HID examples hid_composite.ino hid_boot_keyboard.ino
//                    https://github.com/adafruit/Adafruit_TinyUSB_Arduino/
//                (3) Bodmer-Adafruit Keypad Keypad_480x320.ino
//                    https://github.com/Bodmer/TFT_eSPI/examples/
//                
// For an inspirational interface refer to the beautifully useful Adafruit Touch Desk which 
// shares a similar layout approach to what is used here - their design dates back to early 2021. 
// https://learn.adafruit.com/touch-deck-diy-tft-customized-control-pad?view=all
//
// Adapted by Tobias van Dyk August 2022 - May 2025 - for the Pico 2 RP2350 and ILI9488 480x320 LCD
// This use the the Waveshare 3.5inch Touch Display Module for Raspberry Pi Pico 1,2 with included SDCard module:
// https://www.waveshare.com/pico-restouch-lcd-3.5.htm
//
// Use the same code but different coloursDef.h and LCD-dimmed values for the:
// Waveshare 4inch Touch LCD Arduino Shield with built-in SDCard
// https://www.waveshare.com/4inch-tft-touch-shield.htm and also the
// Waveshare Raspberry Pi TouchLCD 3.5 inch Type B with an SDCard SPI reader module added
// https://www.waveshare.com/product/3.5inch-RPi-LCD-B.htm and also the
// Waveshare RPi TouchLCD 3.5 inch Type C (125 MHz) TFT with an SDCard SPI reader module added.
// https://www.waveshare.com/3.5inch-rpi-lcd-c.htm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/************************************************************************************
 Adafruit invests time and resources providing this open source code, please support 
 Adafruit and open-source hardware by purchasing products from Adafruit
 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
*************************************************************************************/
#include "Adafruit_TinyUSB.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>      // Case sensitive - up to 255 char filenames - all chars except NULL and '/'
#include <SDFS.h>          // Case preserving not case sensitive - Up to 255 char filenames
#include "strings24.h"    // S keys S1 - S24 load with *fs*[EXE] save to Flash/SDCard with *ss*[EXE]
#include "stringt24.h"    // T keys T1 - T24 load with *ft*[EXE] save to Flash/SDCard with *st*[EXE]
#include "stringRSL.h"    // Reboot or Shutdown or Logout strings for Windows, Linux, RaspiOS
#include "coloursDef.h"   // Colours used
#include "keysDef.h"      // Keys used
#include "mathKeys.h"     // Special symbols, Greek characters , and Math Symbols 
#include "macroBanks.h"   // 5 Sets of 24 macro keys
#include "sdcard.h"       // 20 Sets of 24 files stored on SDCard + 96 n-keys aA01-xX96 001-996
#include <TimeLib.h>      // Replacement fpr Pico 1 RTC functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int pinSdCs = 22;
const int pinSdClk = 10;
const int pinSdMosi = 11;
const int pinSdMiso = 12;
///////////////////////////

bool ResetOnce = true;
bool ResetOnceEnable = false;

byte OptionOS = 0;   // 0 Windows 1 Linux 2 RPiOS 3 MacOS
char OSName[4][18]   = {"Microsoft Windows","GNU/Linux","Raspberry Pi OS","Apple macOS" }; 
char OSChr [4][2]    = {"W",                "L",        "R",              "A"           };   // Use "w" if "W" too large for space
bool SaveOptionOS = false; // OS changed can save with [Sav]

#define HIDCons RID_CONSUMER_CONTROL 
#define HIDKbrd RID_KEYBOARD  
#define HIDMouse RID_MOUSE       

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

uint8_t static const conv_table[128][2] =  { HID_ASCII_TO_KEYCODE };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0   KeySkip 1  CheckSerial 0  KeyHeldEnable  1           BLOnOff 1    Rotate180 0              KeyFontBold 0      ResetOnceEnable 0
// 7     nKeys 1       nChar  n      nKeysPage  8  nKeysCharSet[10] c         CRLF 0                    crlf1 0x0D             crlf2 0x0A
// 23    iList 0       MuteOn 0           VolOn 1  LayerAxD 0 Media 0       XFiles 0               Brightness 0           
// 30   BsDNum 0       RetNum 8         LayerAD 0     KeyFontColour 0   SaveLayout 2                 OptionOS 0            KeyRepeat 6 
// 37  NormVal 0       DimVal 3         nKeys34 1          nDir[20] c        nDirZ always=0  nKeysLnkChar[10] 10               nDirX 0,1,2,3
// 72   MLabel 0       SLabel 0          TLabel 0           VolOn1  1      VolOn2  1                   VolOn3 1 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Currently last entry TLabel = Config1[74]; Can use strcpy((char *)&Config1[40], nDir); and inverse, to access as char string array nDirZ=0=EOS 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cSt byte Config1Size = 80;       //   0   1   2   3   4   5   6   7  8  9  10  11  12  13  14  15  16  17  18  19 20 21   22   23 24 25 26 27 28 29  30  31 
byte Config1[Config1Size]          = {1,  0,  1,  1,  0,  0,  0,  1,'n',8,'n','o','p','q','r','s','t','m','a','k',0, 0x0D,0x0A,0, 1, 0, 0, 0, 0, 0,  0,  8, 
                                      0,  0,  2,  0,  6,  0,  3,  1,'/',0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0,   0,   0, 0, 0, 0, 0, 0,'n','o','p',
                                     'q','u','v','w','x','y','z', 0, 0, 0, 0,  1,  1,  1,  0,  0  };
cSt byte Config1Reset[Config1Size] = {1,0,1,1,0,0,0,1,'n',8,'n','o','p','q','r','s','t','m','a','k',0,0x0D,0x0A,0, 1, 0, 0, 0, 0, 0,  0,  8, 
                                      0,0,2,0,6,0,3,1,'/',0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,0,   0,   0, 0, 0, 0, 0, 0,'n','o','p',
                                     'q','u','v','w','x','y','z', 0, 0, 0, 0,  1,  1,  1,  0,  0  };                                    
bool WriteConfig1Change = false; // Do save if true
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
unsigned long NowMillis = 0;           // LCD Backlight Saver
unsigned long LastMillis = 0;          // LCD Backlight Saver
unsigned long RepTimePeriod = 600;     // Change with Keyrepeat 200 - 900 milleseconds After this key repeat is active
unsigned long RepLast = 0;             // Time when key first pressed
unsigned long RepNow = 0;              // Time when key still pressed
byte Layout = 1;                       // Layout 1 2 3 4 = M Config+KeyBrds S T
byte SaveLayout = 0;                   // 5 possible Values are 0 to 4 for 0=default=layout2(Cfg) or else layout 1 to 4
byte LayoutPrev = 1;                   // Save Layout temporarily while using Macro functions
byte LayerAD = 0;                      // Switch [Layout A B C D] for [Layers 1 3 4] A[M1-M6] B[M7-M12] C[M13-M18] D[M19-M24]
char LayerADLetter[2] = " ";           // LayerAD = 0-3 = A-D [1] = 65+LayerAD
bool LayerAxD = false;                 // Toggle by pressing Pad M, S, T on same page M, S, T  - then use SDCard files or Coded actions
bool LayerAxDSave = false;             // Save and Restore in DoLink()
bool LayerADChange = false;            // If changes A-D

int  LCDBackLight = 13;                // Set as OUTPUT and HIGH by TFT init or Brightness level via analog PWM
bool BLOnOff = true;                   // If Option BLOnOff is ON then pressing the black button switch backlight off
bool BLOnOffToggle = false;            // Switch Backlight ON/OFF without leaving Powerscreen
bool BackLightOn = true;               // LCD Backlight Saver and used to activate dimmed LCD                                   
int  DimVal  = 3;                      // 0 -> Backlight 0=OFF   1-99 dimmed LCD brightness use *db*0-9 or *db*00-99
int  NormVal = 0;                      // 0 -> Backlight 100=ON  1-99 normal LCD brightness use *bb*0-9 or *bb*00-99
char TimerArr[11] = {"          "};    // Hold the last WriteTimers value as ASCII number 10 digits max in millisec = 25 hours
char timeU = 's';                      // s seconds m minutes h hours

char TimerVal[8][11] = {"","","","","","","",""};                                       // DimData Timer value as string
unsigned long TimePeriod = 300000;                                                      // LCD Backlight Timer *tb*n,nn
unsigned long DimData[9] = { 30000,30000,3000000,30000,3000000,9000,90000,9000,90000 }; // *tb* 2xtimePowerOff 2xtimeRestart 2xtimeOnceof 2xtimeRepeat 
unsigned long timePowerOff, TimePowerOff, timeRestart, TimeRestart;                     // Initialized with ReadTimers(1)
unsigned long timeOnceof, TimeOnceof, timeRepeat, TimeRepeat;                           // 
unsigned long timeOnceofPrev=0, TimeOnceofPrev=0, timeRepeatPrev=0, TimeRepeatPrev=0;

//                      012345678901234567890
char BlankArr[22]    = "LCD Blank Time      s";
char DimArr[]        = "LCD Blank Dim       %";
char NormArr[]       = "LCD Brightness      %";
char mTArr[]         = "Oneshot Time               ";
char rTArr[]         = "Repeat Time                ";

///////////////////////////////////////// volatile makes TouchPad freeze if Caplock pressed
//volatile bool CapsLock = false;        // Green "C"
//volatile bool NumLock = false;         // Green "N"
//volatile bool ScrollLock = false;      // Green "S" 
//volatile bool Change = false;          // Indicators changed
bool CapsLock = false;        // Green "C"
bool NumLock = false;         // Green "N"
bool ScrollLock = false;      // Green "S" 
bool Change = false;          // Indicators changed
bool BusyCNS = false;         // Lock display until changes complete

bool MacroUL = false;                  // Upper/Lower case Macrokeys M S T N filenames (Only matters on Flash not SDCard)
byte ConfigKeyCount = 0;               // Start with Layout 2 Cfg key not pressed 
bool MuteOn = false;                   // Disable VolMute with Layout 1 2 3 4 select Default is L1-L4
bool VolOn = true;                     // Disable Vol+ Vol- then keys are Del/BSpc Enter VolOn1-VolOn3 Volume also on in Layouts 1,3,4
bool MediaChange = false;              // Change in Volume, Mute or Meduakeys that require saving to Flash
bool Media = false;                    // Activate 4 of possible 11 Media Keys in Layout 2
bool MouseK = false;                   // 12 Mouse keys active via Pad option           
bool MediaPrev = false;                // Restore 4 Media Keys in Layout 2
bool ToneControls = false;             // Bass Treble on Media Keyboard
bool Brightness = false;               // *br* = brightness up/dwn in M S T replace volume V+/V-
int8_t MouseScrollAmount = 1;          // Scroll Amount (if - then Down if + Up
int8_t MouseDelta = 5;                 // Mouse Cursor Movement Amount in pixels
byte MouseConfig[2] = { 1, 5 };        // Mouse Config
bool MediaCfg = false;                 // Set later = to MediaConfig[0]
byte MediaConfig[1] = { 0 };           // Values are 0 - 4 for Media+Volume+Mute+Tone combinations 
byte KeyHeld = 0;                      // Key held in number 0 - 17
bool KeyHeldEnable = true;             // Enable/Disable Volume Mute Processing if [Vo][L1-L4 key is pressed long
byte KeyRepeat = 6;                    // Duration before Key Repeat is active in KeyRepeatx100 milliseconds
byte KeyHeldLayout = 0;                // Save Layout temporarily while checking KeyHeld
bool SaveVar = false;                  // Save if any [Key] key changes
byte VarNum = 0 ;                      // [Key] 0 = not pressed 1,2,3-5,6-8 1[Del]2[Ret] 3-5 Mkeys 6-8 ST keys changes with bottom Pad (o)
bool Bank123Change = false;            // If true save Bank123 in GetSysInfo()                              
byte OptNum = 0;                       // [Opt] 0 = not pressed 1 = toggle KeyHeldEnable 2 = Select StartUpLayout 1-4 2 = MacroBank123

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// [Del] [Ret] Available in Layout 1,2,3,4    [Cut][Cpy][Pst] Available in Layout 1   [cX][cC][cV] Available in Layout 3,4
const static char VarStatus[9][31] = {"", /* 
 0123456789012345678901234567890123456   0123456789012345678901234567890123456    0123456789012345678901234567890123456  */                                                                                          
"[ Delete ] use Pad (o)",               "[ Return ] use Pad (o)",                                                   // Layout 1,2,3,4
"[ Cut ] L1 Key1 use Pad (o)",          "[ Copy ] L1 Key2 use Pad (o)",          "[ Paste ] L1 Key3 use Pad (o)",   // Layout 1 
"[ cX ] L2 L3 Key1 use Pad (o)",        "[ cC ] L2 L3 Key2 use Pad (o)",         "[ cV ] L2 L3 Key3 use Pad (o)" }; // Layout 3,4
const static char OptStatus[13][38] = {"",                                                                          // OptStatus[1]-[9] used
"Select nKeys Character use Pad (o)",   "VolMute long-press OnOff use Pad (o)",  "L1 - L4 Startup Select use Pad (o)", 
"[ M ] Keys MacroBank 1-5 use Pad (o)", "[ S ] Keys MacroBank 1-5 use Pad (o)",  "[ T ] Keys MacroBank 1-5 use Pad (o)",
"Macro U/L Case Filenames use Pad (o)", "Select SDCard FileSet use Pad (o)",     "File List SDCard + Flash use Pad (o)",
"[ M ] Custom Label press Pad (o)",     "[ S ] Custom Label press Pad (o)",      "[ T ] Custom Label press Pad (o)"   };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Use [Cfg][Sav] with serial monitor to read 5 number current calibration data (last line printed)
// To get new calibration data: 
// 1. Send the macro *ca* with the built-in keyboard to toggle re-calibration option to ON
// 2. Restart or Reset the macro pad
// 3. Do the calibration - press at the TIP of the arrow
// 4. Send the macro *ca* with the built-in keyboard to toggle re-calibration option to OFF
// Press [Del]ete in [Cfg] options - next power-up will do a re-calibration
// Typical Calibration Data: 277, 3636, 279, 3501, 5 for type C 125MHz LCD
// Note if rotate 180 was used: New version uf2 files will use rotate=normal not 180
// This means LCD will freeze if rotate180 is off (in Config1) and the calibration data is for rotate=180
// Use flashnuke.uf2 to reset all Flash memory data to zero and then load the new uf2 and do the rotate 180 and re-calibration
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cSt uint16_t calData0[5]   = {277, 3636, 279, 3501, 5};  // rotate = 0
cSt uint16_t calData180[5] = {297, 3572, 288, 3476, 3};  // rotate = 180     
uint16_t calData[5];                                     // size is 5 x 2bytes = 10 bytes                         
uint8_t  calDataOK = 0;
#define CalFile "TouchCalData"  // Name of the Touch calibration file
bool DoCal = false;             // Set true if you want the calibration to run on each boot 

uint16_t t_x = 0, t_y = 0;      // Touch coordinates
/////////////////////////////////////////////////////////////////////////////////////////
// Layout 1 to 4 Keys  480x320
/////////////////////////////////////////////////////////////////////////////////////////
// Key sizes and spacing
#define KEY_W 98            // Key width  98 x 4 = 392 +  9 x 4 = 428  
#define KEY_H 80            // Key height 80 x 3 = 240 + 10 x 4 = 220
#define KEY_SPACING_X 9     // 9 X gap
#define KEY_SPACING_Y 10    // Y gap
#define KEY_X 52            // 30 is off-screen on left
#define KEY_Y 50            // 30 is the topmost with no cut-off

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Layout 5 small Config Buttons s k m n o (SpecialCharMath MacroKeybrd Mouse nKeysNumpad OptionCapsNum)            
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PAD_W 42          //  Key width  PAD_X is middle 
#define PAD_H 44          //  Key height
#define PAD_SPACING_Y 53  //  Strange but true
#define PAD_X 456         //  config buttons right side
#define PAD_Y 34          //  config buttons top
////////////////////////////////////////////////////////////////////////////
// Indicators status Caps-Num-Scroll C N S
////////////////////////////////////////////////////////////////////////////
#define IND_X1 105        //  Centred on this
#define IND_X2 212   
#define IND_X3 320    
#define IND_Y  175   
////////////////////////////////////////////////////////////////////////////
// Macro Src NN Dst NN Indicators 
////////////////////////////////////////////////////////////////////////////
#define MST_X1 106  
#define MST_X2 212  
#define MST_X3 318  
#define MST_Y  275 
////////////////////////////////////////////////////////////////////////////
// Options Indicators status D T L/W (Direct Mode, Timer, Linux/Windows)
////////////////////////////////////////////////////////////////////////////
#define OPT_X0 6    // LED LEFT
#define OPT_X1 106    
#define OPT_X2 212  
#define OPT_X3 320  
#define OPT_X4 420  // LED RIGHT
#define OPT_Y  86    
////////////////////////////////////////////////////////////////////////////
// Message status line at bottom
////////////////////////////////////////////////////////////////////////////
#define STATUS_X 220 // Centred on this - not middle buttons are off-centre
#define STATUS_Y 296 // 305 bottom of y p etc cut-off

#define KEY_TEXTSIZE 1   // Font size multiplier

// Delay between keypress to give USB HID time to respond
#define  dt50  50       // keycode[1,..,6] delay milliseconds
#define  dt25  25       // keyPress delay
#define  dt200 200      // keyPress delay for Logoff Poweroff Restart
#define  dt100 100      // keyPress delay for Logoff Poweroff Restart
#define  dt500 500      // used in UAC confirm

// Create the screen object
TFT_eSPI tft = TFT_eSPI();

#define ArraySz(x) (sizeof(x)/sizeof((x)[0]))  
#define cSt const static  // line space needed
#define SerPr1 Serial.print(" ")
#define SerPr2 Serial.println()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// if MLabel SLabel TLabel > 0 use 5-char max labels in files MLabel SLabel TLabel instead of M,S,T 1 - M,S,T 24
// 
char keyLabel[12][6] = {""};               //  5 characters max n0000 - n9999

// Custom Label up to five characters for 24 M,S,T keys used if MLabel 1 SLabel 1 TLabel 1
// Then read contents of the filename in file LabelM,S,T into LabelArrM,S,T 
// Need fast access to this - cannot read from SDCard every time
char LabelArr[3][24][6];      // Size 144*3=432 LabelArr[0] M LabelArr[1] S LabelArr[2] T custom keys
bool MLabel, SLabel, TLabel;  // if>0 then custom labels for 24 M,S.T keys
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  If Cfg Key pressed new options (new Labels and new Colours)      
/////////////////////////////////////////////////////////////////////////////////////////////////
const static char CfgLabel[12][4] = 
{"Opt", "Key",  "Vol", "   ", "A-D",  "Sav", "Vm",   "   ",  "mCT", "ROf", "Med", "   "};  // All 3 chars + \0  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  If Media Controls Arrow Right Key pressed new options in different colour:                                        
//                  "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn",  "PgD",  "V-/Ret"
// MediaLabel[x][]  "Stp", "   ", "   ", "   ",    "Prv",  "   ",  "Nxt", "   ",     "P-P", "   ",  "   ",  "   " 
// MediaLabel2[x][] "|||", "B+",  "T+",  "   ",    "|<",   "   ",  ">|",  "   ",     ">||", "B-",   "T-",  "   " 
// keyLabel2[x][]     0     1      2                4               6                 8      9       10
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// const static char MediaLabel[12][4] = 
// {"Stp", "   ", "   ", "   ",    "Prv",  "   ",  "Nxt", "   ",  "P-P", "   ",  "   ", "   "}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MediaLabel[12][4] =                                                    // With Up/Dwn and PageUp/PageDwn
{   "|||", "   ", "   ", "   ",    "|<",   "   ",  ">|",  "   ",  ">||", "   ",  "   ", "   "}; 
const static char MediaLabel2[12][4] =                                                   // Tone Controls Bass Treble Up/Dwn
{   "|||", "B+",  "T+",  "   ",    "|<",   "   ",  ">|",  "   ",  ">||", " B-",   "T-",  "   "}; 
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  If Numeric Keys selected via KeyBrd command *kb* new options:
//                 "Hme", "Up", "PgU", "DelV+", "<",   "Cfg",  ">",  "V0L14", "End", "Dwn", "PgD", "V-Ret"
// Numkeys1[x][]   "BsD", "7",  "8",   "9",     "Ret",  "4",    "5",  "6",     "0",   "1",   "2",    "3"
// nKeys[x][]       n01    n02   n03    n04      n05     n06     n07   n08      n09    n10    n11     n12                                        
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int nKeySize = 200;            // Indirection i.e. n01-n996 keys only holds [/folder][/]filename[.ext] of target file
char nFile[nKeySize] = "1234567890"; // Name of current nKey content i.e. [path]filename  
char MacroChar[11] = {"stmakSTMAK"}; // These are handled as macros/strings not file paths
bool nKeys = true;                   // Numpad Pages or Macro keys n01 - n996 change char n with *0n*char 
int  nKeysPage = 12;                 // Number of nKeys pages change with *0p*pages where pages = 1-83
const int NumKeysPageMax = 8;        // Change Config1[9] as well - normally 8 = max pages NumPad
char nChar = 'n';                    // Current nKeys first char (on keys and filename) can be 0-9, a-z, A-Z change with *0n*char 
char nDir[22] = "/";                 // Default is nKeys in root folder /nKeysfile, if dirname must start AND end with "/" for strcat filename 
byte nDirX = 0;                      // 0 = Not changed 1 = Default "/"  2 = /dirname/  3 = /nChar/ 
char nDirXMsg[4][30] = {"Use /0-20-char dir-name or //", "nKeys default dir /", "nKeys special dir /x/", "nKeys dir changed" };
bool NumKeys = false;                // Numeric Keyboard
bool nKeysShow = false;              // Pressing Pad[s] toggles thsi, if on pressing nKey displays its content not execute it - not saved
bool nKeys34 = true;                 // nKeys count n00-n99,n100-n996 if true (default), else n000-n996 use different n01-n99 files n001-n099
char nKeysCharSet[10] = {'n','o','p','q','r','s','t','m','a','k'};  // preferred list of 10 nKeys first char
char nKeysAllChar[63] = {"abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};  // List of all NKeys first char
char nKeysLnkChar[10] = {'n','o','p','q','u','v','w','x','y','z'};  // DoLinkString do these as nKeys not macros Xnn = 00-99 exclude rR dD lL  
int  nCharNum = 0;                   // Current position into nKeysCharSet[nCharNum] when pressing Option (o) Pad
int  nCharAll = 0;                   // Current position into nKeysAllChar[nCharAll] when [s] active and pressing Pads (+)(-)
char nKeysCurr[] = {"x is current nKeys char"}; 
byte CRLF = 0;                       // Filter CR 0D \n and LF 0A \r in large text files. 1=Filter CR=0x0D 2=Filter LF=0x0A 3=Filter both CR and LF
byte crlf1 = 0x0D, crlf2 = 0x0A;     // Two Nkeys text filter chars default = 0x0D 0x0A 

char NumkeysX[12][6] =  { 0 };
const static char NumkeysLabel[NumKeysPageMax][12][6] = 
{"BsD", "7",  "8",   "9",   "Ret",  "4",  "5",  "6",  "0",  "1",  "2",  "3",
 "BsD", " ",  "%",   "\\",  "Ret",  ",",  ".",  "=",  "+",  "-",  "*",  "/",
 "BsD", "aA", "bB",  "cC",  "Ret",  "dD", "eE", "fF", "gG", "hH", "iI", "jJ",
 "BsD", "kK", "lL",  "mM",  "Ret",  "nN", "oO", "pP", "qQ", "rR", "sS", "tT",
 "BsD", "uU", "vV",  "wW",  "Ret",  "xX", "yY", "zZ", "(",  ")",  "[",  "]",
 "BsD", "{",  "}",   "<",   "Ret",  ">",  "!",  "@",  "# ", "$ ", "^",  "&",  
 "BsD", "\"", "?",   ":",   "Ret",  ";",  "~",  "`",  "'",  "  ", "  ", "  ",
 "BsD", "Esc","Tab", "Del", "Ret",  "PrS","Hme","End","PgU","PgD","Pse","Ins" }; 
//////////////////////////////////////////////////////////////////////////////////////////////////
// nxx files contains the name of a file to be executed with filename length <30 
// filename can include / char => /paintmacro/paint001 will execute file in folder /paintmacro
// If file name in nxx ends with Link it is a linkfile (list of files, repeats, delays etc.)
// => -/paintmacro/paintlink001.03 will execute file paintlink001.03 in folder /paintmacro
// Change the first char (default n) with *0n*char where char = 0-9, aA-zZ
// Change the number of pages (default is max = 8) with *0p*pages where pages = 1-8
//////////////////////////////////////////////////////////////////////////////////////////////////
char NKeysX[12][6]  =  { 0x00 }; // "n01" - "n96" 
                                 // "n0001" - "n9984" is 832 pages 12 keys each 104 times more than 96 key-sets
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//  If Mouse Keys selected via KeyBrd command *mu* new options:
//                        "Hme", "Up",  "PgU", "DelV+", "<",   "Cfg",  ">",   "V0L14", "End", "Dwn", "PgD", "V-Ret"
// MouseKeysLabel[x][]    "LBt", "MUp", "RBt", "Sc+",   "<-M", "MBt",  "M->", "L2x",   "Bck", "MDn", "Fwd", "Sc-"                                           
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                
const static char MouseKeysLabel[12][4] =             
{"LBt", "Up", "RBt", "Sc+",   "<", "MBt",  ">", "L2x",  "Bck", "Dwn", "Fwd", "Sc-"}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char StartUpLayout[5][18] =             
{"L2 Startup", "L1 Startup M1-M24", "L2 Startup Config", "L3 Startup S1-S24", "L4 Startup T1-T24" }; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char KbrdLabel[5][12][4] =                                                    // Xlate only needed for kcode not Keypress
{"abc",  "def",  "ghi",  "EXE", "jkl", "mno", "pqr", "NXT", "stu", "vwx", "yz ", "ADD",    // ASCII->HID Subtract  dec
 "ABC",  "DEF",  "GHI",  "exe", "JKL", "MNO", "PQR", "nxt", "STU", "VWX", "YZ_", "add",    // ASCII->HID Subtract 61 dec
 "012",  "345",  "678",  "EXE", "9+-", "*=/", "*Cm", "NXT", "Sym", "Brc", "Fnn", "ADD",    // 1-9 subtract 19 dec 0 subtract 9 dec
 "Lst",  "Ren",  "Rmv",  "Snd", "Tmr", "Cpy", "Lnk", "NXT", "Src", "Dst", "Num", "Sav",    // *Cm=StarCodes Lst=MacroContent Rmv=RemoveMacro
 "ALT",  "SHF",  "CTR",  "EXE", "GUI", "TEI", "CRF", "NXT", "LHR", "UED", "UND", "ADD"};   // Mainly modifiers                                                                                      
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char KeyBrdSpc[3][12][4] = 
{"ALT",  "SHF",  "CTR",  "   ", "GUI", "Tab", "C/R", "  ", "A-L", "A-U", "PgU", "ADD",
 "A-R",  "S-R",  "C-R",  "   ", "G-R", "Esc", "L/F", "  ", "Hme", "End", "PgD", "ADD",
 "PrS",  "Del",  "BSp",  "   ", "Nul", "Ins", "RET", "  ", "A-R", "A-D", "Bsp", "ADD"};
const static byte KeyBrdNum[3][12] = 
{AltL,   ShfL,   CtrL,    0,    WinL,  Tab,   0x0D,  0, ArrLeft,  ArrUp,  PageUp, 0,
 AltR,   ShfR,   CtrR,    0,    WinR,  Esc,   0x0A,  0, KeyHome,  KeyEnd, PageDwn,0,
 PScr,   DelK,   BckS,    0,    0x00,  Ins,   0x28,  0, ArrRight, ArrDown,NmL,    0};
const static byte KeyBrdFx[24] =   // F01 to F24 must use keycode keyPress 0x3C = < not F3 - else put a modifier before Fx such as Shift + F3 
{F01, F02, F03, F04, F05, F06, F07, F08, 
 F09, F10, F11, F12, F13, F14, F15, F16,               
 F17, F18, F19, F20, F21, F22, F23, F24 };
const static char KeyBrdFxChr[24][4] = // F01 to F24
{"F01", "F02", "F03", "F04", "F05", "F06", "F07", "F08",
 "F09", "F10", "F11", "F12", "F13", "F14", "F15", "F16",
 "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24" };
const static char KeyBrdBrackets[8][2] =    
{ "[", "]", "(", ")", "{", "}", "<", ">" };
const static char KeyPadNum[10][2] =                  // not implemented use *kp* to toggle both numpad and keybrd numbers
{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"  };
const static byte KeyPadVal[10][2] =    
{ 0x62,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61 };
const static char KeyBrdSymbols[17][2] =    
{ "@", "#", "?", "$", "%", "^", "&", ",", ".", "~", "`", "\\", "\'", "\"", "|", ":", ";" };
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CmKey = false;                 // Check if *codes are from pressing [*Cm] key or entered directly
const static int StarCodesMax = 92; // StarCodes Count 16+16+16+16+16+12 StarNum = 0-91
const static char StarCode[StarCodesMax][5] =    
{ "*ad*", "*ae*", "*am*", "*as*", "*at*", "*bb*", "*bl*", "*br*", "*ca*", "*cf*", "*cm*", "*cr*", "*ct*", "*cx*", "*db*", "*de*", 
  "*df*", "*e0*", "*e1*", "*e2*", "*e3*", "*e4*", "*fa*", "*fc*", "*fm*", "*fo*", "*fs*", "*ft*", "*im*", "*is*", "*it*", "*ix*", 
  "*kb*", "*ke*", "*kr*", "*ks*", "*ld*", "*lf*", "*lm*", "*ls*", "*lt*", "*lx*", "*m1*", "*m2*", "*mt*", "*mT*", "*nt*", "*nT*", 
  "*os*", "*ot*", "*oT*", "*po*", "*r0*", "*r1*", "*r2*", "*r3*", "*rn*", "*ro*", "*rt*", "*rT*", "*sa*", "*sd*", "*se*", "*sm*", 
  "*ss*", "*st*", "*ta*", "*tb*", "*tp*", "*tt*", "*tw*", "*ua*", "*ul*", "*up*", "*x0*", "*x1*", "*x2*", "*x3*", "*x4*", "*x5*", 
  "*x6*", "*x7*", "*x8*", "*x9*", "*0R*", "*09*", "*0d*", "*0n*", "*0p*", "*0s*", "*0t*", "*0x*"  };

const static byte StarCodeType[StarCodesMax] =    
{ 57,     59,     1,      1,      1,      2,      36,     5,      6,      56,     7,      50,     8,      51,     3,      9,      
  17,     10,     10,     10,     10,     10,     11,     12,     11,     13,     11,     11,     44,     44,     44,     44,     
  14,     39,     38,     15,     16,     42,     55,     55,     55,     58,     18,     19,     20,     20,     21,     21,     
  22,     23,     23,     25,     37,     26,     40,     41,     49,     27,     24,     24,     28,     29,     30,     28,     
  28,     28,     31,      4,     31,     31,     31,     33,     32,     43,     35,     35,     35,     35,     35,     35,     
  35,     35,     35,     35,     34,     45,     53,     46,     47,     48,     54,     52      };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 5 Small Config Buttons between 1 st and 3rd row Red Blue Green SkyBlue Gold - if MacroUL=1 then o->O m s t -> M S T
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char padLabel[20][2]      = {"a", "m", "s", "t", "d", "s", "k", "m", "n", "o", "a", "m", "s", "t", "d", "a", "m", "s", "t", "d"}; 
char padNKeysLabel[3][5][2] = {"s", "k", "m", "n",   "o", "+", "k", "s", "n", "-",   "+", "k", "S", "n", "-"} ; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NumPad Resistor Colour Code // https://eepower.com/resistor-guide/resistor-standards-and-codes/resistor-color-code/#
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte     RXlat[12]   = {0,     7,     8,   9,       0,       4,      5,    6,      0,      1,      2,    3      };  // NumPad keys
//       RColour[12] = {Black, Brown1,Red, Orange,  Yellow4, DGreen, Blue, Purple, Black1, LGrey,  Gold, Silver };  // 4 band - 3 band colour
float    RVal[12]    = {0,     1,     2,   3,       4,       5,      6,    7,      8,      9,      1/10,  1/100 }; 
bool Resistor = true;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Power Restart [Restart  Long Time] [Abort ] [PowerOff  Long Time] [Restart  Clock]   [R-T] [Stp] [O-T] [R-C]   
//               [Restart Short Time] [  Cfg ] [PowerOff Short Time] [PowerOff Clock]   [R-t] [Cfg] [O-t] [O-C]       
//               [Restart  Immediate] [Logoff] [PowerOff   Imediate]                    [Rst] [Log] [Off] [   ]
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char PowerKeysLabel[12][4] = {"R-T",  "Stp",  "O-T", "R-C",
                                           "R-t",  "Cfg",  "O-t", "O-C",
                                           "Rst",  "Log",  "Off", "   " };                                           

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static byte PadCapsNum[2][4]     = { 0x53, 0x39, 0x53, 0x53,    // not 0x00 as 1st byte in keycode[]
                                           0x39, 0x00, 0x39, 0x00 };  // Capslock Numlock
int PadCapsNumN = 0 ;                                                 // 0-3 Index into PadCapsNum[0-1][0-3]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro Timer KeyPad [Repeat Long  Time][  Stop    ][Once Long  Time]    [R-T] [Stp] [O-T] [OcT ]   
//                    [Repeat Short Time][  Cfg     ][Once Short Time]    [R-t] [Cfg] [O-t] [ ]       
//                    [Repeat Clock Time][Repeat x N][Once Clock Time]    [R-C] [Rxx] [O-C] [ ]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MacroTimerLabel[12][4] =   {"R-T",  "Stp",  "O-T", "RcT",
                                              "R-t",  "Cfg",  "O-t", "OcT",
                                              "R-C",  "Rep",  "O-C", "   " };

char TimerDisp[23] = {"Macro X nn Timer   R-T"};
int  XLat1[6]      = {'K','M','A','S','T','k'};
char XLat2[24][3]  = {"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24" };
char XLat3[8][4]   = {"R-T","R-t","O-T","O-t","R-C","O-C","RcT","OcT" };

bool MacroTimerOK = false;                                          
                                                                                    
const static long unsigned int mtArray[10] = {10800000,  30000,   60000,    90000,   120000, 180000, 300000, 600000, 1800000, 3600000 };
const static char mtArrayStr[10][10]       = {"3 hrs",  "30 sec","60 sec","90 sec", "2 min","3 min","5 min","10 min","30 min","1 hrs" };
const static long unsigned int mTArray[10] = {10800000,  7200000, 10800000, 18000000, 21600000, 36000000, 43200000, 86400000, 172800000, 604800000 };
const static char mTArrayStr[10][10]       = {"300 hrs", "2 hrs", "3 hrs",   "5 hrs",  "6 hrs", "10 hrs", "12 hrs",  "24 hrs", "48 hrs",  "1 week" };                     
                         
const static char hex16[16][2] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"}; 
static const char* digits = "0123456789ABCDEF";       // dec 2 hex digits positional map
char Int2HexStr[64] = "  ";
                        
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
int  KeyBrdX = 2;                // values 0 1 2 3 4 - Should start on Page 3 or KeyBrdX = 2 = *Cm Page
byte KeyBrdByte[200]= ""; // ={};// Hold values to be sent
byte KeyCode[200];               // Parallel keycodes
int  KeyBrd123 = 0;              // Such as Key [abc] = a or b or c - values 0,1,2
int  KeyBrdF1F24 = 0;            // Single key cycles through all 24 Fnn keys
int  KeyBrdByteNum = 0;          // Size KeyBrdByte
int  KPNum = 0;                  // KeyPad Numbers 0,1 - 9 = 0x62,0x59 - 0x61 
int  BracketsNum = 0;            // Single key cycles through all 8 Brackets symbols
int  SymbolsNum = 0;             // Single key cycles through all 14 symbols
int  StarNum = 0;                // Single key cycles through all Starcodes such as *ca*
int  PrevButton = 0;             // Used in case-button to check for double key press
int  Numkeys123 = 0;             // Numkeys1 to Numkeys3 - 3 chars for each key such as 3 4 5
char KBDisp[200]= "";   //  ={}; // Keybrd display macro buffer in status line
int  KBDispPos = 0;              // nect char position
bool KBType = false;             // If 1st byte>0x7F use keycode[0-5] else use keyPress
bool KBrdActive = false;         // Let [ADD] know any char key has been pressed at least once before
bool KeyBrdDirect = false;       // Echo Keyboard keys to PC as they are typed
bool DoUpKey = false;            // Set true by SendBytes to let Upkey know valid macro can be saved
bool KeySkip = true;             // Skip first key pressed in LCD in dimmed state - used to wake LCD - toggle with *ks*
bool LinkOk = false;             // Used in DoKeyMST() DoKey16() DoLinkStr() test if linkfile executed ok

char MSTAName[40]  = "xnn";      // 26 if nDir added to MSTA filename usually a01-a99 m01-m24 s01-s24 t01-t24 from Option1 = 0-23 Option2 = 0-23                             
char MSTName[40]   = "xnn";      // 26 if nDir added to MST filename usually m01-m24 s01-s24 t01-t24 from Layout = 1-4 
char MSTLinkName[] = "XnnLink";  // Filenames M01Link, S24Link T09Link, K01-K24Link  Layout = 1-4 and 5 for K
char NameStr1[40]  = { "" };     // Used in: DoNKey() ListMacro() RemoveMacro() DoLinkStr() DoKeyMST() DoKey16() - NameStr1
char NameStr2[40]  = { "" };     // Used in: CopyMacro() RenameMacro() - NameStr1 + NameStr2
char NameStr3[240] = { "" };     // Used in: DoNKeys must handle nDir + NKeys filename = 20 + 200

bool PadKeys = false;            // 5 Small Config Buttons on RH side Red Blue Green
bool KeyFontBold = false;        // Button Font Bold/Normal labels
bool KeyFontColour = false;      // FontColour is White/Black
int  KeyFont = White;            // Colour White or Black
bool macroA = false;             // If 1 m s t macro also saved a as 0 + 24 + 48 - change macroA with *Code not yet implemented
int  Option1 = 0;                // Source Macros number 0-23 and 0-99
int  Option2 = 0;                // Destination Macros number 0-23 and 0-99
int  MST1 = 3;                   // Source Macro MST1 = 0 1 2 3 4  = M S T  - macro copy + link
int  MST2 = 3;                   // Destination Macro MST2 = 0 1 2 3 4  = M S T A K N - macro copy + link
int  NumMode = 1;                // Used to indicate that [Num] key should define Source (1) or Destination (2) macro
byte SrcDst = 0;                 // Indicate Src/Dest SDCard/Flash=Orange/White SrcDst=0=00=W->W =1=01=W->O =2=10=O->W =3=11=O->O

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 = not assigned, or executed in MacroEditor in SendBytes, 2 = assigned in FillKeysStr, 1 = Saved DoUpKey via ArrowUp
// 3 = assigned in CopyMacro SaveMacro DoNewData  4 = when read in InitCfg, 5 = , 6 = SaveMacro
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte MacroM1M12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                             
byte MacroS1S12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte MacroT1T12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 

// Byte size of macro of assigned + saved macro
int  MacroSizeM1M12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
int  MacroSizeS1S12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
int  MacroSizeT1T12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 

char TimerName[8][8] = {"","","","","","","",""};    // MacroTimer name Xnn or XnnLink

bool PowerKeys = false;          // Bottom row on Cfg Layout 3 Logoff Off Restart
bool PowerKeysMenu = false;      // If true use GUI+X+u,i,r instead of GUI+R+shutdown - change with *po*
int  PowerClock = 0;             // 1 or 2 = Restart on PowerOff on Clock Time callback for Time triggered

bool MacroTimerDisp = false;        // Display Timer Keys if ON 
bool MacroTimer18 = false;          // One of the Timers1-4 has been set Once-of or Repeat Macro by time or by clock (need PicoW?)
bool MacroTimer1  = false;          // Repeat Time
bool Macrotimer2  = false;          // Repeat time
bool MacroTimer3  = false;          // Onceof Time
bool Macrotimer4  = false;          // Onceof time
bool MacroTimer5  = false;          // Repeat Clock Time
bool MacroTimer6  = false;          // Onceof Clock time
bool MacroTimer7  = false;          // Repeat Countdown Clock Time
bool MacroTimer8  = false;          // Onceof Countdown Clock time
int  MacroRepeat  = 1;              // Repeat Onceof x times
byte MacroTimerArr1[4] = {0,0,0,0}; // Layout = 1,3,4 A K  Key 1-24 1-99  0 1 Flash SDCard  0 1 Xnn XnnLink
byte MacroTimerArr2[4] = {0,0,0,0};   
byte MacroTimerArr3[4] = {0,0,0,0};  
byte MacroTimerArr4[4] = {0,0,0,0};  
byte MacroTimerArr5[4] = {0,0,0,0};  
byte MacroTimerArr6[4] = {0,0,0,0};  
byte MacroTimerArr7[4] = {0,0,0,0};  
byte MacroTimerArr8[4] = {0,0,0,0};  

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if [Cfg] then [Home][UpArrow][PageUp]  [<][>]  [End][DwnArrow][PageDown] keys pressed
//            StrButton[button]     0  1  2  3  4  5  6  7  8  9  10 11          // 3 7 8 = V+ Vo V-
//                        CfgOK     x  x  x     x  x  x     x  x  x              // Not used with
static const byte CfgButton[12] = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };        // Cfg key
//////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////  ###
//                StrButton[button]             4  5  6     8  9  10                 4  5  6      8  9  10
//  M1-M6 M7-M12 etc            c               0  1  2     3  4  5                  6  7  8      9  10 11
static const byte StrButton[48] = { 0, 0, 0, 0, 0, 1, 2, 0, 3, 4, 5,  0, 0, 0, 0, 0, 6, 7, 8,  0, 9, 10,11,0, 
                                    0, 0, 0, 0, 12,13,14,0, 15,16,17, 0, 0, 0, 0, 0, 18,19,20, 0, 21,22,23, 0  };
                                    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char * str1to12[] = {str1, str2, str3, str4, str5, str6, str7, str8, str9, str10,str11,str12,
                     str13,str14,str15,str16,str17,str18,str19,str20,str21,str22,str23,str24} ; // S keys
                     
char * ttr1to12[] = {ttr1, ttr2, ttr3, ttr4, ttr5, ttr6, ttr7, ttr8, ttr9, ttr10,ttr11,ttr12,
                     ttr13,ttr14,ttr15,ttr16,ttr17,ttr18,ttr19,ttr20,ttr21,ttr22,ttr23,ttr24} ; // T keys
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int  StrSize =  200;       // Check if not byte used if made larger 
const int  ByteSize = 200;       // 
const byte MaxBytes = StrSize;   // 200 * 24 * 4 = 19.2 kbytes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte Str1[ByteSize],   Str2[ByteSize],   Str3[ByteSize],   Str4[ByteSize],   Str5[ByteSize],   Str6[ByteSize];
byte Str7[ByteSize],   Str8[ByteSize],   Str9[ByteSize],   Str10[ByteSize],  Str11[ByteSize],  Str12[ByteSize];
byte Str13[ByteSize],  Str14[ByteSize],  Str15[ByteSize],  Str16[ByteSize],  Str17[ByteSize],  Str18[ByteSize];
byte Str19[ByteSize],  Str20[ByteSize],  Str21[ByteSize],  Str22[ByteSize],  Str23[ByteSize],  Str24[ByteSize];
byte Ttr1[ByteSize],   Ttr2[ByteSize],   Ttr3[ByteSize],   Ttr4[ByteSize],   Ttr5[ByteSize],   Ttr6[ByteSize];
byte Ttr7[ByteSize],   Ttr8[ByteSize],   Ttr9[ByteSize],   Ttr10[ByteSize],  Ttr11[ByteSize],  Ttr12[ByteSize];
byte Ttr13[ByteSize],  Ttr14[ByteSize],  Ttr15[ByteSize],  Ttr16[ByteSize],  Ttr17[ByteSize],  Ttr18[ByteSize];
byte Ttr19[ByteSize],  Ttr20[ByteSize],  Ttr21[ByteSize],  Ttr22[ByteSize],  Ttr23[ByteSize],  Ttr24[ByteSize];
byte Mtr1[ByteSize],   Mtr2[ByteSize],   Mtr3[ByteSize],   Mtr4[ByteSize],   Mtr5[ByteSize],   Mtr6[ByteSize];
byte Mtr7[ByteSize],   Mtr8[ByteSize],   Mtr9[ByteSize],   Mtr10[ByteSize],  Mtr11[ByteSize],  Mtr12[ByteSize]; 
byte Mtr13[ByteSize],  Mtr14[ByteSize],  Mtr15[ByteSize],  Mtr16[ByteSize],  Mtr17[ByteSize],  Mtr18[ByteSize];
byte Mtr19[ByteSize],  Mtr20[ByteSize],  Mtr21[ByteSize],  Mtr22[ByteSize],  Mtr23[ByteSize],  Mtr24[ByteSize];

byte MacroBuff[ByteSize] = { 0 };
int  MacroBuffSize = ByteSize;
bool LargeFile = false;

byte * Str1to12[] = {Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,Str11,Str12,
                     Str13,Str14,Str15,Str16,Str17,Str18,Str19,Str20,Str21,Str22,Str23,Str24} ;   
byte * Ttr1to12[] = {Ttr1,Ttr2,Ttr3,Ttr4,Ttr5,Ttr6,Ttr7,Ttr8,Ttr9,Ttr10,Ttr11,Ttr12,
                     Ttr13,Ttr14,Ttr15,Ttr16,Ttr17,Ttr18,Ttr19,Ttr20,Ttr21,Ttr22,Ttr23,Ttr24} ;
byte * Mtr1to12[] = {Mtr1,Mtr2,Mtr3,Mtr4,Mtr5,Mtr6,Mtr7,Mtr8,Mtr9,Mtr10,Mtr11,Mtr12,
                     Mtr13,Mtr14,Mtr15,Mtr16,Mtr17,Mtr18,Mtr19,Mtr20,Mtr21,Mtr22,Mtr23,Mtr24} ;

char inputString[StrSize] = "Send new text to serial port start with <x end with >\n where x = key number 1 to 6 or 0 for [M4]\n";  // Link Str Max Size

bool StrOK = false;          // String is complete - ended with 0x0A
bool ByteOK = false;         // Bytes received is complete - ended with 0x00
int  RecBytesLen = 0;        // Number of chars in RecBytes
byte RecBytes[MaxBytes];     // Raw bytes received must start < and end with > - can have length byte/int after < to include <> in data
byte NumBytes = 0;
bool NewData = false;
char AltNum[] = "1D6D1";
bool CheckSerial = false;    // Switch off serial check unless *se* toggles CheckSerial - default is off

const int NumButtons = 17;             // Total number of buttons = 12 keys (3 rows x 4 columns) + 5 pads on right
TFT_eSPI_Button key[NumButtons];       // Create 12 keys + 5 config buttons for keypad
bool pressed = false;                  // Detect touch

bool sSens = false;                    // PC sensor value sent from HWInfo
char sSArr[2][21]  = { "CPU    C Fan     rpm", "Sys    C Fan     rpm" };  

bool mPlay = false;                    // PC Playing Music
static const int mPlaySize = 96;       // Must check this
char mPlayArr[mPlaySize]  = { " " };    

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool TimeSet = false;                      // true if Clock Time has been set for example <tyymmddwhhmm> through serial port
bool tTimeDate = false;                    // Show Date and Time
static const int tTimeDateSize = 48;       // Must check this Tuesday, May 16, 2023 11:27:51 AM
time_t NowT;
bool timerEnable = false;                  // Set in GUI then clock activates
bool alarmEnable = false;                  // Set in GUI then clock activates
bool powerEnable = false;                  // Set in GUI then clock activates
char tTimeDateArr[mPlaySize]  = { "t             " };  
char wTimeDateArr[mPlaySize]  = { "w             " };
char aTimeDateArr[mPlaySize]  = { "a             " };
char pTimeDateArr[mPlaySize]  = { "p             " }; 
// Wday day of week, sunday is day 1  Year offset from 1970; 
tmElements_t t     = { .Second = 05, .Minute = 20, .Hour = 16, .Wday = 5, .Day = 17, .Month = 04, .Year = 55  };  // Clock Date and Time
tmElements_t alarm = { .Second = 05, .Minute = 20, .Hour = 16, .Wday = 5, .Day = 17, .Month = 04, .Year = 55  };  // Macro Clock Time Repeat+Oneshot
tmElements_t power = { .Second = 05, .Minute = 20, .Hour = 16, .Wday = 5, .Day = 17, .Month = 04, .Year = 55  };  // Restart-Off Clock Timer
tmElements_t timer = { .Second = 05, .Minute = 20, .Hour = 16, .Wday = 5, .Day = 17, .Month = 04, .Year = 55  };  // Macro Clock Timer Repeat+Oneshot
static volatile bool alarm_fired = false;  // Set in callback triggers Macro Send xOne xMany based on Clock Time
static volatile bool power_fired = false;  // Set in callback triggers Restart or PowerOff based on Clock Time
static volatile bool timer_fired = false;  // Set in callback triggers Restart or PowerOff based on Clock Timer


///////////////////////////////////////
// Setup
///////////////////////////////////////
void setup() 
{ SPI1.setSCK(pinSdClk);
  SPI1.setTX(pinSdMosi);
  SPI1.setRX(pinSdMiso);
  // SPI1.setCS(pinSdCs); // USB device not recognized if included
  SPI1.begin();   
    
  Serial.begin(9600);                 // serial rate is ignored for usb-serial
  // Serial.ignoreFlowControl(1); // https://arduino-pico.readthedocs.io/en/latest/serial.html  
  // if (Serial.available() )    { Serial.println(); Serial.flush();  } // Can also delay a few seconds  
  
  pinMode(LED_BUILTIN, OUTPUT);     // Used for Caps Num Scroll lock
  digitalWrite(LED_BUILTIN, LOW);   

  pinMode(LCDBackLight, OUTPUT);    // Used for Backlight HIGH is ON
  //digitalWrite(LCDBackLight, HIGH); // Switch on here to prevent blank screen when Coordinates missing 

  //if (!LittleFS.begin()) {LittleFS.format(); LittleFS.begin(); }   
  LittleFS.begin(); // LittleFs automatically format the filesystem if one is not detected

  SDFSConfig c2;      // See C:\Users\Name\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\3.9.2\libraries\SDFS\src\SDFS.h
  c2.setCSPin(22);    // class SDFSConfig: setAutoFormat setCSPin setSPISpeed setSPI setPart
  c2.setSPI(SPI1);    // Can also move this block to after tft.init();
  SDFS.setConfig(c2); // Without this block SDFS does not start
  // if (!SDFS.begin())  status("Insert SDCard"); 
  // while (!SDFS.begin())  { Serial.println("Card Mount Failed") ;  }      
  // digitalWrite(22, HIGH);
  
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));  
  usb_hid.setStringDescriptor("TinyUSB HID Composite");
  usb_hid.setReportCallback(NULL, hid_report_callback); // Activate report CapsScrollNum Lock indicators
  usb_hid.begin();                                      // Assume it does -> tusb_init();
  
  while( !TinyUSBDevice.mounted() ) delay(1);

  SDFS.begin(); delay(200);   

  InitCfg(1);                           // Must read rotate180 early 

  // setTime(01,02,03,20,04,2025);                                    // setTime(hr,min,sec,day,mnth,yr);
  if(timeStatus()==timeNotSet) TimeSet = false; else TimeSet = true;  // true if timeNeedsSync or timeSet 
  
  // Initialise the TFT screen TFT_eSPI/TFT_eSPI.h
  tft.init();
  if (NormVal==0) digitalWrite(LCDBackLight, HIGH);    // Backlight Full ON
            else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON            
        
  if (Rotate180) tft.setRotation(1);                   
            else tft.setRotation(3);    // Set the display orientation to 1 or 3 (180 degrees different)
  touch_calibrate();                    // New Calibration Data obtained if DoCal = true
     
  tft.fillScreen(Black); 
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);       
  tft.setTextSize(KEY_TEXTSIZE); 
  BackLightOn = true;                   // TFT init will turn it on   
  tft.setTouch(calData);                // void setTouch(uint16_t *data); in  TFT_eSPI/Extensions/Touch.h   

  if (SDNum>0) SDCardSelectFiles(0);    // SDCard File use is enabled

  ResetOnce = ResetOnceEnable;          // Both false unless *r1* state true
  
  if (SaveLayout>0) Layout = SaveLayout; else Layout = 2;  // if SaveLayout > 0 Layout 1 to 4 else default Layout 2 (Cfg)  
  ConfigKeyCount = 0;                                      // Start up
  ConfigButtons(0);                                        // Draw Buttons and Labels 0 = All 3+5 rows

  NumKeysChange();                              // NumkeysX = Layout 0  
  LastMillis = millis();                        // Initial start time
  NewData = KBrdActive = Kbrd = false;          // Can skip this
  KeyBrdByteNum = KBDispPos = 0;                // Toggle On/Off with yellow pad in Kbrd mode
  KeyBrdX = 2;                                  // Start on KeyBrd Page 3 - *Cm Page - also change void SendBytesEnd
  KeyBrdDirect = PadKeys = false;               // 5 Small Pads RH side Config Page = Red SkyBlue Yellow Grey Green 
  KeyBrd123 = Numkeys123 = 0;                   // Can skip this
  optionsindicators(0);
}

/////////////////////////////
// Main Loop
/////////////////////////////
void loop() 
{ if (ResetOnce) { if (!LittleFS.exists("ResetOnce")) {File f = LittleFS.open("ResetOnce", "w"); f.close(); rp2040.reboot(); }
                   else {ResetOnce = false; LittleFS.remove("ResetOnce");}}  // This is not needed anymore
  
  NowT = now();                           // Time now from Timelib
  if (MacroTimer18) CheckMacroTimers();   // Check Macro Timers 1-8 Oneshot Repeating Clocktime 
                           
  if (powerEnable && NowT>=makeTime(power)) power_fired = true;   // Compare now() with alarm or timer                              
  if (alarmEnable && NowT>=makeTime(alarm)) alarm_fired = true;   // Macro Timers 5 and 6
  if (timerEnable && NowT>=makeTime(timer)) timer_fired = true;   // Macro Timers 7 and 8 

  if (power_fired) { if (PowerClock==1) { DoPowerKeys('r', PowerKeysMenu, 8);  }
                     if (PowerClock==2) { DoPowerKeys('u', PowerKeysMenu, 10); } powerEnable = false; PowerClock = 0; power_fired = false; }
                        
  NowMillis = millis();   // get the current "time" (number of milliseconds since started)
  if ((NowMillis - LastMillis) >= TimePeriod)               // test whether the period has elapsed
      if (!BLOnOffToggle)                                   // Is toggled ON after Black Key Pressed for OFF state
         {if (DimVal==0) digitalWrite(LCDBackLight, LOW);   // Backlight Off
                    else analogWrite(LCDBackLight, DimVal); // Backlight Dimmed
      LastMillis = NowMillis;                               // Start Timer again
      RepLast = RepNow = NowMillis;                         // Reset repeat key timer      
      if (!Kbrd) status("");                                // Clear the status line if KeyBrd not active
      OptNum = VarNum = 0;                                  // [Key] and [Opt] Keys reset to unpressed state
      BackLightOn = false;   }                              // Until keypress    
    
  pressed = tft.getTouch(&t_x, &t_y, 650);                                     // True if valid key pressed 650 = threshold see touch.h
  if (TinyUSBDevice.suspended() && (pressed)) {TinyUSBDevice.remoteWakeup(); } // Wake up host if in suspend mode + REMOTE_WAKEUP feature enabled by host
  for (uint8_t b = 0; b < NumButtons; b++) 
      { if (pressed && key[b].contains(t_x, t_y)) key[b].press(true); else key[b].press(false); }
     
  RepNow = millis(); KeyHeld = 0; KeyHeldLayout = Layout;       // get the current time and save KeyHeld 
  for ( uint8_t b = 0; b < NumButtons; b++) 
      { if (key[b].justReleased()) {key[b].drawButton(false); RepLast = RepNow; }                      // draw normal - code at release time
        if (key[b].justPressed())  {key[b].drawButton(true); { buttonpress(b); RepLast = millis(); }}  // draw invert - code at press time  
            else if (((RepNow - RepLast) >= RepTimePeriod) && (key[b].isPressed()))                    // code while the button is held
                      {buttonpress(b); KeyHeld = b;}    }                                              // Do button number b always 17 on loop exit

  if (KeyHeld==7&&KeyHeldEnable) { Layout = KeyHeldLayout;                    // Swop Keys [L1-L4] to Key [Vo] if key held in > 600 millisecond
                    if (!MuteOn) { if (Layout==1) Layout=4; else Layout--; }  // RepTimePeriod = 600 Try 500-700 After this key repeat is active 
                    MuteOn = !MuteOn; ConfigButtons(1);  }              
                    
  if (CheckSerial) RecSerial();  // Switch off serial check with *se* toggle -  CheckSerial default is off
  // if (NewData) showRecData();
  if (NewData) if (!LayerAxD) DoNewData();     // First char 0-6 store file, t, a, p clock alarm timer, 7-9 non-ASCII data, PCData, Foobar, Time  
                         else DoNewSDCard();   // Same <n......> format save SDCard file choice 1-9 key 1-24 or M,S.T or K,k
                         
  if (Change && !BusyCNS) 
     { indicators();
       if (!BackLightOn) {if (NormVal==0) digitalWrite(LCDBackLight, HIGH);    // Backlight Full ON
                                    else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON
           BackLightOn = true; } 
        Change = false;
     }
                
} // main Loop

////////////////////////////////////////////
void DoCMTimers(byte TimerArr[], byte Num)
////////////////////////////////////////////
{
 LayerAxD = TimerArr[2]; Layout = TimerArr[0]; 
 if (TimerArr[3]==0) { strcpy(MSTAName,    TimerName[Num]); MacroKeys(TimerArr[1], 3); }  // M S T A
               else  { strcpy(MSTLinkName, TimerName[Num]); DoKeyMST(TimerArr[1]);     }  // M S T A K Link
 optionsindicators(0);
}
/////////////////////////////////////////////////////////////////////////////////////
// Check Macro Timers // TimerName[0-8] = Timer type Repeat T Repeat t Once T Once t
// Timers 1 - 4 are incremented by millis() count Timers 5 - 8 are 0/1 Off/On=Active
/////////////////////////////////////////////////////////////////////////////////////
void CheckMacroTimers()
/////////////////////////////////////////////////////////////////////////////////////
{ unsigned long TimeNow = millis(); 
  int LayerAxDPrev;
   
  LayoutPrev = Layout;  LayerAxDPrev = LayerAxD;
  
  if (MacroTimer1) { if ((TimeNow - TimeRepeatPrev) >= TimeRepeat)                           // Repeating MacroTimer1 still ON
                        { TimeRepeatPrev = TimeNow; DoCMTimers(MacroTimerArr1, 0);  } }
  
  if (Macrotimer2) { if ((TimeNow - timeRepeatPrev) >= timeRepeat)                           // Repeating MacroTimer2 still ON 
                        { timeRepeatPrev = TimeNow;  DoCMTimers(MacroTimerArr2, 1); } }    

  if (MacroTimer3) { if ((TimeNow - TimeOnceofPrev) >= TimeOnceof)                           // Oneshot MacroTimer3 OFF
                        { MacroTimer3 = false; DoCMTimers(MacroTimerArr3, 2); } }
                     
  if (Macrotimer4) { if ((TimeNow - timeOnceofPrev) >= timeOnceof)                           // Oneshot MacroTimer4 OFF
                        { Macrotimer4 = false; DoCMTimers(MacroTimerArr4, 3); } }

  if (alarm_fired) { if (MacroTimer5) { DoCMTimers(MacroTimerArr5, 4);    }                  // Repeating Clock MacroTimer5 ON - Oneshot Clock MacroTimer6 OFF
                     if (MacroTimer6) { MacroTimer6 = false; DoCMTimers(MacroTimerArr6, 5); }
                     alarm_fired = false; }    
                     
  if (timer_fired) { if (MacroTimer7) { DoCMTimers(MacroTimerArr7, 6);     }                // Repeating Clock MacroTimer7 ON - Oneshot Clock MacroTimer8 OFF
                     if (MacroTimer8) { MacroTimer8  = false; DoCMTimers(MacroTimerArr8, 7); }
                     timer_fired = false; }   
                               
  MacroTimer18 = MacroTimer8 || MacroTimer7 || MacroTimer6 || MacroTimer5 || Macrotimer4 || MacroTimer3 || Macrotimer2 || MacroTimer1; 
  
  Layout = LayoutPrev; LayerAxD = LayerAxDPrev;
  
}

///////////////////////////////////////////////////////////////////////////////
void GetTimeData(tmElements_t *a, char *tArr)
///////////////////////////////////////////////////////////////////////////////
{ bool SetT = false;
  if (tArr[0]=='t') SetT = true;
  
  if (RecBytes[1]!=0x2d)  { a->Year   = (2000 + (RecBytes[1]-48)*10 + (RecBytes[2]-48)) - 1970;  }
  if (RecBytes[3]!=0x2d)  { a->Month  = (RecBytes[3]-48)*10 + (RecBytes[4]-48); }
  if (RecBytes[5]!=0x2d)  { a->Day    = (RecBytes[5]-48)*10 + (RecBytes[6]-48); }
  if (RecBytes[7]!=0x2d)  { a->Wday   = (RecBytes[7]-48); }
  if (RecBytes[8]!=0x2d)  { a->Hour   = (RecBytes[8]-48)*10 + (RecBytes[9]-48); }       
  if (RecBytes[10]!=0x2d) { a->Minute = (RecBytes[10]-48)*10 + (RecBytes[11]-48); } 
  a->Second = 0; 
  
  if (SetT) { setTime(a->Hour,a->Minute,0,a->Day,a->Month,a->Year+1970); 
              if(timeStatus()==timeNotSet) TimeSet = false; else TimeSet = true; optionsindicators(0); }
  
  for (int n=0; n<12; n++) { if (RecBytes[n+1]!=0x2d) tArr[n] = RecBytes[n+1]; else tArr[n] = ' '; }  // Improve later

  if (tArr[0]=='a') { alarmEnable = true; optionsindicators(0); }
  if (tArr[0]=='w') { timerEnable = true; optionsindicators(0); } 
}

/////////////////////////////////////////////////////////////////////////////
void WriteSensorData()                   // PC sensor value sent from HWInfo
                                         // Make sure A-D is white not brown
/////////////////////////////////////////////////////////////////////////////
// CPU Data <s0    REG_SZ    36>
//           012345678901234567
// CPU  Fan <s3    REG_SZ    978 >
//           01234567890123456789
///////////////////////////////////////////////////////////////////////////// 
{ // char sSArr[2][21]  = { "CPU    C Fan     rpm", "Sys    C Fan     rpm" };
  //                         01234567890123456789    01234567890123456789
  //                   Option    0        3
  int i;                 
  byte Option = RecBytes[1] - 48;
  
  if (Option==0) { for (i=4;  i<6;  i++) sSArr[0][i] = RecBytes[i+12]; }
  if (Option==3) { for (i=13; i<16; i++) sSArr[0][i] = RecBytes[i+3];  
                   if (NumBytes>20) sSArr[0][16] = RecBytes[19];       }
                   
  status(sSArr[0]);  
  sSens = false;
}

/////////////////////////////////////////////////////////////////////////////
void WriteMusicPlayingData()              // PC music Playing from Foobar2000
                                          // Make sure A-D is white not brown
/////////////////////////////////////////////////////////////////////////////
// Music Playing <mARTIST - TITLE                   max = 96 chararacters   >
///////////////////////////////////////////////////////////////////////////// 
{ int i; 
  if (NumBytes>mPlaySize) { status("Music Data too long..."); return; }               
  for (i=0;  i<NumBytes;  i++) mPlayArr[i] = RecBytes[i+1];                   
  status(mPlayArr); 
  mPlay = false; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void WriteDateTime()              // Alternative Date Time - only displayed not set a sytem time-date
                                  // Make sure A-D is white not brown
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Date Time <TTuesday, May 16, 2023 11:27:51 AM>   max = 48 chararacters 
///////////////////////////////////////////////////////////////////////////// 
{ int i; 
  if (NumBytes>tTimeDateSize) { status("Date Time Data too long..."); return; }               
  for (i=0;  i<NumBytes;  i++) tTimeDateArr[i] = RecBytes[i+1];    // Skip "T"               
  status(tTimeDateArr); 
  tTimeDate = false; 
}

///////////////////////////////////////////////////////////////////////////////////////////////
void DoMSTLabel(byte Option, byte mst)  // Pointer to LabelArrM,S.T [24][6] Size 192
///////////////////////////////////////////////////////////////////////////////////////////////
{ char LabelFile[7] = "LabelM";
  char MST[] = "0MCST";
  int a, b, n, m;
  File f1, f2;
  // char LabelArr[24][6];  // Size 192

  a = Layout-1-(Layout>2); // a = 0,1,2 for M,S,T Layouts
  b = mst-1-(mst>2);       // b = 0,1,2 for M,S,T Layouts
    
  if (Option==0)
    { LabelFile[5] = MST[mst];                      // 73 + (Layout==3)*6 + (Layout==4)*7;   // Layout = 1-4 LayerAD = 0-3
      if (LayerAxD) f1 = SDFS.open(LabelFile, "r"); else f1 = LittleFS.open(LabelFile, "r"); n = f1.size();
      f1.readBytes(NameStr1, n); f1.close();       // NameStr1 is the filename that holds the 24 (5 char max), new label names

     if (LayerAxD) f2 = SDFS.open(NameStr1, "r"); else f2 = LittleFS.open(NameStr1, "r"); n = f2.size(); 
     f2.readBytes((char *)LabelArr[b], n);  f2.close();
    }      

  if (Option==1) { if (a==0 && !MLabel) return; if (a==1 && !SLabel) return; if (a==2 && !TLabel) return;
                   for (n=4; n<7; n++)  strcpy(keyLabel[n], LabelArr[a][n-4+LayerAD*6]);    // 0+0,1+0.2+0 0+6
                   for (n=8; n<11; n++) strcpy(keyLabel[n], LabelArr[a][n-5+LayerAD*6]); }  // 3+0,4+0,5+0 3+6   
}

///////////////////////////////////////////////////////////////////////////////////////
// Check and save new SDCard file strings SDFilename is in sdcard.h select with *sd*n
// These are limited size Bytesize = 200 - sensible here because wait until written
// Directly written to SDCard files with PC only limited by SDCard capacity
///////////////////////////////////////////////////////////////////////////////////////
void DoNewSDCard()
{ int n = 0;
  byte *BytePtr;
  bool Found = false, Label = false;
  byte a, b, d, c = 0;
  int ASize;
  char LabelFile[18] = "LabelM"; 
  File f; 
  
  Found = NewData = StrOK = ByteOK = false;  

  a = RecBytes[0];      // * char  
  b = RecBytes[3];      // Also * if Starcode 
  if (a==0x2A&&b==0x2A) { for (n=0; n<=NumBytes; n++) KeyBrdByte[n] = RecBytes[n]; 
                          KeyBrdByteNum = NumBytes; if (SendBytesStarCodes()) { ConfigButtons(1); return; }  } // <*xy*nn> sent
                          
  a = a - 48;                          // ASCII Number 0-9 subtract 48
  Label = (a==61 || a==67 || a==68);   // a = m,s,t is labelfile name which points to another file with new labels for 24 M,S,T keys;
  Found = (a<10);                      // a = 1 to 6 - only 6 Keys on every Layer A-D

  if (Label) { LabelFile[5] = RecBytes[0] - 32;                                      // +48-32 = M, S, T
               BytePtr = MacroBuff; 
               for (n=1; n<=NumBytes; n++) { BytePtr[n-1] = RecBytes[n]; } // Skip 1 = char <m,<s,<t < is removed earlier

               if (LayerAxD) f = SDFS.open(LabelFile, "w"); else f = LittleFS.open(LabelFile, "w"); // Filename LabelM,S,T
               f.write(BytePtr, NumBytes-1);                                                        // Write filename to SDCard or Flash
               f.print('\0');                                                                       // Add NULL to end of filename in File LabelX    
               f.close();  
               strcat(LabelFile, " saved"); status(LabelFile); return; }
  
  if (Found){ if (a>0) c = a + (LayerAD)*6 - 1;    // S1-S6=>S19-S24 T1-T6=>T19=T24 M1-M6=>M19=M24 -> 1-24 used  
                                                   // c = 0 to 23 for every Layout M S T c=1+0x6-1=0 c=6+3x6-1=23
                                                   
              if (Layout!=2) {if (a>0) {ByteOK = true;  BytePtr = MacroBuff;           // No need for 24 SDC1to24[c]
                                        status((char *)SDName[c]); }  }                // SDCard file 1-21 x 1-24
                  
              for (n=1; n<=NumBytes; n++) { BytePtr[n-1] = RecBytes[n]; }  }           // Skip 1,2 = char <#               
    
   if ((Found)&&(ByteOK))   { File f = SDFS.open(SDName[c], "w");  // Filename set with *sd*1-9,(13-19), k,K,M,S,T
                              f.write(BytePtr, NumBytes);          // (13-19=A-G must be directly copied to SDCard
                              //f.print('\0');                     // 4-9=U-Z can be written here    
                              f.close();    }                                        
}

///////////////////////////////////////////////////
// Check and save new character strings or macros
///////////////////////////////////////////////////
void DoNewData()
{ int n = 0;  
  byte *BytePtr;
  bool Found = false, tTime = false, aTime = false, pTime = false, wTime = false;
  byte a, b, c = 0;
  int ASize;

  Found = NewData = StrOK = ByteOK = false;  

  a = RecBytes[0];      // * char  
  b = RecBytes[3];      // Also * if Starcode 
  if (a==0x2A&&b==0x2A) { for (n=0; n<=NumBytes; n++) KeyBrdByte[n] = RecBytes[n]; 
                          KeyBrdByteNum = NumBytes; if (SendBytesStarCodes()) { ConfigButtons(1); return; }  } // <*xy*nn> sent
                          
  a = a - 48;                // ASCII Number 0-9 subtract 48
  tTime     = (a==68);       // 0x74 = 't' Date-Time setting
  aTime     = (a==49);       // 0x61 = 'a' Date-Time setting
  pTime     = (a==64);       // 0x70 = 'p' Date-Time setting
  wTime     = (a==71);       // 0x77 = 'w' Date-Time setting  
  sSens     = (a==67);       // 0x73 = 's' PC sensor value from HWInfo
  mPlay     = (a==61);       // 0x6D = 'm' PC music Playing
  tTimeDate = (a==36);       // 0x54 = 'T' Time Date Display (not system time-date)
  Found = (a<10);            // a = 1 to 6 text a = 7 - 9 non ASCII

  if (sSens)     { WriteSensorData();       sSens     = false; return; }
  if (mPlay)     { WriteMusicPlayingData(); mPlay     = false; return; }
  if (tTimeDate) { WriteDateTime();         tTimeDate = false; return; }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //             0 12 34 56 7 89 01
  // Use serial <t yy mm dd w hh mm> 22110351200 12:00am 3 Nov 2022 Thursday
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (tTime) { GetTimeData(&t, tTimeDateArr); TimeSet = true; status(tTimeDateArr); return; }              
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (aTime) { GetTimeData(&alarm, aTimeDateArr); status("Macro Timer set [R-C][O-C]"); return; }
  if (wTime) { GetTimeData(&timer, wTimeDateArr); status("Macro Timer set [RcT][OcT]"); return; }
  if (pTime) { GetTimeData(&power, pTimeDateArr); status("Power Timer set [O-C][R-C]"); return; }   
    
  if (Found){ if (a>0) c = a + (LayerAD)*6 - 1;    // S1-S6=>S19-S24 T1-T6=>T19=T24 M1-M6=>M19=M24
  DoMSTName(c, Layout);   

              if (Layout==3) {if (a>0) {ByteOK = true;  BytePtr = Str1to12[c]; 
                                        MacroS1S12[c] = 1; MacroSizeS1S12[c] = NumBytes; status("Saved [Sx]"); }  }  // Keys S1-S24                                  
              if (Layout==4) {if (a>0) {ByteOK = true;  BytePtr = Ttr1to12[c]; 
                                        MacroT1T12[c] = 1; MacroSizeT1T12[c] = NumBytes; status("Saved [Tx]"); }  }  // Keys T1-T24                                  
              if (Layout==1) {if (a>0) {ByteOK = true;  BytePtr = Mtr1to12[c];
                                        MacroM1M12[c] = 1; MacroSizeM1M12[c] = NumBytes; status("Saved [Mx]"); }  }  // Keys M1-M24
                  
              for (n=1; n<=NumBytes; n++) { BytePtr[n-1] = RecBytes[n]; }  }           // Skip 1,2 = char <#               
    
   if ((Found)&&(ByteOK)) {ASize = DoFileBytes(ByteOK, MSTName, BytePtr, NumBytes, 0); }  // Save to Flash                                         
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
    SerPr2;
    Serial.println(RecStr);
    //NewData = false;
}

////////////////////////////////////////////////////////////////////////////////////
void Timer2Str(char * TStr, byte Option, long int PVal)
////////////////////////////////////////////////////////////////////////////////////
// Used: https://www.geeksforgeeks.org/how-to-convert-an-integer-to-a-string-in-c/
{ int j, k, i;
  // Serial.println(PVal);
  if (Option==0) PVal = PVal/1000;                                                            // millis -> s seconds
  if (Option==1) { if (timeU=='m') PVal = PVal/60000; if (timeU=='h') PVal = PVal/3600000; }  // seconds -> m minutes -> h hours
  
  i = 0; while (PVal>0) { TStr[i++] = (PVal%10) + 48; PVal = PVal/10; }; TStr[i] = 0x00;
  for (j = 0, k = i - 1; j < k; j++, k--) { char temp = TStr[j]; TStr[j] = TStr[k]; TStr[k] = temp; }                 
}

///////////////////////////////////////////////
void DoPowerKeys (char Cmd, bool Menu, int s) 
///////////////////////////////////////////////
// Cmd = 'i', 'u', 'r', 't', 'T', 'R', 'a', 'O'
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// case 0:  R-T // HOME - Media Previous    DoPowerKeys ('T', PowerKeysMenu, button);   
// case 1:  Stp // ARROW UP                 DoPowerKeys ('a', PowerKeysMenu, button);
// case 2:  O-T // PAGE UP                  DoPowerKeys ('T', PowerKeysMenu, button);
// case 3:  R-C // V+ or Delete or Execute  DoPowerKeys ('R', PowerKeysMenu, button); PowerClock=1; status("Restart on Clock"); 
// case 4:  R-t // ARROW LEFT               DoPowerKeys ('t', PowerKeysMenu, button);
// case 5:  Cfg // CONFIG                   ---
// case 6:  O-t // ARROW RIGHT              DoPowerKeys ('t', PowerKeysMenu, button);
// case 7:  O-C // Vol Mute                 DoPowerKeys ('O', PowerKeysMenu, button); PowerClock=2; status("PowerOff on Clock");
// case 8:  Rst // END                      DoPowerKeys ('r', PowerKeysMenu, button);
// case 9:  Log // ARROW DOWN               DoPowerKeys ('i', PowerKeysMenu, button);
// case 10: Off // PAGE DOWN                DoPowerKeys ('u', PowerKeysMenu, button);
// case 11: [ ] // V- or Enter or Add       ---
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  if (power_fired) { power_fired = false; if (PowerClock==1) { DoPowerKeys('r', PowerKeysMenu, 8);  } // Enable with *po*
//                                          if (PowerClock==2) { DoPowerKeys('u', PowerKeysMenu, 10); } PowerClock = 0; }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here                  
  int b, j, k, i, n = 0;          // s = button pressed
  long int PVal;
  byte xLate1[] = { 4,0,2,0,  3,0,1,0,  0,0,0,0,  0,0,0,0};  // [] only: Key s 0-11 matched DimData[value-in-here] 0 = DimData not needed
  
  if (s==3||s==7) {PowerKeys = false; powerEnable = true; ConfigButtons(1); return;} // s 3 PowerClock=1 s 7 PowerClock=2
   
  if ((s>7)||(s==1)) { PowerKeys = false; // If immediate action Must do this before the actions below
                       status(" ");  ConfigButtons(1);  }
  ///////////////////////////////////////////////////////////////
  // Windows Restart Shutdown Logout via Menu
  /////////////////////////////////////////////////////////////// 
  if (Menu&&s>0&&OptionOS==0) { keycode[0] = GuiL;             // or use HID_KEY_GUI_RIGHT
                                keycode[1] = HID_KEY_X;        // 'X' or 'x' not the same here
                                keycode[2] = 0x00; 
                                usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(dt50); // GUI + X then U for shutdown options
                                usb_hid.keyboardRelease(HIDKbrd);              delay(dt50); // Confirm with ArrowLeft Enter 
                                usb_hid.keyboardPress(HIDKbrd, 'u');           delay(dt50); // After U it is r u i
                                usb_hid.keyboardRelease(HIDKbrd);              delay(dt50); 
                                usb_hid.keyboardPress(HIDKbrd, Cmd);           delay(dt50); // [R]estart sh[U]tdown S[i]gnout
                                usb_hid.keyboardRelease(HIDKbrd);              delay(dt50); return; }

  ///////////////////////////////////////////////////////////////
  // Raspberry Pi OS (LXDE) Restart Shutdown Logout via Menu
  ///////////////////////////////////////////////////////////////
  if (Menu&&s>0&&OptionOS==2) { keycode[0] = GuiL;             // PiOS: Open Menu
                                keycode[1] = 0x00; 
                                usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(dt50); // GUI + Up for shutdown options
                                usb_hid.keyboardRelease(HIDKbrd);                   delay(dt50); 
                                keycode[0] = ArrUp;            // Up to [Shutdown Restart Logout] options
                                keycode[1] = 0x00; 
                                usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(dt50); // GUI + Up for shutdown options
                                usb_hid.keyboardRelease(HIDKbrd);                   delay(dt50); 
                                usb_hid.keyboardPress(HIDKbrd, '\r');               delay(dt50); // Confirm with Enter 
                                usb_hid.keyboardRelease(HIDKbrd);                   delay(dt500); 
                                if (Cmd=='i') { keycode[0] = ArrDown;               // PiOS: '2 x ArrDwn''\r' Logout
                                                keycode[1] = 0x00;            
                                                usb_hid.keyboardReport(HIDKbrd, 0, keycode);  delay(dt50); 
                                                usb_hid.keyboardRelease(HIDKbrd);             delay(dt50); }
                                if (Cmd=='r'||Cmd=='i') { keycode[0] = ArrDown;                         // PiOS: '1 x ArrDwn''\r' Restart                                             
                                                          keycode[1] = 0x00;            
                                                          usb_hid.keyboardReport(HIDKbrd, 0, keycode);  delay(dt50); 
                                                          usb_hid.keyboardRelease(HIDKbrd);             delay(dt500); }  
                                                              
                                usb_hid.keyboardPress(HIDKbrd, '\r'); delay(dt50); // Cmd = '\r' PowerOff include if (Cmd=='u'||'i'||'R')
                                usb_hid.keyboardRelease(HIDKbrd);     delay(dt500); return; } 
                                                                        
  ////////////////////////////////////////////////////////////////////////
  // Raspberry Pi OS (LXDE) Restart Shutdown Logout via Run Box in Menu
  // See comments section at end of this code for another way to do this
  ////////////////////////////////////////////////////////////////////////
  if (OptionOS==2) { keycode[0] = AltL;             // PiOS: Open Run Box
                     keycode[1] = F02; 
                     keycode[2] = 0x00; 
                     usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(dt50); 
                     usb_hid.keyboardRelease(HIDKbrd);                   delay(dt50);

                     delay(dt500);                     
                     for (n=0;  n<strlen(ShutDwn2[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn2[s][n]); delay(dt50);
                                                             usb_hid.keyboardRelease(HIDKbrd);               delay(dt50); } }                     
                                            
   /////////////////////////////////////////////////////////////////////////////////                     
   // See comments section at end of stringsRSL.h for 2 other ways to do this
   // Change OptionOS with *os*[EXE]
   /////////////////////////////////////////////////////////////////////////////////        
   if (OptionOS==1) { keycode[0] = CtrL;               // Ctr+Alt+T open terminal 
                      keycode[1] = AltL;               // Works in Debian 11.7 without loosing focus to desktop search box
                      keycode[2] = HID_KEY_T;          // Ubuntu 22 and Mint 21 and PiOS (sometimes) loose focus to desktop search box
                      keycode[3] = 0x00;               // Adjust delay longer if characters are missing
                      usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(dt50);  
                      usb_hid.keyboardRelease(HIDKbrd);              delay(dt50); 
                     
                      delay(dt500); strcpy(ShutDwn1, ShutDwnLin1); strcat(ShutDwn1, ShutDwnLin2[s]); 
                      for (n=0;  n<strlen(ShutDwn1); n++) { usb_hid.keyboardPress(HIDKbrd, ShutDwn1[n]); delay(dt50);
                                                            usb_hid.keyboardRelease(HIDKbrd);            delay(dt50); } 
                    } 
                     
   if (OptionOS==0) { keycode[0] = GuiL;             // or use HID_KEY_GUI_RIGHT
                      keycode[1] = HID_KEY_R;        // 'R' or 'r' not the same here
                      usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(dt25); // GUI + R
                      usb_hid.keyboardRelease(HIDKbrd);              delay(dt25);

                      b = xLate1[s]; PVal = DimData[b]/1000;  
                      if (b>0)  { Timer2Str(TimerArr, 2, PVal); Serial.println(TimerArr);
                                  strcpy(ShutDwn0, ShutDwnWin1); strcat(ShutDwn0, ShutDwnWin2[s]); strcat(ShutDwn0, TimerArr); } 
                      if (b==0) { strcpy(ShutDwn0, ShutDwnWin1); strcat(ShutDwn0, ShutDwnWin2[s]);  }
                      for (n=0;  n<strlen(ShutDwn0); n++) { usb_hid.keyboardPress(HIDKbrd, ShutDwn0[n]); delay(dt25);
                                                            usb_hid.keyboardRelease(HIDKbrd);            delay(dt25); }
                    }
                               
  usb_hid.keyboardPress(HIDKbrd, '\r'); delay(dt25);           // Confirm with Enter
  usb_hid.keyboardRelease(HIDKbrd);

  if (PowerKeys) { PowerKeys = false; // Clear Power Keys for Timed action
                   status("Timer running");
                   ConfigButtons(1); }
}

///////////////////////////////
void DoBsdCodes(byte Num)
///////////////////////////////-
{ uint8_t keycode[6]; 
  for (int n=0; n<6; n++) keycode[n] = 0x00;

  // Do Linkfiles K01Link to K24Link on SDCard or Flash
  if (Num>26) { DoKey16(Num); if (LinkOk) return; }; 
  status(" "); // clear LinkMacro not found message 
               
  // Check if Key have defines i.e not 0x00 see macroBanks.h then do defines in BSD1-3
  if (BsDCode1[Num]==0x00) return; 
  
  keycode[0] = BsDCode1[Num];   // 54 options Delete BackSpace Tab AltTab Insert Esc PScr Num Caps Scroll etc
  keycode[1] = BsDCode2[Num]; 
  keycode[2] = BsDCode3[Num];                        
  usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt25); 
  usb_hid.keyboardRelease(HIDKbrd);            delay(dt25); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DoLinkStr(int NameStrLen) // Read Flash or SDCard filenames and execute in sequence 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Repeat rnn = repeat 01=1 to 99=99 times the next macro    - r03a01 repeat a01 3x
// Repeat Rnn = repeat 01=100 to 9900 times the next macro   - R03a01 repeat a01 300x
// Repeat RRn = repeat 1=10000 to 90000 times the next macro - RR3a01 repeat a01 30000x
// Use rnna01 = repeat a01 nn times and not a01rnn as in DuckyScript
// 
// Delay ddn - delay 0=100 to 9=900 milliseconds - dd3a01 delay 300 milliseconds before a01 executed
// Delay dnn - delay 01=1  to 99=99 seconds      - d03a01 delay 3 seconds before a01 executed
// Delay Dnn - delay 01=1  to 99=99 minutes      - D03a01 delay 3 minutes before a01 executed
// Delay DDn - delay 1=1   to 9=9 hours          - DD3a01 delay 3 hours before a01 executed
// 
// Load Link File LxnXXX Lxn n=0 Do not change current SDCard/Flash storage to load linkfile XXXLink 
// XXX = a-z, A-Z, 0-9       n=1 Use Flash to load linkfile XnnLink
// in any combination        n=2 Use SDCard to load linkfile XnnLink 
// lowercase L also ok       n=3 Flip current storage to load linkfile XnnLink 
// If LayerAxD changed       x=0 L0n: Do not restore LayerAxD on completion
// no/restore original       x=1 L1n: Restore LayerAxD on completion
//
// nKeys executed in stringlist from a 10 char list in nKeysLnkChar.
// Then same rules for repeat, delay as macro Xnn but can only use NKeys 00-99
// Change nKeysLnkChar with *0t*list-of-10-char. Default is 'n','o','p','q','u','v','w','x','y','z'. 
// Alternative list could be '0','1','2','3','4','5','6','7','8','9'.
// For example a32r03n54a43, will do macro a32, then repeat nKey n54 3 times, then do macro a32 again. 
// Cannot use dD rR lL as nKeys in stringlist as they are reserved for delay, repeat, link. 
// Considering alternative choices of w (wait) for d (delay), and x (times) in DoLinkStr()
//
// For all 3 forms of repeat - r nn, R nn, RR n if nn = 00 or n = 0 r, R, RR is executed as if n = 1 
// For all 4 forms of delay  - dd n, d nn, D nn DD n if nn = 00 or n = 0 dd, d, D, DD is executed as if n = 1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 };      // simultaneous keys pressed in here
  char NameStr[4] = { "Xnn" };
  byte c, e, *BPtr, nkey = 0;  
  uint32_t a, b, d;
  int i, k, m, n, j = 0;  
  int LinkDelay = 0, LinkRepeat = 0; 
  const int nr =     100;        // repeat 100x
  const int nS =    1000;        // d nn delay in seconds
  const int nR =   10000;        // repeat 10000x  
  const int nM =   60000;        // D nn delay in minutes
  const int nH = 3600000;        // DD n delay in hours   
  
  while (j<NameStrLen-1) 
        {for (i=0; i<3; i++) { NameStr[i] = inputString[i+j]; }       // Save and restore nChar, NKeysX, Numkeys123 in DoKey16() and DoKeyMST()
         LinkOk = false; nkey = LinkDelay = 0; a = NameStr[i-1]-48; e = NameStr[i-2]; c = NameStr[i-3]; b = e-48; d = b*10 + a; 
         for (n=0; n<10; n++) if (c==nKeysLnkChar[n]) nkey = 1;       // Do Xnn as nkey not macro exclude rR dD lL
                               
         if (c=='d'||c=='D') { LinkDelay = nS*(c=='d'&&e!='d')*d + nM*(c=='D'&&e!='D')*d + nH*(c=='D'&&e=='D')*a + nr*(c=='d'&&e=='d')*a;
                               if (LinkDelay>nH*10) { status("Delay too long"); return; }
                               delay(LinkDelay);  }                                                                // dnn ddn Dnn DDn
                               
         if (c=='r'||c=='R') { LinkRepeat = (c=='r')*d + nr*(c=='R')*d + nR*(c=='R'&&e=='R')*a; LinkDelay = 1; 
                               if (LinkRepeat>nR*100) { status("Repeat too many times"); return; }  }              // rnn Rnn RRn
                               
         if (c=='l'||c=='L') { if (b==1) LayerAxDSave = LayerAxD; if (a==3) LayerAxD = !LayerAxD;                  // Restore after completion
                               if (a==1||a==2) LayerAxD = a-1;    if (b==0) LayerAxDSave = LayerAxD;               // L01 Flash L02 SSDCard
                               for (k=0; k<3; k++) { NameStr1[k] = inputString[i+j+k]; } LinkOk = true; return; }  // L00Xnn XnnLink next Link File 

         if (nkey) { do { nChar = c; Numkeys123 = d/12; m = d%12; strcpy(NKeysX[m], NameStr); DoNKeys(m); LinkDelay = 1; LinkRepeat--; }  
                     while (LinkRepeat>=0) ; } // Skip 1 cycle                                            
                                                           
         // SerPr2; Serial.print(j); SerPr1; Serial.print(LinkDelay); SerPr1; Serial.print(LinkRepeat); SerPr1; Serial.println(NameStr);  
            
         j += 3; 
         if (LinkDelay==0)
            {NameStr[j+1] = 0x00; 
             BPtr = MacroBuff; 
             MacroBuffSize = DoFileBytes(0, NameStr, BPtr, ByteSize, LayerAxD); 
             if (MacroBuffSize==0) { status("Macro not found"); return; }
             LinkOk = true;
             if (MacroBuff[0]<0x80) 
                {n = 0; while (MacroBuff[n]!=0) { usb_hid.keyboardPress(HIDKbrd, MacroBuff[n]); delay(dt25);
                                                  usb_hid.keyboardRelease(HIDKbrd);             delay(dt25); 
                                                  n++; if (n>ByteSize) break; }    }            // limit is inputString size 
             if (MacroBuff[0]>0x7F) 
                {for (n=0;  n<6; n++) { keycode[n] = MacroBuff[n]; if (keycode[n]==0) break; }  // allow one 0x00 in keycode
                                        usb_hid.keyboardReport(HIDKbrd, 0, keycode);  delay(dt25); 
                                        usb_hid.keyboardRelease(HIDKbrd);             delay(dt25); } 
             if (LinkRepeat>0) j -= 3;
           } LinkRepeat-- ;                     // if (LinkDelay==0)
        }                                       // while (j<NameStrLen)
}

////////////////////////////////////////////////////////////////////////////////////////////
void DoKey16(byte Num)        // Currently the same as DoKeyMST() but will change in future
////////////////////////////////////////////////////////////////////////////////////////////
// Keys1-24 read content of files K01-K24 on SDCard or Flash which contains: 
// 3-letter filesname on flash memory such as a01-a99 (read in groups of 3 bytes = filename)
// Send *sd*K, then make A-D orange A, goto Layer 3 (S), send <2a50a51a52>, it shows K02Link
// in statusbar and file K02Link on SDCard size 10, Then assign Key 2 to one of the special 
// 5 keys - when pressed it will execute the contents of the three files:
// a50: GUI r, a51: "notepad", a52: Ctrl + increase size
////////////////////////////////////////////////////////////////////////////////////////////
{ int NameStrLen = 0;
  File f;    
  int n = 0;
  
  LinkOk = false;
  nKeysShow = 0;           // Else nKeys will not execute
  char xChar = nChar;      // Save nChar
  int n123 = Numkeys123;   // Save Numkeys123 
  
  LayerAxDSave = LayerAxD;        // Save if DoLinkStr no change restore
  DoMSTLinkName(Num-27, 5);       // Layout = 5 filename such as K01Link
  strcpy(NameStr1, MSTLinkName);  // KxxLink + 0x00 length = 8                        
 
  do { if (LayerAxD) f = SDFS.open(NameStr1, "r"); else f = LittleFS.open(NameStr1, "r"); 
       NameStrLen = f.size(); f.readBytes(inputString, NameStrLen); f.close();        
       NameStr1[0] = 0x00; n++; // Only do DoLinkStr() once unless L0nXnn loads new NameStr1 
       DoLinkStr(NameStrLen);            
     } while (NameStrLen>0) ;    // Can also stop with 3-char entry such as *** in Link file or n>1000?     
  
  LayerAxD = LayerAxDSave; optionsindicators(0); // Restore or new value
  nChar = xChar; 
  Numkeys123 = n123;  
}

////////////////////////////////////////////////////////////////////////////////////////////
void DoKeyMST(byte Num)        // Currently the same as DoKey16() but will change in future
////////////////////////////////////////////////////////////////////////////////////////////
// Keys M1-M24 read content of files M01Link-M24Link on SDCard/FlashMem which contains: 
// 3-letter filesname on flash memory such as a01-a99 (read in groups of 3 bytes = filename)
// Send *sd*k, make A-D orange A, goto Layer 1 (M), send <2a50a51a52>, it shows M02Link in status
// and file M02Link on SDCard size 10, Then Key [M2] when pressed it will execute the contents 
// of the three files: a50: GUI r, a51: "notepad", a52: Ctrl + increase size
// Can also create file M02Link using the Macro Editor: Setup m02 as Target, then create 
// a50a51a52 using [ADD]. Then press [Lnk] and file M02Link will be created on the SDCard if
// A-D is orange or in FlashMem if A-D is white (FlashMem is case-sensitive => two files
// named m01Link and M01Link can be present at the same time.)
////////////////////////////////////////////////////////////////////////////////////////////
{ int NameStrLen = 0;  
  File f;
  int n = 0;
  LinkOk = false;
  nKeysShow = 0;  // Else nKeys will not execute
  char xChar = nChar;      // Save nChar
  int n123 = Numkeys123;   // Save Numkeys123 
  
  LayerAxDSave = LayerAxD;
  DoMSTLinkName(Num, Layout);      // Layout -> M S T filename such as M01Link
  strcpy(NameStr1, MSTLinkName);   // MxxLink + 0x00 length = 8 
  
  do { if (LayerAxD) f = SDFS.open(NameStr1, "r"); else f = LittleFS.open(NameStr1, "r"); 
       NameStrLen = f.size(); f.readBytes(inputString, NameStrLen); f.close();        
       NameStr1[0] = 0x00; n++; // Only do DoLinkStr() once unless L0nXnn loads new NameStr1 
       DoLinkStr(NameStrLen);            
     } while (NameStrLen>0) ;    // Can also stop with 3-char entry such as *** in Link file or n>1000?     
  
  LayerAxD = LayerAxDSave; optionsindicators(0); // Restore or new value
  nChar = xChar; 
  Numkeys123 = n123; 
}

//////////////////////////////////////////////////////////////////////////////////////////////
bool UnlinkKeyMST(byte Option)  // Source
//////////////////////////////////////////////////////////////////////////////////////////////
// For example remove file p05Link on SDCard press[*Cm] until *ua* then [ADD] p05 press [EXE]
//////////////////////////////////////////////////////////////////////////////////////////////
{ bool LinkFound = false;
  char LinkStr[30] = {"Removed Link File: "};
  bool sdCard = false;
  byte k = 0, n = 0;

  if (KeyBrdByteNum>4) 
     { while (k!='L' && n<=KeyBrdByteNum-4) { k = NameStr1[n] = KeyBrdByte[n+4]; n++; if (n>6) { status("Link xnnnn name too long"); return LinkFound; } } 
       NameStr1[n-1] = 0x00; strcat(NameStr1, "Link");  LinkFound = true; }   
        
  if (KeyBrdByteNum==4) 
     { if (!Option) DoMSTLinkName(Option1, MST1);                 // Layout -> M S T filename such as M01Link 
       if (Option)  DoMSTALinkName(Option1, MST1);                // Layout -> M S T A K filename such as K01Link 
       strcpy (NameStr1, MSTLinkName); } 
  
  if (SrcDst==0||SrcDst==1) sdCard = false; if (SrcDst==2||SrcDst==3) sdCard = true;   // For Source 0,1=Flash 2,3=SDCard
  if (sdCard) { LinkFound = SDFS.remove(NameStr1); } else LinkFound = LittleFS.remove(NameStr1); 

  if (LinkFound) { strcat(LinkStr, NameStr1); status(LinkStr); delay(1000); }
             
  return LinkFound; 
}

///////////////////////////////
bool DoCodeOption(byte c)
///////////////////////////////
{ usb_hid.keyboardPress(HIDKbrd, Labels[LayerAD][Layout-1][c][0]);       delay(dt25);
  usb_hid.keyboardRelease(HIDKbrd);                                      delay(dt25);
  usb_hid.keyboardPress(HIDKbrd, Labels[LayerAD][Layout-1][c][1]);       delay(dt25);
  usb_hid.keyboardRelease(HIDKbrd);                                      delay(dt25);
  usb_hid.keyboardPress(HIDKbrd, Labels[LayerAD][Layout-1][c][2]);       delay(dt25);
  usb_hid.keyboardRelease(HIDKbrd);                                      delay(dt25);

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ***Mouse
void PadKeysState(int Pbutton, bool Restore)  // Pads are now Pbuttons 1 to 5
{ if (Restore) { status(" ") ;                                                   }                            
          else { if (Pbutton==1) { Kbrd = MouseK  = NumKeys = false; }
                 if (Pbutton==2) { Math = NumKeys = MouseK  = false; }
                 if (Pbutton==3) { Math = Kbrd    = NumKeys = false; }                   
                 if (Pbutton==4) { Math = Kbrd    = MouseK  = false; } // Used in MacroEditor to toggle KeyboardDirect On/Off
                 if (Pbutton==5) { ;}  }                               // Used for Capslock Numlock toggle or [Opt] [Key] keys
  ConfigButtons(1);
  if (Kbrd)    status("[NXT]Page [ADD]Char [EXE]Macro"); else status(" ");
  if (MouseK)  status("Mouse Buttons-LMR-BF Scroll-UD Cursor-UD-LR"); else status(" ");
  if (NumKeys) status("Pad (+)(-) Page-nChr Up-Dwn (e)(s) Execute-Show"); else status(" ");
}

/////////////////////////////////////////////////
// Select Macro Bank
/////////////////////////////////////////////////
void Bank123Select(int B, byte c, int Button)
{ uint8_t keycode[6];
  int n; 
  for (n=0; n<6; n++) keycode[n] = 0x00; // set last+1 = NULL 
  
  if (Bank123[B]==1) {for (n=0; n<4; n++) keycode[n] = WinMacro[n][c];  }
  if (Bank123[B]==2) {for (n=0; n<3; n++) keycode[n] = PShopMacro[n][c];}
  if (Bank123[B]==3) {keycode[0] = BsDCode1[c]; keycode[1] = BsDCode2[c]; keycode[2] = BsDCode3[c];}
  if (Bank123[B]==4) {keycode[0] = GuiL;        keycode[1] = KeyR;} // Open Run Box
  if (Bank123[B]==5) {keycode[0] = GuiL;        keycode[1] = Key2;} // Open Powershell 

  usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt100);
  usb_hid.keyboardRelease(HIDKbrd);            delay(dt25);

  if (Bank123[B]==4) {n = 0; while (RunCode[c][n]!=0) { usb_hid.keyboardPress(HIDKbrd, RunCode[c][n]); delay(dt25); // Add \r at end
                                                        usb_hid.keyboardRelease(HIDKbrd);              delay(dt25); 
                                                        n++; if (n>ByteSize) break; } }                // Bank123[B]=4

                                                         
  if (Bank123[B]==5) {n = 0; while (CmdCode[c][n]!=0) { usb_hid.keyboardPress(HIDKbrd, CmdCode[c][n]); delay(dt25); // Add \r at end
                                                        usb_hid.keyboardRelease(HIDKbrd);              delay(dt25); 
                                                        n++; if (n>ByteSize) break; } }                // Bank123[B]=5
                                                        
  if ((Bank123[B]==4)||(Bank123[B]==5))               { usb_hid.keyboardPress(HIDKbrd, '\r');          delay(dt25);
                                                        usb_hid.keyboardRelease(HIDKbrd); }            // Bank123[B]=4,5
  
  status(Labels[LayerAD][Layout-1][Button]); 
}

///////////////////////////////////////////////////////////////////////////
bool ReadSDCard(byte c)   // This is for large textfiles stored on SDCard
                          // Executed by Layout 3 (S1-S24 keys) only
///////////////////////////////////////////////////////////////////////////
{ File SDFile;   
  if (SDNum==0) return false;   // SDCard Files disabled 
  
  if (LayerAxD)  SDFile = SDFS.open(SDName[c], "r");
  if (!LayerAxD) SDFile = LittleFS.open(SDName[c], "r");
  if (SDFile) { while (SDFile.available()) { SDByte = (SDFile.read()); 
                                             usb_hid.keyboardPress(HIDKbrd, SDByte); delay(10);
                                             usb_hid.keyboardRelease(HIDKbrd);       delay(10); } 
                       SDFile.close(); return true; }                                    
         else { status("Text File unavailable"); return false; }
}

////////////////////////////////////////////////
bool MacroKeys(byte c, byte Option)   
////////////////////////////////////////////////     
{ uint8_t keycode[6] = { 0 };   // simultaneous keys pressed in here
  uint8_t a, n, b; 
  int i;
  byte *BPtr; 
  bool MacroKeysOK = false;    
  File f;
  
  for (int n=0; n<6; n++) keycode[n] = 0x00;

  // if (Option==1) { DoMSTAName(c, Layout); Option=3; }   // No use with 1 yet

  if (Option==2) 
     {DoMSTName(c, Layout);
      BPtr = MacroBuff; 
      MacroBuffSize = DoFileBytes(0, MSTName, BPtr, ByteSize, LayerAxD);    
      if (MacroBuffSize==0) return MacroKeysOK; }   

  if (Option==3)          // DonKeys go here with MSTAName includes /nDir
     {BPtr = MacroBuff;   // SendBytes if (macroA) also comes here
      MacroBuffSize = DoFileBytes(0, MSTAName, BPtr, ByteSize, LayerAxD);    
      if (MacroBuffSize==0) return MacroKeysOK; }
  
  if (Option==0)   // Some callers already copied into MacroBuff
  { if (Layout==1) { for (n=0; n<ByteSize; n++) { MacroBuff[n] = Mtr1to12[c][n]; if (MacroBuff[n]==0x00) break; } }
    if (Layout==3) { for (n=0; n<ByteSize; n++) { MacroBuff[n] = Str1to12[c][n]; if (MacroBuff[n]==0x00) break; } }
    if (Layout==4) { for (n=0; n<ByteSize; n++) { MacroBuff[n] = Ttr1to12[c][n]; if (MacroBuff[n]==0x00) break; } } MacroBuffSize = n; }
 
  if (MacroBuffSize==0 && !LargeFile) { if (LayerAxD) status("SDCard File not found"); else status("Flash File not found"); return MacroKeysOK; }
  if (MacroBuffSize==0 && LargeFile)  { status("Large file - use M S T A K nKeys"); return MacroKeysOK; }                                               

  if (MacroBuff[0]==0x2A&&MacroBuff[1]!=0x2A)    // Check for starcodes ignore if double ** - make sure to add 0x00 at end
     { n = 0; while (MacroBuff[n]!=0) { KeyBrdByte[n] = MacroBuff[n]; n++; } KeyBrdByte[n] = 0x00; KeyBrdByteNum = n;
       if (SendBytesStarCodes()) return true; }  // Else continue to process *.... as a text string below   
  if ((MacroBuff[0]==0x2A)&&(MacroBuff[1]==0x2A)) { for (n = 0; n < MacroBuffSize; n++) MacroBuff[n] = MacroBuff[n+1]; MacroBuffSize--; }                                 
   
  if (MacroBuff[0]<0x80) { n=0; while (MacroBuff[n]!=0 && MacroBuffSize>n) { usb_hid.keyboardPress(HIDKbrd, MacroBuff[n]); delay(dt25);
                                                                             usb_hid.keyboardRelease(HIDKbrd);             delay(dt25); 
                                                                             n++; if (n>ByteSize) break; } MacroKeysOK = true; } 
  if (MacroBuff[0]>0x7F) { for (n=0;  n<6; n++) {keycode[n] = MacroBuff[n]; if (keycode[n]==0) break; }         // allow one 0x00 in keycode
                                                              usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(dt25); 
                                                              usb_hid.keyboardRelease(HIDKbrd);                 delay(dt25); 
                                                              MacroKeysOK = true; }                             // Repeat
  return MacroKeysOK;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void DoNKeys(int Button)      
/////////////////////////////////////////////////////////////////////////////////////////////////
// The 996 n01-n996 keys (n changeable to 0-9, aA-zZ) are default white label on Cyan background
// nKeysShow is setto 0 in DoLink and maybe also in DimLCD timeout in main()
/////////////////////////////////////////////////////////////////////////////////////////////////
{ char *ChrPtr;
  uint8_t keycode[6] = { 0 };      // simultaneous keys pressed in here
  int n, s, NameStrLen = 0, StrLen = 0, nStrLen = 0;
  uint8_t a, b; 
  byte *BPtr; 
  bool isLink = false;
  bool isMacro = false;   
  File f;
  
  int c = Button+(Numkeys123)*12;   // c = 0,1,2, 996 used in MacroKeys(c, 3)
  BPtr = MacroBuff; 
  MacroBuffSize = 0;
  MacroBuff[0] = 0x00;

  strcpy(NameStr3, nDir); if (nDir[2]=='/') NameStr3[1] = nChar;  // Default is "/" and "//" use /nChar as folder name such as /n/nKeysfiles 
  strcat(NameStr3, NKeysX[Button]);                               // nDir = /dirname/ ezxcept if default just one / - maximum size 200 char
  
  ///////////////////////////////////////////////////////////////////// Test if 1sr char is m,s,t,a,k or M,S,T,A,K then handle as macro/text file
  // For example with Source m01 construct GUIx in the KeyBrd Editor and press [EXE] then [Up] key to save. A file m01 will be saved 
  // with a length of 3 - press [Lst] to list the contents which is E3 1B 00. Exit the KeyBrd and then press [Cfg][ and then press [Opt]
  // until the nKeys character choice shows - press the option Pad [o] until m shows as the nKey character press [L2][L3][L4] and then 
  // key [M01] and the Windows Menu will open. Press [L1] thn press Pad [n] and then press nKey [m01] and the Windows Menu will also open.
  // mstak-MSTAK files macro/string = tested ok with files m11 and m121 content text S11, on SDCard, when nKeys m11, m121 pressed both send 
  // text "S11" to notepad 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Note that when nKeys = m01-m24 it will not execute the macros associated with the normal M1-M24 keys - it will look for a file /nDir/m01-m24
  // and execute the contents of that file either as a macro or a short text file
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  if (!nKeysShow) { a = NKeysX[Button][0];  strcpy(MSTAName, NameStr3);  // Note this handles m-keys > m99 if via DoMSTA then <99
                    for (n=0; n<10; n++) if (a==MacroChar[n]) { isMacro = true; if (MacroKeys(c, 3)) return; } }  
  
  if (LayerAxD)  f = SDFS.open(NameStr3, "r"); else f = LittleFS.open(NameStr3, "r");
  nStrLen = f.size();
  if (nStrLen<nKeySize) { f.readBytes(nFile, nStrLen); f.close(); nFile[nStrLen] = 0; } // Valid Filename     
  // if (isMacro) strcpy(nFile, NameStr3);                                              // un-redirection - MSTAK large file ?
  
  // SerPr2; Serial.print(nFile); SerPr1; Serial.print(NameStr3); SerPr1; Serial.print(nDir); SerPr1; Serial.print(c); SerPr2;
  
  if (nKeysShow) { status(nFile); Option1 = Option2 = c; MST1 = MST2 = 5; return; }  // Only display its nKeys content -switch off nKeysShow in DoLink
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Check for special commands Start with * eg *ab*n n = 0-9 - ignore * codes if double **
  // For example with Source p01 construct *bb*7 in the KeyBrd Editor and press [Sav] (not [EXE] that will execute the star command)
  // A file p01 will be saved with a length of 6 - press [Lst] to list the contents which is 2A 62 62 2A 37 00. Press the key [p01]
  // which will set the normal LCD rightness to level 7
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((nFile[0]==0x2A)&&(nFile[1]!=0x2A)) 
     {for (n = 0; n < nStrLen-1; n++) KeyBrdByte[n] = nFile[n]; KeyBrdByteNum = nStrLen;
      if (SendBytesStarCodes()) return; else status("*Code incorrect"); return; }   
  if ((nFile[0]==0x2A)&&(nFile[1]==0x2A)) { for (n = 0; n < nStrLen; n++) nFile[n] = nFile[n+1]; nStrLen--; } 
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////// File is a linkefile  
  
  if ((nFile[3]=='L'&&nFile[6]=='k')||(nFile[nStrLen-4]=='L'&&nFile[nStrLen-1]=='k'))  // First or Last part of filename is Lxxk or Link
     { n = 0;                                                                                                   
       LayerAxDSave = LayerAxD;
       strcpy(NameStr1, nFile); 
         
       do { if (LayerAxD) f = SDFS.open(NameStr1, "r"); else f = LittleFS.open(NameStr1, "r"); 
            NameStrLen = f.size(); f.readBytes(inputString, NameStrLen); f.close();        
            NameStr1[0] = 0x00; n++;  // Only do DoLinkStr() once unless L0nXnn loads new NameStr1 
            DoLinkStr(NameStrLen);            
          } while (NameStrLen>0) ;    // Can also stop with 3-char entry such as *** in Link file or n>1000?     
  
       LayerAxD = LayerAxDSave; optionsindicators(0); // Restore or new value
       return;
     }
   
  BPtr = MacroBuff; 
  MacroBuffSize = DoFileBytes(0, nFile, BPtr, ByteSize, LayerAxD);    
     
  if (MacroBuffSize==0 && !LargeFile) { if (LayerAxD) status("nKeys File not found on SDCard"); else status("nKeys File not found on Flash"); return; }
  
  if (MacroBuff[0]>0x7F)                         // Only process maximum of the first 6 bytes
     {for (n=0; n<6; n++)   keycode[n] = 0x00;   // clear
      for (n=0; n<6; n++) { keycode[n] = MacroBuff[n]; if (keycode[n]==0) break; }         
      usb_hid.keyboardReport(HIDKbrd, 0, keycode);  delay(dt25); 
      usb_hid.keyboardRelease(HIDKbrd); 
  return; }  
   
  if (MacroBuff[0]<0x80) StrLen = DoLargeFile(nFile);     // Can do both large and ByteSize files on both SDCard and Flash
  if (StrLen==0) { if (LayerAxD) status("nKeys File not found on SDCard"); else status("nKeys File not found on Flash"); }
                                   
}

/////////////////////////////////////
int DoLargeFile(const char *STRf)
/////////////////////////////////////
{ int n, s, dTime = 5, nStrLen = 0;
  byte b;
  File f;
  bool isCRLF = false;

  if (LayerAxD)  if (SDFS.exists(STRf))     f = SDFS.open(STRf, "r");     else return 0;
  if (!LayerAxD) if (LittleFS.exists(STRf)) f = LittleFS.open(STRf, "r"); else return 0;
  
  nStrLen = f.size(); if (nStrLen<ByteSize) dTime = 10; 
  
  while( f.position() < f.size() )
       { s = f.readBytes((char *)MacroBuff, sizeof(MacroBuff)); 
         n = 0; while (MacroBuff[n]!=0) { b = MacroBuff[n]; isCRLF = false;
                                          while ((b==crlf1 && (CRLF==1||CRLF==3)) || (b==crlf2 && (CRLF>1))) 
                                                { n++; if (n>=s) { isCRLF = true; break; } b = MacroBuff[n]; }
                                          if (!isCRLF) { usb_hid.keyboardPress(HIDKbrd, b); delay(dTime);
                                                         usb_hid.keyboardRelease(HIDKbrd);  delay(dTime); }
                                          n++; if (n>=s) break; 
                                        } 
       } 
       
  f.close();
  return nStrLen;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DoMacroButtons(int Button, byte c, byte Option)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Instruction List Simple: (Only execute 0-9,a-d exit if 0, can be in any order or length (set to 12 currently) such as 6 1 3 0 )
// 1=SDCardTextFiles 2=Flash+M-L 3=Flash+L-M 4=SDCard+M-L 5=SDCard+L-M 6=Do1 7=Do2 8=Do3 9=Bank123 0=Exit 
// a=Flash+M b=Flash+L c=SDCard+M d=SDCard+L Note: a=0x62=97->after -48 a b c d = 49 50 51 52
// For example 1 2 5 9 0   is the same as Layout=3 previously
//             2 5 9 0     is the same as Layout=4 previously 
//             2 5 6 7 9 0 is the same as Layout=1 previously  
//
// Instruction List Expanded: (Only execute if >20) not yet implemented
// For example 11  102 203 31 41 90    is the same as Layout=3 previously
//             102 103 31  41 90       is the same as Layout=4 previously 
//             102 103 42  43 31 41 90 is the same as Layout=1 previously  
//  
// SDCardTextFiles  Bank123      CodedAction   Exit      Flash-Link/Macro  SDCard-Link/Macro  
// 20-29            30-39        40-49         90=Exit   100-109           200-209
// 0=Ignore         0=Ignore     0=Ignore      0=Done    0=Macro           0=Macro
// 1=Execute        1=Selected   1=Execute               1=Link            1=Link
//                               2-9=Do1-Do8             2=Macro -> Link   2=Macro -> Link
//                                                       3=Link  -> Macro  3=Link  -> Macro                 
//                                                      (UC/lc combinations future expansion)                                                                      
//////////////////////////////////////////////////////////////////////////////////////////////
  {byte Do2 = 2;    // 2 = A-D switch switch between two macros same name on both SDCard and FlashMem, 0 = only do current macro loaded
   int a, b, n = 0;

   if (!iList) goto iListOff;    // do it without instruction list
   
   b = (Layout-1) - (Layout>2); // Layout 1,3,4 = 0,1,2
   if (Layout==3&&MacroS1S12[c]==2) Do2 = 0;     // Do *fs* for current session
   if (Layout==4&&MacroT1T12[c]==2) Do2 = 0;     // Do *ft* for current session
   
  do { a = MacroInstructionList[b][n]; if (a>9) a = a-39; // a-d=10-13 from a=0x62=97->after -48 a b c d = 49 50 51 52
       switch (a) {
       case 0:   break;
       case 1:  if (LayerAxD)  if (ReadSDCard(c))           a=0; break; // if (!LayerAxD) will also execute break which is correct
       case 2:  if (!LayerAxD) if (MacroKeys(c, Do2))     { a=0; break; } else { DoKeyMST(c); if (LinkOk) a=0; } break;    
       case 3:  if (!LayerAxD) { DoKeyMST(c); if (LinkOk) { a=0; break; } else if (MacroKeys(c, Do2))     a=0; } break;  
       case 4:  if (LayerAxD)  if (MacroKeys(c, Do2))     { a=0; break; } else { DoKeyMST(c); if (LinkOk) a=0; } break;    
       case 5:  if (LayerAxD)  { DoKeyMST(c); if (LinkOk) { a=0; break; } else if (MacroKeys(c, Do2))     a=0; } break;  
       case 10: if (!LayerAxD) if (MacroKeys(c, Do2))       a=0;   break; // if (LayerAxD) will also execute break which is correct
       case 11: if (!LayerAxD) { DoKeyMST(c); if (LinkOk)   a=0; } break; 
       case 12: if (LayerAxD)  if (MacroKeys(c, Do2))       a=0;   break; // if (!LayerAxD) will also execute break which is correct
       case 13: if (LayerAxD)  { DoKeyMST(c); if (LinkOk)   a=0; } break; 
       case 6:  if (Option==1) if (LayerAD==0) { DoAdminCmd();        a=0; } break;
       case 7:  if (Option==2) if (LayerAD==0) { DoAdminPowershell(); a=0; } break;
       case 9:  Bank123Select( b, c, Button); a=0; break; }
       n++; 
     } while (a>0&&n<iListMax);

  return;     // iList = true;  
  iListOff:   // iList = false;
  
  if (Layout==3)                                                                                     // Keys S1-S24
    { if (LayerAxD) if (ReadSDCard(c)) return;                                                       // If orange A-D first do any SDCard Textfiles 
      if (MacroS1S12[c]==2) Do2 = 0;                                                                 // S key(s) FillStr done execute first
      if (!LayerAxD) {if (MacroKeys(c, Do2))   return;  else { DoKeyMST(c); if (LinkOk) return; } }  // FlashMem A-D 1st Macro then MacroLink
      if (LayerAxD)  {DoKeyMST(c); if (LinkOk) return;  else if (MacroKeys(c, Do2))     return; }    // SDCard A-D 1st MacroLink then Macro              
      Bank123Select(1, c, Button);    }                                             
               
 if (Layout==4)                                                                                      // Keys T1-T24
    { if (MacroT1T12[c]==2) Do2 = 0;                                                                 // T key(s) FillStr done execute first
      if (!LayerAxD) {if (MacroKeys(c, Do2))   return;  else { DoKeyMST(c); if (LinkOk) return; } }  // FlashMem A-D 1st Macro then MacroLink
      if (LayerAxD)  {DoKeyMST(c); if (LinkOk) return;  else if (MacroKeys(c, Do2))     return; }    // SDCard A-D 1st MacroLink then Macro              
      Bank123Select(2, c, Button);    }                                          
  
 if (Layout==1)                                                                                      // Keys M1-M24
    { if (!LayerAxD) {if (MacroKeys(c, Do2))   return;  else { DoKeyMST(c); if (LinkOk) return; } }  // FlashMem A-D 1st Macro then MacroLink
      if (LayerAxD)  {DoKeyMST(c); if (LinkOk) return;  else if (MacroKeys(c, Do2))     return; }    // SDCard A-D 1st MacroLink then Macro              
      if (Option==1) {if (LayerAD==0) { DoAdminCmd(); return;        }  }
      if (Option==2) {if (LayerAD==0) { DoAdminPowershell(); return; }  }
      Bank123Select(0, c, Button);    } 
  
}

/////////////////////////////
void buttonpress(int Button)
/////////////////////////////
{
  uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here
  uint8_t i, m, n, a, b, c, x, y; // 
  uint8_t CfgOK;                  // Do Cfg actions
  uint8_t modifiers = 0;          // Alt Shift Control Gui modifiers b7 RG| RA| RS| RC| LG| LA| LS| LC| b0  
  byte *BPtr;
  char Bank123X[] = "Bank X  ";           // X=m,s,t Bank 1-5
  char MST[] = "MST";                     // 0-2 -> MST
  char SDCardStatus[] = "SDCard Set X";   // X=1-3 4-9=U-Z MST mst = 10-12 Kk = 20,21
  bool sdCard = false;                    // For Src-Dest Orange-White Flash-SDCard combinations
  
  if ( !usb_hid.ready() ) return; // assume it includes -> if (tud_hid_ready()) 
  
  for (n=0; n<6; n++) keycode[n] = 0x00; // set last+1 = NULL
  
  ////////////////////////////////////////////////////////////// 
  // Skip 1st any-key keypress which re-activates backlight
  // Disable the keypress skip with *ks* toggle KeySkip on-off
  //////////////////////////////////////////////////////////////
  LastMillis = millis();                                   // Reset Backlight Timer if any key pressed                                        
  if (!BackLightOn)                                        // Keypress only active if backlight on
     {if (NormVal==0) digitalWrite(LCDBackLight, HIGH);    // Backlight Full ON
                else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON
      BackLightOn = true;                                  //
      if (KeySkip) return; }                               // Option to ignore first keypress if dimmed state
  
  // Macro Timers must either be executed (buttons 1 or 0,2,3,4,6,7,8,9,10) or cancelled (button 5 or 11) or stopped (button 1) 
  // If Option BLOnOff is ON then pressing the black button (11) will switch Backlight ON/OFF without leaving MacroTimer Config Screen 
  if (MacroTimerDisp) {if (Button==11) { if (!BLOnOff) { status("MacroTimer Config Exit"); MacroTimerDisp=false; ConfigButtons(1); return;}  
                                         if (BLOnOff)  { digitalWrite(LCDBackLight, byte(BLOnOffToggle)); BLOnOffToggle = !BLOnOffToggle; return;} } }
                                      
  // Power Keys must either be executed (buttons 0,2,3,4,6,7,8,9,10) or cancelled (button 5 or 11) or stopped (button 1)
  // If Option BLOnOff is ON then pressing the black button (11) will switch Backlight ON/OFF without leaving Power Config Screen
  if (PowerKeys) {if (Button==11) { if (!BLOnOff) { status("Power Keys Config Exit"); PowerKeys=false; ConfigButtons(1); return;}  
                                    if (BLOnOff)  { digitalWrite(LCDBackLight, byte(BLOnOffToggle)); BLOnOffToggle = !BLOnOffToggle; return;} } }

  if (Button==16||Button==1)             ; else if (VarNum>0) VarNum=0;  // Reset [Key] key to not-pressed
  if (Button==16||Button==15||Button==0) ; else if (OptNum>0) OptNum=0;  // Reset [Opt] key to not-pressed
  //////////////////////////////////////////////////////////////////////////////////////////////  
  // 5 Config Pads Blue Gray Purple Gray Red button > 11
  // Handle 1st then return - as if button = 0-11
  // Pad Keys switch off all other KeyBrds 
  ///////////////////////////////////////////////////////////////////////////////////////////////
  if ((Button>11)&&(Layout==2)) 
     { if (OptNum==1 && Button!=16) OptNum=0; // Switch off Pad[o] nKeys char select if Pad[n] nKeys is pressed
       PadKeys = true;        
       if (Button==12) { if (NumKeys) { if (nKeys)  if (nKeysShow)  { nChar = nKeysAllChar[nCharAll]; nCharAll++; if (nCharAll>61) nCharAll=0;  
                                                                      NumKeysChange(); WriteConfig1Change = true; return; }
                                                    if (!nKeysShow) { if (Numkeys123<nKeysPage-1) Numkeys123++; else Numkeys123 = 0; 
                                                                      NumKeysChange(); ConfigButtons(1); return; }    // NumKeys max 8 nKeys max 83 pages
                                        if (!nKeys) { if (Numkeys123<NumKeysPageMax-1) Numkeys123++; else Numkeys123 = 0;  
                                                      NumKeysChange(); ConfigButtons(1); return; } }
                         Math = !Math;           PadKeysState(Button-11, !Math);      return; }      
       if (Button==14) { if (NumKeys && nKeys) { nKeysShow = !nKeysShow;  ConfigButtons(5); return; }
                         MouseK = !MouseK;       PadKeysState(Button-11, !MouseK);    return; }
       if (Button==15) { if (OptNum==8) { SDCardArr[2] = !SDCardArr[2]; 
                                          if (SDCardArr[2]) { SDCardArr[1] = SDCardStatus[11] = SD2[SDNum]; } 
                                                       else { SDCardArr[1] = SDCardStatus[11] = SD1[SDNum]; } 
                                          if (SDNum>0) { SDCardSelectFiles(0); status(SDCardStatus); } else status("SDCard Disabled"); 
                                          SDNumChange = true; optionsindicators(0); return; }
                         if (Kbrd) {KeyBrdDirect = !KeyBrdDirect; optionsindicators(0);  
                                    if (KeyBrdDirect) status("KeyBoard Direct ON"); else status("KeyBoard Direct OFF"); return;}
                         NumKeys = !NumKeys; PadKeysState(Button-11, !NumKeys); return; }   
       if (Button==16) { if (VarNum>0) { if (VarNum<9) SaveVar = true; else return;  // press [Sav] to save
                                         if (VarNum==1) { BsDNum++; if (BsDNum>=BSDMax) BsDNum=0; status(BsDName[BsDNum]); }     // [Del]ete Key etc
                                         if (VarNum==2) { RetNum++; if (RetNum>=BSDMax) RetNum=0; status(BsDName[RetNum]); }     // [Ret]urn Key etc
                                         if (VarNum>2&&VarNum<6) { XFiles = true; y=VarNum-3; XNum[y]++; SaveX1X6 = true;        // 3,4,5
                                                                   if (XNum[y]>=BSDMax) XNum[y]=0; status(BsDName[XNum[y]]); }   // Layout1 Keys1-3
                                         if (VarNum>5&&VarNum<9) { XFiles = true; y=VarNum-3; XNum[y]++; SaveX1X6 = true;        // 6,7,8
                                                                   if (XNum[y]>=BSDMax) XNum[y]=0; status(BsDName[XNum[y]]); }   // Layout3+4 Keys1-3                                             
                                         ConfigButtons(1); return; }  
                         if (OptNum>0) { if (OptNum==1) { nKeysCurr[0] = nChar = nKeysCharSet[nCharNum]; nCharNum++; if (nCharNum>9) nCharNum=0;  // nKeyChar <- 10-list                
                                                          NumKeysChange(); WriteConfig1Change = true; status(nKeysCurr); return; }   
                                         if (OptNum==2) { KeyHeldEnable = !KeyHeldEnable; Config1[2] = KeyHeldEnable; WriteConfig1(0); ConfigButtons(1); }
                                         if (OptNum==3) { SaveLayout++; if (SaveLayout>4) SaveLayout = 1; status(StartUpLayout[SaveLayout]); } 
                                         if (OptNum==4||OptNum==5||OptNum==6) 
                                            {Bank123[OptNum-4]++; if (Bank123[OptNum-4]>5) Bank123[OptNum-4]=1;
                                             Bank123X[5] = MST[OptNum-4]; Bank123X[7] = Bank123[OptNum-4]+48; Bank123Change = true; status (Bank123X); }
                                         if (OptNum==7) { MacroUL = !MacroUL; SwitchMacroUL(1); ConfigButtons(1); return; }
                                         if (OptNum==8) { SDNumChange = true; SDNum++; if (SDNum>21) SDNum=0; SDCardArr[0] = SDNum; 
                                                          if (SDCardArr[2]) { SDCardArr[1] = SDCardStatus[11] = SD2[SDNum]; } 
                                                                       else { SDCardArr[1] = SDCardStatus[11] = SD1[SDNum]; } 
                                                          if (SDNum>0) { SDCardSelectFiles(0); status(SDCardStatus); } else status("SDCard Disabled"); 
                                                          optionsindicators(0); }     
                                         if (OptNum==9) { DoFileList(); } 
                                         if (OptNum>9)   { strcpy(NameStr3, " OFF"); strcpy(NameStr2, "LabelX"); }
                                         if (OptNum==10) { Config1[72] = MLabel = !MLabel; m = 1; NameStr2[5] = 'M'; if (MLabel) strcpy(NameStr3, " ON"); }
                                         if (OptNum==11) { Config1[73] = SLabel = !SLabel; m = 3; NameStr2[5] = 'S'; if (SLabel) strcpy(NameStr3, " ON"); }
                                         if (OptNum==12) { Config1[74] = TLabel = !TLabel; m = 4; NameStr2[5] = 'T'; if (TLabel) strcpy(NameStr3, " ON"); }
                                         if (OptNum>9)   { DoMSTLabel(0, m); WriteConfig1(0); strcat(NameStr2, NameStr3); status(NameStr2); }                                                                 
                                         return; }    // Option>0 
                         if (Kbrd) {SrcDst++; if (SrcDst>3) SrcDst=0; 
                                    if (SrcDst==0) LayerAxD = false; if (SrcDst==3) LayerAxD = true;  // All White or all Orange                                     
                                    optionsindicators(0); return; }                                   // SrcDst 0 W->W 1 W->O 2 O->W 3 O->O Copy                                    
                         if (NumKeys) { if (nKeys)  { if (nKeysShow)  { nChar = nKeysAllChar[nCharAll]; if (nCharAll>0) nCharAll--; else nCharAll = 61;  
                                                                        NumKeysChange(); WriteConfig1Change = true; return; }  
                                                      if (!nKeysShow) { if (Numkeys123>0) Numkeys123--; else Numkeys123 = nKeysPage-1; }  } 
                                        if (!nKeys) { if (Numkeys123>0) Numkeys123--; else Numkeys123 = NumKeysPageMax-1; }   // NumKeys max 8 pages nKeys max 83 pages        
                                        NumKeysChange(); ConfigButtons(1); return; }
                         // if (Media) {ToneControls = !ToneControls; ConfigButtons(1); return;}
                         /////////////////////////////////////////////////// Toggle Caplock Numlock in 4 coombinations
                         PadCapsNumN++; if (PadCapsNumN>3) PadCapsNumN = 0;
                         keycode[0] = PadCapsNum[0][PadCapsNumN]; keycode[1] = PadCapsNum[1][PadCapsNumN];
                         usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50);
                         usb_hid.keyboardRelease(HIDKbrd);            delay(dt25);
                         return; }   // Button=16 = Pad[o]    
       Kbrd = !Kbrd; SendBytesEnd(0); PadKeysState(Button-11, !Kbrd); return;  }  // do remaining pad   

  if ((Button>11)&&(Layout!=2))   // 5 Navigation Pads Blue Gray Purple Purple Blue button > 11                                                    
     {if (Button==16) { if (LayerAD<3) LayerAD++; else LayerAD = 0;}                  
      if (Button==12) { if (LayerAD>0) LayerAD--; else LayerAD = 3;} 
      if (Button==16||Button==12) {PadKeys = true; LayerADLetter[0] = 65 + LayerAD; optionsindicators(0); ConfigMacroButtons(); return; }
      if ((Button==13&&Layout==1)||(Button==14&&Layout==3)||(Button==15&&Layout==4))              // If own Padkey pressed in own Layout
         { LayerAxD = !LayerAxD; if (LayerAxD) status("SDCard Used"); else status ("Flash Used"); optionsindicators(0); return; } 
      Layout = Button-11-(Button==13); PadKeys = true; status(" "); ConfigButtons(1); optionsindicators(0); return; }   // pad MST = button 13,14,15 = Layout 0,2,4 
  
  n = 0;                                 // for the while loops                                            
  if (PrevButton!=Button) KeyBrd123 = 0; // Start at a not b or c if other button previously used
  PrevButton = Button;   
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  //                StrButton[button]             4  5  6     8  9  10                 4  5  6      8  9  10
  //  M1-M6 M7-M12 etc            c               0  1  2     3  4  5                  6  7  8      9  10 11
  //           byte StrButton[48] = { 0, 0, 0, 0, 0, 1, 2, 0, 3, 4, 5,  0, 0, 0, 0, 0, 6, 7, 8,  0, 9, 10,11,0, 
  //                                  0, 0, 0, 0, 12,13,14,0, 15,16,17, 0, 0, 0, 0, 0, 18,19,20, 0, 21,22,23, 0  };
  c = StrButton[Button + (LayerAD*12)];   // c = 0 - 23 map button 0-11 to macrokeys 0-23 -> 1-24
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // reset if not [Cfg] and [Home] [UpArrow] [PageUp] [<] [Cfg] [>] [End] [DwnArrow] [PageDown] key pressed
  // StrButton[button]      0  1  2  3  4  5  6  7  8  9  10 11
  //             CfgOK      x  x  x     x  x  x     x  x  x 
  // byte CfgButton[12] = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
  CfgOK = CfgButton[Button];

  if (XFiles==true) x = Button + Layout - (Layout==1) - (Layout==4);  // Used in DoBsdCodes make Button in 2 groups: Layout1 and Layout2+3
  
  if ((Layout==2) && (CfgOK==0) && (ConfigKeyCount>0)) {ConfigKeyCount = 0; status(" "); } 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
                                                           
  if (Layout!=2) {      // Layout 1 or 3 or 4
    switch(Button)  {   // 0 - 11 
    case 0: ///////////////////////////////////////////////// Cut = control + x  Cannot do case 0 ... 2:
      if (XFiles) { DoBsdCodes(XNum[x]); break; }  
                       
      keycode[0] = KeyX; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(dt50); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = KeyX;  
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      //usb_hid.keyboardRelease(HIDKbrd);  
      break;
    case 1: ///////////////////////////////////////////////// Copy = control + c
      if (XFiles) { DoBsdCodes(XNum[x]); break; }
      
      keycode[0] = KeyC; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(dt50); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = KeyC; 
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      //usb_hid.keyboardRelease(HIDKbrd);  
      break;
    case 2: ////////////////////////////////////////////////// Paste = control + v
      if (XFiles) { DoBsdCodes(XNum[x]); break; }
      
      keycode[0] = KeyV; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(dt50); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = KeyV;  
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      //usb_hid.keyboardRelease(HIDKbrd);  
      break;
    case 3: //////////////////////////////////////////////// Volume Increase or Delete BackSpace Tab Insert

      if (!VolOn) { DoBsdCodes(BsDNum);  break; } // Delete BackSpace Tab AltTab Insert Esc PScr Num Caps Scroll etc 
   
      if (Brightness) usb_hid.sendReport16(HIDCons, 0x006F);
                 else usb_hid.sendReport16(HIDCons, VolUp);  delay(dt50); 
      usb_hid.sendReport16(HIDCons, 0);      break;
      
    case 4: /////////// 12 keys handled here: M1 M7 M13 M19 - S1 S7 S13 S19 - T1 T7 T13 T19 - select by [Layout=1,3,4 and LayerAD=0,1,2,3        
         DoMacroButtons(Button, c, 0); break;                                            
    case 5: /////////// 12 keys handled here: M2 M8 M14 M20 - S2 S8 S14 S20 - T2 T8 T14 T20 - select by [Layout=1,3,4 and LayerAD=0,1,2,3 
         DoMacroButtons(Button, c, 1); break;      
    case 6: /////////// 12 keys handled here: M3 M9 M15 M21 - S3 S9 S15 S21 - T3 T9 T15 T21 - select by [Layout=1,3,4 and LayerAD=0,1,2,3 
         DoMacroButtons(Button, c, 0); break;        
    case 7: //////////////////////////////////////////////// Volume Mute and change Layouts
      if (!MuteOn) { Layout++; if (Layout>4) Layout=1; ConfigButtons(1); optionsindicators(0); break; } 

      usb_hid.sendReport16(HIDCons, VolMute); delay(dt25);
      usb_hid.sendReport16(HIDCons, 0);       delay(dt25); break;
      
    case 8: /////////// 12 keys handled here: M4 M10 M16 M22 - S4 S10 S16 S22 - T4 T10 T16 T22 - select by [Layout=1,3,4 and LayerAD=0,1,2,3 
         DoMacroButtons(Button, c, 0); break;                                                                                                    
    case 9: /////////// 12 keys handled here: M5 M11 M17 M23 - S5 S11 S17 S23 - T5 T11 T17 T23 - select by [Layout=1,3,4 and LayerAD=0,1,2,3
         DoMacroButtons(Button, c, 2); break; 
    case 10: /////////// 12 keys handled here: M6 M12 M18 M24 - S6 S12 S18 S24 - T6 T12 T18 T24 - select by [Layout=1,3,4 and LayerAD=0,1,2,3         
         DoMacroButtons(Button, c, 0); break;    
    case 11: /////////////////////////////////////////// Volume decrease or Return
      if (!VolOn) { DoBsdCodes(RetNum);  break; } // Delete BackSpace Tab AltTab Insert Esc PScr Num Caps Scroll etc

      if (Brightness) usb_hid.sendReport16(HIDCons, 0x0070);
                 else usb_hid.sendReport16(HIDCons, VolDown);  delay(dt50); 
      usb_hid.sendReport16(HIDCons, 0);       break; 
      
  }   // switch Button
  return; }   // if Layout = 1,3.4
  
  //////////////////////////////////// if Layout==2
  if (Layout==2) {  
    switch(Button) {
    case 0: // HOME - Media Previous //////////////////////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('T', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer1 = !MacroTimer1; MacroTimer18 = MacroTimer18 || MacroTimer1;
                        if (MacroTimer1) { TimeRepeatPrev = millis(); status("[R-T] Repeat Macro Timer started"); }
                                                                 else status("[R-T] Repeat Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }
    
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break; }

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_LEFT);  delay(dt50);             // ***Mouse
                   usb_hid.mouseButtonRelease(RID_MOUSE);                   delay(dt50); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, StopMed); delay(dt50);
                  usb_hid.sendReport16(HIDCons, 0); break; }      

      if (NumKeys) { if (nKeys) { DoNKeys(Button); Serial.println(Button); }
                          else {keycode[0] = BckS;     // Backspace
                                keycode[1] = 0x00;     // NULL
                                usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt25);
                                usb_hid.keyboardRelease(HIDKbrd);         }  break;          }                   
      
      // 0 not pressed   1 Upper/Lower case Macro filenames   2 KeyHeldEnable   3 L1-L4 StartUpSelect 
      // 4,5,6 MST Bank123 Select   7 Select nKeys char  8 SDCard File Set   9 List Files   
      if (ConfigKeyCount==1) {OptNum++; if (OptNum>12) OptNum=1; status(OptStatus[OptNum]); break; } 

      keycode[0] = KeyHome;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 1: // ARROW UP ////////////////////////////////////////////////
    
      if (DoUpKey)      // Set by SendBytes Make last param of DoFileBytes = LayerAxD to save to SDCard=1 or to Flash=0
         { DoMSTAName(Option2, MST2); 
           if (MST2==0) { for (n=0; n<MacroBuffSize; n++) { Mtr1to12[Option2][n] = MacroBuff[n]; } status("M Macro OK"); 
                          MacroSizeM1M12[Option2] = MacroBuffSize; MacroM1M12[Option2] = 1; BPtr = Mtr1to12[Option2]; } 
           if (MST2==1) { for (n=0; n<MacroBuffSize; n++) { Str1to12[Option2][n] = MacroBuff[n]; } status("S Macro OK"); 
                          MacroSizeS1S12[Option2] = MacroBuffSize; MacroS1S12[Option2] = 1; BPtr = Str1to12[Option2];  } 
           if (MST2==2) { for (n=0; n<MacroBuffSize; n++) { Ttr1to12[Option2][n] = MacroBuff[n]; } status("T Macro OK");            
                          MacroSizeT1T12[Option2] = MacroBuffSize; MacroT1T12[Option2] = 1; BPtr = Ttr1to12[Option2];  }
           if (SrcDst==1||SrcDst==3) sdCard = true;  if (SrcDst==2||SrcDst==0) sdCard = false;  // For Destination 0,2=Flash 1,3=SDCard
           DoFileBytes(1, MSTAName, BPtr, MacroBuffSize, sdCard); DoUpKey = false; }
    
      if (PowerKeys) { DoPowerKeys ('a', PowerKeysMenu, Button); break; }

      if (Media && ToneControls) {usb_hid.sendReport16(HIDCons, BassUp); delay(dt50);
                                  usb_hid.sendReport16(HIDCons, 0); break; }
                  
      if (MacroTimerDisp) 
         { MacroTimer8 = MacroTimer7 = MacroTimer6 = MacroTimer5 = Macrotimer4 = MacroTimer3 = Macrotimer2 = MacroTimer1 = MacroTimerDisp = MacroTimer18 = false;
            MacroTimerOK = false; status("All Macro Timers stopped"); ConfigButtons(1); optionsindicators(Button); break; }
      
      if (Math) {MakeMathStr(Button); break; }
      
      if (Kbrd) {MakeStr(Button); break; }

      if (MouseK) {usb_hid.mouseMove(RID_MOUSE, 0, -1*MouseDelta);  delay(dt50); break; }
               
      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                 }  break;        }    
                     
      // 0 not pressed  1,2 [Del][Ret]  3-5 MKey1-3   6-8 STKey1-3 changes with bottom Pad (o)
      if (ConfigKeyCount==1) {VarNum++; if (VarNum>8) VarNum=1; status(VarStatus[VarNum]); break; }
                        
      keycode[0] = ArrUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 2: // PAGE UP /////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('T', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer3 = !MacroTimer3; MacroTimer18 = MacroTimer18 || MacroTimer3;
                        if (MacroTimer3){ TimeOnceofPrev = millis(); status("[O-T] Oneshot Macro Timer started"); }
                                                                else status("[O-T] Oneshot Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }   
    
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break;}

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_RIGHT); delay(dt50);           // ***Mouse
                   usb_hid.mouseButtonRelease(RID_MOUSE);                   delay(dt50); break; }

      if (Media && ToneControls) {usb_hid.sendReport16(HIDCons, TrebUp); delay(dt50);
                                  usb_hid.sendReport16(HIDCons, 0); break; }
      
      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                 }  break;        }                   
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              VolOn = !VolOn; 
                              //GetSysInfo(1); // Save state on Flash - only save if [Sav] pressed
                              if (VolOn) status("Volume V+V- On"); else status("Volume V+V- Off");
                              ConfigButtons(1); break; }

      keycode[0] = PageUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 3: // V+ or Delete or Escape(Math) or Execute(Keybrd) //////////////////////////////////////
      if (PowerKeys) { PowerClock=1; status("Restart on Clock"); DoPowerKeys ('R', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer7 = !MacroTimer7; MacroTimer18 = MacroTimer18 || MacroTimer7;
                        if (MacroTimer7) { timerEnable = true;  status("[RcT] Repeat Macro Countdown Clock started"); }
                                    else { timerEnable = false; status("[RcT] Repeat Macro Countdown Clock stopped"); }
                        ConfigButtons(1); optionsindicators(Button); break; }
    
      if (Math) {Math = false; status("Math exit");  MathX = 0; Math123 = 0; ConfigButtons(1); break;}
    
      if (Kbrd) {if (KeyBrdX==3) {MakeStr(Button); break;}  // [EXE] change to [Snd] Send
                 if (KeyBrdDirect)  {usb_hid.keyboardPress(HIDKbrd, KeyBrdByte[KeyBrdByteNum]); delay(dt50);     
                                     usb_hid.keyboardRelease(HIDKbrd);                          delay(dt50); break; }
                 Kbrd = false; status((char *)KBDisp);                    
                 if (KeyBrdByteNum>0) {SendBytes();}          // Execute macro + exit keybrd
                 SendBytesEnd(1); ConfigButtons(1); break;}   // Clear incase wrong symbols added or no char added

      if (MouseK) {usb_hid.mouseScroll(RID_MOUSE, MouseScrollAmount, 0); break; }         
       
      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                 }  break;        }

      if (!VolOn) { DoBsdCodes(BsDNum);  break; } // Delete BackSpace Tab AltTab Insert Esc PScr Num Caps Scroll etc 
        
      usb_hid.sendReport16(HIDCons, VolUp); delay(dt50);
      usb_hid.sendReport16(HIDCons, 0);     break;
      
    case 4: // ARROW LEFT //////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('t', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { Macrotimer2 = !Macrotimer2; MacroTimer18 = MacroTimer18 || Macrotimer2;
                        if (Macrotimer2) { timeRepeatPrev = millis(); status("[R-t] Repeat Macro Timer started"); } 
                                                                 else status("[R-t] Repeat Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }
    
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break;}
      
      if (MouseK) {usb_hid.mouseMove(RID_MOUSE, -1*MouseDelta, 0);  delay(dt50); break; } 

      if (Media) {usb_hid.sendReport16(HIDCons, PrevMed); delay(dt50);
                  usb_hid.sendReport16(HIDCons, 0); break; }      
    
      if (NumKeys) { if (nKeys) DoNKeys(Button);
                          else {usb_hid.keyboardPress(HIDKbrd, '\r'); delay(dt50);
                                usb_hid.keyboardRelease(HIDKbrd);  }  break;        }      
                   
      if (ConfigKeyCount==1) { LayerAD++; if (LayerAD==4) { LayerAxD = !LayerAxD;  LayerAD = 0; }   // Cycle A-D with X off then A-D with X on                                      
                               LayerADLetter[0] = 65 + LayerAD; optionsindicators(0); ConfigButtons(1); LayerADChange = true; break; }

      keycode[0] = ArrLeft;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 5: // CONFIG //////////////////////////////////////////////////
      if (PowerKeys) { status("Power Keys exit"); PowerKeys=false; ConfigButtons(1); break; }

      if (MacroTimerDisp) { status("MacroTimer Display Exit"); MacroTimerDisp=false; ConfigButtons(1); break; } // Timers still running - enter [Cfg][mCT] then [Stp] to stop
    
      if (Math) {MakeMathStr(Button); break; }
      
      if (Kbrd) {MakeStr(Button); break;}
      
      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                 }  break;        }

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_MIDDLE); delay(dt50);                 
                   usb_hid.mouseButtonRelease(RID_MOUSE);                    delay(dt50); break; }
     
      // Now includes GetSysInfo(5); and GetSysInfo(3);           
      if (ConfigKeyCount==1) { if (XFiles&&SaveX1X6) GetSysInfo(4); MediaChange = true; // Save Vol Mute Media state if [Sav] pressed
                               GetSysInfo(0); status("Config and Files Saved"); ConfigKeyCount = 0; }     // Key = [Sav]               
                        else { status("Start Key Vol A-D Save Mute mCT ROf Media"); ConfigKeyCount++ ; }  // Key = [Cfg]
      ConfigButtons(1);  break;
      
    case 6: // ARROW RIGHT /////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys('t', PowerKeysMenu, Button); break; }
      
      if (MacroTimerDisp) { Macrotimer4 = !Macrotimer4; MacroTimer18 = MacroTimer18 || Macrotimer4;
                        if (Macrotimer4) { timeOnceofPrev = millis(); status("[O-t] Oneshot Macro Timer started"); }
                                                                 else status("[O-t] Oneshot Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }
      
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break;}

      if (MouseK) {usb_hid.mouseMove(RID_MOUSE, MouseDelta, 0);  delay(dt50); break; } 

      if (Media) {usb_hid.sendReport16(HIDCons, NxtMed); delay(dt50);
                  usb_hid.sendReport16(HIDCons, 0); break; }
      
      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                 }  break;        }      

      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              MuteOn = !MuteOn; 
                              //GetSysInfo(1); // Save state on Flash
                              if (!MuteOn) status("Volume Mute Off"); else status("Volume Mute On");
                              ConfigButtons(1); break; }

      keycode[0] = ArrRight;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 7: // Vol Mute ///////////////////////////////////////////////////////////////
      if (PowerKeys) { PowerClock=2; status("PowerOff on Clock"); DoPowerKeys ('O', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer8 = !MacroTimer8; MacroTimer18 = MacroTimer18 || MacroTimer8;
                        if (MacroTimer8) { timerEnable = true; status("[OcT] Oneshot Macro Countdown Clock started");  }                        
                                    else { timerEnable = false; status("[OcT] Oneshot Macro Countdown Clock stopped"); }
                        ConfigButtons(1); optionsindicators(Button); break; }
             
      if (Math) {if (MathX<3) MathX++; else MathX=0; 
          Math123=0; ConfigButtons(1); break;} // Changes 4 Math KeyBrds from Greek abc -> ABC -> Calculus -> Logic
        
      if (Kbrd) {if (KeyBrdX<4) KeyBrdX++; else KeyBrdX=0; // [NXT] Changes 5 KeyBrds from abc - ABC - 012 - *^% - SHFT
          KeyBrd123=0; ConfigButtons(1); break;}           // For new page start at first of 3 char group per key  

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_LEFT);  delay(dt50);             // ***Mouse
                   usb_hid.mouseButtonRelease(RID_MOUSE);                   delay(200); 
                   usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_LEFT);  delay(dt50);        // ***Mouse
                   usb_hid.mouseButtonRelease(RID_MOUSE);                   delay(dt50);
                   break; }
    
      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                 }  break;        }

      if (!MuteOn) { Layout++; if (Layout>4) Layout=1; ConfigButtons(1); optionsindicators(0); break; } 

      usb_hid.sendReport16(HIDCons, VolMute); delay(dt50);
      usb_hid.sendReport16(HIDCons, 0);       break;
      
    case 8: // END //////////////////////////////////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('r', PowerKeysMenu, Button); break;} 

      if (MacroTimerDisp) { MacroTimer5 = !MacroTimer5; MacroTimer18 = MacroTimer18 || MacroTimer5;
                        if (MacroTimer5) { alarmEnable = true;  status("[R-C] Repeat Macro Clock started"); }
                                    else { alarmEnable = false; status("[R-C] Repeat Macro Clock stopped"); }
                        ConfigButtons(1); optionsindicators(Button); break; }

      if (Math) {MakeMathStr(Button); break; }
      
      if (Kbrd) {MakeStr(Button); break;}

      if (Media) {usb_hid.sendReport16(HIDCons, PlayMed); delay(dt50);
                  usb_hid.sendReport16(HIDCons, 0); break; }

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_BACKWARD);  delay(dt50);
                   usb_hid.mouseButtonRelease(RID_MOUSE);                       delay(dt50); break; }

      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                 }  break;        }                   

      // Power Restart Logoff Timer options
      if (ConfigKeyCount==1) { ConfigKeyCount--; 
                               if (MacroTimerOK) { MacroTimerDisp = !MacroTimerDisp; }
                                            else { status("No Macro -> Timer Link"); ConfigButtons(1); break; }
                               if (MacroTimerDisp) status("[R-T/C]Repeat Timer/Clock [O-T/C]Oneshot"); else status("MacroTimer Exit");
                               ConfigButtons(1); break; }

      keycode[0] = KeyEnd;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 9: // ARROW DOWN //////////////////////////////////////////////
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break;}

      if (MouseK) {usb_hid.mouseMove(RID_MOUSE, 0, MouseDelta);  delay(dt50); break; } 

      if (Media && ToneControls) {usb_hid.sendReport16(HIDCons, BassDwn); delay(dt50);
                                  usb_hid.sendReport16(HIDCons, 0); break; }
             
      if (PowerKeys) { DoPowerKeys ('i', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { status("Repeat Macro not yet implemented"); MacroTimerDisp = false; ConfigButtons(1); break; }
      
      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                 }  break;        }      

      // Power Restart Logoff Timer options
      if (ConfigKeyCount==1) {ConfigKeyCount--; PowerKeys = !PowerKeys;  
          if (PowerKeys) status("[Rst]art [Log]off [Stop] Power[Off] Clock-Timer"); else status(" ");
          ConfigButtons(1); break;} 
     
      keycode[0] = ArrDown;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 10: // PAGE DOWN //////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('u', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer6 = !MacroTimer6; MacroTimer18 = MacroTimer18 || MacroTimer6;
                        if (MacroTimer6) { alarmEnable = true;  status("[O-C] Oneshot Macro Clock started");  }                        
                                    else { alarmEnable = false; status("[O-C] Oneshot Macro Clock stopped");  }
                        ConfigButtons(1); optionsindicators(Button); break; }
                        
      if (Math) {MakeMathStr(Button); break; }
      
      if (Kbrd) {MakeStr(Button); break;}

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_FORWARD);  delay(dt50);
                   usb_hid.mouseButtonRelease(RID_MOUSE);                      delay(dt50); break; }

      if (Media && ToneControls) {usb_hid.sendReport16(HIDCons, TrebDwn); delay(dt50);
                                  usb_hid.sendReport16(HIDCons, 0); break; }
      
      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                 }  break;        }                   

      if (ConfigKeyCount==1) {ConfigKeyCount--;               
                              Media = !Media; 
                              if (Media) { if (MediaCfg) MediaConfig2Media(0); status("Media Keys ON");  }
                                  else   { if (MediaCfg) MediaConfig2Media(1); status("Media Keys OFF"); }
                              ConfigButtons(1); break; }
                  if (Media) {usb_hid.sendReport16(HIDCons, PlayMed); delay(dt50);
                  usb_hid.sendReport16(HIDCons, 0); break; }

      keycode[0] = PageDwn;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 11: // V- or Enter or Add /////////////////////////////////////////////////////////////////////////////////////////////////////
      if (Math) { SendMath(); status("Math symbol sent"); MathByteNum=0; break; }  
    
      if (Kbrd) {if (KeyBrdX==3) {MakeStr(Button); break;}  // [ADD] now [Sav] Save
                if (!KBrdActive) {status("Press char/mod first"); break;} // No char key or modifier keys has been pressed at least once
                if (KeyBrdX==4) {KBDispPos = KBDispPos + 2;  if (KeyBrdByte[0]>0x7F) KBType = true; } // can be 2nd or 3rd?
                if (Fx) {KBDispPos = KBDispPos + 2; }
                KBDispPos++; KeyBrdByteNum++; break; }  

      if (MouseK) {usb_hid.mouseScroll(RID_MOUSE, -1*MouseScrollAmount, 0); break; } 
                 
      if (MacroTimerDisp) { status("Exit Timer Config"); MacroTimerDisp = false; ConfigButtons(1); break; }
      
      if (NumKeys) {if (nKeys) DoNKeys(Button);
                         else {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(dt50);
                               usb_hid.keyboardRelease(HIDKbrd);                                  }  
                    break; }

      if (!VolOn) { DoBsdCodes(RetNum);  break; } // Return Delete BackSpace Tab AltTab Insert Esc PScr Num Caps Scroll etc

      usb_hid.sendReport16(HIDCons, VolDown); delay(dt50);
      usb_hid.sendReport16(HIDCons, 0);       break;
  }  } // Layout=2 Button switch
}      // buttonpress

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigMacroButtons() {  // Only updates six macro buttons - reduce screen flicker - LCD Type C this is not an issue
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int n, m, b;
  uint8_t rows = 3;    // 3 rows of labels
  uint8_t row, col;    // keys

  b = Layout - 1;                              //  L=1/b=0  L=2/b=1  L=3/b=2 L=4/b=3    
 
  uint16_t OutlineColor = ButtonOutlineColor[b];
  
  for (n=4; n<11; n++)  { keyColor[n] = Colours[b][n]; strcpy(keyLabel[n], Labels[LayerAD][b][n]); } 

  if (MLabel+SLabel+TLabel>0) DoMSTLabel(1, Layout);
              
  for (row = 1; row < rows; row++)   {     // 2 rows 1,2 
       for (col = 0; col < 3; col++) {     // 3 columns 0,1,2
        b = col + row * 4;
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X), KEY_Y + row * (KEY_H + KEY_SPACING_Y), 
                                               KEY_W, KEY_H, OutlineColor, keyColor[b], KeyFont, keyLabel[b], KEY_TEXTSIZE);
        key[b].drawButton();  }  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw the keys ( 3 times 4 loops to create 12) Button Label 10 chars max
// The button number is the current column added to the current row times  the number of buttons per row. 
// First the row is multiplied by 4 and then the col is added.
// The first button is 0. Example: col = 2 (3rd column), row = 1 (2nd row) =>: 1 * 4 = 4 --> 4 + 2 = 6 (7th button).
// TFT_eSPI/Extensions/Button.h           x-middle,  y-middle,   width,      height,     outline,          fill,   text
// void initButton(TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
//                                uint16_t textcolor, char *label, uint8_t textsize);
// When button is pressed: draw with fill/text swapped to indicate 'pressed' 
// void drawButton(bool inverted = false, String long_name = "");
// Compute the squared distance (d2) from the button center to the current touch coordinates.
// d2 = (bx - tx)*(bx - tx) + (by - ty)*(by - ty);
// If d2 is less than the button radius squared, then that button was selected.
// You will find this function, which does not do any multiplies, in the Adafruit graphics library:
// boolean Adafruit_GFX_Button::contains(int16_t x, int16_t y) 
//          {return ((x >= _x1) && (x < (_x1 + _w)) &&  (y >= _y1) && (y < (_y1 + _h))); }}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigButtons(uint8_t rowcount) {  // rowcount=0 all 4 rows rowcount=2 last row only  rowcount=1 no background redraw
                                        // rowcount=5 only redraw Pad keys 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int n, m, b, p, k;
  uint8_t rows = 3;    // 3 rows of labels
  uint8_t row, col;    // keys
  uint16_t PadB, PadC; // 5 Pads Red Blue Green SkyBlue Yellow
  int mR[4] = { 0, 4, 6, 8 };
  int mT[8] = { 0, 1, 2, 4, 6, 8, 9, 10 };
  int cR[9] = { 0, 1, 2, 4, 5, 6, 8, 9, 10 }; 
  /////////////////////////////////////////////////////////////////////////////////////////////
  // Four Layouts possible:  Layout=1 b=0   Layout=2 b=1   Layout=3 b=2   Layout=4 b=3
  /////////////////////////////////////////////////////////////////////////////////////////////
  b = Layout - 1;                              //  L=1/b=0  L=2/b=1  L=3/b=2 L=4/b=3
  uint16_t OutlineColor = ButtonOutlineColor[b]; 
  
  if (rowcount==0) tft.fillScreen(BackgroundColor[b]);      // All four layouts Black background
  if (rowcount==1) rowcount = 0;                            // No background refresh
  if (rowcount==5) goto DoPadKeys;                          // Update 5 Pads only
  
  if (!MuteOn) b = b + 4;               // b = 4,5,6,7
  if (!VolOn)  b = b + 8;               // b = 8,9,10,11 
  // (!VolOn) && (!MuteOn) b = b + 12   // b = 12,13,14,15
  
  for (n=0; n<12; n++) strcpy(keyLabel[n], Labels[LayerAD][b][n]); // label = 3 chars max except nKeys 5 chars   
  for (n=0; n<12; n++) keyColor[n] = Colours[b][n];

  if (Layout!=2 && MLabel+SLabel+TLabel>0) DoMSTLabel(1, Layout);
  
  if (!VolOn) for (m=0; m<3; m++) { keyLabel[3][m]  = BsDLabel[BsDNum][m]; 
                                    keyLabel[11][m] = BsDLabel[RetNum][m];  }                      // Delete -> ScrollL  

  if (KeyHeldEnable) keyLabel[7][0] = 'V';     // V1-V4 istead of L1-L4 if longpress for Volume Mute is active toggle with *ke*
     
  b = Layout - 1;                               // reset b to 0 - 3
  k = (b>1)*3;
  if (XFiles && b!=1) { for (n=0; n<3; n++) for (m=0; m<3; m++) keyLabel[n][m] = BsDLabel[XNum[n+k]][m]; }  // Delete -> CtrlV
  
  if (Brightness && b!=1 && VolOn) { keyLabel[3][0] = keyLabel[11][0] = 'B'; }  // *br* = brightness up/dwn in M S T repace volume  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  If Layout 2 [Cfg] Key pressed new options in different colour:
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // also see for loop comma operator use here: https://forum.arduino.cc/t/arduino-does-not-understand-a-ranged-for-loop/164312/10
  if (ConfigKeyCount>0) {for (m=0; m<9; m++) { n = cR[m]; strcpy(keyLabel[n], CfgLabel[n]); keyColor[n]=CfgColour;} } 
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                 "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn",  "PgD", "V-/Ret"
  // MediaLabel[x][] "Stp", "   ", "   ", "   ",    "Prv",  "   ",  "Nxt", "   ",     "P-P", "   ",  "  ",  "   " 
  // MediaLabel[x][] "|||", "   ", "   ", "   ",    "|<",   "   ",  ">|",  "   ",     ">||", "   ",  "  ",  "   " 
  // keyLabel[x][]     0                              4               6                8                                           
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // MediaLabel2[x][] "|||", "B+",  "T+",  "   ",    "|<",   "   ",  ">|",  "   ",  ">||", " B-",   "T-",  "   "};
  // keyLabel2[x][]     0     1      2                 4               6                8    9       10
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if ((Media)&&(b==1)&&(ConfigKeyCount==0)) {
      if (!ToneControls) {for (m=0; m<4; m++) {n = mR[m]; strcpy(keyLabel[n], MediaLabel[n]); keyColor[n] = MediaColour; } }                         
      if (ToneControls)  {for (m=0; m<8; m++) {n = mT[m]; strcpy(keyLabel[n], MediaLabel2[n]); keyColor[n] = MediaColour;} } }                                            
                 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // PowerKeys
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((PowerKeys)&&(b==1)&&(ConfigKeyCount==0)) {for (m=0; m<12; m++) {strcpy(keyLabel[m], PowerKeysLabel[m]); keyColor[m] = PowerKeysColor[m]; } }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Macro Timers
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((MacroTimerDisp)&&(b==1)&&(ConfigKeyCount==0)) {for (m=0; m<12; m++) {strcpy(keyLabel[m],  MacroTimerLabel[m]); keyColor[m] = MacroTimerColor[m]; } }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Math) { for (m=0; m<12; m++) {strcpy(keyLabel[m], MathLabel[MathX][m]);}
              for (m=0; m<11; m++) {keyColor[m] = MathColour1;} keyColor[3]=keyColor[7]=keyColor[11]=MathColour2; goto KeyBrdsDone; } // Do it once

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Kbrd) {for (m=0; m<12; m++) {strcpy(keyLabel[m], KbrdLabel[KeyBrdX][m]);}
             for (m=0; m<12; m++) {keyColor[m] = CfgColour;}; goto KeyBrdsDone; }                                                     // or twice

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (MouseK) {for (m=0; m<12; m++) {strcpy(keyLabel[m], MouseKeysLabel[m]); keyColor[m] = MouseKeyColour[m]; }; goto KeyBrdsDone; }  // even thrice                                              // and thrice
                 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (NumKeys) { if (nKeys)  { for (m=0; m<12; m++) {strcpy(keyLabel[m], NKeysX[m]);}
                               for (m=0; m<12; m++) {keyColor[m] = nKeysColour;} }
                 if (!nKeys) { if (!Resistor) { for (m=0; m<12; m++) {strcpy(keyLabel[m], NumkeysX[m]);}
                                                for (m=0; m<12; m++) {keyColor[m] = Black;} }
                               if (Resistor)  { for (m=0; m<12; m++) {strcpy(keyLabel[m], NumkeysX[m]);}
                                                if (Numkeys123==0) for (m=0; m<12; m++) {keyColor[m] = RColour[RXlat[m]];} 
                                                              else for (m=0; m<12; m++) {keyColor[m] = Black;} }  }  }
  /////////////
  KeyBrdsDone:
  /////////////
  b = Layout - 1;                               // reset b to 0 - 3
              
  for (row = 0 + rowcount; row < rows; row++) {   // 3 rows 0,1,2 => b = 0+(0*4)=0 1+(0*4)=1 ... 0+(1*4)=4 ....
    for (col = 0; col < 4; col++) {               // 4 columns 0,1,2,3
        b = col + row * 4;
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X), KEY_Y + row * (KEY_H + KEY_SPACING_Y), 
                                               KEY_W, KEY_H, OutlineColor, keyColor[b], KeyFont, keyLabel[b], KEY_TEXTSIZE);
        key[b].drawButton();  }  }

   DoPadKeys:
   /////////////////////////////////////////////////////      0 1 2 3 4   5 6 7 8 9    5 6 7   8    9
   // 5 Small Config Buttons between 1 st and 3rd row  // Pad             s k m n 0    + k e/s char -  
   ////////////////////////////////////////////////////       
   b = Layout - 1;                     // reset b to 0 - 3 
 
   padLabel[8][0] = nChar;
   if (NumKeys) { padLabel[5][0] = '+'; padLabel[9][0] = '-'; padColor[5] = Red; 
                  if (nKeysShow) { padColor[8] = DGreen; padLabel[7][0] = 's'; padColor[7] = Red;  } 
                            else { padColor[8] = dCyan;  padLabel[7][0] = 'e'; padColor[7] = Blue; } }
           else { padLabel[5][0] = 's'; padLabel[7][0] = 'm'; padLabel[9][0] = 'o'; padColor[8] = dCyan; padColor[5] = padColor[7] = Blue;  }
         
   for (row = 0; row < 5; row++) 
       {p = row+b*5; key[row+12].initButton(&tft, PAD_X, PAD_Y + row * PAD_SPACING_Y, 
                                                  PAD_W, PAD_H, White, padColor[p], KeyFont, padLabel[p], KEY_TEXTSIZE);
        key[row+12].drawButton();  }             
             
  if (Change) indicators();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Output report callback for LED indicator such as Caplocks
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  BusyCNS = true;
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
  // digitalWrite(LED_BUILTIN, ledIndicator & KEYBOARD_LED_CAPSLOCK);
  
  CapsLock   = (ledIndicator & KEYBOARD_LED_CAPSLOCK); 
  NumLock    = (ledIndicator & KEYBOARD_LED_NUMLOCK);
  ScrollLock = (ledIndicator & KEYBOARD_LED_SCROLLLOCK);
 
  Change = ((CapsLock!=CapsLockPrev)||(NumLock!=NumLockPrev)||(ScrollLock!=ScrollLockPrev));

  BusyCNS = false;

  /*
  if (Change) indicators();
   
  if ((Change)&&(!BackLightOn))  {if (NormVal==0) digitalWrite(LCDBackLight, HIGH);     // Backlight Full ON
                                             else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON
                                  BackLightOn = true; }
  */                                
}

////////////////////////////////////////////////
// Number Pad Page or Macrokeys n01-n996 Select
////////////////////////////////////////////////
void NumKeysChange()
{ int a, b, c, d, n, m, cm1; // Can use itoa() or Timer2Arr() but less flexible than used here with nKeys34 option
  c = (Numkeys123)*12;       // c = 0, 12, 24, 36, 48, 60, 72, 84, 96, 108 Max pages = 1000/12 = 83 -> n01/n001 - n996
  
  for (int m=0; m<12; m++) { if (nKeys) { cm1=c+m+1; d = 48 + cm1/100; a = 48 + (cm1/10)-10*(d-48); b = 48 + cm1%10;
                                          // Serial.print(d);  Serial.print(' '); Serial.print(a);  Serial.print(' '); Serial.println(b);
                                          NKeysX[m][0] = nChar; 
                                          if (d==48&&nKeys34) { NKeysX[m][1] = a; NKeysX[m][2] = b; NKeysX[m][3] = 0x00; }
                                                        else  { NKeysX[m][1] = d; NKeysX[m][2] = a; NKeysX[m][3] = b; NKeysX[m][4] = 0x00; } 
                                        } else { strcpy(NumkeysX[m], NumkeysLabel[Numkeys123][m]);  }  
                           }  
  if (nDirX==2) nDir[1]=nChar; ConfigButtons(5); // Only update 5 Pad keys and only update 2nd char in nDir if /n/ mode
  // for (m = 0; m<12; m++) for (n = 0; n < 6; n++) {Serial.print(NKeysX[m][n], HEX); Serial.print(' '); } SerPr2;                                      
}

///////////////////
void ReadBank123()
///////////////////
{File f1 = LittleFS.open("Bank123File", "r");
 f1.read(Bank123, 3); 
 f1.close();  
}
///////////////////
void Savex1x6()
///////////////////
{ File f1 = LittleFS.open("x1x6", "w"); 
  f1.write(XNum, 6); 
  f1.close();  
}
///////////////////
void Readx1x6()
///////////////////
{File f1 = LittleFS.open("x1x6", "r"); 
 f1.read(XNum, 6); 
 f1.close();   
}
///////////////////
void SaveMouse()
///////////////////
{ MouseConfig[0] = MouseScrollAmount;
  MouseConfig[1] = MouseDelta; 
  File f1 = LittleFS.open("MouseCfg", "w"); 
  f1.write(MouseConfig, 2); 
  f1.close();  
}
///////////////////
void ReadMouse()
///////////////////
{File f1 = LittleFS.open("MouseCfg", "r"); 
 f1.read(MouseConfig, 2); 
 f1.close(); 
 MouseScrollAmount = MouseConfig[0]; 
 MouseDelta = MouseConfig[1];   
}

////////////////////////////////////
void MediaConfig2Media(bool Cancel)
////////////////////////////////////
{ if (Cancel) { VolOn = false;     
                MuteOn = false;    
                ToneControls = false;
                return; }

  if (MediaConfig[0]>1) VolOn = true; 
  if (MediaConfig[0]>2) MuteOn = true;
  if (MediaConfig[0]>3) ToneControls = true;
}

///////////////////////
void SaveMediaConfig()
///////////////////////
{ MediaConfig2Media(0);
  File f1 = LittleFS.open("MediaCfg", "w"); 
  f1.write(MediaConfig, 1); 
  f1.close();  
}
////////////////////////
void ReadMediaConfig()
////////////////////////
{
  File f1 = LittleFS.open("MediaCfg", "r"); 
  f1.read(MediaConfig, 1); 
  f1.close();  
  MediaCfg = MediaConfig[0];                  //  false if 0, true if > 0
  //if (MediaConfig[0]>1) MediaConfig2Media(0); // Else on boot always on 
   
}
///////////////////////
void SaveLayoutConfig()
///////////////////////
{ Config1[34] = SaveLayout; 
  WriteConfig1Change = true; 
}
////////////////////////
void ReadLayoutConfig()
////////////////////////
{ SaveLayout = Config1[34];
  if (SaveLayout==0) { Layout = 2; SaveLayoutConfig(); }   //  Layout = 1 to 4 or 2 if SaveLayout = 0    
}

///////////////////////
void SaveSDCardArr()
///////////////////////
{ File f1;
  if (SDNum<=19) SDCardArr[0] = SDNum; else return; // *sd*k,K not stored
  f1 = LittleFS.open("SDCardArr", "w"); 
  f1.write(SDCardArr, 4); 
  f1.close();  
}
////////////////////////
void ReadSDCardArr()
////////////////////////
{ File f1;
  f1 = LittleFS.open("SDCardArr", "r"); 
  f1.read(SDCardArr, 4); 
  f1.close();   
  if (SDCardArr[0]<=19) SDNum = SDCardArr[0];  
}
///////////////////////////////////
void SaveBSDKeyKArr(bool ClearBSD)
///////////////////////////////////
{ byte n, k = 0;
  File f1;
  
  if (ClearBSD) { for (n=27; n<51; n++) BsDCode3[n] = BsDCode2[n] = BsDCode1[n] = 0;  // Use 51 not BSDMax
                  LittleFS.remove("BSDKeyK"); return; }
  
  for (n=27; n<51; n++) { BSDKeyKArr[k]=BsDCode1[n]; BSDKeyKArr[k+1]=BsDCode2[n]; BSDKeyKArr[k+2]=BsDCode3[n]; k=k+3;}  
      
  f1 = LittleFS.open("BSDKeyK", "w"); 
  f1.write(BSDKeyKArr, 72);            // 24 Kxx Keys x 3 = 72     
  f1.close(); 
  // if (ClearBSD) ReadBSDKeyKArr();   // Reset current BSD also = 0x00 
}
////////////////////////
void ReadBSDKeyKArr()
////////////////////////
{ int n, k = 0;
  File f1;
  if (LittleFS.exists("BSDKeyK"))
     {f1 = LittleFS.open("BSDKeyK", "r"); 
      f1.read(BSDKeyKArr, 72); 
      f1.close(); 
      for (n=27; n<51; n++) { BsDCode1[n] = BSDKeyKArr[k]; BsDCode2[n] = BSDKeyKArr[k+1]; BsDCode3[n] = BSDKeyKArr[k+2]; k=k+3;}
     }       
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ReadConfig1()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ int n;
  File f1 = LittleFS.open("Config1", "r");
  int FSz = f1.size(); // Check for valid Config1
  if (FSz!=Config1Size) { for (int n=0; n<Config1Size; n++) Config1[n] = Config1Reset[n]; WriteConfig1(0); return; }
  f1.read(Config1, Config1Size); 
  f1.close();
  
  KeySkip = Config1[0];
  CheckSerial = Config1[1];
  KeyHeldEnable = Config1[2];
  BLOnOff = Config1[3];   
  Rotate180 = Config1[4];                                    // Must check if rotate=0 but calibrate-data is for rotate=180  
  KeyFontBold = Config1[5];                                  // Use: if (calData[4]==5 && Rotate180==1) Rotate180=0;  
  ResetOnceEnable = Config1[6];                              //  or: if (calData[4]==3 && Rotate180==0) Rotate180=1;     
  nKeys = Config1[7];                                        // 
  nChar = Config1[8];     if (nChar<0x30) nChar = 'n';       // Old Config1 with nChar=0x00 then nKeys label = blank 
  nKeysPage = Config1[9]; if (nKeysPage<1) nKeysPage = 8;    // Old Config1 with nKeysPage=0 not valid 
  for (n=1; n<11; n++) nKeysCharSet[n-1] = Config1[9+n];     // if (nKeysCharSet[0]<0x30) reset all 10 to default
  CRLF =  Config1[20];                                       // CR LF Filter
  crlf1 = Config1[21]; 
  crlf2 = Config1[22];
  if (crlf1==0 && crlf2==0) { crlf1 = 0x0D; crlf2 = 0x0A; }  // Reset to default if both NULL character filters
  iList =    Config1[23]; 
  MuteOn =   Config1[24]; 
  VolOn =    Config1[25];
  LayerAxD = Config1[26]; 
  Media =    Config1[27]; 
  XFiles =   Config1[28];
  Brightness = Config1[29];
  BsDNum =   Config1[30];  // 0 = DeleteBackspace  
  RetNum =   Config1[31];  // 8 = ReturnKey
  LayerAD =  Config1[32];  
  KeyFontColour = Config1[33]; if (KeyFontColour) KeyFont = Black; else KeyFont = White;  // Button Font Bold/Normal labels
  SaveLayout =    Config1[34]; 
  OptionOS =      Config1[35]; 
  KeyRepeat =     Config1[36]; if (KeyRepeat<2) KeyRepeat=6; RepTimePeriod = KeyRepeat*100;  // Unusable Macropad if too short
  NormVal =       Config1[37]; 
  DimVal =        Config1[38];
  nKeys34 =       Config1[39];
  strcpy(nDir, (char *)&Config1[40]); // 40-59 60=nDirZ=0 
  for (n=1; n<11; n++) nKeysLnkChar[n-1] = Config1[60+n];    // 61-70 Config1[61-70] 
  nDirX =         Config1[71];
  MLabel = Config1[72];  SLabel = Config1[73]; TLabel = Config1[74];  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WriteConfig1(bool Option)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ int n;
  if (Option==1) {Config1[0] = KeySkip;
                  Config1[1] = CheckSerial;
                  Config1[2] = KeyHeldEnable;
                  Config1[3] = BLOnOff;
                  Config1[4] = Rotate180;
                  Config1[5] = KeyFontBold;
                  Config1[6] = ResetOnceEnable;
                  Config1[7] = nKeys;
                  Config1[8] = nChar;
                  Config1[9] = nKeysPage; 
                  for (n=1; n<11; n++) Config1[9+n] = nKeysCharSet[n-1];
                  Config1[20] = CRLF; 
                  Config1[21] = crlf1; 
                  Config1[22] = crlf2;  
                  Config1[23] = iList;   // iList 0,1
                  Config1[24] = MuteOn; 
                  Config1[25] = VolOn;
                  Config1[26] = LayerAxD;                  
                  Config1[27] = Media; 
                  Config1[28] = XFiles;
                  Config1[29] = Brightness;
                  Config1[30] = BsDNum;  // 0 = DeleteKey  
                  Config1[31] = RetNum;  // 8 = ReturnKey
                  Config1[32] = LayerAD; // LayerADLetter (A-D) = LayerAD (0-3) + 65 -> 0-3 => A-D
                  Config1[33] = KeyFontColour; 
                  Config1[34] = SaveLayout; 
                  Config1[35] = OptionOS;   
                  Config1[36] = KeyRepeat;
                  Config1[37] = NormVal; 
                  Config1[38] = DimVal; 
                  Config1[39] = nKeys34;                  
                  strcpy((char *)&Config1[40], nDir); // 40-59 60=nDirZ=0                    
                  for (n=1; n<11; n++) Config1[60+n] = nKeysLnkChar[n-1];   // Config1[61-70] 
                  nDirX       = Config1[71];
                  Config1[72] = MLabel; Config1[73] = SLabel; Config1[74] = TLabel;    
                }
  
  File f1 = LittleFS.open("Config1", "w"); 
  if (Option==2) f1.write(Config1Reset, Config1Size);   // Reset all to default - could be Rotate180 problem
            else f1.write(Config1,      Config1Size);   // Write current Config1 values if Option = 0 or 1
  f1.close();  
}

///////////////////////////////////////////////////////////////
void InitCfg(bool Option)    // Only 1 on cold start or reboot
///////////////////////////////////////////////////////////////
{ char *ChrPtr;
  char NameStr[] = { "0123456789" };
  byte b, *BPtr;
  int ASize =0, m = 0, n = 0;
  uint16_t ByteLen = 0;
  
  // WriteConfig1(0); delay(10); // Use only once then comment out when there are changes to Config1 structure or size
  
  if (Option) {
      
      if (LittleFS.exists("Config1"))           ReadConfig1();          else WriteConfig1(2);           // Read Config1 else write default values
     
      if (LittleFS.exists("SDCardArr"))         ReadSDCardArr();
      if (LittleFS.exists("MouseCfg"))          ReadMouse();
      if (LittleFS.exists("MediaCfg"))          ReadMediaConfig();
      if (LittleFS.exists("iList"))             ReadInstructionList();                                  // iList -> MacroInstructionList
      if (LittleFS.exists("DoCal"))             DoCal    = true;        else DoCal           = false;   // 1 -> Run calibration on start
      if (LittleFS.exists("MacroUL"))           MacroUL  = true; SwitchMacroUL(0);                      // Upper/Lower case filenames
      if (LittleFS.exists("x1x6"))              Readx1x6();                                             // always read if exists
      if (LittleFS.exists("Bank123File"))       ReadBank123();                                          // always read if exists
  }    
  
  for (n=0; n<24; n++) {DoMSTName(n, 3); BPtr = Str1to12[n]; MacroS1S12[n] = 4;                   // DoFileBytes read eof LayerAxD 0 FlashMem 1 SDCard
                        MacroSizeS1S12[n] = DoFileBytes(0, MSTName, BPtr, ByteSize, LayerAxD); }  // 0 = read 1 = write
           
  for (n=0; n<24; n++) {DoMSTName(n, 4);  BPtr = Ttr1to12[n]; MacroT1T12[n] = 4;                  // DoFileBytes read eof LayerAxD 0 FlashMem 1 SDCard   
                        MacroSizeT1T12[n] = DoFileBytes(0, MSTName, BPtr, ByteSize, LayerAxD); }
           
  for (n=0; n<24; n++) {DoMSTName(n, 1);  BPtr = Mtr1to12[n]; MacroM1M12[n] = 4;                  // DoFileBytes read eof LayerAxD 0 FlashMem 1 SDCard         
                        MacroSizeM1M12[n] = DoFileBytes(0, MSTName, BPtr, ByteSize, LayerAxD); } 
  
  if (LittleFS.exists("inputStr"))   { ChrPtr = inputString; DoFileStrings(StrOK, "inputStr", ChrPtr, 0); }
  if (LittleFS.exists("TimersData")) ReadTimers(1); else ReadTimers(2); // If not exist create TimersData with default data

  if (MLabel) DoMSTLabel(0, 1); if (SLabel) DoMSTLabel(0, 3); if (TLabel) DoMSTLabel(0, 4);

  ReadBSDKeyKArr();    // New defines for K1-K24 in BSD1-3
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t DoFileBytes(byte DoWrite, const char *STRf,  byte *BytePtr, uint16_t ByteArrayLen, bool SDCard)
/////////////////////////////////////////////////////////////////////////////////////////////////////////
{ uint16_t ByteLen = 0;
  int n;
  byte b;
  File f;  

  LargeFile = false; 
    
  if (!DoWrite)                                 // DoWrite = 0 read data     
     {if (SDCard)  { if (SDFS.exists(STRf))     f = SDFS.open(STRf, "r");      else return 0; }
      if (!SDCard) { if (LittleFS.exists(STRf)) f = LittleFS.open(STRf, "r");  else return 0; }
      if (f.size() > ByteSize) 
         { for (n=0; n<10; n++) { b = f.read(); BytePtr[n++] = b; }
           ByteLen = f.size(); f.close(); LargeFile = true; return 0; }  // return 0 not ByteLen test LargeFile true
       
      n = 0;
      while (f.available())
            {b = f.read();
             BytePtr[n++] = b; }
      //if (n >= ByteArrayLen) break; }
      ByteLen = f.size();
      f.close(); }
     
  if (DoWrite>0)                                 // DoWrite = 1 write data DoWrite = 2 do not add 0x00 at end 
     {if (SDCard) f = SDFS.open(STRf, "w");
             else f = LittleFS.open(STRf, "w");              
      b = BytePtr[ByteArrayLen-1];  
      f.write(&BytePtr[0], ByteArrayLen);        // write the whole array to the file
      if (b!=0x00 && DoWrite!=2) f.print('\0');  // need this extra 0x00 only once CopyMacro uses DoWrite = 2 here
      ByteLen = f.size();
      f.close();  }

  return ByteLen;
}

////////////////////////////////////////////////////////////////////////////
// case 1:  ChrPtr = Ttr1; NameStr = TTR1f; 
// if (Found) DoFileStrings(StrOK, NameStr,   ChrPtr); 
//            DoFileStrings(StrOK, TTR1f, ChrPtr); 
// Can use: void DoFileStrings(bool DoWrite, char *STRf,  char *ChrPtr)
//          void DoFileStrings(bool DoWrite, char STRf[],  char *ChrPtr)
// Must then redo all such as DoFileStrings(0, "DeleteBackspace", ChrPtr);
// Because ISO C++ forbids converting a string constant to 'char*' 
////////////////////////////////////////////////////////////////////////////
void DoFileStrings(bool DoWrite, const char *STRf,  char *ChrPtr, bool SDCard)
{ uint16_t StrLen;
  File f;

  LargeFile = false; 
    
  if (!DoWrite)                                 // read data     
     {if (SDCard)  { if (SDFS.exists(STRf))     f = SDFS.open(STRf, "r");      else return; }
      if (!SDCard) { if (LittleFS.exists(STRf)) f = LittleFS.open(STRf, "r");  else return; }
      if (f.size() > StrSize)  { f.close(); LargeFile = true; return; }
      
      //StrLen = f.available(); 
      //StrLen = strlen(Str1); 
      StrLen = f.size();
      f.readBytes((char *)ChrPtr, StrLen);
      f.close(); }
       
  if (DoWrite)                                // write data
     {if (SDCard) f = SDFS.open(STRf, "w");
             else f = LittleFS.open(STRf, "w"); 
      StrLen = strlen(ChrPtr);               // Sent string <5This is S5> then STRf=StrData5 StrLen=10 which is correct
      f.print(ChrPtr);                       // if byte[] not char[]?
      f.print('\0');
      f.close();  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// LittleFS supports directories. Path + filename have a maximum of 255 characters 
// Dir dir = LittleFS.openDir("/");
// while (dir.next()) {
//        Serial.print(dir.fileName());
//        File f = dir.openFile("r");
//        Serial.println(f.size()); }
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Should replace with:
// Dir dir = LittleFS.openDir("/data"); while (dir.next()) 
//                                            { Serial.print(dir.fileName());  
//                                              if (dir.fileSize()) { File f = dir.openFile("r"); 
//                                                  Serial.println(f.size()); } }
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ListFiles()
{ int n = 1;
  File root = LittleFS.open("/", "r");
  File file = root.openNextFile();  
  while(file){
      Serial.print(n);
      SerPr1;
      Serial.print(file.name());
      SerPr1;
      Serial.println(file.size());
      n++;
      file = root.openNextFile();
  }
}
void ListSDFiles(bool ClearFiles)
{ int n = 1;
  char NameStr[30] = { "" };
  File root = SDFS.open("/", "r");
  File file = root.openNextFile();
  while(file){
      Serial.print(n);
      SerPr1;
      Serial.print(file.name());
      SerPr1;
      Serial.println(file.size());
      if (ClearFiles) SDFS.remove(file.name());
      n++;
      file = root.openNextFile();
  }
}
//////////////////////////////////////////////////////////////////////////
void DeleteFiles(byte Option)  // delete text/number macros and config
//////////////////////////////////////////////////////////////////////////
{ File f1, f2;

  if (Option==1)   // Save calibration file "TouchCalData" from flash to SDCard
     {if (LittleFS.exists(CalFile)) {File f1 = LittleFS.open(CalFile, "r"); 
      if (f1) {if (f1.readBytes((char *)calData, 14) == 14) calDataOK = 1;  f1.close(); }  }  
      File f2 = SDFS.open(CalFile, "w"); if (f2) { f2.write((const unsigned char *)calData, 14); f2.close(); }
     }
     
  if (LittleFS.format()); 
     {LittleFS.begin();

 if (Option==1)   // Restore calibration file "TouchCalData" from SDCard to Flash SDCard
     {// if (SDFS.exists(CalFile)) {File f2 = SDFS.open(CalFile, "r"); // calData is still valid but can do this anyway
      // if (f2) {if (f2.readBytes((char *)calData, 14) == 14) calDataOK = 1;  f2.close(); }  }  
      File f1 = LittleFS.open(CalFile, "w"); if (f1) { f1.write((const unsigned char *)calData, 14); f1.close(); }
     }
      
 InitCfg(0);   // This will create TimersData file again with default values
 Serial.println("Files + Config deleted");}
}

////////////////////////////////////////////////
void DisplayClocks(bool DisplayOption)
////////////////////////////////////////////////
{ SerPr2;
  Serial.print("Date and Time <t>: ");
  Serial.print(tTimeDateArr);
  if (DisplayOption) { status(tTimeDateArr); delay(3000); }
  SerPr2;

  Serial.print("Alarm Macro [R-C][O-C] <a>: ");
  Serial.print(aTimeDateArr);
  if (DisplayOption) { status(aTimeDateArr); delay(3000); }
  SerPr2;

  Serial.print("Alarm Macro [RcT][OcT] <w>: ");
  Serial.print(wTimeDateArr);
  if (DisplayOption) { status(wTimeDateArr); delay(3000); }
  SerPr2;

  Serial.print("Alarm Power [R-C][O-C] <p>: ");
  Serial.print(pTimeDateArr);
  if (DisplayOption) { status(pTimeDateArr); delay(3000); }
  SerPr2; 

  Serial.print("Time in seconds since 1970: "); Serial.print(now());                   // Time in seconds since 1970
  SerPr2; 
  
  Serial.print("Alarm Time in seconds since 1970: "); Serial.print(makeTime(alarm));   // Alarm Time in seconds since 1970
  SerPr2;
}

/////////////////////////////////////////////////////////////////////
void GetSysInfo(int Action) 
/////////////////////////////////////////////////////////////////////
//GetSysInfo(0) only access if [Cfg] pressed twice i.e. [Sav] shows
///////////////////////////////////////////////////////////////////// 
{ char *ChrPtr;
  byte *BPtr;
  uint32_t  Bt;
  int StrLen, n = 0, m = 0, b = 0;
  int fHeap = rp2040.getFreeHeap();
  int uHeap = rp2040.getUsedHeap();
  int tHeap = rp2040.getTotalHeap();
  int fCPU  = rp2040.f_cpu();
  
  if (Action>0) 
     { if (SaveLayout>0) { SaveLayoutConfig(); }        
       if (Action==4) { Savex1x6(); SaveX1X6 = false; Config1[28] = XFiles; WriteConfig1(1); }
  return; } // if (Action>0)

  if (WriteConfig1Change) { WriteConfig1(1); WriteConfig1Change = false; }
  if (SDNumChange) { SaveSDCardArr(); SDNumChange = false; }  
  if (Bank123Change) { File f = LittleFS.open("Bank123File", "w"); if (f) {f.write(Bank123, 3); f.close(); } Bank123Change = false; }
  if (LayerADChange) { if ( LayerAxD!=Config1[26] || LayerAD!=Config1[32] ) WriteConfig1(1); LayerADChange = false; }
  if (SaveVar) { WriteConfig1(1); SaveVar = false; }  
  if (MediaChange) { if (VolOn!=Config1[25] || MuteOn!=Config1[24] || Media!=Config1[27]) { WriteConfig1(1); MediaChange = false; } }      
  if (SaveOptionOS) { WriteConfig1(1); SaveOptionOS = false; }  
  
  Serial.println("Version: VolumeMacro2303 Tobias van Dyk March 2025 License GPL3");
  Serial.println("Hardware: Waveshare Pico 2 RP2350 ILI9488 Resistive TouchLCD 3.5inch"); 
  Serial.printf("CPU MHz (Pico 1 or RP20240): %d\n\r", fCPU);
  Serial.printf("FreeHeap: %d\n\r", fHeap);
  Serial.printf("UsedHeap: %d\n\r", uHeap);
  Serial.printf("TotalHeap: %d\n\r", tHeap);
  Serial.printf("Core temperature: %2.1fC\n\r", analogReadTemp());  
  Serial.println("\n");
  
  Serial.println("Flash Files (Number Name Size):");
  ListFiles();
  Serial.print("\n");
  Serial.println("SDCard Files (Number Name Size):");
  ListSDFiles(0);

  Serial.print("SDCard Set: "); Serial.print(SDNum); SerPr1;   
  Serial.print("SDCard Element 0: "); n=0; while (SDName[0][n]!=0x00 && n<20) { Serial.print(SDName[0][n]); n++; }
  Serial.println("\n");
  
  DisplayClocks(false);
  
  SerPr2;
  ReadTimers(1);
  Serial.printf("LCD Blank Timeout: %d seconds\n\r", TimePeriod/1000);     // make sure this is never 0 

  b = (DimVal*100)/255;
  Serial.printf("LCD Dimmed: %d percent (%d/255)\n", b, DimVal);  

  b = (NormVal*100)/255; if (b==0) b = 100;
  Serial.printf("LCD Brightness: %d percent (%d/255)\n", b, NormVal);
      
  Serial.printf("Restart Time  (seconds): %d %d\n", timeRestart/1000,  TimeRestart/1000);
  Serial.printf("PowerOff Time (seconds): %d %d\n", timePowerOff/1000, TimePowerOff/1000);

  Serial.print("CR LF Filter Mode Char 1 Char 2: "); 
  Serial.print(CRLF); SerPr1; Serial.print(crlf1, HEX); SerPr1; Serial.println(crlf2, HEX);
  
  Serial.print("nKeys Page Maximum: ");  Serial.print(nKeysPage);  Serial.print(" nKeys Numbers: "); Serial.print(12*nKeysPage); SerPr2; 
  Serial.print("nKeys Directory Path: "); Serial.print(nDir); Serial.print(" Size: "); Serial.print(strlen(nDir)); 
  Serial.print(" nKeys Dir Mode: "); Serial.print(nDirX); SerPr2;
  Serial.print("nKeys Quick-Access:  "); for (int i = 0; i < 10; i++) Serial.print(nKeysCharSet[i]); SerPr2;
  Serial.print("nKeys Link-String:   "); for (int i = 0; i < 10; i++) Serial.print(nKeysLnkChar[i]); SerPr2;

  Serial.print("Calibration Data: "); for (int i = 0; i < 5; i++) { Serial.print(calData[i]); if (i < 4) Serial.print(", "); }
   
  //showKeyData();   
}
////////////////////
void ResetTimers(byte Option)
////////////////////
{ if (Option==1) { DimData[1] = timePowerOff = 30000;       // Reset to default values 30s 50m                    
                   DimData[2] = TimePowerOff = 3000000;
                   DimData[3] = timeRestart  = 30000;                          
                   DimData[4] = TimeRestart  = 3000000;
                   return; }
                   
  if (Option==2) { DimData[5] = timeOnceof   = 9000000;                          
                   DimData[6] = TimeOnceof   = 90000000;
                   DimData[7] = timeRepeat   = 9000000;                          
                   DimData[8] = TimeRepeat   = 90000000; 
                   return; }
                   
  DimData[0] = TimePeriod = 30000;      // *tb* = 30 seconds   
}
/////////////////////////////////////
void ReadTimers(byte Option) 
/////////////////////////////////////
{ unsigned long int current = 0;   // https://stackoverflow.com/questions/62816850/arduino-read-integer-from-txt
 
 if (DimData[1]==0&&DimData[3]==0&&DimData[2]==0&&DimData[4]==0) { ResetTimers(1); Option==2; } // Use default values if all 4 are 0
 
 if (Option==2) { File s = LittleFS.open("TimersData", "w");   // Will create file if not exist
                  s.write((const unsigned char *)DimData, 36); // Store 9 long unsigned int
                  s.close(); return; }                         // All done file created

  File f = LittleFS.open("TimersData", "r");  
  if (f.readBytes((char *)DimData, 36) == 36) current = DimData[0]; f.close(); // Serial.println(DimData[0]);
  if (current>10000) TimePeriod = current; else TimePeriod = 30000;            // 10 seconds minimum Blank time - never 0!
  
  if (Option==1) { timePowerOff = DimData[1];  // 30 seconds all                   
                   TimePowerOff = DimData[2];  // 3000    
                   timeRestart  = DimData[3];  // 30                         
                   TimeRestart  = DimData[4];  // 3000   
                   timeOnceof = DimData[5];    // 9000                          
                   TimeOnceof = DimData[6];    // 90000
                   timeRepeat = DimData[7];    // 9000                          
                   TimeRepeat = DimData[8]; }  // 90000  
}

////////////////////////////////////////////////////////////////////////////////////
bool WriteTimers(unsigned long PVal, byte Option, byte b) 
////////////////////////////////////////////////////////////////////////////////////
{ char PArray[4][10] = { "tPowerOff", "TPowerOff", "tRestart", "TRestart" };
  //                      01234567890123456789    01234567890123456789
  char RPArr[4][21]  = { "PowerOff t         s", "PowerOff T         s",
                         "Restart t          s", "Restart T          s" };
  int n; 
                          
  DimData[Option] = PVal; // DimData[0] BlankTime DimData[1-4]=PowerTimers DimData[5-8]=MacroTimers
              
  File f = LittleFS.open("TimersData", "w");
  if (f) f.write((const unsigned char *)DimData, 36); else return 0; f.close();  // Store 9 long unsigned int
  
  if (Option>0) { Timer2Str(TimerArr, 0, PVal); // Serial.println(TimerArr);
                  strcpy(TimerVal[Option-1], TimerArr);
                  if (timeU=='m'||timeU=='h') Timer2Str(TimerArr, 1, PVal); // Serial.println(TimerArr);
                  
                  n=0; while ( TimerArr[n]!=0x00) { RPArr[Option-1][n+12] = TimerArr[n]; n++; }
                  RPArr[Option-1][19] = timeU;  // s,m,h
  
                  status(RPArr[Option-1]); SendBytesEnd(1); 
                }
  return 1;   
}

/////////////////////////////////////////////////
// Display mini status bar
// STATUS_X 150 // Centred on this 320/2=160
// STATUS_Y 222 // bottom
/////////////////////////////////////////////////
void status(const char *msg) 
/////////////////////////////////////////////////
{  // tft.setCursor(STATUS_X, STATUS_Y);
  if (LayerAxD) tft.setTextColor(Orange, Black); else tft.setTextColor(SkyBlue1, Black); 
  if (Kbrd) tft.setTextColor(Yellow, Black);
  if (Math) tft.setTextColor(White, Black);
  if (PowerKeys) tft.setTextColor(Orange, Black);
  if (sSens||mPlay||tTimeDate) tft.setTextColor(Green, Black);
  
  tft.setTextPadding(480);  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(1);
  tft.drawString(statusBlank, STATUS_X, STATUS_Y); 
  tft.drawString(msg, STATUS_X, STATUS_Y);
  
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);  else tft.setFreeFont(&FreeSans12pt7b);                            
}
////////////////////////////////////
void FillKeysStr(int SelectLayout)
////////////////////////////////////
{ int n, m;
  for (n=0; n<24; n++) {                  // SelectLayout=0 do all three M,S,T
  if (SelectLayout==3||SelectLayout==0)   // Keys S1-S24
  { MacroS1S12[n] = 2; for (m=0; m<=strlen(str1to12[n]); m++) Str1to12[n][m] = str1to12[n][m]; MacroSizeS1S12[n] = m; }   
  if (SelectLayout==4||SelectLayout==0)   // Keys T1-T24
  { MacroT1T12[n] = 2; for (m=0; m<=strlen(ttr1to12[n]); m++) Ttr1to12[n][m] = ttr1to12[n][m]; MacroSizeT1T12[n] = m; }
  if (SelectLayout==1||SelectLayout==0)   // Keys M1-M24
  { MacroM1M12[n] = 2; for (m=0; m<=strlen(str1to12[n]); m++) Mtr1to12[n][m] = str1to12[n][m]; MacroSizeM1M12[n] = m; } }
}
/////////////////////////////////////////
unsigned long ReadMacroTimers(int Option)
/////////////////////////////////////////
{ long unsigned int current = 0; 
  char TArray[4][10] = {"tRepeat", "TRepeat", "tOnce", "TOnce"};
  File f = LittleFS.open(TArray[Option], "r"); 
  while (f.available()) { long unsigned int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  return current; 
}
//////////////////////////////////////////////////////////////
void WriteMacroTimers(unsigned long TVal, int Option, byte b) 
//////////////////////////////////////////////////////////////
{ int n;
  byte a;
  char TArray[4][10] = {"tRepeat", "TRepeat", "tOnce", "TOnce"};
  //                      01234567890123456789    0123456789012345689
  char TPArr[4][21]  = { "Repeat  t           ", "Repeat  T          ",
                         "Onceof  t           ", "Onceof  T          " };                          
     
  File f = LittleFS.open(TArray[Option], "w"); 
  if (f) {f.print(TVal); f.close(); }
  a = 1;
  if (Option==1||Option==3) { n = 12; while (a!=0x00) { a = mTArrayStr[b][n-12]; TPArr[Option][n] = a;  n++; } }  
  if (Option==2||Option==4) { n = 12; while (a!=0x00) { a = mtArrayStr[b][n-12]; TPArr[Option][n] = a;  n++; } }
  //status((char *)mTArrayStr[b]); 
  status(TPArr[Option]); SendBytesEnd(1); 
}
///////////////////////////////////////////////////////////////////////////////////////////
byte FindSDNum(byte C)
///////////////////////////////////////////////////////////////////////////////////////////
{byte SD3[10] = { 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };
 byte SD4[23] = { 0x30,0x31,0x32,0x33,0x55,0x56,0x57,0x58,0x59,0x60,0x4D,0x53,0x54,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x4B,0x6B };
 byte SD5[23] = { 0x30,0x31,0x32,0x33,0x75,0x76,0x77,0x78,0x79,0x80,0x6D,0x73,0x74,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x4B,0x6B }; 
 byte n, x = 0;
 
 for (n=0; n<10; n++) if (C==SD3[n]) { SDCardArr[2] = 0; return n; }
 for (n=0; n<23; n++) if (C==SD4[n]) { SDCardArr[2] = 0; return n; }
 for (n=0; n<23; n++) if (C==SD5[n]) { SDCardArr[2] = 1; return n; }
  
 return x;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Select SDCard Filename
// 1-3 = sdcard.h   4-9 = U V W X Y Z   10 = M m   11 = S s   12 = T t  13-19 A B C D E F G  20 = KxxLink  21 = k,K
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SDCardSelectFiles(byte Option)  // Option = 0=[Cfg][Opt], setup(), 1 = *sd*n  
///////////////////////////////////////////////////////////////////////////////////
{ int a, b, c, n, m = 0;  // m = 0 = uppercase
  char SDCardSetX[19] =   {"SDCard Files Set X"};   
  
  if (Option) c = FindSDNum(SDNum);              // Return with SDNum and 0,1 in [2] Option = 1 = *sd*n
  m = SDCardArr[2];                              // Also now 0,1 Upper/Lowerscase filename 
  
  if (SDNum==0) { SDCardArr[0] = SDCardArr[1] = 0x00; return; }  // SDCard File use is disabled = 0x00  
   
  if (Option) { SDNum = c; if (SDCardArr[2]) SDCardSetX[17] = SD2[c]; else SDCardSetX[17] = SD1[c]; 
                status((char *)SDCardSetX); SDCardArr[0] = SDNum; SDCardArr[1] = SD1[c];}           // Now SDNum = 1-21  
  
  for (n=0; n<24; n++)   
      {if (SDNum==1) strcpy(SDName[n], SDName1[n]);     // *sd*1
       if (SDNum==2) strcpy(SDName[n], SDName2[n]);     // *sd*2    
       if (SDNum==3) strcpy(SDName[n], SDName3[n]);     // *sd*3
       
       a = (n+1)/10; b = (n+1) % 10; 
       if (SDNum>3) { if (m) SDName[n][0] = SD2[SDNum]; else SDName[n][0] = SD1[SDNum]; 
                      SDName[n][1] = a + 48; SDName[n][2] = b + 48; SDName[n][3] = 0x00;
       
       if (SDNum==20) strcat(SDName[n], "Link");  } if (sdShow) Serial.println(SDName[n]); }    // *sd*K K01Link - K24Link         
              
    if (Option) SDCardArr[1] = SDCardSetX[17];     // SDCardArr[1] = char " ", 1-3, SDNum 4-9 = U,V,W,X,Y,Z   M,S,T  A,B,C,D,E,F,G   k K 
           
    // Serial.println(SDNum); Serial.println(SDName[0]); Serial.println(SDName[23]); 
    // Serial.println(SDCardArr[0]); Serial.println(SDCardArr[1]);
}

///////////////////////////
void SaveInstructionList()
///////////////////////////
{ byte m, n, iListArr[iListMax*3];
  for (m=0; m<3; m++) for (n=0; n<iListMax; n++) iListArr[n+iListMax*m] = MacroInstructionList[m][n]; 
  File f1 = LittleFS.open("iList", "w"); 
  f1.write(iListArr, iListMax*3); 
  f1.close();  
}
///////////////////////////
void ReadInstructionList()
///////////////////////////
{ byte m, n, iListArr[iListMax*3];  
  File f1 = LittleFS.open("iList", "r"); 
  f1.read(iListArr, iListMax*3); 
  f1.close(); 
  for (m=0; m<3; m++) for (n=0; n<iListMax; n++) MacroInstructionList[m][n] = iListArr[n+iListMax*m];   
}

////////////////////////////////
bool DoBank123(byte b, byte k2)
////////////////////////////////
{ if (b>5) { status("Add number 1-5"); return false; }      // Use the [ADD]ed number to assign 1-3 (or 0)
  if (k2==0x6d) Bank123[0] = b;                             // *am*n = Keys M1-M24 macro action select 1-3 
  if (k2==0x73) Bank123[1] = b;                             // *as*n = Keys S1-S24 macro action select 1-3
  if (k2==0x74) Bank123[2] = b;                             // *at*n = Keys T1-T24 macro action select 1-3
  File f = LittleFS.open("Bank123File", "w"); if (f) {f.write(Bank123, 3); f.close(); } // Must still read back on boot 
  status("Action Bank 1-5 changed");
  return true;
}
/////////////////////////////////////////////
void SwitchMacroUL(bool Option)
/////////////////////////////////////////////
// Only change Pad keys MST O or mst o here
{char Lcase[] = "amstdskmnoamstdamstd" ;
 char Ucase[] = "aMSTdskmnOaMSTdaMSTd" ;
 char MacroULStatus[30] = "Macro Filenames Case: ";
 byte n;
   
 for (n=0; n<20; n++) { if (MacroUL) padLabel[n][0]=Ucase[n]; else padLabel[n][0]=Lcase[n]; }

 if (Option) { if (MacroUL)  { File f = LittleFS.open("MacroUL", "w"); if (f) {f.print(MacroUL); f.close(); 
                               strcat(MacroULStatus, "Upper"); status(MacroULStatus); } } 
               if (!MacroUL) { LittleFS.remove("MacroUL"); 
                    strcat(MacroULStatus, "Lower"); status(MacroULStatus); } 
             }
}
//////////////////////////
// Find entered StarCode
//////////////////////////
byte FindStarNum()
{ byte StarPos = 100;
  byte n ;
  char s1 = KeyBrdByte[1];
  char s2 = KeyBrdByte[2];
  for (n=0; n<StarCodesMax; n++) {if ((s1==StarCode[n][1])&&(s2==StarCode[n][2])) { StarPos = n; break; } }
  return StarPos;
}

//////////////////////////////////////
unsigned long GetT(byte knum)
//////////////////////////////////////
{unsigned long T = 0; 
 byte b = KeyBrdByte[4]-48, c = KeyBrdByte[5], d = KeyBrdByte[6], e = KeyBrdByte[7]; timeU='s';
 if (b>=0&&b<=9) ; else return T;
 if (knum==8) { if (e=='s') T = (b*100 + (c-48)*10 + d-48)*1000; }                                             // *xx*nnn s 000-999 sec
 if (knum==7) { if (d=='h') { T=(b*10 + c-48)*3600000; timeU=d; }                                              // *xx*nn h/m/s 00-99 hrs/min/sec
                if (d=='m') { T=(b*10 + c-48)*60000;   timeU=d; }   
                if (d=='s') { T=(b*10 + c-48)*1000;    timeU=d; }                    
                if (timeU=='s' && d!='s') T = (b*100 + (c-48)*10 + d-48)*1000; }                               // *xx*nnn 000-999 sec                                   
 if (knum==6) { if (c=='h') { T=b*3600000; timeU=c; }                                                          // *xx*n h/m/s 0-9 hrs/min/sec
                if (c=='m') { T=b*60000;   timeU=c; }   
                if (c=='s') { T=b*1000;    timeU=c; }                    
                if (timeU=='s' && c!='s') T = (b*10 + c-48)*1000; }                                            // *xx*nn 01-99 sec                                   
 if (knum==5) { T=b*60000; timeU='m'; }                                                                        // *xx*n=1-9 minutes
 // Serial.println(knum); Serial.println(T); Serial.println(timeU); 
 return T;  // Value in milliseconds  
}

///////////////////////////////////////////////////////////////////////////////////////
bool SendBytesStarCodes()
/////////////////////////////////////////////////////////////////////////////////////// 

{ unsigned long T, z;  // t = timeclock struct
  uint8_t a, b, c, d, h, n, m, i, k2, k5, k6, knum;
  bool UnLink = false, StarOk = false, Ok = true;  
  File f, f1;
  
  int c99 = 0;         // b*10 + KeyBrdByte[5]-48; 
                                                                 
  a = FindStarNum(); c = StarCodeType[a];     // Position of *code in StarCode list                              
  CmKey = false;                              // Check if *codes are from pressing [*Cm] key or entered directly
  if (a==100) return StarOk;                  // Not found 
                               
  b = KeyBrdByte[4]-48;                 //  01234 = *  *b
  k2 = KeyBrdByte[2];                   //  01234 = * k*
  k5 = KeyBrdByte[5];                   //  *xx* k5
  k6 = KeyBrdByte[6];                   //  *xx*  k6
  knum = KeyBrdByteNum;                 //  *xx* = 4 *xx*nnn = 7
  c99 = b*10 + k5-48;                   //  00-99
  // c999 = b*100 + (k5-48)*10 + k6-48; //  000=999
  
  switch(c)
      { case 1: ///////////////////// KeyBrdByte[1]==0x61 *am*n *as*n *at*n
      { if (b>5) { status("Add number 1-5"); break; }             // Use the [ADD]ed number to assign 1-3 (or 0)
        if (k2==0x6d) Bank123[0] = b;                             // *am*n = Keys M1-M24 macro action select 1-3 
        if (k2==0x73) Bank123[1] = b;                             // *as*n = Keys S1-S24 macro action select 1-3
        if (k2==0x74) Bank123[2] = b;                             // *at*n = Keys T1-T24 macro action select 1-3
        Bank123Change = true; status("Action Bank 1-5 changed"); StarOk = true; break; }
        case 2: ///////////////////// KeyBrdByte[1]==0x62&&KeyBrdByte[2]==0x62 *bb*n nn backlight pwm *bb*(0-9)x10 or *bb*00-100
      { if (knum==4) { Config1[37] = NormVal = 0; NormArr[16] = ' '; NormArr[17] = 'F'; }                       // no number 100% PWM
        if (knum==5) { Config1[37] = NormVal = b*10*255/100;  NormArr[16] = KeyBrdByte[4]; NormArr[17] = '0'; } // n x 10 5 -> 50% PWM
        if (knum==6) { Config1[37] = NormVal = (c99)*255/100; NormArr[16] = KeyBrdByte[4]; NormArr[17] = k5; }
        if (NormVal==0) { digitalWrite(LCDBackLight, HIGH); }                      // 255   Backlight full ON
                     else analogWrite(LCDBackLight, NormVal);                      // 1-254 Backlight Brightness part ON
        if (knum>6) { status("Use *bb*nn percent 01-99 00=max=*bb*"); delay(2000); break; } 
               else { status(NormArr); SendBytesEnd(1); WriteConfig1Change = StarOk = true; break; } }        
        case 3: ///////////////////// KeyBrdByte[1]==0x64&&KeyBrdByte[2]==0x62 *db*nn percent 00-99 00=*db*=Off
      { if (knum==4) { Config1[38] = DimVal = 0; DimArr[16] = '0'; DimArr[17] = '0'; }                       // no number OFF
        if (knum==5) { Config1[38] = DimVal = b*10*255/100;  DimArr[16] = KeyBrdByte[4]; DimArr[17] = '0'; } // *db*5 -> 50% PWM
        if (knum==6) { Config1[38] = DimVal = (c99)*255/100; DimArr[16] = KeyBrdByte[4]; DimArr[17] = k5; }
        if (knum>6) { status("Use *db*nn percent 00-99 00=*db*=Off"); delay(2000); break; } 
               else { status(DimArr); SendBytesEnd(1); WriteConfig1Change = StarOk = true; break; } }
        case 4: ///////////////////// KeyBrdByte[1]==0x74&&KeyBrdByte[2]==0x62 blank lcd timemout *tb*nnn + s
      { DimData[0] = TimePeriod = GetT(knum);  
        if (TimePeriod<10000) DimData[0] = TimePeriod = 30000;  // *tb* or Time minimum 10 sec else use default 30 seconds
        Timer2Str(TimerArr, 0, TimePeriod); for (n=0; n<strlen(TimerArr); n++) { BlankArr[n+16] = ' '; BlankArr[n+15] = TimerArr[n]; }      
        if (WriteTimers(TimePeriod, 0, 0)) { status(BlankArr); StarOk = true; } SendBytesEnd(1); break; }
        case 5: ///////////////////// KeyBrdByte[1]==0x62&&KeyBrdByte[2]==0x72 *br* = brightness on/off volume off/on
      { Brightness = !Brightness; Config1[29] = Brightness; WriteConfig1Change = true;         // or use WriteConfig1(0); 
        if (Brightness) status("Brightness ON"); else status("Brightness OFF"); ConfigButtons(1); SendBytesEnd(1); StarOk = true; break; }
        case 6: ///////////////////// KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x61 *ca* = Calibration On/Off 
      { DoCal = !DoCal; if (DoCal) { status("Calibrate ON - for OFF repeat *ca*"); 
                                     File f = LittleFS.open("DoCal", "w"); if (f) {f.print(DoCal); f.close(); } } 
                              else { status("Calibrate OFF - send *ca* ON"); LittleFS.remove("DoCal");}   
                StarOk = true; break; }
        case 7: ///////////////////// KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x6D *cm* or *cmnnXnn = copy macros MSTA -> MSTA + K
      { CopyMacro(0); SendBytesEnd(0); StarOk = true; break; }
        case 8: ///////////////////// KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x74 "*ct*" = display clocks 4x 1 second delay
      { DisplayClocks(true); StarOk = true; break; }
        case 9: ///////////////////// KeyBrdByte[1]==0x64&&KeyBrdByte[2]==0x65 "*de*" = delete config and macro files
      { status("Files deleted"); DeleteFiles(1); ConfigButtons(1); SendBytesEnd(1); StarOk = true; break; }
        case 10: //////////////////// KeyBrdByte[1]==0x65  *e0* to *e4*  Media keys Activated   
      { Media = MediaCfg = true;   // Assume media modes now on except for *e0*       
        if (k2==0x30)              // *e0* restore to normal Mediakeys behaviour
           {Media = MediaCfg = MediaConfig[0] = 0; LittleFS.remove("MediaCfg");  // all false
           status("Media Mode 0");      } 
        if (k2==0x31)              // *e1* = 4 Media keys
           {MediaConfig[0] = 1; status("Media Mode 1 C");     }
        if (k2==0x32)              // *e2* = 4 Media keys + Volume
           {MediaConfig[0] = 2; status("Media Mode 2 CV");    }
        if (k2==0x33)              // *e3* = 4 Media keys + Volume + Mute
           {MediaConfig[0] = 3; status("Media Mode 3 CVM");   }
        if (k2==0x34)              // *e4* = 4 Media keys + Volume + Mute + Tone
           {MediaConfig[0] = 4; status("Media Mode 4 CVMT");  }          
        if (MediaCfg) SaveMediaConfig(); ConfigButtons(1); StarOk = true; break; } 
        case 11: //////////////////// KeyBrdByte[1]==0x66 *f  a,m,s,t Fill keys
      { if (k2==0x73)              // *fs* = Fill S keys with predefined coded strings
           {FillKeysStr(3); status("[S1-S24] filled"); StarOk = true; break; }
        if (k2==0x74)              // *ft* = Fill T keys with predefined coded strings
           {FillKeysStr(4); status("[T1-T24] filled");  StarOk = true; break;}
        if (k2==0x6d)              // *fm* = Fill M keys with predefined coded strings
           {FillKeysStr(1); status("[M1-M24] filled"); StarOk = true; break; }
        if (k2==0x61)              // *fa* = Fill M S T keys with predefined coded strings
           {FillKeysStr(0); status("[All 1-24] filled");  StarOk = true; break; }  }
        case 12: //////////////////// KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x63)  *fc* = Key Label Font Colour Black White
      { KeyFontColour = !KeyFontColour; Config1[33] = KeyFontColour; WriteConfig1Change = true; // or use WriteConfig1(0); 
        if (KeyFontColour) { status("Key Font Colour is Black"); KeyFont = Black; } else { status("Key Font Colour is White"); KeyFont = White; } 
        ConfigButtons(1); SendBytesEnd(1); StarOk = true; break; }
        case 13: //////////////////// KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x6F *fo* = Key Label Bold Normal Font
      { KeyFontBold = !KeyFontBold; Config1[5] = KeyFontBold; WriteConfig1(0);
        if (KeyFontBold) { status("Key Font Bold is ON");  tft.setFreeFont(&FreeSansBold12pt7b); }
                    else { status("Key Font Bold is OFF"); tft.setFreeFont(&FreeSans12pt7b);     } 
        ConfigButtons(1); SendBytesEnd(1); StarOk = true; break; }
        case 14: //////////////////// KeyBrdByte[1]==0x6B&&KeyBrdByte[2]==0x62 *kb* = num keyboard turn on
      { NumKeys = !NumKeys; ConfigButtons(1); SendBytesEnd(1); status("NumPad ON/OFF"); StarOk = true; break; }
        case 15: //////////////////// KeyBrdByte[1]==0x6b&&KeyBrdByte[2]==0x73 *ks* Skip first key pressed in LCD in dimmed state  
      { KeySkip = !KeySkip; Config1[0] = KeySkip; WriteConfig1(0);
        if (!KeySkip) status("Wakeup Key disabled"); else status("Wakeup Key enabled"); StarOk = true; break; }
        case 16: //////////////////// KeyBrdByte[1]==0x6c&&KeyBrdByte[2]==0x64 *ld* List Data Links and Macros via serial port
      { showKeyData(); status("Links Data Sent"); StarOk = true; break; }
        case 17: //////////////////// KeyBrdByte[1]==0x64&&KeyBrdByte[2]==0x66 "*df*" = delete all SDCard files
      { status("SDCard Files deleted"); ListSDFiles(1); StarOk = true; break; }
        case 18: //////////////////// KeyBrdByte[1]==0x6d&&KeyBrdByte[2]==0x31 *m1*nn = Scroll move amount 
      { if (knum>5) b = c99;    // b = 0, 1-20
        if (b<11) {MouseScrollAmount = b; status("Scroll changed"); StarOk = true; break; } else break; }
        case 19: //////////////////// KeyBrdByte[1]==0x6d&&KeyBrdByte[2]==0x32 *m2*nn = Cursor move amount 
      { if (knum>5) b = c99;    // b = 0, 1-99
        if (b<100) {MouseDelta = b; status("Cursor Move changed"); SaveMouse(); StarOk = true; break; } else break; }
        case 20: //////////////////// KeyBrdByte[1]==0x6d *mt or *mT = macro onceof timers
      { T = GetT(knum);                                              // "tOnce", "TOnce"
        if (k2==0x74) { timeOnceof = T; WriteMacroTimers(T, 5, b); } // *mt*num timeOnceof
        if (k2==0x54) { TimeOnceof = T; WriteMacroTimers(T, 6, b); } // *mT*num TimeOnceof
        StarOk = true; break; }
        case 21: //////////////////// KeyBrdByte[1]==0x6e) *nt or *nT = macro repeat timers
      { T = GetT(knum);                                              // "tRepeat", "TRepeat"
        if (k2==0x74) { timeRepeat = T; WriteMacroTimers(T, 7, b); } // *nt*num timeRepeat
        if (k2==0x54) { TimeRepeat = T; WriteMacroTimers(T, 8, b); } // *nT*num TimeRepeat
        StarOk = true; break;  }        
        case 22: //////////////////// KeyBrdByte[1]==0x6F&&KeyBrdByte[2]==0x73 *os* operating system toggle Windows/Linux/PiOS on/off
      { OptionOS++; if (OptionOS>2) OptionOS=0; status(OSName[OptionOS]); SaveOptionOS = true; StarOk = true; break; }
        case 23: ////////////////////// Power Keys: KeyBrdByte[1]==0x6F  *o For example: 30 sec = *ot*030s  10 min = *oT*010m
      { T = GetT(knum);                                                // "tPowerOff", "TPowerOff" 
        if (k2==0x74) { if (knum==4) { timePowerOff =   36000; status("PowerKeys *ot* Defaults"); }
                                else { timePowerOff = T; WriteTimers(T, 1, 0); } }  // *ot*num timePowerOff *ot*=default
        if (k2==0x54) { if (knum==4) { TimePowerOff = 3600000; status("PowerKeys *oT* Defaults"); }
                                else { TimePowerOff = T; WriteTimers(T, 2, 0); } }  // *oT*num TimePowerOff *oT*=default
        StarOk = true; break;  }        
        case 24: ////////////////////// Power Keys: KeyBrdByte[1]==0x72 *r For example: 30 sec = *rt*030s  10 min = *rT*010m
      { T = GetT(knum);                                                // "tRestart", "TRestart"
        if (k2==0x74) { if (knum==4) { timeRestart =   36000; status("PowerKeys *rt* Defaults"); }
                                else { timeRestart = T; WriteTimers(T, 3, 0); } }  // *rt*num timeRestart *rt*=default
        if (k2==0x54) { if (knum==4) { timeRestart = 3600000; status("PowerKeys *rT* Defaults"); }
                                else { TimeRestart = T; WriteTimers(T, 4, 0); } }  // *rT*num TimeRestart *rT*=default
        StarOk = true; break;    }           
        case 25: ////////////////////// KeyBrdByte[1]==0x70&&KeyBrdByte[2]==0x6F) *po* = Power Keys Menu Option 
      { PowerKeysMenu = !PowerKeysMenu; if (PowerKeysMenu) status("PowerKeys Menu ON"); 
                                                      else status("PowerKeys Menu OFF"); StarOk = true; break; }
         case 26: ////////////////////// KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x31 *r1* ResetOnceEnable true/false;
       { ResetOnceEnable = !ResetOnceEnable; Config1[6] = ResetOnceEnable; WriteConfig1(0);       // Toggle
         if (ResetOnceEnable) status("Reset Once Enabled"); else status("Reset Once Disabled"); StarOk = true; break; }
         case 27: ////////////////////// KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x6F *ro* = rotate 180 degrees turn on
       { Rotate180 = !Rotate180; Config1[4] = Rotate180; WriteConfig1(0);                                          // Toggle On/Off
         status("Restart 180 degrees rotate calibrate"); LittleFS.remove("TouchCalData"); StarOk = true; break; }  // this forces re-calibration 
         case 28: ////////////////////// KeyBrdByte[1]==0x73         // *sa,m,s,t save to flash
       { if (k2==0x73)                // *ss* = Save all S keys to Flash
            {MST1 = 1; for (n=0; n<24; n++) { Option1 = n; SaveMacro(0); } status("[S1-S24] saved"); StarOk = true; break;}                
         if (k2==0x74)                // *st* = Save all T keys to Flash 
            {MST1 = 2; for (n=0; n<24; n++) { Option1 = n; SaveMacro(0); } status("[T1-T24] saved"); StarOk = true; break;}
         if (k2==0x6d)                // *sm* = Save all M keys to Flash 
            {MST1 = 1; for (n=0; n<24; n++) { Option1 = n; SaveMacro(0); } status("[M1-M24] saved"); StarOk = true; break;}
         if (k2==0x61)                // *sa* = Save all M S T keys to Flash
            {for (m=0; m<3; m++) {MST1 = m; for (n=0; n<24; n++) { Option1 = n; SaveMacro(0); } } status("[All MST1-24] saved"); StarOk = true; break;} } 
         case 29: ////////////////////// KeyBrdByte[1]==0x73&&KeyBrdByte[2]==0x64 *sd*n = 1-9 or mM sS tT or aA-gG or K,k Select SDCard Files 
       { if (knum>5) break;                  // SDNum = single digits or char only after *sd* or *sd*= sdShow = true 
         if (knum==4) { sdShow = !sdShow; 
                                 if (sdShow) status("SDFileNames Listed ON"); else status("SDFileNames Listed OFF"); StarOk = true; break; }        
         SDNum = KeyBrdByte[4];                       // Use the [ADD]ed number to assign 1 - 9 or calc 10 - 21 next       
         SDCardSelectFiles(1); SDNumChange = true; StarOk = true; break; }
         case 30: ////////////////////// KeyBrdByte[1]==0x73&&KeyBrdByte[2]==0x65 *se* Switch off serial monitor - default is off - state is saved  
       { CheckSerial = !CheckSerial; Config1[1] = CheckSerial; WriteConfig1(0); 
         if (CheckSerial) status("Serial COM port monitor enabled"); else status("Serial COM port monitor disabled"); StarOk = true; break; }
         //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //               0 12 34 56 7 89 01 -> copy to RecBytes [1] to [11]
         // Use serial *tx* yy mm dd w hh mm> 22110341200 12:00am 3 Nov 2022 Thursday where x = t,a,p,w
         // t = Main Time/Clock a  = Macro Clock Repeat-Oneshot [R-C][O-C] p = Macro Clock Countdown [RcT][OcT] w = Power Clock [O-C][R-C]
         // datetime_t t = { .year  = 2022, .month = 11, .day   = 03, .dotw  = 4, .hour  = 14,  .min   = 00, .sec   = 00 };
         //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         case 31: ////////////////////// KeyBrdByte[1]==0x74)  *t Time 
      {  for (n=1;n<12;n++) RecBytes[n] = KeyBrdByte[n+3];
         if (k2==0x74) {GetTimeData(&t, tTimeDateArr); TimeSet = true; StarOk = true; break;} // *tt* Main TimeClock Set          
         if (k2==0x61) {GetTimeData(&alarm, aTimeDateArr); StarOk = true; break;}             // *ta* [R-C][O-C]
         if (k2==0x70) {GetTimeData(&timer, wTimeDateArr); StarOk = true; break;}             // *tp* [RcT][OcT]
         if (k2==0x77) {GetTimeData(&power, pTimeDateArr); StarOk = true; break;}    }        // *tw* [O-C][R-C]
         case 32: ////////////////////// KeyBrdByte[1]==0x75&&KeyBrdByte[2]==0x6c *ul* = unlink current Source Key Num
       { UnLink = UnlinkKeyMST(0); if (UnLink) status("Macro MSTLink removed"); else status("No MSTLink found"); StarOk = true; break; }
         case 33: ////////////////////// KeyBrdByte[1]==0x75&&KeyBrdByte[2]==0x61 *ua* = unlink all macros
       { UnLink = UnlinkKeyMST(1); if (UnLink) status("Macro MSTAKLink removed"); else status("No MSTAKLink found"); StarOk = true; break; }
         case 34: ////////////////////// KeyBrdByte[1]==0x30&&KeyBrdByte[2]==0x52 *0R* = Zero R Resistor Colours NumPad Toggle On/Off 
       { Resistor = !Resistor; if (Resistor) status("Resistors active"); else status("Resistors passive"); StarOk = true; break;  }
         case 35: ////////////////////// KeyBrdByte[1]==0x78   *xn* n=1-6 x1-x3 keys M x1 - x3 S T x4 - x6 keys n=7,8 [Del[Ret] keys
       { if (knum>5) b = c99;           // *xn*bc b values entered are b = 0, 1-54, n values are 0, 1-9
         n = k2-48; if (n==0) b = 0;    // n = 0, 1 - 6 or 7,8 for [Del][Ret] or 9 for list 8 keys
         if (knum<4&&n>0) { status("Add 1-54 or 0 clear"); delay(1500); break; }    // *x0*0 = clear all but *xn*m n>0 requires m
         if (b>BSDMax)             { Serial.println(b); status("Invalid Number");      delay(1500); break; }    // Out of range          
         if (b==0) {if (n==0||n==9) { for (m=14; m<17; m++) { XNum[m-14]=m; XNum[m-11]=m; } // *x9*0 Clear 1-6 2 x (Cut Copy Paste) + Del + Ret
                                      BsDNum = 0; RetNum = 8; XFiles = false; }  
                    if (n==8)       { BsDNum = 0; }                               // *x8*0 Reset Del to default
                    if (n==7)       { RetNum = 8; }                               // *x9*0 Reset Del to default 
                    if (n>3&&n<7)   { for (m=14; m<17; m++) XNum[m-11]=m; }       // *x4,5,6*0 Reset 4-6 (cX cC cV)
                    if (n<4)        { for (m=14; m<17; m++) XNum[m-14]=m; }       // *x1,2,3*0 Reset 1-3 (Cut Copy Paste)
                    XFiles = TestXFiles(); GetSysInfo(4); StarOk = true; break; }  // if (b==0)   
                           
         if (n==7) { BsDNum = b-1; }                                               // [Del] key options 1-54
         if (n==8) { RetNum = b-1; }                                               // [Ret] key options 1-54
         if (n==9) { h = 0; for (m=0; m<6; m++) { XNum[m] = ((KeyBrdByte[4+h]-48)*10 + KeyBrdByte[5+h]-48)-1; h = h+2; } // *x9*list all 8 values 1-6 7,8
                     BsDNum = ((KeyBrdByte[16]-48)*10 + KeyBrdByte[17]-48)-1; RetNum = ((KeyBrdByte[18]-48)*10 + KeyBrdByte[19]-48)-1; }                                      
         if (b>0) { XNum[n-1] = b-1; }                                             // *x1,2,3,4,5,6*nn nn  1-54       
         XFiles = TestXFiles(); GetSysInfo(4); StarOk = true; break; }  
         case 36: ////////////////////// KeyBrdByte[1]==0x62&&KeyBrdByte[2]==0x6c *bl* Black Switch BL On/Off
       { BLOnOff = !BLOnOff; Config1[3] = BLOnOff; WriteConfig1(0);                                         // Toggle On/Off 
         if (BLOnOff) status("Black Switch BL On/Off Enabled"); else status("Black Switch BL On/Off Disabled"); StarOk = true; break; }  
         case 37: ///////////////////// KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x30 *r0* Reset/Reboot LCD
       { status("Rebooting LCD in 2.222 seconds..."); delay(2222); rp2040.reboot(); break; }
         case 38: ////////////////////// KeyBrdByte[1]==0x6b&&KeyBrdByte[2]==0x72 *kr* Duration/100 before KeyRepeat activein milliseconds
       { //               01234567890123456789012345678
         char KRVal[] = {"Key Repeat x00 msec"} ;
         if (b<2) { status("Add number 2-9"); break; }             // Use the [ADD]ed number to assign 2-9
         RepTimePeriod = b*100; KeyRepeat = b; KRVal[11] = b+48;   // Value 2-9 => 200-900 milliseconds 
         WriteConfig1Change = true;  status((char *)KRVal); StarOk = true; break; }   
         case 39: ////////////////////// KeyBrdByte[1]==0x6b&&KeyBrdByte[2]==0x65 *ke* Enable/Disable Volume Mute Processing 
       { KeyHeldEnable = !KeyHeldEnable; Config1[2] = KeyHeldEnable; WriteConfig1(0);
         if (KeyHeldEnable) status("KeyHeldEnable On"); else status("KeyHeldEnable Off"); StarOk = true; break; }  // Toggle On/Off
         case 40: ///////////////////// KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x32 *r2* Reboot to UF2 Loader
       { status("Rebooting LOAD NEW FIRMWARE UF2"); delay(3000); 
         status("TO CANCEL PRESS HW RESET NOW . . ."); delay(10000); rp2040.rebootToBootloader(); break; }   
         case 41: ///////////////////// KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x33 *r3* Enable touble-press HW Reset for Reboot to UF2 Loader
       { status("Pico 2 NA Enable double-tap HW reset new UF2"); delay(3000); /*rp2040.enableDoubleResetBootloader();*/ StarOk = true; break; }  
         case 42: ///////////////////// KeyBrdByte[1]==0x6c&&KeyBrdByte[2]==0x66 *lf* List Files on SDCard and Flash
       { DoFileList(); StarOk = true; break; } 
         case 43: ////////////////////// KeyBrdByte[1]==0x75&&KeyBrdByte[2]==0x70 *up* Upper/Lowercase macrokeys filenames
       { MacroUL = !MacroUL; SwitchMacroUL(1); StarOk = true; break; }   // Toggle On/Off and change Padlabels
         case 44: ////////////////////// KeyBrdByte[1]==0x69&&KeyBrdByte[2]==0x73,74,6d *im,s,t*12numbers for MacroInstructionList 12 numbers 0-9
       { if (k2==0x78) { iList = !iList; Config1[23] = iList; WriteConfig1Change = true;      // or use WriteConfig1(0);
                         if (iList) status("Instruction List ON"); else status("Instruction List OFF"); StarOk = true; break; }       
         if (k2==0x73) i = 1; if (k2==0x74) i = 2; if (k2==0x6d) i = 0;                        // *is*, *it*, *im* 
         if (knum<5) { for (n=0; n<iListMax; n++) MacroInstructionList[i][n] = MacroInstructionListDefault[i][n]; 
             status("Instruction List Reset"); SaveInstructionList(); StarOk = true; break; }  // Reset if not numbers added                  
         for (n=0; n<knum; n++) MacroInstructionList[i][n] = KeyBrdByte[4+n]-48;      // Instruction list 0-9, or 49-52 - max iListMax=12 entries
         status("Instruction List filled"); SaveInstructionList(); StarOk = true; break; }     // a=0x62=97->after -48 a b c d = 49 50 51 52
         case 45: ////////////////////// KeyBrdByte[1]==0x30&&KeyBrdByte[2]==0x39 *09* Number-Pad or 96 n-Keys n01-n96
       { nKeys = !nKeys; Config1[7] = nKeys; WriteConfig1(0); 
         if (nKeys) status("n-Keys n01-n996 On"); else status ("NumPad pages 1-8 On"); NumKeysChange(); StarOk = true; break; }
         case 46: ////////////////////// KeyBrdByte[1]==0x30&&KeyBrdByte[2]==0x6e *0n*char = 1-9 or aA - zZ Select nKeys Char 
       { if (knum>5) break; // Single digits or char only after *0n* no filename validity checks
         if (knum==5) { status("nKeys character changed"); Config1[8] = nChar = KeyBrdByte[4]; 
         WriteConfig1(1); NumKeysChange(); StarOk = true; break; } }
         case 47: ////////////////////// KeyBrdByte[1]==0x30&&KeyBrdByte[2]==0x70 *0p*pages = 1-83 Select pages number 
       { if (knum>6) break; // 1-83 only after *0p* 
         if (knum==5) { Config1[9] = nKeysPage = b;   if (b==0||b>9) { status("Use 1-9 Pages"); break; } }
         if (knum==6) { Config1[9] = nKeysPage = c99; if (nKeysPage==0||nKeysPage>83) { status("Use 1-83 Pages"); break; } }
         status("1-83 Pages nKeys changed"); WriteConfig1(1); StarOk = true; break; }
         case 48: ////////////////////// KeyBrdByte[1]==0x30&&KeyBrdByte[2]==0x73 *0s*10char = set of 10 char to choose from 
       { if (knum>5) { status("nKeys shortlist characters changed");
                       for (n=0; n<knum-4; n++) nKeysCharSet[n] = KeyBrdByte[4+n]; WriteConfig1(1); StarOk = true; break; } }                     
         case 49: ////////////////////// KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x6E *rn*newname=oldname  
       { if (knum<7) { status("Use *rn*oldname=newname name is file or /folder"); StarOk = true; break; }
         n = 0; while (n<=knum-4) { KeyBrdByte[n] = KeyBrdByte[n+4]; n++; } knum = knum - 4;        
         RenameMacro(); StarOk = true; break; }  // Test by *rn*old=new then press [EXE] (not [Sav] or [Ren])
         case 50: ////////////////////// KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x72 *cr*0-3 
         // byte CRLF = 0,1,2,3  Filter CR 0D \n and LF 0A \r in large text files. 1=Filter CR=0x0D 2=Filter LF=0x0A 3=Filter both CR and LF 
       { if (knum==5 && b<4) { CRLF = Config1[20] = b; WriteConfig1(1);
                               if (b) status("CRLF filter changed"); else status("CRLF filter cleared"); StarOk = true; } break; }     
         case 51: ////////////////////// KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x79 *cx*XY X,Y = Two Nkeys text filter chars default = 0x0D 0x0A 
         // byte crlf1 = 0x0D, crlf2F = 0x0A;  // Two Nkeys text filter chars default = 0x0D 0x0A  
       { if (knum==6) { crlf1 = Config1[21] = KeyBrdByte[4]; crlf2 = Config1[22] = k5;   status("Text filter changed"); StarOk = true; }
         if (knum==5 && b==0) { crlf1 = Config1[21] = 0x0D;  crlf2 = Config1[22] = 0x0A; status("Text filter CR LF");   StarOk = true; } 
         if (StarOk) WriteConfig1(1); break; }
         case 52: ////////////////////// KeyBrdByte[1]==0x30&&KeyBrdByte[2]==0x79 *0x* nKeys34 = nKeys count n00-n99,n100-n999 if true else n000-n999         
       { nKeys34 = !nKeys34; Config1[39] = nKeys34; WriteConfig1(0); 
         if (nKeys34) status("nKeys n01-n99 n100-n999"); else status ("nKeys n001-n999"); NumKeysChange(); StarOk = true; break; }  
         case 53: ////////////////////// KeyBrdByte[1]==0x30&&KeyBrdByte[2]==0x64 *0d*/20-char-max or default mode if *0d*/ or *0d*
       { nDirX=0; if (knum>4) { for (n=0; n<knum-4; n++) nDir[n] = KeyBrdByte[4+n]; 
                                     if (k5=='/') { nDir[1]=nChar; nDir[2]='/'; nDir[3]=0x00; nDirX=2; }  // 2: /nChar/
                                             else { nDir[n]='/'; nDir[n+1]=0x00; nDirX=1+(n>1)*2; }  }    // 3: normal /dirname/ 1: default /filename
         if (knum==4) { nDir[0]='/'; nDir[1]=0x00; nDirX=0; }                                             // 0: default /filename but nDirX=0
         Config1[60] = nDirX; status(nDirXMsg[nDirX]); WriteConfig1Change = StarOk = true; break; }
         case 54: ////////////////////// KeyBrdByte[1]==0x30&&KeyBrdByte[2]==0x74 *0t*10char = set of 10 char in Linkstring to execute as nKeys 
       { if (knum>5) { status("nKeys LinkString characters changed");
                       for (n=0; n<knum-4; n++) nKeysLnkChar[n] = KeyBrdByte[4+n]; WriteConfig1(1); StarOk = true; break; } } 
         case 55: ////////////////////// KeyBrdByte[1]==0x6c&&KeyBrdByte[2]==0x6D,0x73,0x74 *lm* *ls* *lt* + optional filename that contains 24 keylabels
       { // Keylabels On/Off + optional filename that contains 24 keylabels for example *lt*label1 -> LabelT now has content label1 
         // If one char added after *lm,s,t* such as *lm,s,t*x then file FileM,S,T reset with default text label1,2,3
         strcpy(NameStr3, " OFF"); strcpy(NameStr1, "labelx"); strcpy(NameStr2, "LabelX"); // label1,2,3 is default text in files LabelM,S,T 
         if (k2==0x6D) { Config1[72] = MLabel = !MLabel || (knum>5); NameStr2[5] = 'M'; NameStr1[5] = '1'; if (MLabel) strcpy(NameStr3, " ON"); m = 1; }
         if (k2==0x73) { Config1[73] = SLabel = !SLabel || (knum>5); NameStr2[5] = 'S'; NameStr1[5] = '2'; if (SLabel) strcpy(NameStr3, " ON"); m = 3; }
         if (k2==0x74) { Config1[74] = TLabel = !TLabel || (knum>5); NameStr2[5] = 'T'; NameStr1[5] = '3'; if (TLabel) strcpy(NameStr3, " ON"); m = 4; }
         if (knum>4)   { if (knum>5) for (n=0; n<knum-4; n++) NameStr1[n] = KeyBrdByte[4+n];   // default is NameStr1 = label1,2,3
                         if (LayerAxD) f1 = SDFS.open(NameStr2, "w"); else f1 = LittleFS.open(NameStr2, "w"); 
                         f1.print(NameStr1); f1.print('\0'); f1.close();  }         
         DoMSTLabel(0, m); WriteConfig1(0); strcat(NameStr2, NameStr3); status(NameStr2); StarOk = true; break; }
        case 56: ///////////////////// KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x66 *cf*number or *cf*src=dst - copy file source=destination
      { byte cfoption = 0;          // *cf*0 or *cf*00 -> Copy all six default label files from SDCard to Flash, *cf*1 or *cf*01 is Flash to SDCard
        if (knum==4) { status("Use *cf*number=0-9 or *cf*copyfrom=copyto"); StarOk = true; break; }  
        if (knum<7)  { if (knum==5) cfoption = b+1; else cfoption = c99+1; CopyMacro(cfoption); SendBytesEnd(0); StarOk = true; break; }  // 1-10 or 1-100
         n = 0; while (n<=knum-4) { KeyBrdByte[n] = KeyBrdByte[n+4]; n++; } knum = knum - 4;  // remove the *cf* part i.e done by CopyMacro asif no *cm*     
         CopyMacro(cfoption); SendBytesEnd(0); StarOk = true; break; }  
        case 57: ///////////////////// KeyBrdByte[1]==0x61&&KeyBrdByte[2]==0x64 *ad*xs x=a,b,c,d s=s,f (SDCard or Flash) switch layouts A-D brown and white
      { if (knum==4) { LayerAxD = !LayerAxD; }             // LayerAD = 0,1,2,3 = a,b,c,d toggle brown/white keep same A-D A=65 a=97
        if (knum>4)  { n = LayerAD; LayerAD = KeyBrdByte[4] - 97; if (LayerAD<4) LayerADLetter[0] = 65 + LayerAD; else { LayerAD = n; break; } }
        if (knum==6) { if (k5=='s') LayerAxD = true; else if (k5=='f') LayerAxD = false; else break;  }
        optionsindicators(0); ConfigButtons(1); LayerADChange = true; StarOk = true; break; } 
        case 58: ///////////////////// KeyBrdByte[1]==0x6c&&KeyBrdByte[2]==0x79 *lx*lxs l=1-4 x=a,b,c,d s=s,f (SDCard or Flash) switch layouts A-D brown and white
      { if (knum==4) { Layout = 2; }             // Layout = 1-4 LayerAD = 0,1,2,3 = a,b,c,d toggle brown/white keep same A-D A=65 a=97
        if (knum>4)  { if (b>0&&b<5) { Layout = b; if (knum==5) { ConfigButtons(1); StarOk = true; break; } } }
        if (knum>5)  { n = LayerAD; LayerAD = k5 - 97; if (LayerAD<4) LayerADLetter[0] = 65 + LayerAD; else { LayerAD = n; break; } }
        if (knum==7) { if (k6=='s') LayerAxD = true; else if (k6=='f') LayerAxD = false; else break;  }
        optionsindicators(0); ConfigButtons(1); LayerADChange = true; StarOk = true; break; } 
        case 59: ///////////////////// KeyBrdByte[1]==0x61&&KeyBrdByte[2]==0x65 *ae*x x=0-3 Macroeditor Source Destination = SDCard/Flash brown/white
      { if (knum==4) { SrcDst++; if (SrcDst>3) SrcDst=0; status("SrcDst +"); } // 00 both flash 01 Src=flash Dst=sdcard 10 Sce=sdcard Dst=flash 11 both sdcard
        if (knum==5) { if (b<4) { SrcDst = b; status("SrcDst changed"); } else status("Enter*ar*0-3 Src Dst");  }
        if (knum>5)  { status("Enter*ar*0-3 Src/Dst"); }
        StarOk = true; break; }                             
      } return StarOk; 
}

////////////////////
bool TestXFiles()
////////////////////
{ byte m;
  bool isXFiles = false;
  byte xnum[6] = { 14,15,16,14,15,16 };  
  for (m=0; m<6; m++) { if (XNum[m] != xnum[m]) isXFiles = true; }
  return isXFiles;
}
 
////////////////////
void DoFileList()
///////////////////
{ 
  status("SDCard and Flash Filelist Sent"); 
  Serial.println("Flash Files (Number Name Size):");  ListFiles(); 
  Serial.print("\n"); 
  Serial.println("SDCard Files (Number Name Size):"); ListSDFiles(0);
}

////////////////////////////////////////////////////////
void DoMSTAName(int n, int msta)
////////////////////////////////////////////////////////
{ //                  012345678901
  char macroMSTA[] = "mstaknMSTAKN";     // MST1 MST2 0-5
  macroMSTA[5] = macroMSTA[11] = nChar;  // No upper/lower case switch for nKeys nChar  
  n++;  
  MSTAName[1] = 48 + n/10;  MSTAName[2] = (n)%10 + 48;   
  MSTAName[0] = macroMSTA[msta+(MacroUL)*6];
  // Serial.println(MSTAName);
  if (nDirX && msta==5) strcat(nDir, MSTAName);
  // Serial.println(MSTAName);
}
///////////////////////////////////////////////////////////////////////////////
void DoMSTName(int n, int mst)  // MacroUL=0,1 Lower,Uppercase
///////////////////////////////////////////////////////////////////////////////
{ char macroMST[] = "0mcst0MCST0";  // Layout 1,3,4 = M,S,T
  n++;
  MSTName[1] = 48 + n/10;  MSTName[2] = (n)%10 + 48;   
  MSTName[0] = macroMST[mst+(MacroUL)*5];  
  // Serial.println(MSTName);
}
///////////////////////////////////////////////////////////////
void DoMSTLinkName(int n, int mst)
///////////////////////////////////////////////////////////////
// char MSTLinkName = "XnnLink"; 
// Use n+1 because filenames are 01-24 
///////////////////////////////////////////////////////////////
{ char macroMST[] = "0MCSTKN";  // Layout 1,3,4 = M,S,T also K
  macroMST[6] = nChar;
  n++;
  MSTLinkName[1] = 48 + n/10;  MSTLinkName[2] = (n)%10 + 48;   
  MSTLinkName[0] = macroMST[mst] ;
  // Serial.println(MSTLinkName); 
}
///////////////////////////////////////////////////////////////
void DoMSTALinkName(int n, int mst)
///////////////////////////////////////////////////////////////
// char MSTLinkName = "XnnLink"; 
// Use n+1 because filenames are 01-24 
///////////////////////////////////////////////////////////////
{ char macroMST[] = "MSTAKN";  // MST1 MST2 0-5
  macroMST[5] = nChar;
  n++;
  MSTLinkName[1] = 48 + n/10;  MSTLinkName[2] = (n)%10 + 48;   
  MSTLinkName[0] = macroMST[mst] ;
  // Serial.println(MSTLinkName); 
}

//////////////////////////////////////////////////////////////////////////////////
void SendBytesEnd(bool All)
//////////////////////////////////////////////////////////////////////////////////
{ int n;
  KeyBrdByteNum = 0; KBDispPos = 0;  KeyBrdX = 2; KeyBrd123 = 0;
  for (n = 0; n < 200; n++) { KeyBrdByte[n] = 0x00; KBDisp[n] = '\0'; }
  if (All) {Kbrd = false;} 
  KBType = false; Fx = false; KBrdActive = false; 
  optionsindicators(0); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void SendBytes()    
/////////////////////////////////////////////////////////////////////////////////////////////////////        
// Send and (possibly must always aA kK) Save KeyBrd Macro Editor macros 01 - 99,00
// NB: If mst and then not saved by [Up] then key Mx Sx Tx will not execute macro if key is pressed
// Destination is Option 2 i.e. [Dst] = rightmost number in KeyBrd 
// M S T A is also known but only saved to M S T when [Up] key pressed (only 24 keys max)
// If destination is A then saved to a00-a99 in here (100 keys max if A selected)
// If nKeys action here is to display content of n01-n96 files - does not execute the content
// If *Code then return early - do not try to execute as macro/text/link, and/or save to a file
/////////////////////////////////////////////////////////////////////////////////////////////////////
{ uint8_t keycode[6]; // simultaneous keys pressed in here
  uint8_t a, b, n, i, MST2A;
  bool sdCard = false;
  //                  0123456789012345678
  char status0[]  = {"Macro a00 saved"} ;       // saved as a00-a99 
  char status1[]  = {"Press [Up] Save x00"} ;   // saved as m s t 00-24 or n01-n96
  //                  012345678901
  char status2[]  = {"mstaknMSTAKN"} ;          // MST2 0 1 2 3 4 5 -> m s t a k nChar
  char NameStr[4] = {"xnn"}  ;  
  File f;
  
  status2[5] = status2[11] = nChar;
  
  // Check for special commands Start with * eg *ab*n n = 0-9 - ignore * codes if double **
  // CmKey Check if *codes are from pressing [*Cm] key or entered directly
  if ((KeyBrdByte[0]==0x2A)&&(KeyBrdByte[1]!=0x2A)) 
     {if (CmKey) StarNum--; if (SendBytesStarCodes()) return; else status("*Code incorrect"); return; }   
  if ((KeyBrdByte[0]==0x2A)&&(KeyBrdByte[1]==0x2A)) { for (n = 0; n < KeyBrdByteNum; n++) KeyBrdByte[n] = KeyBrdByte[n+1]; KeyBrdByteNum--; }

  for (n=0; n<6; n++) keycode[n] = 0x00;  MacroBuffSize = KeyBrdByteNum;
  
  // sequential via keyPress 
  if ((!Fx)&&(KeyBrdByte[0]<0x80))                                              // F1-F24 must be sent with keycode not keyPress
     { for (n = 0; n < KeyBrdByteNum; n++) if (KeyBrdByte[n]<0x80)              // When 1st char is normal alphanumeric all must be <0x80    
           {usb_hid.keyboardPress(HIDKbrd, KeyBrdByte[n]); delay(dt50);     // Converted via end of hid.h from ascii to hid
            usb_hid.keyboardRelease(HIDKbrd);              delay(dt50); 
            MacroBuff[n] = KeyBrdByte[n];}    }

  // simultaneous via keycode         
  if ((Fx)||(KeyBrdByte[0]>0x7F)&&(KeyBrdByteNum<6))                            // Fnn = 3A-45 + 68-73 hex
     { for (n = 0; n < KeyBrdByteNum; n++) {keycode[n] = KeyBrdByte[n]; MacroBuff[n] = KeyBrdByte[n]; }  
           usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(dt50);
           usb_hid.keyboardRelease(HIDKbrd);            delay(dt50); } 

  MST2A = Option2 + 24*(MST2==1) + 48*(MST2==2);   // M to a01-a24 S to a25-a48 T to a49-a72   
  if (macroA) { DoMSTAName(MST2A, 3);             strcpy(NameStr, MSTAName); 
                status1[17] = (MST2A+1)/10 + 48;   status1[18] = (MST2A+1)%10 + 48;   }  // a01-a24, a25-a48, a49-a72
        else  { DoMSTAName(Option2, 3+(MST2==4)+2*(MST2==5)); strcpy(NameStr, MSTAName); 
                status1[17] = (Option2+1)/10 + 48; status1[18] = (Option2+1)%10 + 48; }  // a01 - a99 k01 - k99 n01 - n96
  status1[16] = status2[MST2+(MacroUL)*6]; 

  if (SrcDst==1||SrcDst==3) sdCard = true;  if (SrcDst==2||SrcDst==0) sdCard = false;  // For Destination 0,2=Flash 1,3=SDCard 
  
  if (MST2>2) { if (!sdCard) f = LittleFS.open(NameStr, "w");      // Only save to file axx or kxx or nxx if option target is a k n
                if (sdCard)  f = SDFS.open(NameStr, "w");          
                if (f) { f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); } 
                status0[6] = status1[16]; status0[7] = status1[17]; status0[8] = status1[18]; status(status0); DoUpKey = false; }
                 
  if (MST2<3)  { status(status1); DoUpKey = true; }

  // Note also saves: DoFileBytes(1, M,S,T TRname[Option2], BPtr, MacroBuffSize[Option2]) in buttons() case [Up]
  
   /*  for (n = 0; n < KeyBrdByteNum; n++) 
          {Serial.print(KeyBrdByte[n], HEX);
           Serial.print(' '); }
      Serial.println(" ");
      for (n = 0; n < 6; n++)   
          {Serial.print(keycode[n], HEX);
           Serial.print(' '); }               */ 
              
  SendBytesEnd(1); //clean-up
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool LinkFileMacro() // Destination
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
// Link Keys M,S,T,K 1-24 to an Mxx Sxx Txx SDCard/Flash File that contains 3 letter filenames stored in SDCard/Flash
// will be executed in sequence such as file M01 on SDCard has content a01a22s01s22 => execute first file a01 contents
// then a22 then s01 then s22. Files can be mix of string or macros. Size not needed stop file excution when byte = 0x00
//
// The XnnLink file will use the destination MST nn (not Source MST nn), to form the XnnLink file
//
// Construct the string a01a22s01s22 in the Macro editor or send from PC serial as <1a01a22s01s22> when in Layout 1 
// Layer orange A-D then saved on SDCard if white A-D saved on Flash with a capital MST (not mst) first letter filename
//
// From Macro Editor do when [Lnk] pressed use KeyBrdByte[KeyBrdByteNum] save as file M01 on SDCard instead of saving 
// two files a01 to flash memory when [EXE] pressed and also m01 when [Up] pressed
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ uint16_t ByteLen = 0;   
  char LinkStr[20] = {"Link File: "};
  bool LinkOK = false;
  bool sdCard = false;
  File f;

  DoMSTALinkName(Option2, MST2);
  if (SrcDst==1||SrcDst==3) sdCard = true;  if (SrcDst==2||SrcDst==0) sdCard = false;  // For Destination 0,2=Flash 1,3=SDCard
  
  if (sdCard) f = SDFS.open(MSTLinkName, "w"); else f = LittleFS.open(MSTLinkName, "w");
  f.write(KeyBrdByte, KeyBrdByteNum);   // write array flashmem file case-sensitive
  f.print('\0');                        // f.println();
  ByteLen = f.size();
  f.close();  
  
  strcat(LinkStr, MSTLinkName); 
                  
  // Serial.println(NameStr); Serial.println(ByteLen); Serial.print(KeyBrdByte[0]);SerPr1;Serial.println(KeyBrdByte[1]);
      
  if (ByteLen>0) { status((char *)LinkStr); LinkOK = true; } else status("Link invalid");
  return LinkOK; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool RenameMacro() 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Source (or entered name) -> Destination must be both on same SDCard or Flash
//                    Oldname=Newname or /OldFolder=/NewFolder 
//                    *Code and Serial: Use *rn*old=new[EXE] or <*rn*old=new> or *rn*/olddir=/newdir
//                    Can be small or large files > 200 bytes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ bool RenOK = false;
  bool sdCard = false;
  byte m, k = 0, n = 0;
  char RenameStr[70] = {"Renamed: "}; 
  
  if (KeyBrdByteNum>0) 
     { while (k!=0x3D){ k = NameStr1[n] = KeyBrdByte[n]; n++; if (n>80) { status("Names too long"); return RenOK; } } NameStr1[n-1] = 0x00; // 0x3D = 
       for (m=n; m<KeyBrdByteNum; m++) NameStr2[m-n] = KeyBrdByte[m]; NameStr2[m-n] = 0x00; }          
   
  if (KeyBrdByteNum==0) { DoMSTAName(Option1, MST1); strcpy(NameStr1, MSTAName); DoMSTAName(Option2, MST2); strcpy(NameStr2, MSTAName); }

  if (SrcDst==0||SrcDst==1) sdCard = false; if (SrcDst==2||SrcDst==3) sdCard = true;   // For Source 0,1=Flash 2,3=SDCard
    
  if (!sdCard) RenOK = LittleFS.rename(NameStr1, NameStr2);    // FlasMem files: Rename 1 to 2 for m s t k files and a00-a99
          else RenOK = SDFS.rename(NameStr1, NameStr2);        // SDCard files:  Rename 1 to 2 for M S T K files and a00-a99
  
  if (RenOK) { strcat(RenameStr, NameStr1); strcat(RenameStr, " to "); strcat(RenameStr, NameStr2); status((char *)RenameStr); }
          else status("Use Oldname=Newname or Xnn Ynn");
          
  return RenOK;  
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool RemoveMacro() // Source
/////////////////////////////////////////////////////////////////////////////////////////////
// If entered name include two // to indicate directory to be deleted
// One /name will be treated as a file name and //name as a directory name
// For deleting non-empty folders see: https://github.com/littlefs-project/littlefs/issues/43
// And the fix: https://github.com/littlefs-project/littlefs/pull/557
// clear all K keys = 0x00 if name = "k00" entered
/////////////////////////////////////////////////////////////////////////////////////////////
{ bool RemOK = false;
  bool sdCard = false;
  byte doDir = 0;
  char RemStr[40] = { "Source file/dir File Deleted: " }; 
  int a, b, n;
  bool ClearBSD = false;

  // clear all K keys = 0x00 if name = "k00" entered 
  if (KeyBrdByteNum==3) { a = KeyBrdByte[0]; b = (KeyBrdByte[1]-48)*10 + KeyBrdByte[2]-48; ClearBSD = (a==107&&b==0); }                                 
  if (ClearBSD) { SaveBSDKeyKArr(ClearBSD); ReadBSDKeyKArr(); status("Key K01-K24 macros cleared"); return true; } 

  if (KeyBrdByteNum>3&&KeyBrdByte[0]=='/'&&KeyBrdByte[1]=='/') doDir = 1;  // Serial.println("isDir"); // Two '//' means delete directory
                           
  if (KeyBrdByteNum>0&&KeyBrdByteNum<40) { for (n=0; n<(KeyBrdByteNum-doDir); n++) NameStr1[n] = KeyBrdByte[n+doDir]; NameStr1[n] = 0x00; }
      else { DoMSTAName(Option1, MST1); strcpy(NameStr1, MSTAName); }   

  // Serial.println(NameStr1);    
       
  if (SrcDst==0||SrcDst==1) sdCard = false; if (SrcDst==2||SrcDst==3) sdCard = true;   // For Source 0,1=Flash 2,3=SDCard 
      
  if (sdCard)  { if (!doDir) RemOK  = SDFS.remove(NameStr1);     else RemOK  = SDFS.rmdir(NameStr1);     }
  if (!sdCard) { if (!doDir) RemOK  = LittleFS.remove(NameStr1); else RemOK  = LittleFS.rmdir(NameStr1); }

  // Serial.println(NameStr1); Serial.println(sdCard); Serial.println(MSTAName); 
  
  if (RemOK) { strcat(RemStr, NameStr1); status((char *)RemStr); } else status("File/Dir not found/empty");
  return RemOK;   
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool SendMacro()  // Source
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Different from [EXE] - no [Up]key save option - already saved to a file <200 bytes size
// Cannot list macros here without a file - for example those loaded using *fm,s,t*
// Because DoFileBytes(0, NameStr will not find file NameStr
// Works on macros loaded using serial port - they are saved to a file such as <3This is Key S3>
// Executes first on anything entered with [ADD]
/////////////////////////////////////////////////////////////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 };      // simultaneous keys pressed in here
  char NameStr[4] = { "Xnn" };
  byte *BPtr;
  int n;
  bool sdCard = false;
  
  if (KeyBrdByteNum>0) { MacroBuffSize = KeyBrdByteNum; for (n = 0; n < KeyBrdByteNum; n++) MacroBuff[n] = KeyBrdByte[n]; }  // If a macro content was entered
      else { DoMSTAName(Option1, MST1); strcpy(NameStr, MSTAName);             // A K N - Use files ak01n01 - ak99n96 
             if (SrcDst==0||SrcDst==1) sdCard = false; if (SrcDst==2||SrcDst==3) sdCard = true;   // For Source  0,1=Flash 2,3=SDCard 
                   
             BPtr = MacroBuff; MacroBuffSize = DoFileBytes(0, NameStr, BPtr, ByteSize, LayerAxD); 
             if (MacroBuffSize==0) { status("Macro's not found or too large"); return false; } }
             
  if (MacroBuff[0]<0x80) { n = 0; while (MacroBuff[n]!=0 && MacroBuffSize>n) {usb_hid.keyboardPress(HIDKbrd, MacroBuff[n]);  delay(dt25);
                                                                              usb_hid.keyboardRelease(HIDKbrd);              delay(dt25); 
                                                                              n++; if (n>ByteSize) break; } return true; }
  if (MacroBuff[0]>0x7F) { for (n=0;  n<6; n++) {keycode[n] = MacroBuff[n]; if (keycode[n]==0) break; }    // allow one 0x00 in keycode
                                                 usb_hid.keyboardReport(HIDKbrd, 0, keycode);              delay(dt25); 
                                                 usb_hid.keyboardRelease(HIDKbrd);                         delay(dt25); 
                                                 return true; }                                            // if (Layout==2) 
 return false;
}

/////////////////////////////////
void CopyLabelFiles(byte Option)
/////////////////////////////////
{ char LabelFile[7] = "LabelM";
  char LabelF[7]    = "label1";
  byte a = 0;
  int n;
  char MST[]      = "MST";
  char label123[] = "123";
  File f1, f2;
  
  for (a=0; a<3; a++) 
      { LabelFile[5] = MST[a];                       
        if (Option==1) f1 = SDFS.open(LabelFile, "r"); else f1 = LittleFS.open(LabelFile, "r"); n = f1.size(); if (!f1) return;
        f1.readBytes(NameStr1, n); f1.close();        // NameStr1 is the filename that holds the 24 (5 char max), new label names
        
        if (Option==1) f2 = LittleFS.open(LabelFile, "w"); else f2 = SDFS.open(LabelFile, "w");  if (!f2) return;  // Filename LabelM,S,T
        f2.write(NameStr1, n-1);                                                                                   // Write filename to SDCard or Flash
        f2.print('\0');                                                                                            // Add NULL to end of filename in File LabelX    
        f2.close(); 

        LabelF[5] = label123[a];                       
        if (Option==1) f1 = SDFS.open(LabelF, "r"); else f1 = LittleFS.open(LabelF, "r"); n = f1.size(); if (!f1) return;
        f1.readBytes(NameStr2, n); f1.close();        // NameStr2 is the label1,2,3 filename that has the label definitions 144 bytes size
        
        if (Option==1) f2 = LittleFS.open(LabelF, "w"); else f2 = SDFS.open(LabelF, "w");  if (!f2) return;  // Filename label1,2,3
        f2.write(NameStr2, n);                                                                               // Write filename to SDCard or Flash
        // f2.print('\0');                                                                                   // Add NULL to end of filename in File LabelX    
        f2.close(); 
      }
}

/////////////////////////////////////////////////////////////////////////
// To copy Macro1-99 to Keys M1-24,S1-24,T1-24,K1-K24
// Compose *cm*nnXmm via [ADD] where nn = 01-99 X = M S T A K mm = 01-24
// KeyBrdByte = *cm*nnXmm  = KBDisp
// OR just use *cm* will then copy using current settings displayed
// via MST1, MST2, Option1, Option2
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CopyMacro(byte Option)  
// Only use for files < 200 char/bytes not large files because its use MacroBuff to copy
// Use SourceFile=DestinationFile can be any combination of SDCard and Flash - or use description below:  
// Source -> Destination where Source Macro M S T A K N must already be saved to a file
// Destination Macro can be M S T or A K N (or use *cm*nnXmm )
// Does not add extra 0x00 at end via DoFileBytes(2,  ) -> size same
// Knn macros not saved as files they are BsDCode1-BsDCode3 -> max 3 bytes
// There are files k01,K01-K24,k24 used by S1-S24 keys and loaded via DoSDCard - these file are not
// connected to Keys K1-K24 which execute either 3 byte macros in BSD1-3 or execute macrolinkfiles K01Link-K24Link
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                             012345678901234
{ char KeyBrd5status[16] =   {"Macro nn -> Xnn"};
  int Xlat1[5] = {6,7,12,13,14};
  int MST = 4; // Later assigned to MST1 or MST2
  byte *BPtr;
  int a, b, m, n =0, k=0;
  bool StarCMOK = false;
  bool sdCard = false;
  bool Found2Names = false; 

  
  if (Option==1) { CopyLabelFiles(Option); status("Label files copied SDCard->Flash"); return; }
  if (Option==2) { CopyLabelFiles(Option); status("Label files copied Flash->SDCard"); return; }
  if (Option>2)  { status("Option 2 not yet implemented"); return; } 

  if (KeyBrdByteNum>2&&KeyBrdByte[0]!=0x2A)  // Not * starcode - use Name1=Name2 -> Copy Name 1 to Name 2
     { while (k!=0x3D){ k = NameStr1[n] = KeyBrdByte[n]; n++;  // 0x3D = 
                        if (n>60) { status("Names too long"); return; } 
                      } NameStr1[n-1] = 0x00;  
       Found2Names = true; for (m=n; m<KeyBrdByteNum; m++) NameStr2[m-n] = KeyBrdByte[m]; NameStr2[m-n] = 0x00; }       
  
  MST = MacroBuffSize = 0;
  a = KeyBrdByte[6];    // M S T A K nChar = 77 83 84 65 75 xx = 0 1 2 3 4 5
  b = (a==65||a==97)*3 + (a==75||a==107)*4  + (a==77||a==109)*0 + (a==83||a==115)*1 + (a==84||a==116)*2 + (a==nChar)*5; // aA kK mM sS tT xX
  if (b>5) b = 5;                                                                                 // nChar can be also = m s t a k M S T A K

  ////////////////////////////////////////////////////////////////////////////////////////////// Do *cm* here
  if (KeyBrdByteNum>4&&!Found2Names) 
     { MST = b; if (MST>5) return;                                          // *cm*nnXmm MSTAKN = 0 1 2 3 4 5
       for (int n=0; n<5; n++)  KeyBrd5status[Xlat1[n]]  = KeyBrdByte[n+4]; // 012345678 S=83 83-77=6->=1 MST2
       Option1 = (KeyBrdByte[4]-48)*10 + KeyBrdByte[5]-49;                  // Option1 = 0 - 23 = macro a01 - a99
       Option2 = (KeyBrdByte[7]-48)*10 + KeyBrdByte[8]-49;                  // Option2 = 0 - 23 keys M S T K = 1 - 24
       if (Option1>99 || Option2>99) return;                                // Max A=99 Max MSTK = 24 Max N = 96
       MST1 = 3; MST2 = MST; StarCMOK = true; }                             // Copy from a01-a99 to M S T K 01-24
  
  DoMSTAName(Option1, MST1);  // Source Filename in MSTAName  
  if (!Found2Names) strcpy(NameStr1, MSTAName); 
  
  if (SrcDst==0||SrcDst==1) sdCard = false; if (SrcDst==2||SrcDst==3) sdCard = true;   // For Source 0,1=Flash 2,3=SDCard 
  
  BPtr = MacroBuff; MacroBuffSize = DoFileBytes(0, NameStr1, BPtr, ByteSize, sdCard);  // Read source macro from Flash or SDCard  
  if (MacroBuffSize==0)          { status("Source Macro not found");   return; }   
  // if (MacroBuffSize>3&&MST2==4)  { status("Knn maximum 3 byte macro"); return; }    // Knn macros are BsDCode1-BsDCode3 -> max 3 bytes

  DoMSTAName(Option2, MST2);  // Destination Filename in MSTAName
  if (!Found2Names) { strcpy(NameStr2, MSTAName);
                      if (Option1==Option2&&MST1==MST2&&(SrcDst==0||SrcDst==3)) { status("Source=Destination"); return; } } 

  if (SrcDst==1||SrcDst==3) sdCard = true;  if (SrcDst==2||SrcDst==0) sdCard = false;  // For Destination 0,2=Flash 1,3=SDCard  

  if (Found2Names) {  BPtr = MacroBuff; MacroBuffSize = DoFileBytes(2, NameStr2, BPtr, MacroBuffSize, sdCard); // Write destination macro to Flash or SDCard  
                     if (MacroBuffSize>0) status("Macro Copied check file list"); else status("Macro not Copied"); return; }        
  
  if (MST2==3) { DoFileBytes(1, MSTAName, BPtr, MacroBuffSize, sdCard); }              // Save Destination a01-a99 to Flash or SDCard

  if (MST2==4) { BsDCode1[Option2+27] = MacroBuff[0]; BsDCode2[Option2+27] = MacroBuff[1]; BsDCode3[Option2+27] = MacroBuff[2]; // Define K01-K24 keys
                 SaveBSDKeyKArr(0); }                                                  // Save new defines for K01-K24 keys
          
  if (MST2==0) { for (n=0; n<MacroBuffSize; n++) { Mtr1to12[Option2][n] = MacroBuff[n]; } MacroM1M12[Option2] = 3; BPtr = Mtr1to12[Option2];  
                      MacroSizeM1M12[Option2] = MacroBuffSize; DoFileBytes(2, MSTAName, BPtr, MacroBuffSize, sdCard); }  // Save Destination File

  if (MST2==1) { for (n=0; n<MacroBuffSize; n++) { Str1to12[Option2][n] = MacroBuff[n]; } MacroS1S12[Option2] = 3; BPtr = Str1to12[Option2];  
                      MacroSizeS1S12[Option2] = MacroBuffSize; DoFileBytes(2, MSTAName, BPtr, MacroBuffSize, sdCard); }  // Save Destination File  

  if (MST2==2) { for (n=0; n<MacroBuffSize; n++) { Ttr1to12[Option2][n] = MacroBuff[n]; } MacroT1T12[Option2] = 3; BPtr = Ttr1to12[Option2];
                      MacroSizeT1T12[Option2] = MacroBuffSize; DoFileBytes(2, MSTAName, BPtr, MacroBuffSize, sdCard); }  // Save Destination File

  if ((StarCMOK)&&(MacroBuffSize>0)) { status((char *)KeyBrd5status); return; }
  if ((MST!=4)&&(MacroBuffSize>0))     status("Macro Copied check file list"); else status("Macro's not found");
                 
}

///////////////////////////////////////////////////////////////////////////////////////////
// MST1 MST2  used to define source and destination macro for copy and/or linking/chaining
// Source = A (mAcro00-mAcro99) or Keys M S T 1-24 for copy or A, K 1-99 for linking
// Destination = Keys M S T 1-24 for copy or A K for linking
///////////////////////////////////////////////////////////////////////////////////////////
int GetMST(byte b)                       // MST = 0 1 2 3 4 5 = M S T A K N keys
/////////////////////////////////////////////////////////////////////////////////////
{ 
  int MST = 5; // On return assigned MST1 or MST2
  // MST = b-62; if (MST!=3) MST = MST-12; if (MST>0) MST = MST-6;  
  if (b==0x4D)  MST = 0; // 0x4D=77 Source = M Keys 1-24
  if (b==0x53)  MST = 1; // Source = S Keys 1-24 
  if (b==0x54)  MST = 2; // Source = T Keys 1-24 
  if (b==0x41)  MST = 3; // Source = A = a01-a99 
  if (b==0x4B)  MST = 4; // Source = k = k01-k99 
  if (b==nChar) MST = 5; // 0x4E=77=N Keys 1-96 or xX
  return MST;
}
///////////////////////////////////////////////////////////////////// 
void DoSourceMacro()    
/////////////////////////////////////////////////////////////////////
{ byte a, b;
  if (KeyBrdByteNum==0) {MST1++; if (MST1>5) MST1=0; }  
   
  if (KeyBrdByteNum>0)
     {a = KeyBrdByte[0]; b = a - 48;      
      if (KeyBrdByteNum==3)             { Option1 = (KeyBrdByte[1]-48)*10 + KeyBrdByte[2]-49; MST1 = GetMST(a); }
      if ((KeyBrdByteNum==2)&&(b<10))     Option1 = b*10 + KeyBrdByte[1]-49;
      if ((KeyBrdByteNum==2)&&(b>9))    { Option1 = KeyBrdByte[1]-49;                         MST1 = GetMST(a); }
      if ((KeyBrdByteNum==1)&&(b<10))     Option1 = b; 
      KeyBrdByteNum=0; }                  // Else [Src][Dst][Num] ignored
    
 NumMode = 1;
 optionsindicators(0);
}
///////////////////////////////////////////////////////////////////// 
void DoDestMacro()    
/////////////////////////////////////////////////////////////////////
{ byte a, b;
   if (KeyBrdByteNum==0) {MST2++; if (MST2>5) MST2=0; }  
 
  if (KeyBrdByteNum>0)
     {a = KeyBrdByte[0]; b = a - 48;      // Option2 = 0 - 11 = macro 1 - 99  
      if (KeyBrdByteNum==3)             { Option2 = (KeyBrdByte[1]-48)*10 + KeyBrdByte[2]-49; MST2 = GetMST(a); }
      if ((KeyBrdByteNum==2)&&(b<10))     Option2 = b*10 + KeyBrdByte[1]-49;
      if ((KeyBrdByteNum==2)&&(b>9))    { Option2 = KeyBrdByte[1]-49;                         MST2 = GetMST(a); }
      if ((KeyBrdByteNum==1)&&(b<10))     Option2 = b; 
      KeyBrdByteNum=0; }                  // Else [Src][Dst][Num] ignored
          
 NumMode = 2;
 optionsindicators(0);
}
///////////////////////////////////////////////////////////////////// 
void DoNumMacro()    
/////////////////////////////////////////////////////////////////////
{ byte a, b;
  int Option12, nCount; // Assigned to Option1/2

  nCount = nKeysPage*12;    // Default = 20 pages = 240 nKeys but can be 12 - 996 = 1 - 83 pages 
  
  if (KeyBrdByteNum==0) { if (NumMode==1) { Option1++; 
                             if (MST1<3)           if (Option1>23)      Option1=0; // M S T = 01 - 24 (6 x 4 pages)        
                             if (MST1==3||MST1==4) if (Option1>98)      Option1=0; // A K = 01 - 99
                             if (MST1==5)          if (Option1>=nCount) Option1=0; // N = 01 - 996 (12 x 83 pages max)
                             status("Source Macro Number");  }  
                         if (NumMode==2) { Option2++; 
                             if (MST2<3)            if (Option2>23)      Option2=0;
                             if (MST2==3||MST2==4)  if (Option2>98)      Option2=0; 
                             if (MST2==5)           if (Option2>=nCount) Option2=0; // N Keys
                             status("Destination Macro Number"); } }
  
  if (KeyBrdByteNum>0)
     {a = KeyBrdByte[0]; b = a - 48;      
      if (KeyBrdByteNum==3)             { return; }
      if ((KeyBrdByteNum==2)&&(0<b<10))   Option12 = b*10 + KeyBrdByte[1]-49;
      if ((KeyBrdByteNum==2)&&(b>9))    { return; }
      if ((KeyBrdByteNum==1)&&(0<b<10))   Option12 = b;    
      if (NumMode==1) { Option1 = Option12; status("Source Macro Number"); }
                  else{ Option2 = Option12; status("Destination Macro Number");  }
      KeyBrdByteNum=0; }                  // Else [Src][Dst][Num] ignored      

 optionsindicators(0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetTimerMacro ()  // Source
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// char TimerName[8][10] = {"Xnn    ","XnnLink","Macro00","Macro00","Macro00","Macro00","Macro00","Macro00"}; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ byte a, b, c, n;
  bool isLink = false;        // If >1 character added then will use XnnLink not Xnn for macroname
  bool sdCard = false;
  
  byte XLat[] = {1,3,4,2,0,0};  // MST1 = 012345 MSTAKN to Layout 1 3 4 (2 0 0 = A K N)
  
  // MacroTimerOK = false; // Set true if both Macro and Timer assigned - can check TimerDisp[6]!='X' for Macro and TimerDisp[17]!=' '
  
  b = 0; // b = 1,2,3,4,5,6,7,8 = TimerName[0-7] = Timer type Repeat T Repeat t Once T Once t Repeat C Once C RcT OcT (Clock Countdown Timers) 
  if (KeyBrdByteNum>0) { a = KeyBrdByte[0]; b = a - 48; }  // add 1-8 and a ='1'-'8' b=1-8
  if (b>8||b==0||KeyBrdByteNum==0) { status("Add 1-8 timer option"); return false; } 
  
  if (KeyBrdByteNum>1) { isLink = true;  DoMSTALinkName(Option1, MST1); strcpy(TimerName[b-1],  MSTLinkName); }  // XnnLink if 1*-8* or 1L-8L added    
                  else { isLink = false; DoMSTAName(Option1, MST1);     strcpy(TimerName[b-1],  MSTAName);    }  // Xnn if 1-8 added
  
  //for (n=0; n<8; n++) TimerName[n][0] = 0x00;

  if (SrcDst==0||SrcDst==1) sdCard = false; if (SrcDst==2||SrcDst==3) sdCard = true;   // For Source 0,1=Flash 2,3=SDCard 
  c = XLat[MST1];                                                                      // c = Layout 1 3 4 (2 0 0 = A K N)

  switch(b) 
  { case 1: MacroTimerArr1[0] = c; MacroTimerArr1[1] = Option1; MacroTimerArr1[2] = sdCard; MacroTimerArr1[3] = isLink; break;
    case 2: MacroTimerArr2[0] = c; MacroTimerArr2[1] = Option1; MacroTimerArr2[2] = sdCard; MacroTimerArr2[3] = isLink; break;
    case 3: MacroTimerArr3[0] = c; MacroTimerArr3[1] = Option1; MacroTimerArr3[2] = sdCard; MacroTimerArr3[3] = isLink; break;
    case 4: MacroTimerArr4[0] = c; MacroTimerArr4[1] = Option1; MacroTimerArr4[2] = sdCard; MacroTimerArr4[3] = isLink; break;
    case 5: MacroTimerArr5[0] = c; MacroTimerArr5[1] = Option1; MacroTimerArr5[2] = sdCard; MacroTimerArr5[3] = isLink; break;
    case 6: MacroTimerArr6[0] = c; MacroTimerArr6[1] = Option1; MacroTimerArr6[2] = sdCard; MacroTimerArr6[3] = isLink; break;
    case 7: MacroTimerArr7[0] = c; MacroTimerArr7[1] = Option1; MacroTimerArr7[2] = sdCard; MacroTimerArr7[3] = isLink; break;
    case 8: MacroTimerArr8[0] = c; MacroTimerArr8[1] = Option1; MacroTimerArr8[2] = sdCard; MacroTimerArr8[3] = isLink; break; }

    //                        0123456789012345678901
    // char TimerDisp[23] = {"Macro X nn Timer   R-T"};
    // int  XLat1[6]      = {'K','M','A','S','T','k'};
    // char XLat2[24][3]  = {"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24" };
    // char XLat3[8][4]   = {"R-T","R-t","O-T","O-t","R-C","O-C","RcT","OcT" };   
    
  if (c>10) c = c - 10;
  TimerDisp[6]  = XLat1[c]; TimerDisp[17] = a; TimerDisp[8] = XLat2[Option1][0]; TimerDisp[9] = XLat2[Option1][1];
  TimerDisp[19] = XLat3[b-1][0]; TimerDisp[20] = XLat3[b-1][1]; TimerDisp[21] = XLat3[b-1][2];
  status((char *)TimerDisp);
  
  return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ListMacro()    // Source 
                    // Source https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cannot list macros here without a file - for example those loaded using *fm,s,t*
// Because DoFileBytes(NameStr) will not find file NameStr
// Works on macros loaded using serial port - they are saved to a file such as <3This is Key S3>
// For files/macros > 200 bytes size will list the first 10 bytes and show " LF" for Large File before the filename
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ int i, value;
  byte *BPtr;
  byte a, b;
  int m, n;  
  bool sdCard = false;
  
  for (n=0; n<48; n++) Int2HexStr[n] = 0x20; // fill with spaces not NULL
  
  if (KeyBrdByteNum>0&&KeyBrdByteNum<30) { for (n=0; n<KeyBrdByteNum; n++) NameStr1[n] = KeyBrdByte[n]; NameStr1[n] = 0x00; }
      else { DoMSTAName(Option1, MST1); strcpy(NameStr1, MSTAName); } 
  
  if (SrcDst==0||SrcDst==1) sdCard = false; if (SrcDst==2||SrcDst==3) sdCard = true;   // For Source 0,1=Flash 2,3=SDCard 
  
  BPtr = MacroBuff; MacroBuffSize = DoFileBytes(0, NameStr1, BPtr, ByteSize, sdCard); 
  if (MacroBuffSize==0 && !LargeFile) { status("Macro's not found"); return; }         
          
  n = 0; m = 0; a = 10;                                                 // Status only place to display 10 bytes + size + name
  while (n<a) { value = MacroBuff[n];                                   // Contents in hex
                Int2HexStr[n+m]   = digits[(value >> 4) & 0x0F];   
                Int2HexStr[n+m+1] = digits[value & 0x0F]; n++; m=m+3; }  

  m = m + 3; 
  if (!LargeFile) { value = MacroBuffSize;               // Size in hex
                    Int2HexStr[n+m]   = digits[(value >> 4) & 0x0F];   
                    Int2HexStr[n+m+1] = digits[value & 0x0F]; n++; m=m+3; }
             else { Int2HexStr[n+m]   = 'L'; Int2HexStr[n+m+1]   = 'F'; m=m+3; }     // Large files "LF" as size

   b = 0x20; i = 0; m = m + 2;
   while (b!=0) { b = NameStr1[i]; Int2HexStr[n+m+i] = b; i++; }
                
  status((char *)Int2HexStr); // Should print up to first NULL - hex value 00 is not NULL it is "0" or zero

  LargeFile = false;          // reset
  
}

////////////////////////
void MakeStr(int Button)
////////////////////////
{   int i, n;
    byte a, b, c;
    a = 0;
    Fx = false;     // Only true for one MakeStr type keypress at a time   
    
    b = KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][Button][KeyBrd123];  // b is current key label character or modifier
    
    KBrdActive = true; // Let [ADD] know any char key has been pressed at least once before

    if (KBType) {  a = conv_table[b][1]; }   // KBType true if KeyBrdByte[0]>0x7F - if keycode keys must be from 1st key

    if (KeyBrdX==2) {if (Button==10) {a = KeyBrdFx[KeyBrdF1F24]; Fx = true;}  // Xlate F1-F24 - see if (a>0) below where it is used 
                     if (Button==9)  {a = b = KeyBrdBrackets[BracketsNum][0]; BracketsNum++; if (BracketsNum==8) BracketsNum=0; }
                     if (Button==8)  {a = b = KeyBrdSymbols[SymbolsNum][0];   SymbolsNum++;  if (SymbolsNum==17) SymbolsNum=0; } 
                     if (Button==6)  { if (StarNum==StarCodesMax) StarNum = 0; 
                                       CmKey = true; for (n=0; n<4; n++) { b = StarCode[StarNum][n]; KeyBrdByte[n] = b; KBDisp[n] = b; } 
                                       KeyBrdByteNum = 4; KBDispPos = 4; status((char *)KBDisp); delay(10);
                                       StarNum++; return; } }  // Return to same *code if [KbrdKey such as EXE] was pressed
    if (KeyBrdX==3) { switch(Button)
                    {        case 0:  ListMacro();                                  break;  
                             case 1:  RenameMacro(); InitCfg(0);                    break; 
                             case 2:  RemoveMacro(); InitCfg(0);                    break;  
                             case 3:  if (SendMacro()) status("Macro Sent");        break;   
                             case 4:  MacroTimerOK = GetTimerMacro();               break; 
                             case 5:  CopyMacro(0); InitCfg(0);                     break; 
                             case 6:  LinkFileMacro();                              break;  
                             case 8:  DoSourceMacro(); status("Source Macro");      break; 
                             case 9:  DoDestMacro();   status("Destination Macro"); break; 
                             case 10: DoNumMacro();                                 break; 
                             case 11: if (SaveMacro(1)) status("Macro Saved");      break;   // Only used by *fs* *ft* *fm* *fa*                             
                    } return; }
                     
    if (KeyBrdX==4) {KeyBrdByte[KeyBrdByteNum] = KeyBrdNum[KeyBrd123][Button];   }   // Get special code such as ShiftLeft etc
        // for (n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } SerPr2; }
           
    // n = 0;
    if (KeyBrdX<4) 
       {KBDisp[KBDispPos] = b; KBDisp[KBDispPos+1] = ' '; KBDisp[KBDispPos+2] = ' '; 
        if (a>0) KeyBrdByte[KeyBrdByteNum] = a; 
        if (Fx) { for (n=0; n<3; n++) {KBDisp[KBDispPos+n] = KeyBrdFxChr[KeyBrdF1F24][n]; }  KeyBrdF1F24++; if (KeyBrdF1F24>23) KeyBrdF1F24=0; } } // Fx true 1 MakeStr()
        
    if (KeyBrdX==4) for (n=0; n<3; n++) KBDisp[KBDispPos+n] = KeyBrdSpc[KeyBrd123][Button][n];
    
    for (i = 0; i <= KBDispPos+n; i++) {Serial.print(KBDisp[i]);  Serial.print(' '); } SerPr2;
    for (int n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } SerPr2;    

    status((char *)KBDisp);

    if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; // chars per key 0 1 2 repeat
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
bool SaveMacro(bool Notfmst)   // Also used by *sa,m,s,t then all 24 (U/L case) are saved to flash or SDCard
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Saves current Source Macro to Flash/SDCard but with no execution - if already a file overwrite
// Also used to create directory if entered with // in front of directory name:
// For example enter //folder1 then press [Sav]. The directory folder1 has been created check with [Cfg]Sav] via a serial monitor.
// Then with the Macro Editor again enter //folder1 then press [Rmv] - then folder1 will be deleted
// Note that only empty directories can be deleted.
// Can be used to save *Code such as *fo* (bold fonts) to a file such as m,s,t, 1-24 a01-a99 - when pressed will execute *Code
// If data has been entered then that will be saved to the Source macro filename but not copied to macro itself - need keypress to run
// When Key receives data via *fm,s,t* then MacroSizeM1M12[1-12] size = and MacroM1M12[1-12] Status = 3
// When Key receives data from KeyBrd Macro Editor then Size = number of bytes and Status = 1
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ int m, n;        
  byte * BPtr;
  bool doDir = false, mkDir = false, sdCard = false;  
  
  n = 0;
  DoMSTAName(Option1, MST1); 
  if (SrcDst==0||SrcDst==1) sdCard = false; if (SrcDst==2||SrcDst==3) sdCard = true;   // For Source 0,1=Flash 2,3=SDCard 
  
  if (Notfmst) { if (KeyBrdByteNum>3&&KeyBrdByte[0]=='/'&&KeyBrdByte[1]=='/') doDir = 1;  // Serial.println("isDir"); // Two '//' create directory                           
                 if (doDir)  { for (n=0; n<(KeyBrdByteNum-doDir); n++) NameStr1[n] = KeyBrdByte[n+doDir]; NameStr1[n] = 0x00;  
                               if (sdCard)  mkDir  = SDFS.mkdir(NameStr1);     
                               if (!sdCard) mkDir  = LittleFS.mkdir(NameStr1); 
                               if (mkDir) status("Dir created"); else status("Dir not created"); delay(1000); return mkDir; }
                  else if (KeyBrdByteNum>0&&KeyBrdByteNum<MaxBytes)  
                          {BPtr = KeyBrdByte; n = KeyBrdByteNum; DoFileBytes(1, MSTAName, BPtr, n, sdCard); return true;}
               }
  
  if (MST1==0) { for (n=0; n<ByteSize; n++) { if (Mtr1to12[Option1][n]==0x00) break;  } 
                 MacroM1M12[Option1] = 6; BPtr = Mtr1to12[Option1]; MacroSizeM1M12[Option1] = n; }   

  if (MST1==1) { for (n=0; n<ByteSize; n++) { if (Str1to12[Option1][n]==0x00) break; } 
                 MacroS1S12[Option1] = 6; BPtr = Str1to12[Option1]; MacroSizeS1S12[Option1] = n; }    

  if (MST1==2) { for (n=0; n<ByteSize; n++) { if (Ttr1to12[Option1][n]==0x00) break;  } 
                 MacroT1T12[Option1] = 6; BPtr = Ttr1to12[Option1]; MacroSizeT1T12[Option1] = n; }
                        
  DoFileBytes(1, MSTAName, BPtr, n, sdCard);   // Save Source File content already configured in MSTtr1to13[Option1]
 
  if (Notfmst) if (n>1) status ("Saved"); 
  return (n>1);        
}

/////////////////////////////
void MakeMathStr(int Button)
/////////////////////////////
{   int i, n;
    for (i=0; i<4;  i++) {MathHexNum[i] = MathNum[MathX][Math123][Button][i];     // [KeyBrd 0-3=MathX][Symbol 0,1,2 = Math123][button 1-11]
                          MathChr[i] = MathHexNum[i]; }                           // MathNum = 4 hex chars + 0x00 -> use i<4 not i<5                          
    MathChr[i] = Spc;                         
    for (n=0; n<32; n++) {MathChr[n+i+1] = MathName[MathX][Math123][Button][n]; if (MathChr[n+i+1]==0) break; }          
                          
    // for (n=0; n<5; n++) {Serial.print(MathHexNum[n]); Serial.print(' '); } Serial.println(MathChr);   
    status((char *)MathChr);

    if (Math123<2) Math123++; else Math123=0; 
}

////////////////////////
void SendMath()
////////////////////////
{   int n;
    uint8_t keycode[6];       // uint8_t keycode[6] = { 0 }; simultaneous keys pressed in here    
    /////////////////////////////////////////////////////////////////////////////////////////
    // 1D6D1 + Alt + X = small pi works in MS Word and LibreOffice (1D6D1 = 120529)
    // Watch out for auto-correction if the symbol is the first in a sentence/line.
    /////////////////////////////////////////////////////////////////////////////////////////
    for (n=0; n<4; n++) { usb_hid.keyboardPress(HIDKbrd, MathHexNum[n]); delay(dt25);   // MathNum = 4 hex chars + 0x00 -> use i<4 not i<5
                          usb_hid.keyboardRelease(HIDKbrd);              delay(dt25);}
                          
    for (n=0; n<6; n++) keycode[n] = 0x00;                          
    keycode[0] = AltL;
    keycode[1] = 0x1B; // char X from hid.h       
    usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(100);
    usb_hid.keyboardRelease(HIDKbrd);                 delay(dt25);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "\x80 \x81 \x82 \x83 \x84 \x85 \x86 \x87 \x88 \x89 \x8A \x8B \x8C \x8D \x8E \x8F"
// "\x90 \x91 \x92 \x93 \x94 \x95 \x96 \x97 \x98 \x99 \x9A \x9B \x9C \x9D \x9E \x9F"   // umlauts etc
// "\xA0 \xA1 \xA2 \xA3 \xA4 \xA5 \xA6 \xA7 \xA8 \xA9 \xAA \xAB \xAC \xAD \xAE \xAF"   // 
// "\xB0 \xB1 \xB2 \xB3 \xB4 \xB5 \xB6 \xB7 \xB8 \xB9 \xBA \xBB \xBC \xBD \xBE \xBF"   // B0 and B1 blocks line drawing
// "\xC0 \xC1 \xC2 \xC3 \xC4 \xC5 \xC6 \xC7 \xC8 \xC9 \xCA \xCB \xCC \xCD \xCE \xCF"   // only few printing chars
// 0xFF, 0x55, 0xFF, 0x55, 0xFF  replaced with 0xFF, 0xFF, 0xFF, 0xFF, 0xFF in TFT_eSPI glcdfont.c line 187
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TestFonts ()
{ tft.setTextPadding(480); tft.setTextDatum(1); tft.setFreeFont(); tft.setTextSize(2); 
  tft.drawString("\xB0 \xB1 \xB2 \xB3 \xB4 \xB5 \xB6 \xB7 \xB8 \xB9 \xBA \xBB \xBC \xBD \xBE \xBF", STATUS_X, STATUS_Y); 
  tft.setTextSize(KEY_TEXTSIZE); 
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b); else tft.setFreeFont(&FreeSans12pt7b);
}
//////////////////////////////////////////////////////////////////////////
// Sourcerc nn Target nn   D  Timers RrTt CcTt
//////////////////////////////////////////////////////////////////////////
void optionsindicators(int Option) 
{ int a, b, c, op1, op2;
  int ColArr[4][2] = {White, White, White, Orange, Orange, White, Orange, Orange}; // 00 01 10 11
  // Indicate Src/Dest SDCard/Flash=Orange/White SrcDst=0=00=W->W =1=01=W->O =2=10=O->W =3=11=O->O
  //                        01234567890
  char SrcDispStr[12]   = {"Src  M  nnn"};
  char DstDispStr[12]   = {"Dst  M  nnn"};
  char MSTArr[6][2] = {"M","S","T","A","K","N"}; 
  char SDNumChar[]  =  "x" ;  
  
  MSTArr[5][0] = nChar;
  
  tft.setTextPadding(110);                              
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(Yellow, Black); 
  tft.setTextDatum(1);
    
  if (Kbrd) 
     { SrcDispStr[5] = MSTArr[MST1][0]; DstDispStr[5] = MSTArr[MST2][0];
       op1=Option1+1;  a = 48 + op1/100;  b = 48 + (op1/10)-10*(a-48); c = 48 + op1%10;
       SrcDispStr[8] = a; SrcDispStr[9] = b;  SrcDispStr[10] = c;
       op2=Option2+1;  a = 48 + op2/100;  b = 48 + (op2/10)-10*(a-48); c = 48 + op2%10;
       // Serial.print(op2); Serial.print(' ');Serial.print(a,HEX); Serial.print(' ');Serial.print(b,HEX);Serial.print(' ');Serial.println(c,HEX);
       DstDispStr[8] = a; DstDispStr[9] = b;  DstDispStr[10] = c;  
       tft.setTextColor(ColArr[SrcDst][0], Black); tft.drawString(SrcDispStr, MST_X1-56, MST_Y); 
       tft.setTextColor(ColArr[SrcDst][1], Black); tft.drawString(DstDispStr, MST_X1+54, MST_Y);}
  else { tft.setTextPadding(220); tft.drawString(" ", MST_X1, MST_Y); }
  
  MacroTimer18 = MacroTimer8 || MacroTimer7 || MacroTimer6 || MacroTimer5 || Macrotimer4 || MacroTimer3 || Macrotimer2 || MacroTimer1;   
  if (MacroTimer18 && MacroTimerOK) tft.drawString(TimerDisp, MST_X3, MST_Y); 
                               else tft.drawString("  ", MST_X3, MST_Y);

  tft.setTextPadding(18);                // 14 to 16 does not clear "W" OptionOS for next letter L,R - can use "w" or set to 17

  if (SDNum<22) { if (SDCardArr[2]) { SDNumChar[0] = SD2[SDNum]; tft.setTextColor(Cyan, Black); }    // 0,1 Upper/Lowerscase filename 
                               else { SDNumChar[0] = SD1[SDNum]; tft.setTextColor(Yellow, Black); }  // SDNumArr[1] = 0x00
      tft.drawString(SDNumChar, OPT_X1, OPT_Y); }
      
  tft.setTextColor(White, Black);
  tft.drawString(OSChr[OptionOS], OPT_X2, OPT_Y); 
  
  if (KeyBrdDirect&&Kbrd) { tft.setTextColor(Cyan, Black); tft.drawString("D", OPT_X1, OPT_Y); }
  
  LayerADLetter[0] = 65 + LayerAD;
  if (LayerAxD) tft.setTextColor(Orange, Black); else tft.setTextColor(White, Black);  
  tft.drawString(LayerADLetter, OPT_X3, OPT_Y);  

  tft.setTextPadding(10);
  tft.setFreeFont();   // or NULL => GLX Font
  if (!CheckSerial) tft.setTextColor(Red, Black); else tft.setTextColor(Green, Black);
  tft.drawString("\xB1", OPT_X0, OPT_Y+5); // glcdfont.c has 177=B1 75%

  if (Layout!=2) { b = Layout - (Layout==1) - 2*(Layout==3||Layout==4); tft.setTextColor(Bank123Color[Bank123[b]-1], Black);
      tft.drawString("\xB1", OPT_X0, IND_Y+6); } else tft.drawString(" ", 6, IND_Y+6);// glcdfont.c has 177=B1 75%

  if (TimeSet) tft.setTextColor(Green, Black); else tft.setTextColor(Red, Black);
  tft.drawString("\xB1", OPT_X4, OPT_Y+5);  

  if (timerEnable || alarmEnable || MacroTimerOK) tft.setTextColor(Yellow, Black); else tft.setTextColor(Red, Black);
  tft.drawString("\xB1", OPT_X4, IND_Y+6);        
                
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);   
}
 
//////////////////////////////////////////////////////////////////////////////////
// Indicators status Caps-Num-Scroll C N S 
//////////////////////////////////////////////////////////////////////////////////
void indicators() 
{ if (BusyCNS) return;
  tft.setTextPadding(18); tft.setFreeFont(&FreeSansBold12pt7b);  
  tft.setTextColor(Green, Black); 
  tft.setTextDatum(1);
  
  if (CapsLock)    tft.drawString("C", IND_X1, IND_Y);  else tft.drawString(" ", IND_X1, IND_Y);
  if (NumLock)     tft.drawString("N", IND_X2, IND_Y);  else tft.drawString(" ", IND_X2, IND_Y);
  if (ScrollLock)  tft.drawString("S", IND_X3, IND_Y);  else tft.drawString(" ", IND_X3, IND_Y);
  
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);   
}  

////////////////////////
void DoAdminCmd()
////////////////////////
{ uint8_t keycode[6];
  int n; 
  for (n=0; n<6; n++) keycode[n] = 0x00; // NULLed 
  
  keycode[0] = GuiL;                                               // or use HID_KEY_GUI_RIGHT
  keycode[1] = HID_KEY_X;
  usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(dt25); // GUI + X then A admin powershell i normal powershell g Computer Management
  usb_hid.keyboardRelease(HIDKbrd);              delay(dt25); // Confirm with ArrowLeft Enter 
  usb_hid.keyboardPress(HIDKbrd, 'A');           delay(dt25); // Can also use Alt + "Y" too confirm
  usb_hid.keyboardRelease(HIDKbrd);              delay(dt500); // Can also open via Run GUI + R then Ctrl+Shft+Enter then Alt+Y
  keycode[0] = AltL;                                               // Alt + Y on UAC window instead of ArrowLeft+Enter
  keycode[1] = KeyYes;                                             // Y or y - no Enter required
  /////////////////////////////////////////////////////////////////// Alternative
  // keycode[0] = ArrLeft;                                         // Seems <- pressed and held slightly before Enter
  // keycode[1] = KEnt;                                            //
  ///////////////////////////////////////////////////////////////////
  usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(dt25); 
  usb_hid.keyboardRelease(HIDKbrd); 
}
////////////////////////
void DoAdminPowershell()
////////////////////////
{ uint8_t keycode[6];
  char PShellStr[11] = "powershell"; 
  int n; 
  for (n=0; n<6; n++) keycode[n] = 0x00; // NULLed 
  
  keycode[0] = GuiL;             // Admin CMD/Powershell: GUI + R then cmd/powershell then instead of Enter use Ctrl+Shft+Enter then Alt+Y
  keycode[1] = HID_KEY_R;        // 
  usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(dt25);
  usb_hid.keyboardRelease(HIDKbrd);                 delay(dt25);
      
  for (n=0;  n<11; n++)                  // str8 powershell 
      {usb_hid.keyboardPress(HIDKbrd, PShellStr[n]); delay(dt25);
       usb_hid.keyboardRelease(HIDKbrd);             delay(dt25); }
   delay(dt500);
   keycode[0] = CtrL;     // Instead of just [Enter] use [Control + Shift + Enter] => Run as admin
   keycode[1] = ShfL;     // 
   keycode[2] = KEnt;     // 
   usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(dt25);
   usb_hid.keyboardRelease(HIDKbrd);                 delay(dt25);
   delay(dt500);
   keycode[0] = AltL;     // Alt + Y on UAC window instead of ArrowLeft+Enter
   keycode[1] = KeyYes;   // Y or y
   // keycode[2] = KEnt;  // After Y or y - no Enter required
   usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(dt25);
   usb_hid.keyboardRelease(HIDKbrd);                 delay(dt25);  
}

/*////////////////////////////
void MakeKBMacro()
///////////////////////////
{ int n;
  for (n=0; n<100; n++) { KBMacro[n][0] = 'a';  KBMacro[n][1] = ((n+1)/10) + 48; KBMacro[n][2] = n + 49;  KBMacro[n][3] = 0x00; b++;}  
}*/
////////////////////////////
void touch_calibrate()
///////////////////////////
{ LittleFS.begin();

  // DigitalWrite(LCDBackLight, HIGH); // Switch on here incase TFT.init did not - press sequence is TopLeft,BottomLeft,TopRight,BottomRight
    
  if (LittleFS.exists(CalFile))        
     {if ( DoCal)                
         { LittleFS.remove(CalFile);}             // Delete if we want to re-calibrate
      else {File f = LittleFS.open(CalFile, "r"); // check if calibration file exists and size is correct
            if (f) {if (f.readBytes((char *)calData, 10) == 10) calDataOK = 1;  f.close(); }  }  }

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
            if (f) { f.write((const unsigned char *)calData, 10); f.close();  }  }

}

////////////////////////
void showKeyData() 
////////////////////////
{  byte p, n, m, b;
   char c; 
   File f1, f2, f3;
   
   SerPr2;
   Serial.print("Labels M: " );
   if (MLabel) Serial.print("On  Target: "); else Serial.print("Off Target: ");
   if (LayerAxD) f1 = SDFS.open("LabelM", "r"); else f1 = LittleFS.open("LabelM", "r"); 
   n = f1.size(); f1.readBytes(NameStr1, n); f1.close(); Serial.print(NameStr1);
   SerPr2;
   Serial.print("Labels S: " );
   if (SLabel) Serial.print("On  Target: "); else Serial.print("Off Target: ");
   if (LayerAxD) f2 = SDFS.open("LabelS", "r"); else f2 = LittleFS.open("LabelS", "r"); 
   n = f2.size(); f2.readBytes(NameStr2, n); f2.close(); Serial.print(NameStr2);
   SerPr2;
   Serial.print("Labels T: " );
   if (TLabel) Serial.print("On  Target: "); else Serial.print("Off Target: ");
   if (LayerAxD) f3 = SDFS.open("LabelT", "r"); else f3 = LittleFS.open("LabelT", "r"); 
   n = f3.size(); f3.readBytes(NameStr3, n); f3.close(); Serial.print(NameStr3);
   SerPr2;
   
   SerPr2;
   Serial.println("Config1: " );
   for ( m = 0; m<80; m++ ) { Serial.print(Config1[m], HEX); SerPr1; if (m==20||m==40||m==60) SerPr2; }
   SerPr2;

   SerPr2;
   for ( m = 0; m<3; m++ ) 
       { Serial.print("MacroInstructionList " ); Serial.print(m); Serial.print(": ");
         for ( n = 0; n < iListMax; n++) 
             { b =  MacroInstructionList[m][n]; Serial.print(b); SerPr1; }
         SerPr2; }
        
   SerPr2;
   Serial.print(" Buff 20bytes " ); Serial.print(MacroBuffSize); SerPr1;
          for ( n = 0; n <= 20; n++) 
              { b =  MacroBuff[n]; Serial.print(b, HEX); SerPr1; }
          SerPr2; 

   SerPr2;
   Serial.println("K Keys BSD:" ); 
          for ( n = 27; n <= 50; n++) 
              { Serial.print(n);  SerPr1;
                b =  BsDCode1[n]; Serial.print(b, HEX); SerPr1;
                b =  BsDCode2[n]; Serial.print(b, HEX); SerPr1;
                b =  BsDCode3[n]; Serial.print(b, HEX); SerPr1; }
          SerPr2;

   SerPr2;
   Serial.print("Macro Buffer Assigment M S T Keys:" ); 
                b =  Bank123[0];  Serial.print("  M "); Serial.print(b);
                b =  Bank123[1];  Serial.print("  S "); Serial.print(b);
                b =  Bank123[2];  Serial.print("  T "); Serial.print(b); 
          SerPr2;

    SerPr2;
    for ( m = 0; m<24; m++ ) 
         { Serial.print(m); Serial.print(" Mxx 20bytes " ); Serial.print(MacroSizeM1M12[m]); Serial.print(" Mtr Status " ); Serial.print(MacroM1M12[m]); SerPr1;
          for ( n = 0; n < 20; n++) 
              { b =  Mtr1to12[m][n]; Serial.print(b, HEX); SerPr1; }
          SerPr2; }

   SerPr2;
   for ( m = 0; m<24; m++ ) 
         { Serial.print(m); Serial.print(" Sxx 20bytes Size " ); Serial.print(MacroSizeS1S12[m]); Serial.print(" Str Status " ); Serial.print(MacroS1S12[m]); SerPr1;
          for ( n = 0; n < 20; n++) 
              { b =  Str1to12[m][n]; Serial.print(b, HEX); SerPr1; }
          SerPr2; }

   SerPr2;
   for ( m = 0; m<24; m++ ) 
         { Serial.print(m); Serial.print(" Txx 20bytes Size " ); Serial.print(MacroSizeT1T12[m]); Serial.print(" Ttr Status "); Serial.print(MacroT1T12[m]); SerPr1;
          for ( n = 0; n < 20; n++) 
              { b =  Ttr1to12[m][n]; Serial.print(b, HEX); SerPr1; }
         SerPr2;  }        
   
         
 }

/************* EOF line 4578 *****************/
