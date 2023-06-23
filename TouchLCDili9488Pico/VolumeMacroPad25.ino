/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Combination of (1) Raspberry Pi Pico Touch Macro Keyboard by Dustin Watts
//                    https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard
//                (2) AdafruitTinyUSB HID examples hid_composite.ino hid_boot_keyboard.ino
//                    https://github.com/adafruit/Adafruit_TinyUSB_Arduino/
//                (3) Bodmer-Adafruit Keypad Keypad_480x320.ino
//                    https://github.com/Bodmer/TFT_eSPI/examples/
//
// For an inspirational interface refer to the stunningly useful and beautiful Adafruit Touch Desk
// which shares a similar layout approach to what I used here - their design dates back to early 2021.
// https://learn.adafruit.com/touch-deck-diy-tft-customized-control-pad?view=all
//
// Adapted by Tobias van Dyk August 2022 - June 2023 for the ILI9486/ILI9488 480x320 LCD and Adafruit 
// TinyUSB stack and the remarkably versatile and agile human thumb(s).
//////////////////////////////////////////////////////////////////////////////////////////////////////// 
/************************************************************************************
 Adafruit invests time and resources providing this open source code, please support 
 Adafruit and open-source hardware by purchasing products from Adafruit
 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
*************************************************************************************/
#include "Adafruit_TinyUSB.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include <FS.h>
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/rtc.h"
#include "strings24.h"  // S keys S1 - S24 load with *fs*[EXE] save to Flash with *ss*[EXE]
#include "stringt24.h"  // T keys T1 - T24 load with *ft*[EXE] save to Flash with *st*[EXE]
#include "stringRSL.h"  // Reboot or Shutdown or Logout strings for Windows, Linux, RaspiOS
#include "coloursDef.h" // Colours used
#include "keysDef.h"    // Keys used
#include <Adafruit_NeoPixel.h>

// Which pin is connected to the NeoPixels?
#define PIN       4
#define NUMPIXELS 1 
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

byte OptionOS[1] = {0}; // 0 Windows 1 Linux 2 RPiOS 3 MacOS
char OSName[4][18]   = {"Microsoft Windows","GNU/Linux","Raspberry Pi OS","Apple macOS" };
char OSChr [4][2]    = {          "W",          "L",    "R",              "A"           };  

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// rtc.h from Raspberry Pi Pico SDK where Location = C:\Users\Username\AppData\Local\Arduino15\packages\rp2040\
// Location\2.6.4\pico-sdk\src\rp2040\hardware_structs\include\\hardware\rtc.h   \hardware\structs\rtc.h   \hardware\regs\rtc.h
// Problem: min is an Arduino reserved key word uses it as min() (macro) function - but Pico SDK rtc.h use it as minutes .min
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Keys S s01-s24 T t01-t24 M m01-m24 and KBMacro a01-a99 + a00 -> 99 + 3 x 24 = 171 macros that can be chained
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char STRname   [24][4] = { "s01", "s02", "s03", "s04",  "s05",  "s06", "s07", "s08", "s09", "s10", "s11", "s12",
                                        "s13", "s14", "s15", "s16",  "s17",  "s18", "s19", "s20", "s21", "s22", "s23", "s24" }; // Maps to a01-a24
static const char TTRname   [24][4] = { "t01", "t02", "t03", "t04",  "t05",  "t06", "t07", "t08", "t09", "t10", "t11", "t12",
                                        "t13", "t14", "t15", "t16",  "t17",  "t18", "t19", "t20", "t21", "t22", "t23", "t24" }; // Maps to a25-a48
static const char MTRname   [24][4] = { "m01", "m02", "m03", "m04",  "m05",  "m06", "m07", "m08", "m09", "m10", "m11", "m12",  
                                        "m13", "m14", "m15", "m16",  "m17",  "m18", "m19", "m20", "m21", "m22", "m23", "m24" }; // Maps to a49-a72
                                    
static const char KBMacro  [100][4] = 
{ "a01","a02","a03","a04","a05","a06","a07","a08","a09","a10","a11","a12","a13","a14","a15","a16","a17","a18","a19","a20","a21","a22","a23","a24","a25",
  "a26","a27","a28","a29","a30","a31","a32","a33","a34","a35","a36","a37","a38","a39","a40","a41","a42","a43","a44","a45","a46","a47","a48","a49","a50",
  "a51","a52","a53","a54","a55","a56","a57","a58","a59","a60","a61","a62","a63","a64","a65","a66","a67","a68","a69","a70","a71","a72","a73","a74","a75",
  "a76","a77","a78","a79","a80","a81","a82","a83","a84","a85","a86","a87","a88","a89","a90","a91","a92","a93","a94","a95","a96","a97","a98","a99","a00" };

int LCDBackLight = 13; // Will be set as OUTPUT and HIGH by TFT init or Brightness level via analog PWM

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// See the Arduino language specification discussion here: https://github.com/arduino/language/discussions/2
// on multitasking, timing logic and limitations of state machines overcome by threads.
////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
unsigned long NowMillis = 0;           // LCD Backlight Saver
unsigned long LastMillis = 0;          // LCD Backlight Saver
unsigned long TimePeriod = 300000;     // 300 seconds LCD Backlight Timer change with *tb*num=0,1-5 via keybrd 1=30 sec, 0=120 days
unsigned long RepTimePeriod = 500;     // After this key repeat is active
unsigned long RepLast = 0;             // Time when key first pressed
unsigned long RepNow = 0;              // Time when key still pressed
bool BackLightOn = true;               // LCD Backlight Saver
byte VolMuteCount = 1;                 // Can be 0 1 2 4 if !LockMedia 
byte Layout = 1;                       // Layout 1 2 3 4 = M Config+KeyBrds S T
byte LayoutPrev = 1;                   // Save Layout temporarily while using Macro functions
byte LayerAD = 0;                      // Switch [Layout A B C D] for [Layers 1 3 4] A[M1-M6] B[M7-M12] C[M13-M18] D[M19-M24]
char LayerADFile[2] = "0";             // char [0] = 0-3 on file
char LayerADLetter[2] = " ";           // LayerAD = 9-3 = A-D [1] = 65+LayerAD
bool LayerAxD = false;                 // Ignore A -> D use the programmed action toggle by pressing Pad M, S, T on same page M, S, T

int DimVal  = 3;                       // 0 -> Backlight ON/OFF 1-9 dimmed value
int NormVal = 0;                       // Max normal value LCD brightness
const static uint8_t dbArr[10] = {   0,    4,    8,    16,   25,   38,   51,   64,  102,  128 };
const static uint8_t bbArr[10] = {   0,    8,   16,    25,   38,   51,   64,  102,  128,  166 };
const static char DBArr[10][3] = { " 0", " 1", " 3",  " 6", "10", "15", "20", "25", "40", "50"};
const static char BBArr[10][4] = {"100", " 3", " 6",  "10", "15", "20", "25", "40", "50", "65"};

unsigned long timePowerOff = 30;   // seconds                       
unsigned long TimePowerOff = 3000;
unsigned long timeRestart = 30;                          
unsigned long TimeRestart = 3000;

unsigned long timeOnceof = 9000;                          
unsigned long TimeOnceof = 90000;
unsigned long timeRepeat = 9000;                          
unsigned long TimeRepeat = 90000;
unsigned long timeOnceofPrev = 0;                          
unsigned long TimeOnceofPrev = 0;
unsigned long timeRepeatPrev = 0;                          
unsigned long TimeRepeatPrev = 0;

//                    012345678901234567890123456
char BlankArr[]    = "Blank Time          seconds"; 
char DimArr[]      = "LCD Blank Dim        (%)   ";
char NormArr[]     = "LCD Brightness       (%)   ";
char mTArr[]       = "Oneshot Time               ";
char rTArr[]       = "Repeat Time                ";

bool CapsLock = false;                 // Green "C"
bool NumLock = false;                  // Green "N"
bool ScrollLock = false;               // Green "S" 
bool Change = false;                   // Indicators changed
byte ConfigKeyCount = 0;               // Start with Layout 2 Cfg key not pressed 
bool MuteDisable = true;               // Disable VolMute with Layout 1 2 3 4 select Default is L1-L4
bool VolDisable = false;               // Disable Vol+ Vol- then keys are Del/BSpc Enter
bool LockMedia = false;                // Lock-in layout 2 if it is all media keys + escape to Layer 2 if Layer 1 + [Vo]
byte SaveLayout[1] = { 0 };            // 5 possible Values are 0 to 4 for 0=default=layout2(Cfg) or else layout 1 to 4
bool Media = false;                    // Activate 4 of possible 11 Media Keys in Layout 2
bool MouseK = false;                   // 12 Mouse keys active via Pad option           
bool MediaPrev = false;                // Restore 4 Media Keys in Layout 2
bool ToneControls = false;             // Bass Treble on Media Keyboard
bool Brightness = false;               // *br* = brightness up/dwn in M S T repace volume
int8_t MouseScrollAmount = 1;          // Scroll Amount (if - then Down if + Up
int8_t MouseDelta = 5;                 // Mouse Cursor Movement Amount in pixels
byte MouseConfig[2] = { 1, 5 };        // Mouse Config
bool MediaCfg = false;                 // Set later = to MediaConfig[0]
byte MediaConfig[1] = { 0 };           // Values are 0 - 4 for Media+Volume+Mute+Tone combinations 

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

uint16_t t_x = 0, t_y = 0;      // Touch coordinates

/////////////////////////////////////////////////////////////////////////////////////////
// Layout 1 to 4 Keys
/////////////////////////////////////////////////////////////////////////////////////////
int ScrSizeType = 1;        // 1 = 480x320  0 = 320x240; 

// Key sizes and spacing
#define KEY_W 100           // Key width  95 x 4 = 380 + 15 x 4 = 440  
#define KEY_H 80            // Key height 80 x 3 = 240 + 12 x 4 = 288
#define KEY_SPACING_X 9     // 15 X gap
#define KEY_SPACING_Y 10    // Y gap
#define KEY_X 54            // 30 is off-screen on left
#define KEY_Y 50            // 30 is the topmost with no cut-off

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Layout 5 small Config Buttons m k u n o (Math MacroKeybrd Mouse Numpad CapsNumOption) between 1st and 3rd row   // ***Mouse            
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PAD_W 30          //  Key width  PAD_X is middle -> Pad X is from 317 to 303 
#define PAD_H 44          //  Key height
#define PAD_SPACING_Y 53  // 
#define PAD_X 455         //  config buttons right side
#define PAD_Y 34          // config buttons top
////////////////////////////////////////////////////////////////////////////
// Indicators status Caps-Num-Scroll C N S
////////////////////////////////////////////////////////////////////////////
#define IND_X1 107   //  Centred on this
#define IND_X2 216   
#define IND_X3 326   
#define IND_Y  175   
////////////////////////////////////////////////////////////////////////////
// Macro Src NN Dst NN Indicators 
////////////////////////////////////////////////////////////////////////////
#define MST_X1 108  
#define MST_X2 216 
#define MST_X3 324 
#define MST_Y  275 
////////////////////////////////////////////////////////////////////////////
// Options Indicators status D T L/W (Direct Mode, Timer, Linux/Windows)
////////////////////////////////////////////////////////////////////////////
#define OPT_X0 0 
#define OPT_X1 108   
#define OPT_X2 216 
#define OPT_X3 325 
#define OPT_X4 433 
#define OPT_Y  86    
////////////////////////////////////////////////////////////////////////////
// Message status line at bottom
////////////////////////////////////////////////////////////////////////////
#define STATUS_X 220 // Centred on this - not middle buttons are off-centre
#define STATUS_Y 296 // 305 bottom of y p etc cut-off

#define KEY_TEXTSIZE 1   // Font size multiplier

// Delay between keypress to give USB HID time to respond
uint8_t keydelay = 50;        // keycode[1,..,6] delay milliseconds
uint8_t keydelay2 = 25;       // keyPress delay
uint8_t keydelay1 = 200;      // keyPress delay for Logoff Poweroff Restart
uint16_t keydelay3 = 500;     // used in UAC confirm

// Create the screen object
TFT_eSPI tft = TFT_eSPI();

#define ArraySz(x) (sizeof(x)/sizeof((x)[0]))  
#define cSt const static  // line space needed
/////////////////////////////////////////////////////////////////////////////////////////////////
//0 1 2      4   5 6 7   8   9 10 11   12
char keyLabel[12][4] = {""};               //  = {};  = ""; all 0x00  
/////////////////////////////////////////////////////////////////////////////////////////////////
//  If Cfg Key pressed new options (new Labels and new Colours)
//  Labels[0/1][1,5,9,13][0-11][0-3]
//  "Hme", "Up",  "PgU", "Del/V+", "<",   "Cfg", ">",  "V0/L14",  "End", "Dwn", "PgD", "V-/Ret",
//  "1L4", "  ",  "Vol", "      ", "AD",  "Sav", " ",  "      ",  "Med", "   ", "1S4", "      "         
/////////////////////////////////////////////////////////////////////////////////////////////////
const static char CfgLabel[12][4] =                                          
{"1L4", "Var",  "Vol", "   ", "A-D",  "Sav", "mCT",  "   ",  "Med", "ROf", "1S4", "   "};  // All 3 chars + \0  

byte Bank123[3] = {0,1,2}; // Save ActionM ActionS ActionT in here [0]*am*n [1*as*n [2]*at*n with n=1-5 1-3 sets of 24 macro actions or 4 Run or 5 Cmd
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                       0    1    2    3    4    5     6    7    8    9    10   11    12   13   14   15   16   17    18   19   29   21   22   23
cSt byte PShop1[24]  = { CtrL,CtrL,CtrL,CtrL,CtrL,CtrL, CtrL,CtrL,CtrL,CtrL,CtrL,CtrL, CtrL,CtrL,CtrL,CtrL,CtrL,CtrL, CtrL,CtrL,CtrL,CtrL,CtrL,CtrL };  
cSt byte PShop2[24]  = { 0x2E,0x2D,0x27,ShfL,ShfL,0x1A, AltL,ShfL,ShfL,ShfL,ShfL,AltL, 0x11,0x1A,0x17,0x4A,0x07,ShfL, AltL,0x56,AltL,AltL,AltL,AltL }; 
cSt byte PShop3[24]  = { 0x00,0x00,0x00,0x11,0x16,0x00, ShfL,0x11,0x08,0x07,0x0C,0x04, 0x00,0x00,0x00,0x00,0x00,0x16, 0x56,0x00,0x15,0x17,0x37,0x06 }; 
//                       Notepad Shortcuts              Photoshop Shortcuts            Firefox Shortcuts              MSWord Special Characters
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                        0    1    2    3    4    5     6    7    8    9    10   11    12   13   14   15   16   17    18   19   29   21   22   23
cSt byte Windows1[24] = { CtrL,GuiL,GuiL,GuiL,GuiL,GuiL, GuiL,GuiL,GuiL,AltL,ShfL,CtrL, CtrL,AltL,GuiL,GuiL, GuiL,GuiL,GuiL,GuiL,GuiL,GuiL,GuiL,GuiL };  
cSt byte Windows2[24] = { ShfL,KeyX,KeyR,KPse,Key1,Key2, Key3,Tab, CtrL,PScr,F10, AltL, ShfL,KEnt,CtrL,Key9, Key6,Key7,Key8,Key0,KEqu,F01, Key4,Key5 }; 
cSt byte Windows3[24] = { Esc, 0x00,0x00,0x00,0x00,0x00, 0x16,0x00,ShfL,0x00,0x00,DelK, KeyN,0x00,KeyO,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }; 
cSt byte Windows4[24] = { 0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,KeyB,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }; 
//  Windows Shortcuts     TskMan StrtRC Run PCInfo Calc PwerSh FMan TskView WakePC APrtScr MseRClk CtrAltDel NewFldr Prop OSK GUI9 6 7 8 0 = F1 4 5      
//                        GUI 4 Restart 5 Shutdown 6 Edge 7 Outlook 8 Word 9 Firefox 0 Minimize = Magnifier F1 Help 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Control panel tool Command: Accessibility Options control access.cpl Add New Hardware control sysdm.cpl add new hardware Add/Remove Programs control appwiz.cpl 
// Date/Time Properties control timedate.cpl Display Properties control desk.cpl FindFast control findfast.cpl Fonts Folder control fonts Internet control inetcpl.cpl
// Joystick Properties control joy.cpl Keyboard Properties control main.cpl keyboard Messaging control mlcfg32.cpl Post Office control wgpocpl.cpl 
// Modem Properties control modem.cpl Mouse Properties control main.cpl Multimedia Properties control mmsys.cpl Network Properties control netcpl.cpl 
// NOTE: In Windows NT 4.0, Network properties is Ncpa.cpl, not Netcpl.cpl Password Properties control password.cpl PC Card control main.cpl
// Power control powercfg.cpl Printers Folder control printers Regional Settings control intl.cpl Scanners and Cameras control sticpl.cpl 
// Sound Properties control mmsys.cpl sounds System Properties control sysdm.cpl
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cSt char RunCode[24][60] = 
{"netplwiz","cmd","winver","msconfig","sndvol","msinfo32",
 "lusrmgr.msc","dxdiag","powershell","sndvol","SystemPropertiesPerformance.exe","rundll32.exe keymgr.dll KRShowKeyMgr",
 "msinfo32","regedit","control userpasswords2","mmc","control printers","printui.exe /s",
 "control panel","rundll32.exe shell32.dll,Control_RunDLL appwiz.cpl","control Ncpa.cpl","control inetcpl.cpl users","control main.cpl","control main.cpl keyboard" };
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Other CMD (not Powershell): assoc netstat -an powercfg /a  tasklist schtasks systeminfo  nslookup getmac gpresult /r  whoami  set  route print  
 // diskpart  net user  arp  hostname  rsop   w32tm /query /computer: /configuration    ver  -  Use & or && to chain commands
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cSt char CmdCode[24][50] = 
{"Get-Process","Get-Service","Get-History","Get-ExecutionPolicy","Get-Command","Get-Help",
 "ipconfig /all","ipconfig /Displaydns","ipconfig /flushdns","ipconfig /registerdns","ipconfig /release","ipconfig /renew",
 "start-process PowerShell -verb runas","Get-Location","Get-PSDrive","Get-EventLog -List","Get-EventLog -LogName System -EntryType Error","Clear-History",
 "Clear-History -Command *help*, *command","Get-WinEvent -ListLog *","Get-WinEvent -LogName System","Get-Clipboard","Get-Date","Get-Random" };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                           
static const int BSDMax = 23;     // Number of choices for actions
// #define BSDMax 24;             // Number of choices for actions
int BsDNum = 0 ;                  // Index into BsDLabel[0-3]
char BsDNumFile[2] = "0";         // char [0] = 0-8 on file = BsDNum ASCII - 48
//                         0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23
cSt char BsDLabel[24][4]={"Del","Bks","Tab","aTb","Ins","Esc","PrS","aPr","Ret","Snp", "Osk","Num","Cap","Scr","Cut","Cpy","Pst","Tsk","Run","wX","Pws","FFx","Edg","Mnm"};   
cSt byte BsDCode1[24]   ={ DelK, BckS, Tab,  AltL, Ins,  Esc,  PScr, AltL, KEnt, GuiL, GuiL, 0x53, 0x39, 0x47, CtrL, CtrL, CtrL, CtrL, GuiL, GuiL, GuiL, GuiL, GuiL, GuiL};  
cSt byte BsDCode2[24]   ={ 0x00, 0x00, 0x00, Tab,  0x00, 0x00, 0x00, PScr, 0x00, ShfL, CtrL, 0x00, 0x00, 0x00, KeyX, KeyC, KeyV, ShfL, KeyR, KeyX, Key2, Key9, Key6, Key0}; 
cSt byte BsDCode3[24]   ={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, KeyS, KeyO, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, Esc,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 

// Use same BSDCode1 - BSDCode3 XNum[0,1,2] x1,x2,x3 for Layout1, XNum[3,4,5] x,4,x5,x6 for Layouts 3, and 4 -> use *x1*1-BSDMax to *x6*1-BSDMax
// XKeys active replacing Cut Copy Paste in Layouts M S T 
static const char XLabel [6][4] = { "x 1", "x 2", "x 3", "x 4", "x 5", "x 6" };  // Maps 3x to a72-a90 - not yet programmed to use replace x1-x6 options
byte XNum[6] = { 14,15,16,14,15,16 };                                            // Index into BSDCode1 - BSDCode3 XNum[6] = { 14,15,16,14,15,16 }; 
char XFileNum[2] = "0";                                                          // char [0] = 0-5 on file = XNumMST ASCII - 48  
bool XFiles = false;                                                             // static const bool XFiles = true;
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
// keyLabel[x][]     0             2                4               6                                            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char NumkeysX[12][4] =  { 0 };
const static char Numkeys1[12][4] = 
{"BsD", "7",  "8",   "9",   "Ret",  "4",  "5",  "6",  "0",  "1",  "2", "3"}; 
const static char Numkeys2[12][4] = 
{"BsD", " ",  "%",   "xX",  "Ret",  ",",  ".",  "=",  "+",  "-",  "*", "/"}; 
const static char Numkeys3[12][4] = 
{"BsD", "aA", "bB",  "cC",  "Ret",  "dD", "eE", "fF", "(",  ")",  "[", "]"}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ***Mouse
//  If Mouse Keys selected via KeyBrd command *mu* new options:
//                        "Hme", "Up",  "PgU", "DelV+", "<",   "Cfg",  ">",   "V0L14", "End", "Dwn", "PgD", "V-Ret"
// MouseKeysLabel[x][]    "LBt", "MUp", "RBt", "Sc+",   "<-M", "MBt",  "M->", "L2x",   "Bck", "MDn", "Fwd", "Sc-"                                           
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                
const static char MouseKeysLabel[12][4] =             
{"LBt", "Up", "RBt", "Sc+",   "<", "MBt",  ">", "L2x",  "Bck", "Dwn", "Fwd", "Sc-"}; 
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
const static int StarCodesMax = 56; // StarCodes Count 15+15+15+11
const static char StarCode[StarCodesMax][5] =    
{ "*am*", "*as*", "*at*", "*bb*", "*br*", "*ca*", "*cm*", "*ct*", "*db*", "*de*", "*e0*", "*e1*", "*e2*", "*e3*", "*e4*", 
  "*fa*", "*fc*", "*fm*", "*fo*", "*fs*", "*ft*", "*kb*", "*ks*", "*ld*", "*lr*", "*m1*", "*m2*", "*mt*", "*mT*", "*nt*", 
  "*nT*", "*os*", "*ot*", "*oT*", "*po*", "*rt*", "*rT*", "*sa*", "*se*", "*sm*", "*ss*", "*st*", "*ta*", "*tb*", "*tp*", 
  "*tt*", "*tw*", "*ua*", "*ul*", "*0R*", "*x1*", "*x2*", "*x3*", "*x4*", "*x5*", "*x6*" };
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
cSt int statusSz = 32;                                                   // Character space in statusbar
cSt char statusBlank[statusSz + 1]  = "                                ";  // To clear statusbar
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
const static char MathName[4][3][12][statusSz] = 
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
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 5 Small Config Buttons between 1 st and 3rd row Red Blue Green SkyBlue Gold
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char padLabel[20][2] = {"a", "m", "s", "t", "d", "m", "k", "u", "n", "o", "a", "m", "s", "t", "d", "a", "m", "s", "t", "d"}; 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NumPad Resistor Colour Code // https://eepower.com/resistor-guide/resistor-standards-and-codes/resistor-color-code/#
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte     RXlat[12]   = {0,     7,     8,   9,       0,       4,      5,    6,      0,      1,      2,    3      };  // NumPad keys
//       RColour[12] = {Black, Brown1,Red, Orange,  Yellow4, DGreen,  Blue, Purple, Black1, LGrey, Gold, Silver };  // 4 band - 3 band colour
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
int  XLat1[6]      = {'A','M','x','S','T','X'};
char XLat2[12][3]  = {"01","02","03","04","05","06","07","08","09","10","11","12" };
char XLat3[8][4]   = {"R-T","R-t","O-T","O-t","R-C","O-C","RcT","OcT" };

bool MacroTimerOK = false;                                          
                                                                                    
const static long unsigned int tbArray[10] = {600000000, 30000,   60000,   90000,    120000, 180000, 300000, 600000, 1800000, 3600000 };
const static char tbArrayStr[10][10]       = {"170 hrs","30 sec","60 sec","90 sec", "2 min","3 min","5 min","10 min","30 min","1 hrs" };
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
int  KeyBrdX = 0;                // values 0 1 2 3 4 - Should start on Page 4 or KeyBrdX = 3 - Macro Tools Page
byte KeyBrdByte[200]= ""; // ={};// Hold values to be sent
byte KeyCode[200];               // Parallel keycodes
int  KeyBrd123 = 0;              // Such as Key [abc] = a or b or c - values 0,1,2
int  KeyBrdF1F24 = 0;            // Single key cycles through all 24 Fnn keys
int  KeyBrdByteNum = 0;          // Size KeyBrdByte
int  KPNum = 0;                  // KeyPad Numbers 0,1 - 9 = 0x62,0x59 - 0x61 
int  BracketsNum = 0;            // Single key cycles through all 8 Brackets symbols
int  SymbolsNum = 0;             // Single key cycles through all 14 symbols
int  StarNum = 0;                // Single key cycles through all Starcodes such as *ca*
int  Numkeys123 = 0;             // Numkeys1 to Numkeys3 - 3 chars for each key such as 3 4 5 

int  PrevButton = 0;             // Used in case-button to check for double key press
char KBDisp[200]= "";   //  ={}; // Keybrd display macro buffer in status line
int  KBDispPos = 0;              // nect char position
bool KBType = false;             // If 1st byte>0x7F use keycode[0-5] else use keyPress
bool KeyRepeat = false;          // If button same a sprevious button pressed
bool KBrdActive = false;         // Let [ADD] know any char key has been pressed at least once before
bool KeyBrdDirect = false;       // Echo Keyboard keys to PC as they are typed
bool DoUpKey = false;            // Set true by SendBytes to let Upkey know valid macro can be saved
bool KeySkip = true;             // Skip first key pressed in LCD in dimmed state - used to wake LCD - toggle with *ks*

int  MacroM1M12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0, 
                        0,0,0,0,0,0,0,0,0,0,0,0};    // 0 = not assigned 1 = assigned 2-1000 = repeat count all key macros
int  MacroS1S12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0, 
                        0,0,0,0,0,0,0,0,0,0,0,0};    // Must set repeatcount for whole key i.e. all linked macros in key repeat
int  MacroT1T12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0, 
                        0,0,0,0,0,0,0,0,0,0,0,0};    // set with star code *rm*01-12rr *rs*01-12rr *rm*01-12rr rr = repeat 1-99

int  MacroSizeM1M12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0, 
                            0,0,0,0,0,0,0,0,0,0,0,0 }; // Byte size of macro of assigned + saved macro
int  MacroSizeS1S12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0, 
                            0,0,0,0,0,0,0,0,0,0,0,0 }; 
int  MacroSizeT1T12[24] = { 0,0,0,0,0,0,0,0,0,0,0,0, 
                            0,0,0,0,0,0,0,0,0,0,0,0 };

byte  LinkM[24] = { 0,0,0,0,0,0,0,0,0,0,0,0, 
                    0,0,0,0,0,0,0,0,0,0,0,0 };          // 0 - 9 Number of linked macro's for each Key 1-12
byte  LinkS[24] = { 0,0,0,0,0,0,0,0,0,0,0,0, 
                    0,0,0,0,0,0,0,0,0,0,0,0 };
byte  LinkT[24] = { 0,0,0,0,0,0,0,0,0,0,0,0, 
                    0,0,0,0,0,0,0,0,0,0,0,0 };

bool  SaveReadLinks = true;                            // set via *lr* - later default true

#define mc10  "a00","a00","a00","a00","a00","a00","a00","a00","a00","a00"
// [24] keys for each M S T [10] linked macrofilenames length 4 chars  0x00 Linked filenames a01-a99 
// Size = 24x10x4 = 960 bytes - Note that M,S,T filenames require 4 bytes for s t m+nn+0x00
char LinkNameM[24][10][4] = {mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10};  
char LinkNameS[24][10][4] = {mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10};   
char LinkNameT[24][10][4] = {mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10,mc10}; 

char TimerName[8][4] = {"a00","a00","a00","a00","a00","a00","a00","a00"};    

bool NumKeys = false;            // Numeric Keyboard
bool PowerKeys = false;          // Bottom row on Cfg Layout 3 Logoff Off Restart
bool PowerKeysMenu = false;      // If true use GUI+X+u,i,r instead of GUI+R+shutdown - change with *po*
int  PowerClock = 0;              // 1 or 2 = Restart on PowerOff on Clock Time callback for Time triggered
bool MacroTimerDisp = false;     // Display Timer Keys if ON 
bool MacroTimer18 = false;       // One of the Timers1-4 has been set Once-of or Repeat Macro by time or by clock (need PicoW?)
bool MacroTimer1  = false;       // Repeat Time
bool Macrotimer2  = false;       // Repeat time
bool MacroTimer3  = false;       // Onceof Time
bool Macrotimer4  = false;       // Onceof time
bool MacroTimer5  = false;       // Repeat Clock Time
bool MacroTimer6  = false;       // Onceof Clock time
bool MacroTimer7  = false;       // Repeat Countdown Clock Time
bool MacroTimer8  = false;       // Onceof Countdown Clock time
int  MacroRepeat  = 1;           // Repeat Onceof x times
byte MacroTimerArr1[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr2[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr3[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr4[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr5[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr6[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr7[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr8[2] = {0,0};  // Layout = 1,3,4 Key 1-12
bool PadKeys = false;            // 5 Small Config Buttons on RH side Red Blue Green
bool KeyFontBold = false;        // Button Font Bold/Normal labels
bool KeyFontColour = false;      // FontColour is White/Black
int  KeyFont = White;            // Colour White or Black
int  Option1 = 0;                // Source Macros number 0-11 and 0-99
int  Option2 = 0;                // Destination Macros number 0-11 and 0-99
int  MST1 = 4;                   // Source Macro MST1 = 0 1 2 3 4  = M S T mAcro 4 = not defined keys - macro copy + link
int  MST2 = 4;                   // Destination Macro MST2 = 0 1 2 3 4  = M S T mAcro 4 = not defined keys - macro copy + link
int  NumMode = 0;                // Used to indicate that [Num] key should define Source (1) or Destination (2) macro

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

const int  StrSize =  200;     // Check if not byte used if mafe larger 
const int  ByteSize = 200;
const byte MaxBytes = StrSize;

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

byte * Str1to12[] = {Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,Str11,Str12,
                     Str13,Str14,Str15,Str16,Str17,Str18,Str19,Str20,Str21,Str22,Str23,Str24} ;   
byte * Ttr1to12[] = {Ttr1,Ttr2,Ttr3,Ttr4,Ttr5,Ttr6,Ttr7,Ttr8,Ttr9,Ttr10,Ttr11,Ttr12,
                     Ttr13,Ttr14,Ttr15,Ttr16,Ttr17,Ttr18,Ttr19,Ttr20,Ttr21,Ttr22,Ttr23,Ttr24} ;
byte * Mtr1to12[] = {Mtr1,Mtr2,Mtr3,Mtr4,Mtr5,Mtr6,Mtr7,Mtr8,Mtr9,Mtr10,Mtr11,Mtr12,
                     Mtr13,Mtr14,Mtr15,Mtr16,Mtr17,Mtr18,Mtr19,Mtr20,Mtr21,Mtr22,Mtr23,Mtr24} ;

char inputString[StrSize] = "Send new text to serial port start with <x end with >\n where x = key number 1 to 6 or 0 for [M4]\n";  // String [M4] Key

bool StrOK = false;          // String is complete - ended with 0x0A
bool ByteOK = false;         // Bytes received is complete - ended with 0x00
int  RecBytesLen = 0;        // Number of chars in RecBytes
byte RecBytes[MaxBytes];     // Raw bytes received must start < and end with > - can have length byte/int after < to include <> in data
byte NumBytes = 0;
bool NewData = false;
char AltNum[] = "1D6D1";
bool CheckSerial = false;    // For testing purpose switch off serial check unless *se* toggles CheckSerial but it is not saved default is off

const int NumButtons = 17;             // Total number of buttons = 12 keys (3 rows x 4 columns) + 5 pads on right
TFT_eSPI_Button key[NumButtons];       // Create 12 keys + 5 config buttons for keypad
bool pressed = false;                  // Detect touch

bool sSens = false;                    // PC sensor value sent from HWInfo
char sSArr[2][21]  = { "CPU    C Fan     rpm", "Sys    C Fan     rpm" };  

bool mPlay = false;                    // PC Playing Music
static const int mPlaySize = 96;       // Must check this
char mPlayArr[mPlaySize]  = { " " };    

bool tTimeDate = false;                    // Show Date and Time
static const int tTimeDateSize = 48;       // Must check this Tuesday, May 16, 2023 11:27:51 AM
char tTimeDateArr[mPlaySize]  = { " " };                
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RTC knows how many days are in each month - https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
// RTC does not check that the programmed values are in range. No built-in calendar function. 
// RTC will not compute the correct day of the week it will only increment existing value - wraps through 0 to 6 whenever it reaches midnight.
// If the current value of YEAR in SETUP_0 is evenly divisible by 4, a leap year is detected, and 28 Feb is followed by 29 Feb instead 1 March. 
// Since this is not always true (century years for example), the leap year checking can be forced off by setting CTRL.FORCE_NOTLEAPYEAR.
// int day_of_week(int y, int m, int d) { return (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7; }
// RTC uses 0=Sunday, 1=Monday, 6=Saturday and the above returns that.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char datetime_buf[256];
char *datetime_str = &datetime_buf[0]; 
char alarm_buf[256];
char *alarm_str = &alarm_buf[0]; 
char power_buf[256];
char *power_str = &power_buf[0]; 
char timer_buf[256];
char *timer_str = &timer_buf[0]; 
// For rtc alarm set field = -1 then alarm ignore this - for example can then set alarm every 60 minutes by setting all other fileds -1 and min=00-59
// 0 is Sunday, so 5 is Friday - all byte8 length except year which is int16
datetime_t t     = { .year  = 2022, .month = 11, .day   = 03, .dotw  = 4, .hour  = 14,  .min   = 00, .sec   = 00 };  // Clock Date and Time
datetime_t alarm = { .year  = 2022, .month = 11, .day   = 04, .dotw  = 5, .hour  = 15,  .min   = 20, .sec   = 05 };  // Macro Clock Time Repeat+Oneshot
datetime_t power = { .year  = 2022, .month = 11, .day   = 05, .dotw  = 6, .hour  = 16,  .min   = 20, .sec   = 05 };  // Restart-Off Clock Timer
datetime_t timer = { .year  = 2022, .month = 11, .day   = 06, .dotw  = 0, .hour  = 17,  .min   = 20, .sec   = 05 };  // Macro Clock Timer Repeat+Oneshot
static volatile bool alarm_fired = false;  // Set in callback triggers Macro Send xOne xMany based on Clock Time
static volatile bool power_fired = false;  // Set in callback triggers Restart or PowerOff based on Clock Time
static volatile bool timer_fired = false;  // Set in callback triggers Restart or PowerOff based on Clock Timer
bool TimeSet = false;         // true if Clock Time has been set for example <tyymmddwhhmm> through serial port

///////////////////////////////////////
// Setup
///////////////////////////////////////
void setup() 
{ rtc_init();            // Can check times-date set via serial, *tx*, or powershell, using *ct* [EXE] which displays all 4 times
  rtc_set_datetime(&t);  // Use values above lese comms serial <tyymmddwhhmm> 221103w1200 12:00am 3 Nov 2022 w =0 Sunday 6 = Saturday   

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(50);
  
  Serial.begin(115200);  // shold use Serial.flush() with Adafruit TinyUSB after serial print
  Serial.println();      // print an empty line returns number of chars printed
  //Serial.flush();        // Waits for send buffer to complete
  
  //if (!LittleFS.begin()) {LittleFS.format(); LittleFS.begin(); }   
  LittleFS.begin(); // LittleFs automatically format the filesystem if one is not detected anyway
  
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));  
  usb_hid.setStringDescriptor("TinyUSB HID Composite");
  usb_hid.setReportCallback(NULL, hid_report_callback); // Activate report CapsScrollNum Lock indicators
  usb_hid.begin();                                      // Assume it does -> tusb_init();
  
  while( !TinyUSBDevice.mounted() ) delay(1);
    
  tft.init();
  if (Rotate180) tft.setRotation(1);
            else tft.setRotation(3);    // Set the display orientation to 1 or 3 (180 degrees different)
  touch_calibrate();                    // New Calibration Data obtained if DoCal = true
  
  tft.fillScreen(Black); 
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);       
  tft.setTextSize(KEY_TEXTSIZE);
  tft.setTouch(calData);   // void setTouch(uint16_t *data); in  TFT_eSPI/Extensions/Touch.h 
  
  BackLightOn = true;      // TFT init will turn it on
  
  if (NormVal==0) digitalWrite(LCDBackLight, HIGH);    // Backlight Full ON
            else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON
  
  InitCfg(1);              // Setup config values and strings/macros
  
  if (SaveLayout[0]>0) Layout = SaveLayout[0]; else Layout = 2;  // if SaveLayout > 0 Layout 1 to 4 else default Layout 2 (Cfg)
  VolMuteCount = Layout;                                         // Layout change - for on 1st start
  ConfigKeyCount = 0;                                            // Start up
  ConfigButtons(0);                                              // Draw Buttons and Labels 0 = All 3+5 rows

  NumKeysChange();                              // NumkeysX = Layout 0

  LastMillis = millis();                        // Initial start time
  NewData = KBrdActive = Kbrd = false;
  KeyBrdByteNum = KBDispPos = 0;                // Toggle On/Off with yellow pad in Kbrd mode
  KeyBrdX = 3;                                  // Start on KeyBrd Page 3 - Macro Tools Page
  KeyBrdDirect = PadKeys = false;               // 5 Small Pads RH side Config Page = Red SkyBlue Yellow Grey Green 
  KeyBrd123 = Numkeys123 = 0; 
  optionsindicators(0);
}

/////////////////////////////
// Main Loop
/////////////////////////////
void loop() 
{ if (MacroTimer18) CheckMacroTimers();   // Check Macro Timers 1-8 Oneshot Repeating Clocktime
  
  if (power_fired) { power_fired = false; if (PowerClock==1) { DoPowerKeys('r', PowerKeysMenu, 8);  }
                                          if (PowerClock==2) { DoPowerKeys('u', PowerKeysMenu, 10); } PowerClock = 0; }
                     
  NowMillis = millis();   // get the current "time" (number of milliseconds since started)
  if ((NowMillis - LastMillis) >= TimePeriod)            //test whether the period has elapsed
     {if (DimVal==0) digitalWrite(LCDBackLight, LOW);    // Backlight Off
               else  analogWrite(LCDBackLight, DimVal);  // Backlight Dimmed
      LastMillis = NowMillis;                            // Start Timer again
      RepLast = RepNow = NowMillis;                      // Reset repeat key timer
      if (!MuteDisable) VolMuteCount = 0;                // VolMuteKey pressed Count = 0 if not L1-L4 keys active
      if (!Kbrd) status("");                             // Clear the status line if KeyBrd not active
      BackLightOn = false;   }                           // Until keypress    
    
  pressed = tft.getTouch(&t_x, &t_y, 450);                                     // True if valid key pressed
  if (TinyUSBDevice.suspended() && (pressed)) {TinyUSBDevice.remoteWakeup(); } // Wake up host if in suspend mode + REMOTE_WAKEUP feature enabled by host
  for (uint8_t b = 0; b < NumButtons; b++) 
      { if (pressed && key[b].contains(t_x, t_y)) key[b].press(true); else key[b].press(false); }
     
  RepNow = millis();                                                                                   // get the current time
  for (uint8_t b = 0; b < NumButtons; b++) 
      { if (key[b].justReleased()) {key[b].drawButton(false); RepLast = RepNow; }                      // draw normal - code at release time
        if (key[b].justPressed())  {key[b].drawButton(true); { buttonpress(b); RepLast = millis(); }}  // draw invert - code at press time  
            else if (((RepNow - RepLast) >= RepTimePeriod) && (key[b].isPressed()))                    // code while the button is held
                      {buttonpress(b); }    }                                                          // Do button number
                    
  if (CheckSerial) RecSerial();  // For testing purpose switch off serial check unless *se* toggles CheckSerial but it is not saved default is off
  // if (NewData) showRecData();
  if (NewData) DoNewData();      // First char 0,1,2,3,4,5,6 store file, t, a, p clock alarm timer data, 7 to 9 non-ASCII data   
  
} // main Loop

/////////////////////////////////////////////////////////////////////////////////////
// Check Macro Timers // TimerName[0-8] = Timer type Repeat T Repeat t Once T Once t
// Timers 1 - 4 are incremented by millis() count Timers 5 - 8 are 0/1 Off/On=Active
/////////////////////////////////////////////////////////////////////////////////////
void CheckMacroTimers()
{ unsigned long TimeNow = millis(); 

  LayoutPrev = Layout;
  
  if (MacroTimer1) { if ((TimeNow - TimeRepeatPrev) >= TimeRepeat)                                  // Repeating MacroTimer1 still ON
                        { TimeRepeatPrev = TimeNow; Layout = MacroTimerArr1[0]; MacroKeys(MacroTimerArr1[1], 0); optionsindicators(0);} }
                   
  if (Macrotimer2) { if ((TimeNow - timeRepeatPrev) >= timeRepeat)                                  // Repeating MacroTimer2 still ON 
                        { timeRepeatPrev = TimeNow; Layout = MacroTimerArr2[0]; MacroKeys(MacroTimerArr2[1], 0); optionsindicators(0);} }
    
  if (MacroTimer3) { if ((TimeNow - TimeOnceofPrev) >= TimeOnceof)                                  // Oneshot MacroTimer3 OFF
                        { MacroTimer3 = false; Layout = MacroTimerArr3[0];      MacroKeys(MacroTimerArr3[1], 0); optionsindicators(0);} }
                     
  if (Macrotimer4) { if ((TimeNow - timeOnceofPrev) >= timeOnceof)                                  // Oneshot MacroTimer4 OFF
                        { Macrotimer4 = false; Layout = MacroTimerArr4[0];      MacroKeys(MacroTimerArr4[1], 0); optionsindicators(0);} }

  if (alarm_fired) { if (MacroTimer5) { Layout = MacroTimerArr5[0]; MacroKeys(MacroTimerArr5[1], 0); }  // O-C s 10 and R-C s 8 
                     if (MacroTimer6) { Layout = MacroTimerArr6[0]; MacroKeys(MacroTimerArr6[1], 0); MacroTimer6 = false; }
                     alarm_fired = false; optionsindicators(0);}   // Repeating Clock MacroTimer5 ON - Oneshot Clock MacroTimer6 OFF
                     
  if (timer_fired) { if (MacroTimer7) { Layout = MacroTimerArr7[0]; MacroKeys(MacroTimerArr7[1], 0); }  //  RcT s 3 and OcT s 7 
                     if (MacroTimer8) { Layout = MacroTimerArr8[0]; MacroKeys(MacroTimerArr8[1], 0); MacroTimer8 = false; }
                     timer_fired = false; optionsindicators(0);}   // Repeating Clock MacroTimer7 ON - Oneshot Clock MacroTimer8 OFF
                               
  MacroTimer18 = MacroTimer8 || MacroTimer7 || MacroTimer6 || MacroTimer5 || Macrotimer4 || MacroTimer3 || Macrotimer2 || MacroTimer1; 
  
  if (Layout!=LayoutPrev) { Layout = LayoutPrev; }
  
  // optionsindicators();  // Causes T flicker if put here  
}
///////////////////////////////////////////////////////////////////////////////
void GetTimeData(datetime_t *a)
///////////////////////////////////////////////////////////////////////////////
// Note min is an Arduino reserved key word uses it as min() (macro) function  
// Pico SDK rtc.h use it as minutes .min
///////////////////////////////////////////////////////////////////////////////
{ if (RecBytes[1]==0x2d)  a->year  = -1; else { a->year  = 2000 + (RecBytes[1]-48)*10 + (RecBytes[2]-48);  }
  if (RecBytes[3]==0x2d)  a->month = -1; else { a->month = (RecBytes[3]-48)*10 + (RecBytes[4]-48); }
  if (RecBytes[5]==0x2d)  a->day   = -1; else { a->day   = (RecBytes[5]-48)*10 + (RecBytes[6]-48); }
  if (RecBytes[7]==0x2d)  a->dotw  = -1; else { a->dotw  = (RecBytes[7]-48); }
  if (RecBytes[8]==0x2d)  a->hour  = -1; else { a->hour  = (RecBytes[8]-48)*10 + (RecBytes[9]-48); }       
  if (RecBytes[10]==0x2d) a->min   = -1; else { a->min   = (RecBytes[10]-48)*10 + (RecBytes[11]-48); } 
  a->sec = 0;   
}

/////////////////////////////////////////////////////////////////////////////
void WriteSensorData()                   // PC sensor value sent from HWInfo
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
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Date Time <TTuesday, May 16, 2023 11:27:51 AM>   max = 48 chararacters 
///////////////////////////////////////////////////////////////////////////// 
{ int i; 
  if (NumBytes>tTimeDateSize) { status("Date Time Data too long..."); return; }               
  for (i=0;  i<NumBytes;  i++) tTimeDateArr[i] = RecBytes[i+1];    // Skip "T"               
  status(tTimeDateArr); 
  tTimeDate = false; 
}

/////////////////////////////////////////////
// Check for new character strings or macros
/////////////////////////////////////////////
void DoNewData()
{ int n = 0;
  char *ChrPtr;
  byte *BytePtr;
  bool Found = false, tTime = false, aTime = false, pTime = false, wTime = false;
  byte a, c = 0;
  String NameStr;
  int ASize;

  Found = NewData = StrOK = ByteOK = false;

  a = RecBytes[0] - 48;      // ASCII Number 0-9 subtract 48 
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
  // Use serial <t yy mm dd w hh mm> 22110341200 12:00am 3 Nov 2022 Thursday
  // datetime_t t = { .year  = 2022, .month = 11, .day   = 03, .dotw  = 4, .hour  = 14,  .min   = 00, .sec   = 00 };
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (tTime) { GetTimeData(&t); rtc_set_datetime(&t); TimeSet = true; status(datetime_str); return; }              
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (aTime) { GetTimeData(&alarm); status("Macro Timer set [R-C][O-C]"); return; }
  if (wTime) { GetTimeData(&timer); status("Macro Timer set [RcT][OcT]"); return; }
  if (pTime) { GetTimeData(&power); status("Power Timer set [O-C][R-C]"); return; } 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // These are currently the same type of alarm timer as aTime
  // Can be changed to Clock Time countdown timer: add_alarm_in_ms(2000, alarm_callback, NULL, false);
  //                                               add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
    
  if (Found){ if (a>0) c = a + (LayerAD)*6 - 1;    // S1-S6=>S19-S24 T1-T6=>T19=T24 M1-M6=>M19=M24   ###

              if (Layout==3) {if (a>0) {ByteOK = true;  NameStr = STRname[c]; BytePtr = Str1to12[c]; 
                                        MacroS1S12[c] = 1; MacroSizeS1S12[c] = NumBytes; status("Saved [Sx]"); }  }  // Keys S1-S24                                  
              if (Layout==4) {if (a>0) {ByteOK = true;  NameStr = TTRname[c]; BytePtr = Ttr1to12[c]; 
                                        MacroT1T12[c] = 1; MacroSizeT1T12[c] = NumBytes; status("Saved [Tx]"); }  }  // Keys T1-T24                                  
              if (Layout==1) {if (a>0) {ByteOK = true; NameStr = MTRname[c]; BytePtr = Mtr1to12[c];
                                        MacroM1M12[c] = 1; MacroSizeM1M12[c] = NumBytes; status("Saved [Mx]"); }  }  // Keys M1-M24
                  
              for (n=1; n<=NumBytes; n++) { BytePtr[n-1] = RecBytes[n]; }  }           // Skip 1,2 = char <# 
     
   if ((Found)&&(ByteOK)) {ASize = DoFileBytes(ByteOK, NameStr, BytePtr, NumBytes); }  // Save the numbers - check ASize = NumBytes                                        
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static void alarm_callback(void)  // https://github.com/raspberrypi/pico-examples/blob/master/rtc/
////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not call a non-SDK function from these callbacks - also keep as short as possible
///////////////////////////////////////////////////////////////////////////////////////////////////
{ // char datetime_buf[256]; char *datetime_str = &datetime_buf[0]; datetime_t t = {0};  
  // rtc_get_datetime(&t); datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
  // printf("Alarm Fired At %s\n", datetime_str);
  // stdio_flush(); // if included fail compile
  alarm_fired = true; // Macro Timers 5 and 6
}
///////////////////////////////////
static void power_callback(void)  
///////////////////////////////////
{ 
  power_fired = true;  
}
/////////////////////////////////////
static void timer_callback(void)  
/////////////////////////////////////
{
  timer_fired = true;  // Macro Timers 7 and 8 
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
// Cmd = 'i', 'u', 'r', 't', 'T', 'R', 'a', 'O'
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// case 0:  R-T // HOME - Media Previous    DoPowerKeys ('T', PowerKeysMenu, button);   
// case 1:  Stp // ARROW UP                 DoPowerKeys ('a', PowerKeysMenu, button);
// case 2:  O-T // PAGE UP                  DoPowerKeys ('T', PowerKeysMenu, button);
// case 3:  R-C // V+ or Delete or Execute  DoPowerKeys ('R', PowerKeysMenu, button); PowerClock=1; status("Restart on Clock"); 
// case 4:  R-t // ARROW LEFT               DoPowerKeys ('t', PowerKeysMenu, button);
// case 5:  Cfg // CONFIG                   ---
// case 6:  O-t // ARROW RIGHT              DoPowerKeys('t', PowerKeysMenu, button);
// case 7:  O-C // Vol Mute                 DoPowerKeys ('O', PowerKeysMenu, button); PowerClock=2; status("PowerOff on Clock");
// case 8:  Rst // END                      DoPowerKeys ('r', PowerKeysMenu, button);
// case 9:  Log // ARROW DOWN               DoPowerKeys ('i', PowerKeysMenu, button);
// case 10: Off // PAGE DOWN                DoPowerKeys ('u', PowerKeysMenu, button);
// case 11: [ ] // V- or Enter or Add       ---
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here                  
  int n = 0;                      // s = button pressed

  if (s==3||s==7) {PowerKeys = false; rtc_set_alarm(&power, &power_callback); ConfigButtons(1); return;} // s 3 PowerClock=1 s 7 PowerClock=2
   
  if ((s>7)||(s==1)) { PowerKeys = false; // If immediate action Must do this before the actions below
                       status(" ");  ConfigButtons(1);  }
  ///////////////////////////////////////////////////////////////
  // Windows Restart Shutdown Logout via Menu
  /////////////////////////////////////////////////////////////// 
  if (Menu&&s>0&&OptionOS[0]==0) { keycode[0] = GuiL;             // or use HID_KEY_GUI_RIGHT
                                   keycode[1] = HID_KEY_X;        // 'X' or 'x' not the same here
                                   keycode[2] = 0x00; 
                                   usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay); // GUI + X then U for shutdown options
                                   usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay); // Confirm with ArrowLeft Enter 
                                   usb_hid.keyboardPress(HIDKbrd, 'u');           delay(keydelay); // After U it is r u i
                                   usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay); 
                                   usb_hid.keyboardPress(HIDKbrd, Cmd);           delay(keydelay); // [R]estart sh[U]tdown S[i]gnout
                                   usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay); return; }

  ///////////////////////////////////////////////////////////////
  // Raspberry Pi OS (LXDE) Restart Shutdown Logout via Menu
  ///////////////////////////////////////////////////////////////
  if (Menu&&s>0&&OptionOS[0]==2) { keycode[0] = GuiL;             // PiOS: Open Menu
                                   keycode[1] = 0x00; 
                                   usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay); // GUI + Up for shutdown options
                                   usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay); 
                                   keycode[0] = ArrUp;            // Up to [Shutdown Restart Logout] options
                                   keycode[1] = 0x00; 
                                   usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay); // GUI + Up for shutdown options
                                   usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay); 
                                   usb_hid.keyboardPress(HIDKbrd, '\r');               delay(keydelay); // Confirm with Enter 
                                   usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay3); 
                                   if (Cmd=='i') { keycode[0] = ArrDown;               // PiOS: '2 x ArrDwn''\r' Logout
                                                   keycode[1] = 0x00;            
                                                   usb_hid.keyboardReport(HIDKbrd, 0, keycode);  delay(keydelay); 
                                                   usb_hid.keyboardRelease(HIDKbrd);             delay(keydelay); }
                                   if (Cmd=='r'||Cmd=='i') { keycode[0] = ArrDown;                         // PiOS: '1 x ArrDwn''\r' Restart                                             
                                                             keycode[1] = 0x00;            
                                                             usb_hid.keyboardReport(HIDKbrd, 0, keycode);  delay(keydelay); 
                                                             usb_hid.keyboardRelease(HIDKbrd);             delay(keydelay3); }  
                                                              
                                   usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay); // Cmd = '\r' PowerOff include if (Cmd=='u'||'i'||'R')
                                   usb_hid.keyboardRelease(HIDKbrd);     delay(keydelay3); return; } 
                                                                        
  ////////////////////////////////////////////////////////////////////////
  // Raspberry Pi OS (LXDE) Restart Shutdown Logout via Run Box in Menu
  // See comments section at end of this code for another way to do this
  ////////////////////////////////////////////////////////////////////////
  if (OptionOS[0]==2) { keycode[0] = AltL;             // PiOS: Open Run Box
                        keycode[1] = F02; 
                        keycode[2] = 0x00; 
                        usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay); 
                        usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay);

                        delay(keydelay3);   
                        for (n=0;  n<strlen(ShutDwn2[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn2[s][n]); delay(keydelay);
                                                                usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay); } }                     
                                            
   /////////////////////////////////////////////////////////////////////////////////                     
   // See comments section at end of stringsRSL.h for 2 other ways to do this
   // Change OptionOS with *os*[EXE]
   /////////////////////////////////////////////////////////////////////////////////        
   if (OptionOS[0]==1) {keycode[0] = CtrL;               // Ctr+Alt+T open terminal 
                        keycode[1] = AltL;               // Works in Debian 11.7 without loosing focus to desktop search box
                        keycode[2] = HID_KEY_T;          // Ubuntu 22 and Mint 21 and PiOS (sometimes) loose focus to desktop search box
                        keycode[3] = 0x00;               // Adjust delay longer if characters are missing
                        usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay);  
                        usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay); 
                        delay(keydelay3);   
                        for (n=0;  n<strlen(ShutDwn1[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn1[s][n]); delay(keydelay);
                                                                usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay); } } 

  if (OptionOS[0]==0) { keycode[0] = GuiL;             // or use HID_KEY_GUI_RIGHT
                        keycode[1] = HID_KEY_R;        // 'R' or 'r' not the same here
                        usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); // GUI + X then U for shutdown options
                        usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2);  
  
                        for (n=0;  n<strlen(ShutDwn0[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn0[s][n]); delay(keydelay2);
                                                                usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); } }                                        
                            
  usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay2);           // Confirm with Enter
  usb_hid.keyboardRelease(HIDKbrd);

  if (PowerKeys) { PowerKeys = false; // Clear Power Keys for Timed action
                   status("Timer running");
                   ConfigButtons(1); }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MacroKeys(byte c, int Option) // = MacroM1M12 = 0,1,repeat set with star code *rm*01-12rr *rs*01-12rr *rm*01-12rr rr = repeat 1-99     
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
{ uint8_t keycode[6] = { 0 };      // simultaneous keys pressed in here
  uint8_t a, n, b; 
  int i;
  bool MacroKeysOK = false;
  //showKeyData();
  
  if (Option==0)   // Some callers already copied into MacroBuff
  { if (Layout==1) { for (n=0; n<ByteSize; n++) { MacroBuff[n] = Mtr1to12[c][n]; if (MacroBuff[n]==0x00) break; } }
    if (Layout==3) { for (n=0; n<ByteSize; n++) { MacroBuff[n] = Str1to12[c][n]; if (MacroBuff[n]==0x00) break; } }
    if (Layout==4) { for (n=0; n<ByteSize; n++) { MacroBuff[n] = Ttr1to12[c][n]; if (MacroBuff[n]==0x00) break; } } }
 
  if (MacroBuff[0]==0x00) return false;  // X1-X24 Assume NULL as first byte means no content but an instruction 
                                         // Can use this to add delay - test MacroBuff[1] if also 0 return if >0 delay x seconds here
 
  if (MacroBuff[0]<0x80) { n = 0; while (MacroBuff[n]!=0) { usb_hid.keyboardPress(HIDKbrd, MacroBuff[n]);       delay(keydelay2);
                                                            usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay2); 
                                                            n++; if (n>ByteSize) break; } MacroKeysOK = true; } // Yes else will do it
  if (MacroBuff[0]>0x7F) { for (n=0;  n<6; n++) {keycode[n] = MacroBuff[n]; if (keycode[n]==0) break; }         // allow one 0x00 in keycode
                                                              usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2); 
                                                              usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); 
                                                              MacroKeysOK = true; }                             // Repeat
  return MacroKeysOK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For a repeat macro function do the loop in MacroKeys not here
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DoMLink(byte c, int a)
{ byte n;
   
  File f1 = LittleFS.open(LinkNameM[c][a], "r"); n=0;
  while (f1.available()) { Mtr1to12[c][n] = f1.read(); n++; } f1.close();  
  MacroSizeM1M12[c] = n - (n>0); MacroKeys(c, 0);  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MacroLinkM (byte c) 
{ byte i, n;

  DoMLink(c, 0);
  for (i=1; i<10; i++) { DoMLink(c, i); if (LinkM[c]==i) return true; }
  return false; 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DoSLink(byte c, int a)
{ byte n;
    
  File f1 = LittleFS.open(LinkNameS[c][a], "r"); n=0; 
  while (f1.available()) { Str1to12[c][n] = f1.read(); n++; } f1.close(); 
  MacroSizeS1S12[c] = n - (n>0); MacroKeys(c, 0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MacroLinkS (byte c) 
{ byte i, n;

  DoSLink(c, 0);
  for (i=1; i<10; i++) { DoSLink(c, i); if (LinkS[c]==i) return true; }
  return false; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DoTLink(byte c, int a)
{ byte n;
    
  File f1 = LittleFS.open(LinkNameT[c][a], "r"); n=0; 
  while (f1.available()) { Ttr1to12[c][n] = f1.read(); n++; } f1.close(); 
  MacroSizeT1T12[c] = n - (n>0); MacroKeys(c, 0); 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MacroLinkT (byte c)  // Only 5 chained but can be up to 10
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ byte i, n;
 
  DoTLink(c, 0);
  for (i=1; i<10; i++) { DoTLink(c, i); if (LinkT[c]==i) return true; }      
  return false; 
}

///////////////////////////////
bool DoXFiles(byte xNum)
///////////////////////////////
{ uint8_t keycode[6] = { 0 }; 
  for (int n=0; n<6; n++) keycode[n] = 0x00; 
  keycode[0] = BsDCode1[xNum];     // xNum = xNum[button] = 0-BSDMax = action = Delete etc MSDMax options
  keycode[1] = BsDCode2[xNum]; 
  keycode[2] = BsDCode3[xNum];  
  usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay2); 
  usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2);
  return true;
}

///////////////////////////////
bool DoCodeOption(byte c)
///////////////////////////////
{ usb_hid.keyboardPress(HIDKbrd, Labels[LayerAD][Layout-1][c][0]);       delay(keydelay2);
  usb_hid.keyboardRelease(HIDKbrd);                                      delay(keydelay2);
  usb_hid.keyboardPress(HIDKbrd, Labels[LayerAD][Layout-1][c][1]);       delay(keydelay2);
  usb_hid.keyboardRelease(HIDKbrd);                                      delay(keydelay2);
  usb_hid.keyboardPress(HIDKbrd, Labels[LayerAD][Layout-1][c][2]);       delay(keydelay2);
  usb_hid.keyboardRelease(HIDKbrd);                                      delay(keydelay2);

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ***Mouse
void PadKeysState(int Pbutton, bool Restore)  // Pads are now Pbuttons 1 to 5
{ if (Restore) { status(" ") ;                                                   }                            
          else { if (Pbutton==1) { Kbrd = MouseK  = NumKeys = false; }
                 if (Pbutton==2) { Math = NumKeys = MouseK = false;  }
                 if (Pbutton==3) { Math = Kbrd    = NumKeys = false; }                   
                 if (Pbutton==4) { Math = Kbrd    = MouseK  = false; }
                 if (Pbutton==5) { ;}  } // used as an option key
  ConfigButtons(1);
  if (Kbrd)   status("[NXT]Page [ADD]Char [EXE]Macro"); else status(" ");
  if (MouseK) status("Mouse Buttons-LMR-BF Scroll-UD Cursor-UD-LR"); else status(" ");
}
/////////////////////////////////////////////////
// Select Macro Bank
/////////////////////////////////////////////////
void Bank123Select(int B, byte c, int Button)
{ uint8_t keycode[6];
  int n; 
  for (n=0; n<6; n++) keycode[n] = 0x00; // set last+1 = NULL 
  
  if (Bank123[B]==1) {keycode[0] = Windows1[c]; keycode[1] = Windows2[c]; keycode[2] = Windows3[c]; keycode[3] = Windows4[c];}
  if (Bank123[B]==2) {keycode[0] = PShop1[c];   keycode[1] = PShop2[c];   keycode[2] = PShop3[c];}
  if (Bank123[B]==3) {keycode[0] = BsDCode1[c]; keycode[1] = BsDCode2[c]; keycode[2] = BsDCode3[c];}
  if (Bank123[B]==4) {keycode[0] = GuiL;        keycode[1] = KeyR;} // Open Run Box
  if (Bank123[B]==5) {keycode[0] = GuiL;        keycode[1] = Key2;} // Open Powershell 

  usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
  usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2);

  if (Bank123[B]==4) {n = 0; while (RunCode[c][n]!=0) { usb_hid.keyboardPress(HIDKbrd, RunCode[c][n]); delay(keydelay2); // Add \r at end
                                                        usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); 
                                                        n++; if (n>ByteSize) break; } }                // Bank123[B]=4

                                                         
  if (Bank123[B]==5) {n = 0; while (CmdCode[c][n]!=0) { usb_hid.keyboardPress(HIDKbrd, CmdCode[c][n]); delay(keydelay2); // Add \r at end
                                                        usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); 
                                                        n++; if (n>ByteSize) break; } }                // Bank123[B]=5
                                                        
  if ((Bank123[B]==4)||(Bank123[B]==5))               { usb_hid.keyboardPress(HIDKbrd, '\r');          delay(keydelay2);
                                                        usb_hid.keyboardRelease(HIDKbrd); }            // Bank123[B]=4,5
  
  status(Labels[LayerAD][Layout-1][Button]); 
}
/////////////////////////////
void buttonpress(int Button)
/////////////////////////////
{
  uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here
  uint8_t i, n, a, b, c, x;       // 
  uint8_t CfgOK;                  // Do Cfg actions
  uint8_t modifiers = 0;          // Alt Shift Control Gui modifiers b7 RG| RA| RS| RC| LG| LA| LS| LC| b0
  byte *BPtr;
  
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
  
  // Macro Timers must either be executed (buttons 1 or 0,2,3,4,6,7,8,9,10) or cancelled (button 5 or 11)  
  if (MacroTimerDisp) { if (Button==11) { if (MacroTimer18)  status("Macro Timer running in background");  
                                          if (!MacroTimer18) status("No Macro Timer running");
                                          MacroTimerDisp=false; ConfigButtons(1); return; } }
                                          
  // Power Keys must either be executed (buttons 1 or 0,2,3,4,6,7,8,9,10) or cancelled (button 5 or 11)
  if (PowerKeys) {if (Button==11) {status("Power Keys exit"); PowerKeys=false; ConfigButtons(1); return;} }
  
  ///////////////////////////////////////////////////////////////////////////////// ***Mouse
  // 5 Config Pads Blue Gray Purple Gray Red button > 11
  // Handle 1st then return - as if button = 0-11
  // Pad Keys switch off all other KeyBrds 
  ///////////////////////////////////////////////////////////////////////////////
  if ((Button>11)&&(Layout==2)) 
     { PadKeys = true; 
       if (Button==12) { Math = !Math;           PadKeysState(Button-11, !Math);      return; } // Pbutton = 1 to 5                       
       if (Button==14) { MouseK = !MouseK;       PadKeysState(Button-11, !MouseK);    return; }
       if (Button==15) { NumKeys = !NumKeys;     PadKeysState(Button-11, !NumKeys);   return; }       
       if (Button==16) { if (Kbrd) {KeyBrdDirect = !KeyBrdDirect; optionsindicators(10); // 10 = space
                                    if (KeyBrdDirect) status("KeyBoard Direct ON"); else status("KeyBoard Direct OFF"); return;}
                         if (NumKeys) { if (Numkeys123<2) Numkeys123++ ; else Numkeys123 = 0; NumKeysChange(); ConfigButtons(1); return;}
                         // if (Media) {ToneControls = !ToneControls; ConfigButtons(1); return;}
                         PadCapsNumN++; if (PadCapsNumN>3) PadCapsNumN = 0;
                         keycode[0] = PadCapsNum[0][PadCapsNumN]; keycode[1] = PadCapsNum[1][PadCapsNumN];
                         usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
                         usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2);
                         return; }       
       Kbrd = !Kbrd; SendBytesEnd(0); PadKeysState(Button-11, !Kbrd); return;  }  // do remaining pad   

  if ((Button>11)&&(Layout!=2))   // 5 Navigation Pads Blue Gray Purple Purple Blue button > 11                                                    
     {if (Button==16) { if (LayerAD<3) LayerAD++; else LayerAD = 0;}                  
      if (Button==12) { if (LayerAD>0) LayerAD--; else LayerAD = 3;} 
      if (Button==16||Button==12) {PadKeys = true; LayerADLetter[0] = 65 + LayerAD; optionsindicators(0); ConfigMacroButtons(); return; }
      if ((Button==13&&Layout==1)||(Button==14&&Layout==3)||(Button==15&&Layout==4))              // If own Padkey pressed in own Layout
         { LayerAxD = !LayerAxD; if (LayerAxD) status("Preprogram On"); else status ("Preprogram Off"); optionsindicators(0); return; } 
      Layout = Button-11-(Button==13); PadKeys = true; status(" "); ConfigButtons(1); return; }   // pad MST = button 13,14,15 = Layout 0,2,4  
                                  
  n = 0;                          // for the while loops
  KeyRepeat = false;                                            
  if (PrevButton!=Button) KeyBrd123 = 0; else KeyRepeat = true; // Start at a not b or c if other button previously used
  PrevButton = Button;
    
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////  ###
  //                StrButton[button]             4  5  6     8  9  10                 4  5  6      8  9  10
  //  M1-M6 M7-M12 etc            c               0  1  2     3  4  5                  6  7  8      9  10 11
  //           byte StrButton[48] = { 0, 0, 0, 0, 0, 1, 2, 0, 3, 4, 5,  0, 0, 0, 0, 0, 6, 7, 8,  0, 9, 10,11,0, 
  //                                  0, 0, 0, 0, 12,13,14,0, 15,16,17, 0, 0, 0, 0, 0, 18,19,20, 0, 21,22,23, 0  };
  c = StrButton[Button + (LayerAD*12)];   // c = 0 - 23 map button 0-11 to macrokeys 0-23 -> 1-24
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  LockMedia = (!MuteDisable&&Media&&!VolDisable);
  
  //if ((!MuteDisable)&&(VolMuteCount>0)&&(button!=7)) {VolMuteCount = 0; // Any other key resets Mute pressed count
  //                                    VolMuteMillisLast  = millis(); }  // Set Timer = now 5 seconds to select next Layout
 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // reset if not [Cfg] and [Home] [UpArrow] [PageUp] [<] [Cfg] [>] [End] [DwnArrow] [PageDown] key pressed
  // StrButton[button]      0  1  2  3  4  5  6  7  8  9  10 11
  //             CfgOK      x  x  x     x  x  x     x  x  x 
  // byte CfgButton[12] = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
  CfgOK = CfgButton[Button];

  if (XFiles==true) x = Button + Layout - (Layout==1) - (Layout==4);  // Used in DoXFiles
  
  if ((Layout==2) && (CfgOK==0) && (ConfigKeyCount>0)) {ConfigKeyCount = 0; status(" "); } 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////// 
                                                           
  if (Layout!=2) {   // Layout 1 or 3 or 4
  switch(Button){    
    case 0: ///////////////////////////////////////////////// Cut = control + x  Cannot do case 0 ... 2:
      if (XFiles) { DoXFiles(XNum[x]); break; }  
                       
      keycode[0] = KeyX; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(keydelay); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = KeyX;  
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      //usb_hid.keyboardRelease(HIDKbrd);  
      break;
    case 1: ///////////////////////////////////////////////// Copy = control + c
      if (XFiles) { DoXFiles(XNum[x]); break; }
      
      keycode[0] = KeyC; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(keydelay); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = KeyC; 
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      //usb_hid.keyboardRelease(HIDKbrd);  
      break;
    case 2: ////////////////////////////////////////////////// Paste = control + v
      if (XFiles) { DoXFiles(XNum[x]); break; }
      
      keycode[0] = KeyV; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(keydelay); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = KeyV;  
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      //usb_hid.keyboardRelease(HIDKbrd);  
      break;
    case 3: //////////////////////////////////////////////// Volume Increase or Delete BackSpace Tab Insert

     if (VolDisable) { keycode[0] = BsDCode1[BsDNum];   // Delete BackSpace Tab AltTab Insert Esc PScr Num Caps Scroll etc
                       keycode[1] = BsDCode2[BsDNum]; 
                       keycode[2] = BsDCode3[BsDNum]; 
                       // keycode[3] = keycode[4] = keycode[5] = 0x00; 
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay2); 
                       usb_hid.keyboardRelease(HIDKbrd); delay(keydelay2); break; }
   
      if (Brightness) usb_hid.sendReport16(HIDCons, 0x006F);
                 else usb_hid.sendReport16(HIDCons, VolUp);  delay(keydelay); 
      usb_hid.sendReport16(HIDCons, 0);      break;
      
    case 4: /////////// 12 keys handled here: M1 M7 M13 M19 - S1 S7 S13 S19 - T1 T7 T13 T19 - select by [Layout=1,3,4 and LayerAD=0,1,2,3        
         if (Layout==3)                                                            // Keys S1 S7 S13 S19
            { if (!LayerAxD) { if (LinkS[c]==0) { if (MacroKeys(c, 0)) break; }    // If not "X" and macro defined then do this
                               if (LinkS[c]>0)  { if (MacroLinkS(c))   break; } }            
             
               Bank123Select(1, c, Button);   break; }  // Layout=3    
               
         if (Layout==4)                                                            // Keys T1 T7 T13 T19
            { if (!LayerAxD) { if (LinkT[c]==0) { if (MacroKeys(c, 0)) break; }    // If not "X" and macro defined then do this
                               if (LinkT[c]>0)  { if (MacroLinkT(c))   break; } }  

              Bank123Select(2, c, Button);   break; }  // Layout=4 

         if (Layout==1)                                                            // Keys M1 M7 M13 M19
            { if (!LayerAxD) { if (LinkM[c]==0) { if (MacroKeys(c, 0)) break; }    // If not "X" and macro defined then do this
                               if (LinkM[c]>0)  { if (MacroLinkM(c)) break;   } }     
              // Code here for Keys M1 M7 M13 M19 select each by LayerAD=0,1,2,3 
              // In this case no selection => all four keys will do Alt PrintScreen
              keycode[0] = AltL; 
              keycode[1] = PScr;              
              usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
              usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2);
              status("PScr Current Window");   }  break; // Layout=1 or default
      
    case 5: /////////// 12 keys handled here: M2 M8 M14 M20 - S2 S8 S14 S20 - T2 T8 T14 T20 - select by [Layout=1,3,4 and LayerAD=0,1,2,3 
         if (Layout==3)                                                             // Keys S2 S8 S14 S20
            { if (!LayerAxD) { if (LinkS[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkS[c]>0)  { if (MacroLinkS(c))   break; } }
                               
              Bank123Select(1, c, Button);   break; }  // Layout=3  
                
         if (Layout==4)                                                            // Keys T2 T8 T14 T2
            { if (!LayerAxD) { if (LinkT[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkT[c]>0)  { if (MacroLinkT(c))   break; } } 
                                
              Bank123Select(2, c, Button);   break; }  // Layout=4  
              
        if (Layout==1)                                                               // Keys M1 M7 M13 M19
            { if (!LayerAxD) { if (LinkM[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkM[c]>0)  { if (MacroLinkM(c)) break;   } }  
              // Code here for Keys M2 M8 M14 M20 select each by LayerAD=0,1,2,3 
              // In this case no selection => all four keys open Admin Powershell 
             keycode[0] = GuiL;                                               // or use HID_KEY_GUI_RIGHT
             keycode[1] = HID_KEY_X;
             usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); // GUI + X then A admin powershell i normal powershell g Computer Management
             usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); // Confirm with ArrowLeft Enter 
             usb_hid.keyboardPress(HIDKbrd, 'A');           delay(keydelay2); // Can also use Alt + "Y" too confirm
             usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay3); // Can also open via Run GUI + R then Ctrl+Shft+Enter then Alt+Y
             keycode[0] = ArrLeft;
             keycode[1] = KEnt;
             usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); 
             usb_hid.keyboardRelease(HIDKbrd);   } break; // Layout=1
      
    case 6: /////////// 12 keys handled here: M3 M9 M15 M21 - S3 S9 S15 S21 - T3 T9 T15 T21 - select by [Layout=1,3,4 and LayerAD=0,1,2,3 
         if (Layout==3) 
            { if (!LayerAxD) { if (LinkS[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkS[c]>0)  { if (MacroLinkS(c))   break; } }
             
              Bank123Select(1, c, Button);   break; }  // Layout=3 
               
         if (Layout==4) 
            { if (!LayerAxD) { if (LinkT[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkT[c]>0)  { if (MacroLinkT(c))   break; } }  
                               
              Bank123Select(2, c, Button);   break; }  // Layout=4

        if (Layout==1) 
            { if (!LayerAxD) { if (LinkM[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkM[c]>0)  { if (MacroLinkM(c)) break;   } }  

             keycode[0] = GuiL;                 // or use HID_KEY_GUI_RIGHT
             keycode[1] = HID_KEY_X;
             usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
             usb_hid.keyboardRelease(HIDKbrd);  }  break; // Layout=1
      
    case 7: //////////////////////////////////////////////// Volume Mute and change Layouts
      VolMuteCount++; if (VolMuteCount>4) VolMuteCount=1; Layout = VolMuteCount; ConfigButtons(1);    
      if (MuteDisable) break;  // No VolumeMute action toggle with [Cfg][Hme] sequence

      usb_hid.sendReport16(HIDCons, VolMute); delay(keydelay2);
      usb_hid.sendReport16(HIDCons, 0);       delay(keydelay2); break;
      
    case 8: /////////// 12 keys handled here: M4 M10 M16 M22 - S4 S10 S16 S22 - T4 T10 T16 T22 - select by [Layout=1,3,4 and LayerAD=0,1,2,3 
         if (Layout==3) 
            { if (!LayerAxD) { if (LinkS[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkS[c]>0)  { if (MacroLinkS(c))   break; } }
             
              Bank123Select(1, c, Button);   break; }  // Layout=3
               
         if (Layout==4) 
            { if (!LayerAxD) { if (LinkT[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkT[c]>0)  { if (MacroLinkT(c))   break; } }  
                                
              Bank123Select(2, c, Button);   break; }  // Layout=4 
              
        if (Layout==1) 
            { if (!LayerAxD) { if (LinkM[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkM[c]>0)  { if (MacroLinkM(c)) break;   } } 
                          
              for ( n=0;  n<strlen(inputString); n++ ) 
                  { usb_hid.keyboardPress(HIDKbrd, inputString[n]); delay(keydelay2);
                    usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); }   }  break; // Layout=1 
                                                                                                   
    case 9: /////////// 12 keys handled here: M5 M11 M17 M23 - S5 S11 S17 S23 - T5 T11 T17 T23 - select by [Layout=1,3,4 and LayerAD=0,1,2,3
         if (Layout==3) 
            { if (!LayerAxD) { if (LinkS[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkS[c]>0)  { if (MacroLinkS(c))   break; } }
             
              Bank123Select(1, c, Button);   break; }  // Layout=3

         if (Layout==4) 
            { if (!LayerAxD) { if (LinkT[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkT[c]>0)  { if (MacroLinkT(c))   break; } }  
                               
              Bank123Select(2, c, Button);   break; }  // Layout=4 
             
        if (Layout==1) 
            { if (!LayerAxD) { if (LinkM[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkM[c]>0)  { if (MacroLinkM(c)) break;   } } 
          
             keycode[0] = GuiL;             // Admin CMD/Powershell: GUI + R then cmd/powershell then instead of Enter use Ctrl+Shft+Enter then Alt+Y
             keycode[1] = HID_KEY_R;        // 
             usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
             usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
      
            for (n=0;  n<ArraySz(str9); n++)                  // str7 Normal Cmd Prompt str8 winver
                {usb_hid.keyboardPress(HIDKbrd, str9[n]);     delay(keydelay2);
                 usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2); }
                 delay(keydelay3);
                 keycode[0] = CtrL;     // Instead of just [Enter] use [Control + Shift + Enter] => Run as admin
                 keycode[1] = ShfL;     // 
                 keycode[2] = KEnt;     // 
                 usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
                 usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
                 delay(keydelay3);
                 keycode[0] = AltL;     // Alt + Y on UAC window instead of ArrowLeft+Enter
                 keycode[1] = KeyYes;   // Y or y
                 keycode[2] = KEnt;     //
                 usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
                 usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); }    // Layout=1
          break; 
          
    case 10: /////////// 12 keys handled here: M6 M12 M18 M24 - S6 S12 S18 S24 - T6 T12 T18 T24 - select by [Layout=1,3,4 and LayerAD=0,1,2,3         
         if (Layout==3) 
            { if (!LayerAxD) { if (LinkS[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkS[c]>0)  { if (MacroLinkS(c))   break; } }
             
              Bank123Select(1, c, Button);   break; }  // Layout=3

         if (Layout==4) 
            { if (!LayerAxD) { if (LinkT[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkT[c]>0)  { if (MacroLinkT(c))   break; } }  
                               
              Bank123Select(2, c, Button);   break; }  // Layout=4

        if (Layout==1) 
            { if (!LayerAxD) { if (LinkM[c]==0) { if (MacroKeys(c, 0)) break; }
                               if (LinkM[c]>0)  { if (MacroLinkM(c)) break;   } }                

      // Open Task Manager ///////////////////////////////////////////////////////
      keycode[0] = Esc; // Thsi works and is shorter than the other way below
      tud_hid_keyboard_report(HIDKbrd, ModCtrL + ModShfL + 0x00, keycode); delay(keydelay); 
      tud_task();        // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode); }   break; // Layout=1
       
    case 11: ////////////////////////////////////////// Volume decrease or Return
      if (VolDisable) { usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(HIDKbrd); break; }

      if (Brightness) usb_hid.sendReport16(HIDCons, 0x0070);
                 else usb_hid.sendReport16(HIDCons, VolDown);  delay(keydelay); 
      usb_hid.sendReport16(HIDCons, 0);       break; 
      
  }   // switch Button
  }   // if Layout = 1,3.4

  //////////////////////////////////// if Layout==2
  else     
  switch(Button){
    case 0: // HOME - Media Previous //////////////////////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('T', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer1 = !MacroTimer1; MacroTimer18 = MacroTimer18 || MacroTimer1;
                        if (MacroTimer1) { TimeRepeatPrev = millis(); status("[R-T] Repeat Macro Timer started"); }
                                                                 else status("[R-T] Repeat Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }
    
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break; }

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_LEFT);  delay(keydelay);             // ***Mouse
                   usb_hid.mouseButtonRelease(RID_MOUSE);                   delay(keydelay); break; }
     
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
    
      if (DoUpKey)      // Set by SendBytes
         { if (MST2==0) { for (n=0; n<MacroBuffSize; n++) { Mtr1to12[Option2][n] = MacroBuff[n]; } status("M Macro OK"); MacroSizeM1M12[Option2] = MacroBuffSize;
                          MacroM1M12[Option2] = 1; BPtr = Mtr1to12[Option2]; DoFileBytes(1, MTRname[Option2], BPtr, MacroBuffSize); } 
           if (MST2==1) { for (n=0; n<MacroBuffSize; n++) { Str1to12[Option2][n] = MacroBuff[n]; } status("S Macro OK"); MacroSizeS1S12[Option2] = MacroBuffSize;
                          MacroS1S12[Option2] = 1; BPtr = Str1to12[Option2]; DoFileBytes(1, STRname[Option2], BPtr, MacroBuffSize);  } 
           if (MST2==2) { for (n=0; n<MacroBuffSize; n++) { Ttr1to12[Option2][n] = MacroBuff[n]; } status("T Macro OK"); MacroSizeT1T12[Option2] = MacroBuffSize; 
                          MacroT1T12[Option2] = 1; BPtr = Ttr1to12[Option2]; DoFileBytes(1, TTRname[Option2], BPtr, MacroBuffSize);  }
           DoUpKey = false; }
    
      if (PowerKeys) { DoPowerKeys ('a', PowerKeysMenu, Button); break; }

      if (Media && ToneControls) {usb_hid.sendReport16(HIDCons, BassUp); delay(keydelay);
                                  usb_hid.sendReport16(HIDCons, 0); break; }
                  
      if (MacroTimerDisp) 
         { MacroTimer8 = MacroTimer7 = MacroTimer6 = MacroTimer5 = Macrotimer4 = MacroTimer3 = Macrotimer2 = MacroTimer1 = MacroTimerDisp = MacroTimer18 = false;
            MacroTimerOK = false; status("All Macro Timers stopped"); ConfigButtons(1); optionsindicators(Button); break; }
      
      if (Math) {MakeMathStr(Button); break; }
      
      if (Kbrd) {MakeStr(Button); break; }

      if (MouseK) {usb_hid.mouseMove(RID_MOUSE, 0, -1*MouseDelta);  delay(keydelay); break; }
               
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}     
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              BsDNum++; if (BsDNum>BSDMax) BsDNum=0; // Delete BackSpace Tab AltTab Insert Esc etc
                              GetSysInfo(3);                         // Save state on Flash                              
                              ConfigButtons(1); break; }
                        
      keycode[0] = ArrUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 2: // PAGE UP /////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('T', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer3 = !MacroTimer3; MacroTimer18 = MacroTimer18 || MacroTimer3;
                        if (MacroTimer3){ TimeOnceofPrev = millis(); status("[O-T] Oneshot Macro Timer started"); }
                                                                else status("[O-T] Oneshot Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }   
    
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break;}

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_RIGHT); delay(keydelay);           // ***Mouse
                   usb_hid.mouseButtonRelease(RID_MOUSE);                   delay(keydelay); break; }
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              VolDisable = !VolDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (VolDisable) status("Volume V+V- Disabled"); else status("Volume  V+V- Enabled");
                              ConfigButtons(1); break; }

      if (Media && ToneControls) {usb_hid.sendReport16(HIDCons, TrebUp); delay(keydelay);
                                  usb_hid.sendReport16(HIDCons, 0); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = PageUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 3: // V+ or Delete or Escape(Math) or Execute(Keybrd) //////////////////////////////////////
      if (PowerKeys) { PowerClock=1; status("Restart on Clock"); DoPowerKeys ('R', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer7 = !MacroTimer7; MacroTimer18 = MacroTimer18 || MacroTimer7;
                        if (MacroTimer7) { rtc_set_alarm(&timer, &timer_callback); status("[RcT] Repeat Macro Countdown Clock started");}
                                                                              else status("[RcT] Repeat Macro Countdown Clock stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }
    
      if (Math) {Math = false; status("Math exit");  MathX = 0; Math123 = 0; ConfigButtons(1); break;}
    
      if (Kbrd) {if (KeyBrdX==3) {MakeStr(Button); break;}  // [EXE] change to [Snd] Send
                 if (KeyBrdDirect)  {usb_hid.keyboardPress(HIDKbrd, KeyBrdByte[KeyBrdByteNum]); delay(keydelay);     
                                     usb_hid.keyboardRelease(HIDKbrd);                          delay(keydelay); break; }
                 Kbrd = false; status((char *)KBDisp);                    
                 if (KeyBrdByteNum>0) {SendBytes();}          // Execute macro + exit keybrd
                 SendBytesEnd(1); ConfigButtons(1); break;}   // Clear incase wrong symbols added or no char added

      if (MouseK) {usb_hid.mouseScroll(RID_MOUSE, MouseScrollAmount, 0); break; }         
       
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

     if (VolDisable) { keycode[0] = BsDCode1[BsDNum];      // Delete BackSpace Tab AltTab Insert Esc PScr Num Caps Scrol
                       keycode[1] = BsDCode2[BsDNum]; 
                       keycode[2] = BsDCode3[BsDNum];                        
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay2); 
                       usb_hid.keyboardRelease(HIDKbrd); delay(keydelay2); break; }
        
      usb_hid.sendReport16(HIDCons, VolUp); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);     break;
      
    case 4: // ARROW LEFT //////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('t', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { Macrotimer2 = !Macrotimer2; MacroTimer18 = MacroTimer18 || Macrotimer2;
                        if (Macrotimer2) { timeRepeatPrev = millis(); status("[R-t] Repeat Macro Timer started"); } 
                                                                 else status("[R-t] Repeat Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }
    
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break;}
      
      if (MouseK) {usb_hid.mouseMove(RID_MOUSE, -1*MouseDelta, 0);  delay(keydelay); break; } 
                   
      if (ConfigKeyCount==1) {ConfigKeyCount--; LayerAD++; if (LayerAD==4) LayerAD = 0; LayerADLetter[0] = 65 + LayerAD;
                              optionsindicators(0); ConfigButtons(1); GetSysInfo(2); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, PrevMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }      
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);               break;}

      keycode[0] = ArrLeft;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 5: // CONFIG //////////////////////////////////////////////////
      if (Math) {MakeMathStr(Button); break; }
      
      if (Kbrd) {MakeStr(Button); break;}
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_MIDDLE); delay(keydelay);                  // ***Mouse
                   usb_hid.mouseButtonRelease(RID_MOUSE);                    delay(keydelay); break; }
     
      GetSysInfo(0);           
      if (ConfigKeyCount==1) { status("Config and Files Saved"); ConfigKeyCount = 0; }                 
                        else { status("1L4 Bs Vol AD Sav Mou Del Po 1S4"); ConfigKeyCount++ ; } 
      ConfigButtons(1);  break;
      
    case 6: // ARROW RIGHT /////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys('t', PowerKeysMenu, Button); break; }
      
      if (MacroTimerDisp) { Macrotimer4 = !Macrotimer4; MacroTimer18 = MacroTimer18 || Macrotimer4;
                        if (Macrotimer4) { timeOnceofPrev = millis(); status("[O-t] Oneshot Macro Timer started"); }
                                                                 else status("[O-t] Oneshot Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }
      
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break;}

      if (MouseK) {usb_hid.mouseMove(RID_MOUSE, MouseDelta, 0);  delay(keydelay); break; } 
    
      // Power Restart Logoff Timer options
      if (ConfigKeyCount==1) { ConfigKeyCount--; 
                               if (MacroTimerOK) { MacroTimerDisp = !MacroTimerDisp; }
                                            else { status("No Macro -> Timer Link"); ConfigButtons(1); break; }
                               if (MacroTimerDisp) status("[R-T/C]Repeat Timer/Clock [O-T/C]Oneshot"); else status("MacroTimer Exit");
                               ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, NxtMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = ArrRight;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 7: // Vol Mute ///////////////////////////////////////////////////////////////
      if (PowerKeys) { PowerClock=2; status("PowerOff on Clock"); DoPowerKeys ('O', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer8 = !MacroTimer8; MacroTimer18 = MacroTimer18 || MacroTimer8;
                        if (MacroTimer8) {rtc_set_alarm(&timer, &timer_callback); status("[OcT] Oneshot Macro Countdown Clock started");  }                        
                                                                             else status("[OcT] Oneshot Macro Countdown Clock stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }
             
      if (Math) {if (MathX<3) MathX++; else MathX=0; 
          Math123=0; ConfigButtons(1); break;} // Changes 4 Math KeyBrds from Greek abc -> ABC -> Calculus -> Logic
        
      if (Kbrd) {if (KeyBrdX<4) KeyBrdX++; else KeyBrdX=0; 
          KeyBrd123=0; ConfigButtons(1); break;} // Changes 5 KeyBrds from abc - ABC - 012 - *^% - SHFT

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_LEFT);  delay(keydelay);             // ***Mouse
                   usb_hid.mouseButtonRelease(RID_MOUSE);                   delay(200); 
                   usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_LEFT);  delay(keydelay);        // ***Mouse
                   usb_hid.mouseButtonRelease(RID_MOUSE);                   delay(keydelay);
                   break; }
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      if (!LockMedia) { VolMuteCount = Layout; VolMuteCount++; if (VolMuteCount>4) VolMuteCount=1; Layout = VolMuteCount; ConfigButtons(1); }
           
      if (MuteDisable) break;  // No VolumeMute do not do the action below

      usb_hid.sendReport16(HIDCons, VolMute); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);       break;
      
    case 8: // END //////////////////////////////////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('r', PowerKeysMenu, Button); break;} 

      if (MacroTimerDisp) { MacroTimer5 = !MacroTimer5; MacroTimer18 = MacroTimer18 || MacroTimer5;
                        if (MacroTimer5) { rtc_set_alarm(&alarm, &alarm_callback); status("[R-C] Repeat Macro Clock started");}
                                                                              else status("[R-C] Repeat Macro Clock stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }

      if (Math) {MakeMathStr(Button); break; }
      
      if (Kbrd) {MakeStr(Button); break;}

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_BACKWARD);  delay(keydelay);
                   usb_hid.mouseButtonRelease(RID_MOUSE);                       delay(keydelay); break; }
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;               
                              Media = !Media; 
                              if (Media) { if (MediaCfg) MediaConfig2Media(0); status("Media Keys ON");  }
                                  else   { if (MediaCfg) MediaConfig2Media(1); status("Media Keys OFF"); }
                              ConfigButtons(1); break; }
                  if (Media) {usb_hid.sendReport16(HIDCons, PlayMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }

      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);            break;}

      keycode[0] = KeyEnd;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 9: // ARROW DOWN //////////////////////////////////////////////
      if (Math) {MakeMathStr(Button); break; }
    
      if (Kbrd) {MakeStr(Button); break;}

      if (MouseK) {usb_hid.mouseMove(RID_MOUSE, 0, MouseDelta);  delay(keydelay); break; } 

      // Power Restart Logoff Timer options
      if (ConfigKeyCount==1) {ConfigKeyCount--; PowerKeys = !PowerKeys;  
          if (PowerKeys) status("Timer: [Rst]art [Log]off Power[Off] Clock R-O"); else status(" ");
          ConfigButtons(1); break;} 

      if (Media && ToneControls) {usb_hid.sendReport16(HIDCons, BassDwn); delay(keydelay);
                                  usb_hid.sendReport16(HIDCons, 0); break; }
             
      if (PowerKeys) { DoPowerKeys ('i', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { status("Repeat Macro not yet implemented"); MacroTimerDisp = false; ConfigButtons(1); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}
     
      keycode[0] = ArrDown;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 10: // PAGE DOWN //////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('u', PowerKeysMenu, Button); break; }

      if (MacroTimerDisp) { MacroTimer6 = !MacroTimer6; MacroTimer18 = MacroTimer18 || MacroTimer6;
                        if (MacroTimer6) {rtc_set_alarm(&alarm, &alarm_callback); status("[O-C] Oneshot Macro Clock started"); }                        
                                                                             else status("[O-C] Oneshot Macro Clock stopped");
                        ConfigButtons(1); optionsindicators(Button); break; }
                        
      if (Math) {MakeMathStr(Button); break; }
      
      if (Kbrd) {MakeStr(Button); break;}

      if (MouseK) {usb_hid.mouseButtonPress(RID_MOUSE, MOUSE_BUTTON_FORWARD);  delay(keydelay);
                   usb_hid.mouseButtonRelease(RID_MOUSE);                      delay(keydelay); break; }
      
      if (ConfigKeyCount==1) {ConfigKeyCount--; SaveLayout[0]++; if (SaveLayout[0]>4) SaveLayout[0] = 1; GetSysInfo(1); // Save the layout state
                              if (Layout>0) status("Startup Layout L1-L4"); else status("Startup Layout L2"); ConfigButtons(1); break;}

      if (Media && ToneControls) {usb_hid.sendReport16(HIDCons, TrebDwn); delay(keydelay);
                                  usb_hid.sendReport16(HIDCons, 0); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = PageDwn;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 11: // V- or Enter or Add /////////////////////////////////////////////////////////////////////////////////////////////////////
      if (Math) { SendMath(); status("Math symbol sent"); MathByteNum=0; break; }  
    
      if (Kbrd) {if (KeyBrdX==3) {MakeStr(Button); break;}  // [ADD] now [Sav] Save
                if (!KBrdActive) {status("Press char/mod first"); break;} // No char key or modifier keys has been pressed at least once
                 /*if (KeyRepeat) {if (KeyBrdX>2) break;
                                 KeyBrdByte[KeyBrdByteNum] = KeyBrdByte[KeyBrdByteNum-1]; KBDispPos--; KeyBrdByteNum--;
                                 MakeStr(PrevButton);}  */
                 if (KeyBrdX==4) {KBDispPos = KBDispPos + 2;  if (KeyBrdByte[0]>0x7F) KBType = true; } // can be 2nd or 3rd?
                 if (Fx) {KBDispPos = KBDispPos + 2; }
                 KBDispPos++; KeyBrdByteNum++; break; }  

      if (MouseK) {usb_hid.mouseScroll(RID_MOUSE, -1*MouseScrollAmount, 0); break; } 
                 
      if (MacroTimerDisp) { status("Exit Timer Config"); MacroTimerDisp = false; ConfigButtons(1); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[Button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      if (VolDisable) { usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(HIDKbrd); break; }

      usb_hid.sendReport16(HIDCons, VolDown); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);       break;
  }   // Layout=2 Button switch
}     // buttonpress

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigMacroButtons() {  // Only updates six macro buttons - reduce screen flicker
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int n, m, b;
  uint8_t rows = 3;    // 3 rows of labels
  uint8_t row, col;    // keys

  b = Layout - 1;                              //  L=1/b=0  L=2/b=1  L=3/b=2 L=4/b=3    
 
  uint16_t OutlineColor = ButtonOutlineColor[b];
  
  for (n=4; n<11; n++)  { keyColor[n] = Colours[b][n]; for (m=0; m<3; m++) keyLabel[n][m] = Labels[LayerAD][b][n][m]; } 
              
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
// void drawButton(bool inverted = false, String long_name = "");
// Compute the squared distance (d2) from the button center to the current touch coordinates.
// d2 = (bx - tx)*(bx - tx) + (by - ty)*(by - ty);
// If d2 is less than the button radius squared, then that button was selected.
// You will find this function, which does not do any multiplies, in the Adafruit graphics library:
// boolean Adafruit_GFX_Button::contains(int16_t x, int16_t y) 
//          {return ((x >= _x1) && (x < (_x1 + _w)) &&  (y >= _y1) && (y < (_y1 + _h))); }}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigButtons(uint8_t rowcount) {  // rowcount=0 all 4 rows rowcount=2 last row only  rowcount=1 no background redraw 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int n, m, b, p, k;
  uint8_t rows = 3;    // 3 rows of labels
  uint8_t row, col;    // keys
  uint16_t PadB, PadC; // 5 Pads Red Blue Green SkyBlue Yellow
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
  
  for (n=0; n<12; n++) for (m=0; m<3; m++) keyLabel[n][m] = Labels[LayerAD][b][n][m]; // label always = 4 chars \0 terminate stringlen 1,2,3  
  for (n=0; n<12; n++) keyColor[n] = Colours[b][n];
  
  if (VolDisable) for (m=0; m<3; m++) keyLabel[3][m] = BsDLabel[BsDNum][m];                       // Delete -> ScrollL  
     
  b = Layout - 1;                               // reset b to 0 - 3
  k = (b>1)*3;
  if (XFiles && b!=1) { for (n=0; n<3; n++) for (m=0; m<3; m++) keyLabel[n][m] = BsDLabel[XNum[n+k]][m]; }  // Delete -> CtrlV
  
  if (Brightness && b!=1 && !VolDisable) { keyLabel[3][0] = keyLabel[11][0] = 'B'; }  // *br* = brightness up/dwn in M S T repace volume  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  If Layout 2 [Cfg] Key pressed new options in different colour:
  //   "Hme", "Up",   "PgU", "Del/V+",  "<",    "Cfg", ">",    "V0/L14",  "End", "Dwn", "PgD", "V-/Ret"
  //   "1L4", "Var",  "Vol", "   ",     "A-D",  "Sav", "Med",  "   ",     "Del", "Kbd",  "1S4", "   " 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // also see for loop comma operator use here: https://forum.arduino.cc/t/arduino-does-not-understand-a-ranged-for-loop/164312/10
  int cR[9] = { 0, 1, 2, 4, 5, 6, 8, 9, 10 };    
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
  int mR[4] = { 0, 4, 6, 8 };
  int mT[8] = { 0, 1, 2, 4, 6, 8, 9, 10 };
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
              for (m=0; m<11; m++) {keyColor[m] = MathColour1;} keyColor[3]=keyColor[7]=keyColor[11]=MathColour2; goto KeyBrdsDone; } // I can do it once

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Kbrd) {for (m=0; m<12; m++) {strcpy(keyLabel[m], KbrdLabel[KeyBrdX][m]);}
             for (m=0; m<12; m++) {keyColor[m] = CfgColour;}; goto KeyBrdsDone; }                                                     // or twice

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (MouseK) {for (m=0; m<12; m++) {strcpy(keyLabel[m], MouseKeysLabel[m]); keyColor[m] = MouseKeyColour[m]; }; goto KeyBrdsDone; }     // even thrice                                              // and thrice
                 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // byte     RXlat[12]   = {0,     7,     8,   9,      0,      4,      5,    6,      0,     1,      2,    3      };  // NumPad keys
  // uint16_t RColour[12] = {Black, Brown, Red, Orange, Yellow, Green2, Blue, Purple, DGrey, White1, Gold, Silver };  // 4 band - 3 band colour
  // float    RVal[12]    = {0,     1,     2,   3,      4,      5,      6,    7,      8,     9,      1/10,  1/100 }; 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (NumKeys)  { if (!Resistor) { for (m=0; m<12; m++) {strcpy(keyLabel[m], NumkeysX[m]);}
                                   for (m=0; m<12; m++) {keyColor[m] = Black;} }
                  if (Resistor)  { for (m=0; m<12; m++) {strcpy(keyLabel[m], NumkeysX[m]);}
                                   if (Numkeys123==0) for (m=0; m<12; m++) {keyColor[m] = RColour[RXlat[m]];} 
                                                 else for (m=0; m<12; m++) {keyColor[m] = Black;} }  }
  
  KeyBrdsDone:

  b = Layout - 1;                               // reset b to 0 - 3
              
  for (row = 0 + rowcount; row < rows; row++) {   // 3 rows 0,1,2 => b = 0+(0*4)=0 1+(0*4)=1 ... 0+(1*4)=4 ....
    for (col = 0; col < 4; col++) {               // 4 columns 0,1,2,3
        b = col + row * 4;
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X), KEY_Y + row * (KEY_H + KEY_SPACING_Y), 
                                               KEY_W, KEY_H, OutlineColor, keyColor[b], KeyFont, keyLabel[b], KEY_TEXTSIZE);
        key[b].drawButton();  }  }

   ////////////////////////////////////////////////////////////////////////////////
   // 5 Small Config Buttons between 1 st and 3rd row Red Blue Green SkyBlue Gold
   ////////////////////////////////////////////////////////////////////////////////
   b = Layout - 1;                     // reset b to 0 - 3 
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
  pixels.fill(ledIndicator & KEYBOARD_LED_CAPSLOCK ? 0xff0000 : 0x000000); //Red if Capslock
  pixels.show();
  
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
{ if (Cancel) { VolDisable = true;     
                MuteDisable = true;    
                ToneControls = false;
                return; }

  if (MediaConfig[0]>1) VolDisable = false; 
  if (MediaConfig[0]>2) MuteDisable = false;
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
{ File f1 = LittleFS.open("Layout", "w"); 
  f1.write(SaveLayout, 1); 
  f1.close();  
}
////////////////////////
void ReadLayoutConfig()
////////////////////////
{
  File f1 = LittleFS.open("Layout", "r"); 
  f1.read(SaveLayout, 1); 
  f1.close();  
  Layout = SaveLayout[0]; if (Layout==0) Layout = 2;;   //  Layout = 1 to 4 or 2 if SaveLayout[0] = 0    
}
///////////////////////
void SaveOS()
///////////////////////
{ File f1 = LittleFS.open("OptionOS", "w"); 
  f1.write(OptionOS, 1); 
  f1.close();  
}
////////////////////////
void ReadOS()
////////////////////////
{
  File f1 = LittleFS.open("OptionOS", "r"); 
  f1.read(OptionOS, 1); 
  f1.close();      
}
/////////////////////////////////////////////////////
void SaveLinks() // each 480 bytes - default is off
/////////////////////////////////////////////////////
{ int n = 0, m = 0;

  File f1 = LittleFS.open("LinkM", "w"); f1.write(LinkM, 24); f1.close(); 
  File f2 = LittleFS.open("LinkNameM", "w"); for (n=0; n<24; n++) for (m=0; m<10; m++) f2.write(LinkNameM[n][m], 4); f2.close();    
  File f3 = LittleFS.open("LinkS", "w"); f3.write(LinkS, 24); f3.close(); 
  File f4 = LittleFS.open("LinkNameS", "w"); for (n=0; n<24; n++) for (m=0; m<10; m++) f4.write(LinkNameS[n][m], 4); f4.close(); 
  File f5 = LittleFS.open("LinkT", "w"); f5.write(LinkT, 24); f5.close(); 
  File f6 = LittleFS.open("LinkNameT", "w"); for (n=0; n<24; n++) for (m=0; m<10; m++) f6.write(LinkNameT[n][m], 4); f6.close(); 
}
///////////////////////////////////////////////////
void ReadLinks() // each 480 bytes - default is on
///////////////////////////////////////////////////
{ int m = 0, n = 0;
    
  File f1 = LittleFS.open("LinkM", "r"); f1.read(LinkM, 24); f1.close();                 
  File f2 = LittleFS.open("LinkNameM", "r"); for (n=0; n<24; n++) for (m=0; m<10; m++) f2.readBytes((char *)LinkNameM[n][m], 4); f2.close();  
  File f3 = LittleFS.open("LinkS", "r"); f3.read(LinkS, 24); f3.close();
  File f4 = LittleFS.open("LinkNameS", "r"); for (n=0; n<24; n++) for (m=0; m<10; m++) f4.readBytes((char *)LinkNameS[n][m], 4); f4.close();   
  File f5 = LittleFS.open("LinkT", "r"); f5.read(LinkT, 24); f5.close();
  File f6 = LittleFS.open("LinkNameT", "r"); for (n=0; n<24; n++) for (m=0; m<10; m++) f6.readBytes((char *)LinkNameT[n][m], 4); f6.close(); 
}
///////////////////////////////////////////////////////////////
void InitCfg(bool Option)    // Only 1 on cold start or reboot
///////////////////////////////////////////////////////////////
{ char *ChrPtr;
  String NameStr;
  byte b, *BPtr;
  int ASize =0, m = 0, n = 0;
  uint16_t ByteLen = 0;
  
  if (Option) {
      if (LittleFS.exists("Layout"))            ReadLayoutConfig();     else Layout        = 2;       // Selected Saved Layout 1 - 4 restored
      if (LittleFS.exists("OptionOS"))          ReadOS();               else OptionOS[0]   = 0;       // Windows = 0 = default
      if (LittleFS.exists("NoKeySkip"))         KeySkip       = false;  else KeySkip       = true;    // When dimmed skip first keypress
      if (LittleFS.exists("CheckSerial"))       CheckSerial   = true;   else CheckSerial   = false;   // Check COM port in main loop 
      if (LittleFS.exists("MouseCfg"))          ReadMouse();
      if (LittleFS.exists("MediaCfg"))          ReadMediaConfig(); 
      if (LittleFS.exists("MuteDisable"))       MuteDisable   = false;  else MuteDisable   = true;    // L1-L4 enabled = default VolMute Key disabled 
      if (LittleFS.exists("VolDisable"))        VolDisable    = true;   else VolDisable    = false;   // V+ V- disabled Delete-Return enabled
      if (LittleFS.exists("MediaKeys"))         Media         = true;   else Media         = false;   // Media Keys in Layout 2 restored
      if (LittleFS.exists("Brightness"))        Brightness    = true;   else Brightness    = false;   // *br* = brightness up/dwn in M S T repace volume
      if (LittleFS.exists("DoCal"))             DoCal         = true;   else DoCal         = false;   // 1 -> Run calibration on start
      if (LittleFS.exists("Rotate180"))         Rotate180     = true;   else Rotate180     = false;   // Set display orientation to 1 or 3 (180 degrees different)
      if (LittleFS.exists("KeyFontBold"))       KeyFontBold   = true;   else KeyFontBold   = false;   // Button Font Bold/Normal labels
      if (LittleFS.exists("SRLinks"))           SaveReadLinks = true;   else SaveReadLinks = false;   // Save and Read Macro Links
   
      if (LittleFS.exists("LayerAD"))           { ChrPtr = LayerADFile; DoFileStrings(0, "LayerAD", ChrPtr); LayerAD  = LayerADFile[0] - 65; }       
      if (LittleFS.exists("DeleteBackspace"))   { ChrPtr = BsDNumFile; DoFileStrings(0, "DeleteBackspace", ChrPtr); BsDNum  = BsDNumFile[0] - 65; }  

      if (LittleFS.exists("XFiles"))            XFiles        = true;   else XFiles        = false;   // x1-x6 keys new actions active
      if (LittleFS.exists("x1x6"))              Readx1x6();                                           // always read if exists

      if (LittleFS.exists("KeyFontColour"))    {KeyFontColour = true;   KeyFont = Black; }   
                                         else  {KeyFontColour = false;  KeyFont = White; }   }        // Button Font Bold/Normal labels  
  
  for (n=0; n<24; n++) 
      {NameStr = STRname[n]; BPtr = Str1to12[n]; MacroS1S12[n] = 0;               // ByteSize is max - ok DoFileBytes read until eof   ###
       if (LittleFS.exists(NameStr)) { MacroSizeS1S12[n] = DoFileBytes(0, NameStr, BPtr, ByteSize);  MacroS1S12[n] = 1; } } // 0 = read 1 = write
           
  for (n=0; n<24; n++) 
      {NameStr = TTRname[n]; BPtr = Ttr1to12[n];  MacroT1T12[n] = 0;              // ByteSize is max - ok DoFileBytes read until eof    
       if (LittleFS.exists(NameStr)) { MacroSizeT1T12[n] = DoFileBytes(0, NameStr, BPtr, ByteSize); MacroT1T12[n] = 1; } }
           
  for (n=0; n<24; n++) 
      {NameStr = MTRname[n]; BPtr = Mtr1to12[n];  MacroM1M12[n] = 0;              // ByteSize is max - ok DoFileBytes read until eof         
       if (LittleFS.exists(NameStr)) { MacroSizeM1M12[n] = DoFileBytes(0, NameStr, BPtr, ByteSize); MacroM1M12[n] = 1; }  } 
  
  if (LittleFS.exists("inputStr")) {ChrPtr = inputString; DoFileStrings(StrOK, "inputStr", ChrPtr);}

  if (LittleFS.exists("LCDBlankTime"))  {ReadLCDBlank();}
  if (LittleFS.exists("LCDDimmer"))     {ReadLCDDim();}
  if (LittleFS.exists("LCDBrightness")) {ReadLCDBright();} 

  if (LittleFS.exists("tRestart"))  { timeRestart  = ReadPowerKeys(0);}
  if (LittleFS.exists("TRestart"))  { TimeRestart  = ReadPowerKeys(1);}
  if (LittleFS.exists("tPowerOff")) { timePowerOff = ReadPowerKeys(2);}
  if (LittleFS.exists("TPowerOff")) { TimePowerOff = ReadPowerKeys(3);}

  if (LittleFS.exists("tRepeat")) { timeRepeat  = ReadMacroTimers(0);}
  if (LittleFS.exists("TRepeat")) { TimeRestart = ReadMacroTimers(1);}
  if (LittleFS.exists("tOnce"))   { timeOnceof  = ReadMacroTimers(2);}
  if (LittleFS.exists("TOnce"))   { TimeOnceof  = ReadMacroTimers(3);}

  if (SaveReadLinks) ReadLinks();  // stored by SaveLinks() in GetSysinfo
  // ReadLinks();                       // Save Links occasionally - always read them
  //showKeyData();
}

///////////////////////////////////////////////////////////////////////////////////////
uint16_t DoFileBytes(bool DoWrite, String STRf,  byte *BytePtr, uint16_t ByteArrayLen)
///////////////////////////////////////////////////////////////////////////////////////
{ uint16_t ByteLen = 0;
  int n;
  byte b;
      
  if (!DoWrite)                            // read data
     //BytePtr[0] = 0x00;                    // For test in buttons() M1 - M3
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
      ByteLen = f.size();
      f.close();    }

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
void DisplayClocks(bool DisplayOption)
////////////////////////////////////////////////
{ Serial.println();
  Serial.print("Date and Time <t>: ");
  rtc_get_datetime(&t);
  datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
  Serial.print(datetime_str);
  if (DisplayOption) { status("Date and Time <t>: "); delay(2000); status(datetime_str); delay(3000); }
  Serial.println();

  Serial.print("Alarm Macro [R-C][O-C] <a>: ");
  // rtc_get_datetime(&alarm); // If included this sets alarm = date time
  datetime_to_str(alarm_str, sizeof(alarm_buf), &alarm);
  Serial.print(alarm_str);
  if (DisplayOption) { status("Alarm Macro [R-C][O-C] <a>"); delay(2000); status(alarm_str); delay(3000); }
  Serial.println();

  Serial.print("Alarm Macro [RcT][OcT] <w>: ");
  datetime_to_str(timer_str, sizeof(timer_buf), &alarm);
  Serial.print(timer_str);
  if (DisplayOption) { status("Alarm Macro [RcT][OcT] <w>: "); delay(2000); status(timer_str); delay(3000); }
  Serial.println();

  Serial.print("Alarm Power [R-C][O-C] <p>: ");
  datetime_to_str(power_str, sizeof(power_buf), &alarm);
  Serial.print(power_str);
  if (DisplayOption) { status("Alarm Power [R-C][O-C] <p>: "); delay(2000); status(power_str); delay(3000); }
  Serial.println();  
}
////////////////////////////////////////////////
void GetSysInfo(int Action) // Also save config
//////////////////////////////////////////////// 
{ char *ChrPtr;
  byte *BPtr;
  uint32_t  Bt;
  int StrLen, n = 0, m = 0, b = 0;
  int fHeap = rp2040.getFreeHeap();
  int uHeap = rp2040.getUsedHeap();
  int tHeap = rp2040.getTotalHeap();
  int fCPU  = rp2040.f_cpu();
  
  if (Action>0) 
     { if (SaveLayout[0]>0) { SaveLayoutConfig(); }         //  else LittleFS.remove("Layout");     
       if (Action==2) { LayerADFile[0] = LayerAD + 65; ChrPtr = LayerADFile; DoFileStrings(true, "LayerAD",  ChrPtr); }           
       if (Action==3) { BsDNumFile[0] = BsDNum + 65; ChrPtr = BsDNumFile; DoFileStrings(true, "DeleteBackspace",  ChrPtr); }       
       if (Action==4) { if (XFiles) { Savex1x6(); XFileNum[0] = '1'; ChrPtr = XFileNum;  DoFileStrings(true, "XFiles", ChrPtr); } // x1-x6 top row keys new actions
                            else LittleFS.remove("XFiles"); }
  return; } // if (Action>0)
  
  if (VolDisable) {if (!LittleFS.exists("VolDisable")) {char VolD[2]   = "1"; ChrPtr = VolD;  DoFileStrings(true, "VolDisable",   ChrPtr);} }
      else LittleFS.remove("VolDisable");
      
  if (!MuteDisable) {if (!LittleFS.exists("MuteDisable")) {char MuteD[2]  = "1"; ChrPtr = MuteD; DoFileStrings(true, "MuteDisable",  ChrPtr);} }
       else LittleFS.remove("MuteDisable"); 

  if (Media) {if (!LittleFS.exists("MediaKeys")) {char MediaD[2]  = "1"; ChrPtr = MediaD; DoFileStrings(true, "MediaKeys",  ChrPtr); } }
      else LittleFS.remove("MediaKeys");

  if (OptionOS[0]<3)   { SaveOS(); }           //  else LittleFS.remove("OptionOS"); 

  if (SaveReadLinks) {if (!LittleFS.exists("SRLinks")) {char SRLinks[2]  = "1"; ChrPtr = SRLinks; DoFileStrings(true, "SRLinks",  ChrPtr); } }
      else LittleFS.remove("SRLinks"); 
  
  Serial.println("Version: VolumeMacro120 GPL3 Tobias van Dyk June 2023");
  Serial.printf("CPU MHz: %d\n\r", fCPU);
  Serial.printf("FreeHeap: %d\n\r", fHeap);
  Serial.printf("UsedHeap: %d\n\r", uHeap);
  Serial.printf("TotalHeap: %d\n\r", tHeap);
  Serial.printf("Core temperature: %2.1fC\n\r", analogReadTemp());
  
  Serial.println("Files (Number Name Size):");
  ListFiles();
  
  DisplayClocks(false);
  
  Serial.println();
  ReadLCDBlank();
  Serial.printf("LCD Blank Timeout: %d seconds\n\r", TimePeriod/1000);     // make sure this is never 0 

  ReadLCDDim(); b = (DimVal*100)/255;
  Serial.printf("LCD Dimmed: %d percent (%d/255)\n", b, DimVal);  

  ReadLCDBright(); b = (NormVal*100)/255; if (b==0) b = 100;
  Serial.printf("LCD Brightness: %d percent (%d/255)\n", b, NormVal);
      
  Serial.printf("Restart Time  (seconds): %d %d\n", timeRestart,  TimeRestart);
  Serial.printf("PowerOff Time (seconds): %d %d\n", timePowerOff, TimePowerOff);

  Serial.print("Calibration Data: "); for (int i = 0; i < 5; i++) { Serial.print(calData[i]); if (i < 4) Serial.print(", "); }

  if (SaveReadLinks) SaveLinks();  // Restored by ReadLinks() in InitCfg - makes the [Cfg] [Sav] key respond <much> slower
  
  //showKeyData();   
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

  int i = 6;                                                                    // 3 digits
  while (i!=0) { RPArr[Option][i+12] = (PVal%10) + 48; PVal = PVal/10; i--; };  // modulus (%) and division (/) operations 
  status(RPArr[Option]); SendBytesEnd(1);  
}

/////////////////////////////////////////////////
// Display mini status bar
// STATUS_X 150 // Centred on this 320/2=160
// STATUS_Y 222 // bottom
/////////////////////////////////////////////////
void status(const char *msg) 
/////////////////////////////////////////////////
{  // tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(SkyBlue1, Black); 
  if (Kbrd) tft.setTextColor(Yellow, Black);
  if (Math) tft.setTextColor(White, Black);
  if (PowerKeys) tft.setTextColor(Orange, Black);
  if (sSens||mPlay||tTimeDate) tft.setTextColor(Green, Black);
  
  if (ScrSizeType==1) { tft.setTextPadding(480);  tft.setFreeFont(&FreeSansBold9pt7b); }
                                                     
  if (ScrSizeType==0) { tft.setTextPadding(320);  tft.setFreeFont(&FreeSans9pt7b); }                      
  
  tft.setTextDatum(1);
  tft.drawString(statusBlank, STATUS_X, STATUS_Y); 
  tft.drawString(msg, STATUS_X, STATUS_Y);
  
  if (ScrSizeType==1) { if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
                            else tft.setFreeFont(&FreeSans12pt7b); }
  if (ScrSizeType==0) { if (KeyFontBold) tft.setFreeFont(&FreeSansBold9pt7b);
                            else tft.setFreeFont(&FreeSans9pt7b); }                              
                            
}
////////////////////////////////////
void FillKeysStr(int SelectLayout)
////////////////////////////////////
{ int n, m;
  if (SelectLayout==3||SelectLayout==0)   // Keys S1-S24
  for (n=0; n<24; n++) { MacroS1S12[n] = 1; for (m=0; m<=strlen(str1to12[n]); m++) Str1to12[n][m] = str1to12[n][m]; MacroSizeS1S12[n] = m; }   
  if (SelectLayout==4||SelectLayout==0)   // Keys T1-T24
  for (n=0; n<24; n++) { MacroT1T12[n] = 1; for (m=0; m<=strlen(ttr1to12[n]); m++) Ttr1to12[n][m] = ttr1to12[n][m]; MacroSizeT1T12[n] = m; }
  if (SelectLayout==1||SelectLayout==0)   // Keys M1-M24
  for (n=0; n<24; n++) { MacroM1M12[n] = 1; for (m=0; m<=strlen(str1to12[n]); m++) Mtr1to12[n][m] = str1to12[n][m]; MacroSizeM1M12[n] = m; }
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
/////////////////////////
bool SendBytesStarCodes()
///////////////////////// 
{ unsigned long T, z;  // t = timeclock struct
  uint8_t a, b, n, m, i, d;
  bool UnLink = false;

  if (KeyBrdByte[1]==0x61)    // *a
     {b = KeyBrdByte[4]-48; if (b>5) { status("Add number 1-5"); return false; }  // Use the [ADD]ed number to assign 1-3 (or 0)
      if (KeyBrdByte[2]==0x6d) Bank123[0] = b;                                    // *am*n = Keys M1-M24 macro action select 1-3 
      if (KeyBrdByte[2]==0x73) Bank123[1] = b;                                    // *as*n = Keys S1-S24 macro action select 1-3
      if (KeyBrdByte[2]==0x74) Bank123[2] = b;                                    // *at*n = Keys T1-T24 macro action select 1-3
      File f = LittleFS.open("Bank123File", "w"); if (f) {f.write(Bank123, 3); f.close(); }  
      status("Action Bank 1-5 changed"); return true; }


  // Skip first key pressed in LCD in dimmed state - used to wake LCD - toggle with *ks* - state is saved
  if (KeyBrdByte[1]==0x6b&&KeyBrdByte[2]==0x73)           // *ks* 
     { KeySkip = !KeySkip; if (!KeySkip) { File f = LittleFS.open("NoKeySkip", "w"); 
                                           if (f) {f.print(KeySkip); f.close(); }                          status("Wakeup Key disabled"); }
                                    else { if (LittleFS.exists("NoKeySkip")) LittleFS.remove("NoKeySkip"); status("Wakeup Key enabled");  }
       return true; } 

  // Switch off serial monitor unless *se* toggles CheckSerial - default is off - state is saved
  if (KeyBrdByte[1]==0x73&&KeyBrdByte[2]==0x65)           // *se* 
     { CheckSerial = !CheckSerial; if (CheckSerial) { File f = LittleFS.open("CheckSerial", "w"); 
                                                      if (f) {f.print(CheckSerial); f.close(); }  status("Serial COM port monitor enabled");  }
                                               else { if (LittleFS.exists("CheckSerial")) LittleFS.remove("CheckSerial"); 
                                                                                                  status("Serial COM port monitor disabled"); }
       return true; }                                      

  if (KeyBrdByte[1]==0x6F&&KeyBrdByte[2]==0x73)           // *os* operating system toggle Windows/Linux/PiOS on/off
     { OptionOS[0]++; if (OptionOS[0]>2) OptionOS[0]=0; status(OSName[OptionOS[0]]); return true; }
     
  if (KeyBrdByte[1]==0x6d&&KeyBrdByte[2]==0x31)           // *m1*nn = Scroll move amount 
     {b = KeyBrdByte[4]-48;                               // Use the [ADD]ed number to assign 0 - 10
      if (KeyBrdByteNum>5) b = b*10 + KeyBrdByte[5]-48;   // b = 0, 1-20
      if (b<11) {MouseScrollAmount = b; status("Scroll changed"); return true; } else return false; }

  if (KeyBrdByte[1]==0x6d&&KeyBrdByte[2]==0x32)           // *m2*nn = Cursor move amount 
     {b = KeyBrdByte[4]-48;                               // Use the [ADD]ed number to assign 0 - 99
      if (KeyBrdByteNum>5) b = b*10 + KeyBrdByte[5]-48;   // b = 0, 1-99
      if (b<100) {MouseDelta = b; status("Cursor Move changed"); SaveMouse(); return true; } else return false; }

  /////////////////////////////////////////////////// *e0* to *e4*  Media keys Activated
  if (KeyBrdByte[1]==0x65)                         // MediaConfig[0] = KeyBrdByte[2] - 48;       
     { Media = MediaCfg = true;                    // Assume media modes now on except for *e0*
       
       if (KeyBrdByte[2]==0x30)                   // *e0* restore to normal Mediakeys behaviour
          {Media = MediaCfg = MediaConfig[0] = 0; LittleFS.remove("MediaCfg");  // all false
           status("Media Mode 0");      }         

       if (KeyBrdByte[2]==0x31)                   // *e1* = 4 Media keys
          {MediaConfig[0] = 1; status("Media Mode 1 C");     }

       if (KeyBrdByte[2]==0x32)                  // *e2* = 4 Media keys + Volume
          {MediaConfig[0] = 2; status("Media Mode 2 CV");    }  

       if (KeyBrdByte[2]==0x33)                  // *e3* = 4 Media keys + Volume + Mute
          {MediaConfig[0] = 3; status("Media Mode 3 CVM");   }

       if (KeyBrdByte[2]==0x34)                 // *e4* = 4 Media keys + Volume + Mute + Tone
          {MediaConfig[0] = 4; status("Media Mode 4 CVMT");  } 
          
       if (MediaCfg) SaveMediaConfig(); ConfigButtons(1); return true; }  
           

  if (KeyBrdByte[1]==0x30&&KeyBrdByte[2]==0x52)         // *0R* = Zero R Resistor Colours NumPad Toggle On/Off 
     {Resistor = !Resistor; if (Resistor) status("Resistors active"); else status("Resistors passive"); return true; }

  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x6F)     // *fo* = Key Label Bold Normal Font
     {KeyFontBold = !KeyFontBold; 
      if (KeyFontBold) { status("Key Font Bold is ON"); 
                         File f = LittleFS.open("KeyFontBold", "w"); 
                         if (f) {f.print("1"); f.close(); } 
                         tft.setFreeFont(&FreeSansBold12pt7b); }
                  else { status("Key Font Bold is OFF"); 
                         LittleFS.remove("KeyFontBold"); 
                         tft.setFreeFont(&FreeSans12pt7b); } 
      ConfigButtons(1); SendBytesEnd(1); return true; }

  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x63)     // *fc* = Key Label Font Colour Black White
     {KeyFontColour = !KeyFontColour; 
      if (KeyFontColour) { status("Key Font Colour is Black"); 
                         File f = LittleFS.open("KeyFontColour", "w"); 
                         if (f) {f.print("1"); f.close(); } 
                         KeyFont = Black; }
                  else { status("Key Font Colour is White"); 
                         LittleFS.remove("KeyFontColour"); 
                         KeyFont = White; } 
      ConfigButtons(1); SendBytesEnd(1); return true; }
    
      
   if (KeyBrdByte[1]==0x64&&KeyBrdByte[2]==0x65)    // "*de*" = delete config and macro files
      {status("Files deleted"); DeleteFiles(); ConfigButtons(1); SendBytesEnd(1); return true;  }
      
  if (KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x74)    // "*ct*" = display clocks 4x 1 second delay
      {DisplayClocks(true); return true;  }
  
  if (KeyBrdByte[1]==0x6B&&KeyBrdByte[2]==0x62)     // *kb* = num keyboard turn on
     {NumKeys = !NumKeys; ConfigButtons(1); SendBytesEnd(1); status("NumPad ON/OFF"); return true; }

  if (KeyBrdByte[1]==0x62&&KeyBrdByte[2]==0x72)     // *br* = brightness on/off volume off
     {Brightness = !Brightness; 
     if (Brightness) { status("Brightness ON"); 
                       File f = LittleFS.open("Brightness", "w"); 
                       if (f) {f.print("1"); f.close(); }      }
                else { status("Brightness OFF"); 
                       LittleFS.remove("Brightness");   }
     ConfigButtons(1); SendBytesEnd(1); return true;    }

  if (KeyBrdByte[1]==0x75&&KeyBrdByte[2]==0x61)     // *ua* = unlink all macros
     { for (n=0; n<24; n++) { LinkM[n] = 0; LinkS[n] = 0; LinkT[n] = 0; }               
       status("All Macros unlinked"); return true; } 

  if (KeyBrdByte[1]==0x75&&KeyBrdByte[2]==0x6c)     // *ul* = unlink current Source Key Num
     { UnLink = true;
       if (MST1==0) { if (LinkM[Option1]>0) LinkM[Option1] = 0; else UnLink = false; }
       if (MST1==1) { if (LinkS[Option1]>0) LinkS[Option1] = 0; else UnLink = false; }
       if (MST1==2) { if (LinkT[Option1]>0) LinkT[Option1] = 0; else UnLink = false; } 
       if (UnLink) status("Macros unlinked"); else status("No link found"); return true; }      

  if (KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x6D)     // 63 6D *cm* = copy macros 
     {CopyMacro(); SendBytesEnd(0); return true; }  

  if (KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x6F)     // *ro* = rotate 180 degrees turn on
     {Rotate180 = !Rotate180;                       // Toggle On/Off
     if (Rotate180) 
        {File f = LittleFS.open("Rotate180", "w"); 
         if (f) {f.print(Rotate180); f.close(); } }
     if (!Rotate180) LittleFS.remove("Rotate180");      // no else here must be crystal-clear
     status("Restart 180 degrees rotate calibrate"); 
     LittleFS.remove("TouchCalData"); return true; }    // this forces re-calibration as well     

  if (KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x61)         // *ca* = Calibration On/Off 
     {DoCal = !DoCal; if (DoCal) { status("Calibrate ON - send *ca* OFF"); 
                                   File f = LittleFS.open("DoCal", "w"); if (f) {f.print(DoCal); f.close(); } return true; } 
                            else { status("Calibrate OFF - send *ca* ON"); LittleFS.remove("DoCal"); return true; }  }      
  
  if (KeyBrdByte[1]==0x6c&&KeyBrdByte[2]==0x72)         // *lr* = Save and Read Links from Flash
     {SaveReadLinks = !SaveReadLinks; if (SaveReadLinks) status("Links Save Restore ON"); else status("Links Save Restore OFF"); return true; } 
      
  if (KeyBrdByte[1]==0x6c&&KeyBrdByte[2]==0x64)         // *ld* = Data Links and Macros sent to serial port
     {showKeyData(); status("Links Data Sent"); return true; } 
 
  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x73)         // *fs* = Fill S keys with predefined coded strings
     {FillKeysStr(3); status("[S1-S24] filled"); return true; }
  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x74)         // *ft* = Fill T keys with predefined coded strings
     {FillKeysStr(4); status("[T1-T24] filled");  return true; }
  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x6d)         // *fm* = Fill M keys with predefined coded strings
     {FillKeysStr(1); status("[M1-M24] filled"); return true; }
  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x61)         // *fa* = Fill M S T keys with predefined coded strings
     {FillKeysStr(0); status("[All 1-24] filled");  return true; }      

  if (KeyBrdByte[1]==0x73&&KeyBrdByte[2]==0x73)         // *ss* = Save all S keys to Flash
     {MST1 = 1; for (n=0; n<24; n++) { Option1 = n; SaveMacro(1); } status("[S1-S24] saved"); return true; }                
  if (KeyBrdByte[1]==0x73&&KeyBrdByte[2]==0x74)         // *st* = Save all T keys to Flash 
     {MST1 = 2; for (n=0; n<24; n++) { Option1 = n; SaveMacro(1); } status("[T1-T24] saved"); return true; }
  if (KeyBrdByte[1]==0x73&&KeyBrdByte[2]==0x6d)         // *sm* = Save all M keys to Flash 
     {MST1 = 1; for (n=0; n<24; n++) { Option1 = n; SaveMacro(1); } status("[M1-M24] saved"); return true; }
  if (KeyBrdByte[1]==0x73&&KeyBrdByte[2]==0x61)         // *sa* = Save all M S T keys to Flash
     {for (m=0; m<3; m++) {MST1 = m; for (n=0; n<24; n++) { Option1 = n; SaveMacro(1); } } status("[All MST1-24] saved");  return true; }     
  
  if (KeyBrdByte[1]==0x70&&KeyBrdByte[2]==0x6F)         // *po* = Power Keys Menu Option 
     {PowerKeysMenu = !PowerKeysMenu; if (PowerKeysMenu) status("PowerKeys Menu ON"); else status("PowerKeys Menu OFF"); return true; }

  b = (KeyBrdByte[4]-48);    // b = 0 - 9
  if (b >9) return false;    // do nothing if not
                      
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // z = TimePeriod/1000;  i = 6; while (i!=0) { BlankArr[i+12] = (z%10) + 48; z = z/10; i--; }   // modulus (%) and division (/) operations 
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  if (KeyBrdByte[1]==0x74&&KeyBrdByte[2]==0x62)                        // *tb*num = blank lcd timemout *tb*0,1-9 
      {T = TimePeriod; TimePeriod=tbArray[b];                          // Use tbArrayStr to convert number to string
       z = TimePeriod/1000;  i = 6;                                    // 6 digits
       while (i!=0) { BlankArr[i+12] = (z%10) + 48; z = z/10; i--; }   // modulus (%) and division (/) operations 
       File f = LittleFS.open("LCDBlankTime", "w");
       if (f) {f.print(TimePeriod); f.close(); }                       // Stored as 6 ASCII number bytes such as 49 53 48 48 48 48 = 150000 = 150 sec
       Serial.println(TimePeriod);
       SendBytesEnd(1); status(BlankArr); return true; }  
                                                                                                               
  if (KeyBrdByte[1]==0x64&&KeyBrdByte[2]==0x62)                        // *db*num = dim backlight pwm  *db*0,1-9
      {DimVal=dbArr[b];                                                // 
       for (i=0; i<2; i++) DimArr[i+16] = BBArr[b][i];                 //
       File f = LittleFS.open("LCDDimmer", "w");
       if (f) {f.print(DimVal); f.close(); }                           // Stored as 1-3 ASCII number bytes 48 48 56 = 008 = 8/255 %
       Serial.println(DimVal);
       SendBytesEnd(1); status(DimArr); return true; }      

  if (KeyBrdByte[1]==0x62&&KeyBrdByte[2]==0x62)                        // *bb*num = normal backlight pwm  *bb*0,1-9 0 8/255 - 128/255 % PWM
      {NormVal=bbArr[b];                                               // 0, 8/255 - 128/255 % PWM
      if (NormVal==0) digitalWrite(LCDBackLight, HIGH);                // Backlight Full ON
                else  analogWrite(LCDBackLight, NormVal);              // Backlight Brightness ON
       for (i=0; i<3; i++) NormArr[i+15] = BBArr[b][i];                // 
       File f = LittleFS.open("LCDBrightness", "w");
       if (f) {f.print(NormVal); f.close(); }                          // Stored as 1-3 ASCII number bytes 48 48 56 = 008 = 8/255 %
       Serial.println(NormVal);
       SendBytesEnd(1); status(NormArr); return true; }      
       
  // Power Keys  //
  
   if (KeyBrdByte[1]==0x6F)                                            // o
      { if (KeyBrdByte[2]==0x74)                                       // *ot*num 
           {T = timePowerOff; timePowerOff=b*30;                       // 1 = 30 seconds to 9 = 270 seconds
           if (timePowerOff>600) timePowerOff = T;                     // 10 minutes max
           if (timePowerOff==0) timePowerOff = 600;                    // 10 minutes
           WritePowerKeys(timePowerOff, 2);}                           // "tRestart", "TRestart", "tPowerOff", "TPowerOff"
        if (KeyBrdByte[2]==0x54)                                       // *oT*num 
           {T = TimePowerOff; TimePowerOff=b*1000;                     // 1 = 1000 seconds to 9 = 9000 seconds (150 minutes)
           if (TimePowerOff>60000) TimePowerOff = T;                   // restore to current time
           if (TimePowerOff==0) TimePowerOff = 60000;                  // 1000 minutes max (16.7 hours)
           WritePowerKeys(TimePowerOff, 3) ;} return true; }           // "tRestart", "TRestart", "tPowerOff", "TPowerOff"
      
   if (KeyBrdByte[1]==0x72)                                            // r
      { if (KeyBrdByte[2]==0x74)                                       // *rt*num 
           {T = timeRestart; timeRestart=b*30;                         // 1 = 30 seconds to 9 = 270 seconds
           if (timeRestart>600) timeRestart = T;                       // 10 minutes max
           if (timeRestart==0) timeRestart = 600;                      // 10 minutes
           WritePowerKeys(timeRestart, 0); return true; }              // "tRestart", "TRestart", "tPowerOff", "TPowerOff"
       if (KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x54)                   // *rT*num 
          {T = TimeRestart; TimeRestart=b*1000;                        // 1 = 1000 seconds to 9 = 9000 seconds (150 minutes)
          if (TimeRestart>60000) TimeRestart = T;                      // restore to current time
          if (TimeRestart==0) TimeRestart = 60000;                     // 1000 minutes max (16.7 hours)
          WritePowerKeys(TimeRestart, 1); return true;} }              // "tRestart", "TRestart", "tPowerOff", "TPowerOff" 
             
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Macro Timers 
   // Only set MacroTime and MacroTimer1-4 in button code if MacroTimer key has been pressed after setting an option here
   // Associate Macro with Timer in KeyBrd Macro Tools Page with Key [Tmr] if 1 2 3 4 have been added to display bottom then 
   // Source Number Dest Number is associated with (1)(2) Repeat,(3)(4) Once of,(5) Clock or (8) pure repeat
   // 32 bits (4 bytes)  0 to 4,294,967,295 -> maximum value about 4000 x 10800000
   //                                    3 hours     30    60    90 sec  2 min   3 min   5 min  10 min   30 min  1 hour
   //  long unsigned int mtArray[10] = {10800000,  30000, 60000, 90000, 120000, 180000, 300000, 600000, 1800000, 3600000 };
   // x 100                              300 hrs     2 hrs    3 hrs    5 hrs     6 hrs     10 hrs    12 hrs   24 hrs     48 hrs     1 week
   //  long unsigned int mTArray[10] = {10800000,  7200000, 10800000, 18000000, 21600000, 36000000, 43200000, 86400000, 172800000, 604800000 };
   //                        012345678901234567890123456
   //  char mTArr[]       = "Oneshot Time               ";
   //  char rTArr[]       = "Repeat Time                ";
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if (KeyBrdByte[1]==0x6d)                                            // m = macro onceof timers
      { if (KeyBrdByte[2]==0x74)                                       // *mt*num 
           {T = timeOnceof; timeOnceof = mtArray[b];         
           if (timeOnceof>mtArray[0]) timeOnceof = T;                  //  
           if (timeOnceof==0) timeOnceof = T;                          // no change
           WriteMacroTimers(timeOnceof, 2, b);}                        // "tRepeat", "TRepeat", "tOnce", "TOnce"
        if (KeyBrdByte[2]==0x54)                                       // *mT*num 
           {T = TimeOnceof; TimeOnceof = mTArray[b];                   //  
           if (TimeOnceof>mTArray[0]) TimeOnceof = T;                  // restore to current time
           if (TimeOnceof==0) TimeOnceof = T;                          //  
           WriteMacroTimers(TimeOnceof, 3, b) ;} return true; }        // "tRepeat", "TRepeat", "tOnce", "TOnce"
      
   if (KeyBrdByte[1]==0x6e)                                            // n = macro repeat timers
      { if (KeyBrdByte[2]==0x74)                                       // *nt*num 
           {T = timeRepeat; timeRepeat = mtArray[b];                   // 
           if (timeRepeat>mtArray[0]) timeRepeat = T;                  //  
           if (timeRepeat==0) timeRepeat = T;                          //  
           WriteMacroTimers(timeRepeat, 0, b); return true; }          // "tRepeat", "TRepeat", "tOnce", "TOnce"
       if (KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x54)                   // *nT*num 
          {T = TimeRepeat; TimeRepeat= mTArray[b];                     //  
          if (TimeRepeat>mTArray[0]) TimeRepeat = T;                   // restore to current time
          if (TimeRepeat==0) TimeRepeat = 60000;                       //  
          WriteMacroTimers(TimeRepeat, 1, b); return true;} }          // "tRepeat", "TRepeat", "tOnce", "TOnce" 

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //               0 12 34 56 7 89 01 -> copy to RecBytes [1] to [11]
  // Use serial *tx* yy mm dd w hh mm> 22110341200 12:00am 3 Nov 2022 Thursday where x = t,a,p,w
  // t = Main Time/Clock a  = Macro Clock Repeat-Oneshot [R-C][O-C] p = Macro Clock Countdown [RcT][OcT] w = Power Clock [O-C][R-C]
  // datetime_t t = { .year  = 2022, .month = 11, .day   = 03, .dotw  = 4, .hour  = 14,  .min   = 00, .sec   = 00 };
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (KeyBrdByte[1]==0x74)                                            // t Time 
      {for (n=1;n<12;n++) RecBytes[n] = KeyBrdByte[n+3];
      if (KeyBrdByte[2]==0x74) {GetTimeData(&t); rtc_set_datetime(&t); TimeSet = true; return true;} // *tt* Main TimeClock Set          
      if (KeyBrdByte[2]==0x61) {GetTimeData(&alarm); return true;}                                   // *ta* [R-C][O-C]
      if (KeyBrdByte[2]==0x70) {GetTimeData(&timer); return true;}                                   // *tp* [RcT][OcT]
      if (KeyBrdByte[2]==0x77) {GetTimeData(&power); return true;}  }                                // *tw* [O-C][R-C]

   if (KeyBrdByte[1]==0x78)                                 // *xn* n=1-6 x1-x3 keys M x1 - x3 S T x4 - x6 keys
      { b = KeyBrdByte[4]-48;                               // Use the [ADD]ed number to assign 0 - 20
        if (KeyBrdByteNum>5) b = b*10 + KeyBrdByte[5]-48;   // b = 0, 1-20
        n = KeyBrdByte[2]-48;                               // n = 1 - 6
        if (b>BSDMax || n>6) return false;                  // NIR
        XNum[n-1] = b-1;  
        if (b>0) XFiles = true; else XFiles = false; 
        GetSysInfo(4); return true; }
      
   return false;
}

//////////////////////////////////////////////////////////////////////////////////
void SendBytesEnd(bool All)
//////////////////////////////////////////////////////////////////////////////////
{ int n;
  KeyBrdByteNum = 0; KBDispPos = 0;  KeyBrdX = 3; KeyBrd123 = 0;
  for (n = 0; n < 200; n++) { KeyBrdByte[n] = 0x00; KBDisp[n] = '\0'; }
  if (All) {Kbrd = false;} 
  KBType = false; Fx = false; KBrdActive = false; 
  optionsindicators(0); 
}
/////////////////////////////////////////////////////////////////////////////////////////
void SendBytes()    
////////////////////////////////////////////////////////////////////////////////////////        
// Send and Save KeyBrd Macro Editor macros 01 - 99,00
// Destination is Option 2 i.e. [Dst] = rightmost number in KeyBrd 
// M S T A is also known but only saved to M S T when [Up] key pressed (only 24 keys max)
// If destination is A then saved to a00-a99 in here (100 keys max if A selected)
///
// Watch out for Caplock or Shift states during [EXE] phase
// They do not affect the characters during [ADD] phase
// This is why there are separate lower case and uppercase pages
// These will change a -> A or A -> a and Numbers to symbols such as 9 -> ( and 0 -> )
////////////////////////////////////////////////////////////////////////////////////////
{ uint8_t keycode[6]; // simultaneous keys pressed in here
  uint8_t a, b, n, i, MST2A;
  //                 0123456789012345678
  char status1[] = {"Press [Up] Save a00"} ;   // saved as a00-a99
  String NameStr;
  
  for (n=0; n<6; n++) keycode[n] = 0x00;
  
  // Check for special commands Start with * eg *ab*n n = 0-9 - ignore * codes if double **
  if ((KeyBrdByte[0]==0x2A)&&(KeyBrdByte[1]!=0x2A)) { if (SendBytesStarCodes()) return; else {status("*Code incorrect"); return; } }  
  if ((KeyBrdByte[0]==0x2A)&&(KeyBrdByte[1]==0x2A)) { for (n = 0; n < KeyBrdByteNum; n++) KeyBrdByte[n] = KeyBrdByte[n+1]; KeyBrdByteNum--; }
  
  MST2A = Option2 + 24*(MST2==1) + 48*(MST2==2);   // M to a01-a24 S to a25-a48 t to a49-a72
  NameStr = KBMacro[MST2A];                        // Option2 KeyBrd Destination = 7 will save to a08
  if (MST2A<10) {status1[18] = MST2A + 49;} else {status1[17] = (MST2A/10) + 48; status1[18] = MST2A - 10*(MST2A/10) + 49;}  // Display Option2 nn 
    
  // sequential via keyPress 
  if ((!Fx)&&(KeyBrdByte[0]<0x80))                                              // F1-F24 must be sent with keycode not keyPress
     { for (n = 0; n < KeyBrdByteNum; n++) if (KeyBrdByte[n]<0x80)              // When 1st char is normal alphanumeric all must be <0x80    
           {usb_hid.keyboardPress(HIDKbrd, KeyBrdByte[n]); delay(keydelay);     // Converted via end of hid.h from ascii to hid
            usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay); 
            MacroBuff[n] = KeyBrdByte[n];}
       File f = LittleFS.open(NameStr, "w");
       if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }         
       status(status1); MacroBuffSize = KeyBrdByteNum; DoUpKey = true; }

  // simultaneous via keycode 
  for (n=0; n<6; n++) keycode[n] = 0x00;       
  if ((Fx)||(KeyBrdByte[0]>0x7F)&&(KeyBrdByteNum<6))                            // Fnn = 3A-45 + 68-73 hex
     { for (n = 0; n < KeyBrdByteNum; n++) {keycode[n] = KeyBrdByte[n]; MacroBuff[n] = KeyBrdByte[n]; }  
           usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
           usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay);  
       File f = LittleFS.open(NameStr, "w");
       if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }  
       status(status1); MacroBuffSize = KeyBrdByteNum; DoUpKey = true; }
  
  // Note also saves: DoFileBytes(1, M,S,T TRname[Option2], BPtr, MacroBuffSize[Option2]) in buttons() case [Up] 

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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// int LinkM[24] LinkS[24] LinkT[24]  
// 0 - 9 Number of linked macro's for each Key 1-24
// char LinkNameM[24][10][9] = { 0 };  
// File names on Flash of linked macro's -> a01, a02
// [24] keys for each M S T [10] linked macrofilenames length 8 chars  0x00
// Linked filenames always aNN NN = 01 - 99
// Link M to a01-a24 S to a25-a48 t to a49-a72
// Chained linking is always additive - it does not ovewrite previous link but adds an additional link
// To overwrite first remove links using *ul* on source macro
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool LinkMacro()   // Link M to a01-a24 S to a25-a48 t to a49-a72
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Two types of Repeat - not yet implemented
// (1) Repeat as in MacroM1M12[24] = 0.1.2.3. etc - applied to all max 10 macros per key - set with star code *rm*01-12rr *rs*01-12rr 
//     *rm*01-12rr rr = repeat 1-99. 
// (2) Repeat individual linked macro itself for each of 10 links for each key for each group M S T -use up one of 10 links
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////           
{ byte r, b = 0;
  bool LinkOK = false;
  bool RepeatMacro = false; // Repeat the whole key with all 10 linked macros
      
  if (KeyBrdByteNum>=1) b = KeyBrdByte[0]-49;         // Use the [ADD]ed number to assign links to Source Key M S T Number
  if (KeyBrdByteNum>1)  b = b*10 + KeyBrdByte[1]-49;  // instead of using Source key Number = 1  

  if (MST1>2) { status("Set Source = M S T not A"); return false; }  // Can set MST2 = A to bypass something later

  if ((MST1==MST2)&&(Option1==Option2)) RepeatMacro = true;  // Repeat MST1 = M S T Option1 = 1-24 
  
  if (MST1==0) { if (LinkM[b]==0) strcpy(LinkNameM[b][LinkM[b]],   KBMacro[Option1]);  // Link M01-M24 to a01-a24 
                                  strcpy(LinkNameM[b][LinkM[b]+1], KBMacro[Option2]); 
                 LinkM[b]++; if (MacroM1M12[b]==0) MacroSizeM1M12[b] = MacroM1M12[b] = 1; LinkOK = true; }

  if (MST1==1) { if (LinkS[b]==0) strcpy(LinkNameS[b][LinkS[b]],   KBMacro[Option1+24]);  // Link S01-S24 to a25-a48 
                                  strcpy(LinkNameS[b][LinkS[b]+1], KBMacro[Option2+24]); 
                 LinkS[b]++; if (MacroS1S12[b]==0) MacroSizeS1S12[b] = MacroS1S12[b] = 1; LinkOK = true; }

  if (MST1==2) { if (LinkT[b]==0) strcpy(LinkNameT[b][LinkS[b]],   KBMacro[Option1+48]);  // Link T01-S24 to a49-a72
                                  strcpy(LinkNameT[b][LinkS[b]+1], KBMacro[Option2+48]); 
                 LinkT[b]++; if (MacroT1T12[b]==0) MacroSizeT1T12[b] = MacroT1T12[b] = 1; LinkOK = true; }
   
  if (RepeatMacro) status("Repeat Macro included"); 
  else if (LinkOK) status("Macro's linked"); else status("Macro's not found");  
  
  return LinkOK;  // MST1 = 4 undefined  
}
/////////////////////////////////////////////////////////////
bool RenameMacro() // LittleFS.rename(pathFrom, pathTo) 
/////////////////////////////////////////////////////////////
// Must restart for new filemacro to be copied to M S T 1-24
/////////////////////////////////////////////////////////////
{ String NameStr1, NameStr2;
  bool RenOK = false;

  if (MST1==3) NameStr1 = KBMacro[Option1]; if (MST1==0) NameStr1 = MTRname[Option1];
  if (MST1==1) NameStr1 = STRname[Option1]; if (MST1==2) NameStr1 = TTRname[Option1];

  if (MST2==3) NameStr2 = KBMacro[Option2]; if (MST2==0) NameStr2 = MTRname[Option2];
  if (MST2==1) NameStr2 = STRname[Option2]; if (MST2==2) NameStr2 = TTRname[Option2];

  RenOK = LittleFS.rename(NameStr1, NameStr2);    // Rename 1 to 2 for M S T files and a00-a99
  
  if (RenOK) status("Macro renamed Src -> Dst"); else status("Rename Macro's not found");
  return RenOK;  // MST1 = 4 undefined  
}
////////////////////
bool RemoveMacro()
////////////////////
{ String NameStr;
  bool RemOK = false, RemA = false;  // Should also remove all a01-a72 files?
  
  if (MST1==3) { NameStr = KBMacro[Option1];    RemA  = LittleFS.remove(NameStr); }    // Delete Source File a00-a99
  if (MST1==0) { NameStr = MTRname[Option1];    RemOK = LittleFS.remove(NameStr); 
                 NameStr = KBMacro[Option1];    RemA  = LittleFS.remove(NameStr); }    // Delete Read Source File M1 - M24               
  if (MST1==1) { NameStr = STRname[Option1];    RemOK = LittleFS.remove(NameStr); 
                 NameStr = KBMacro[Option1+24]; RemA  = LittleFS.remove(NameStr); }    // Delete Read Source File S1 - S24  
  if (MST1==2) { NameStr = TTRname[Option1];    RemOK = LittleFS.remove(NameStr); 
                 NameStr = KBMacro[Option1+48]; RemA  = LittleFS.remove(NameStr); }    // Delete Read Source File T1 - T24 
  
  if (RemOK||RemA) status("Source macro deleted"); else status("Source macro not found");
  return RemOK||RemA;  // MST1 = 4 undefined  
}
/////////////////////////////////////////////////////////////////////
// To copy Macro1-99 to Keys M1-24,S1-24,T1-24 
// Compose *cm*nnXmm via [ADD] where nn = 01-99 X = M S T mm = 01-24
//              012345678
// KeyBrdByte = *cm*nnXmm  = KBDisp
// OR just use *cm* will then copy using current settings displayed
// via MST1, MST2, Option1, Option2
// byte MacroBuff[ByteSize];
// int  MacroBuffSize = ByteSize;
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool SendMacro()  
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Different from [EXE] - no [Up]key save option - already saved to a file
// Cannot list macros here without a file - for example those loaded using *fm,s,t*
// Because DoFileBytes(0, NameStr will not find file NameStr
// Works on macros loaded using serial port - they are saved to a file such as <3This is Key S3>
/////////////////////////////////////////////////////////////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 };      // simultaneous keys pressed in here
  String NameStr;
  byte *BPtr;
  int n;
  int MSTLayout[5] = {1,3,4,2,2};                                     // Convert MST to Layout if Layout = 2
 
  Layout = MSTLayout[MST1];                                           // Use [Src][Num] Macro detail in KeyBrd

  if (Layout!=2) { MacroKeys(Option1, 0); Layout = 2; return true; }  // Send M S T Option1 for 1 x execute                                                                        

  Layout = 2;                                                         // M S T keys = Layouts 1, 3, 4, will return if executed
 
  NameStr = KBMacro[Option1];         // A - Use files a01 - a99 
  BPtr = MacroBuff; MacroBuffSize = DoFileBytes(0, NameStr, BPtr, ByteSize); if (MacroBuffSize==0) { status("Macro's not found"); return false; }
                  
  if (MacroBuff[0]<0x80) { n = 0; while (MacroBuff[n]!=0) {usb_hid.keyboardPress(HIDKbrd, MacroBuff[n]);  delay(keydelay2);
                                                           usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); 
                                                           n++; if (n>ByteSize) break; } return true; }
  if (MacroBuff[0]>0x7F) { for (n=0;  n<6; n++) {keycode[n] = MacroBuff[n]; if (keycode[n]==0) break; }    // allow one 0x00 in keycode
                                                 usb_hid.keyboardReport(HIDKbrd, 0, keycode);              delay(keydelay2); 
                                                 usb_hid.keyboardRelease(HIDKbrd);                         delay(keydelay2); 
                                                 return true; }                                            // if (Layout==2) 
 return false;
}
////////////////////////////////////////////////////////////////////////////// 
void CopyMacro()    // Source Macro M S T A must already be saved to a file
                    // Destination Macro can be M S T or A
                    // Can also use *cm*nnXmm
////////////////////////////////////////////////////////////////////012345678/
//                             012345678901234
{ char KeyBrd5status[16] =   {"Macro nn -> Xnn"};
  int Xlat1[5] = {6,7,12,13,14};
  int MST = 4; // Later assigned to MST1 or MST2
  String NameStr;
  byte *BPtr;
  int n;
  bool StarCMOK = false;
  
  MST = MacroBuffSize = 0;
 
  if (KeyBrdByteNum>4) { MST = KeyBrdByte[6]-77; if (MST>0) MST = MST-5; if (MST>2) return;   // *cm*nnXmm MSTA = 0 1 2 3 - 4 notdef
                         for (int n=0; n<5; n++)  KeyBrd5status[Xlat1[n]]  = KeyBrdByte[n+4]; // 012345678 S=83 83-77=6->=1 MST2
                         Option1 = (KeyBrdByte[4]-48)*10 + KeyBrdByte[5]-49;                  // Option1 = 0 - 23 = macro a01 - a99
                         Option2 = (KeyBrdByte[7]-48)*10 + KeyBrdByte[8]-49;                  // Option2 = 0 - 23 keys M S T = 1 - 24
                         if (Option1>99 || Option2>12) return;                                // 
                         MST1 = 3; MST2 = MST; StarCMOK = true; }                             // Copy from a01-a99 to M S T 01-24                          
  
  if (MST1==3) { NameStr = KBMacro[Option1]; }   // A - Use files a01 - a99                         
  if (MST1==0) { NameStr = MTRname[Option1]; }   // M - Use files m01 - m24
  if (MST1==1) { NameStr = STRname[Option1]; }   // S
  if (MST1==2) { NameStr = TTRname[Option1]; }   // T

  BPtr = MacroBuff; MacroBuffSize = DoFileBytes(0, NameStr, BPtr, ByteSize);   // Read source macro from Flash
  if (MacroBuffSize==0) { status("Macro's not found"); return; }   

  if (MST2==3) { DoFileBytes(1, KBMacro[Option2], BPtr, MacroBuffSize); }      // Save Destination a01-a99 - this may remove 1:1 M->a etc assoc

  if (MST2==0) { for (n=0; n<MacroBuffSize; n++) { Mtr1to12[Option2][n] = MacroBuff[n]; } MacroM1M12[Option2] = 1; BPtr = Mtr1to12[Option2];  
                      MacroSizeM1M12[Option2] = MacroBuffSize; DoFileBytes(1, MTRname[Option2], BPtr, MacroBuffSize); }  // Save Destination File

  if (MST2==1) { for (n=0; n<MacroBuffSize; n++) { Str1to12[Option2][n] = MacroBuff[n]; } MacroS1S12[Option2] = 1; BPtr = Str1to12[Option2];  
                      MacroSizeS1S12[Option2] = MacroBuffSize; DoFileBytes(1, STRname[Option2], BPtr, MacroBuffSize); }  // Save Destination File  

  if (MST2==2) { for (n=0; n<MacroBuffSize; n++) { Ttr1to12[Option2][n] = MacroBuff[n]; } MacroT1T12[Option2] = 1; BPtr = Ttr1to12[Option2];
                      MacroSizeT1T12[Option2] = MacroBuffSize; DoFileBytes(1, TTRname[Option2], BPtr, MacroBuffSize); }  // Save Destination File

  if ((StarCMOK)&&(MacroBuffSize>0)) { status((char *)KeyBrd5status); return; }
  if ((MST!=4)&&(MacroBuffSize>0))     status("Macro Copied check file list"); else status("Macro's not found");
                 
}
///////////////////////////////////////////////////////////////////////////////////////////
// MST1 MST2  used to define source and destination macro for copy and/or linking/chaining
// Source = A (mAcro00-mAcro99) or Keys M S T 1-24 for copy or A for linking
// Destination = Keys M S T 1-24 for copy or A for linking
///////////////////////////////////////////////////////////////////////////////////////////
int GetMST(byte b)                       // MST = 0 1 2 3 4  = M S T A undef keys
/////////////////////////////////////////////////////////////////////////////////////
{ 
  int MST = 4; // On return assigned MST1 or MST2
  // MST = b-62; if (MST!=3) MST = MST-12; if (MST>0) MST = MST-6;  
  if (b==0x4D) MST = 0; // 0x4D=77 Source = M Keys 1-24
  if (b==0x53) MST = 1; // Source = S Keys 1-24 
  if (b==0x54) MST = 2; // Source = T Keys 1-24 
  if (b==0x41) MST = 3; // Source = A = macro01-macro99 
  return MST;
}
///////////////////////////////////////////////////////////////////// 
void DoSourceMacro()    
/////////////////////////////////////////////////////////////////////
{ byte a, b;
   if (KeyBrdByteNum==0) {MST1++; if (MST1>3) MST1=0; }  // Do not use MST1 = 4 = undefined as an option
   
  if (KeyBrdByteNum>0)
     {a = KeyBrdByte[0]; b = a - 48;      
      if (KeyBrdByteNum==3)             { Option1 = (KeyBrdByte[1]-48)*10 + KeyBrdByte[2]-49; MST1 = GetMST(a); }
      if ((KeyBrdByteNum==2)&&(b<10))   Option1 = b*10 + KeyBrdByte[1]-49;
      if ((KeyBrdByteNum==2)&&(b>9))    { Option1 = KeyBrdByte[1]-49;                         MST1 = GetMST(a); }
      if ((KeyBrdByteNum==1)&&(b<10))   Option1 = b; 
      KeyBrdByteNum=0; }                  // Else [Src][Dst][Num] ignored
    
 NumMode = 1;
 optionsindicators(0);
}
///////////////////////////////////////////////////////////////////// 
void DoDestMacro()    
/////////////////////////////////////////////////////////////////////
{ byte a, b;
   if (KeyBrdByteNum==0) {MST2++; if (MST2>3) MST2=0; }  // Do not use MST2 = 4 = undefined as an option
 
  if (KeyBrdByteNum>0)
     {a = KeyBrdByte[0]; b = a - 48;      // Option2 = 0 - 11 = macro 1 - 99  
      if (KeyBrdByteNum==3)             { Option2 = (KeyBrdByte[1]-48)*10 + KeyBrdByte[2]-49; MST2 = GetMST(a); }
      if ((KeyBrdByteNum==2)&&(b<10))   Option2 = b*10 + KeyBrdByte[1]-49;
      if ((KeyBrdByteNum==2)&&(b>9))    { Option2 = KeyBrdByte[1]-49;                         MST2 = GetMST(a); }
      if ((KeyBrdByteNum==1)&&(b<10))   Option2 = b; 
      KeyBrdByteNum=0; }                  // Else [Src][Dst][Num] ignored
          
 NumMode = 2;
 optionsindicators(0);
}
///////////////////////////////////////////////////////////////////// 
void DoNumMacro()    
/////////////////////////////////////////////////////////////////////
{ byte a, b;
  int Option12; // Assigned to Option1/2
  
  if (KeyBrdByteNum==0) {if (NumMode==1) { Option1++; 
                             if (MST1<3)  if (Option1>23) Option1=0;          
                             if (MST1==3) if (Option1>98) Option1=0; 
                             status("Source Macro Number");  }  
                         if (NumMode==2) { Option2++; 
                             if (MST2<3)  if (Option2>23) Option2=0;
                             if (MST2==3) if (Option2>98) Option2=0; 
                             status("Destination Macro Number");} }
  
  if (KeyBrdByteNum>0)
     {a = KeyBrdByte[0]; b = a - 48;      
      if (KeyBrdByteNum==3)             { return; }
      if ((KeyBrdByteNum==2)&&(0<b<10))   Option12 = b*10 + KeyBrdByte[1]-49;
      if ((KeyBrdByteNum==2)&&(b>9))    { return; }
      if ((KeyBrdByteNum==1)&&(0<b<10))   Option12 = b;    
      if (NumMode==1) { Option1 = Option12; status("Source Macro Number"); }
                  else{ Option2 = Option12; status("Destination Macro Number"); }
      KeyBrdByteNum=0; }                  // Else [Src][Dst][Num] ignored      

 optionsindicators(0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetTimerMacro ()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// char TimerName[8][10] = {"Macro00","Macro00","Macro00","Macro00","Macro00","Macro00","Macro00","Macro00"}; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ byte a, b, c, n;

  // MacroTimerOK = false; // Set true if both Macro and Timer assigned - can check TimerDisp[6]!='X' for Macro and TimerDisp[17]!=' '
  
  b = 0; // b = 1,2,3,4,5,6,7,8 = TimerName[0-7] = Timer type Repeat T Repeat t Once T Once t Repeat C Once C RcT OcT (Clock Countdown Timers) 
  if (KeyBrdByteNum>0) { a = KeyBrdByte[0]; b = a - 48; }  // add 1-8 and a ='1'-'8' b=1-8
  
  //for (n=0; n<8; n++) TimerName[n][0] = 0x00;
   
  if (MST1==0) { strcpy(TimerName[b-1],  MTRname[Option1]); c = 1; } // Needs 4 byte filename
  if (MST1==1) { strcpy(TimerName[b-1],  STRname[Option1]); c = 3; } 
  if (MST1==2) { strcpy(TimerName[b-1],  TTRname[Option1]); c = 4; } 
  if (MST1==3) { strcpy(TimerName[b-1],  KBMacro[Option1]); c = 0; } // Needs 4 byte filename
  
  if (MST1>3)  { status("Select Source Macro for Timer");             return false; } 
  if (b>8||b==0||KeyBrdByteNum==0) { status("Need 1-8 option added"); return false; } 

  switch(b) 
  { case 1: MacroTimerArr1[0] = c; MacroTimerArr1[1] = Option1; break;
    case 2: MacroTimerArr2[0] = c; MacroTimerArr3[1] = Option1; break;
    case 3: MacroTimerArr3[0] = c; MacroTimerArr3[1] = Option1; break;
    case 4: MacroTimerArr4[0] = c; MacroTimerArr4[1] = Option1; break;
    case 5: MacroTimerArr5[0] = c; MacroTimerArr5[1] = Option1; break;
    case 6: MacroTimerArr6[0] = c; MacroTimerArr6[1] = Option1; break;
    case 7: MacroTimerArr7[0] = c; MacroTimerArr7[1] = Option1; break;
    case 8: MacroTimerArr8[0] = c; MacroTimerArr8[1] = Option1; break; }

  TimerDisp[6]  = XLat1[c]; TimerDisp[17] = a; TimerDisp[8] = XLat2[Option1][0]; TimerDisp[9] = XLat2[Option1][1];
  TimerDisp[19] = XLat3[b-1][0]; TimerDisp[20] = XLat3[b-1][1]; TimerDisp[21] = XLat3[b-1][2];
  status((char *)TimerDisp);
  
  return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ListMacro()    // Source https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Cannot list macros here without a file - for example those loaded using *fm,s,t*
// Because DoFileBytes(0, NameStr) will not find file NameStr
// Works on macros loaded using serial port - they are saved to a file such as <3This is Key S3>
/////////////////////////////////////////////////////////////////////////////////////////////////////
{ String NameStr;
  int i, value;
  byte *BPtr;
  byte a, b;
  int m, n;  
  for (n=0; n<48; n++) Int2HexStr[n] = 0x20; // fill with spaces not NULL
      
  if (MST1==3) { NameStr = KBMacro[Option1]; }   // A - Use files a01 - a99                         
  if (MST1==0) { NameStr = MTRname[Option1]; }   // M - Use files m01 - m24 -> a01 - a24
  if (MST1==1) { NameStr = STRname[Option1]; }   // S - Use files s01 - s24 -> a25 - a48
  if (MST1==2) { NameStr = TTRname[Option1]; }   // T - Use files t01 - t24 -> a49 - a72

  BPtr = MacroBuff; MacroBuffSize = DoFileBytes(0, NameStr, BPtr, ByteSize); 
  if (MacroBuffSize==0) { status("Macro's not found"); return; }         // See notes above must have existing file
          
  n = 0; m=0; 
  if (ScrSizeType==1) a = 10; else a = 5;                               // 480x240 or 320x240
  while (n<a) { value = MacroBuff[n];                                   // Contents in hex
                Int2HexStr[n+m]   = digits[(value >> 4) & 0x0F];   
                Int2HexStr[n+m+1] = digits[value & 0x0F]; n++; m=m+3; }  

  m = m + 3; 
  value = MacroBuffSize;                                               // Size in hex
  Int2HexStr[n+m]   = digits[(value >> 4) & 0x0F];   
  Int2HexStr[n+m+1] = digits[value & 0x0F]; n++; m=m+3; 

   b = 0x20; i = 0;
   while (b!=0) { b = NameStr[i]; Int2HexStr[n+m+i] = b; i++; }
                
  status((char *)Int2HexStr); // Should print up to first NULL
  
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
                     if (Button==6)  { for (n=0; n<4; n++) { b = StarCode[StarNum][n]; KeyBrdByte[n] = b; KBDisp[n] = b; } 
                                       KeyBrdByteNum = 4; KBDispPos = 4; status((char *)KBDisp); delay(10);
                                       StarNum++; if (StarNum==StarCodesMax) StarNum = 0; return; } }
                                       // For some *codes such as *os* should return to same *code if [EXE] was pressed
    if (KeyBrdX==3) { switch(Button)
                    {        case 0:  ListMacro();                                  break;  
                             case 1:  RenameMacro(); InitCfg(0);                    break; 
                             case 2:  RemoveMacro(); InitCfg(0);                    break;  
                             case 3:  if (SendMacro()) status("Macro Sent");        break;   
                             case 4:  MacroTimerOK = GetTimerMacro();               break; 
                             case 5:  CopyMacro(); InitCfg(0);                      break; 
                             case 6:  LinkMacro();                                  break;  
                             case 8:  DoSourceMacro(); status("Source Macro");      break; 
                             case 9:  DoDestMacro();   status("Destination Macro"); break; 
                             case 10: DoNumMacro();                                 break; 
                             case 11: if (SaveMacro(1)) status("Macro Saved");      break;                                
                    } return; }
                     
    if (KeyBrdX==4) {KeyBrdByte[KeyBrdByteNum] = KeyBrdNum[KeyBrd123][Button];   }   // Get special code such as ShiftLeft etc
        // for (n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } Serial.println(); }
           
    // n = 0;
    if (KeyBrdX<4) 
       {KBDisp[KBDispPos] = b; KBDisp[KBDispPos+1] = ' '; KBDisp[KBDispPos+2] = ' '; 
        if (a>0) KeyBrdByte[KeyBrdByteNum] = a; 
        if (Fx) { for (n=0; n<3; n++) {KBDisp[KBDispPos+n] = KeyBrdFxChr[KeyBrdF1F24][n]; }  KeyBrdF1F24++; if (KeyBrdF1F24>23) KeyBrdF1F24=0; } } // Fx true 1 MakeStr()
        
    if (KeyBrdX==4) for (n=0; n<3; n++) KBDisp[KBDispPos+n] = KeyBrdSpc[KeyBrd123][Button][n];
    
    // for (i = 0; i <= KBDispPos+n; i++) {Serial.print(KBDisp[i]);  Serial.print(' '); } Serial.println();
    // for (int n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } Serial.println();    

    status((char *)KBDisp);

    if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; // chars per key 0 1 2 repeat
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
bool SaveMacro(bool Overwrite)    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Saves current Source Macro to flash - if already a file overwrite if overwrite option = ON (default)
// When Key receives data via *fm,s,t* then MacroSizeM1M12[1-12] size = and MacroM1M12[1-12] Status =
// When Key receives data from KeyBrd Macro Editor then Size = number of bytes and Status = 1
// for both String type (1st byte <0x80 and Modifier type (1st byte > 0x7FF)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ int m, n;        
  byte * BPtr;
  
  n = 0;
  if (MST1==0) { if (LittleFS.exists(MTRname[Option1])&&!Overwrite) { status("[M] Macro already saved/exists"); return false; }
                 for (n=0; n<ByteSize; n++) { if (Mtr1to12[Option1][n]==0x00) break;  } MacroM1M12[Option1] = 1; BPtr = Mtr1to12[Option1];  
                      MacroSizeM1M12[Option1] = n; DoFileBytes(1, MTRname[Option1], BPtr, n); }  // Save Destination File

  if (MST1==1)  { if (LittleFS.exists(STRname[Option1])&&!Overwrite) { status("[S] Macro already saved/exists"); return false; }
                  for (n=0; n<ByteSize; n++) { if (Str1to12[Option1][n]==0x00) break; } MacroS1S12[Option1] = 1; BPtr = Str1to12[Option1];  
                      MacroSizeS1S12[Option1] = n; DoFileBytes(1, STRname[Option1], BPtr, n); }  // Save Destination File 

  if (MST1==2) { if (LittleFS.exists(TTRname[Option1])&&!Overwrite) { status("[T] Macro already saved/exists"); return false; }
                 for (n=0; n<ByteSize; n++) { if (Ttr1to12[Option1][n]==0x00) break;  } MacroT1T12[Option1] = 1; BPtr = Ttr1to12[Option1];  
                      MacroSizeT1T12[Option1] = n; DoFileBytes(1, TTRname[Option1], BPtr, n); }  // Save Destination File 
 
  if (n>1) return true; else return false;        
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
    for (n=0; n<4; n++) { usb_hid.keyboardPress(HIDKbrd, MathHexNum[n]); delay(keydelay2);   // MathNum = 4 hex chars + 0x00 -> use i<4 not i<5
                          usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2);}
                          
    for (n=0; n<6; n++) keycode[n] = 0x00;                          
    keycode[0] = AltL;
    keycode[1] = 0x1B; // char X from hid.h       
    usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(100);
    usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
}

//////////////////////////////////////////////////////////////////////////
// Sourcerc nn Target nn   D  Timers RrTt CcTt
//////////////////////////////////////////////////////////////////////////
void optionsindicators(int Option) {
  int ColArr[5] = {LGrey3, SkyBlue1, Pink, Green2, Red};
  char MacroDispStr[25] = {"Source M nn Target M nn"};
  char MSTArr[5][2] = {"M","S","T","A","X"};   
  int b;  
  
  tft.setTextPadding(220);                              
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(Yellow, Black); 
  tft.setTextDatum(1);
    
  if (Kbrd) 
     { tft.setTextColor(ColArr[MST1], Black);
       MacroDispStr[7]  = MSTArr[MST1][0]; MacroDispStr[19] = MSTArr[MST2][0];
       MacroDispStr[9]  = (Option1+1)/10 + 48;  MacroDispStr[10] = (Option1+1)%10 + 48;
       MacroDispStr[21] = (Option2+1)/10 + 48;  MacroDispStr[22] = (Option2+1)%10 + 48;  
       tft.drawString(MacroDispStr, MST_X1, MST_Y); }
  else tft.drawString(" ", MST_X1, MST_Y);
  
  MacroTimer18 = MacroTimer8 || MacroTimer7 || MacroTimer6 || MacroTimer5 || Macrotimer4 || MacroTimer3 || Macrotimer2 || MacroTimer1;   
  if (MacroTimer18 && MacroTimerOK) tft.drawString(TimerDisp, MST_X3, MST_Y); 
                               else tft.drawString("  ", MST_X3, MST_Y);

  tft.setTextPadding(14); 
  if (KeyBrdDirect&&Kbrd) tft.drawString("D", OPT_X1, OPT_Y); else tft.drawString(" ",  OPT_X1, OPT_Y);
  
  if (!LayerAxD) LayerADLetter[0] = 65 + LayerAD; else LayerADLetter[0] = 'X';
  
  tft.drawString(LayerADLetter, OPT_X3, OPT_Y); 

  tft.drawString(OSChr[OptionOS[0]], OPT_X2, OPT_Y); 

  if (!KeyBrdDirect) 
      {if (!CheckSerial) tft.setTextColor(Orange, Black); else tft.setTextColor(Green, Black);
       tft.drawString("S", OPT_X1, OPT_Y); }
  
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);   
}
 
//////////////////////////////////////////////////////////////////////////////////
// Indicators status Caps-Num-Scroll C N S 
//////////////////////////////////////////////////////////////////////////////////
void indicators() 
{ if (ScrSizeType==1) { tft.setTextPadding(18); tft.setFreeFont(&FreeSansBold12pt7b); }
  if (ScrSizeType==0) { tft.setTextPadding(14); tft.setFreeFont(&FreeSansBold9pt7b);  }
  
  tft.setTextColor(Green, Black);
 
  tft.setTextDatum(1);
  if (CapsLock)    tft.drawString("C", IND_X1, IND_Y);  else tft.drawString(" ", IND_X1, IND_Y);
  if (NumLock)     tft.drawString("N", IND_X2, IND_Y);  else tft.drawString(" ", IND_X2, IND_Y);
  if (ScrollLock)  tft.drawString("S", IND_X3, IND_Y);  else tft.drawString(" ", IND_X3, IND_Y);
  
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);   
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

////////////////////////
void showKeyData() 
////////////////////////
{  byte p, n, m, b;
   char c;
   
   Serial.println();
   for ( m = 0; m<24; m++ )   // char LinkNameM[12][10][4] -> only a01-a99 allowed                                      
         { Serial.print(m); Serial.print(" LinkM " ); Serial.print(LinkM[m]); Serial.print(" ");
          for ( n = 0; n < 10; n++) 
              { for (p=0; p<4; p++) {c = LinkNameM[m][n][p]; Serial.print(c); } Serial.print(" "); }
         Serial.println();  }
         
   Serial.println();
   for ( m = 0; m<24; m++ )   // char LinkNameS[12][10][4] -> only a01-a99 allowed
         { Serial.print(m); Serial.print(" LinkS " ); Serial.print(LinkS[m]); Serial.print(" ");
          for ( n = 0; n < 10; n++) 
              { for (p=0; p<4; p++) {c = LinkNameS[m][n][p]; Serial.print(c); } Serial.print(" "); }
         Serial.println();  }
         
   Serial.println();
   for ( m = 0; m<24; m++ )   // char LinkNameT[12][10][4] -> only a01-a99 allowed
         { Serial.print(m); Serial.print(" LinkT " ); Serial.print(LinkT[m]); Serial.print(" ");
          for ( n = 0; n < 10; n++) 
              { for (p=0; p<4; p++) {c = LinkNameT[m][n][p]; Serial.print(c); } Serial.print(" "); }
         Serial.println();  }
         
   Serial.println();
   Serial.print(" Buff 20bytes " ); Serial.print(MacroBuffSize); Serial.print(" ");
          for ( n = 0; n <= 20; n++) 
              { b =  MacroBuff[n]; Serial.print(b, HEX); Serial.print(" "); }
          Serial.println(); 

    Serial.println();
    for ( m = 0; m<24; m++ ) 
         { Serial.print(m); Serial.print(" Mxx 20bytes " ); Serial.print(MacroSizeM1M12[m]); Serial.print(" Mtr Status " ); Serial.print(MacroM1M12[m]); Serial.print(" ");
          for ( n = 0; n < 20; n++) 
              { b =  Mtr1to12[m][n]; Serial.print(b, HEX); Serial.print(" "); }
          Serial.println(); }

   Serial.println();
   for ( m = 0; m<24; m++ ) 
         { Serial.print(m); Serial.print(" Sxx 20bytes Size " ); Serial.print(MacroSizeS1S12[m]); Serial.print(" Str Status " ); Serial.print(MacroS1S12[m]); Serial.print(" ");
          for ( n = 0; n < 20; n++) 
              { b =  Str1to12[m][n]; Serial.print(b, HEX); Serial.print(" "); }
          Serial.println(); }

   Serial.println();
   for ( m = 0; m<24; m++ ) 
         { Serial.print(m); Serial.print(" Txx 20bytes Size " ); Serial.print(MacroSizeT1T12[m]); Serial.print(" Ttr Status "); Serial.print(MacroT1T12[m]); Serial.print(" ");
          for ( n = 0; n < 20; n++) 
              { b =  Ttr1to12[m][n]; Serial.print(b, HEX); Serial.print(" "); }
         Serial.println();  }        
         
 }

// The END
